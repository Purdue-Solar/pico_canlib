import argparse
import csv
import sys
from collections import defaultdict
from datetime import datetime
from pathlib import Path
from statistics import median

import cantools

"""
toDbc.py - Build a skeleton Vector DBC file from a raw CAN log CSV.

HOW TO USE
----------
Run from the project root (the folder that contains tools/ and tests/):

    python tools/toDbc.py <input_log.csv> [output.dbc] [--decimal-ids]

Examples:

    python tools/toDbc.py tests/test1.csv
    python tools/toDbc.py tests/test1.csv out/bus.dbc
    python tools/toDbc.py tests/test1.csv --decimal-ids


What it produces:
    - One message per unique ID, named MSG_<ID> (e.g. MSG_403)
    - Message length taken from the largest dlc seen for that ID
    - cycle_time from the median gap between that ID's timestamps
    - One placeholder 8-bit unsigned signal per byte (B0..B7), little-endian
      Rename and regroup these by hand once you know what the bytes mean.

Skipped rows: remote frames, and any row whose status is not OK.

Requires: pip install cantools
"""

def parse_time(text):
    return datetime.strptime(text.strip(), "%Y-%m-%d %H:%M:%S.%f").timestamp()


def readLog(input_csv, id_base):
    frames = defaultdict(lambda: {"dlc": 0, "times": []})
    with input_csv.open(newline="", encoding="utf-8-sig") as f:
        reader = csv.DictReader(f)
        required = {"timestamp", "id", "extended", "remote", "dlc"}
        missing = required - set(reader.fieldnames or [])
        if missing:
            raise ValueError(f"log CSV is missing columns: {sorted(missing)}")

        for line, row in enumerate(reader, start=2):
            try:
                if row["remote"].strip() == "1":
                    continue  # remote frames carry no payload
                if (row.get("status") or "OK").strip().upper() != "OK":
                    continue  # skip error frames
                key = (int(row["id"].strip(), id_base), row["extended"].strip() == "1")
                frames[key]["dlc"] = max(frames[key]["dlc"], int(row["dlc"]))
                frames[key]["times"].append(parse_time(row["timestamp"]))
            except (ValueError, KeyError) as error:
                raise ValueError(f"row {line}: {error!r}") from error
    return frames


def toDbc(input_csv, output, id_base=16):
    frames = readLog(input_csv, id_base)
    if not frames:
        raise ValueError("No usable frames found in the log.")

    db = cantools.database.can.Database()

    for (frame_id, extended), info in sorted(frames.items()):
        dlc = info["dlc"]
        times = sorted(info["times"])
        gaps = [b - a for a, b in zip(times, times[1:])]
        cycle_time = round(median(gaps) * 1000) if gaps else None

        signals = [
            cantools.database.can.Signal(
                name=f"B{i}",
                start=i * 8,
                length=8,
                byte_order="little_endian",
                is_signed=False,
            )
            for i in range(dlc)
        ]

        db.messages.append(cantools.database.can.Message(
            frame_id=frame_id,
            name=f"MSG_{frame_id:X}",
            length=dlc,
            signals=signals,
            cycle_time=cycle_time,
            is_extended_frame=extended,
        ))

    db.refresh()

    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(db.as_dbc_string(), encoding="utf-8")
    return len(db.messages)


def main():
    parser = argparse.ArgumentParser(description="Build a skeleton Vector DBC from a CAN log CSV.")
    parser.add_argument("input_csv", type=Path, help="CAN log CSV (timestamp,id,extended,remote,dlc,data,...)")
    parser.add_argument("output_dbc", type=Path, nargs="?", help="default: same name as input, .dbc")
    parser.add_argument("--decimal-ids", action="store_true", help="IDs in the log are decimal (default: hex)")
    args = parser.parse_args()

    output = args.output_dbc or args.input_csv.with_suffix(".dbc")
    try:
        count = toDbc(args.input_csv, output, 10 if args.decimal_ids else 16)
    except (OSError, ValueError) as error:
        sys.exit(f"error: {error}")
    print(f"Wrote {count} messages: {args.input_csv} -> {output}")


if __name__ == "__main__":
    main()