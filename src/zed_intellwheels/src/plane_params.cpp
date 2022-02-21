/*
Reads .pcd files and calculates the parameters of a plane. It sums the parameters, so that the vector can be normalized.
*/

#include "obstacle_warn.h"

ros::Publisher warn_pub;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "plane_params");
    ros::NodeHandle nh;

    std::vector<float> sum = {0, 0, 0, 0};
    int num_pc = 0;

    //std::string path = "/home";
    //for(const auto & entry : std::filesystem::directory_iterator(path))
    for(int i = 0; i <=0; i++)
    {
        std::stringstream ss;
        ss << "pointcloud_" << i << ".pcd";

        pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZRGB>);

        pcl::io::loadPCDFile<pcl::PointXYZRGB> (ss.str(), *cloud);

        pcl::ModelCoefficients::Ptr coefficients (new pcl::ModelCoefficients);
        pcl::PointIndices::Ptr inliers (new pcl::PointIndices);
        // Create the segmentation object
        pcl::SACSegmentation<pcl::PointXYZRGB> seg;
        // Optional
        seg.setOptimizeCoefficients (true);
        // Mandatory
        seg.setModelType (pcl::SACMODEL_PLANE);
        seg.setMethodType (pcl::SAC_RANSAC);
        seg.setDistanceThreshold (0.1); //0.01

        seg.setInputCloud (cloud);
        seg.segment (*inliers, *coefficients);

        if (inliers->indices.size () == 0)
        {
        PCL_ERROR ("Could not estimate a planar model for the given dataset.\n");
        return (-1);
        }

        std::cerr << "Model coefficients: " << coefficients->values[0] << " "
                                          << coefficients->values[1] << " "
                                          << coefficients->values[2] << " "
                                          << coefficients->values[3] << std::endl;

        std::cerr << "Model inliers: " << inliers->indices.size() << std::endl;

        pcl::PointCloud<pcl::PointXYZRGB> extracted_cloud;

        pcl::copyPointCloud(*cloud, *inliers, extracted_cloud);

        //pcl::io::savePCDFileASCII("plane.pcd", extracted_cloud);

        for (const auto& idx: inliers->indices)
        {
          cloud->points[idx].r = 255;
        }

        pcl::visualization::PCLVisualizer::Ptr viewer (new pcl::visualization::PCLVisualizer ("3D Viewer"));

        viewer->setBackgroundColor (0, 0, 0);
        pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb(cloud);
        viewer->addPointCloud<pcl::PointXYZRGB> (cloud, rgb, "sample cloud");
        viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "sample cloud");
        viewer->addCoordinateSystem (1.0);
        viewer->initCameraParameters ();

        viewer->addPlane(*coefficients, "chao");

        while (!viewer->wasStopped ())
        {
          viewer->spinOnce(1);
        }

        char c;
        std::cout << "Save params? s/n:";
        std::cin >> c;

        if(c == 's')
        {
          num_pc++;
          sum[0] = sum[0] + coefficients->values[0];
          sum[1] = sum[1] + coefficients->values[1];
          sum[2] = sum[2] + coefficients->values[2];
          sum[3] = sum[3] + coefficients->values[3];
        }

        std::cerr << "Model coefficients: " << sum[0] << " "
                                          << sum[1] << " "
                                          << sum[2] << " "
                                          << sum[3] << std::endl;

        //pcl::io::savePCDFileASCII("red.pcd", *cloud);
    }
    return (0);
}
