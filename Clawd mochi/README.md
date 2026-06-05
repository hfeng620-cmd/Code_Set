# Pico Mochi OLED Test

This is a minimal MicroPython version for Raspberry Pi Pico RP2040 plus a
0.96-inch SSD1306 I2C OLED display.

## Wiring

Use 3.3V power if the module supports it.

| OLED pin | Pico pin |
| --- | --- |
| GND | GND |
| VCC | 3V3 OUT |
| SCL | GP1 |
| SDA | GP0 |

If your OLED pins are labeled `SCK/SDA`, use `SCK -> GP1` and `SDA -> GP0`.

## Use

1. Flash MicroPython firmware to the Pico.
2. Open Thonny.
3. Save `main.py` to the Pico.
4. Reset the Pico.

If the screen stays blank, try changing the I2C address in `main.py` from
`0x3C` to `0x3D`.

