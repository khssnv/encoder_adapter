#!/usr/bin/env python3

import time
import serial

print("starting")

# with serial.Serial("COM7", 115200, timeout=1) as ser:
with serial.Serial("/dev/ttyUSB0", 115200, timeout=1) as ser:
    t1 = 0.0
    count = 0

    while True:
        t2 = time.time()
        dt = t2 - t1
        if dt > 1.0:
            data = ser.readline()
            t1 = time.time()
            timestr = time.strftime("%H:%M:%S", time.localtime(t1))
            print(f"[{timestr}] -- {count} -- {data}")
            count = 0
        else:
            ser.readline()
            count += 1
