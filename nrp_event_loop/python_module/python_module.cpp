#include <boost/python.hpp>

#include "nrp_event_loop/python/functional_node.h"
#include "nrp_event_loop/nodes/dummy/input_dummy.h"
#include "nrp_event_loop/nodes/dummy/output_dummy.h"
#include "nrp_event_loop/nodes/engine/input_node.h"
#include "nrp_event_loop/nodes/engine/output_node.h"

#ifdef ROS_ON
#include "nrp_event_loop/nodes/ros/ros_edge_factory.h"
#endif

#include "nrp_event_loop/config/cmake_constants.h"

BOOST_PYTHON_MODULE(EVENT_LOOP_PYTHON_MODULE_NAME)
{

    // basic decorators
    bpy::class_<PythonFunctionalNode>("FunctionalNode", bpy::init<const std::string &, const boost::python::list&>( (bpy::arg("name"), bpy::arg("outputs")) ))
            .def("__call__", &PythonFunctionalNode::pySetup);

    bpy::class_<F2FEdge>("FromFunctionalNode", bpy::init<const std::string &, const std::string &>( (bpy::arg("keyword"), bpy::arg("address")) ))
            .def("__call__", &F2FEdge::pySetup);

    bpy::class_< InputDummyEdge >("InputDummy", bpy::init<const std::string &, const std::string &, boost::python::object>((bpy::arg("keyword"), bpy::arg("address"), bpy::arg("value")) ))
            .def("__call__", &InputDummyEdge::pySetup);

    bpy::class_< OutputDummyEdge >("OutputDummy", bpy::init<const std::string &, const std::string &>( (bpy::arg("keyword"), bpy::arg("address")) ))
            .def("__call__", &OutputDummyEdge::pySetup);

    bpy::register_ptr_to_python<std::shared_ptr<PythonFunctionalNode> >();
    
    // other node types decorators
    bpy::class_< InputEngineEdge >("FromEngine", bpy::init<const std::string &, const std::string &>((bpy::arg("keyword"), bpy::arg("address")) ))
            .def("__call__", &InputEngineEdge::pySetup);

    bpy::class_< OutputEngineEdge >("ToEngine", bpy::init<const std::string &, const std::string &>( (bpy::arg("keyword"), bpy::arg("address")) ))
            .def("__call__", &OutputEngineEdge::pySetup);

#ifdef ROS_ON
    bpy::class_< RosEdgeFactory >("RosSubscriber", bpy::init<const std::string &, const std::string &, const bpy::object &>((bpy::arg("keyword"), bpy::arg("address"), bpy::arg("type")) ))
            .def("__call__", &RosEdgeFactory::pySetupSelector);

    bpy::class_< RosEdgeFactoryOutput >("RosPublisher", bpy::init<const std::string &, const std::string &, const bpy::object &>((bpy::arg("keyword"), bpy::arg("address"), bpy::arg("type")) ))
            .def("__call__", &RosEdgeFactoryOutput::pySetupSelector);
#endif

}
