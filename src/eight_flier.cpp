#include <ros/ros.h>
#include <ros/package.h>
#include <nodelet/nodelet.h>

#include <mrs_msgs/PathSrv.h>
#include <std_srvs/Trigger.h>

namespace flight_8_path
{
  class EightFlier : public nodelet::Nodelet
  {
  public:
    virtual void onInit();

  private:
    bool is_initialized_ = false;

    // Path generation
    mrs_msgs::Path path_;
    void generatePath();

    int n_points_;
    double scale_;

    // Start service
    bool callbackStart(std_srvs::Trigger::Request &req, std_srvs::Trigger::Response &res);
    ros::ServiceServer srv_server_start_;

    // Trajectory generation client
    ros::ServiceClient srv_client_trajectory_;
  };

  void EightFlier::onInit()
  {
    ros::NodeHandle nh = nodelet::Nodelet::getMTPrivateNodeHandle();

    // Load parameters
    nh.param("n_points", n_points_, 10);
    nh.param("scale", scale_, 2.0);

    srv_server_start_ = nh.advertiseService("start", &EightFlier::callbackStart, this);
    srv_client_trajectory_ = nh.serviceClient<mrs_msgs::PathSrv>("trajectory_generation/path");

    is_initialized_ = true;
  }

  void EightFlier::generatePath()
  {
    // Lambda functions for the parametric equations of the eight-flier
    // See https://www.geogebra.org/m/txztgt7p for a visualization
    // See https://en.wikipedia.org/wiki/Lemniscate_of_Bernoulli for more information
    auto x = [this](double t)
    { return (scale_ * cos(t)) / (1 + sin(t) * sin(t)); };
    auto y = [this](double t)
    { return (scale_ * sin(t) * cos(t)) / (1 + sin(t) * sin(t)); };

    // Fill the path message
    for (int i = 0; i < n_points_; i++)
    {
      double t = 2 * M_PI * i / n_points_;

      mrs_msgs::Reference point;
      point.position.x = x(t);
      point.position.y = y(t);
      point.position.z = 1.0;

      path_.points.push_back(point);
    }
  }

  bool EightFlier::callbackStart(std_srvs::Trigger::Request &req, std_srvs::Trigger::Response &res)
  {
    if (!is_initialized_)
    {
      ROS_ERROR("[EightFlier] Nodelet not initialized");
      return false;
    }

    generatePath();

    mrs_msgs::PathSrv path_srv;
    path_srv.request.path = path_;

    ROS_INFO("[EightFlier] Calling trajectory generation service");
    if (!srv_client_trajectory_.call(path_srv))
    {
      ROS_ERROR("[EightFlier] Failed to call trajectory generation service: %s", srv_client_trajectory_.getService().c_str());
      return false;
    }

    res.success = true;
    res.message = "Path generated";

    return true;
  }
}

#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(flight_8_path::EightFlier, nodelet::Nodelet)