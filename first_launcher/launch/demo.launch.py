import os
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    ld = LaunchDescription()
    
    config = os.path.join(
        get_package_share_directory('first_launcher'),
        'config',
        'params.yaml'
    )

    talker_node = Node(
        package="my_robot_controller",
        executable="test_node",
        name="my_talker",
        #remappings= [
        #    ("chatter", "my_chatter")
        #],
        parameters= [
            config
            #{"param_name": "value"},
            #{"param2": 19}
        ]
    )

    listener_node = Node(
        package="demo_nodes_py",
        executable="listener",
        #remappings= [
        #    ("chatter", "my_chatter")
        #]
    )

    ld.add_action(talker_node)
    ld.add_action(listener_node)
    
    return ld