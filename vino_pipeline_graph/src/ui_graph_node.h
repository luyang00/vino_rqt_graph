#ifndef __UI_GRAPH_NODE__H
#define __UI_GRAPH_NODE__H
#include <iostream>
#include <vector>
#include "pipeline_graph_node.h"
#include <QPoint>
#include <QRect>
#include <QColor>
#include <QPainter>
#include <QString>
#include <QLine>
#include <QMessageBox>
#include <QVector>
#include <QPolygon>
#define RECT_HEIGHT 30.0
#define RECT_WIDTH 115.0
#define LINE_WIDTH_THRESHOLD 3
namespace vino_pipeline_graph{
    class UIPipelineEdge: public PipelineEdge
    {
    private:
        QLine line;
        QColor color;
    public:
        friend class UIPipelineGraph;
        UIPipelineEdge(std::string & from, std::string & to) :
            PipelineEdge(from,to),
            color(0,0,0)
            {}
        bool contains(QPoint p)
        {
            int x1 = line.x1();
            int y1 = line.y1();
            int x2 = line.x2();
            int y2 = line.y2();

           
            float k = (y2-y1)/(x2-x1+ 1e-15);
            float b = y1 - k * x1;
           
            float dist = abs( (k* p.x() - p.y() + b)/sqrt(k*k+1));
          
            if(dist < LINE_WIDTH_THRESHOLD )
                return true;
            else 
                return false;
            
        }
    }; 

    class UIPipelineNode: public PipelineNode
    {
    private:
        QRect rect;
        QColor color;
        QString name_node;      

        // QPaintDevice * canvas_;
        Edge * createEdge(std::string & from, std::string & to){ std::cout << "create edge !" << std::endl; return  new UIPipelineEdge(from,to);}
        
    public:
        friend class UIPipelineGraph;
        UIPipelineNode(std::string node_name): 
            PipelineNode(node_name),
            color(255,255,255),
            rect(1,1,RECT_WIDTH,RECT_HEIGHT)
            {}
  
    };

    class UIPipelineGraph: public PipelineGraph
    {
    private:
        //UIPipelineNode * root_;
        std::string pipeline_name;
        QPaintDevice * canvas_;
        int canvas_width_, canvas_height_;
        float boarder_width_, boarder_height_;
        float rect_width_,rect_height_;
       
        QPoint node_link_target;
        UIPipelineNode * selected_node;
        UIPipelineEdge * selected_edge;
    public:
        
        UIPipelineGraph(QPaintDevice *canvas, int canvas_width, int canvas_height): 
            PipelineGraph(),
            canvas_(canvas),
            canvas_width_(canvas_width),
            canvas_height_(canvas_height),
            boarder_width_(0.04),
            boarder_height_(0.02),
            rect_width_(0.16),
            rect_height_(0.04),
            
          
            selected_edge(NULL),
            selected_node(NULL)

            {
            }

      
        Node* createNode(std::string node_name){  return new UIPipelineNode(node_name);}

        void drawGraph(bool resize = false)
        {
            if(resize == true)
            {
                initialTraverseDrawNode((UIPipelineNode *)root_);
            
            }
            else
            {
                traverseDrawNode((UIPipelineNode *)root_);
               
            }
            traverseDrawEdge((UIPipelineNode *)root_);

            //draw isolated node
            for(int i=0;i<getNumIsolatedNode(); i++)
            {
                traverseDrawNode((UIPipelineNode *)isolated_node_list_[i]);
                traverseDrawEdge((UIPipelineNode *)isolated_node_list_[i]);
            }
         

          
            
        }

        void setCanvasSize(int canvas_width, int canvas_height)
        {
            canvas_height_ = canvas_height;
            canvas_width_ = canvas_width;

            int depth = root_->getMaxDepth()+1 ;
            // rect_width_  = (1.0 - boarder_width_ * (2 + depth))  / depth * canvas_width_ > RECT_WIDTH ? RECT_WIDTH/canvas_width_ : (1.0 - boarder_width_ * (2 + depth))  / depth ;
            
            // rect_height_ = (1.0 - boarder_height_ * 2) * canvas_height_ < RECT_HEIGHT ? (1.0 - boarder_height_ * 2): RECT_HEIGHT/canvas_height;
        
        
        }

