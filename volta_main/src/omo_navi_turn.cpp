#include <ros/ros.h>
#include "move_base_msgs/MoveBaseResult.h"
#include "move_base_msgs/MoveBaseActionGoal.h"
#include "move_base_msgs/MoveBaseActionResult.h"
#include "actionlib_msgs/GoalStatus.h"
#include "std_srvs/Empty.h"

int goal_number = 0;
int status_number = -1;
int a = 0;

ros::Publisher* goal_pose;
move_base_msgs::MoveBaseActionGoal goal_pose_robot;

std_srvs::Empty srv;


void Navi_start();

void result_callback(const move_base_msgs::MoveBaseActionResult &reach)
{
	status_number = reach.status.status;
	a =-1;

	// if(status_number == 3)
	// {
	// 	Navi_start();
	// 	status_number = 0;
	// }

}


void Navi_start()
{
	
	if(goal_number == 0)
	{
		goal_pose_robot.goal.target_pose.header.frame_id = "map";
		goal_pose_robot.goal.target_pose.pose.position.x = 29.0087;
		goal_pose_robot.goal.target_pose.pose.position.y = 4.1428;
		goal_pose_robot.goal.target_pose.pose.orientation.z = 0.7150;
		goal_pose_robot.goal.target_pose.pose.orientation.w = 0.6990;

		ROS_INFO("Goal number 0");
	}
	else if(goal_number == 1)
	{
		goal_pose_robot.goal.target_pose.header.frame_id = "map";
		goal_pose_robot.goal.target_pose.pose.position.x = 20.2498;
		goal_pose_robot.goal.target_pose.pose.position.y = 17.8649;
		goal_pose_robot.goal.target_pose.pose.orientation.z = 0.9999;
		goal_pose_robot.goal.target_pose.pose.orientation.w = 0.0124;

	}
	else if(goal_number == 2)
	{
		goal_pose_robot.goal.target_pose.header.frame_id = "map";
		goal_pose_robot.goal.target_pose.pose.position.x = -0.8466;
		goal_pose_robot.goal.target_pose.pose.position.y = 14.9322;
		goal_pose_robot.goal.target_pose.pose.orientation.z = -0.6891;
		goal_pose_robot.goal.target_pose.pose.orientation.w = 0.7292;

	}
        else if(goal_number == 3)
	{
		goal_pose_robot.goal.target_pose.header.frame_id = "map";
		goal_pose_robot.goal.target_pose.pose.position.x = 3.7466;
		goal_pose_robot.goal.target_pose.pose.position.y = -0.2002;
		goal_pose_robot.goal.target_pose.pose.orientation.z = 0.0371;
		goal_pose_robot.goal.target_pose.pose.orientation.w = 0.9992;

	}

}

int main(int argc, char** argv)
{
	int i = 0;

	ros::init(argc, argv, "omo_navi_turn");
	ros::NodeHandle nh;

	ros::Publisher goal_pose_pub = nh.advertise<move_base_msgs::MoveBaseActionGoal>("/move_base/goal", 10);
	goal_pose = &goal_pose_pub;

	ros::Subscriber result_sub = nh.subscribe("/move_base/result", 10, result_callback);

	ros::ServiceClient clear_costmaps_client = nh.serviceClient<std_srvs::Empty>("/move_base/clear_costmaps"); 

	ros::Rate loop_rate(10);

	ROS_INFO("ROS START");
	ROS_INFO("NAVIGATION START 1ST");

	//status_number = 3;

	while(ros::ok())
	{
		if(a==0)
		{				
			Navi_start();
			goal_pose -> publish(goal_pose_robot);
			
		} 


		i++;
		if(i%50 == 0)
		{
			if(clear_costmaps_client.call(srv))
			{
				ROS_INFO("CLEAR COSTMAP");
			}
		}

		if(status_number == 3)
		{
			if(clear_costmaps_client.call(srv))
			{
				ROS_INFO("CLEAR COSTMAP");
			}
			


			if (goal_number == 0)
			{
				goal_number = 1;
			}
			else if (goal_number == 1)
			{
				goal_number = 2;
			}
			else if (goal_number == 2)
			{
				goal_number = 3;
			}
			else if (goal_number == 3)
			{
				goal_number = 0;
			}


			Navi_start();
			goal_pose -> publish(goal_pose_robot);
			ROS_INFO("NAVIGATION START");
			status_number = 0;
			
		}
		

		ros::spinOnce();
		loop_rate.sleep();

	}

}
