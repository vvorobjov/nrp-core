#include <boost/python.hpp>
#include <boost/python/scope.hpp>
#include <boost/python/enum.hpp>

#include "nrp_event_loop/computational_graph/computational_node_policies.h"
#include "nrp_event_loop/python/functional_node.h"
#include "nrp_event_loop/nodes/dummy/input_dummy.h"
#include "nrp_event_loop/nodes/dummy/output_dummy.h"
#include "nrp_event_loop/nodes/engine/input_node.h"
#include "nrp_event_loop/nodes/engine/output_node.h"

#ifdef SPINNAKER_ON
#include "nrp_event_loop/nodes/spinnaker/input_node.h"
#include "nrp_event_loop/nodes/spinnaker/output_node.h"
#endif

#ifdef ROS_ON
#include "nrp_event_loop/nodes/ros/ros_edge_factory.h"
#endif

#include "nrp_event_loop/config/cmake_constants.h"

class node_policies_ns{

public:

    class input_node_ns{};
    class functional_node_ns{};
    class output_node_ns{};
};

BOOST_PYTHON_MODULE(EVENT_LOOP_PYTHON_MODULE_NAME)
{
    // node policies enums
    {
        bpy::scope node_policies_scope =  bpy::class_<node_policies_ns>("node_policies");

        {
            bpy::scope input_policies_scope =  bpy::class_<node_policies_ns::input_node_ns>("input_node");

            bpy::enum_<InputNodePolicies::MsgCachePolicy>("msg_cache")
                    .value("clear", InputNodePolicies::MsgCachePolicy::CLEAR_CACHE)
                    .value("keep", InputNodePolicies::MsgCachePolicy::KEEP_CACHE)
                    .export_values();

            bpy::enum_<InputNodePolicies::MsgPublishPolicy>("msg_publish")
                    .value("last", InputNodePolicies::MsgPublishPolicy::LAST)
                    .value("all", InputNodePolicies::MsgPublishPolicy::ALL)
                    .export_values();
        }

        {
            bpy::scope fn_policies_scope =  bpy::class_<node_policies_ns::functional_node_ns>("functional_node");

            bpy::enum_<FunctionalNodePolicies::ExecutionPolicy>("exec_policy")
                    .value("on_new_message", FunctionalNodePolicies::ExecutionPolicy::ON_NEW_INPUT)
                    .value("always", FunctionalNodePolicies::ExecutionPolicy::ALWAYS)
                    .export_values();
        }

        {
            bpy::scope output_policies_scope =  bpy::class_<node_policies_ns::output_node_ns>("output_node");

            bpy::enum_<OutputNodePolicies::MsgPublishPolicy>("msg_publish")
                    .value("series", OutputNodePolicies::MsgPublishPolicy::SERIES)
                    .value("batch", OutputNodePolicies::MsgPublishPolicy::BATCH)
                    .export_values();
        }
    }

    // basic decorators
    bpy::class_<PythonFunctionalNode>("FunctionalNode",
                                      bpy::init<const std::string &, const boost::python::list &, FunctionalNodePolicies::ExecutionPolicy>(
                                              (bpy::arg("name"),
                                                      bpy::arg("outputs"),
                                                      bpy::arg("exec_policy") = FunctionalNodePolicies::ExecutionPolicy::ON_NEW_INPUT)))
            .def("__call__", &PythonFunctionalNode::pySetup);

    bpy::class_<F2FEdge>("FromFunctionalNode", bpy::init<const std::string &, const std::string &>( (bpy::arg("keyword"), bpy::arg("address")) ))
            .def("__call__", &F2FEdge::pySetup);

    bpy::class_< InputDummyEdge >("InputDummy", bpy::init<const std::string &, const std::string &, boost::python::object>((bpy::arg("keyword"), bpy::arg("address"), bpy::arg("value")) ))
            .def("__call__", &InputDummyEdge::pySetup);

    bpy::class_< OutputDummyEdge >("OutputDummy", bpy::init<const std::string &, const std::string &>( (bpy::arg("keyword"), bpy::arg("address")) ))
            .def("__call__", &OutputDummyEdge::pySetup);

    bpy::register_ptr_to_python<std::shared_ptr<PythonFunctionalNode> >();
    
    // other node types decorators
    bpy::class_< InputEngineEdge >("FromEngine", bpy::init<const std::string &, const std::string &,
                                   InputNodePolicies::MsgCachePolicy>((bpy::arg("keyword"), bpy::arg("address"), bpy::arg("cache_policy") = InputNodePolicies::KEEP_CACHE) ))
            .def("__call__", &InputEngineEdge::pySetup);

    bpy::class_< OutputEngineEdge >("ToEngine", bpy::init<const std::string &, const std::string &>( (bpy::arg("keyword"), bpy::arg("address")) ))
            .def("__call__", &OutputEngineEdge::pySetup);


#ifdef ROS_ON
    bpy::class_< RosEdgeFactory >("RosSubscriber", bpy::init<const std::string &, const std::string &,
                                  const bpy::object &, InputNodePolicies::MsgPublishPolicy, InputNodePolicies::MsgCachePolicy>(
                                          (bpy::arg("keyword"), bpy::arg("address"), bpy::arg("type"),
                                                  bpy::arg("publish_policy") =  InputNodePolicies::LAST,
                                                  bpy::arg("cache_policy") =  InputNodePolicies::KEEP_CACHE) ))
            .def("__call__", &RosEdgeFactory::pySetupSelector);

    bpy::class_< RosEdgeFactoryOutput >("RosPublisher", bpy::init<const std::string &, const std::string &, const bpy::object &>((bpy::arg("keyword"), bpy::arg("address"), bpy::arg("type")) ))
            .def("__call__", &RosEdgeFactoryOutput::pySetupSelector);
#endif

#ifdef SPINNAKER_ON
    bpy::class_< InputSpinnakerEdge >("FromSpinnaker", bpy::init<const std::string &, const std::string &,
            InputNodePolicies::MsgPublishPolicy, InputNodePolicies::MsgCachePolicy>(
                    (bpy::arg("keyword"), bpy::arg("address"),
                            bpy::arg("publish_policy") =  InputNodePolicies::LAST,
                            bpy::arg("cache_policy") =  InputNodePolicies::KEEP_CACHE) ))
            .def("__call__", &InputSpinnakerEdge::pySetup);

    bpy::class_< OutputSpinnakerEdge >("ToSpinnaker", bpy::init<const std::string &, const std::string &>( (bpy::arg("keyword"), bpy::arg("address")) ))
            .def("__call__", &OutputSpinnakerEdge::pySetup);
#endif
}
