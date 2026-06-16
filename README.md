# CHIP-8 Emulator

A CHIP-8 emulator written in C with SDL2. Built as a learning project to get hands-on with emulation and low-level C.

---

## What is CHIP-8?

CHIP-8 is an interpreted programming language from the 1970s, originally designed to make game development easier on early microcomputers. It's not a real hardware system, but a virtual machine that runs on top of whatever hardware you have.

It's become the "hello world" of emulator development because it's simple to build but teaches you everything you need to know before tackling something like the NES or Game Boy.

---

## Specifications

- **Memory:** 4 KB RAM
  - `0x000–0x1FF` — reserved (interpreter, font data lives here)
  - `0x200` — ROM loads here; PC starts here
  - `0xFFF` — top of RAM
- **Display:** 64×32 pixels, monochrome
- **Font:** 16 characters (0–F), each 5 bytes tall, stored at `0x050–0x09F`
- **PC:** points to the current instruction in memory
- **Index register I:** 16-bit, points at locations in memory
- **Stack:** holds 16-bit addresses for subroutine calls/returns
- **Delay timer:** 8-bit, decremented at 60 Hz
- **Sound timer:** 8-bit, same as delay timer; emits a beep while non-zero
- **Registers:** 16 × 8-bit general-purpose (V0–VF); VF doubles as a flag register

### Currently Implemented Opcodes

| Opcode | Description |
|--------|-------------|
| `00E0` | Clears screen |
| `00EE` | Returns from subroutine |
| `1NNN` | Jumps to address NNN |
| `2NNN` | Calls subroutine at NNN |
| `3XNN` | Skips if reg VX == NN |
| `4XNN` | Skips if reg VX != NN |
| `5XY0` | skips if reg VX == VY |
| `6XNN` | Set reg VX = NN |
| `7XNN` | Adds NN to register VX |
| `8XYN` **(in progress)** | ALU - Arithmetic and Bitwise operations |
| `9XY0` | Skips if VX != VY |
| `ANNN` | Sets index register I = NNN |
| `DXYN` | Draws sprite at (VX, VY), N bytes tall |

---

![CHIP-8 running IBM logo](IBM_LOGO.png)

**In progress / TODO**
- Remaining opcodes
- Keyboard input
- Scalable / resiazable window
- Timers (delay + sound)
- Windows build
- Build script

---

## Dependencies

**Linux / WSL (Ubuntu/Debian):**

```sh
sudo apt update
sudo apt install build-essential cmake libsdl2-dev
```

---

## Building

```sh
mkdir build
cd build
cmake ..
cmake --build .
./CHIP-8
```

The binary looks for ROMs at `../ROMs` relative to where it runs, so run it from inside `build/`.

---

## References

- https://tobiasvl.github.io/blog/write-a-chip-8-emulator/
- http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#0.1
