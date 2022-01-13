#include "obstacle_warn.h"

ros::Publisher warn_pub;

void closest_point(const sensor_msgs::PointCloud2ConstPtr &cloud_msg)
{
    pcl::PointCloud<pcl::PointXYZRGB> zed_cloud;
    pcl::fromROSMsg(*cloud_msg, zed_cloud);

    pcl::PointXYZRGB Point;

    for(size_t i = 0; i < zed_cloud.points.size(); i++)
    {
        Point = zed_cloud.points[i];

        if(Point.x < 0.35)
        {
            std_msgs::String warning_str;
            std::stringstream ss;

            ss << "OBSTACULO\n" << Point.x << "\n" << Point.y << "\n" << Point.z;
            warning_str.data = ss.str();

            warn_pub.publish(warning_str);
            return;
        }
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "obstacle_warn");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe<sensor_msgs::PointCloud2>("/zed2/zed_node/point_cloud/cloud_registered", 1, closest_point);

    warn_pub = nh.advertise<std_msgs::String>("/warning", 1);

    ros::spin();
}
