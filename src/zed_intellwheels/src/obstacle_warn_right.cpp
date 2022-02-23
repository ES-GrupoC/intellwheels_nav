/*
Warns if an obstacle appears. Receives as parameters the plane parameters a, b, c and d (ax+by+cz+d=0) and the distance to that plane that is considered to be an obstacle (e.g. 0.1 meters)
*/

#include "obstacle_warn.h"

ros::Publisher warn_pub;

struct plane{
    double a, b, c, d;
};

plane floor_plane;
double min_dist;

void closest_point(const sensor_msgs::PointCloud2ConstPtr &cloud_msg)
{
    int count = 0;
    pcl::PointCloud<pcl::PointXYZRGB> zed_cloud;
    pcl::fromROSMsg(*cloud_msg, zed_cloud);

    pcl::PointXYZRGB Point;

    //std::cout << floor_plane.a << "\n" << floor_plane.b << "\n" << floor_plane.c << "\n" << floor_plane.d << "\n";
    //std::cout << min_dist;

    for(size_t i = 0; i < zed_cloud.points.size(); i++)
    {
        //std::cout << "CICLO\n";

        Point = zed_cloud.points[i];

        if(Point.x < 0.5f && Point.x > 0.4f && Point.z < 0.02f && Point.z > -0.5f)
        {
            double distance = std::abs(floor_plane.a*Point.x + floor_plane.b*Point.y + floor_plane.c*Point.z + floor_plane.d)/
                              std::sqrt(floor_plane.a*floor_plane.a + floor_plane.b*floor_plane.b + floor_plane.c*floor_plane.c);

            //std::cout << distance << "\n";

            if (distance > min_dist)
            {
                count++;

                if (count >= 10)
                {
                    std_msgs::String warning_str;
                    std::stringstream ss;

                    ss << "OBSTACULO\n" << Point.x << "\n" << Point.y << "\n" << Point.z << "\n";
                    warning_str.data = ss.str();

                    std::cout << ss.str();

                    warn_pub.publish(warning_str);
                    return;
                }
            }
        }
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "obstacle_warn_right");
    ros::NodeHandle nh;
    ros::NodeHandle n_private("~");

    ros::Subscriber sub = nh.subscribe<sensor_msgs::PointCloud2>("/zed2/zed_node_right/point_cloud/cloud_registered", 1, closest_point);

    n_private.getParam("plane_a", floor_plane.a);
    n_private.getParam("plane_b", floor_plane.b);
    n_private.getParam("plane_c", floor_plane.c);
    n_private.getParam("plane_c", floor_plane.d);
    n_private.getParam("min_dist", min_dist);
    /*n_private.param<double>("plane_a", floor_plane.a); //-0.056794737
    n_private.param<double>("plane_b", floor_plane.b, -0.942573684);
    n_private.param<double>("plane_c", floor_plane.c, 0.328961579);
    n_private.param<double>("plane_d", floor_plane.d, -0.587768421);
    n_private.param<double>("min_dist", min_dist, 0.1);
    */
    warn_pub = nh.advertise<std_msgs::String>("/warning", 1);

    ros::spin();
}
