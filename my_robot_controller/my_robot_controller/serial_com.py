#!/usr/bin/env python3
import rclpy
import os
from rclpy.node import Node
import serial
from geometry_msgs.msg import Twist
from turtlesim.msg import Pose
'''
class SerialNode(Node):
    def __init__(self,port, baudrate):
        port = port
        baudrate = baudrate
        super().__init__('serial_node')
        self.stm_data = serial.Serial('/dev/ttyUSB0',9600)
        self.create_timer(0.10, self.request_position)
'''
class SerialNode(Node):
    def __init__(self,port, baudrate):
        port = port
        baudrate = baudrate
        super().__init__('serial_node')
        self.stm_data = serial.Serial(port,baudrate)
        self.create_timer(0.5, self.request_position)
#################
        self.cmd_vel_subscriber = self.create_subscription(
            Twist, "/turtle1/cmd_vel",self.read_command, 10)

    def read_command(self, msg: Twist):
        message = str("linear velocity= ")+str(msg.linear.x)+str("\tangular velocity = ")+str(msg.angular.z)
        if msg.linear.x == -2:
            self.send_cmd("DOWN")
        if msg.linear.x == 2:
            self.send_cmd("UP")
        if msg.angular.z == 2:
             self.send_cmd("LEFT")
        if msg.angular.z == -2:
             self.send_cmd("RIGHT")
##################

    def request_position(self):
            self.send_cmd('REQ')

    def send_cmd(self, cmd):
            cmd = cmd + '\r'
            self.stm_data.write(cmd.encode())
            dataPacket = self.stm_data.readline()
            dataPacket = str(dataPacket,'utf-8')
            dataPacket = dataPacket.strip('\r\n')
            print(dataPacket)
            dataPacket = 0
        
def find_serial_port_with_string(string):
    usb_devices_path = "/dev/serial/by-id/"
    for device_file in os.listdir(usb_devices_path):
        if string in device_file:
            device_path = os.path.join(usb_devices_path, device_file)
            device_port = os.path.realpath(device_path)
            if device_port.startswith("/dev/ttyUSB"):
                return device_port
    return None

def main(args=None):
    print(find_serial_port_with_string('CP210'))
    rclpy.init(args=args)
    node = SerialNode(find_serial_port_with_string('CP210'),115200)
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()
