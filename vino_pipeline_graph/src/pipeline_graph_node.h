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

        Node::NodeParams parseNodePrams(const std::string name, Params::ParamManager::PipelineParams pipeline) 
        {
            vino_pipeline_graph::Node::NodeParams params;
            params.name  = name;
            //To-do : use std::bind and std::find_if implement implement these code
            for (auto it = pipeline.infers.begin(); it != pipeline.infers.end(); ++it) 
            {     
                if((*it).name == name)
                {
                params.model = (*it).model;
                params.engine = (*it).engine;
                params.label = (*it).label;
                }
            
            }

            for(auto it= pipeline.inputs.begin(); it != pipeline.inputs.end();++it){
                
                if( (*it) == name)
                    params.type = Node::NodeType::Input;
            }
            for(auto it= pipeline.infers.begin(); it != pipeline.infers.end();++it){
                
                if( (*it).name == name)
                    params.type = Node::NodeType::Infer;
            }
            for(auto it= pipeline.outputs.begin(); it != pipeline.outputs.end();++it){
                if( (*it) == name)
                    params.type = Node::NodeType::Output;
            }

           
            return params;
        }

        bool compare(const Params::ParamManager::InferenceParams& infer1, const Params::ParamManager::InferenceParams& infer2 )
        {
            return infer1.name == infer2.name;
        }


        void getPipelineParams( Params::ParamManager::PipelineParams & pipeline_params, Node * node = NULL)
        {
            if(root_ == NULL) throw std::logic_error("No root created before saving...");
            if(node == NULL && root_!=NULL)
            {
                node = root_;
            }

            //Push node into PipelineParams structure
            auto node_params =  node->getNodeParams();

            if(node_params.type == Node::NodeType::Input)
            {
            
                auto it = std::find(pipeline_params.inputs.begin(), pipeline_params.inputs.end(), node_params.name);
                if(it==pipeline_params.inputs.end())
                {
                    
                    pipeline_params.inputs.push_back(node_params.name);

                    //to-do rewrite duplicate code
                    for(int i=0;i<node->getNumOfEdges();i++)
                    {  
                        Edge * edge = node->getEdges()[i]; 
                        pipeline_params.connects.insert({edge->getFrom(),edge->getTo()});    
                    }
                    
                }
            }
            else if(node_params.type == Node::NodeType::Output)
            {
                auto it = std::find(pipeline_params.outputs.begin(), pipeline_params.outputs.end(), node_params.name);
                if(it==pipeline_params.outputs.end())
                {
                    
                    pipeline_params.outputs.push_back(node_params.name);
                            //to-do rewrite duplicate code
                    for(int i=0;i<node->getNumOfEdges();i++)
                    {  
                        Edge * edge = node->getEdges()[i]; 
                        pipeline_params.connects.insert({edge->getFrom(),edge->getTo()});    
                    }
                }               
            }
            else if(node_params.type == Node::NodeType::Infer)
            {
                auto it = std::find(pipeline_params.infers.begin(), pipeline_params.infers.end(), node_params.name);
                if(it==pipeline_params.infers.end())
                {
                   
                    auto infer_params = Params::ParamManager::InferenceParams(
                                                                                node_params.name,
                                                                                node_params.engine,
                                                                                node_params.model,
                                                                                node_params.label);
                    pipeline_params.infers.push_back(infer_params);
                            //to-do rewrite duplicate code
                    for(int i=0;i<node->getNumOfEdges();i++)
                    {  
                        Edge * edge = node->getEdges()[i]; 
                        pipeline_params.connects.insert({edge->getFrom(),edge->getTo()});    
                    }
                }  
            }
            
            //Push edge into connection structure
            
           


            if (node->getChilds().size() == 0 ) return;

          
            for(int i=0;i< node->getChilds().size(); i++)
            {
                Node * node_child =   node->getChilds()[i];   
              
                getPipelineParams( pipeline_params, node_child);
               
             
            }
            return;
        }
        
    public:
        PipelineGraph(): Graph(){};
        
        void parsePipeline(std::string file_path)
        {
            
            Params::ParamManager::getInstance().parse(file_path);
            Params::ParamManager::getInstance().print();

            auto pcommon = Params::ParamManager::getInstance().getCommon();
            auto pipelines = Params::ParamManager::getInstance().getPipelines();
           
            if (pipelines.size() < 1) throw std::logic_error("Pipeline parameters should be set!");

            this->setPipelineName(pipelines[0].name);
            // Only use the first pipeline if there are several.
            this->makeRoot(pipelines[0].inputs[0]);
            
            Node::NodeParams params_parent,params_child;

            for (auto it = pipelines[0].connects.begin(); it != pipelines[0].connects.end(); ++it) {
                
                    auto param_parent = parseNodePrams(it->first, pipelines[0]);
                    auto param_child =  parseNodePrams(it->second, pipelines[0]);

                   
                    this->makeConnection(it->first, param_parent,
                            it->second, param_child);
            }


      
        }
        void exportPipeline(std::string file_path)
        {
            Params::ParamManager::PipelineParams pipeline_params;
            getPipelineParams(pipeline_params);
            std::cout << "try export pipeline" << std::endl;
            
            Params::ParamManager::getInstance().save(pipeline_params,file_path);
        }
    }; 
}
#endif