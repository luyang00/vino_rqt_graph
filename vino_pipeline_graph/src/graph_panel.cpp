#include <iostream>
#include <math.h>

#include <QPainter>
#include <QMouseEvent>

#include "graph_panel.h"
#include "base_graph_node.h"
#include "ui_graph_node.h"






namespace vino_pipeline_graph
{

PipelinePaintWidget::PipelinePaintWidget( QFrame* parent )
  : QWidget( parent ),
  mouse_release(true),
  mouse_press(false)
{
    // this->resize(400,566);
    QPalette   palette;
    palette.setBrush(QPalette::Window, QBrush(Qt::white));
    this->setPalette(palette);
    this->setAutoFillBackground(true);


    //test vino_param
    std::string FLAGS_config;
    std::string prefix_path;

    prefix_path = ros::package::getPath("vino_param_lib");
    FLAGS_config = prefix_path + "/param/pipeline.yaml";
    std::cout  << "FLAGS_config=" << FLAGS_config << std::endl;

    graph = new UIPipelineGraph(this, 640,480);

    graph->parsePipeline(FLAGS_config);

    // graph->makeRoot("StandardCamera");
    // graph->makeConnection("StandardCamera","ObjectDetection");
    // graph->makeConnection("ObjectDetection","ImageWindow");
    // graph->makeConnection("ObjectDetection","Rviz");
    // graph->makeConnection("ObjectDetection","Rostopic");
    // graph->makeConnection("ObjectDetection","FaceDetection");
    // graph->makeConnection("FaceDetection","Face Re-ID");
    // graph->makeConnection("Face Re-ID","ImageWindow");
    // graph->makeConnection("FaceDetection","ImageWindow");
    // graph->makeConnection("FaceDetection","Tracking");
    // graph->makeConnection("Tracking","Rviz");
    // graph->makeConnection("FaceDetection","111");
    // graph->makeConnection("ObjectDetection","???");
    // graph.printAllNodes();
    // graph.printAllEdges();
    // graph.printIsolatedNodes();
    
    // std::cout << "Max depth: " << graph.getMaxDepth() << std::endl;
    // std::cout << "Max width: " << graph.getMaxWidth() << std::endl;
    
    // graph.removeConnection("ObjectDetection","FaceDetection");
    // graph.removeNode("FaceDetection");
    // graph.printAllEdges();
    // graph.printAllEdges();
    // graph.printIsolatedNodes();



    


   
    
}

// This paintEvent() is complex because of the drawing of the two
// arc-arrows representing wheel motion.  It is not particularly
// relevant to learning how to make an RViz plugin, so I will kind of
// skim it.
void PipelinePaintWidget::paintEvent( QPaintEvent* event )
{
    QSize window_size = this->size();
    if(window_size.width()<50 || window_size.height()<200){
        QPainter painter(this);
        painter.setPen(Qt::blue);
        painter.drawText(5,5,"Resize to view it.");
    }
    graph->setCanvasSize(window_size.width(),window_size.height());


    graph->drawGraph();
    
    if(isAddingEdge)
    {
        
        graph->drawLink();
       
    }



}

void PipelinePaintWidget::AddEdge(void)
{
    isAddingEdge = true;
    std::cout << "ADD edge" << std::endl;
}

void PipelinePaintWidget::mousePressEvent(QMouseEvent *event)
{
    mouse_release = false;
   
    if(isAddingEdge)
    {
        graph->createEdgeBySelection();
        isAddingEdge = false;
        update();
        return;
    }

    if (event->button() == Qt::LeftButton){
        std::cout <<"Cick: " <<event->pos().x() << ","<<  event->pos().y() << std::endl;

        pos_mouse_press  = event->pos();

        graph->selectdNode(event->pos());

        graph->selectEdge(event->pos());
        update();
		//setMouseState( Qt::MouseState::L_C, 0);
	}

    
    
    
}

void PipelinePaintWidget::mouseReleaseEvent(QMouseEvent *event)
{
    
    mouse_release = true;
   
    
}

void PipelinePaintWidget::resizeEvent(QResizeEvent * event)
{
    // QSize window_size = event->size();
    // if(window_size.width()<50 || window_size.height()<200){
    //     QPainter painter(this);
    //     painter.setPen(Qt::blue);
    //     painter.drawText(5,5,"Resize to view it.");
    // }
}
void PipelinePaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(mouse_release!= true)
    {
        graph->drawMovedNode(event->pos() , pos_mouse_press);
        pos_mouse_press = event->pos();
        update();
    }

    if(isAddingEdge)
    {
        graph->setNodeLinkTo(event->pos());
        update();
    }
}

} // end namespace rviz_plugin_tutorials
