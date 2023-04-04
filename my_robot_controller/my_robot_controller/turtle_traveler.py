#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
from turtlesim.msg import Pose
from turtlesim.srv import SetPen
from functools import partial

class turtle_traveler(Node):

    def __init__(self):
        super().__init__("turtle_traveler")
        self.pervious_x_ = 0
        self.counter = 0
        self.cmd_vel_pub = self.create_publisher(
            Twist, "/turtle1/cmd_vel", 10)
        self.pose_subscriber_ = self.create_subscription(
            Pose, "/turtle1/pose",self.pose_callback, 10)
        #self.timer_ = self.create_timer(0.4, self.send_velocity_command)
        self.get_logger().info("turtle controller has started")
        self.create_timer(0.5, self.timer_reset)
    
    def timer_reset(self):
        self.counter += 1

    def pose_callback(self, pose: Pose):
        cmd = Twist()
        i = 0
        if pose.x > 9.0 or pose.y > 9.0 or pose.x < 1.0 or pose.y < 1.0:
            cmd.linear.x = 10.0
            cmd.angular.z = 9.0
        else:
            cmd.linear.x = 10.0
            cmd.angular.z = 0.0
        if self.counter == 10:
            cmd.linear.x = 10.0
            cmd.angular.z = 0.0
            self.counter = 0
        self.cmd_vel_pub.publish(cmd)
    
        if   3.0 < pose.x and self.pervious_x_ <=3.0:
            self.pervious_x_ = pose.x
            self.get_logger().info("change color to red ")
            self.call_set_service_pen(255,50,0, 3, 0)
        elif   3.0 > pose.x and self.pervious_x_ >=3.0:
            self.pervious_x_ = pose.x
            self.get_logger().info("change color to blue ")
            self.call_set_service_pen(0,50,255, 3, 0)    
        elif 7.0 < pose.x and self.pervious_x_ <=7.0:
            self.pervious_x_ = pose.x
            self.get_logger().info("change color to white")
            self.call_set_service_pen(255,250,255,4,0)
        elif 7.0 > pose.x and self.pervious_x_ >=7.0:
            self.pervious_x_ = pose.x
            self.get_logger().info("change colot to red")
            self.call_set_service_pen(255,50,0,4,0)
        self.pervious_x_ = pose.x

    def call_set_service_pen(self, r, g ,b, w, off):
        client = self.create_client(SetPen, "/turtle1/set_pen")
        while not client.wait_for_service(1.0):
            self.get_logger().warn("Waiting for service...")
        request =SetPen.Request()
        request.r = r
        request.g = g 
        request.b = b 
        request.width = w
        request.off = off

        future = client.call_async(request)
        future.add_done_callback(partial(self.callback_set_pen))
    
    def callback_set_pen(self, future):
        try:
            response = future.result()
        except Exception as e:
            self.get_logger().error("Service call failed: %r " % (e,))
    

def main(args=None):
    rclpy.init(args=args)
    node = turtle_traveler()
    rclpy.spin(node)
    rclpy.shutdown()