import cv2
import numpy as np

SIZE_X=300
SIZE_Z=400
PATH = "./imgs/img14/12"
cam=cv2.VideoCapture("/dev/video2")

def storeImg(i):
    l_green = np.array([36,38,59])
    u_green = np.array([88,255,205])
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

#storeImg(0)

#exit()

def nothing(e):
    pass

cv2.namedWindow("Trackbars")
cv2.resizeWindow("Trackbars",300,300)
cv2.createTrackbar('l-h',"Trackbars",36,255,nothing)
cv2.createTrackbar('l-s',"Trackbars",38,255,nothing)
cv2.createTrackbar('l-v',"Trackbars",59,225,nothing)
cv2.createTrackbar('u-h',"Trackbars",88,255,nothing)
cv2.createTrackbar('u-s',"Trackbars",255,255,nothing)
cv2.createTrackbar('u-v',"Trackbars",205,255,nothing)


while True:
    rat,original = cam.read()
#    original = cv2.imread("/home/cervenym/Pictures/mesh.png")
    posunX = (len(original[0])-SIZE_X)//2
    posunZ = (len(original)-SIZE_Z)//2
    img = original.copy()
    original = original[posunZ:SIZE_Z+posunZ,posunX:SIZE_X+posunX]
    
    # osa fotky
    for i in range(-1,2):
        for j in range(len(img[0])):
            img[len(img)//2+i][j] = [0,0,255]
    
    print(SIZE_X//2+i)
    for i in range(-1,2):
        for j in range(len(img)):
            img[j][len(img[0])//2+i] = [0,0,255]



    hsv = cv2.cvtColor(original,cv2.COLOR_BGR2HSV)
    lh = cv2.getTrackbarPos("l-h","Trackbars")
    ls = cv2.getTrackbarPos("l-s","Trackbars")
    lv = cv2.getTrackbarPos("l-v","Trackbars")
    uh = cv2.getTrackbarPos("u-h","Trackbars")
    us = cv2.getTrackbarPos("u-s","Trackbars")
    uv = cv2.getTrackbarPos("u-v","Trackbars")
    
    l_green = np.array([lh,ls,lv])
    u_green = np.array([uh,us,uv])
    mask=cv2.inRange(hsv, l_green, u_green)
    
    cv2.imshow('frame',mask)
    cv2.imshow('frame2',img)
    
    if cv2.waitKey(1) == ord('q'):
   #     cam.release()
        break


cv2.destroyAllWindows()