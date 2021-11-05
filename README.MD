#	Aznable (custom 8-bit computer system)

## Overview

## Acknowledgements

## Hardware

A simple-ish bespoke (i.e. I made it up with no real plan) 8-bit system with a 40x30 character display (each character foreground and background colour can be set independently to one of 256 colours), and a hardware sprite system (32 sprites, 16 colour palette from 32768).  No sound capabilities whatsoever.  All relevant MiSTer inputs are mapped into memory regions for the CPU to access directly.  There is also a dedicated microsecond timer (runs 0-32677, resettable by writing to any bit of the address).

### Components
 - Z80 CPU (tv80 by Guy Hutchison, based on VHDL T80 core by Daniel Wallner)
 - jtframe_vtimer for VGA timing (from JTFRAME (https://github.com/jotego/jtframe) by Jotego)
 - jtframe_cen24 for clock enables (from JTFRAME (https://github.com/jotego/jtframe) by Jotego)
 - 32Kb program ROM (port 1 cpu, port 2 download)
 - 16Kb work RAM (port 1 cpu)
 - 2Kb character ROM (port 1 cpu, port 2 download)
 - 2Kb character RAM (port 1 cpu, port 2 charmap)
 - 2Kb foreground colour RAM (port 1 cpu, port 2 graphics)
 - 2Kb background colour RAM (port 1 cpu, port 2 graphics)
 - 128b sprite register RAM (port 1 cpu, port 2 graphics)
 - 2Kb sprite linebuffer RAM (port 1 write, port 2 read)
 - 32b palette ROM (port 1 8-bit download, port 2 16-bit read)
 - 2Kb sprite ROM (port graphics, port 2 download) 
 - 9 Memory-mapped IO regions.  _Yes I know about MREQ but SFRs turned out to be annoying in sdcc so :)_
   - Hardware state (H/V Sync, H/V Blank etc) (8 bytes, read-only)
   - joystick_5->0 from HPS (192 bytes, read-only)
   - joystick_l_analog_5->0 from HPS (96 bytes, read-only)
   - joystick_r_analog_5->0 from HPS (96 bytes, read-only)
   - paddle_5->0 from HPS (48 bytes, read-only)
   - spinner_5->0 from HPS (96 bytes, read-only)
   - ps2_key from HPS (11 bytes, read-only)
   - ps2_mouse from HPS (48 bytes, combination of ps2_mouse and ps2_mouseext with some padding to align bytes)
   - timestamp from HPS (33 bytes, read-only)
 - Hardware microsecond timer (16 bytes read/write)
 - LFSR-based starfield generator (from Project F 0 - Ad Astra (https://projectf.io/posts/fpga-ad-astra/) by Will Green)

### Memory Map
Start|End|Length|Name
---|---|---|---
0x0000|0x8000|Program ROM
0x8000|0x0001|System inputs (video signals)
0x8100|0x00C0|Joystick inputs
0x8200|0x0060|Analog left inputs
0x8300|0x0060|Analog right inputs
0x8400|0x0030|Paddle inputs
0x8500|0x0060|Spinner inputs
0x8600|0x000C|PS2 keyboard input
0x8700|0x0030|PS2 mouse input
0x8800|0x0021|Timestamp
0x8900|0x0011|Timer
0xA000|0x0800|Char RAM
0xA800|0x0800|Colour RAM
0xB000|0x0800|Char ROM
0xC000|0x4000|Work RAM

## Software

/src contains a C program compiled to Z80 assembly using sdcc.  

All required ROMs (compiled program and default MiSTer font) are built into the core when compiled with Quartus.

Selecting Load BIOS in the core allows hot loading of binary OS ROM files for easier development.
Selecting Load Font in the core allows hot loading of MiSTer .pf font files (https://github.com/MiSTer-devel/Fonts_MiSTer).

### Building the OS

#### Required packages
- `sdcc` 
- `srecord`
#### Build steps
- Run `make` in `/src/Makefile` to create binary ROM
- Run `od -An -t x1 -v src/os.bin > rtl/rom.hex` to create hex version of ROM for Quartus build

## Simulation



## License
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.