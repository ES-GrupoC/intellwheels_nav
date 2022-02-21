/*
Saves 100 images and a Point Clouds at the same time instant. Used to acquire images for the plane calibration.
*/

#include "img_pc_acquisition.h"

int count = 0;

void callback(const sensor_msgs::PointCloud2ConstPtr &cloud_msg, const sensor_msgs::ImageConstPtr& msg)
{
    if (count < 100)
    {
        std::stringstream ss, ss2;
        std::string path;

        cv::Mat img_left = cv_bridge::toCvShare(msg, "bgr8")->image;

        ss << "imagem_captura_" << count << ".jpg";
        ss >> path;

        cv::imwrite(path, img_left);

        pcl::PointCloud<pcl::PointXYZRGB> zed_cloud;
        pcl::fromROSMsg(*cloud_msg, zed_cloud);

        ss2 << "pointcloud_" << count << ".pcd";

        pcl::io::savePCDFileASCII (ss2.str(), zed_cloud);

        count++;
    }

    return;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "img_pc_acquisition");
    ros::NodeHandle nh;

    message_filters::Subscriber<sensor_msgs::PointCloud2> point_cloud(nh, "/zed2/zed_node/point_cloud/cloud_registered", 1);
    message_filters::Subscriber<sensor_msgs::Image> image_left(nh, "/zed2/zed_node/rgb/image_rect_color", 1);


    typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::PointCloud2, sensor_msgs::Image> MySyncPolicy;
    message_filters::Synchronizer<MySyncPolicy> img_sync(MySyncPolicy(10), point_cloud, image_left);
    img_sync.registerCallback(boost::bind(&callback, _1, _2));

    ros::spin();
}
