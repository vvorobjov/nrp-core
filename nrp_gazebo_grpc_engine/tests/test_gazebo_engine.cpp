#include <gtest/gtest.h>

#include "nrp_gazebo_grpc_engine/config/gazebo_config.h"
#include "nrp_gazebo_grpc_engine/config/nrp_gazebo_cmake_constants.h"
#include "nrp_gazebo_grpc_engine/devices/physics_camera.h"
#include "nrp_gazebo_grpc_engine/devices/physics_joint.h"
#include "nrp_gazebo_grpc_engine/devices/physics_link.h"
#include "nrp_gazebo_grpc_engine/nrp_client/gazebo_engine_grpc_nrp_client.h"
#include "nrp_general_library/process_launchers/process_launcher_basic.h"

#include "tests/test_env_cmake.h"

#include <fstream>

TEST(TestGazeboEngine, Start)
{
	// Setup config
	ConfigStorage<nlohmann::json> confHolder;
	confHolder.Data = nlohmann::json({{GazeboConfigConst::GazeboWorldFile, ""}});

	GazeboConfig conf(confHolder);
	conf.gazeboRNGSeed() = 12345;
	conf.gazeboWorldFile() = TEST_EMPTY_WORLD_FILE;
	conf.maxWorldLoadTime() = 1;

	conf.engineProcStartParams().push_back("--verbose");

	confHolder.Data = conf.writeConfig();

	std::cout << confHolder.Data.dump(4, ' ') << std::endl;

	// Launch gazebo server
	GazeboEngineGrpcLauncher launcher;
	PtrTemplates<GazeboEngineGrpcNRPClient>::shared_ptr engine = std::dynamic_pointer_cast<GazeboEngineGrpcNRPClient>(
	        launcher.launchEngine(confHolder, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic())));

	ASSERT_NE(engine, nullptr);

	ASSERT_EQ(engine->initialize(), EngineInterface::RESULT::ERROR);
}

TEST(TestGazeboEngine, WorldPlugin)
{
	// Setup config
	ConfigStorage<nlohmann::json> confHolder;
	confHolder.Data = nlohmann::json({{GazeboConfigConst::GazeboWorldFile, ""}});

	GazeboConfig conf(confHolder);
	conf.gazeboRNGSeed() = 12345;
	conf.gazeboWorldFile() = TEST_WORLD_PLUGIN_FILE;

	conf.engineProcStartParams().push_back("--verbose");

	confHolder.Data = conf.writeConfig();

	// Launch gazebo server
	GazeboEngineGrpcLauncher launcher;
	PtrTemplates<GazeboEngineGrpcNRPClient>::shared_ptr engine = std::dynamic_pointer_cast<GazeboEngineGrpcNRPClient>(
	        launcher.launchEngine(confHolder, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic())));

	ASSERT_NE(engine, nullptr);

	sleep(1);

	ASSERT_EQ(engine->initialize(), EngineInterface::RESULT::SUCCESS);

	ASSERT_EQ(engine->runLoopStep(SimulationTime(1000000)), EngineInterface::RESULT::SUCCESS);

	ASSERT_EQ(engine->waitForStepCompletion(5.0f), EngineInterface::RESULT::SUCCESS);
}

TEST(TestGazeboEngine, CameraPlugin)
{
	// Setup config
	ConfigStorage<nlohmann::json> confHolder;
	confHolder.Data = nlohmann::json({{GazeboConfigConst::GazeboWorldFile, ""}});

	GazeboConfig conf(confHolder);
	conf.gazeboRNGSeed() = 12345;
	conf.gazeboWorldFile() = TEST_CAMERA_PLUGIN_FILE;

	conf.engineProcEnvParams().push_back("GAZEBO_MODEL_PATH=" TEST_GAZEBO_MODELS_DIR ":$GAZEBO_MODEL_PATH");

	conf.engineProcStartParams().push_back("--verbose");

	confHolder.Data = conf.writeConfig();

	// Launch gazebo server
	GazeboEngineGrpcLauncher launcher;
	PtrTemplates<GazeboEngineGrpcNRPClient>::shared_ptr engine = std::dynamic_pointer_cast<GazeboEngineGrpcNRPClient>(
	        launcher.launchEngine(confHolder, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic())));

	ASSERT_NE(engine, nullptr);

	sleep(1);

	ASSERT_EQ(engine->initialize(), EngineInterface::RESULT::SUCCESS);

	//ASSERT_EQ(engine->runLoopStep(1.0f), EngineInterface::RESULT::SUCCESS);
	//ASSERT_EQ(engine->waitForStepCompletion(5.0f), EngineInterface::RESULT::SUCCESS);

	auto devices = engine->requestOutputDevices({DeviceIdentifier("nrp_camera::camera", conf.engineName(), PhysicsCamera::TypeName.data())});
	ASSERT_EQ(devices.size(), 1);

	const PhysicsCamera &camDat = dynamic_cast<const PhysicsCamera&>(*(devices[0]));
