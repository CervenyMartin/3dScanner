import serial
import time
#arduino = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=.1)
#def message(x):
#    arduino.write(bytes(x, 'utf-8'))

#for i in range(0,90):
#    for j in range(0,4):
#        print(i+j*90)
import cv2
  
  
# define a video capture object
vid = cv2.VideoCapture("/dev/video2")
  
while(True):
      
    # Capture the video frame
    # by frame
    ret, frame = vid.read()
  
    # Display the resulting frame
    cv2.imshow('frame', frame)
      
    # the 'q' button is set as the
    # quitting button you may use any
    # desired button of your choice
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
  
# After the loop release the cap object
vid.release()
# Destroy all the windows
cv2.destroyAllWindows()