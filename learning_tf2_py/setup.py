from setuptools import setup
import os
from glob import glob

package_name = 'learning_tf2_py'

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (os.path.join('share', package_name, 'launch'), glob(os.path.join('launch', '*.launch.py'))),
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
    'static_turtle_tf2_broadcaster = learning_tf2_py.static_turtle_tf2_broadcaster:main',
    'turtle_tf2_broadcaster = learning_tf2_py.turtle_tf2_broadcaster:main',    
    'scan_tf2_static_broadcaster.py = learning_tf2_py.scan_tf2_static_broadcaster:main'    
        ],
    },
)
