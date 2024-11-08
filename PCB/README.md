# PD Arcade picohub PCB
=================================
![PCB](/PCB/images/PD_Arcade_picohub_board.png)
## Parts list
- PCB: 1x `PD_Arcade_picohub_board` **1.6mm thickness** (KiCad 7.99+ (7.99.0-697-g4d05bd62da), release build required for exporting Gerber files or take Gerber files archive in `PCB/gerber/PD_Arcade_picohub_board.zip`)
  - U1: `Raspberry Pi Pico` board
  - U2, U3: 2x `ULN2003` transistor assembly
  - U4: `TXS0108EPW` bi-directional level-shifter
  - C1, C2: `0.1uf` 0603 SMD condenser
- Parts for PWM brightness control (short jumpers if no PWM used, **brightness control will not work**):
  - Q1: `IRFR9024N` P-channel MOSFET
  - Q2: `AO3404A` N-channel MOSFET
  - R1, R2: `10k` 1206 SMD resistor
- connectors:
  - 2x DIP-16 panel for `ULN2003`
  - 5x `JST XH-02` male connector
  - 4x `JST XH-03` male connector
  - 6x `JST XH-04` male connector
  - female 2.54 single row headset for `Raspberry Pi Pico`