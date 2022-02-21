/*
Calculates the rigid transformation between two poses. whaits for the ENTER key to be pressed.
*/

#include "pc_frame_transform.h"

ros::Publisher pub_pose1, pub_pose2, pub_transform;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "pc_frame_transform");
    ros::NodeHandle nh;

    pub_pose1 = nh.advertise<geometry_msgs::PoseStamped>("/pose1", 1);
    pub_pose2 = nh.advertise<geometry_msgs::PoseStamped>("/pose2", 1);
    //pub_transform = nh.advertise<geometry_msgs::Transform>("transformada", 1);

    //ros::Subscriber sub = nh.subscribe<TIPO_MENSAGEM!!!>("NOME!!!", 1, );

    std::cout << "Pose1?\n (ENTER)";
    getchar();

    boost::shared_ptr<geometry_msgs::PoseStamped const> vertical;
    vertical = ros::topic::waitForMessage<geometry_msgs::PoseStamped>("/zed2/zed_node/pose");


    std::cout << "Pose2?\n (ENTER)";
    getchar();

    boost::shared_ptr<geometry_msgs::PoseStamped const> inclined;
    inclined = ros::topic::waitForMessage<geometry_msgs::PoseStamped>("/zed2/zed_node/pose");

    tf::Stamped<tf::Pose> vertical_stamp, inclined_stamp;
    tf::poseStampedMsgToTF(*vertical, vertical_stamp);
    tf::poseStampedMsgToTF(*inclined, inclined_stamp);

    tf::Transform transformacao;

    transformacao = vertical_stamp.inverseTimes(inclined_stamp);

    std::cout << "Waiting";
    getchar();

    static tf::TransformBroadcaster br;
    ros::Rate rate(1);
    while(ros::ok())
    {
        pub_pose1.publish(vertical);
        pub_pose2.publish(inclined);
        br.sendTransform(tf::StampedTransform(transformacao, ros::Time::now(), "vertical", "base_link"));

        rate.sleep();
    }
    //ros::spin();
}
