





def parse_dotgraph_from_inputs(dotcode_factory, dotgraph ,inputs):
    for input in inputs:
        nodename = input
        nodelabel = nodename
        #check if node exist
        if dotgraph.get_node(nodename):
            raise Exception('Input {} already exist.'.format(nodename))
        dotcode_factory.add_node_to_graph(
            dotgraph,
            nodename=nodename,
            nodelabel=nodelabel,
            nodetype='input',
            shape="rect",
            url=nodename + " (DEAD)",
            color="black")

def parse_dotgraph_from_outputs(dotcode_factory, dotgraph, outputs):
    for output in outputs:
        nodename = output
        nodelabel = nodename

        if dotgraph.get_node(nodename):
            raise Exception('Outputs {} already exist.'.format(nodename))
        dotcode_factory.add_node_to_graph(
            dotgraph,
            nodename=nodename,
            nodelabel=nodelabel,
            nodetype='output',
            shape="rect",
            url=nodename + " (DEAD)",
            color="black")

def parse_dotgraph_from_infers(dotcode_factory, dotgraph, infers):
    for infer in infers:
        nodename = infer['name']
        nodelabel = nodename
        engine = infer['engine']
        model = infer['model']
        label = infer['label']
        batch = infer['batch']
    
        print('check: ',infer)
        if dotgraph.get_node(nodename):
            raise Exception('Inference node {} already exist.'.format(nodename))
        dotcode_factory.add_node_to_graph(
            dotgraph,
            nodename=nodename,
            nodelabel=nodelabel,
            nodetype='infer',
            engine = engine,
            model = model,
            label = label,
            batch = batch,
            shape="rect",
            url=nodename + " (DEAD)",
            color="black")        

def parse_dotgraph_from_connects(dotcode_factory, dotgraph, connects):
    for connect_from in connects:
        connect_to_list = connects[connect_from]
        for connect_to in connect_to_list:
            dotcode_factory.add_edge_to_graph(
                dotgraph,
                nodename1 = connect_from,
                nodename2 = connect_to,
                label='%s -> %s' % (connect_from,connect_to),
                url='%s -> %s' % (connect_from,connect_to))


def parse_dotgraph_from_pipeline(dotcode_factory,pipeline):
    
    #print(pipeline)
    rank = 'same'
    ranksep = 0.2
    simplify = True
    rankdir = 'TB'
    orientation = 'LR'
    dotgraph = dotcode_factory.get_graph(
        rank=rank,
        ranksep=ranksep,
        simplify=simplify,
        rankdir=orientation)

    parse_dotgraph_from_inputs(dotcode_factory, dotgraph ,pipeline['inputs'])
    parse_dotgraph_from_outputs(dotcode_factory, dotgraph ,pipeline['outputs'])
    parse_dotgraph_from_infers(dotcode_factory,dotgraph,pipeline['infers'])
    parse_dotgraph_from_connects(dotcode_factory, dotgraph, pipeline['connects'])

    return { pipeline['name'] : dotgraph}

def generate_dotcode_from_yaml_file(dotcode_factory, param_manager ,file_path):

    pipeline_params = param_manager.parse_path(file_path)
    
    dotgraphs = {}
    for pipeline in pipeline_params:
        name_dotgraph_pair = parse_dotgraph_from_pipeline(dotcode_factory,pipeline)

        dotgraphs.update(name_dotgraph_pair)
    return dotgraphs

def generate_dotcode_from_empty(dotcode_factory, pipeline_name):

    #print(pipeline)
    rank = 'same'
    ranksep = 0.2
    simplify = True
    rankdir = 'TB'
    orientation = 'LR'
    dotgraph = dotcode_factory.get_graph(
        rank=rank,
        ranksep=ranksep,
        simplify=simplify,
        rankdir=orientation)


    return {pipeline_name : dotgraph}