//	std::cout << "Image Height:" << camDat.imageHeight() << std::endl;
//	std::cout << "Image Width:" << camDat.imageWidth() << std::endl;
//	std::cout << "Image Pixel Size:" << std::to_string(camDat.imagePixelSize()) << std::endl;
//	std::cout << "Image Pixel Num:" << camDat.imageData().size() << std::endl;

	ASSERT_EQ(camDat.imageHeight(), 240);
	ASSERT_EQ(camDat.imageWidth(),  320);
	ASSERT_EQ(camDat.imagePixelSize(),  3);
	ASSERT_EQ(camDat.imageData().size(), 320*240*3);

	// Check if cam data is correct (WARNING: Data might be incorrect due to non-determinism. Should be checked)
	//std::fstream compareCamDat(TEST_CAMERA_DATA_FILE, std::ios_base::in);
	//for(const auto dat : camDat.imageData())
	//{
	//	unsigned int val;
	//	compareCamDat >> val;
	//	ASSERT_EQ(dat, val);
	//}
	//compareCamDat.close();
}


TEST(TestGazeboEngine, JointPlugin)
{
	// Setup config
	ConfigStorage<nlohmann::json> confHolder;
	confHolder.Data = nlohmann::json({{GazeboConfigConst::GazeboWorldFile, ""}});

	GazeboConfig conf(confHolder);
	conf.gazeboRNGSeed() = 12345;
	conf.gazeboWorldFile() = TEST_JOINT_PLUGIN_FILE;

	conf.engineProcEnvParams().push_back("GAZEBO_MODEL_PATH=" TEST_GAZEBO_MODELS_DIR ":$GAZEBO_MODEL_PATH");

	conf.engineProcStartParams().push_back("--verbose");

	confHolder.Data = conf.writeConfig();

	// Launch gazebo server
	GazeboEngineGrpcLauncher launcher;
	PtrTemplates<GazeboEngineGrpcNRPClient>::shared_ptr engine = std::dynamic_pointer_cast<GazeboEngineGrpcNRPClient>(
	        launcher.launchEngine(confHolder, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic())));

	ASSERT_NE(engine, nullptr);

	sleep(1);

	ASSERT_EQ(engine->initialize(), EngineInterface::RESULT::SUCCESS);

	//ASSERT_EQ(engine->runLoopStep(1.0f), EngineInterface::RESULT::SUCCESS);
	//ASSERT_EQ(engine->waitForStepCompletion(5.0f), EngineInterface::RESULT::SUCCESS);

	// Test device data getting
	auto devices = engine->requestOutputDevices({DeviceIdentifier("youbot::base_footprint_joint", conf.engineName(), PhysicsJoint::TypeName.data())});
	ASSERT_EQ(devices.size(), 1);

	const PhysicsJoint *pJointDev = dynamic_cast<const PhysicsJoint*>(devices[0].get());
	ASSERT_EQ(pJointDev->position(), 0);

	// Test device data setting
	const auto newTargetPos = 2.0f;

	PhysicsJoint newJointDev(pJointDev->id());
	newJointDev.setEffort(NAN);
	newJointDev.setVelocity(NAN);
	newJointDev.setPosition(newTargetPos);

	ASSERT_EQ(engine->handleInputDevices({&newJointDev}), EngineInterface::RESULT::SUCCESS);
}

TEST(TestGazeboEngine, LinkPlugin)
{
	// Setup config
	ConfigStorage<nlohmann::json> confHolder;
	confHolder.Data = nlohmann::json({{GazeboConfigConst::GazeboWorldFile, ""}});

	GazeboConfig conf(confHolder);
	conf.gazeboRNGSeed() = 12345;
	conf.gazeboWorldFile() = TEST_LINK_PLUGIN_FILE;

	conf.engineProcEnvParams().push_back("GAZEBO_MODEL_PATH=" TEST_GAZEBO_MODELS_DIR ":$GAZEBO_MODEL_PATH");

	conf.engineProcStartParams().push_back("--verbose");

	confHolder.Data = conf.writeConfig();

	// Launch gazebo server
	GazeboEngineGrpcLauncher launcher;
	PtrTemplates<GazeboEngineGrpcNRPClient>::shared_ptr engine = std::dynamic_pointer_cast<GazeboEngineGrpcNRPClient>(
	        launcher.launchEngine(confHolder, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic())));

	ASSERT_NE(engine, nullptr);

	sleep(1);

	ASSERT_EQ(engine->initialize(), EngineInterface::RESULT::SUCCESS);

	//ASSERT_EQ(engine->runLoopStep(1.0f), EngineInterface::RESULT::SUCCESS);
	//ASSERT_EQ(engine->waitForStepCompletion(5.0f), EngineInterface::RESULT::SUCCESS);

	// Test device data getting
	auto devices = engine->requestOutputDevices({DeviceIdentifier("link_youbot::base_footprint", conf.engineName(), PhysicsJoint::TypeName.data())});
	ASSERT_EQ(devices.size(), 1);

	const PhysicsLink *pLinkDev = dynamic_cast<const PhysicsLink*>(devices[0].get());
	ASSERT_NE(pLinkDev, nullptr);

	// TODO: Check that link state is correct
}
