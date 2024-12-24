# PD Arcade picohub PCB
=================================
![PCB](/PCB/images/PD_Arcade_picohub_board.png)
## Parts list
- PCB: 1x `PD_Arcade_picohub_board` **1.6mm thickness** (KiCad 7.99+ (7.99.0-697-g4d05bd62da), release build required for exporting Gerber files or take Gerber files archive in `PCB/gerber/PD_Arcade_picohub_board.zip`)
  - U1: `Raspberry Pi Pico` board
  - U2, U3: 2x `ULN2003` transistor assembly
  - U4: `TXS0108EPW` bi-directional level-shifter *(used for connection [SHINSANWASWITCH](https://github.com/steelpuxnastik/SHINSANWASWITCH) optical switches as main buttons and [LKP](https://github.com/Project-Alpaca/LKP) slider. Those components are basically uses 5v power supply, that is why it needs level shifter to connect to Raspberry Pi Pico that uses 3.3v signal level. There are possible other configurations of those components and in case of their 3.3v power supply you can exclude `TXS0108EPW` and connect them directly to Raspberry Pi Pico pins bypassing level shifter)*
  - C1, C2: `0.1uf` 0603 SMD condenser *(used for level shifter)*
- Parts for PWM brightness control (short jumpers if no PWM used, **brightness control will not work**):
  - Q1: `IRFR9024N` P-channel MOSFET
  - Q2: `AO3404A` N-channel MOSFET
  - R1: `1k` 1206 SMD resistor
  - R2: `10k` 1206 SMD resistor
- connectors:
  - 2x DIP-16 panel for `ULN2003`
  - 5x `JST XH-02` male connector
  - 4x `JST XH-03` male connector *(used for connection [SHINSANWASWITCH](https://github.com/steelpuxnastik/SHINSANWASWITCH) optical switches for main buttons. It's an active component, so it uses 3 wires. If you use passive switches - you can change to `JST XH-02` and connect it directly to Raspberry Pi Pico pins bypassing level shifter)*
  - 6x `JST XH-04` male connector
  - female 2.54 single row headset for `Raspberry Pi Pico`
  
  ![PCB assembled](/PCB/images/photo_2024-11-25_15-13-38.jpg)
