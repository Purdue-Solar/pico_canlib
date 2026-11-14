#!/usr/bin/env python3
"""
toHeader.py - Generate a compile-time C++ CAN ID header from a Vector DBC database.

Translates a Vector DBC file (.dbc) into type-safe C++ compile-time structures:
    - Structs: BitDefinition, SignalDefinition, MessageDefinition
    - Scoped enums for signals and master MessageID
    - Constexpr master message definition table
    - Type-safe compile-time MessageOf<SignalEnum> associations
    - Accessors: getID, getMessageDefinition, getSignal, getBitPosition,
      dataEndian, dataEndianFlip, getNumCanBytesMessage, sbit

Usage:
    python tools/toHeader.py <input.dbc> [output.hpp] [--array-name <name>]

Example:
    python tools/toHeader.py tests/PSR_Artemis27.dbc Inc/artemis_canid.hpp
"""

import argparse
import re
import sys
from datetime import datetime, timezone
from pathlib import Path

try:
    import cantools
except ImportError:
    sys.exit("error: 'cantools' is required. Install it using: pip install cantools")


# ─────────────────────────────────────────────────────────────────────────────
# String Normalization Helpers
# ─────────────────────────────────────────────────────────────────────────────

def to_pascal_case(text):
    """Convert arbitrary text or snake_case string to valid C++ PascalCase identifier."""
    cleaned = re.sub(r"[^a-zA-Z0-9]+", " ", str(text)).strip()
    words = cleaned.split()
    if not words:
        return "Unknown"
    # If a single word is already in PascalCase/camelCase (e.g. 'BmsSafetyCritical'), preserve it
    if len(words) == 1 and any(c.isupper() for c in words[0][1:]):
        pascal = words[0][0].upper() + words[0][1:]
    else:
        # For multiple words or all-caps acronyms, capitalize properly without crushing inner casing
        pascal = "".join(w[0].upper() + w[1:] if not w.isupper() else w.capitalize() for w in words)
    if pascal[0].isdigit():
        pascal = f"Msg{pascal}"
    return pascal


def to_snake_case(text):
    """Convert PascalCase or mixed string to valid C++ snake_case identifier."""
    cleaned = re.sub(r"[^a-zA-Z0-9]+", "_", str(text)).strip("_")
    snake = re.sub(r"([a-z0-9])([A-Z])", r"\1_\2", cleaned).lower()
    snake = re.sub(r"_+", "_", snake)
    if not snake:
        return "unknown"
    if snake[0].isdigit():
        snake = f"sig_{snake}"
    return snake


def get_cycle_time(msg):
    """Extract transmission cycle time in ms from message attributes."""
    if hasattr(msg, "cycle_time") and msg.cycle_time is not None:
        return int(msg.cycle_time)
    if hasattr(msg, "attributes") and msg.attributes and "GenMsgCycleTime" in msg.attributes:
        return int(msg.attributes.get("GenMsgCycleTime", 0) or 0)
    return 0


# ─────────────────────────────────────────────────────────────────────────────
# Header Generator
# ─────────────────────────────────────────────────────────────────────────────

