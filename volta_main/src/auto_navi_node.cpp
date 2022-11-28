#include <ros/ros.h>
#include "move_base_msgs/MoveBaseResult.h"
#include "move_base_msgs/MoveBaseActionGoal.h"
#include "move_base_msgs/MoveBaseActionResult.h"
#include "actionlib_msgs/GoalStatus.h"
#include "std_srvs/Empty.h"
#include "volta_main/navi.h"
#include "volta_main/return_navi.h"
#include "std_msgs/UInt16.h"

int park_number = 0;
int status_number = 0;	// goal status
int goal_number = 0;
bool navi_first = 0;
bool auto_navi_reach = false;
bool auto_navi_start = false;
bool return_auto_navi_reach = false;
bool return_auto_navi_start = false;

ros::Publisher* goal_pose;
move_base_msgs::MoveBaseActionGoal goal_pose_robot;

std_srvs::Empty srv;

void Park_Navi_Set();
void Return_Navi_Set();

void result_callback(const move_base_msgs::MoveBaseActionResult &reach)
{
	status_number = reach.status.status;
}

void park_callback(const std_msgs::UInt16 &park)
{
    //ROS_INFO("park_number = %d", park.data);
    park_number = park.data;
  
}

bool Auto_Navi_Callback(volta_main::navi::Request &req, volta_main::navi::Response &res)
{
    auto_navi_start = true;

    if(auto_navi_reach == true)
    {
        auto_navi_start = false;
        auto_navi_reach = false;
        res.navi_ok = true;
    }
}

bool Return_Auto_Navi_Callback(volta_main::return_navi::Request &req, volta_main::return_navi::Response &res)
{
    return_auto_navi_start = true;

    if(return_auto_navi_reach == true)
    {
        return_auto_navi_start = false;
        return_auto_navi_reach = false;
        res.return_navi_ok = true;
    }
    
}

void Park_Navi_Set()
{
	
	if(park_number == 0)
	{
		ROS_INFO("Park number No data");
	}
	else if(park_number == 1)   // A-1
	{
        ROS_INFO("GO to A-1");
		goal_pose_robot.goal.target_pose.header.frame_id = "map";
		goal_pose_robot.goal.target_pose.pose.position.x = 27.6602;
		goal_pose_robot.goal.target_pose.pose.position.y = 3.9295;
		goal_pose_robot.goal.target_pose.pose.orientation.z = 0.9999;
		goal_pose_robot.goal.target_pose.pose.orientation.w = 0.0009;
	}
	else if(park_number == 2)   // A-2
	{
        ROS_INFO("GO to A-2");
		goal_pose_robot.goal.target_pose.header.frame_id = "map";
		goal_pose_robot.goal.target_pose.pose.position.x = -0.8466;
		goal_pose_robot.goal.target_pose.pose.position.y = 14.9322;
		goal_pose_robot.goal.target_pose.pose.orientation.z = -0.6891;
		goal_pose_robot.goal.target_pose.pose.orientation.w = 0.7292;
	}
    else if(park_number == 3)   // A-3
	{
        ROS_INFO("GO to A-3");
		goal_pose_robot.goal.target_pose.header.frame_id = "map";
		goal_pose_robot.goal.target_pose.pose.position.x = 3.7466;
		goal_pose_robot.goal.target_pose.pose.position.y = -0.2002;
		goal_pose_robot.goal.target_pose.pose.orientation.z = 0.0371;
		goal_pose_robot.goal.target_pose.pose.orientation.w = 0.9992;
	}
    else if(park_number == 4)   // A-4
	{
        ROS_INFO("GO to A-4");
		goal_pose_robot.goal.target_pose.header.frame_id = "map";
		goal_pose_robot.goal.target_pose.pose.position.x = 3.7466;
		goal_pose_robot.goal.target_pose.pose.position.y = -0.2002;
		goal_pose_robot.goal.target_pose.pose.orientation.z = 0.0371;
		goal_pose_robot.goal.target_pose.pose.orientation.w = 0.9992;
	}

}

