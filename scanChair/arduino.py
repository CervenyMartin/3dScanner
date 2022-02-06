import serial
import cv2
import time
import numpy as np
import os
PATH = "./imgs/img30/12"
SIZE_X=300
SIZE_Z=400

arduino = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=None)
currentlyRotated = 0
cam=cv2.VideoCapture("/dev/video2")

def write(x):
    global currentlyRotated
    arduino.write(bytes(x, 'utf-8'))
    arduino.read(1)

def storeImg(i):
    for j in range(60):
        _,original = cam.read()
    posunX = (len(original[0])-SIZE_X)//2
    posunZ = (len(original)-SIZE_Z)//2

    original = original[posunZ:SIZE_Z+posunZ,posunX:SIZE_X+posunX]

    hsv = cv2.cvtColor(original,cv2.COLOR_BGR2HSV)
    rgb = cv2.cvtColor(original,cv2.COLOR_BGR2RGB)
    mask=cv2.inRange(hsv, l_green, u_green)

    with open(f"{PATH}/pbm/{i}.pbm", mode="w") as file:
        bits = f"P1\n{len(mask[0])}\n{len(mask)}\n"
        for r in mask:
            for c in r:
                if(c == np.uint8(255)):
                    bits+="1"
                else: bits += "0"

        file.write(bits)
        file.close()


    with open(f"{PATH}/ppm/{i}.ppm", mode="w") as file:
        bits = f"P3\n{len(rgb[0])}\n{len(rgb)}\n255"
        print(bits,file=file)
        for r in rgb:
            for c in r:
                for p in c:
                    print(p, end = " ", file=file)
        file.close()

    
l_green = np.array([36,71,59])
u_green = np.array([88,255,205])
time.sleep(2)
write("200")
write("-200")

for i in range(0,100):
    time.sleep(2.4)
    storeImg(i)
    write(str(2))

    


