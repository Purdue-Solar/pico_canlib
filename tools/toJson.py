import argparse
import csv
import json
import sys
from datetime import datetime, timezone
from pathlib import Path


'''
To use this script, run it from the command line with the input CSV file and optional output JSON file:

    python {relative path of this script} {relative path of input CSV} {relative path of output JSON}
    The JSON object will be written to the output file, or to a file with the same name as the input CSV but with a .json extension if no output file is specified.
    It can be found in the same directory as the input CSV file.
'''

CAN_COLUMNS = {"message_name", "message_id", "period_ms", "signal_name", "start_bit", "length", "endian"}


def to_int(value):
    return int(value.strip(), 0)


def cell(row, column):
    return (row.get(column) or "").strip()


def optional(row, **fields):
    return {key: cell(row, col) for key, col in fields.items() if cell(row, col)}


def parse_value(text):
    text = (text or "").strip()
    for cast in (lambda s: int(s, 0), float):
        try:
            return cast(text)
        except ValueError:
            pass
    return text or None


def convert_can(rows):
    messages = {}
    for line, row in enumerate(rows, start=2):
        try:
            name, msg_id = cell(row, "message_name"), to_int(row["message_id"])
            msg = messages.setdefault((name, msg_id), {
                "name": name,
                "id": f"0x{msg_id:X}",
                "period_ms": to_int(row["period_ms"]),
                "signals": {},
                **optional(row, enum="message_enum"),
            })
            sig_name = cell(row, "signal_name")
            sig = msg["signals"].setdefault(sig_name, {
                "name": sig_name,
                "start_bit": to_int(row["start_bit"]),
                "length": to_int(row["length"]),
                "endian": cell(row, "endian").lower(),
                "bits": [],
                **optional(row, enum="signal_enum"),
            })
            if cell(row, "bit_name"):
                sig["bits"].append({
                    "name": cell(row, "bit_name"),
                    "index": to_int(row["bit_index"]),
                    **optional(row, enum="bit_enum"),
                })
        except (ValueError, KeyError, AttributeError) as error:
            raise ValueError(f"row {line}: {error!r}") from error

    result = list(messages.values())
    for msg in result:
        msg["signals"] = list(msg["signals"].values())
    return {"messages": result}


def convert_generic(rows):
    return {"data": [{k: parse_value(v) for k, v in row.items() if k is not None} for row in rows]}


def convert(input_path, output_path):
    with input_path.open(newline="", encoding="utf-8-sig") as f:
        reader = csv.DictReader(f)
        rows = list(reader)
        is_can = CAN_COLUMNS.issubset(reader.fieldnames or [])

    document = {
        "schema_version": 1,
        "generated_at": datetime.now(timezone.utc).isoformat(timespec="seconds").replace("+00:00", "Z"),
        "source": {"format": "csv", "file": input_path.name},
        **(convert_can(rows) if is_can else convert_generic(rows)),
    }
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(json.dumps(document, indent=2) + "\n", encoding="utf-8")


def main():
    parser = argparse.ArgumentParser(description="Convert a CSV file to JSON.")
    parser.add_argument("input_csv", type=Path)
    parser.add_argument("output_json", type=Path, nargs="?", help="default: same name as input, .json")
    args = parser.parse_args()

    output = args.output_json or args.input_csv.with_suffix(".json")
    try:
        convert(args.input_csv, output)
    except (OSError, ValueError) as error:
        sys.exit(f"error: {error}")
    print(f"Converted {args.input_csv} -> {output}")

if __name__ == "__main__":
    main()