//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2021 NRP Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This project has received funding from the European Union’s Horizon 2020
// Framework Programme for Research and Innovation under the Specific Grant
// Agreement No. 945539 (Human Brain Project SGA3).
//

#include "nrp_world_plugin/nrp_world_plugin.h"

#include "nrp_gazebo_grpc_engine/engine_server/nrp_communication_controller.h"

#include <gazebo/physics/PhysicsEngine.hh>
#include <gazebo/physics/World.hh>

void gazebo::NRPWorldPlugin::Load(gazebo::physics::WorldPtr world, sdf::ElementPtr sdf)
{
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);

	NRPLogger::info("NRPWorldPlugin: Loading world plugin...");

	this->_initialWorldState = gazebo::physics::WorldState(world);
	this->_world             = world;
	this->_worldSDF          = sdf;

	// Pause simulation
	world->SetPaused(true);

	// Tell simulation to go as fast as possible
//	world->Physics()->SetRealTimeUpdateRate(0);

	NRPLogger::info("NRPWorldPlugin: Registering world controller with communicator...");
	NRPCommunicationController::getInstance().registerStepController(this);
}

void gazebo::NRPWorldPlugin::Reset()
{
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);
	// Reset the world to the initial state
	// Reset doesn't take into account the <state> tag, so we have to reload it manually

	WorldPlugin::Reset(); // <- does nothing
	this->_world->SetState(this->_initialWorldState);
}

SimulationTime gazebo::NRPWorldPlugin::runLoopStep(SimulationTime timeStep)
{
	NRP_LOGGER_TRACE("{} called [ timeStep: {} ]", __FUNCTION__, timeStep.count());
	
	std::scoped_lock lock(this->_lockLoop);

	//std::cout << "NRPWorldPlugin: Executing loop step\n";

	try
	{
		const auto     maxStepSizeUs = toSimulationTime<double, std::ratio<1>>(this->_world->Physics()->GetMaxStepSize());
		const unsigned numIterations = std::max(static_cast<unsigned int>(static_cast<double>(timeStep.count()) / static_cast<double>(maxStepSizeUs.count())), 1u);

		this->startLoop(numIterations);
	}
	catch(const std::exception &e)
	{
		NRPLogger::error("Error while executing gazebo step: [ {} ]",  e.what());
		throw;
	}

	//std::cout << "NRPWorldPlugin: Finished loop step. Time:" <<  this->_world->SimTime().Double() << "\n";

	const auto simTime = this->_world->SimTime();

	return toSimulationTime<int32_t, std::ratio<1>>(simTime.sec) + toSimulationTime<int32_t, std::nano>(simTime.nsec);
}

bool gazebo::NRPWorldPlugin::finishWorldLoading()
{
	NRP_LOGGER_TRACE("{} called", __FUNCTION__);

	NRPLogger::info("Finalizing gazebo loading... Time: {}",  this->_world->SimTime().Double());
	
	// Run a single iteration and reset the world
	// This should force all plugins to load

	this->startLoop(1);
	this->Reset();

	NRPLogger::info("Gazebo loading finalized Time: {}",  this->_world->SimTime().Double());

	return true;
}

bool gazebo::NRPWorldPlugin::resetWorld()
{
	NRPLogger::debug("gazebo::NRPWorldPlugin::resetWorld(): Time before: {}", this->_world->SimTime().Double());

	// There is a World::Reset() function:
	// this->_world->Reset() 
	// that is supposed to call Reset() functions for all plugins, including model and sensor plugins.
	// But for some reason this->_world->Reset()  just calls NRPWorldPlugin::Reset() function
	// i.e. only WorldPlugin is affected.
	// it would be cool to use this feature when the problem is fixed:
	// this->_world->Reset();

	this->Reset();

	NRPLogger::debug("gazebo::NRPWorldPlugin::resetWorld(): Time after: {}", this->_world->SimTime().Double());

	return true;
}

void gazebo::NRPWorldPlugin::startLoop(unsigned int numIterations)
{
	NRP_LOGGER_TRACE("{} called [ numIterations: {} ]", __FUNCTION__, numIterations);

	this->_world->Step(numIterations);
	this->_world->SetPaused(true);
}
