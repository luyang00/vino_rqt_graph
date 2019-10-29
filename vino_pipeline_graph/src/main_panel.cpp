#include <stdio.h>


#include <geometry_msgs/Twist.h>
#include <QCoreApplication>


#include "main_panel.h"




namespace vino_pipeline_graph
{


MainPanel::MainPanel( QWidget* parent )
  : rviz::Panel( parent )
{
  

  main_layout = new QVBoxLayout;
  
  controller_layout = new QHBoxLayout;

  controller_layout->addWidget( new QLabel( "pipeline:" ));
  combo_pipeline_names = new QComboBox();
  connect(combo_pipeline_names, SIGNAL (currentIndexChanged(int)), this, SLOT (selectPipelineHandler(int)));
  controller_layout->addWidget( combo_pipeline_names);

  initMenuBtn();

  btn_save_pipeline = new QPushButton("Save");
  connect(btn_save_pipeline, SIGNAL (released()), this, SLOT (savePipelineBtnHandler()));
  controller_layout ->addWidget(btn_save_pipeline );



  btn_startPipeline = new QPushButton("Start");
  btn_pausePipeline = new QPushButton("Pause");
  btn_stopPipeline = new QPushButton("Stop");
  connect(btn_startPipeline, SIGNAL (released()), this, SLOT (startPipelineHandler()));
  connect(btn_pausePipeline, SIGNAL (released()), this, SLOT (pausePipelineHandler()));
  connect(btn_stopPipeline , SIGNAL (released()), this, SLOT (stopPipelineHandler()));

  controller_layout->addWidget( btn_startPipeline);
  controller_layout->addWidget( btn_pausePipeline);
  controller_layout->addWidget( btn_stopPipeline);
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
  label_name = new QLabel("Name :");
  attri_name_layout ->addWidget(label_name);
  edit_name = new QLineEdit();
  edit_name->setReadOnly(true);
  attri_name_layout->addWidget(edit_name);
  attribute_layout->addLayout(attri_name_layout);

  //attribute param node_type
  QHBoxLayout* attri_type_layout = new QHBoxLayout;
  label_type = new QLabel("Type  :");
  attri_type_layout ->addWidget(label_type);
  edit_type = new QLineEdit();
  edit_type->setReadOnly(true);
  //edit_type->setDisabled(true);
  attri_type_layout->addWidget(edit_type);
  attribute_layout->addLayout(attri_type_layout);

  //attribute parm model 
  QHBoxLayout* attri_model_layout = new QHBoxLayout;
  label_model = new QLabel("Model :");
  attri_model_layout ->addWidget(label_model);
  edit_model = new QLineEdit();
  attri_model_layout->addWidget(edit_model);
  attribute_layout->addLayout(attri_model_layout);

  //attribute param engine
  QHBoxLayout* attri_engine_layout = new QHBoxLayout;
  label_engine = new QLabel("Engine:");
  attri_engine_layout ->addWidget(label_engine);
  edit_engine = new QLineEdit();
  attri_engine_layout->addWidget(edit_engine);
  attribute_layout->addLayout(attri_engine_layout);

   //attribute param engine
  QHBoxLayout* attri_label_layout = new QHBoxLayout;
  label_label = new QLabel("Label  :");
  attri_label_layout ->addWidget(label_label);
  edit_label = new QLineEdit();
  attri_label_layout->addWidget(edit_label);
  attribute_layout->addLayout(attri_label_layout);
  
  QHBoxLayout* attr_action_layout = new QHBoxLayout;

  btn_apply = new QPushButton("Update");
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



  pipeline_srv = new vino_service::PipelineService("/openvino_toolkit/pipeline/service");
  

}


void MainPanel::initMenuBtn(void)
{

    //locate rviz resources file path
  QString resource_path_prefix = QCoreApplication::applicationDirPath () + "/../share/rviz/icons/";


  std::cout << "QT: "<< resource_path_prefix.toStdString()<< std::endl;

  btn_add_pipeline=new QPushButton();
  QPixmap pix_add(resource_path_prefix + "plus.png");
  btn_add_pipeline->setToolTip( "Add a new pipeline" );
  btn_add_pipeline->setIcon(QIcon(pix_add));
  controller_layout->addWidget(btn_add_pipeline);

  menu = new QMenu(this);
  act_load_from_file = new QAction(menu);
  act_create_from_new = new QAction(menu);
  menu_create_from_example = new QMenu("create from examples");
  

  act_load_from_file->setText(QString("load from a pipeline file(*.yaml)"));
  act_create_from_new->setText(QString("create a new pipeline"));  


  menu->addAction(act_load_from_file);
  menu->addSeparator();
  menu->addAction(act_create_from_new);
  menu->addMenu(menu_create_from_example);
  btn_add_pipeline->setMenu(menu);

  connect(act_load_from_file, SIGNAL (triggered()), this, SLOT (loadPipelineBtnHandler()));
  connect(menu_create_from_example, SIGNAL (aboutToShow()), this, SLOT (loadMenuExamplesHandler()));
  connect(act_create_from_new, SIGNAL (triggered()), this, SLOT (createPipelineHandler()));


  btn_del_pipeline=new QPushButton();
  QPixmap pix_del(resource_path_prefix + "minus.png");
  btn_del_pipeline->setIcon(QIcon(pix_del));
  controller_layout->addWidget(btn_del_pipeline);
  connect(btn_del_pipeline,SIGNAL(released()),this,SLOT(removePipelineHandler()));
}


void MainPanel::updateAttributeDisplay(vino_pipeline_graph::Node::NodeParams params)
{
  btn_apply->setEnabled(true);
  edit_name->setText(QString::fromStdString(params.name));

  if(params.type == vino_pipeline_graph::Node::NodeType::Input )
  {
      label_model->hide();
      label_engine->hide();
      label_label->hide();
      edit_model->hide();
      edit_engine->hide();
      edit_label->hide();

      edit_type->setText(QString("Input"));

  }
  else if(params.type == vino_pipeline_graph::Node::NodeType::Infer )
  {   
      label_model->show();
      label_engine->show();
      label_label->show();
      edit_model->show();
      edit_engine->show();
      edit_label->show();

      edit_model->setText(QString::fromStdString(params.model));
      edit_engine->setText(QString::fromStdString(params.engine));
      edit_label->setText(QString::fromStdString(params.label));
      
      edit_type->setText(QString("Inference"));
  }
  else if(params.type == vino_pipeline_graph::Node::NodeType::Output)
  {
      label_model->hide();
      label_engine->hide();
      label_label->hide();

      edit_model->hide();
      edit_engine->hide();
      edit_label->hide();

      edit_type->setText(QString("Output"));
      
  }

}
void MainPanel::clearAttributeDisplay()
{
  btn_apply->setDisabled(true);
  edit_name->setText(QString(""));
  edit_model->setText(QString(""));
  edit_engine->setText(QString(""));
  edit_label->setText(QString(""));
  edit_type->setText(QString(""));

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
  std::string filename = QFileDialog::getOpenFileName(NULL,"Load pipeline file(*.yaml)",".","*.yaml").toStdString();

  loadPipeline(filename);

}
void MainPanel::loadPipeline(std::string filename)
{
  auto pipeline_names = pipeline_widget->loadPipeline(filename);
  //combo_pipeline_names->clear();
  for(int i=0; i<pipeline_names.size();i++)
      combo_pipeline_names->addItem(QString::fromStdString(pipeline_names[i]));
  
  auto current_pipelines = combo_pipeline_names->count() - 1;
  combo_pipeline_names->setCurrentIndex(current_pipelines);

}
void MainPanel::loadMenuExamplesHandler(void)
{
  
  std::string path = ros::package::getPath("vino_launch") + "/param/";

  QDir source(QString::fromStdString(path));

  if(!source.exists())
  {
    QMessageBox messageBox;
    messageBox.critical(0,"Pipeline examples not found!","Please check vino_launch package and vino_launch/param/ exist");
    return;
  }
  else{

  
      QStringList const files = source.entryList(QStringList() << "*.yaml", QDir::Files);
      
      if(menu_create_from_example->actions().size() >=files.size()) return ;
      for( auto & f : files)
      {
        auto filename = path + f.toStdString();
        menu_create_from_example->addAction(f, [this,filename](){this->loadPipeline(filename);});
      }
  } 
}


void MainPanel::removePipelineHandler(void)
{
    int index =  combo_pipeline_names->currentIndex();
    
    pipeline_widget->graphs.erase(pipeline_widget->graphs.begin() + index);
    combo_pipeline_names->removeItem(index);
    
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
void MainPanel::selectPipelineHandler(int)
{
 
    int index =  combo_pipeline_names->currentIndex();
    std::cout << "index select: " << index << std::endl;
    if(index >= 0)
    { 
      pipeline_widget->graph = pipeline_widget->graphs[index];
      pipeline_widget->isResize = true;
      pipeline_widget->draw();    
    }
    else
    {
      pipeline_widget->graph = NULL;
      pipeline_widget->draw();
    }
  
  

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
  //to-do change str to marco
  if(edit_type->text() == "Inference" ){
 
    params.model = edit_model->text().toStdString();
    params.engine = edit_engine->text().toStdString();
    params.label = edit_label->text().toStdString();
    params.type = vino_pipeline_graph::Node::NodeType::Infer;
  }
  else if(edit_type->text() == "Output" )
  {
    params.type = vino_pipeline_graph::Node::NodeType::Output;
  }
  else if(edit_type->text() == "Input" )
  {
    params.type = vino_pipeline_graph::Node::NodeType::Input;
  }
  
  pipeline_widget->graph->setSelectedNodeParams(params);
  
}
  void MainPanel::addNodeBtnHandler(void)
  {
    if(!pipeline_widget->graph) return;

    //load all supported models from config files
    Params::ParamManager::getInstance().parseConfs("/home/intel/catkin_ws2/src/vino_rviz_plugin/vino_pipeline_graph/models.yaml");
    auto infers_supported = Params::ParamManager::getInstance().getInfersSupported();


    QDialog dialog(this);
    dialog.setWindowTitle(QString("Add a new node"));
    QFormLayout form(&dialog);
     // Type
    combo_node_type = new QComboBox(&dialog);
    QStringList nodeTypeList;
    nodeTypeList<<"Inference"<<"Output";
    combo_node_type->addItems(nodeTypeList);
    form.addRow(QString("Node Type: "), combo_node_type);
    connect(combo_node_type, SIGNAL(currentIndexChanged(int)),this, SLOT(selectNodeTypeHandler(int)));
    // Name
    edit_node_name = new QLineEdit(&dialog);
    label_node_name = new QLabel("Node name:");
    form.addRow(label_node_name, edit_node_name);
    //Model
    combo_node_model = new QComboBox(&dialog);
    QStringList modelList;
    for(auto & infer : infers_supported)  modelList << QString::fromStdString(infer.name); 
    combo_node_model->addItems(modelList);


    edit_node_model = new QLineEdit(&dialog);
    QHBoxLayout* model_layout = new QHBoxLayout;
    model_layout ->addWidget(combo_node_model);
    model_layout ->addWidget(edit_node_model );
    label_node_model = new QLabel("Infer model:");
    form.addRow(label_node_model, model_layout);
    connect(combo_node_model, SIGNAL(currentIndexChanged(int)),this, SLOT(selectModelTypeHandler(int)));

    //Engine
    combo_node_engine = new QComboBox(&dialog);
    label_node_engine = new QLabel("Node Engine:");
    QStringList engineList;
    engineList<<"CPU"<<"GPU"<<"Myriad";
    combo_node_engine->addItems(engineList);
    form.addRow(label_node_engine,combo_node_engine);
    //label
    label_node_label = new QLabel("Node label:");
    edit_node_label = new QLineEdit(&dialog);
    form.addRow(label_node_label, edit_node_label);
    //Output type
    label_node_output_type = new QLabel("Output Type");
    combo_node_output_type = new  QComboBox(&dialog);
    QStringList outputTypeList;
    outputTypeList<<"Rviz"<<"ImageWindow"<<"Rostopic";
    combo_node_output_type->addItems(outputTypeList);
    form.addRow(label_node_output_type, combo_node_output_type);
    label_node_output_type->hide();
    combo_node_output_type->hide();
    


    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));


    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted) {
       vino_pipeline_graph::Node::NodeParams params;

       if(combo_node_type->currentText() == "Inference")
       {
            params.name = edit_node_name->text().toStdString();
            params.type = vino_pipeline_graph::Node::NodeType::Infer;//skip input from infer to output
            params.engine =  combo_node_engine->currentText().toStdString();
            params.model = edit_node_model->text().toStdString();
            params.label = edit_node_label->text().toStdString();
            pipeline_widget->addNode(params);
       }
       else if(combo_node_type->currentText() == "Output" )
       {
            params.name = combo_node_output_type->currentText().toStdString() ;
            params.type = vino_pipeline_graph::Node::NodeType::Output;
            pipeline_widget->addNode(params);
       }
           
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

  void MainPanel::selectNodeTypeHandler(int index)
  {
      if(combo_node_type->currentText() == "Inference")
      {
        //For inference node
        edit_node_name->show();
        label_node_name->show();

        combo_node_model->show();
        edit_node_model->show();
        label_node_model->show();
        combo_node_engine->show();
        label_node_engine->show();
        label_node_label->show();
        edit_node_label->show();
        //For output node
        label_node_output_type->hide();
        combo_node_output_type->hide();
       
      }
      else if(combo_node_type->currentText() == "Output" )
      {
        edit_node_name->hide();
        label_node_name->hide();
        combo_node_model->hide();
        edit_node_model->hide();
        label_node_model->hide();
        combo_node_engine->hide();
        label_node_engine->hide();
        label_node_label->hide();
        edit_node_label->hide();
        //For output node
        label_node_output_type->show();
        combo_node_output_type->show();
      }
  }
  void MainPanel::selectModelTypeHandler(int index)
  {
      Params::ParamManager::getInstance().parseConfs("/home/intel/catkin_ws2/src/vino_rviz_plugin/vino_pipeline_graph/models.yaml");
      auto infers_supported = Params::ParamManager::getInstance().getInfersSupported();
      for(int i=0;i< infers_supported.size();i++)
      {
        if(index == i) 
        {
          edit_node_model->setText(QString::fromStdString(infers_supported[i].model));
        }
      }
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


  void MainPanel::startPipelineHandler(void)
  {
      if(!pipeline_widget->graph) return;
      pipeline_srv->reqStartPipeline();

  }
  void MainPanel::pausePipelineHandler(void)
  {
      if(!pipeline_widget->graph) return;
      pipeline_srv->reqPausePipeline(pipeline_widget->graph-> getPipelineName());
  }
  void MainPanel::stopPipelineHandler(void)
  {
      if(!pipeline_widget->graph) return;
      pipeline_srv->reqStopPipeline(pipeline_widget->graph-> getPipelineName());
  }


} // end namespace vino_pipeline_graph

// Tell pluginlib about this class.  Every class which should be
// loadable by pluginlib::ClassLoader must have these two lines
// compiled in its .cpp file, outside of any namespace scope.
#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(vino_pipeline_graph::MainPanel,rviz::Panel )
// END_TUTORIAL
