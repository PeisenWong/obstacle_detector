#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"

void scanCallBack(const sensor_msgs::LaserScan msg)
{
    ROS_INFO("I heard %s", msg.header.frame_id);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "listener");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("scan", 1000, scanCallBack);
}
