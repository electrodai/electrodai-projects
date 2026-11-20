# HSV tuner - not tested yet
import cv2

cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 320)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 240)

cv2.namedWindow("sliders")
for name, maxv, start in [("Hmin",179,100),("Hmax",179,130),("Smin",255,120),
                          ("Smax",255,255),("Vmin",255,70),("Vmax",255,255)]:
    cv2.createTrackbar(name, "sliders", start, maxv, lambda x: None)

while True:
    ok, frame = cap.read()
    if not ok:
        break
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    v = [cv2.getTrackbarPos(n, "sliders") for n in ("Hmin","Smin","Vmin","Hmax","Smax","Vmax")]
    mask = cv2.inRange(hsv, tuple(v[:3]), tuple(v[3:]))
    cv2.imshow("frame", frame)
    cv2.imshow("mask", mask)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        print("lower =", v[:3], " upper =", v[3:])
        break

cap.release()
cv2.destroyAllWindows()
