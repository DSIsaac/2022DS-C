# UART Control
#
# This example shows how to use the serial port on your OpenMV Cam. Attach pin
# P4 to the serial input of a serial LCD screen to see "Hello World!" printed
# on the serial LCD display.

import sensor, image, time, math, pyb
from pyb import UART

# Always pass UART 3 for the UART number for your OpenMV Cam.
# The second argument is the UART baud rate. For a more advanced UART control
# example see the BLE-Shield driver.
uart = UART(1, 115200)
buf = [0, 0, 0, 0]

round_flag = 0
stop_flag = 0

round_state = 0
stop_state = 0
GRAYSCALE_THRESHOLD = [(0, 78)]
#ROIS = [ # [ROI, weight]
        #(0, 100, 160, 20, 0.7), # You'll need to tweak the weights for your app
        #(0,  50, 160, 20, 0.3), # depending on how your robot is setup.
        #(0,   0, 160, 20, 0.1)
       #]
ROIS = [ # [ROI, weight]
        (0, 100, 60, 20, 0.6),
        (0,  50, 60, 20, 0.2),
        (0,   0, 60, 20, 0.2),
        (60, 100, 40, 20, 0.6),
        (60,  50, 40, 20, 0.2),
        (60,   0, 40, 20, 0.2)
]

# Compute the weight divisor (we're computing this so you don't have to make weights add to 1).
weight_sum = 0
#for r in ROIS: weight_sum += r[4] # r[4] is the roi weight.

# Camera setup...
sensor.reset() # Initialize the camera sensor.
sensor.set_pixformat(sensor.GRAYSCALE) # use grayscale.
sensor.set_framesize(sensor.QQVGA) # use QQVGA for speed.
sensor.skip_frames(time = 2000) # Let new settings take affect.
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
clock = time.clock() # Tracks FPS.
led1 = pyb.LED(1)
led2 = pyb.LED(2)
led3 = pyb.LED(3)
led1.on()
led2.on()
led3.on()
def uart_recieve():
    global round_flag
    global stop_flag
    global stop_state
    if uart.any():
        rxbuf = uart.read(8)
        #print(rxbuf)
        if len(rxbuf) >= 4:
            for i in range(0, 4):
                if rxbuf[i] == 0xae and i < 4:
                    buf[0] = rxbuf[i]
                    buf[1] = rxbuf[i + 1]
                    buf[2] = rxbuf[i + 2]
                    buf[3] = rxbuf[i + 3]
                    uart.read()
                    #print(buf)
                    #print(clock.fps())
                    round_flag = buf[1]
                    #print(round_flag)
                    stop_flag = buf[2]
                    if stop_flag == 0:
                        stop_state = 0
                    break
    else:
        buf[0] = 0
        buf[1] = 0
        buf[2] = 0
        buf[3] = 0

def uart_send(angle = 0, stop = 0):
    send_buf = [0xAE, 0, 0, 0, 0, 0xEF]
    angle = angle * 100
    if angle < 0:
        angle = -angle
        send_buf[1] = 1
    if angle < 10:
        send_buf[2] = 0
        send_buf[3] = int(angle)
    elif (angle > 100 and angle < 10000):
        send_buf[2] = int(angle / 100)
        send_buf[3] = int(angle % 100)
    send_buf[4] = stop
    #print(send_buf)
    uart.writechar(send_buf[0])
    uart.writechar(send_buf[1])
    uart.writechar(send_buf[2])
    uart.writechar(send_buf[3])
    uart.writechar(send_buf[4])
    uart.writechar(send_buf[5])

while(True):
    #uart.write("Hello World!\r")
    #time.sleep_ms(1000)
    clock.tick()
    uart_recieve()
    img = sensor.snapshot().lens_corr(strength = 1.5, zoom = 1.0) # Take a picture and return the image.
    #img.binary(GRAYSCALE_THRESHOLD)
    centroid_sum = 0
    weight_sum = 0
    round_state = 0
    center_pos = 0
    for r in ROIS:
        blobs = img.find_blobs(GRAYSCALE_THRESHOLD, roi=r[0:4], merge=True) # r[0:4] is roi tuple.
        #print(blobs)

        if blobs:
            # Find the blob with the most pixels.
            #img.draw_rectangle(blobs.rect())
            #img.draw_cross(blobs.cx(),
                           #blobs.cy())
            largest_blob = max(blobs, key=lambda b: b.pixels())

            # Draw a rect around the blob.
            img.draw_rectangle(largest_blob.rect())
            img.draw_cross(largest_blob.cx(),
                           largest_blob.cy())
            if r[0] == 0 and largest_blob.w() > 10 and round_flag == 1:
                round_state = 1;
            #print(round_state, round_flag)
            if r[0] > 0 and round_flag == 0:
                centroid_sum += largest_blob.cx() * r[4] # r[4] is the roi weight.
                weight_sum += r[4]
                if largest_blob.w() > 30 and r[1] == 50:
                    stop_state = 1
                #weight_sum += r[4]
            elif round_flag == 1:
                #if round_state == 1 and ((r[0] == 0)):
                if round_state == 1 and ((r[0] == 0) or (r[0] == 60 and r[1] != 0)):
                    centroid_sum += largest_blob.cx() * r[4] # r[4] is the roi weight.
                    weight_sum += r[4]
                    #print(1)
                    #print(r)
                elif r[0] > 0 and round_state == 0:
                    centroid_sum += largest_blob.cx() * r[4] # r[4] is the roi weight.
                    weight_sum += r[4]
                    #print(2)
                    #print(r)
                    if largest_blob.w() > 30 and r[1] == 50:
                        stop_state = 1



    if weight_sum != 0:
        center_pos = (centroid_sum / weight_sum) # Determine center of line.
    #print(center_pos)
    #print(weight_sum)
    # Convert the center_pos to a deflection angle. We're using a non-linear
    # operation so that the response gets stronger the farther off the line we
    # are. Non-linear operations are good to use on the output of algorithms
    # like this to cause a response "trigger".
    deflection_angle = 0

    # The 80 is from half the X res, the 60 is from half the Y res. The
    # equation below is just computing the angle of a triangle where the
    # opposite side of the triangle is the deviation of the center position
    # from the center and the adjacent side is half the Y res. This limits
    # the angle output to around -45 to 45. (It's not quite -45 and 45).
    deflection_angle = -math.atan((center_pos - 80) / 60)

    # Convert angle in radians to degrees.
    deflection_angle = math.degrees(deflection_angle)
    #if(round_state == 1 and round_flag == 1):
        #deflection_angle += 100

    # Now you have an angle telling you how much to turn the robot by which
    # incorporates the part of the line nearest to the robot and parts of
    # the line farther away from the robot for a better prediction.
    #print("Turn Angle: %f" % deflection_angle)

    #print(clock.fps()) # Note: Your OpenMV Cam runs about half as fast while
    # connected to your computer. The FPS should increase once disconnected.


    uart_send(deflection_angle, stop_state)

