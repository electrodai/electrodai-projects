# Pi: send commands to the Arduino over USB - not tested yet
import serial, time

ser = serial.Serial('/dev/ttyACM0', 115200, timeout=1)  # may be /dev/ttyUSB0 on clones
time.sleep(2)            # the Arduino resets when the port opens

for cmd in [b'F', b'S', b'F', b'S']:
    ser.write(cmd)
    print(ser.readline().decode().strip())
    time.sleep(1)
ser.close()
