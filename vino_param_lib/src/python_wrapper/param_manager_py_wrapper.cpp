#include <boost/python.hpp> 
#include <vino_param_lib/param_manager.h>
#include <string>
using namespace boost::python; 

struct vector_to_python
{
   static PyObject* InferenceParam_To_PyObject(const Params::ParamManager::InferenceParams infer_cpp)
   {
      PyObject * infer_py = PyDict_New();
      PyDict_SetItemString(infer_py, "name", PyString_FromString(infer_cpp.name.c_str()));
      PyDict_SetItemString(infer_py, "engine",PyString_FromString(infer_cpp.engine.c_str()));
      PyDict_SetItemString(infer_py, "model", PyString_FromString(infer_cpp.model.c_str()));
      PyDict_SetItemString(infer_py, "label", PyString_FromString(infer_cpp.label.c_str()));
      
      return infer_py;
   } 

   static PyObject* connects_To_PyObject(const std::multimap<std::string, std::string> connects_cpp)
   {
      PyObject * connects_py = PyDict_New();
      
      for (auto it = connects_cpp.begin(); it != connects_cpp.end(); ++it) {

         //check if there is a list of destination nodes
         PyObject * list_to_nodes = PyDict_GetItem(connects_py,PyString_FromString(it->first.c_str()));
         if(list_to_nodes)
         {
            PyList_Insert(list_to_nodes,0,PyString_FromString(it->second.c_str()));
         }
         else
         {
            list_to_nodes = PyList_New(0);
            PyList_Insert(list_to_nodes,0,PyString_FromString(it->second.c_str()));
         }
         
         PyDict_SetItemString(connects_py, it->first.c_str(), list_to_nodes);
      }
      return connects_py;
   }

   static PyObject* PipelineParam_To_PyObject(const Params::ParamManager::PipelineParams pipeline_param_cpp )
   {
      PyObject *pipeline_param_py = PyDict_New();
      PyDict_SetItemString(pipeline_param_py, "name", PyString_FromString(pipeline_param_cpp.name.c_str()));

      PyObject * infer_params_py = PyList_New(0);
      for(std::size_t i=0; i< pipeline_param_cpp.infers.size(); i++)
      {
         PyObject * infer_param_py = InferenceParam_To_PyObject(pipeline_param_cpp.infers[i]);
         PyList_Insert(infer_params_py,i,infer_param_py);
      }
      PyDict_SetItemString(pipeline_param_py, "infers", infer_params_py);
      PyDict_SetItemString(pipeline_param_py, "inputs", Inputs_To_PyObject(pipeline_param_cpp.inputs));
      PyDict_SetItemString(pipeline_param_py, "outputs", Inputs_To_PyObject(pipeline_param_cpp.outputs));
      PyDict_SetItemString(pipeline_param_py, "connects",connects_To_PyObject(pipeline_param_cpp.connects));
      return pipeline_param_py;
   }

   static PyObject* Inputs_To_PyObject(const std::vector<std::string>  inputs_cpp)
   {
      PyObject * inputs_py = PyList_New(0);
      for(std::size_t i=0; i< inputs_cpp.size(); i++)
      {
         PyList_Insert(inputs_py,i,PyString_FromString(inputs_cpp[i].c_str()));
      }      
      return inputs_py;
   }

   static PyObject* Outputs_To_PyObject(const std::vector<std::string>  outputs_cpp)
   {
      PyObject * outputs_py = PyList_New(0);
      for(std::size_t i=0; i< outputs_cpp.size(); i++)
      {
         PyList_Insert(outputs_py,i,PyString_FromString(outputs_cpp[i].c_str()));
      }      
      return outputs_py;
   }
   static PyObject* convert(const std::vector<Params::ParamManager::PipelineParams>& vec)
   {

      Py_Initialize();

      PyObject * pipeline_params = PyList_New(0);
      for(std::size_t i=0; i< vec.size(); i++)
      {
         PyObject * pipeline_param = PipelineParam_To_PyObject(vec[i]);
         PyList_Insert(pipeline_params,i,pipeline_param);
      }
      return pipeline_params;

   }

};

