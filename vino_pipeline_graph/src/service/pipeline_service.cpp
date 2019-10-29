
#include <ros/ros.h>
#include <rviz/panel.h>
// #include <thread>

#include "pipeline_service.h"
#include <vino_param_lib/param_manager.h>
#include <QThread>





namespace vino_service
{


PipelineService::PipelineService(std::string  service_name)
{
    
   ros::NodeHandle n;
   client = n.serviceClient<pipeline_srv_msgs::PipelineSrv>(service_name);
} // end namespace vino_pipeline_graph

 void PipelineService::reqStartPipeline()
 {
   pipeline_srv_msgs::PipelineSrv srv;
   auto pipelines = Params::ParamManager::getInstance().getPipelines();
   std::cout << pipelines.size() << std::endl;

   auto pipeline_desc = Params::ParamManager::getInstance().toString();
   
   srv.request.pipeline_request.cmd = "RUN_PIPELINE_FROM_SERVICE";
   srv.request.pipeline_request.value = pipeline_desc;
 
   auto succ = client.call(srv) ? true : false;
   if(succ) 
   {
      std::cout << "send request success" << std::endl;
   }
   else
   {
      std::cout << "Failed to send a request." << std::endl;
   }
   
 }
 void PipelineService::reqPausePipeline(std::string pipeline_name)
 {
   pipeline_srv_msgs::PipelineSrv srv;


 
   
   srv.request.pipeline_request.cmd = "PAUSE_PIPELINE";
   srv.request.pipeline_request.value = "";
 
   auto succ = client.call(srv) ? true : false;
   if(succ) 
   {
      std::cout << "send request success" << std::endl;
   }
   else
   {
      std::cout << "Failed to send a request." << std::endl;
   }
   
 }
  void PipelineService::reqStopPipeline(std::string pipeline_name)
 {
   pipeline_srv_msgs::PipelineSrv srv;


 
   
   srv.request.pipeline_request.cmd = "STOP_PIPELINE";
   srv.request.pipeline_request.value = "";
 
   auto succ = client.call(srv) ? true : false;
   if(succ) 
   {
      std::cout << "send request success" << std::endl;
   }
   else
   {
      std::cout << "Failed to send a request." << std::endl;
   }
   
 }

}

