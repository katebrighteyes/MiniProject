
#include "ros/ros.h"
#include "darknet_ros_msgs/BoundingBoxes.h"
#include "darknet_ros_msgs/BoundingBox.h"
#include "sensor_msgs/LaserScan.h"
#include "acc_logic_ros/MsgACC.h" 
#include <pthread.h>
#include <math.h>

#define RAD2DEG(x) ((x)*180./M_PI)

using namespace std;

#define REQ_CUR 0
#define REQ_ACCEL 1
#define REQ_DECEL 2
#define REQ_STOP 3

string target = "person";
bool detect_target = false;
int req_num = REQ_CUR; //0: current 1: accel 2 : decel 3 : stop
//acc_logic_ros::MsgACC msg;
ros::Publisher req_pub;
darknet_ros_msgs::BoundingBox detect_box;
darknet_ros_msgs::BoundingBox detect_person;

void msgCallback(const darknet_ros_msgs::BoundingBoxes::ConstPtr& msg)
{
    //cout<<"Bouding Boxes (Class):" << msg->bounding_boxes[0].Class <<endl;
    if(msg->bounding_boxes[0].Class.compare(target) == 0) {
	//cout<<">>>>TARGET (Class):" << msg->bounding_boxes[0].Class <<endl;
        detect_target = true;
	memcpy((void*)&detect_box, (void*)&msg->bounding_boxes[0], 
             sizeof(darknet_ros_msgs::BoundingBoxes));

    } else {
        detect_target = false;
    }
}


void scanCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
        int index = 89;
	float range = msg->ranges[index];
	//ROS_INFO("range = %lf :\n",msg->ranges[index]);
	req_num = REQ_CUR;
	
	if((detect_target==true) && (range <= 0.6))
	{
		req_num = REQ_STOP;
		float xpos = detect_box.xmin +
		            ((detect_box.xmax - detect_box.xmin)/2);
		float ypos = detect_box.ymax;
		float angle1 = atan2(xpos, ypos);
		ROS_INFO("range = %0.2f, x(%0.2f)y(%0.2f)a(%0.2f) : Emergency! STOP!\n",
                    msg->ranges[index], xpos, ypos, angle1);
	        //memcpy((void*)&detect_person, (void*)&detect_box, 
                 //sizeof(darknet_ros_msgs::BoundingBoxes));

	}
        else 
        {
           req_num = REQ_CUR;
           //memset((void*)&detect_person, 0x00, sizeof(darknet_ros_msgs::BoundingBoxes));
        }
}

void *test(void *data)
{
  ros::Rate loop_rate(20);
  while (ros::ok())
  {
    if(req_num == REQ_STOP) 
    {
      ROS_INFO(">> TOPIC Emergency! STOP!\n");
      memcpy((void*)&detect_person, (void*)&detect_box, 
                 sizeof(darknet_ros_msgs::BoundingBoxes));
    } else {
       detect_person.Class = "None";
       memset((void*)&detect_person, 0x00, sizeof(darknet_ros_msgs::BoundingBoxes));
    }    
    req_pub.publish(detect_person);        
	
    loop_rate.sleep();                  

  }
}


int main(int argc, char **argv)
{
    cout<<"acc_logic_node2" <<endl;
#if 0
    ros::init(argc, argv, "acc_logic_node2");
    ros::NodeHandle nh;
    ros::Subscriber obj_sub = nh.subscribe("/darknet_ros/bounding_boxes",100,msgCallback);

	ros::spin();
#endif
#if 1

    pthread_t thread_t;
    int status;

    cout<<"acc_logic_node2" <<endl;

    ros::init(argc, argv, "acc_logic_node2");
    ros::NodeHandle nh;
    ros::Subscriber obj_sub = nh.subscribe("/darknet_ros/bounding_boxes",100,msgCallback);
    ros::Subscriber scan_sub = nh.subscribe<sensor_msgs::LaserScan>("/scan", 1000, scanCallback);

	
    //req_pub = nh.advertise<acc_logic_ros::MsgACC>("acc_logic_msg", 100);
    req_pub = nh.advertise<darknet_ros_msgs::BoundingBox>("detect_obj", 100);

	
    if (pthread_create(&thread_t, NULL, test, 0) < 0)
    {
        printf("thread create error:");
        exit(0);
    }


    ros::spin();

    pthread_join(thread_t, (void **)&status);
    printf("Thread End %d\n", status);
#endif

	return 0;
}

