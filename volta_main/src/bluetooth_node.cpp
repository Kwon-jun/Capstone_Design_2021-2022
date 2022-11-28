#include <ros/ros.h>

#include "volta_main/blue_tooth.h"
#include "std_msgs/UInt8.h"
#include "std_msgs/UInt16.h"

bool a = false;

void mark_callback(const std_msgs::UInt8 &mark)
{
  //ROS_INFO("%d", mark.data);
}

void park_callback(const std_msgs::UInt16 &park)
{
  //ROS_INFO("%d", park.data);
  a = true;
}


bool blue_num(volta_main::blue_tooth::Request &req, volta_main::blue_tooth::Response &res)
{
  if(a == true)
  {
    res.blue_tooth_ok = true;
  }
  else 
  {
    res.blue_tooth_ok = false;
  }
  a = false;

}




int main(int argc, char** argv)
{
	//int i = 0;

	ros::init(argc, argv, "bluetooth_node");
	ros::NodeHandle nh;

	
	ros::Subscriber marker_sub = nh.subscribe("/marker_number", 10, mark_callback);
	ros::Subscriber parking_sub = nh.subscribe("/park_number", 10, park_callback);
	ros::ServiceServer bluetooth_server = nh.advertiseService("/blue_srv", blue_num);



	ros::Rate loop_rate(10);

	while(ros::ok())
	{
		

		ros::spinOnce();
		loop_rate.sleep();

	}

}
