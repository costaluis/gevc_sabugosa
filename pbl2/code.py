import cv2
video = cv2.VideoCapture("road6.mp4")
while True:
    ret,frame = video.read()
    cv2.imshow("video",frame)
    if cv2.waitKey(30) == ord('q'):
        break
video.release()
cv2.destroyAllWindows()
