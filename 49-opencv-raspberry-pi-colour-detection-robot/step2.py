# Colour tracker that sends steering commands - not tested yet
import cv2, serial, time
import numpy as np

LOWER = np.array([100, 120, 70])     # paste your tuned values
UPPER = np.array([130, 255, 255])
MIN_AREA = 300                       # ignore tiny specks
STOP_AREA = 25000                    # ball very close: stop
W, H = 320, 240

ser = serial.Serial('/dev/ttyACM0', 115200, timeout=0.1)  # may be /dev/ttyUSB0
time.sleep(2)                        # Arduino resets when the port opens

# --- USB webcam ---
cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, W)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, H)
def grab():
    ok, f = cap.read()
    return f if ok else None

# --- Pi Camera Module instead: comment the webcam block, uncomment this ---
# from picamera2 import Picamera2
# picam = Picamera2()
# picam.configure(picam.create_preview_configuration(main={"format": "RGB888", "size": (W, H)}))
# picam.start()
# def grab():
#     return picam.capture_array()   # "RGB888" arrives in BGR order, ready for OpenCV

kernel = np.ones((5, 5), np.uint8)
last_cmd = None
last_send = 0.0

while True:
    frame = grab()
    if frame is None:
        ser.write(b'S')
        break
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv, LOWER, UPPER)
    mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)   # remove noise

    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    cmd = 'S'
    if contours:
        c = max(contours, key=cv2.contourArea)
        area = cv2.contourArea(c)
        if MIN_AREA < area < STOP_AREA:
            M = cv2.moments(c)
            cx = int(M["m10"] / M["m00"])
            if cx < W / 3:
                cmd = 'L'
            elif cx > 2 * W / 3:
                cmd = 'R'
            else:
                cmd = 'F'

    now = time.monotonic()
    if cmd != last_cmd or now - last_send >= 0.25:
        ser.write(cmd.encode())
        last_send = now
        if cmd != last_cmd:
            print(cmd)
            last_cmd = cmd
