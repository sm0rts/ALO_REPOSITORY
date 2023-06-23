#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
import serial
from geometry_msgs.msg import Twist
from turtlesim.msg import Pose

class SerialNode(Node):
    def __init__(self):
        super().__init__('serial_node')
        self.stm_data = serial.Serial('/dev/ttyUSB1',9600)
        self.create_timer(0.10, self.request_position)
#################
        self.cmd_vel_subscriber = self.create_subscription(
            Twist, "/turtle1/cmd_vel",self.read_command, 10)

    def read_command(self, msg: Twist):
        message = str("linear velocity= ")+str(msg.linear.x)+str("\tangular velocity = ")+str(msg.angular.z)
        if msg.linear.x == -2:
            self.send_cmd("OFF")
        if msg.linear.x == 2:
            self.send_cmd("ON")
##################

    def request_position(self):
            self.send_cmd('REQ')

    def send_cmd(self, cmd):
            cmd = cmd + '\r'
            self.stm_data.write(cmd.encode())
            dataPacket = stm_data.readline()
            dataPacket = str(dataPacket,'utf-8')
            dataPacket = dataPacket.strip('\r\n')
            print(dataPacket)
            dataPacket = 0
        

def main(args=None):
    rclpy.init(args=args)
    node = SerialNode()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()

stm_data = serial.Serial('/dev/ttyUSB1',9600)