import serial
import time
stm_data = serial.Serial('/dev/ttyUSB0',9600)

while True:
    cmd  = input('Enter command:')
    cmd = cmd + '\r'
    dataPacket = 0
    stm_data.write(cmd.encode())
    dataPacket = stm_data.readline()
    dataPacket = str(dataPacket,'utf-8')
    dataPacket = dataPacket.strip('\r\n')
    print(dataPacket)
    dataPacket = 0