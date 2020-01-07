/*
 * Copyright (c) 2018 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * @brief A header file with declaration for parameter management
 * @file param_manager.hpp
 */



#ifndef VINO_PARAM_LIB_PARAM_MANAGER_H
#define VINO_PARAM_LIB_PARAM_MANAGER_H

#include <ros/ros.h>
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>


namespace Params
{
/**
 * @class ParamManager
 * @brief This class implements yaml-based parameter management.
 *
 * The common usage step:
 *   1. parse a yaml file and generate parameters into ParamManager getInstance
 *      Params::ParamManager::getInstance().parse(FLAGS_config);
 *   2. Get any specific parameter from ParamManager getInstance
 *      auto common = Params::ParamManager::getInstance().getCommon();
 *      std::string topic = common.camera_topic;
 *      < further ops >
 */
class ParamManager  // singleton
{
 public:
  /**
   * @brief Get the singleton instance of ParamManager class.
   * The instance will be created when first call.
   * @return The reference of paramManager class.
   */
  
  static ParamManager& getInstance()
  {
    static ParamManager manager_;
    return manager_;
  }
  
  /**
   * @brief Dump all parameters into slog out.
   * @return None.
   */
  void print() const;

  struct InferenceParams
  {
    std::string name;
    std::string engine;
    std::string model;
    std::string label;
    bool operator==( const std::string& name ) const
    {
        return this->name == name;
    }
    InferenceParams(  std::string name,
                      std::string engine,
                      std::string model,
                      std::string label):
                      name(name),engine(engine),model(model),label(label){};
    InferenceParams(){}
  };

  typedef struct InferenceDesc {
    // struct InferenceParams params;
    std::string infer_name;
    std::vector<InferenceParams> available_models;
    std::string download_link;
    bool download_status;
    std::vector<std::string> connect_from;
    std::vector<std::string> connect_to;
    std::vector<std::string> available_engine;
  } InferenceDesc;



  struct PipelineParams
  {
    std::string name;
    std::vector<InferenceParams> infers;
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
    std::multimap<std::string, std::string> connects;
  };
  struct CommonParams
  {
    std::string custom_cpu_library;
    std::string custom_cldnn_library;
    bool enable_performance_count = false;
    std::string camera_topic;
  };

  /**
   * @brief Parse the give YAML file and generate parameters in ParamManager
   * instance
   * @param[in] path The absolute path of the YAML file which is to be parsed.
   * @return None.
   */
  void parse(std::string path);
  void parseConfs(std::string path);
  void parseSupoortedInfers(std::string path);
  void save(const ParamManager::PipelineParams& pipeline,
                    const std::string& path);
                    
  void save( const std::vector<ParamManager::PipelineParams> & pipelines,
                         const std::string& path);
  std::string toString();
  /**
   * @brief Retrieve pipeline names.
   * @return A list of string storing pipelines' names.
   */
  std::vector<std::string> getPipelineNames() const;

  /**
   * @brief Retrieve pipeline parameters.
   * @return A list of struct PipelineParams storing all pipeline parameters.
   */
  std::vector<PipelineParams> getPipelines() const
  {
    return pipelines_;
  }

  /**
   * @brief Retrieve the specific pipeline parameters by the given pipeline
   * name.
   * @param[in] name: the name of the pipeline to be retrieved.
   * @return The pipeline paratmeters, or throw a loginc error.
   */
  PipelineParams getPipeline(const std::string& name) const;


  std::vector<InferenceParams> getInfersSupported() 
  {
    //To-do: remove this function
    // std::vector<InferenceParams> 
    // return infers_supported;
    std::vector<InferenceParams> v;
    return v;
  }
  std::vector<InferenceDesc> getAllInfersSupported() 
  {
    return infers_supported;
  }
  /**
   * @brief Retrieve common parameters.
   * @return struct CommonParams storing all common parameters.
   */
  CommonParams getCommon() const
  {
    return common_;
  }

   

 private:
  ParamManager()
  {
  }
  ParamManager(ParamManager const&);
  void operator=(ParamManager const&);

  std::vector<PipelineParams> pipelines_;
  std::vector<InferenceDesc> infers_supported;
  CommonParams common_;
};

}  // namespace Params
#endif  // VINO_PARAM_LIB_PARAM_MANAGER_H
