#!/usr/bin/env python3
"""
syncDbc.py - Synchronize Vector DBC database with Inc/artemis_canid.hpp specifications.

Reads the vehicle CAN definitions from Inc/artemis_canid.hpp (read-only) and aligns
the Vector DBC database so that:
  1. The 18 vehicle CAN messages match the canonical names (e.g. BmsSafetyCritical,
     MotorControllerId, MotorBusVoltageCurrent, etc.).
  2. All signal names, start bits, bit lengths, and endianness match artemis_canid.hpp.
  3. All bitfield enums (DTC flags, relay states, contactor errors) are encoded as
     standard Vector DBC value tables (VAL_ / choices).
  4. Transmission cycle times (GenMsgCycleTime in ms) are preserved and aligned.
  5. All additional diagnostic and calibration messages (WaveSculptor, Orion UDS)
     are preserved.

Usage:
    python tools/syncDbc.py [target.dbc] [--header Inc/artemis_canid.hpp]
"""

import argparse
import re
import sys
from pathlib import Path

try:
    import cantools
    from cantools.database.can import Database, Message, Signal
    from cantools.database.conversion import BaseConversion
except ImportError:
    sys.exit("error: 'cantools' is required. Install using: pip install cantools")

PROJECT_ROOT = Path(__file__).resolve().parent.parent


def parse_header_spec(header_path):
    """Extract message, signal, and bitfield specifications from artemis_canid.hpp (read-only)."""
    text = header_path.read_text(encoding="utf-8")

    # 1. Parse message rows in artemis_messages[]:
    # {"BMS Safety Critical", 0x200, 40, bms_signals, array_size(bms_signals)},
    msg_rows = re.findall(
        r'\{"([^"]+)",\s*(0x[0-9A-Fa-f]+|\d+),\s*(\d+),\s*(\w+|nullptr),\s*([^}]+)\}',
        text
    )
    if not msg_rows:
        raise ValueError("Could not find any MessageDefinition rows in artemis_messages[].")

    # 2. Parse MessageID enum entries in exact order:
    msg_id_match = re.search(r'enum class MessageID : uint8_t\s*\{([^}]+)\};', text)
    if not msg_id_match:
        raise ValueError("Could not find MessageID enum in header.")
    msg_id_order = [x.split("=")[0].strip() for x in msg_id_match.group(1).split(",") if x.strip()]

    # 3. Parse all signal arrays:
    # inline constexpr SignalDefinition bms_signals[] = { ... };
    sig_arrays_raw = re.findall(r'inline constexpr SignalDefinition (\w+)\[\] = \{(.*?)\};', text, re.DOTALL)
    sig_map = {}
    for arr_name, body in sig_arrays_raw:
        sigs = []
        s_rows = re.findall(r'\{"([^"]+)",\s*(\d+),\s*(\d+),\s*Endianness::(\w+),\s*(\w+|nullptr),\s*([^}]+)\}', body)
        for s_name, start, length, endian, bit_ptr, bit_cnt in s_rows:
            sigs.append({
                "name": s_name,
                "start": int(start),
                "length": int(length),
                "byte_order": "big_endian" if endian == "Big" else "little_endian",
                "bit_ptr": bit_ptr,
            })
        sig_map[arr_name] = sigs

    # 4. Parse all bit arrays and their enum definitions:
    bit_arrays_raw = re.findall(r'inline constexpr BitDefinition (\w+)\[\] = \{(.*?)\};', text, re.DOTALL)
    bit_map = {}
    for arr_name, body in bit_arrays_raw:
        bits = {}
        b_rows = re.findall(r'\{"([^"]+)",\s*static_cast<uint8_t>\((\w+)::(\w+)\)\}', body)
        if b_rows:
            enum_type = b_rows[0][1]
            enum_vals = {}
            enum_match = re.search(r'enum class ' + enum_type + r'(?: : \w+)?\s*\{([^}]+)\};', text)
            if enum_match:
                for item in enum_match.group(1).split(","):
                    item = item.strip()
                    if "=" in item:
                        k, v = item.split("=")
                        try:
                            enum_vals[k.strip()] = int(v.strip(), 0)
                        except ValueError:
                            pass
            for b_name, e_type, e_val in b_rows:
                val = enum_vals.get(e_val, len(bits))
                bits[val] = e_val
        bit_map[arr_name] = bits

    messages_spec = []
    for idx, (m_display_name, raw_id, period_ms, sig_arr_name, _) in enumerate(msg_rows):
        frame_id = int(raw_id, 0)
        canonical_name = msg_id_order[idx]
        signals = sig_map.get(sig_arr_name, [])
        messages_spec.append({
            "name": canonical_name,
            "display_name": m_display_name,
            "id": frame_id,
            "period_ms": int(period_ms),
            "signals": signals,
        })

    return messages_spec, bit_map


