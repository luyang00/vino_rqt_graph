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
  
   
  btn_load_pipeline = new QPushButton("Load");
  btn_save_pipeline = new QPushButton("Save");
  connect(btn_load_pipeline, SIGNAL (released()), this, SLOT (loadPipelineBtnHandler()));
  connect(btn_save_pipeline, SIGNAL (released()), this, SLOT (savePipelineBtnHandler()));

  controller_layout ->addWidget( btn_load_pipeline);
  controller_layout ->addWidget(btn_save_pipeline );

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

    
  // QScrollArea *pScroll = new QScrollArea(this);
  
  // pScroll->setWidget(  pipeline_widget);
  // pScroll->setWidgetResizable(true);
  // pScroll->setGeometry(0,0,200,200);//要显示的区域大小
  // //PipelinePaintWidget->setWidgetResizable(true);
  // //pipeline_widget->setGeometry(0,0,640,480);//这里变大后，看出他实际滚动的是里面的QWidget窗口


  
   main_layout->addWidget(  pipeline_widget);

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

void MainPanel::loadPipelineBtnHandler(void)
{
  std::string file_name = QFileDialog::getOpenFileName(NULL,"Load pipeline file(*.yaml)",".","*.yaml").toStdString();
//  std::cout << file_name.toStdString() << std::endl;
  pipeline_widget->loadPipeline(file_name);
    // QFileDialog *fileDialog = new QFileDialog(this);
    // fileDialog->setWindowTitle(tr("Open pipeline file(*.yaml)"));
    // fileDialog->setDirectory(".");
    //  fileDialog->setNameFilter(tr("Pipeline(*.yaml)"));
    //  fileDialog->setFileMode(QFileDialog::ExistingFiles);
    //  fileDialog->setViewMode(QFileDialog::Detail);
     
    //  QStringList fileNames;
    //  if(fileDialog->exec())
    //  {
    //    fileNames = fileDialog->selectedFiles();
    //    std::cout << fileNames[0]<< std::end;
    //  }
}

void MainPanel::savePipelineBtnHandler(void)
{
    pipeline_widget->savePipeline();
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
 
          pipeline_widget->addNode(node_name);
      }
      pipeline_widget->draw();
     
  }
  void MainPanel::addEdgeBtnHandler(void)
  {
      pipeline_widget->addEdge();
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
