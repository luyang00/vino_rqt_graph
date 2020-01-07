from qt_dotgraph.pydotfactory import PydotFactory
import pydot

from qt_dotgraph.dot_to_qt import get_unquoted

import yaml
class VinoPydotFactory(PydotFactory):
    def __init__(self):
        pass
        # super(VinoPydotFactory, self).__init__()

    
    def add_node_to_graph(self,
                          graph,
                          nodename,
                          nodetype = None,
                          infer_type = None,
                          model = None,
                          engine = None,
                          label = None,
                          batch = None,
                          nodelabel=None,
                          shape='box',
                          color=None,
                          url=None,
                          tooltip=None):
        """
        creates a node item for this factory, adds it to the graph.
        Node name can vary from label but must always be same for the same node label
        """
        if nodename is None or nodename == '':
            raise ValueError('Empty Node name')
        node = pydot.Node(self.escape_name(nodename))

        if infer_type is not None:
            node.set('infer_type',infer_type)
        if nodetype  is not None:
            node.set('nodetype',nodetype)
        if engine is not None:
            node.set('infer_engine',engine)
        if model is not None:
            node.set('infer_model',model)
        if label is not None:
            node.set('infer_label',label)
        if batch is not None:
            node.set('batch',batch)

        node.set_shape(shape)
        node.set_label(self.escape_label(nodelabel))
        if tooltip is not None:
            node.set_tooltip(tooltip)
        elif url is not None:
            node.set_tooltip(url)
        if url is not None:
            node.set_URL(self.escape_name(url))
        if color is not None:
            node.set_color(color)


        graph.add_node(node)

    def del_node_from_graph(self,graph,nodename,shape='box'):
        #first remove all edges from this nodeg
        # node = graph.get_node(nodename)
        nodename = nodename.encode('utf-8')
        ret_del_node = graph.del_node(nodename)
      

        all_edges = graph.get_edges()
        
      
        for edge in graph.get_edges():
            src_node = edge.get_source()
            dst_node = edge.get_destination()
            edge_pair = [src_node,dst_node]
            if nodename in edge_pair:

                ret_del_edge = graph.del_edge((src_node),dst = dst_node )


    def parse_nodes(self,graph):
        
        graph.nodes_iter = graph.get_node_list
        graph.subgraphs_iter = graph.get_subgraph_list

        nodes = {}
        
        
        # for subgraph in graph.subgraphs_iter():
        #     subgraph_nodeitem = self.getNodeItemForSubgraph(subgraph, highlight_level, scene=scene)
        #     nodes.update(self.parse_nodes(subgraph))
        #     # skip subgraphs with empty bounding boxes
        #     if subgraph_nodeitem is None:
        #         continue

        #     subgraph.nodes_iter = subgraph.get_node_list
        #     nodes[subgraph.get_name()] = subgraph_nodeitem
        #     for node in subgraph.nodes_iter():
        #         # hack required by pydot
        #         if node.get_name() in ('graph', 'node', 'empty'):
        #             continue
        #         nodes[node.get_name()] = self.getNodeItemForNode(node, highlight_level, scene=scene)
        for node in graph.nodes_iter():
            # hack required by pydot
            if node.get_name() in ('graph', 'node', 'empty'):
                continue
            nodes[node.get_name()] = self.getNodeItemForNode(node)
        return nodes



    def getNodeItemForNode(self, node):
        """
        returns a pyqt NodeItem object, or None in case of error or invisible style
        """
        # let pydot imitate pygraphviz api
        attr = {}
        for name in node.get_attributes().keys():
            value = get_unquoted(node, name)
            attr[name] = value
        obj_dic = node.__getattribute__("obj_dict")
        for name in obj_dic:
            if name not in ['attributes', 'parent_graph'] and obj_dic[name] is not None:
                attr[name] = get_unquoted(obj_dic, name)
        node.attr = attr

        if node.attr.get('style') == 'invis':
            return None

        
        print(attr)
      
        
      

    
        return attr