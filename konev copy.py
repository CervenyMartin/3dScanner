import cv2
from cv2 import COLOR_BGR2RGB
import numpy as np
# read image 
for j in [0,20,40,60]:
    for i in range(100):
        img = cv2.imread(f'./imgs/img1/pngs/{j}/{i:04d}.png')
        img = img[200:600,150:450]
        img = cv2.cvtColor(img,COLOR_BGR2RGB)
        with open(f'./imgs/img1/ppms/{j}/{i}.ppm', mode="w") as file:
                bits = f"P3\n{len(img[0])}\n{len(img)}\n255"
                print(bits,file=file)
                for r in img:
                    for c in r:
                        for p in c:
                            print(p, end = " ", file=file)
                file.close()
