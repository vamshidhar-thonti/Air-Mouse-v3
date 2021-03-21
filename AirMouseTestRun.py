import serial
import mouse
from time import sleep


def normalize(IncomingData):
    x_pos, y_pos, LEFT, RIGHT = IncomingData.decode('utf-8').rstrip().split(sep=",")
    if x_pos == '':  # getting rid of empty strings
        x_pos = '0'
    if y_pos == '':
        y_pos = '0'
    if LEFT == '':
        LEFT = '0'
    if RIGHT == '':
        RIGHT = '0'
    x_pos = int(float(x_pos))  # To avoid value error while converting values to int
    y_pos = int(float(y_pos))
    LEFT = int(float(LEFT))
    RIGHT = int(float(RIGHT))
    return x_pos, y_pos, LEFT, RIGHT


SerialPort = serial.Serial(port="COM4", baudrate=9600)

while 1:
    IncomingData = SerialPort.readline()
    if IncomingData:
        # print(IncomingData.decode('utf-8').rstrip())
        x_pos, y_pos, LEFT, RIGHT = normalize(IncomingData)
        print(x_pos, y_pos, LEFT, RIGHT)
        mouse.move(x=x_pos, y=y_pos, absolute=False)
        if LEFT == 1:
            mouse.click('left')
        if RIGHT == 1:
            mouse.click('right')

    sleep(0.01)
