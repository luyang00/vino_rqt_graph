#include <stdio.h>


#include <geometry_msgs/Twist.h>

#include "main_panel.h"




namespace vino_pipeline_graph
{


MainPanel::MainPanel( QWidget* parent )
  : rviz::Panel( parent )
{

  main_layout = new QVBoxLayout;


  // Next we lay out the "output topic" text entry field using a
  // QLabel and a QLineEdit in a QHBoxLayout.
  
  controller_layout = new QHBoxLayout;
  controller_layout->addWidget( new QLabel( "pipeline:" ));
  controller_layout->addWidget( new  QComboBox());
  controller_layout->addWidget( new QPushButton("Start"));
  controller_layout->addWidget( new QPushButton("Pause"));
  controller_layout->addWidget( new QPushButton("Stop"));
  main_layout->addLayout( controller_layout );

  
  editor_layout = new QHBoxLayout;

  btn_addNode = new QPushButton("Add Node");
  btn_addEdge = new QPushButton("Add Edge");
  btn_remove = new QPushButton("Remove");
  connect(btn_addNode, SIGNAL (released()), this, SLOT (addNodeBtnHandler()));
  connect(btn_addEdge, SIGNAL (released()), this, SLOT (addEdgeBtnHandler()));
  connect(btn_remove, SIGNAL (released()), this, SLOT (removeBtnHandler()));



  editor_layout ->addWidget(btn_addNode);
  editor_layout ->addWidget(btn_addEdge);
  editor_layout ->addWidget(btn_remove);
  main_layout->addLayout( editor_layout );

  
  
  pipeline_widget = new PipelinePaintWidget;
  pipeline_widget->setMouseTracking(true);  
  
   main_layout->addWidget( pipeline_widget);

   setLayout( main_layout );

   QTimer* output_timer = new QTimer( this );



    // palette.setBrush(frame->backgroundRole(),QBrush(pixmap));
    // frame->setPalette(palette);
    // frame->setMask(pixmap.mask());  //可以将图片中透明部分显示为透明的
    // frame->setAutoFillBackground(true);
    // frame->setWindowTitle("HelloWord");
    // frame->setWindowFlags(Qt::FramelessWindowHint);//去掉最小化，关闭按钮
 



  // Next we make signal/slot connections.
//   connect( drive_widget_, SIGNAL( outputVelocity( float, float )), this, SLOT( setVel( float, float )));
//   connect( output_topic_editor_, SIGNAL( editingFinished() ), this, SLOT( updateTopic() ));
//   connect( output_timer, SIGNAL( timeout() ), this, SLOT( sendVel() ));

  // Start the timer.
  output_timer->start( 100 );


}

void MainPanel::paintEvent( QPaintEvent* event )
{
    QSize q = pipeline_widget->size();
   // std::cout << q.width() << "," << q.height() << std::endl;  
    // QPushButton *qb = new QPushButton("what??");
    // main_layout->addWidget( qb);
    // setLayout( main_layout );
    
}
// Save all configuration data from this panel to the given
// Config object.  It is important here that you call save()
// on the parent class so the class id and panel name get saved.
void MainPanel::save( rviz::Config config ) const
{
  rviz::Panel::save( config );
}

// Load all configuration data for this panel from the given Config object.
void MainPanel::load( const rviz::Config& config )
{
  rviz::Panel::load( config );
  QString topic;
  if( config.mapGetString( "Topic", &topic ))
  {
    // output_topic_editor_->setText( topic );
    // updateTopic();
  }
}


  void MainPanel::addNodeBtnHandler(void)
  {
      QInputDialog new_node_dialog(this); 
      new_node_dialog.setWindowTitle("Add a new node"); 
      new_node_dialog.setLabelText("Node name:"); 
      new_node_dialog.setInputMode(QInputDialog::TextInput);
      if(new_node_dialog.exec() == QInputDialog::Accepted)
      {
          std::string node_name = new_node_dialog.textValue().toStdString();
 
          pipeline_widget->graph->AddNewNode(node_name);
      }
      pipeline_widget->draw();
     
  }
  void MainPanel::addEdgeBtnHandler(void)
  {
      pipeline_widget->AddEdge();
  }
  void MainPanel::removeBtnHandler(void)
  {
      pipeline_widget->graph->RemoveSelectedElement();
      pipeline_widget->draw();
  }

} // end namespace vino_pipeline_graph

// Tell pluginlib about this class.  Every class which should be
// loadable by pluginlib::ClassLoader must have these two lines
// compiled in its .cpp file, outside of any namespace scope.
#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(vino_pipeline_graph::MainPanel,rviz::Panel )
// END_TUTORIAL
