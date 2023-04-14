
import rplidar.h
#include <signal.h>
#!/usr/bin/env python3
import rclpy
import math
from rclpy.node import Node
from sensor_msgs.msg import LaserScan


class LaserScanSubscriberNode(Node):

    def __init__(self):
        super().__init__("LaserScan_subscriber")
        self.LaserScan_subscriber_ = self.create_subscription(
            LaserScan, "/scan", self.pose_callback, 0.0001)

    def pose_callback(self, msg: LaserScan):
        self.get_logger().info(
            str(msg.angle_min)+str(msg.angle_max))
        
def main(args=None):
    rclpy.init(args=args)
    node = LaserScanSubscriberNode()
    rclpy.spin(node)
    rclpy.shutdown()