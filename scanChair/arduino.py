import serial
import cv2
import time
import numpy as np
import os
PATH = "./imgs/img3/"


arduino = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=None)
currentlyRotated = 0
cam=cv2.VideoCapture("/dev/video2")
def write(x):
    global currentlyRotated
    arduino.write(bytes(x, 'utf-8'))
    arduino.read(1)
    
l_green = np.array([46,25,21])
u_green = np.array([88,245,255])
time.sleep(2)
write("200")
write("-200")
for i in range(20):
    zk = cam.read()
for i in range(0,100):
    time.sleep(3)
    write(str(2))
    time.sleep(2)
    rat,original = cam.read()
    img = original[90:390,203:437]
    hsv = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
    mask=cv2.inRange(hsv, l_green, u_green)

    with open(f"{PATH}pbms/0/{i}.pbm", mode="w") as file:
        bits = f"P1\n{len(mask[0])}\n{len(mask)}\n"
        for r in mask:
            for c in r:
                if(c == np.uint8(255)):
                    bits+="1"
                else: bits += "0"

        file.write(bits)
        file.close()
    with open(f"{PATH}ppms/0/{i}.ppm", mode="w") as file:
        bits = f"P3\n{len(img[0])}\n{len(img)}\n255"
        print(bits,file=file)
        for r in img:
            for c in r:
                for p in c:
                    print(p, end = " ", file=file)
        file.close()


