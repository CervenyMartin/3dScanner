import serial
import cv2
import time
import numpy as np


arduino = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=None)
currentlyRotated = 0
cam=cv2.VideoCapture("/dev/video2")
def write(x):
    global currentlyRotated
    arduino.write(bytes(x, 'utf-8'))
    arduino.read(1)
    
l_green = np.array([46,25,21])
u_green = np.array([100,245,255])
time.sleep(2)
write("90")
write("180")
write("270")
write("0")
for i in range(20):
    zk = cam.read()
for i in range(0,360,5):
    write(str(i))
    time.sleep(1)
    rat,img = cam.read()
    hsv = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
    mask=cv2.inRange(hsv, l_green, u_green)
    #cv2.imwrite(f"{i}.ppm", img, (cv2.COLOR_BGRA2BGR, 0))
    cv2.imwrite(f"{i}.pbm", mask, (cv2.IMWRITE_PXM_BINARY, 0))
    print(i)
    time.sleep(0.5)
