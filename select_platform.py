Import("env")

project_env = DefaultEnvironment()

targets = {
    "CAN_TARGET_ESP32": "esp32",
    "CAN_TARGET_RP2350": "rp2350",
    "CAN_TARGET_TEENSY": "teensy"
}

def get_defines(environment):
    defines = set()

    for item in environment.get("CPPDEFINES", []):
        if isinstance(item, (tuple, list)):
            defines.add(item[0])
        elif isinstance(item, str):
            defines.add(item)

    return defines

defines = get_defines(project_env) | get_defines(env)

selected = [folder for macro, folder in targets.items() if macro in defines]

if len(selected) != 1:
    raise RuntimeError(f"Expected one CAN_TARGET_* macro, found: {selected}. Defines: {sorted(defines)}")

target = selected[0]

env.Replace(SRC_FILTER=[
    "+<canlib.cpp>",
    f"+<{target}/*.cpp>"
])

print(f"CANLib: Building {target} implementation")