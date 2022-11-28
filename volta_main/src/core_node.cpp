#include <ros/ros.h>

#include "volta_main/navi.h"
#include "volta_main/blue_tooth.h"
#include "docking/docking.h"
#include "volta_main/return_navi.h"
#include "docking/door.h"
#include "docking/Init.h"
#include "docking/ppp.h"
#include "robot_msgs/omoalign.h"
#include "robot_msgs/omoapproach.h"
#include "std_srvs/Empty.h"
#include "geometry_msgs/Twist.h"


ros::ServiceClient *blth;
ros::ServiceClient *nav;
ros::ServiceClient *dock;
ros::ServiceClient *re_nav;
ros::ServiceClient *door;
ros::ServiceClient *align;
ros::ServiceClient *approach;
ros::ServiceClient *dock_init;
ros::ServiceClient *p3;

volta_main::blue_tooth blth_srv;
volta_main::navi nav_srv;
volta_main::return_navi re_nav_srv;

docking::docking dock_srv;
docking::door door_srv;
docking::Init Init_srv;
docking::ppp ppp_srv;

robot_msgs::omoalign align_srv;
robot_msgs::omoapproach approach_srv;



ros::Publisher* vel_pub;
geometry_msgs::Twist vel;


int mode = 0;
int marker_num = 0;
int i = 0;
bool charge = 0;
bool a = 0;
bool door_state = 0;

void blth_mode();
void nav_mode();
void dock_mode();
void re_nav_mode();
void open_door();
void align_mode();
void align_mode2();
void robot_approach();
void robot_leave();
void close_door();
void turn_robot();
void docking_init();
void robot_turn();


int main(int argc, char **argv)
{
    ros::init(argc, argv, "core_node");
	ros::NodeHandle nh;

	// ros::Publisher goal_pose_pub = nh.advertise<move_base_msgs::MoveBaseActionGoal>("/move_base/goal", 10);
	// goal_pose = &goal_pose_pub;

	// ros::Subscriber result_sub = nh.subscribe("/move_base/result", 10, result_callback);

	// ros::ServiceClient clear_costmaps_client = nh.serviceClient<std_srvs::Empty>("/move_base/clear_costmaps"); 
    ros::ServiceClient blue_tooth_client = nh.serviceClient<volta_main::blue_tooth>("/blue_srv");
    ros::ServiceClient navi_client = nh.serviceClient<volta_main::navi>("/navi_srv"); 
    ros::ServiceClient docking_client = nh.serviceClient<docking::docking>("/docking");
    ros::ServiceClient return_navi_client = nh.serviceClient<volta_main::return_navi>("/re_navi_srv");
    ros::ServiceClient door_client = nh.serviceClient<docking::door>("/door");
    ros::ServiceClient init_client = nh.serviceClient<docking::Init>("/Init");
    ros::ServiceClient ppp_client = nh.serviceClient<docking::ppp>("/PPP");
    ros::ServiceClient omoalign_client = nh.serviceClient<robot_msgs::omoalign>("/omo_con_srv");
    ros::ServiceClient omoapproach_client = nh.serviceClient<robot_msgs::omoapproach>("/omo_approach");

    ros::Publisher cmd_vel_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    vel_pub = &cmd_vel_pub;


    blth = &blue_tooth_client;
    nav = &navi_client;
    dock = &docking_client;
    re_nav = &return_navi_client;
    door = &door_client;
    align = &omoalign_client;
    approach = &omoapproach_client;
    dock_init = &init_client;
    p3 = &ppp_client;


    ros::Rate loop_rate(10);

    while(ros::ok())
    {
        
        if(mode == 0)		// bluetooth mode
        {
            ROS_INFO("Waiting blue tooth...");
            blth_mode();
		}
        else if(mode == 1)	// navigation mode
        {
            ROS_INFO("Start Navigation");
			nav_mode();
        }
	    
        else if(mode == 7)  // dock mode
        {
            ROS_INFO("Dock mode");
            dock_mode();
        }
        else if(mode == 8)  // robot leave
        {
            ROS_INFO("Robot Leave");
            robot_leave();
        }
        else if(mode == 9)  // close door
        {
            ROS_INFO("Close Door");
            close_door();
        }
        else if(mode == 11)
        {
            ROS_INFO("Robot Turn");
            robot_turn();
        }
	    else if(mode == 10)  // return navigation mode
	    {
	        ROS_INFO("Start Return Navigation");
	        re_nav_mode();
	    }

        ros::spinOnce();
	    loop_rate.sleep();
    }

}


void blth_mode()    // 0
{
    if(blth -> call(blth_srv) && blth_srv.response.blue_tooth_ok)
    {
        mode = 1;	// go to navigation mode
        blth_srv.response.blue_tooth_ok = 0;
    }
}

void nav_mode()     // 1
{
    if(nav -> call(nav_srv) && nav_srv.response.navi_ok)
    {
	    mode = 7;
        nav_srv.response.navi_ok = 0;
    }
}


void dock_mode()     // 7
{
    dock_srv.request.docking_start = 1;
    if(dock -> call(dock_srv) && dock_srv.response.done)
    {
        mode = 8;
        dock_srv.response.done =0;
    }
}

void robot_leave()      // 8
{   
    i++;
    if(i==80)  // robot moves 8 seconds
    {
        i = 0;
        vel.linear.x = 0.0;
        vel_pub -> publish(vel);
        mode = 9;
    }
    else
    {
        vel.linear.x = -0.2;
        vel_pub -> publish(vel);
    }
    
}   

void close_door()       // 9
{
    door_srv.request.door_start = 0;
    if(door -> call(door_srv) && door_srv.response.door_done)
    {
	    mode = 11;
        door_srv.response.door_done = 0;
    }
}

void robot_turn()   // 11
{
    i++;
    if(i==60)
    {
        i = 0;
        vel.angular.z = 0.0;
        vel_pub -> publish(vel);
        mode = 10;
    }
    else
    {
        vel.angular.z = 0.2;
        vel_pub -> publish(vel);
    }
}

void re_nav_mode()      // 10
{
    if(re_nav-> call(re_nav_srv) && re_nav_srv.response.return_navi_ok)
    {
	    mode = 0;
        re_nav_srv.response.return_navi_ok = 0;
    }
}


