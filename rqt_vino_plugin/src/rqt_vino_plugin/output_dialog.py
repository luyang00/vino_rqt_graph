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
from qt_dotgraph.pydotfactory import PydotFactory


import pydot

import dotparser


from dotparser import generate_dotcode_from_yaml_file

# from vino_param_lib.param_manager_wrapper import ParamManagerWrapper 

class OutputDialog(QDialog):
    def __init__(self, mode, dotgraph , dotcode_factory, dotparser , param_manager, models_desc_file_path, nodename = '', parent = None):
        super(OutputDialog, self).__init__(parent)

        if mode not in ['add','edit']:
            raise Exception('Wrong mode for Ouput Dialog')

        self.setWindowTitle("Add a output pipe")
        self.setGeometry(300,300,450,250)

        # self._widget = QDialog()

        ui_file = os.path.join(rospkg.RosPack().get_path('rqt_vino_plugin'), 'resource', 'add_output_dialog.ui')
        loadUi(ui_file, self)
        
        self.dotgraph = dotgraph
        self.dotparser = dotparser
        self.dotcode_factory = dotcode_factory
 
        self.available_infers_list = param_manager.parse_inferlist_file(models_desc_file_path)
 
    
        
        self.connect_from_listmodel = QStandardItemModel()
        self.output_type_combobox.currentTextChanged.connect(self._update_display)
        self.update_output_types()
        
        if mode == 'add':
            self.buttonBox.accepted.connect(self._create_node_in_graph)
        elif mode == 'edit':
            self.nodename = nodename
            self.buttonBox.accepted.connect(self._edit_node_in_graph)
    

    def update_output_types(self, outputlist_file = None):
        self.output_type_combobox.clear()
        self.output_type_combobox.addItem('ImageWindow')
        self.output_type_combobox.addItem('Rviz')
        self.output_type_combobox.addItem('Rostopic')


    def get_infer_desc(self,infer_name):
        for infer in self.available_infers_list:
            if infer['infer_name'] == infer_name:
                return infer   

    def _update_display(self,output_name):

        self.output_name_display_lineEdit.setText(output_name)
        self.update_connect_to_list(output_name)
        

    def update_connect_to_list(self, output_name):

        self.connect_from_listmodel.clear()

        for node in self.dotgraph.get_node_list():
            if node.get('nodetype') == 'input':
                pass
            elif node.get('nodetype') == 'output':
                pass
            elif node.get('nodetype') == 'infer':
                if  output_name in self.get_infer_desc(node.get_name())['connect_to']:

                    pipeline_item = QStandardItem()
                    pipeline_item.setText(node.get_name())
                    pipeline_item.setCheckable(True)
                    pipeline_item.setCheckState(False)

                    self.connect_from_listmodel.appendRow(pipeline_item)

        self.connect_from_listview.setModel(self.connect_from_listmodel)


    def _create_node_in_graph(self):
        
        output_node_name =  str(self.output_type_combobox.currentText()) 

        #Create Node 
        self.dotparser.parse_dotgraph_from_outputs(self.dotcode_factory,self.dotgraph, [output_node_name])

        #Create connection from input node to infer node
        for i in range(self.connect_from_listview.model().rowCount()):
            node_name = self.connect_from_listmodel.item(i).text()
            check_state = self.connect_from_listmodel.item(i).checkState()
            if check_state:
                connects = { node_name: [output_node_name] }
                self.dotparser.parse_dotgraph_from_connects(self.dotcode_factory,self.dotgraph,connects)
        
    def _edit_node_in_graph(self):
       
        self.dotcode_factory.del_node_from_graph(self.dotgraph,self.nodename)
        self._create_node_in_graph()
 