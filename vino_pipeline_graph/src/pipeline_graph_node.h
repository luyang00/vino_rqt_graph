#ifndef __PIPELINE_GRAPH_NODE__H
#define __PIPELINE_GRAPH_NODE__H
#include <iostream>
#include <vector>
#include "base_graph_node.h"


//test vino param
#include <ros/package.h>
#include <ros/ros.h>
#include <vino_param_lib/param_manager.h>

namespace vino_pipeline_graph{
    class PipelineEdge: public Edge
    {
    private:
        
    public:
        PipelineEdge(std::string & from, std::string & to) :Edge(from,to){}
        
    }; 

    class PipelineNode: public Node
    {
    private:

        //override edge
        
       
    public:
         Edge * createEdge(std::string & from, std::string & to){ return new PipelineEdge(from,to);}
        int disp_row =0;
        int disp_col =0;
        PipelineNode(std::string node_name): Node(node_name){};
       
    };

    class PipelineGraph: public Graph
    {
    private:
        //override node
        Node* createNode(std::string node_name){ return new PipelineNode(node_name);}

      

    public:
        PipelineGraph(): Graph(){};

        void parsePipeline(std::string file_path)
        {
            
            Params::ParamManager::getInstance().parse(file_path);
            Params::ParamManager::getInstance().print();

            auto pcommon = Params::ParamManager::getInstance().getCommon();
            auto pipelines = Params::ParamManager::getInstance().getPipelines();

            if (pipelines.size() < 1) throw std::logic_error("Pipeline parameters should be set!");


            // Only use the first pipeline if there are several.
            this->makeRoot(pipelines[0].inputs[0]);

            for (auto it = pipelines[0].connects.begin(); it != pipelines[0].connects.end(); ++it) {
                // if(root_->findChildByName(it->first))
                    this->makeConnection(it->first,it->second);
            }
           
           
        }
    }; 
}
#endif