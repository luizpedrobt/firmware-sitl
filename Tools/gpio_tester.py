import struct
import random
import time
import os

GPIO_FILE = "../gpio.bin"
HEADER = 0xDEADBEEF

FMT = "<I B B"
RECORD_SIZE = struct.calcsize(FMT)

PINS = [0, 1]


def write_pin(f, file_size, pin, state):
    offset = pin * RECORD_SIZE

    if offset + RECORD_SIZE > file_size:
        print(f"[WARN] Pin {pin} fora do tamanho atual do arquivo. Ignorando.")
        return

    f.seek(offset)

    data = struct.pack(FMT, HEADER, pin, state)
    f.write(data)
    f.flush()


def random_toggle():
    if not os.path.exists(GPIO_FILE):
        print("Arquivo gpio.bin não existe!")
        return

    file_size = os.path.getsize(GPIO_FILE)

    with open(GPIO_FILE, "r+b") as f:
        while True:
            pin = random.choice(PINS)
            state = random.randint(0, 1)

            write_pin(f, file_size, pin, state)

            print(f"[PY] Pin {pin} -> {state}")

            time.sleep(random.uniform(0.1, 1.0))


if __name__ == "__main__":
    random_toggle()