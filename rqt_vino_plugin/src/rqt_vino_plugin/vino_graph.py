import os
import rospy
import rospkg

from python_qt_binding.QtCore import Qt, Signal
from qt_gui.plugin import Plugin
from python_qt_binding import loadUi
from python_qt_binding.QtWidgets import *
from python_qt_binding.QtGui import QStandardItem,QStandardItemModel
from qt_dotgraph.dot_to_qt import DotToQtGenerator

# pydot requires some hacks
#from qt_dotgraph.pydotfactory import PydotFactory



# import pydot

import dotparser


from dotparser import generate_dotcode_from_yaml_file

from vino_param_lib.param_manager_wrapper import ParamManagerWrapper 
from inference_dialog import InferenceDialog
from output_dialog import OutputDialog
from input_dialog import InputDialog
from vino_pydot_factory import VinoPydotFactory
from interactive_graphics_view import InteractiveGraphicsView


class VinoGraph(Plugin):

    def __init__(self, context):
        super(VinoGraph, self).__init__(context)
        # Give QObjects reasonable names
        self.setObjectName('VinoGraph')

        # Process standalone plugin command-line arguments
        from argparse import ArgumentParser
        parser = ArgumentParser()
        # Add argument(s) to the parser.
        parser.add_argument("-q", "--quiet", action="store_true",
                      dest="quiet",
                      help="Put plugin in silent mode")
        args, unknowns = parser.parse_known_args(context.argv())
        if not args.quiet:
            print 'arguments: ', args
            print 'unknowns: ', unknowns

        # Create QWidget
        self._widget = QWidget()
        # Get path to UI file which should be in the "resource" folder of this package
        ui_file = os.path.join(rospkg.RosPack().get_path('rqt_vino_plugin'), 'resource', 'rqt_vino_plugin.ui')
        # Extend the widget with all attributes and children from UI file
        loadUi(ui_file, self._widget,{'InteractiveGraphicsView': InteractiveGraphicsView})
        # Give QObjects reasonable names
        self._widget.setObjectName('VinoGraphUi')
        # Show _widget.windowTitle on left-top of each plugin (when 
        # it's set in _widget). This is useful when you open multiple  QListView 
        # plugins at once. Also if you open multiple instances of your 
        # plugin at once, these lines add number to make it easy to 
        # tell from pane to pane.
        if context.serial_number() > 1:
            self._widget.setWindowTitle(self._widget.windowTitle() + (' (%d)' % context.serial_number()))

        #A dict which stores pipeline name and dotgraph pair
        self._dotgraphs = dict()
        #which dotgraph currently drawing on the scence
        self._current_dotcode = None
        self._current_pipeline_name = ''
        #Pydot
        self.dotcode_factory = VinoPydotFactory()
        self.dot_to_qt = DotToQtGenerator()
        self.param_manager = ParamManagerWrapper()

        #Binding scene canvas 
        self._scene = QGraphicsScene()
        self._scene.setBackgroundBrush(Qt.white)
        self._widget.graphics_view.setScene(self._scene)
        self._widget.graphics_view.setClickNodeCallback(self._edit_node)
        

        
        #QListview of pipelines
        self._listmodel = QStandardItemModel()
        self._widget.pipeline_name_listview.clicked.connect(self._display_choosed_pipeline)
        #self._widget.pipeline_name_listview.itemRenamed.connect(self._rename_pipeline)
        #Load pipelines from yaml file
        self._widget.load_pipeline_push_button.clicked.connect(self._load_pipeline)
        #Create a pipeline
        self._widget.create_pipeline_push_button.clicked.connect(self._create_pipeline)
        #Add input to pipeline graph
        self._widget.add_input_push_button.clicked.connect(self._add_input)
        #Add infer 
        self._widget.add_inference_push_button.clicked.connect(self._add_infer)
        self._widget.add_output_push_button.clicked.connect(self._add_output)

        
        self._widget.save_pipeline_push_button.clicked.connect(self._save_pipeline)

        self.models_desc_file_path = os.path.join(rospkg.RosPack().get_path('vino_param_lib'), 'param', 'models.yaml')

        # Add widget to the user interface
        context.add_widget(self._widget)

    def shutdown_plugin(self):
        # TODO unregister all publishers here
        pass

    def save_settings(self, plugin_settings, instance_settings):
        # TODO save intrinsic configuration, usually using:
        # instance_settings.set_value(k, v)
        pass

    def restore_settings(self, plugin_settings, instance_settings):
        # TODO restore intrinsic configuration, usually using:
        # v = instance_settings.value(k)
        pass

    def _refresh_rosgraph(self):


        self._widget.pipeline_name_listview.setModel(self._listmodel)
        self._widget.pipeline_name_listview.show()



        self._redraw_graph_view()

        # if not self.initialized:
        #     return
        # self._update_graph_view(self._generate_dotcode())

    def _rename_pipeline(self,item,col):
       
        pass
    


    def _display_choosed_pipeline(self,index):
     
        pipeline_name = self._listmodel.data(index)
        if pipeline_name in self._dotgraphs.iterkeys():
            self._current_pipeline_name = pipeline_name

        self._refresh_rosgraph()

   

    def _load_pipeline(self):
        self._dotgraphs.clear()
        self._listmodel.clear()

        file_path, _ = QFileDialog.getOpenFileName(self._widget,"QFileDialog.getOpenFileName()", "","*.yaml")

        self._dotgraphs = generate_dotcode_from_yaml_file(self.dotcode_factory, self.param_manager, str(file_path))



        for pipeline_name, pipeline_dotgraph in self._dotgraphs.items():
          
            pipeline_item = QStandardItem()
            pipeline_item.setText(pipeline_name)
            self._listmodel.appendRow(pipeline_item)
        
    
      
        self._current_pipeline_name = self._dotgraphs.iterkeys().next()


        self._refresh_rosgraph()
        #self._display_choosed_pipeline(pipeline_item)

    def _redraw_graph_view(self):
        
        self._scene.clear()

        if self._dotgraphs == None:
            return 

       
        self._current_dotcode = self.dotcode_factory.create_dot(self._dotgraphs[self._current_pipeline_name])

      
        highlight_level = 2

        (nodes, edges) = self.dot_to_qt.dotcode_to_qt_items(self._current_dotcode,
                                                            highlight_level=highlight_level,
                                                            same_label_siblings=False,
                                                            scene=self._scene)

        self._scene.setSceneRect(self._scene.itemsBoundingRect())
        #if self._widget.auto_fit_graph_check_box.isChecked():

        self._fit_in_view()
    
    def _add_input(self):
        

        dlg = InputDialog('add',self._dotgraphs[self._current_pipeline_name],self.dotcode_factory,dotparser, self.param_manager, parent = self._widget)
       
        dlg.exec_()

      

        self._refresh_rosgraph()

    def _add_infer(self):
        if self._current_pipeline_name == '':
            return
        dlg = InferenceDialog('add',self._dotgraphs[self._current_pipeline_name],self.dotcode_factory,dotparser, self.param_manager,self.models_desc_file_path,parent=self._widget)
        dlg.exec_()

        self._refresh_rosgraph()

    def _edit_node(self,nodename = ''):
        if self._current_pipeline_name == '':
            return
        selected_node = self._dotgraphs[self._current_pipeline_name].get_node(nodename.encode('utf-8'))[0]
      

        selected_node_type = selected_node.get('nodetype')
        
        if selected_node_type == 'input':
            self._edit_input(nodename)
        elif selected_node_type == 'infer':
            self._edit_infer(nodename)
        elif selected_node_type == 'output':
            self._edit_output(nodename)

    def _edit_infer(self,nodename = ''):
        
     
        dlg = InferenceDialog('edit',self._dotgraphs[self._current_pipeline_name],self.dotcode_factory,dotparser, self.param_manager,self.models_desc_file_path,nodename = nodename,parent=self._widget)
        dlg.exec_()

        self._refresh_rosgraph()
    
    def _edit_input(self,nodename = ''):
        
        dlg = InputDialog('edit',self._dotgraphs[self._current_pipeline_name],self.dotcode_factory,dotparser, self.param_manager, nodename = nodename, parent = self._widget)
        dlg.exec_()
        self._refresh_rosgraph()
    def _edit_output(self,nodename = ''):
        dlg = OutputDialog('edit',self._dotgraphs[self._current_pipeline_name],self.dotcode_factory,dotparser, self.param_manager,models_desc_file_path = self.models_desc_file_path, nodename = nodename, parent = self._widget)
        dlg.exec_()

        self._refresh_rosgraph()

    def _add_output(self):
        dlg = OutputDialog('add',self._dotgraphs[self._current_pipeline_name],self.dotcode_factory,dotparser, self.param_manager, models_desc_file_path = self.models_desc_file_path, parent = self._widget)
        dlg.exec_()

        self._refresh_rosgraph()

    def _fit_in_view(self):
        self._widget.graphics_view.fitInView(self._scene.itemsBoundingRect(), Qt.KeepAspectRatio)

    def _create_pipeline(self):
      
        pipeline_name, okPressed = QInputDialog.getText(self._widget, "Create a new pipeline","new pipeline name:", QLineEdit.Normal, "")
        
        if okPressed != True or pipeline_name == '':
            #Empty name
            return 
        

        pipeline_item = QStandardItem()
        pipeline_item.setText(pipeline_name)
        self._listmodel.appendRow(pipeline_item)
        

        new_dotgraph = dotparser.generate_dotcode_from_empty(self.dotcode_factory,pipeline_name)
        self._dotgraphs.update(new_dotgraph)
        self._current_pipeline_name = pipeline_name
        self._refresh_rosgraph()

    def _save_pipeline(self):
        # print(self._dotgraphs )
        self.dotcode_factory.parse_nodes(self._dotgraphs[self._current_pipeline_name])