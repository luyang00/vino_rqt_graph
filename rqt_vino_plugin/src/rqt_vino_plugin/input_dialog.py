import os
import rospy
import rospkg

from python_qt_binding.QtCore import Qt, Signal
from qt_gui.plugin import Plugin
from python_qt_binding import loadUi
from python_qt_binding.QtWidgets import *
from python_qt_binding.QtGui import QStandardItem,QStandardItemModel
from qt_dotgraph.dot_to_qt import DotToQtGenerator

from vino_pydot_factory import VinoPydotFactory


from python_qt_binding.QtWidgets import *
import dotparser

# from vino_param_lib.param_manager_wrapper import ParamManagerWrapper 

class InputDialog(QDialog):
    def __init__(self, mode, dotgraph , dotcode_factory, dotparser , param_manager, nodename = '', parent = None):
        super(InputDialog, self).__init__(parent)
        
        self.nodename = nodename
        if mode not in ['add','edit']:
            raise Exception('Only support add & edit action for input node')

        self.dotgraph = dotgraph
        self.dotparser = dotparser
        self.dotcode_factory = dotcode_factory

        self.input_layout = QFormLayout(self)

        self.input_dlg_buttons = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel,Qt.Horizontal, self)

        self.input_dlg_buttons.accepted.connect(self.accept)
        self.input_dlg_buttons.rejected.connect(self.reject)
        self.input_type_label = QLabel(self)
        self.input_type_label.setText('Input type:')
        self.input_type_combo = QComboBox(self)
        self.input_type_combo.addItem('RealSenseCamera')
        self.input_type_combo.addItem('StandardCamera')
        self.input_type_combo.addItem('RealSenseCameraTopic')

        self.input_layout.addRow(self.input_type_label,self.input_type_combo)
        self.input_layout.addRow(self.input_dlg_buttons)

        self.setLayout(self.input_layout)
        self.setWindowModality(Qt.ApplicationModal)

        if mode == 'add':
            self.input_dlg_buttons.accepted.connect(self._create_node_in_graph)
        elif mode == 'edit':
            self.input_dlg_buttons.accepted.connect(self._edit_node_in_graph)

    def _edit_node_in_graph(self):

        self.dotcode_factory.del_node_from_graph(self.dotgraph,self.nodename)
        self._create_node_in_graph()

    def _create_node_in_graph(self):
        intputs = [ str(self.input_type_combo.currentText()) ]
        dotparser.parse_dotgraph_from_inputs(self.dotcode_factory,self.dotgraph,intputs)