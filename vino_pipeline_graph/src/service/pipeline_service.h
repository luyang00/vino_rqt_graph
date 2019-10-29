#ifndef __PIPELINE_SERVICE__H
#define __PIPELINE_SERVICE__H
#include <ros/ros.h>
#include <rviz/panel.h>
#include <pipeline_srv_msgs/PipelineSrv.h>




namespace vino_service
{



class PipelineService 
{
public: 
   PipelineService(std::string  service_name);
   ~PipelineService(){};
   void reqStartPipeline();
   void reqPausePipeline(std::string pipeline_name);
   void reqStopPipeline(std::string pipeline_name);
private:
   
   ros::ServiceClient client;
   
};

} // end namespace vino_pipeline_graph

#endif // TELEOP_PANEL_H
