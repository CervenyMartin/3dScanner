import cv2
import numpy as np



def nothing(e):
    pass

cv2.namedWindow("Trackbars")
cv2.resizeWindow("Trackbars",300,300)
cv2.createTrackbar('l-h',"Trackbars",36,255,nothing)
cv2.createTrackbar('l-s',"Trackbars",31,255,nothing)
cv2.createTrackbar('l-v',"Trackbars",46,225,nothing)
cv2.createTrackbar('u-h',"Trackbars",88,255,nothing)
cv2.createTrackbar('u-s',"Trackbars",255,255,nothing)
cv2.createTrackbar('u-v',"Trackbars",255,255,nothing)

cam=cv2.VideoCapture("/dev/video2")
while True:
    rat,original = cam.read()
    img = original[90:390,203:437]
    hsv = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
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
        cam.release()
        break


cv2.destroyAllWindows()