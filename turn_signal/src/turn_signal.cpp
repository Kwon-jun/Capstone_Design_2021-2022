#include <ros/ros.h>
#include <math.h>
#include "move_base_msgs/MoveBaseResult.h"
#include "move_base_msgs/MoveBaseActionGoal.h"
#include "move_base_msgs/MoveBaseActionResult.h"
#include "move_base_msgs/MoveBaseActionFeedback.h"
#include "actionlib_msgs/GoalStatus.h"
#include "std_srvs/Empty.h"
#include "std_msgs/Int8.h"
#include "std_msgs/Bool.h"
#include "geometry_msgs/Twist.h"

ros::Publisher* turn_led;
std_msgs::Int8 turn_led_sign;

ros::Publisher* stop_led;
std_msgs::Bool stop_led_sign;

float x_now = 0;
float y_now = 0;
float z_now = 0;
float w_now = 0;

float x_goal = 0;
float y_goal = 0;
float z_goal = 0;
float w_goal = 0;

int i = 0;
bool left_led = 0;
bool right_led = 0;

void Left_Blink();
void Right_Blink();




void feedback_callback(const move_base_msgs::MoveBaseActionFeedback &current_pos)
{
    x_now = current_pos.feedback.base_position.pose.position.x;
    y_now = current_pos.feedback.base_position.pose.position.y;
    z_now = current_pos.feedback.base_position.pose.orientation.z;
    w_now = current_pos.feedback.base_position.pose.orientation.w;

}

void goal_callback(const move_base_msgs::MoveBaseActionGoal &goal_pos)
{
    x_goal = goal_pos.goal.target_pose.pose.position.x;
    y_goal = goal_pos.goal.target_pose.pose.position.y;
    z_goal = goal_pos.goal.target_pose.pose.orientation.z;
    w_goal = goal_pos.goal.target_pose.pose.orientation.w;

}

void vel_callback(const geometry_msgs::Twist &vel)
{
    if(vel.linear.x < 0.00)
    {
        if(i%10 == 0)
        {
            turn_led_sign.data = 1;
            turn_led_sign.data = 2;
        }
        else if(i%10 ==5)
        {
            turn_led_sign.data = 0;
            turn_led_sign.data = 0;
        }
        
    }
    else if(vel.linear.x < 0.01)
    {
        stop_led_sign.data = 1;
        
    }
    else
    {
        stop_led_sign.data = 0;
    }

    

}


void Left_Blink()
{
    if (i%10 == 0)
    {
        turn_led_sign.data = 1;     //left led on
        // ROS_INFO("      <<");
        // ROS_INFO("    <<  ");
        // ROS_INFO("  <<    ");
        // ROS_INFO("<<      ");
        // ROS_INFO("  <<    ");
        // ROS_INFO("    <<  ");
        // ROS_INFO("      <<");
    }
    else if (i%10 == 5)
    {
        turn_led_sign.data = 0;     //led off
        // ROS_INFO("--------");
        // ROS_INFO("--------");
        // ROS_INFO("--------");
        // ROS_INFO("--------");
        // ROS_INFO("--------");
        // ROS_INFO("--------");
        // ROS_INFO("--------");
    }
}


void Right_Blink()
{   
    if (i%10 == 0)
    {
        turn_led_sign.data = 2;     //right led on
        // ROS_INFO(">>      ");
        // ROS_INFO("  >>    ");
        // ROS_INFO("    >>  ");
        // ROS_INFO("      >>");
        // ROS_INFO("    >>  ");
        // ROS_INFO("  >>    ");
        // ROS_INFO(">>      ");
    }
    else if (i%10 == 5)
    {
        turn_led_sign.data = 0;     //led off
        // ROS_INFO("--------");
        // ROS_INFO("--------");
        // ROS_INFO("--------");
        // ROS_INFO("--------");
        // ROS_INFO("--------");
        // ROS_INFO("--------");
        // ROS_INFO("--------");
    }
}




int main(int argc, char** argv)
{
    float goal_distance = 0;

	ros::init(argc, argv, "turn_signal");
	ros::NodeHandle nh;

	// ros::Publisher goal_pose_pub = nh.advertise<move_base_msgs::MoveBaseActionGoal>("/move_base/goal", 10);
	// goal_pose = &goal_pose_pub;

	// ros::Subscriber result_sub = nh.subscribe("/move_base/result", 10, result_callback);
	// ros::ServiceClient clear_costmaps_client = nh.serviceClient<std_srvs::Empty>("/move_base/clear_costmaps"); 

    ros::Publisher turn_led_pub = nh.advertise<std_msgs::Int8>("/turn_led_signal", 10);
    turn_led = &turn_led_pub;
    ros::Publisher stop_led_pub = nh.advertise<std_msgs::Bool>("/stop_led_signal", 10);
    stop_led = &stop_led_pub;


    ros::Subscriber feedback_sub = nh.subscribe("/move_base/feedback", 10, feedback_callback);
    ros::Subscriber goal_sub = nh.subscribe("/move_base/goal", 10, goal_callback);
    ros::Subscriber vel_sub = nh.subscribe("/cmd_vel", 10, vel_callback);

    turn_led_sign.data = 0;


	ros::Rate loop_rate(10);

	while(ros::ok())
	{
        i++;

        goal_distance = sqrt((x_goal-x_now)*(x_goal-x_now) + (y_goal-y_now)*(y_goal-y_now));

        if(goal_distance < 10 && goal_distance > 1)   // boundary distance
        {
        
            if ((z_now > -0.1 && z_now < 0.1) && (w_now > 0.9))     // front dir
            {
                if (y_now < y_goal)     // left turn
                {
                    left_led = 1;
                }
                else if (y_now > y_goal)  // right turn
                {
                    right_led = 1;
                }
            }
            else if ((z_now > 0.6 && z_now < 0.8) && (w_now > 0.6 && w_now < 0.8))   // left dir
            {
                if (x_now < x_goal)         // right turn
                {
                    right_led = 1;
                }
                else if (x_now > x_goal)    // left turn
                {
                    left_led = 1;
                }

            }
            else if ((z_now > -0.8 && z_now < -0.6) && (w_now > 0.6 && w_now < 0.8))    // right dir
            {
                if (x_now < x_goal)         // left turn
                {
                    left_led = 1;
                }
                else if (x_now > x_goal)    // right turn
                {
                    right_led = 1;
                } 
            }
            else if ((z_now > 0.9) && (w_now > -0.1 && w_now < 0.1))    // back dir
            {
                if (y_now < y_goal)         // right turn
                {
                    right_led = 1;
                }
                else if (y_now > y_goal)    // left turn
                {
                    left_led = 1;
                }
            }


        }
        else    // led off
        {
            left_led = 0;
            right_led = 0;

            turn_led_sign.data = 0;
            //ROS_INFO("LED ALL OFF");
        }

        if(left_led == 1)           // left led
        {
            Left_Blink();
        }
        else if(right_led == 1)     // right led
        {
            Right_Blink();
        }

        turn_led -> publish(turn_led_sign);
        stop_led -> publish(stop_led_sign);
		

		ros::spinOnce();
		loop_rate.sleep();

	}

}
