#include <iostream>
#include "base_graph_node.h"
namespace vino_pipeline_graph{
  
Edge::Edge(std::string & from, std::string & to):hz_(0)
{
    this->from_ = from;
    this->to_=to; 
}

Edge * Node::createEdge(std::string & from, std::string & to)
{
    return new Edge(from,to); 
}

Node::Node(std::string node_name) : node_name_(node_name)
{
}

bool Node::addChild(std::string child_name)
{
    Node * child_node = findChildByName(child_name);
    if( NULL != child_node){
        if(child_node->findChildByName(this->getNodeName())!=NULL)
        {
            std::cout << "loop detected! create link failed!" << std::endl;
            return false;
        }
    }//If the child exist in the graph 
    else // If not exist then create one
    {
        child_node = new Node( child_name);
    }
    child_node->in_edges_.push_back(createEdge(this->node_name_,child_node->node_name_));
    child_nodes_.push_back( child_node);
    return true;
}

bool Node::addChild(Node * child_node)
{
    if(child_node == NULL) 
    {
        std::cout << "Node is null pointer!" << std::endl;
        return false;
    }
    if(child_node->findChildByName(this->getNodeName())!=NULL)
    {
        std::cout << "loop detected! create link failed!" << std::endl;
        return false;
    }
    child_node->in_edges_.push_back(createEdge(this->node_name_,child_node->node_name_));
    child_nodes_.push_back(child_node);
    return true;
}


void Node::removeChildByName(std::string find_name)
{
    for(std::vector<Node*>::iterator it = child_nodes_.begin(); it!= child_nodes_.end(); ++it )
    {
        if ( find_name == (*it)->getNodeName()){
            // delete *it;
            child_nodes_.erase(it);
            return;
        }
        //(*it)->removeChildByName(find_name);
    }
}
void Node::removeEdgeByName(std::string name_parent)
{
    for(std::vector<Edge *>::iterator it = in_edges_.begin(); it!= in_edges_.end(); ++it )
    {
        if ( name_parent == (*it)-> getFrom()){
            in_edges_.erase(it);
            return;
        }
    }
}

Node * Node::findChildByName(std::string child_name)
{
    //if staisfy the node 
    if( child_name == this->getNodeName())  return this;
    //else recursively iterate child nodes
    Node * ret;
    for(int i=0;i< child_nodes_.size(); i++)
    {
        ret =  child_nodes_[i]->findChildByName(child_name);
        if (ret != NULL) return ret;
    }    
}

int Node::getMaxDepth(void){
    depth_ = compute_depth(0);
    return depth_;
}

int Node::compute_depth(int current_depth)
{ 
    if( child_nodes_.size() == 0) return current_depth;
    else current_depth ++;
    int max_iter_depth = 0;
    for(int i=0;i< child_nodes_.size(); i++)
    {
        max_iter_depth =  std::max(max_iter_depth, child_nodes_[i]->compute_depth(current_depth));
    }    
    return max_iter_depth;
}

int Node::getMaxWidth(void){
    width_ = std::max( width_, int(child_nodes_.size()));
    for(int i=0;i<child_nodes_.size();i++)
    {
        width_ = std::max( width_, child_nodes_[i]->getMaxWidth() );  
    }
    return width_;    
}

void Node::printNode()
{
    if (child_nodes_.size() == 0 ) return;
    
    for(int i=0;i< child_nodes_.size(); i++)
    {
        std::cout << this->getNodeName() << "->" << child_nodes_[i]->getNodeName() << std::endl;
        child_nodes_[i]->printNode();
    }
    
}

void Node::printEdge()
{
  //  if(in_edges_.size() == 0 ) return;
    for(int i=0;i< in_edges_.size(); i++)
    {
        std::cout << "Edges from: " << (*in_edges_[i]).getFrom()<<
        " to: " << (*in_edges_[i]).getTo() <<
        " Frequency: " <<(*in_edges_[i]).getFrenquency() <<
        std::endl; 
    }
    for(int i=0;i< this->getNumOfChildNodes();i++)
    {
        child_nodes_[i]->printEdge();
    }
}

Node::~Node(){}

Graph::Graph():
    root_(NULL)
{
}

Node * Graph::createNode(std::string node_name)
{
    return new Node(node_name); 
}

void Graph::makeRoot(std::string node_name)
{
    std::cout << "[GRAPH_INFO] make root :" << node_name << std::endl;
    if(root_ != NULL) 
    {
       // throw std::logic_error("root already exist!");
    }
    root_ = createNode(node_name);
}

void Graph::addIsolatedNode(Node * node)
{
    if(node == NULL) 
        return;
    //if a node still has a link after removing one, keep it not isolated
    else if (node->getNumOfEdges()>0)
     {  
         std::cout << "!!!!!!!return " << std::endl;
         return;
     }
    else
    {
        std::cout << "push !!!!" << std::endl;
    isolated_node_list_.push_back(node);
    }
}

void Graph::removeIsolatedNode(Node * node)
{
    if(node == NULL) return;
    // std::cout << "edges: "<< node->getNumOfEdges() << " nodes:" << node->getNumOfChildNodes() << std::endl;
    if(node->getNumOfEdges()>0 )
    {
        for(std::vector<Node*>::iterator it =  isolated_node_list_.begin(); it!=  isolated_node_list_.end(); ++it )
        {
            if ( node->getNodeName() == (*it)->getNodeName()){
                //To-do: do not delete node may cause memory leak
                isolated_node_list_.erase(it);
                return;
            }
        }
    }
}

Node * Graph::findIsolatedNode(std::string node_name)
{
    for(std::vector<Node*>::iterator it =  isolated_node_list_.begin(); it!=  isolated_node_list_.end(); ++it )
    {
        if ( node_name == (*it)->getNodeName()){
            return (*it);
        }
        else if((*it)->findChildByName(node_name)){
            return ((*it)->findChildByName(node_name));
        }
    }
    return NULL;
}

void Graph::printIsolatedNodes()
{   
    std::cout << "[GRPAH_INFO] print isolated: " << "There are " << isolated_node_list_.size() << " isolated nodes." << std::endl;
    for(std::vector<Node*>::iterator it =  isolated_node_list_.begin(); it!=  isolated_node_list_.end(); ++it )
    {
        std::cout << (*it)->getNodeName() << std::endl;
    }
}

int Graph::getNumIsolatedNode(void)
{
    return isolated_node_list_.size();
}

Node * Graph::makeNode(std::string node_name)
{

    if(root_->findChildByName(node_name) != NULL) 
    {
        std::cout <<  node_name << "is already exist in graph." << std::endl;
        return root_->findChildByName(node_name) ;
    }
    else if(findIsolatedNode(node_name) != NULL ){ 
        std::cout <<  node_name << "is already exist in isolated lists" << std::endl;
        return findIsolatedNode(node_name);
    }
    else
    {
       
        Node * node_isolated = createNode(node_name);
        addIsolatedNode(node_isolated);
        std::cout << "create an isolated node: " << node_name << std::endl;
        return node_isolated;
    } 
}
bool Graph::validConnection(void)
{
    bool ret = isolated_node_list_.size() > 0 ? false : true;
    return ret;
}

bool Graph::makeConnection(std::string name_parent, std::string name_child)
{
    std::cout << "[GRPAH_INFO] make connection " << name_parent << " -> " << name_child << std::endl; 
    Node * node_parent = root_->findChildByName(name_parent) ? root_->findChildByName(name_parent) : findIsolatedNode(name_parent);
    Node * node_child = root_->findChildByName(name_child) ? root_->findChildByName(name_child) : findIsolatedNode(name_child);
    
    if( NULL == node_parent)//parent node do not exist in both graph and isolated list which will be an isolated node
    {
        node_parent = makeNode(name_parent);
    }

    if( NULL == node_child)//If not exist then create one
    {
        std::cout << "create child: " << name_child << std::endl;
        node_child = createNode(name_child);
    }
    

    
    bool ret = node_parent->addChild(node_child);
    if(ret)
    {
        removeIsolatedNode(node_child);

        printIsolatedNodes();
        return true;
    }
    else
    {
        return false;
    }
    
}

bool  Graph::makeConnection(
    std::string name_parent, Node::NodeParams param_parent,
    std::string name_child,  Node::NodeParams param_child)
{
    bool ret =  makeConnection(name_parent, name_child);
    
    std::cout << "HERE ????" << " ret:" << ret << std::endl;
    if(ret)
    {
        std::cout << "FIND ????" << std::endl;

        Node * node_parent = root_->findChildByName(name_parent) ? root_->findChildByName(name_parent) : findIsolatedNode(name_parent);
        Node * node_child = root_->findChildByName(name_child) ? root_->findChildByName(name_child) : findIsolatedNode(name_child);
      
        
        node_parent->setNodeParams(param_parent);
        node_child->setNodeParams(param_child);

        std::cout <<"???: " << node_parent->getNodeParams().model << std::endl; 
    }

    return ret; 
}

Node::NodeParams Graph::getNodeParams(std::string node_name)
{
    Node * node = root_->findChildByName(node_name) ?  root_->findChildByName(node_name) : findIsolatedNode(node_name) ;
    return node->getNodeParams();
}   

void Graph::setNodeParams(std::string node_name,Node::NodeParams params)
{
        Node * node = root_->findChildByName(node_name) ?  root_->findChildByName(node_name) : findIsolatedNode(node_name) ;
        node->setNodeParams(params);
}

void Graph::removeConnection(std::string name_parent, std::string name_child)
{
    std::cout << "[GRPAH_INFO] remove connection: " << name_parent << " -> " << name_child <<  std::endl;
    Node * node_parent = root_->findChildByName(name_parent) ?  root_->findChildByName(name_parent) : findIsolatedNode(name_parent) ;
    Node * node_child =  node_parent->findChildByName(name_child);

    if(node_parent == NULL || node_child == NULL){
        std::cout << "error,node doest not exist while removing connection." << std::endl;
    }

    node_child->removeEdgeByName(name_parent);    
    node_parent->removeChildByName(name_child);


    addIsolatedNode(node_child);

}
void Graph::removeNode(std::string node_name)
{
    std::cout << "[GRPAH_INFO] remove node: " << node_name <<  std::endl;
    Node * node_remove = root_->findChildByName(node_name) ? root_->findChildByName(node_name) : findIsolatedNode(node_name);
    if( NULL == node_remove) {
        std::cout << "error, node does not exist." << std::endl;
        return;
    }
    //traverse all child node to delete their in-edges
    std::vector<Node *> node_childs = node_remove->getChilds();
    for(std::vector<Node*>::iterator it =  node_childs.begin(); it!=  node_childs.end(); ++it )
    {
        
        (*it)->removeEdgeByName(node_name);
        addIsolatedNode(*it);
        // std::cout << "!!rm "<< (*it)->getNodeName() << std::endl;
        //node_remove->removeChildByName((*it)->getNodeName());
    }

    //traverse parent node to delete their children
    std::vector<Edge *> edges_child = node_remove->getEdges();
    for(std::vector<Edge *>::iterator it = edges_child.begin(); it!= edges_child.end(); ++it )
    {
        
        
            std::string name_parent = (*it)-> getFrom();
            
            root_->findChildByName(name_parent)->removeChildByName(node_name);
        
    }

    if (findIsolatedNode(node_name))
    {
        for(std::vector<Node*>::iterator it =  isolated_node_list_.begin(); it!=  isolated_node_list_.end(); ++it )
        {
            if ( node_name == (*it)->getNodeName()){
                //To-do: do not delete node may cause memory leak
                isolated_node_list_.erase(it);
                return;
            }
        }
    }
    
    
}

void Graph::printAllNodes()
{
    std::cout << "[GRPAH_INFO] print all nodes" << std::endl;
    //print connected nodes:
    root_->printNode();
    //print isolated nodes:
    //printIsolatedNodes();

}

void Graph::printAllEdges()
{
    std::cout << "[GRPAH_INFO] print all edges" << std::endl;
    root_->printEdge();
}

int Graph::getMaxDepth()
{
    return root_->getMaxDepth();
}

int Graph::getMaxWidth()
{
    return root_->getMaxWidth();
}




}
