from setuptools import setup

package_name = 'my_robot_controller'

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='alo-desktop',
    maintainer_email='estereonos@gmail.com',
    description='TODO: Package description',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
    "test_node = my_robot_controller.my_first_node:main",
    "draw_circle = my_robot_controller.draw_circle:main",
    "pose_subscriber = my_robot_controller.pose_subscriber:main",
    "turtle_traveler = my_robot_controller.turtle_traveler:main",
    "scan_subscriber = my_robot_controller.scan_subscriber:main",
    "serial_com = my_robot_controller.serial_com:main",
    "serial_subscriber = my_robot_controller.serial_subscriber:main"
        ],
    },
)
