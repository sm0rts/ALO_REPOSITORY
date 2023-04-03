#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
from turtlesim.msg import Pose

class turtle_traveler(Node):

    def __init__(self):
        super().__init__("turtle_traveler")
        self.cmd_vel_pub = self.create_publisher(
            Twist, "/turtle1/cmd_vel", 10)
        self.pose_subscriber_ = self.create_subscription(
            Pose, "/turtle1/pose",self.pose_callback, 10)
        #self.timer_ = self.create_timer(0.4, self.send_velocity_command)
        self.get_logger().info("turtle controller has started")
        
    def pose_callback(self, pose: Pose):
        cmd = Twist()
        
        if pose.x > 9.0 or pose.y > 9.0 or pose.x < 1.0 or pose.y < 1.0:
            cmd.linear.x = 10.0
            cmd.angular.z = 90.0
        else:
            cmd.linear.x = 10.0
            cmd.angular.z = 0.0
        self.cmd_vel_pub.publish(cmd)
        '''
        msg = Twist()
        pos = Pose()
        dirvert = round(pos._theta, 2)
        dirhor = abs(dirvert-1.57)
        if(abs(5.5-pos.x)>4 and dirvert <0 ): 
            msg.angular.z = 1.0
        if(abs(5.5-pos.x)>4 and dirvert >0 ): 
            msg.angular.z = 1.0
        if(abs(5.5-pos.x)>4 and dirvert <0 ): 
            msg.angular.z = 1.0
        if(abs(5.5-pos.x)>4 and dirvert >0 ): 
            msg.angular.z = 1.0
        else:
            msg.angular.z = 0
        msg.linear.x = 3.0
        self.cmd_vel_pub .publish(msg)    
    
    def send_velocity_command(self):
        msg = Twist()
        pos = Pose()
        dirvert = round(pos._theta, 2)
        dirhor = abs(dirvert-1.57)
        if(abs(5.5-pos.x)>4 and dirvert <0 ): 
            msg.angular.z = 1.0
        if(abs(5.5-pos.x)>4 and dirvert >0 ): 
            msg.angular.z = 1.0
        if(abs(5.5-pos.x)>4 and dirvert <0 ): 
            msg.angular.z = 1.0
        if(abs(5.5-pos.x)>4 and dirvert >0 ): 
            msg.angular.z = 1.0
        else:
            msg.angular.z = 0
        msg.linear.x = 3.0
        self.cmd_vel_pub .publish(msg)
    '''

def main(args=None):
    rclpy.init(args=args)
    node = turtle_traveler()
    rclpy.spin(node)
    rclpy.shutdown()