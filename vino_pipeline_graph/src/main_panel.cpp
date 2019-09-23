#include <stdio.h>


#include <geometry_msgs/Twist.h>
#include <QCoreApplication>


#include "main_panel.h"




namespace vino_pipeline_graph
{


MainPanel::MainPanel( QWidget* parent )
  : rviz::Panel( parent )
{
  //locate rviz resources file path
  QString resource_path_prefix = QCoreApplication::applicationDirPath () + "/../share/rviz/icons/";


  std::cout << "QT: "<< resource_path_prefix.toStdString()<< std::endl;
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
  combo_pipeline_names = new QComboBox();
  //to-do change path


  connect(combo_pipeline_names, SIGNAL (activated(int)), this, SLOT (selectPipelineHandler()));
  controller_layout->addWidget( combo_pipeline_names);

  btn_add_pipeline=new QPushButton();
  QPixmap pix_add(resource_path_prefix + "plus.png");
  btn_add_pipeline->setToolTip( "Add a new pipeline" );
  btn_add_pipeline->setIcon(QIcon(pix_add));
  controller_layout->addWidget(btn_add_pipeline);
  connect(btn_add_pipeline, SIGNAL (released()), this, SLOT (createPipelineHandler()));


  btn_del_pipeline=new QPushButton();
  QPixmap pix_del(resource_path_prefix + "minus.png");
  btn_del_pipeline->setIcon(QIcon(pix_del));
  controller_layout->addWidget(btn_del_pipeline);

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

  controller_layout = new QHBoxLayout;
  

  QGridLayout *pLayout = new QGridLayout();
  pLayout->addWidget(pipeline_widget, 0, 0, 0, 4);
  attribute_layout = new QVBoxLayout;

  //attribute param name
  QHBoxLayout* attri_name_layout = new QHBoxLayout;
  attri_name_layout ->addWidget(new QLabel("Name :"));
  edit_name = new QLineEdit();
  edit_name->setReadOnly(true);
  attri_name_layout->addWidget(edit_name);
  attribute_layout->addLayout(attri_name_layout);

  //attribute param node_type
  QHBoxLayout* attri_type_layout = new QHBoxLayout;
  attri_type_layout ->addWidget(new QLabel("Type  :"));
  edit_type = new QLineEdit();
  attri_type_layout->addWidget(edit_type);
  attribute_layout->addLayout(attri_type_layout);

  //attribute parm model 
  QHBoxLayout* attri_model_layout = new QHBoxLayout;
  attri_model_layout ->addWidget(new QLabel("Model :"));
  edit_model = new QLineEdit();
  attri_model_layout->addWidget(edit_model);
  attribute_layout->addLayout(attri_model_layout);

  //attribute param engine
  QHBoxLayout* attri_engine_layout = new QHBoxLayout;
  attri_engine_layout ->addWidget(new QLabel("Engine:"));
  edit_engine = new QLineEdit();
  attri_engine_layout->addWidget(edit_engine);
  attribute_layout->addLayout(attri_engine_layout);

   //attribute param engine
  QHBoxLayout* attri_label_layout = new QHBoxLayout;
  attri_label_layout ->addWidget(new QLabel("Label  :"));
  edit_label = new QLineEdit();
  attri_label_layout->addWidget(edit_label);
  attribute_layout->addLayout(attri_label_layout);
  
  QHBoxLayout* attr_action_layout = new QHBoxLayout;
  btn_clear = new QPushButton("Clear");
  btn_apply = new QPushButton("Apply");
  attr_action_layout->addWidget(btn_clear);
  attr_action_layout->addWidget(btn_apply);
  attribute_layout->addLayout( attr_action_layout);

  connect(btn_apply, SIGNAL (released()), this, SLOT (applyChangeBtnHandler()));

  pLayout->addLayout(attribute_layout,0,4,Qt::AlignTop);

  main_layout->addLayout( pLayout );


   //main_layout->addWidget(  pipeline_widget);

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
  connect(pipeline_widget, SIGNAL (__graph_loose_focus_node()), this, SLOT (clearAttributeDisplay()));
  
  connect(pipeline_widget, SIGNAL (__graph_select_node(vino_pipeline_graph::Node::NodeParams)), this, SLOT (updateAttributeDisplay(vino_pipeline_graph::Node::NodeParams)));
  //connect(pipeline_widget, SIGNAL (__graph_select_edge()), this, SLOT (updateAttributeDisplay()));
  // Start the timer.
  output_timer->start( 100 );


}

void MainPanel::updateAttributeDisplay(vino_pipeline_graph::Node::NodeParams params)
{
  edit_name->setText(QString::fromStdString(params.name));
  edit_model->setText(QString::fromStdString(params.model));
  edit_engine->setText(QString::fromStdString(params.engine));
  edit_label->setText(QString::fromStdString(params.label));

}
void MainPanel::clearAttributeDisplay()
{
  edit_name->setText(QString(""));
  edit_model->setText(QString(""));
  edit_engine->setText(QString(""));
  edit_label->setText(QString(""));

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

  auto pipeline_names = pipeline_widget->loadPipeline(file_name);
  combo_pipeline_names->clear();
  for(int i=0; i<pipeline_names.size();i++)
      combo_pipeline_names->addItem(QString::fromStdString(pipeline_names[i]));

}

void MainPanel::createPipelineHandler(void)
{
  
    QDialog dialog(this);
    dialog.setWindowTitle(QString("Create a new pipeline"));
    QFormLayout form(&dialog);
    // Pipeline name
    QLineEdit *edit_node_name = new QLineEdit(&dialog);
    form.addRow(QString("Pipeline name: "), edit_node_name);
    // Input from
    QComboBox *combo_input_name = new QComboBox(&dialog);
    //to-do from a configure file
    combo_input_name->addItem("StandardCamera");
    combo_input_name->addItem("RealSenseCamera");

    form.addRow(QString("Input type: "), combo_input_name);
    



    QLineEdit *edit_camera_topic = new QLineEdit(&dialog);
    form.addRow(QString("Camera_topic: "), edit_camera_topic);
    QLineEdit *edit_input_path = new QLineEdit(&dialog);
    form.addRow(QString("Input path: "), edit_input_path);
    QLineEdit *edit_custom_cpu_library = new QLineEdit(&dialog);
    form.addRow(QString("Custom_cpu_library: "), edit_custom_cpu_library);
    QComboBox *combo_enable_performance_count = new QComboBox(&dialog);
    combo_enable_performance_count->addItem("true");
    combo_enable_performance_count->addItem("false");
    form.addRow(QString("Enable_performance_count: "), combo_enable_performance_count);

    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    if (dialog.exec() == QDialog::Accepted)
    {
      pipeline_widget->createNewPipeline(edit_node_name->text().toStdString(),
        combo_input_name->currentText().toStdString());
      combo_pipeline_names->addItem(edit_node_name->text());
      auto num_pipelines = combo_pipeline_names->count();
      combo_pipeline_names->setCurrentIndex(num_pipelines-1);
        
    }

    
}
void MainPanel::selectPipelineHandler(void)
{
 
    int index =  combo_pipeline_names->currentIndex();
    std::cout << "select " << index << std::endl;
    pipeline_widget->graph = pipeline_widget->graphs[index];
    pipeline_widget->isResize = true;
    pipeline_widget->draw();    
  
  

}

void MainPanel::savePipelineBtnHandler(void)
{
    std::string file_name = "/home/intel/Code/test.yaml";
    pipeline_widget->savePipeline(file_name);
}

void MainPanel::applyChangeBtnHandler(void)
{
 
  vino_pipeline_graph::Node::NodeParams params;
  params.name = edit_name->text().toStdString();
  params.model = edit_model->text().toStdString();
  params.engine = edit_engine->text().toStdString();
  params.label = edit_label->text().toStdString();

  pipeline_widget->graph->setSelectedNodeParams(params);
  
}
  void MainPanel::addNodeBtnHandler(void)
  {
    if(!pipeline_widget->graph) return;

    QDialog dialog(this);
    dialog.setWindowTitle(QString("Add a new node"));
    QFormLayout form(&dialog);
    // Name
    QLineEdit *edit_node_name = new QLineEdit(&dialog);
    form.addRow(QString("Node name: "), edit_node_name);
    // Type
    QComboBox *combo_node_type = new QComboBox(&dialog);
    QStringList nodeTypeList;
    nodeTypeList<<"Input"<<"Inference"<<"Output";
    combo_node_type->addItems(nodeTypeList);
    form.addRow(QString("Node Type: "), combo_node_type);
    //Model
    QLineEdit *edit_model = new QLineEdit(&dialog);
    form.addRow(QString("Node Model: "), edit_model);
    //Engine
    QComboBox *combo_engine = new QComboBox(&dialog);
    QStringList engineList;
    engineList<<"CPU"<<"GPU"<<"Myriad";
    combo_engine->addItems(engineList);
    form.addRow(QString("Node Engine: "),combo_engine);
    //label
    QLineEdit *edit_node_label = new QLineEdit(&dialog);
    form.addRow(QString("Node label: "), edit_node_label);

    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));


    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted) {
       vino_pipeline_graph::Node::NodeParams params;
       params.name = edit_node_name->text().toStdString() ;
       params.type = (vino_pipeline_graph::Node::NodeType) combo_node_type->currentIndex();
       params.engine =  combo_engine->currentText().toStdString();
       params.model = edit_model->text().toStdString() ;
       params.label = edit_node_label->text().toStdString() ;
       pipeline_widget->addNode(params);
           
    }

      // QInputDialog new_node_dialog(this); 
      // new_node_dialog.setWindowTitle("Add a new node"); 
      // new_node_dialog.setLabelText("Node name:"); 
      // new_node_dialog.setInputMode(QInputDialog::TextInput);
      // if(new_node_dialog.exec() == QInputDialog::Accepted)
      // {
      //     std::string node_name = new_node_dialog.textValue().toStdString();
 
      //     pipeline_widget->addNode(node_name);
      // }
      pipeline_widget->draw();
     
  }
  void MainPanel::addEdgeBtnHandler(void)
  {
      if(!pipeline_widget->graph) return;
      pipeline_widget->addEdge();
  }
  void MainPanel::removeBtnHandler(void)
  {
      if(!pipeline_widget->graph) return;
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
