Import("env")

targets = {
    "CAN_TARGET_ESP32": "esp32",
    "CAN_TARGET_RP2350": "rp2350",
    "CAN_TARGET_TEENSY": "teensy"
}

defines = set()

for item in env.get("CPPDEFINES", []):
    if isinstance(item, tuple):
        defines.add(item[0])
    elif isinstance(item, str):
        defines.add(item)

selected = [folder for macro, folder in targets.items() if macro in defines]

if len(selected) != 1:
    raise RuntimeError("Select exactly one CAN_TARGET_* build flag")

target = selected[0]

env.Replace(SRC_FILTER=[
    "+<canlib.cpp>",
    f"+<{target}/*.cpp>"
])

print(f"CANLib: Building {target} implementation")