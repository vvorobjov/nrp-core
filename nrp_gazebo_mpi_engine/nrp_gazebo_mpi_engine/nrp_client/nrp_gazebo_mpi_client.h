#ifndef NRP_GAZEBO_MPI_CLIENT_H
#define NRP_GAZEBO_MPI_CLIENT_H

#include "nrp_general_library/engine_interfaces/engine_mpi_interface/nrp_client/nrp_mpi_client.h"
#include "nrp_general_library/engine_interfaces/engine_interface.h"
#include "nrp_general_library/plugin_system/plugin.h"

#include "nrp_gazebo_mpi_engine/devices/physics_camera.h"
#include "nrp_gazebo_mpi_engine/devices/physics_joint.h"
#include "nrp_gazebo_mpi_engine/devices/physics_link.h"

#include "nrp_gazebo_mpi_engine/config/gazebo_config.h"

#include <unistd.h>

/*! \defgroup gazebo_json_engine "Gazebo REST JSON Engine"
Gazebo Engine based on the \ref json_engine.
 */

/*! \addtogroup gazebo_json_engine
 *  \brief NRP - Gazebo Communicator on the NRP side. Converts DeviceInterface classes from/to JSON objects
 */
class GazeboEngineJSONNRPClient
        : public NRPMPIClient<GazeboEngineJSONNRPClient, GazeboConfig, PhysicsCamera, PhysicsJoint, PhysicsLink>
{
	public:
		static constexpr FixedString DefEngineName = "gazebo_mpi";

		GazeboEngineJSONNRPClient(EngineConfigConst::config_storage_t &config, ProcessLauncherInterface::unique_ptr &&launcher);
		virtual ~GazeboEngineJSONNRPClient() override = default;

		virtual RESULT initialize() override;

		virtual RESULT shutdown() override;
};

using GazeboEngineJSONLauncher = GazeboEngineJSONNRPClient::EngineLauncher<GazeboEngineJSONNRPClient::DefEngineName>;

CREATE_NRP_ENGINE_LAUNCHER(GazeboEngineJSONLauncher);

#endif // NRP_GAZEBO_MPI_CLIENT_H
