import cv2
import numpy as np
CROP_SIZE_X = 702
CROP_SIZE_Y = 900
IMGFOLDER = "img2"
def rmbg(angle, picture):
    img = cv2.imread(f"./{IMGFOLDER}/pngs/{angle}/{picture:04d}.png", 0)
    _ , img = cv2.threshold(img, 100, 255, cv2.THRESH_BINARY_INV)

    with open(f"./{IMGFOLDER}/pbms/{angle}/{picture}.pbm", 'x') as f:
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
for a in range(0,61,20):
    print(f"angle {a}:")
    for i in range(0,101):
        rmbg(a, i)
        if (i%10==0):
            print(f"{i}% .. ",end="")
    print('')
