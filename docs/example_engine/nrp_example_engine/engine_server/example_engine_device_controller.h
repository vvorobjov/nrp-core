#ifndef EXAMPLE_ENGINE_DEVICE_CONTROLLER_H
#define EXAMPLE_ENGINE_DEVICE_CONTROLLER_H

#include <nlohmann/json.hpp>

#include "nrp_json_engine_protocol/device_interfaces/json_device.h"
#include "nrp_general_library/engine_interfaces/device_controller.h"
#include "nrp_general_library/device_interface/device_interface.h"

class ExampleEngineDeviceController
        : public DeviceController<nlohmann::json>
{
	public:
		ExampleEngineDeviceController(DeviceIdentifier &devID)
		    : _devId(devID)
		{}

		virtual nlohmann::json * getDeviceInformation()
		{
			return &_dev;
		}

		void handleDeviceData(const nlohmann::json &data)
		{
			_dev = data;
		}

	private:

		DeviceIdentifier _devId;
		nlohmann::json _dev;
};


#endif // EXAMPLE_ENGINE_DEVICE_CONTROLLER_H