        void drawEdge(UIPipelineEdge * edge)
        {
            QPainter painter(canvas_);
            QPen pen;
            pen.setColor(edge->color);
            pen.setWidth(1);
            painter.setPen(pen);

            
            UIPipelineNode * node_from =(UIPipelineNode *) ((root_->findChildByName(edge->getFrom())) ? root_->findChildByName(edge->getFrom()): findIsolatedNode(edge->getFrom()) );
            UIPipelineNode * node_to =(UIPipelineNode *) (root_->findChildByName(edge->getTo())? root_->findChildByName(edge->getTo()) :findIsolatedNode(edge->getTo ()));
            if( node_from != NULL && node_to != NULL)
            {
                // std::cout << node_from->getNodeName() << " (---->) " << node_to->getNodeName()<< std::endl;
                // std::cout << node_from->rect.bottomRight().x() << std::endl;
                edge->line.setPoints(node_from->rect.bottomRight(), node_to->rect.topLeft());
                painter.drawLine(edge->line);
            }


        }
        void drawNode(UIPipelineNode * node)
        {
            QPainter painter(canvas_);
            QBrush brush(node->color, Qt::Dense4Pattern);
            painter.setBrush(brush);
           
            //painter.drawRect(node->rect);
            // painter.drawRect(node->rect.x(),
            //                  node->rect.y(),
            //                  rect_width_ * canvas_width_,
            //                  rect_height_ * canvas_height_);
            painter.drawRect(node->rect.x() ,
                             node->rect.y() ,
                             rect_width_ * canvas_width_,
                             rect_height_ * canvas_height_);


            //zoom node length


            QFont font;
            int font_size = 1/rect_width_ > 7 ? int(1/rect_width_) : 7;
            font_size = font_size > 11 ? 11 : font_size;
            font.setPointSize(font_size);
          
            painter.setFont(font);
            painter.setPen(Qt::black);
            painter.drawText(node->rect.x()  + 3, 
                             node->rect.y()  + 1/2.0 * rect_height_ * canvas_height_, 
                             QString::fromStdString(node->getNodeName()));
            
        }
        void initialTraverseDrawNode(UIPipelineNode * node)
        {
            if(node == root_){
                node->rect.setRect(0 + boarder_width_ * canvas_width_,
                              (rect_height_ + boarder_height_) * canvas_height_ + boarder_height_ * canvas_height_,
                              rect_width_ * canvas_width_,
                              rect_height_ * canvas_height_);
            }

            drawNode(node);
            
            if (node->getChilds().size() == 0 ) return;

            int row = 1;
            for(int i=0;i< node->getChilds().size(); i++)
            {

                UIPipelineNode * node_child =  (UIPipelineNode *) (node->getChilds()[i]);   
                
                
                node_child->disp_col = node->disp_col + 1;
                node_child->disp_row = node->disp_row + row  + i; 

                // node_child->rect.setX(node_child->disp_col * (rect_width_ + boarder_width_)* canvas_width_ );
                // node_child->rect.setY(node_child->disp_row * (rect_height_ + boarder_height_) * canvas_height_ );
                // node_child->rect.set

                node_child->rect.setRect(node_child->disp_col * (rect_width_ + boarder_width_)* canvas_width_ + boarder_width_ * canvas_width_,
                                         node_child->disp_row * (rect_height_ + boarder_height_) * canvas_height_ + boarder_height_ * canvas_height_,
                                         rect_width_ * canvas_width_,
                                         rect_height_ * canvas_height_);



                row += node_child->getMaxWidth();

                initialTraverseDrawNode(node_child);
                
                std::cout << node->getNodeName() << "->" << node_child->getNodeName() << 
                "( "<< node_child->disp_row << "," << node_child->disp_col << ") "<< std::endl;

              
            }
        }
        void setNodeLinkTo(QPoint mouse_pos)
        {
            node_link_target = mouse_pos;
        }
        //draw a line from a selected node to current mouse moving position
        void drawLink()
        {
            if(selected_node == NULL) return;
            
            QPainter painter(canvas_);
            QLine line;
            QPen pen;
            QColor color(0,255,255);
            pen.setColor(color);
            pen.setWidth(1);
            painter.setPen(pen);

            line.setPoints(selected_node->rect.bottomRight(),node_link_target);
            painter.drawLine(line);

        }
        bool createEdgeBySelection()
        {
            
            UIPipelineNode * source_node = selected_node;
            
            selectdNode(node_link_target);
            UIPipelineNode * target_node =  selected_node;
           
            if(source_node !=NULL && target_node!=NULL && target_node!=root_)
            {
                std::cout << "make connection" <<  source_node->getNodeName() << " - " <<target_node->getNodeName();
                return makeConnection( source_node->getNodeName(),target_node->getNodeName());
            }
            return false;
        }
         void traverseDrawNode(UIPipelineNode * node)
        {

            drawNode(node);
            
            if (node->getChilds().size() == 0 ) return;

          
            for(int i=0;i< node->getChilds().size(); i++)
            {

                UIPipelineNode * node_child =  (UIPipelineNode *) (node->getChilds()[i]);   
                traverseDrawNode(node_child);
             
            }
        }
        void traverseDrawEdge(UIPipelineNode * node)
        {
            
            for(int i=0;i< node->getNumOfEdges();i++)
            {
               
                UIPipelineEdge * p_edge = (UIPipelineEdge *)(node->getEdges()[i]);
               
                drawEdge( p_edge);
              
                
            }

            if (node->getChilds().size() == 0 ) return;

          
            for(int i=0;i< node->getChilds().size(); i++)
            {
                
                UIPipelineNode * node_child =  (UIPipelineNode *) (node->getChilds()[i]);   
                traverseDrawEdge(node_child);
             
            }
        }



