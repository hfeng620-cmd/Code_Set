from machine import Pin, I2C
import framebuf
import random
import time


WIDTH = 128
HEIGHT = 64
I2C_ID = 0
SDA_PIN = 0
SCL_PIN = 1


class SSD1306:
    def __init__(self, width, height, i2c, addr=0x3C):
        self.width = width
        self.height = height
        self.i2c = i2c
        self.addr = addr
        self.pages = height // 8
        self.buffer = bytearray(self.pages * width)
        self.framebuf = framebuf.FrameBuffer(self.buffer, width, height, framebuf.MONO_VLSB)
        self.init_display()

    def write_cmd(self, cmd):
        self.i2c.writeto(self.addr, bytearray([0x80, cmd]))

    def init_display(self):
        for cmd in (
            0xAE, 0x20, 0x00, 0x40, 0xA1, 0xA8, self.height - 1,
            0xC8, 0xD3, 0x00, 0xDA, 0x12, 0xD5, 0x80, 0xD9, 0xF1,
            0xDB, 0x30, 0x81, 0xFF, 0xA4, 0xA6, 0x8D, 0x14, 0xAF
        ):
            self.write_cmd(cmd)
        self.fill(0)
        self.show()

    def fill(self, color):
        self.framebuf.fill(color)

    def pixel(self, x, y, color):
        self.framebuf.pixel(x, y, color)

    def rect(self, x, y, w, h, color):
        self.framebuf.rect(x, y, w, h, color)

    def fill_rect(self, x, y, w, h, color):
        self.framebuf.fill_rect(x, y, w, h, color)

    def ellipse(self, x, y, rx, ry, color, fill=False):
        for yy in range(-ry, ry + 1):
            for xx in range(-rx, rx + 1):
                inside = (xx * xx) * (ry * ry) + (yy * yy) * (rx * rx)
                edge = rx * rx * ry * ry
                if fill:
                    draw = inside <= edge
                else:
                    draw = edge - (rx * rx + ry * ry) * 3 <= inside <= edge
                if draw:
                    px = x + xx
                    py = y + yy
                    if 0 <= px < self.width and 0 <= py < self.height:
                        self.pixel(px, py, color)

    def show(self):
        self.write_cmd(0x21)
        self.write_cmd(0)
        self.write_cmd(self.width - 1)
        self.write_cmd(0x22)
        self.write_cmd(0)
        self.write_cmd(self.pages - 1)
        self.i2c.writeto(self.addr, bytearray([0x40]) + self.buffer)


def draw_eye(display, cx, cy, look_x, blink):
    if blink:
        display.fill_rect(cx - 18, cy - 2, 36, 4, 1)
        return

    display.ellipse(cx, cy, 18, 19, 1, True)
    display.ellipse(cx + look_x, cy + 3, 6, 8, 0, True)
    display.ellipse(cx - 6, cy - 7, 4, 4, 0, True)


def draw_face(display, look_x=0, blink=False):
    display.fill(0)
    draw_eye(display, 40, 31, look_x, blink)
    draw_eye(display, 88, 31, look_x, blink)
    display.show()


def main():
    i2c = I2C(I2C_ID, sda=Pin(SDA_PIN), scl=Pin(SCL_PIN), freq=400000)
    display = SSD1306(WIDTH, HEIGHT, i2c)

    look_x = 0
    next_look = time.ticks_add(time.ticks_ms(), 1000)
    next_blink = time.ticks_add(time.ticks_ms(), 1800)

    while True:
        now = time.ticks_ms()

        if time.ticks_diff(now, next_look) >= 0:
            look_x = random.choice([-6, -3, 0, 3, 6])
            next_look = time.ticks_add(now, random.randint(900, 1800))

        if time.ticks_diff(now, next_blink) >= 0:
            draw_face(display, look_x, True)
            time.sleep_ms(110)
            next_blink = time.ticks_add(now, random.randint(1600, 3200))

        draw_face(display, look_x, False)
        time.sleep_ms(80)


main()
