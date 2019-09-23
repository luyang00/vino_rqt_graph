#ifndef __BASE_GRAPH_NODE__H
#define __BASE_GRAPH_NODE__H
#include <iostream>
#include <vector>
#include <vino_param_lib/param_manager.h>

namespace vino_pipeline_graph{
    
    class Edge{
    private:
        std::string from_;
        std::string to_;
        //Edge attributes:
        int hz_ = 0;
        
    public:
        Edge(std::string & from, std::string & to);
        void setFrequency(int hz) { this->hz_ = hz; } 
        int getFrenquency(void) { return hz_; }
        std::string getFrom() {return from_;}
        std::string getTo() {return to_;}
    };


    class Node
    {
    public:

        typedef enum
        {
            Input = 0,
            Infer = 1,
            Output =2 ,
            Root = -1
        }NodeType;
        
        typedef struct 
        {
            NodeType type;
            std::string name;
            std::string engine;
            std::string model;
            std::string label;
        }NodeParams;
    private:
    
        /* data */
        std::string node_name_;
        int num_childs;
        int depth_ = 0;
        int width_ = 0;
        std::vector<Node *> child_nodes_;
        std::vector<Edge *> in_edges_;
        //Node params
        NodeParams params;
       
        int compute_depth(int current_depth);
    public:
   
        Node(std::string node_name);
        void setNodeParams(NodeParams params){this->params = params;}
        NodeParams getNodeParams(){return params;}
         virtual Edge * createEdge(std::string & from, std::string & to);
        int getNumOfChildNodes(void){return child_nodes_.size();}
        int getNumOfEdges(void){return in_edges_.size();}
        std::string getNodeName(void) {return node_name_;} 
        bool addChild(std::string child_name);
        bool addChild(Node * child_node);
        std::vector<Node *> getChilds(void){return child_nodes_;}
        std::vector<Edge *> getEdges(void) {return in_edges_;}
        void removeChildByName(std::string find_name);
        void removeEdgeByName(std::string name_parent);
        Node * findChildByName(std::string child_name);
        int getMaxDepth(void);
        int getMaxWidth(void);
       
        void printNode();
        void printEdge();
    
        ~Node();
    };   


   
    class Graph
    {
    protected:
        Node * root_;
        //isolated node operation
        std::vector<Node *> isolated_node_list_;
    private:

        std::string pipeline_name;

        virtual Node* createNode(std::string node_name);
    public:
        Graph();
        void setPipelineName(std::string pipeline_name){this->pipeline_name = pipeline_name;}
        std::string getPipelineName(){return pipeline_name;}
        void makeRoot(std::string node_name);
        Node * makeNode(std::string node_name);
        bool makeConnection(std::string name_parent, std::string name_child);
        bool makeConnection(std::string name_parent, Node::NodeParams param_parent,
                            std::string name_child,  Node::NodeParams param_child);
        void removeConnection(std::string name_parent, std::string name_child);
        void removeNode(std::string name_name);
        void printAllNodes();
        void printAllEdges();
        void printIsolatedNodes();
        int getMaxDepth();
        int getMaxWidth();
        int getNumIsolatedNode();
        Node::NodeParams getNodeParams(std::string node_name);
        void setNodeParams(std::string node_name,Node::NodeParams);
        //isolated node operation
        void addIsolatedNode(Node * node);
        void removeIsolatedNode(Node * node);
        Node * findIsolatedNode(std::string node_name);
        bool validConnection(void);
  

        
        
    }; 
}
#endif