def sync_database(target_dbc_path, header_path=None):
    """Synchronize DBC file with specifications from C++ header."""
    header_path = Path(header_path) if header_path else (PROJECT_ROOT / "Inc" / "artemis_canid.hpp")
    if not header_path.is_file():
        raise FileNotFoundError(f"Header file not found: {header_path}")

    target_path = Path(target_dbc_path)
    if not target_path.is_file():
        raise FileNotFoundError(f"DBC file not found: {target_path}")

    # Read header specifications
    messages_spec, bit_map = parse_header_spec(header_path)

    # Load existing DBC
    existing_db = cantools.database.load_file(str(target_path), strict=False)

    # Build updated database
    new_db = Database(strict=False)
    new_db._nodes = list(existing_db.nodes)

    vehicle_ids = set()
    vehicle_names = set()

    for m_spec in messages_spec:
        frame_id = m_spec["id"]
        canonical_name = m_spec["name"]
        vehicle_ids.add(frame_id)
        vehicle_names.add(canonical_name)

        signals = []
        for s in m_spec["signals"]:
            s_name = re.sub(r"[^a-zA-Z0-9]+", "_", s["name"]).strip("_")
            choices = bit_map.get(s["bit_ptr"]) if s.get("bit_ptr") else None

            sig = Signal(
                name=s_name,
                start=s["start"],
                length=s["length"],
                byte_order=s["byte_order"],
                is_signed=False,
                conversion=BaseConversion.factory(scale=1.0, offset=0.0, choices=choices),
                comment=s["name"],
            )
            signals.append(sig)

        max_bit = max((s.start + s.length for s in signals), default=64)
        dlc = max((max_bit + 7) // 8, 8)

        msg = Message(
            frame_id=frame_id,
            name=canonical_name,
            length=dlc,
            signals=signals,
            senders=["Vector__XXX"],
            is_extended_frame=(frame_id > 0x7FF),
            comment=m_spec["display_name"],
            strict=False,
        )
        if m_spec["period_ms"] > 0:
            msg.cycle_time = m_spec["period_ms"]
            if not hasattr(msg, "attributes") or msg.attributes is None:
                msg.attributes = {}
            msg.attributes["GenMsgCycleTime"] = m_spec["period_ms"]

        new_db.messages.append(msg)

    # Preserve any additional diagnostic or hardware messages from existing DBC
    extra_preserved = 0
    for m in existing_db.messages:
        if m.frame_id not in vehicle_ids and m.name not in vehicle_names:
            new_db.messages.append(m)
            extra_preserved += 1

    # Save synchronized DBC
    target_path.write_text(new_db.as_dbc_string(), encoding="latin-1")
    return len(messages_spec), extra_preserved


def main():
    parser = argparse.ArgumentParser(description="Synchronize Vector DBC database with artemis_canid.hpp specifications.")
    parser.add_argument("target_dbc", nargs="?", default="tools/PSR Artemis27.dbc", help="Path to Vector DBC file to synchronize.")
    parser.add_argument("--header", default="Inc/artemis_canid.hpp", help="Path to reference C++ header (read-only).")
    args = parser.parse_args()

    target = Path(args.target_dbc)
    header = Path(args.header)

    try:
        v_count, d_count = sync_database(target, header)
        print(f"Successfully synchronized {target}:")
        print(f"  • {v_count} vehicle messages aligned with {header.name}")
        print(f"  • {d_count} additional diagnostic/WaveSculptor messages preserved")
    except Exception as err:
        sys.exit(f"error synchronizing DBC: {err}")


if __name__ == "__main__":
    main()

