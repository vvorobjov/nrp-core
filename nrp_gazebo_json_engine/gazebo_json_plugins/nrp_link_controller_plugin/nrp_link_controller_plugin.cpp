#include "nrp_link_controller_plugin/nrp_link_controller_plugin.h"

#include "nrp_communication_controller/nrp_communication_controller.h"

#include <gazebo/physics/Model.hh>
#include <gazebo/physics/Link.hh>

using namespace nlohmann;

template<class T>
inline float ToFloat(const T &val)
{
	return static_cast<float>(val);
}

gazebo::LinkDeviceController::LinkDeviceController(const std::string &linkName, const gazebo::physics::LinkPtr &link)
    : EngineJSONDeviceController<PhysicsLink>(_data.createID(linkName, "")),
      _data(_data.createID(linkName, "")),
      _link(link)
{}

void gazebo::LinkDeviceController::handleDeviceDataCallback(PhysicsLink &&data)
{}

const PhysicsLink *gazebo::LinkDeviceController::getDeviceInformationCallback()
{
	const auto &pose = this->_link->WorldCoGPose();
	this->_data.setPosition({ ToFloat(pose.Pos().X()), ToFloat(pose.Pos().Y()), ToFloat(pose.Pos().Z())	});
	this->_data.setRotation({ ToFloat(pose.Rot().X()), ToFloat(pose.Rot().Y()), ToFloat(pose.Rot().Z())	});

	const auto &linVel = this->_link->WorldLinearVel();
	this->_data.setLinVel({ ToFloat(linVel.X()), ToFloat(linVel.Y()), ToFloat(linVel.Z())	});

	const auto &angVel = this->_link->WorldAngularVel();
	this->_data.setAngVel({ ToFloat(angVel.X()), ToFloat(angVel.Y()), ToFloat(angVel.Z())	});

	return &(this->_data);
}

void gazebo::NRPLinkControllerPlugin::Load(gazebo::physics::ModelPtr model, sdf::ElementPtr)
{
	auto &commControl = NRPCommunicationController::getInstance();

	// Register a device for each link
	auto links = model->GetLinks();
	for(const auto &link : links)
	{
		const auto deviceName = NRPCommunicationController::createDeviceName(*this, link->GetName());

		std::cout << "Registering link controller for link \"" << deviceName << "\"\n";

		this->_linkInterfaces.push_back(LinkDeviceController(deviceName, link));
		commControl.registerDevice(deviceName, &(this->_linkInterfaces.back()));
	}
}
