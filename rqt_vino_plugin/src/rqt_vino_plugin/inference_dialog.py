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
# from qt_dotgraph.pydotfactory import PydotFactory


#for test

import pydot

import dotparser
import vino_pydot_factory

from dotparser import generate_dotcode_from_yaml_file

# from vino_param_lib.param_manager_wrapper import ParamManagerWrapper 

class InferenceDialog(QDialog):
    def __init__(self, mode, dotgraph , dotcode_factory, dotparser , param_manager, models_desc_file_path, nodename='', parent = None):
        super(InferenceDialog, self).__init__(parent)



        if mode not in ['add','edit']:
            raise Exception('wrong mode for InferenceDialog')
        
    
        self.nodename = nodename

        self.setWindowTitle("Dialog")
        self.setGeometry(300,300,450,250)

        # self._widget = QDialog()

        ui_file = os.path.join(rospkg.RosPack().get_path('rqt_vino_plugin'), 'resource', 'add_inference_dialog.ui')
        loadUi(ui_file, self)
        

        self.dotgraph = dotgraph
        self.dotparser = dotparser
        self.dotcode_factory = dotcode_factory
 
 
        self.available_infers_list = param_manager.parse_inferlist_file(models_desc_file_path)
        


        # self.node_name_lineedit.textEdited.connect(self._edit_infer_name)
        #self.node_infer_name_combobox.activated.connect(self.update_infer_names)
        self.node_infer_combobox.currentTextChanged.connect(self._select_infer_type)

        self.connect_from_listmodel = QStandardItemModel()#(self.connect_from_listview)
        self.connect_to_listmodel = QStandardItemModel()#(self.connect_to_listview)

        # self.connect_from_listmodel.itemChanged.connect(self._connect_from_changed)
        
        #self.connect_to_listview.setModel(self.connect_to_listmodel)

      
        self.update_infer_names()
        self.update_connect_from_list()

        if mode == 'add':
            self.buttonBox.accepted.connect(self._create_node_in_graph)
        elif mode == 'edit':
            self.buttonBox.accepted.connect(self._edit_node_in_graph)
        

    def update_infer_names(self):
        self.node_infer_combobox.clear()
        for infer in self.available_infers_list:
            self.node_infer_combobox.addItem(infer['infer_name'])

    def get_infer_desc(self,infer_name):
        for infer in self.available_infers_list:
            if infer['infer_name'] == infer_name:
                return infer
    def get_model_desc(self,infer_name,model_type):
        for model_desc in self.get_infer_desc(infer_name)['available_models']:
            if model_desc['name'] == model_type:
                return model_desc
    def update_connect_to_list(self, infer_name):
        self.connect_from_listmodel.clear()
        self.connect_to_listmodel.clear()

        for node in self.dotgraph.get_node_list():
            
            pipeline_item = QStandardItem()
            pipeline_item.setText(node.get_name())
            pipeline_item.setCheckable(True)
            pipeline_item.setCheckState(False)

            if node.get_name() == infer_name.encode('utf-8'):
                continue
            if node.get('nodetype') == 'input':

                self.connect_from_listmodel.appendRow(pipeline_item)

                # self.connect_from_listview.addItem(node.get_name())
            elif node.get('nodetype') == 'output':
                self.connect_to_listmodel.appendRow(pipeline_item)
                # self.connect_to_listview.addItem(node.get_name())
            elif node.get('nodetype') == 'infer':
                if  infer_name in self.get_infer_desc(infer_name)['connect_from']:
                    self.connect_from_listmodel.appendRow(pipeline_item)
                elif  infer_name in self.get_infer_desc(infer_name)['connect_to']:
                    self.connect_to_listmodel.appendRow(pipeline_item)

            
        self.connect_from_listview.setModel(self.connect_from_listmodel)
        self.connect_to_listview.setModel(self.connect_to_listmodel)
   

    def update_connect_from_list(self):
        pass

    def update_model_type(self,infer_name):
        self.node_model_combobox.clear()
        # infer = self.get_infer_desc(infer_name)
        for model in self.get_infer_desc(infer_name)['available_models']:
            self.node_model_combobox.addItem(model['name'])
        # self.node_model_combobox.addItem("MobileNetSSD")
        # self.node_model_combobox.addItem("YoloV2")
    def _select_infer_type(self,infer_name):
        self.node_engine_combobox.clear()
        infer_name = str(infer_name)
        
        for infer in self.available_infers_list:
            if infer['infer_name'] == infer_name:
                for engine in infer['available_engine']:
                    self.node_engine_combobox.addItem(engine)

        self.update_model_type(infer_name)
        self.update_connect_to_list(infer_name)

    def _edit_infer_name(self,infer_name):
        self.node_name_display_lineEdit.setText(infer_name)

    
    def _create_node_in_graph(self):

        infer_node_name =  str(self.node_infer_combobox.currentText()) 
        print('ava_list',self.available_infers_list)

        infer_node_engine = self.node_engine_combobox.currentText()
        print(self.get_infer_desc(infer_node_name))
        infer_node_model_type = self.node_model_combobox.currentText()
        infer_node_label = self.get_model_desc(infer_node_name, infer_node_model_type)['label']
        infer_node_model = self.get_model_desc(infer_node_name, infer_node_model_type)['model']
        #Create Node 
        self.dotparser.parse_dotgraph_from_infers(self.dotcode_factory,self.dotgraph, [{'name': infer_node_name,
                                                                                        'engine':infer_node_engine,
                                                                                        'model':infer_node_model,
                                                                                        'label':infer_node_label,
                                                                                        'batch':1}])
                                                                                     #To-do Implement batch configure

        #Create connection from input node to infer node
        for i in range(self.connect_from_listview.model().rowCount()):
            node_name = self.connect_from_listmodel.item(i).text()
            check_state = self.connect_from_listmodel.item(i).checkState()
            if check_state:
                connects = { node_name: [infer_node_name] }
                self.dotparser.parse_dotgraph_from_connects(self.dotcode_factory,self.dotgraph,connects)
        
        #Create connection from infer node to output node
        for i in range(self.connect_to_listview.model().rowCount()):
            node_name = self.connect_to_listmodel.item(i).text()
            check_state = self.connect_to_listmodel.item(i).checkState()
            if check_state:
                connects = { infer_node_name :[node_name] }
                self.dotparser.parse_dotgraph_from_connects(self.dotcode_factory,self.dotgraph,connects)

                
    def _edit_node_in_graph(self):
       
        self.dotcode_factory.del_node_from_graph(self.dotgraph,self.nodename)
        self._create_node_in_graph()
