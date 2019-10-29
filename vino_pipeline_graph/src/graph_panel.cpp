#include <iostream>
#include <math.h>

#include <QPainter>
#include <QMouseEvent>
#include <QObject>
#include <QGridLayout>
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

    // prefix_path = ros::package::getPath("vino_param_lib");
    // FLAGS_config = prefix_path + "/param/pipeline.yaml";
    // std::cout  << "FLAGS_config=" << FLAGS_config << std::endl;


   
}


// This paintEvent() is complex because of the drawing of the two
// arc-arrows representing wheel motion.  It is not particularly
// relevant to learning how to make an RViz plugin, so I will kind of
// skim it.
std::vector<std::string> PipelinePaintWidget::loadPipeline(std::string file_path)
{
    


    //To-do refector code 
    //try to get pipeline numbers 
    Params::ParamManager::getInstance().parse(file_path);
    auto pipelines = Params::ParamManager::getInstance().getPipelines();

    std::cout << pipelines.size() << std::endl;

    std::vector<std::string> pipeline_names;
    for( int i=0;i< pipelines.size();i++)
    {
        
        UIPipelineGraph * pipeline_graph =  new UIPipelineGraph(this, 640,480);
        pipeline_graph->parsePipeline(file_path,i);

        pipeline_names.push_back(pipeline_graph->getPipelineName());
        graphs.push_back(pipeline_graph);
        // graph= new UIPipelineGraph(this, 640,480);
        // graph->parsePipeline(file_path);
  
    }

    graph= graphs[0];

   
    isResize = true;
    update();
    return pipeline_names;

}

void PipelinePaintWidget::createNewPipeline(std::string pipeline_name,std::string input_node_name)
{
    Node::NodeParams params;
    params.type = Node::NodeType::Input;
    params.name = input_node_name;
    UIPipelineGraph * pipeline_graph =  new UIPipelineGraph(this, 640,480);
    pipeline_graph->setPipelineName(pipeline_name);
    pipeline_graph->makeRoot(input_node_name);
    pipeline_graph->setNodeParams(input_node_name,params);
    graphs.push_back(pipeline_graph);
    graph= graphs[graphs.size()-1];
    isResize = true;
    update();
}
void PipelinePaintWidget::savePipeline(std::string file_path)
{
    std::vector< Params::ParamManager::PipelineParams> pipelines_params;
    for(int i=0;i< graphs.size();i++)
    {
        
        auto pipe_param = graphs[i]->exportPipeline(file_path);
        pipelines_params.push_back(pipe_param);
    }
    
    Params::ParamManager::getInstance().save(pipelines_params,file_path);
  
}
void PipelinePaintWidget::paintEvent( QPaintEvent* event )
{
    if(!graph) return;

    QSize window_size = this->size(); 
    //  std::cout << "!!!!!!!!" << "paint event" <<  
    // window_size.width() << " , " <<window_size.height()<< std::endl;
    if(window_size.width()<50 || window_size.height()<200){
        QPainter painter(this);
        painter.setPen(Qt::blue);
        painter.drawText(5,5,"Resize to view it.");
    }
    graph->setCanvasSize(window_size.width(),window_size.height());

    if(isResize) 
    {
         graph->drawGraph(true);
         isResize = false;
         update();
    }
    else
    {
        graph->drawGraph(false);
    }
    

    if(isAddingEdge)
    {
        
        graph->drawLink();
       
    }



}

void PipelinePaintWidget::addEdge(void)
{
    if(!graph) return;
    isAddingEdge = true;
 
}
void PipelinePaintWidget::addNode(vino_pipeline_graph::Node::NodeParams params)
{
    if(!graph) return;
    graph->addNewNode(params);
}



void PipelinePaintWidget::mousePressEvent(QMouseEvent *event)
{
    if(!graph) return;

    mouse_release = false;
   
    if(isAddingEdge)
    {
        graph->createEdgeBySelection();
        isAddingEdge = false;
        update();
        return;
    }

    if (event->button() == Qt::LeftButton){
        

        pos_mouse_press  = event->pos();

        bool is_select_node = graph->selectdNode(event->pos());
        bool is_select_edge = graph->selectEdge(event->pos());
        
        
        if(is_select_node) 
        {
            auto params = graph->getSelectedNodeParams();
           
            Q_EMIT __graph_select_node(params);
        }
        else
        {
           if(is_select_edge) Q_EMIT __graph_select_edge();
           Q_EMIT __graph_loose_focus_node();
           
        }
        
        update();
		//setMouseState( Qt::MouseState::L_C, 0);
	}
}

void PipelinePaintWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    //double click do nothing
    update();
}


void PipelinePaintWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(!graph) return;

    mouse_release = true;
   
   
    
}

void PipelinePaintWidget::resizeEvent(QResizeEvent * event)
{
    if(!graph) return;
    isResize = true;
    
    // QSize window_size = event->size();
    // if(window_size.width()<50 || window_size.height()<200){
    //     QPainter painter(this);
    //     painter.setPen(Qt::blue);
    //     painter.drawText(5,5,"Resize to view it.");
    // }
}
void PipelinePaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(!graph) return;
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
