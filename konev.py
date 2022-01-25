import cv2
import numpy as np
# read image 
l_green = np.array([46,25,21])
u_green = np.array([88,245,255])

for i in range(0,100):
    img = cv2.imread(f'./imgs/img1/pngs/60/{i:04d}.png')
    img = img[200:600,150:450]

    hsv = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
    mask=cv2.inRange(hsv, l_green, u_green)

    # show the image, provide window name first
    with open(f"./imgs/img1/pbms/60/{i}.pbm", mode="w") as file:
            bits = f"P1\n{len(mask[0])}\n{len(mask)}\n"
            for r in mask:
               for c in r:
                    if(c == np.uint8(255)):
                        bits+="1"
                    else: bits += "0"

            file.write(bits)
            file.close()