struct inferenceDesc_to_python
{
   static PyObject* stringVector_To_PyObject(const std::vector<std::string>& vec)
   {
      PyObject * infer_connects_py = PyList_New(0);
      for(std::size_t i=0; i< vec.size(); i++)
      {
         PyObject * infer_connect = PyString_FromString(vec[i].c_str());
         PyList_Insert(infer_connects_py,i,infer_connect);
      }
      return infer_connects_py;

   }
   static PyObject* modelVector_To_PyObject(const std::vector<Params::ParamManager::InferenceParams>&vec)
   {
      PyObject * available_models = PyList_New(0);
      for(std::size_t i=0; i< vec.size(); i++)
      {
         PyObject * model_py = PyDict_New();
         PyDict_SetItemString(model_py, "name", PyString_FromString(vec[i].name.c_str()));
         PyDict_SetItemString(model_py, "engine",PyString_FromString(vec[i].engine.c_str()));
         PyDict_SetItemString(model_py, "model", PyString_FromString(vec[i].model.c_str()));
         PyDict_SetItemString(model_py, "label", PyString_FromString(vec[i].label.c_str()));
         PyList_Insert( available_models ,i,model_py);
      }
      return available_models;
   }
   static PyObject* InferenceDesc_To_PyObject(const Params::ParamManager::InferenceDesc infer_cpp)
   {
      PyObject * infer_py = PyDict_New();
      PyDict_SetItemString(infer_py, "infer_name", PyString_FromString(infer_cpp.infer_name.c_str()));
      // PyDict_SetItemString(infer_py, "engine",PyString_FromString(infer_cpp.params.engine.c_str()));
      // PyDict_SetItemString(infer_py, "model", PyString_FromString(infer_cpp.params.model.c_str()));
      // PyDict_SetItemString(infer_py, "label", PyString_FromString(infer_cpp.params.label.c_str()));
      PyDict_SetItemString(infer_py, "available_models", modelVector_To_PyObject(infer_cpp.available_models));
      PyDict_SetItemString(infer_py, "download_link", PyString_FromString(infer_cpp.download_link.c_str()));
      PyDict_SetItemString(infer_py, "download_status", PyBool_FromLong(infer_cpp.download_status));


     
      PyDict_SetItemString(infer_py, "connect_from", stringVector_To_PyObject(infer_cpp.connect_from));
 
      PyDict_SetItemString(infer_py, "connect_to", stringVector_To_PyObject(infer_cpp.connect_to));
      PyDict_SetItemString(infer_py, "available_engine", stringVector_To_PyObject(infer_cpp.available_engine));


      
      //PyDict_SetItemString(infer_py, "label", PyString_FromString(infer_cpp.connect_from.c_str()));
      
      return infer_py;
   } 

   static PyObject* convert(const std::vector<Params::ParamManager::InferenceDesc> & vec)
   {

      Py_Initialize();

      PyObject * pipeline_params = PyList_New(0);
      for(std::size_t i=0; i< vec.size(); i++)
      {
         PyObject * pipeline_param = InferenceDesc_To_PyObject(vec[i]);
         PyList_Insert(pipeline_params,i,pipeline_param);
      }
      return pipeline_params;

   }
};

class ParamManagerWrapper
{

public:

   ParamManagerWrapper(){}


   std::vector<Params::ParamManager::PipelineParams> parse_path(std::string file_path)
   {
      Params::ParamManager::getInstance().parse(file_path);
      Params::ParamManager::getInstance().print();

      auto pcommon = Params::ParamManager::getInstance().getCommon();
      auto pipelines = Params::ParamManager::getInstance().getPipelines();
      return pipelines;
   }

   std::vector<Params::ParamManager::InferenceDesc> parse_inferlist_file(std::string file_path)
   {
      Params::ParamManager::getInstance().parseSupoortedInfers(file_path);
      auto supported_infers = Params::ParamManager::getInstance().getAllInfersSupported();
      return supported_infers;
   }
 
}; 

BOOST_PYTHON_MODULE(param_manager_wrapper) 
{ 
 
class_<ParamManagerWrapper>("ParamManagerWrapper")
   .def("parse_path", &ParamManagerWrapper::parse_path)
   .def("parse_inferlist_file", &ParamManagerWrapper::parse_inferlist_file); 

boost::python::to_python_converter<std::vector<Params::ParamManager::PipelineParams, std::allocator<Params::ParamManager::PipelineParams>>, vector_to_python >();
boost::python::to_python_converter<std::vector<Params::ParamManager::InferenceDesc, std::allocator<Params::ParamManager::InferenceDesc>>, inferenceDesc_to_python >();
}