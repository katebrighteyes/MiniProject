#!/usr/bin/env python

import rospy, time
from std_msgs.msg import Int32MultiArray
from ackermann_msgs.msg import AckermannDriveStamped
from darknet_ros_msgs.msg import BoundingBoxes

steering_pub = None
box_data = None

def init_node():
    global steering_pub
    rospy.init_node('steering')
    rospy.Subscriber('/darknet_ros/bounding_boxes', BoundingBoxes, callback)
    #steering_pub = rospy.Publisher('xycar_motor_msg', Int32MultiArray, queue_size=1)
    steering_pub = rospy.Publisher('/ackermann_cmd_mux/input/teleop', AckermannDriveStamped, queue_size=1)
    
def exit_node():
    print('finished')

#def drive(angle, speed):
#    global steering_pub
#    drive_info = [angle, speed]
#    pub_data = Int32MultiArray(data=drive_info)
#    steering_pub.publish(pub_data)

def drive(angle, speed):
    global steering_pub
    angle = (((angle + 50) * 0.68) - 34) * -0.01
    ack_msg = AckermannDriveStamped()
    ack_msg.header.stamp = rospy.Time.now()
    ack_msg.header.frame_id = ''
    ack_msg.drive.steering_angle = angle
    ack_msg.drive.speed = speed
    steering_pub.publish(ack_msg)

def callback(data):
    global box_data
    box_data = data

if __name__ == '__main__':
    init_node()
    time.sleep(3)
    rate = rospy.Rate(10)

    while box_data is None:
        rate.sleep()

    while not rospy.is_shutdown():
        nobody = True
	boxes = box_data
        for i in range(len(boxes.bounding_boxes)):
            if boxes.bounding_boxes[i].Class == "person":
                nobody = False
                center = (boxes.bounding_boxes[i].xmax
			+ boxes.bounding_boxes[i].xmin)/2
		angle = 0.2*(center - 320)
		drive(angle, 1)
        if nobody:
            drive(0, 0)
	    print "stop"

    rospy.on_shutdown(exit_node)