void Return_Navi_Set()
{
	if(goal_number == 1)
	{
		goal_pose_robot.goal.target_pose.header.frame_id = "map";
		goal_pose_robot.goal.target_pose.pose.position.x = 23.3490;
		goal_pose_robot.goal.target_pose.pose.position.y = 0.1996;
		goal_pose_robot.goal.target_pose.pose.orientation.z = 0.9999;
		goal_pose_robot.goal.target_pose.pose.orientation.w = 0.0012;
	}
	else if(goal_number == 2)
	{
		goal_pose_robot.goal.target_pose.header.frame_id = "map";
		goal_pose_robot.goal.target_pose.pose.position.x = -0.0854;
		goal_pose_robot.goal.target_pose.pose.position.y = -3.0549;
		goal_pose_robot.goal.target_pose.pose.orientation.z = 0.7084;
		goal_pose_robot.goal.target_pose.pose.orientation.w = 0.7058;
	}
    /*
	else if(goal_number == 3)
	{
		goal_pose_robot.goal.target_pose.header.frame_id = "map";
		goal_pose_robot.goal.target_pose.pose.position.x = 3.7466;
		goal_pose_robot.goal.target_pose.pose.position.y = -0.2002;
		goal_pose_robot.goal.target_pose.pose.orientation.z = 0.0371;
		goal_pose_robot.goal.target_pose.pose.orientation.w = 0.9992;
	}
	*/
}

int main(int argc, char** argv)
{
	int i = 0;

	ros::init(argc, argv, "auto_navi_node");
	ros::NodeHandle nh;

	ros::Publisher goal_pose_pub = nh.advertise<move_base_msgs::MoveBaseActionGoal>("/move_base/goal", 10);
	goal_pose = &goal_pose_pub;

	ros::Subscriber result_sub = nh.subscribe("/move_base/result", 10, result_callback);
    ros::Subscriber parking_sub = nh.subscribe("/park_number", 10, park_callback);

	ros::ServiceClient clear_costmaps_client = nh.serviceClient<std_srvs::Empty>("/move_base/clear_costmaps"); 
    ros::ServiceServer navi_server = nh.advertiseService("/navi_srv", Auto_Navi_Callback);
	ros::ServiceServer return_navi_server = nh.advertiseService("/re_navi_srv", Return_Auto_Navi_Callback);

	ros::Rate loop_rate(5);

	while(ros::ok())
	{
		i++;
		if(i%25 == 0)   // clear costmap every 5 seconds
		{
			if(clear_costmaps_client.call(srv))
			{
				ROS_INFO("CLEAR COSTMAP");
			}
		}

        if(auto_navi_start == true)		// navigation mode
        {
			/*
			if(navi_first == 0)
			{
				goal_pose_robot.goal.target_pose.header.frame_id = "map";
				goal_pose_robot.goal.target_pose.pose.position.x = 3.8602;
				goal_pose_robot.goal.target_pose.pose.position.y = -0.2144;
				goal_pose_robot.goal.target_pose.pose.orientation.z = 0.0002;
				goal_pose_robot.goal.target_pose.pose.orientation.w = 0.9999;

				goal_pose -> publish(goal_pose_robot);

				if(status_number == 3)
				{
					navi_first = 1;
					status_number = 0;
				}
			}
			*/
			
			
			Park_Navi_Set();
            goal_pose -> publish(goal_pose_robot);

			if(status_number == 3)  // clear costmap and response when robot reached
			{
				if(clear_costmaps_client.call(srv))
				{
					ROS_INFO("CLEAR COSTMAP");
				}
				status_number = 0;
				navi_first = 0;
        		auto_navi_reach = true;
			}

			

            
        }
		else if(return_auto_navi_start == true)		// return navigation mode
		{
			if(goal_number == 0)
			{
				status_number = 3;
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
				/*
				else if (goal_number == 2)
				{
					goal_number = 3;
				}
				*/
				else if (goal_number == 2)
				{
					return_auto_navi_reach = true;
				}

				Return_Navi_Set();
				goal_pose -> publish(goal_pose_robot);
				status_number = 0;
			}
		}
		ros::spinOnce();
		loop_rate.sleep();
	}
}
