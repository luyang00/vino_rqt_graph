#ifndef __BASE_GRAPH_NODE__H
#define __BASE_GRAPH_NODE__H
#include <iostream>
#include <vector>
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
    private:
        /* data */
        std::string node_name_;
        int num_childs;
        int depth_ = 0;
        int width_ = 0;
        std::vector<Node *> child_nodes_;
        std::vector<Edge *> in_edges_;
       
        int compute_depth(int current_depth);
    public:
        Node(std::string node_name);
         virtual Edge * createEdge(std::string & from, std::string & to);
        int getNumOfChildNodes(void){return child_nodes_.size();}
        int getNumOfEdges(void){return in_edges_.size();}
        std::string getNodeName(void) {return node_name_;} 
        void addChild(std::string child_name);
        void addChild(Node * child_node);
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

 

        virtual Node* createNode(std::string node_name);
    public:
        Graph();
        void makeRoot(std::string node_name);
        Node * makeNode(std::string node_name);
        void makeConnection(std::string name_parent, std::string name_child);
        void removeConnection(std::string name_parent, std::string name_child);
        void removeNode(std::string name_name);
        void printAllNodes();
        void printAllEdges();
        void printIsolatedNodes();
        int getMaxDepth();
        int getMaxWidth();
        int getNumIsolatedNode();
        //isolated node operation
        void addIsolatedNode(Node * node);
        void removeIsolatedNode(Node * node);
        Node * findIsolatedNode(std::string node_name);
        bool validConnection(void);
        
    }; 
}
#endif