        UIPipelineNode * selectNodeByPos(UIPipelineNode * node,QPoint mouse_pos){
            if(node->rect.contains(mouse_pos))
            {
                node->color.setRgb(125,125,125,255);
                return node;
            }
            if (node->getChilds().size() == 0 ) return NULL;

            for(int i=0;i< node->getChilds().size(); i++)
            {
                UIPipelineNode * node_child =  (UIPipelineNode *) (node->getChilds()[i]);   
                UIPipelineNode * selected_node = selectNodeByPos(node_child,mouse_pos);
                if (NULL!= selected_node) 
                {
                    selected_node->color.setRgb(125, 125, 125, 255);
                    return selected_node; 
                }
            }
            return NULL;
        }

       

        UIPipelineEdge * selectEdgeByPos(UIPipelineNode * node,QPoint mouse_pos){
               
                
                for(int j=0;j< node->getNumOfEdges();j++ )
                {
                    UIPipelineEdge * edge =(UIPipelineEdge *)(node->getEdges()[j]);
                    bool isContained =  edge->contains(mouse_pos);
                    if(isContained) return edge;
                
                }

                // if(node->getNumOfChildNodes()==0) return NULL;
                for(int i=0;i<node->getNumOfChildNodes();i++)
                {
                    UIPipelineNode *  node_child =(UIPipelineNode * )(node->getChilds()[i]);
                    UIPipelineEdge * edge = selectEdgeByPos(node_child,mouse_pos);
                    if(edge != NULL) return edge;
                }
                return NULL;

          
        }     
    

