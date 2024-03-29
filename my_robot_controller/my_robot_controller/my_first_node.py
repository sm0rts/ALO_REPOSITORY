#!/usr/bin/env python3
import rclpy
from rclpy.node import Node

class MyNode(Node):

    def __init__(self):
        super().__init__("first_node")
        self.get_logger().info("Start")
        self.counter = 0
        self.create_timer(1.0, self.timer_callback)
        self.declare_parameters(
            namespace='',
            parameters=[
            ('bool_value', rclpy.Parameter.Type.BOOL),
            ('int_number', rclpy.Parameter.Type.INTEGER)
            ]
        )

    def timer_callback(self):
        self.get_logger().info("Hello " + str(self.counter))
        self.counter +=1

def main(args=None):
    rclpy.init(args=args)
    node = MyNode()
    rclpy.spin(node)
    #

    rclpy.shutdown()

if __name__ == '__main__':
    main()