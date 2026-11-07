Import("env")

platform = env.PioPlatform().name
board = env.BoardConfig().id

if platform == "espressif32":
    target = "esp32"
elif platform in ("raspberrypi", "raspberrypi-custom"):
    target = "rp2350"
elif platform == "teensy":
    target = "teensy"
elif board == "rpipico2":
    target = "rp2350"
else:
    raise RuntimeError(f"Unsupported CAN platform: {platform}, board: {board}")

env.Replace(SRC_FILTER=[
    "+<canlib.cpp>",
    f"+<{target}/*.cpp>"
])

print(f"CANLib: Building {target} implementation")