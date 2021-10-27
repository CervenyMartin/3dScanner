import cv2
import numpy as np
CROP_SIZE_X = 702
CROP_SIZE_Y = 900

img = cv2.imread(f"angle.png", 0)
_ , img = cv2.threshold(img, 100, 255, cv2.THRESH_BINARY_INV)

with open(f"./angle.pmb", 'x') as f:
    startY = (len(img)-CROP_SIZE_Y)//2
    endY = (len(img)-CROP_SIZE_Y)//2+CROP_SIZE_Y
    startX = (len(img[0])-CROP_SIZE_X)//2
    endX = (len(img[0])-CROP_SIZE_X)//2 + CROP_SIZE_X
    for i in range(startY, endY, 3):
        for j in range(startX, endX, 3):
            if img[i][j] == 255:
                print(0, file=f, end='')
            else: print(1, file=f,end='')
    f.close()

