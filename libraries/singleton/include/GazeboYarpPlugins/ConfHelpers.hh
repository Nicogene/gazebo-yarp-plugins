/*
 * Copyright (C) 2007-2013 Istituto Italiano di Tecnologia ADVR & iCub Facility
 * Authors: Enrico Mingo, Alessio Rocchi, Mirko Ferrati, Silvio Traversaro and Alessandro Settimi
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#ifndef GAZEBOYARP_CONFIGURATION_HELPERS_HH
#define GAZEBOYARP_CONFIGURATION_HELPERS_HH


#include <sdf/sdf_config.h>

#if SDF_MAJOR_VERSION >= 3

#include <sdf/Element.hh>

#else

#include <boost/shared_ptr.hpp>

namespace sdf {
   class Element;
   typedef boost::shared_ptr<Element> ElementPtr;
}

#endif

#include <gazebo/sensors/SensorTypes.hh>
#include <gazebo/physics/PhysicsTypes.hh>

namespace yarp {
    namespace os {
        class Property;
        class Searchable;
    }
}

namespace GazeboYarpPlugins {

/**
 * Split a string in a vector of string given a (single char) delimiter
 */
std::vector<std::string> splitString(const std::string &s, const std::string &delim);

/**
 * Concat 2 string lists and return the result
 */
//std::vector<std::string> concatStringVectors(const std::vector<std::string>& sVec1, const std::vector<std::string>& sVec2);
template <typename T, int N>
std::vector<T> concatStringVectors(const std::array<std::vector<T>,N> sVecArray);

template <int N>
std::vector<std::string> concatStringVectors(const std::array<std::vector<std::string>,N> sVecArray);

template <int N>
std::vector<double> concatStringVectors(const std::array<std::vector<double>,N> sVecArray);

/**
 * Load the configuration for a given model plugin,
 * and save the configuration in the plugin_parameters output object.
 * This involves calling addGazeboEnviromentalVariablesModel and then loading
 * the yarp configuration file specified in the sdf with the yarpConfigurationFile tag.
 */
bool loadConfigModelPlugin(gazebo::physics::ModelPtr _parent,
                           sdf::ElementPtr _sdf,
                           yarp::os::Property & plugin_parameters);

/**
 * Add some Gazebo specific "enviromental variables" that it is possible
 * to expand in .ini configuration file loaded using the ${variable} syntax,
 * for writing configuration files where port names and other parameters
 * depend on Gazebo names.
 *
 * This function add some model related "enviromental variables",
 * copying their values from some ModelPtr methods :
 *
 * |  Yarp parameter name       | ModelPtr method    |
 * |:--------------------------:|:------------------:|
 * | gazeboYarpPluginsRobotName | model->GetName()   |
 *
 * @return true if all went well, false otherwise
 */
bool addGazeboEnviromentalVariablesModel(gazebo::physics::ModelPtr _parent,
                                         sdf::ElementPtr _sdf,
                                         yarp::os::Property & plugin_parameters);

/**
 * Load the configuration for a given sensor plugin,
 * and save the configuration in the plugin_parameters output object.
 * This involves calling addGazeboEnviromentalVariablesSensor and then loading
 * the yarp configuration file specified in the sdf with the yarpConfigurationFile tag.
 */
bool loadConfigSensorPlugin(gazebo::sensors::SensorPtr _sensor,
                            sdf::ElementPtr _sdf,
                            yarp::os::Property & plugin_parameters);

/**
 * Add some Gazebo specific "enviromental variables" that it is possible
 * to expand in .ini configuration file loaded using the ${variable} syntax,
 * for writing configuration files where port names and other parameters
 * depend on Gazebo names.
 *
 * This function add some Sensor related "enviromental variables",
 * copying their values from some SensorPtr methods :
 *
 * |  Yarp parameter name        | SensorPtr method    |
 * |:---------------------------:|:-------------------:|
 * | gazeboYarpPluginsSensorName | sensor->GetName()   |
 * | gazeboYarpPluginsRobotName  | Model name as extracted from sensor->GetScopedName()  |
 *
 * @return true if all went well, false otherwise
 */
bool addGazeboEnviromentalVariablesSensor(gazebo::sensors::SensorPtr _sensor,
                                          sdf::ElementPtr _sdf,
                                          yarp::os::Property & plugin_parameters);

/**
 * Return an empty list if the key is not found, and an error (false) if the key was found
 * but it is not a list of strings bool getVectorOfStringFromListInConfig(const std::string&
 * key, const os::Searchable& config, std::vector<std::string> & vectorOfStrings)
 */
bool getVectorOfStringFromListInConfig(const std::string& key, const yarp::os::Searchable& prop,
                                       std::vector<std::string> & vectorOfStrings);

}

#endif  // GAZEBOYARP_CONFIGURATION_HELPERS_HH
