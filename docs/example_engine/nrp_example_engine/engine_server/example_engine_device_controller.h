#ifndef EXAMPLE_ENGINE_DEVICE_CONTROLLER_H
#define EXAMPLE_ENGINE_DEVICE_CONTROLLER_H

#include "nrp_example_engine/devices/example_device.h"

class ExampleEngineDeviceControllerInterface
{
	public:
		ExampleEngineDeviceControllerInterface();
		virtual ~ExampleEngineDeviceControllerInterface() = default;

		virtual DeviceInterface::const_shared_ptr getDeviceInformation() = 0;
		virtual void handleDeviceData(DeviceInterface::unique_ptr &&data) = 0;
};

template<class DEVICE>
class ExampleEngineDeviceController
        : public ExampleEngineDeviceControllerInterface
{
	public:
		ExampleEngineDeviceController()
		{}

		virtual ~ExampleEngineDeviceController() override = default;

		virtual DeviceInterface::const_shared_ptr getDeviceInformation() override;
		virtual void handleDeviceData(DeviceInterface::unique_ptr &&data) override;
};

template<>
class ExampleEngineDeviceController<MyDevice>
        : public ExampleEngineDeviceControllerInterface
{
	public:
		ExampleEngineDeviceController(DeviceIdentifier &&devID)
		    : _dev(std::move(devID))
		{}

		virtual ~ExampleEngineDeviceController() override;

		virtual DeviceInterface::const_shared_ptr getDeviceInformation() override
		{	return this->_dev.moveToSharedPtr();	}

		virtual void handleDeviceData(DeviceInterface::unique_ptr &&data) override
		{
			this->_dev = std::move(*dynamic_cast<MyDevice*>(data.release()));
		}

	private:
		MyDevice _dev;
};


#endif // EXAMPLE_ENGINE_DEVICE_CONTROLLER_H
