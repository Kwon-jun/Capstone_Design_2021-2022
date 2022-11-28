#include <ros.h>
#include <SoftwareSerial.h>
#include <std_msgs/Int8.h>
#include <std_msgs/Bool.h>
#include <std_msgs/UInt8.h>
#include <std_msgs/UInt16.h>

char marker[3] = "0";
char park[3] = "0";
int i = 0;


int left_led = 2;
int right_led = 3;
int red_led = 4;

void marker_num();
void park_num();


ros::NodeHandle  nh;

void turn_led_callback(const std_msgs::Int8 &turn_led)
{
  if(turn_led.data == 1)  // left led on
  {
    digitalWrite(2, HIGH);
  }
  else if(turn_led.data == 2) // right led on
  {
    digitalWrite(3, HIGH);
  }
  else
  {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
  }
}

void stop_led_callback(const std_msgs::Bool &stop_led)
{
  if(stop_led.data == 1)
  {
    digitalWrite(4, HIGH);
  }
  else
  {
    digitalWrite(4, LOW);
  }
}

ros::Subscriber<std_msgs::Int8> sub_turn("/turn_led_signal", &turn_led_callback);
ros::Subscriber<std_msgs::Bool> sub_stop("/stop_led_signal", &stop_led_callback);

std_msgs::UInt8 pub_mark;
ros::Publisher pub_marker("/marker_number", &pub_mark);

std_msgs::UInt16 pub_park;
ros::Publisher pub_parking("/park_number", &pub_park);

void setup()
{
  nh.initNode();
  nh.subscribe(sub_turn);
  nh.subscribe(sub_stop);
  nh.advertise(pub_marker);
  nh.advertise(pub_parking);

  Serial.begin(9600);
  Serial1.begin(9600);         //Serial1 == 18 19번핀, Serial2 == 16 17번핀, Serial3 == 14 15번핀
                                //          Rx, Tx

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);

}

void loop()
{ 
  if (Serial1.available())      // receive
  {
    //delay(5);
    //Serial.write(Serial1.read());
    //Serial.println("receb");

    if(i<3)
    {
      marker[i] = Serial1.read();
    }
    else
    {
      park[i-3] = Serial1.read();
      
      if(i==5)
      {
        Serial.println(marker);
        //marker_num();

        if(marker[2] == '1')
        {
          //Serial.println("1");
          pub_mark.data = 1;
        }
        if(marker[2] == '2')
        {
          pub_mark.data = 2; 
        }
        if(marker[2] == '3')
        {
          pub_mark.data = 3; 
        }
        if(marker[2] == '4')
        {
          pub_mark.data = 4; 
        }
        if(marker[2] == '5')
        {
          pub_mark.data = 5; 
        }
        if(marker[2] == '6')
        {
          pub_mark.data = 6; 
        }
        if(marker[2] == '7')
        {
          pub_mark.data = 7; 
        }
        if(marker[2] == '8')
        {
          pub_mark.data = 8; 
        }
        if(marker[2] == '9')
        {
          pub_mark.data = 9;
        }
        if(marker[2] == '0')
        {
          pub_mark.data = 0;
        }

        pub_marker.publish(&pub_mark);

      
        
        park[3] = '\0';
        Serial.println(park);


        if(park[0] == 'A')
        {
          if(park[2] == '1')
          {
            pub_park.data = 1;
          }
          if(park[2] == '2')
          {
            pub_park.data = 2;
          }
          if(park[2] == '3')
          {
            pub_park.data = 3;
          }
          if(park[2] == '4')
          {
            pub_park.data = 4;
          }
        }
        else if(park[0] == 'B')
        {
          if(park[2] == '1')
          {
            pub_park.data = 5;
          }
          if(park[2] == '2')
          {
            pub_park.data = 6;
          }
          if(park[2] == '3')
          {
            pub_park.data = 7;
          }
          if(park[2] == '4')
          {
            pub_park.data = 8;
          }
        }

        pub_parking.publish(&pub_park);
        //park_num();
        
        i=-1;
      }
      
    }
     
    i++;
  }

  nh.spinOnce();
}