def generate_header_from_db(db, master_array_name="artemis_messages", source_file=None):
    """Generate C++ header string directly from a cantools CAN Database."""
    if not db or not db.messages:
        raise ValueError("The CAN database contains no messages.")

    messages = sorted(db.messages, key=lambda m: m.frame_id)
    timestamp = datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M:%S UTC")
    source_desc = source_file or "Vector DBC"

    lines = []

    # 1. Header Documentation
    lines.append("/* ==============================================================================")
    lines.append(" * CAN ID and Message Definitions (artemis_canid.hpp)")
    lines.append(" *")
    lines.append(" * GENERATION METADATA:")
    lines.append(f" *   Generated on: {timestamp}")
    lines.append(" *   Generator:    tools/toHeader.py")
    lines.append(f" *   Source file:  {source_desc}")
    lines.append(" *")
    lines.append(" * OVERVIEW:")
    lines.append(" *   This file contains compile-time definitions of all CAN message IDs,")
    lines.append(" *   signal layouts, bit-fields, and accessors for the vehicle CAN bus.")
    lines.append(" *   All structures and accessors in this file are evaluated at compile time.")
    lines.append(" *")
    lines.append(" * HOW IT WORKS:")
    lines.append(" *   1. Retrieve CAN ID for a message:")
    lines.append(" *      uint32_t can_id = getID(MessageID::MotorBusVoltageCurrent);")
    lines.append(" *")
    lines.append(" *   2. Retrieve full MessageDefinition:")
    lines.append(" *      const MessageDefinition& def = getMessageDefinition(MessageID::BmsSafetyCritical);")
    lines.append(" *")
    lines.append(" *   3. Retrieve SignalDefinition (Type-Safe):")
    lines.append(" *      const SignalDefinition& sig = getSignal(BmsSafetyCriticalSignal::PackVoltage);")
    lines.append(" *")
    lines.append(" *   4. Endianness normalization on RX buffer:")
    lines.append(" *      dataEndian(buffer, MessageID::BmsSafetyCritical);")
    lines.append(" *")
    lines.append(" * DO NOT EDIT DIRECTLY — Regenerate using tools/toDbc.py")
    lines.append(" * ============================================================================== */")
    lines.append("")
    lines.append("#pragma once")
    lines.append("#if defined(STM32_PROCESSOR)")
    lines.append("#include <cstdint>")
    lines.append("#include <cstddef>")
    lines.append('#elif defined(__has_include) && __has_include("pico/stdlib.h")')
    lines.append('#include "pico/stdlib.h"')
    lines.append("#else")
    lines.append("#include <cstdint>")
    lines.append("#include <cstddef>")
    lines.append("#endif")
    lines.append("#include <algorithm>")
    lines.append("#include <cmath>")
    lines.append("")
    lines.append("template<typename T, size_t N>")
    lines.append("constexpr size_t array_size(const T (&)[N]) { return N; }")
    lines.append("")
    lines.append("enum class Endianness")
    lines.append("{")
    lines.append("    Little,")
    lines.append("    Big")
    lines.append("};")
    lines.append("")
    lines.append("struct BitDefinition")
    lines.append("{")
    lines.append("    const char* name;")
    lines.append("    uint8_t bitIndex;")
    lines.append("};")
    lines.append("")
    lines.append("struct SignalDefinition")
    lines.append("{")
    lines.append("    const char* name;")
    lines.append("    uint8_t startBit;")
    lines.append("    uint8_t length;")
    lines.append("    Endianness endian;")
    lines.append("")
    lines.append("    const BitDefinition* bitDefinitions;")
    lines.append("    size_t bitDefinitionCount;")
    lines.append("};")
    lines.append("")
    lines.append("struct MessageDefinition")
    lines.append("{")
    lines.append("    const char* name;")
    lines.append("    uint32_t id;")
    lines.append("    uint16_t periodMs;")
    lines.append("")
    lines.append("    const SignalDefinition* signals;")
    lines.append("    size_t signalCount;")
    lines.append("};")
    lines.append("")

    message_records = []

    # 2. Per-Message Signal Arrays and Bit Definitions
    for msg in messages:
        msg_name = msg.name
        msg_id_str = f"0x{msg.frame_id:X}"
        period_ms = get_cycle_time(msg)
        msg_enum = to_pascal_case(msg_name)
        msg_prefix = to_snake_case(msg_enum)

        lines.append(f"// ─── {msg_id_str}  {msg_name} " + "─" * max(10, 75 - len(msg_name) - len(msg_id_str)))
        lines.append("")

        sig_records = []

        for sig in msg.signals:
            sig_name = sig.name
            sig_enum = to_pascal_case(sig_name)
            sig_prefix = to_snake_case(sig_enum)
            endian_cpp = "Endianness::Big" if sig.byte_order == "big_endian" else "Endianness::Little"

            bit_array_expr = "nullptr"
            bit_count_expr = "0"

            # Check if signal has bit definitions (choices / VAL_ in DBC fitting in uint8_t)
            if hasattr(sig, "choices") and sig.choices and all(isinstance(k, int) and 0 <= k <= 255 for k in sig.choices.keys()):
                bit_enum_name = f"{msg_enum}{sig_enum}Bit"
                bit_array_name = f"{msg_prefix}_{sig_prefix}_bits"

                lines.append(f"enum class {bit_enum_name} : uint8_t")
                lines.append("{")
                for val, desc in sorted(sig.choices.items(), key=lambda x: x[0]):
                    enum_member = to_pascal_case(desc)
                    lines.append(f"    {enum_member} = {val},")
                lines.append("};")
                lines.append("")

                lines.append(f"inline constexpr BitDefinition {bit_array_name}[] = {{")
                for val, desc in sorted(sig.choices.items(), key=lambda x: x[0]):
                    enum_member = to_pascal_case(desc)
                    safe_desc = str(desc).replace('"', '\\"')
                    lines.append(f'    {{"{safe_desc}", static_cast<uint8_t>({bit_enum_name}::{enum_member})}},')
                lines.append("};")
                lines.append("")

                bit_array_expr = bit_array_name
                bit_count_expr = f"array_size({bit_array_name})"

            sig_records.append({
                "name": sig_name,
                "enum": sig_enum,
                "start_bit": sig.start,
                "length": sig.length,
                "endian": endian_cpp,
                "bit_array": bit_array_expr,
                "bit_count": bit_count_expr,
            })

        sig_array_name = f"{msg_prefix}_signals"
        if sig_records:
            lines.append(f"inline constexpr SignalDefinition {sig_array_name}[] = {{")
            for sr in sig_records:
                display_name = sr["name"].replace('"', '\\"')
                lines.append(
                    f'    {{"{display_name}", {sr["start_bit"]}, {sr["length"]}, {sr["endian"]}, {sr["bit_array"]}, {sr["bit_count"]}}},'
                )
            lines.append("};")
            lines.append("")
        else:
            sig_array_name = "nullptr"

        message_records.append({
            "name": msg_name,
            "id_str": msg_id_str,
            "period_ms": period_ms,
            "enum": msg_enum,
            "sig_array": sig_array_name,
            "has_signals": bool(sig_records),
            "signals": sig_records,
        })

    # 3. MessageID Enum
    lines.append("// ─── Message and Signal ID Enums ─────────────────────────────────────────────")
    lines.append("")
    lines.append("enum class MessageID : uint8_t")
    lines.append("{")
    for idx, mr in enumerate(message_records):
        lines.append(f"    {mr['enum']} = {idx},")
    lines.append("};")
    lines.append("")

    # 4. Per-Message Signal Enums
    for mr in message_records:
        if mr["has_signals"]:
            sig_enum_type = f"{mr['enum']}Signal"
            lines.append(f"enum class {sig_enum_type} : uint8_t")
            lines.append("{")
            for idx, sr in enumerate(mr["signals"]):
                lines.append(f"    {sr['enum']} = {idx},")
            lines.append("};")
            lines.append("")

    # 5. Master Message Table
    lines.append("// ─── Master Message Table ─────────────────────────────────────────────────────")
    lines.append("// Keep this table's row order in sync with the MessageID enum above.")
    lines.append("")
    lines.append(f"inline constexpr MessageDefinition {master_array_name}[] = {{")
    for mr in message_records:
        msg_display_name = mr["name"].replace('"', '\\"')
        if mr["has_signals"]:
            count_expr = f"array_size({mr['sig_array']})"
            sig_ptr_expr = mr["sig_array"]
        else:
            count_expr = "0"
            sig_ptr_expr = "nullptr"
        lines.append(
            f'    {{"{msg_display_name}", {mr["id_str"]}, {mr["period_ms"]}, {sig_ptr_expr}, {count_expr}}},'
        )
    lines.append("};")
    lines.append("")

    # 6. Message-Signal Associations (Compile-time MessageOf traits)
    lines.append("// ─── Message-Signal Associations ─────────────────────────────────────────────")
    lines.append("// Maps each signal enum to its owning message, enforcing correct pairing in getSignal().")
    lines.append("")
    lines.append("template<typename SignalEnum> struct MessageOf; // intentionally undefined")
    lines.append("")
    for mr in message_records:
        if mr["has_signals"]:
            sig_enum_type = f"{mr['enum']}Signal"
            lines.append(
                f"template<> struct MessageOf<{sig_enum_type}> {{ static constexpr MessageID value = MessageID::{mr['enum']}; }};"
            )
    lines.append("")

    # 7. Compile-time Accessors
    lines.append("// ─── Accessors ────────────────────────────────────────────────────────────────")
    lines.append("")
    lines.append("// Returns the bit position (0–7) of a bit-field enum value within its byte.")
    lines.append("template<typename BitEnum>")
    lines.append("constexpr uint8_t getBitPosition(BitEnum bit)")
    lines.append("{")
    lines.append("    return static_cast<uint8_t>(bit);")
    lines.append("}")
    lines.append("")
    lines.append("// Returns the MessageDefinition for a given MessageID.")
    lines.append("constexpr const MessageDefinition& getMessageDefinition(MessageID id)")
    lines.append("{")
    lines.append(f"    return {master_array_name}[static_cast<uint8_t>(id)];")
    lines.append("}")
    lines.append("")
    lines.append("// Returns the can ID for a given MessageID.")
    lines.append("constexpr uint32_t getID(MessageID id)")
    lines.append("{")
    lines.append(f"    return {master_array_name}[static_cast<uint8_t>(id)].id;")
    lines.append("}")
    lines.append("")
    lines.append("// Returns the SignalDefinition for a signal enum value.")
    lines.append("template<typename SignalEnum>")
    lines.append("constexpr const SignalDefinition& getSignal(SignalEnum sig)")
    lines.append("{")
    lines.append(f"    return {master_array_name}[static_cast<uint8_t>(MessageOf<SignalEnum>::value)]")
    lines.append("                            .signals[static_cast<uint8_t>(sig)];")
    lines.append("}")
    lines.append("")
    lines.append("// In-place reverse bytes for Big endian signals.")
    lines.append("template<typename Byte>")
    lines.append("void dataEndian(Byte * buffer, MessageID id)")
    lines.append("{")
    lines.append("    MessageDefinition definition = getMessageDefinition(id);")
    lines.append("    for(size_t i = 0; i < definition.signalCount; i++)")
    lines.append("    {")
    lines.append("        SignalDefinition signal = definition.signals[i];")
    lines.append("        if (signal.endian == Endianness::Big)")
    lines.append("        {")
    lines.append("            auto elem_start = signal.startBit / (8u * sizeof(buffer[0]));")
    lines.append("            auto elem_end   = (signal.startBit + signal.length) / (8u * sizeof(buffer[0]));")
    lines.append("            std::reverse(buffer + elem_start, buffer + elem_end);")
    lines.append("        }")
    lines.append("    }")
    lines.append("}")
    lines.append("")
    lines.append("template<typename Byte>")
    lines.append("void dataEndianFlip(Byte * buffer, MessageID id)")
    lines.append("{")
    lines.append("    MessageDefinition definition = getMessageDefinition(id);")
    lines.append("    for(size_t i = 0; i < definition.signalCount; i++)")
    lines.append("    {")
    lines.append("        SignalDefinition signal = definition.signals[i];")
    lines.append("        auto elem_start = signal.startBit / (8u * sizeof(buffer[0]));")
    lines.append("        auto elem_end   = (signal.startBit + signal.length) / (8u * sizeof(buffer[0]));")
    lines.append("        std::reverse(buffer + elem_start, buffer + elem_end);")
    lines.append("    }")
    lines.append("}")
    lines.append("")
    lines.append("constexpr uint8_t getNumCanBytesMessage(MessageID id)")
    lines.append("{")
    lines.append("    MessageDefinition message = getMessageDefinition(id);")
    lines.append("    if (message.signalCount == 0) return 0;")
    lines.append("    SignalDefinition lastSignal = message.signals[message.signalCount - 1];")
    lines.append("    return static_cast<uint8_t>((lastSignal.startBit + lastSignal.length + 7) / 8);")
    lines.append("}")
    lines.append("")
    lines.append("template<typename Shift>")
    lines.append("constexpr uint8_t sbit(Shift s)")
    lines.append("{")
    lines.append("    return 1u << static_cast<uint8_t>(s);")
    lines.append("}")
    lines.append("")

    return "\n".join(lines)