        void drawMovedNode(QPoint mouse_pos_press,QPoint mouse_pos_release)
        {
            
            // UIPipelineNode * selected_node = findNodeIsClicked((UIPipelineNode *)root_, mouse_pos_press) ;
            if (selected_node  == NULL){
               
                return;
            }
            QPoint new_pos;
            new_pos.setX(selected_node->rect.x() - (mouse_pos_release.x() - mouse_pos_press.x()));
            new_pos.setY(selected_node->rect.y() - (mouse_pos_release.y() - mouse_pos_press.y()));
           
            QRect rect(new_pos.x(),new_pos.y(),selected_node->rect.width(),selected_node->rect.height());
            bool isIntersected = checkIntersection(rect, selected_node->getNodeName() ,(UIPipelineNode *)root_);
            if(isIntersected)
            {    std::cout << "is intesected!" << std::endl; 
                return ;
            }
            selected_node->rect.moveTo( new_pos);
            
    
           
        }
        void selectdNode(QPoint mouse_pos)
        {
            std::cout << "There are " << getNumIsolatedNode() << " isolated node." << std::endl;

            //reinitiate color of selected node
             if(selected_node)
            {
                selected_node->color.setRgb(255,255,255,255);
            }

            for(int i=0; i < getNumIsolatedNode(); i++)
            {
                auto isolated_node = selectNodeByPos((UIPipelineNode *)isolated_node_list_[i], mouse_pos) ;
                if(isolated_node)
                {
                    selected_node = isolated_node;
                    //set focus color
                    selected_node->color.setRgb(124,125,125,255);
                    return;
                }   
            }

           

            selected_node = selectNodeByPos((UIPipelineNode *)root_, mouse_pos) ;
            if (selected_node){
                std::cout << "select node: " << selected_node->getNodeName() << std::endl;
            }
        }

        void selectEdge(QPoint mouse_pos)
        {
            if(selected_edge)
            {   
                selected_edge->color.setRgb(0,0,0,255);
            }

            for(int i=0; i < getNumIsolatedNode(); i++)
            {
                auto isolated_edge = selectEdgeByPos((UIPipelineNode *)isolated_node_list_[i], mouse_pos) ;
                if(isolated_edge){
                    
                    selected_edge= isolated_edge;
                    //set focus color
                    selected_edge->color.setRgb(0,255,0,255);
                    return;
                }   
            }


            selected_edge = selectEdgeByPos((UIPipelineNode *)root_, mouse_pos) ;
            if(selected_edge)
            {
                std::cout << "select an edge: "<< selected_edge->getFrom() << " --> " << selected_edge->getTo() << std::endl;
                selected_edge->color.setRgb(0,255,0,255);
            }
            
            
        }
        void addNewNode(std::string node_name)
        {   
            if(selected_node)
            {   
                selected_node->color.setRgb(255,255,255,255);
            }
            if(selected_edge)
                selected_edge->color.setRgb(0,0,0,255);
            
            if(root_->findChildByName(node_name) || findIsolatedNode(node_name))
            {
                
                QMessageBox message(QMessageBox::NoIcon, "Failed", "Create faield, node duplicated!"); 
                message.exec();

                return;
            }
            UIPipelineNode * new_node =   (UIPipelineNode *)makeNode(node_name);
            new_node->rect.setWidth(((UIPipelineNode *)root_)->rect.width());
            new_node->rect.setHeight(((UIPipelineNode *)root_)->rect.height());
            new_node->color.setRgb(125, 125, 125, 255);

            selected_node = new_node;
        }

        void RemoveSelectedElement()
        {
            if(selected_node)
                removeNode(selected_node->getNodeName());
            else if(selected_edge)
                removeConnection(selected_edge->getFrom(),selected_edge->getTo());
        }
      
        bool checkIntersection(QRect rect, std::string node1_name,  UIPipelineNode * node2)
        {
            //if it is the samenode
            // if(node1_name == node2->getNodeName()) return false;

            bool isIntersected =  rect.intersects(node2->rect);
            if(isIntersected && node1_name != node2->getNodeName()) {
                std::cout <<"intersected with: "<< node2->getNodeName() << std::endl;
                return isIntersected;}

            if (node2->getChilds().size() == 0 ) return false;

            for(int i=0;i< node2->getChilds().size(); i++)
            {
                UIPipelineNode * node_child =  (UIPipelineNode *) (node2->getChilds()[i]);   
                isIntersected = checkIntersection(rect, node1_name, node_child);
                if(isIntersected) return true;
                
            }
            return false;
        }

        
    }; 
}
#endif