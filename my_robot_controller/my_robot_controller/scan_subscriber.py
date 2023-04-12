#!/usr/bin/env python3
import rclpy
import math
from rclpy.node import Node
from sensor_msgs.msg import LaserScan

class LaserScanSubscriberNode(Node):

    def __init__(self):
        super().__init__("LaserScan_subscriber")
        self.LaserScan_subscriber_ = self.create_subscription(
            LaserScan, "/scan", self.pose_callback, 10)

    def pose_callback(self, msg: LaserScan):
        self.get_logger().info(str("angle= ")+str(
            +round(msg.angle_increment, 3))+str("    intensity= ")+str(+round(msg.intensities, 2)))

def main(args=None):
    rclpy.init(args=args)
    node = LaserScanSubscriberNode()
    rclpy.spin(node)
    rclpy.shutdown()