# ─────────────────────────────────────────────────────────────────────────────
# CLI Entry Point
# ─────────────────────────────────────────────────────────────────────────────

def main():
    parser = argparse.ArgumentParser(description="Generate a C++ CAN ID header directly from a Vector DBC file.")
    parser.add_argument("input_file", type=Path, help="Input Vector DBC file (.dbc)")
    parser.add_argument("output_hpp", type=Path, nargs="?", help="Output C++ header file (default: Inc/artemis_canid.hpp)")
    parser.add_argument("--array-name", default="artemis_messages", help="Name of master MessageDefinition array")
    args = parser.parse_args()

    if not args.input_file.is_file():
        sys.exit(f"error: input file '{args.input_file}' not found.")

    output = args.output_hpp or Path("Inc/artemis_canid.hpp")

    try:
        # Load DBC with strict=False to support multiplexed and diagnostic CAN frames
        db = cantools.database.load_file(str(args.input_file), strict=False)
        content = generate_header_from_db(
            db,
            master_array_name=args.array_name,
            source_file=args.input_file.name,
        )
    except Exception as err:
        sys.exit(f"error generating header: {err}")

    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(content, encoding="utf-8")
    print(f"Generated header: {args.input_file} -> {output}")


if __name__ == "__main__":
    main()
