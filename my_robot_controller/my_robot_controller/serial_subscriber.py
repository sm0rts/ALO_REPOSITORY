import rclpy
from rclpy.node import Node
from std_msgs.msg import String

class SerialSubscriber(Node):
    def __init__(self):
        super().__init__('serial_subscriber')
        self.subscription = self.create_subscription(
            String,
            'serial_data',
            self.serial_data_callback,
            10
        )
        self.timer = self.create_timer(2.0, self.timer_callback)  # Publish every 2 seconds

    def serial_data_callback(self, msg):
        received_data = msg.data
        self.get_logger().info(f"Received data: {received_data}")

    def timer_callback(self):
        self.get_logger().info("Timer callback")

def main(args=None):
    rclpy.init(args=args)
    serial_subscriber = SerialSubscriber()
    rclpy.spin(serial_subscriber)
    serial_subscriber.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
