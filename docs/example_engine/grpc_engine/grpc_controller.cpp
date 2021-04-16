template<class SERIALIZER>
class MyDeviceController
    : public EngineDeviceController<SERIALIZER, MyDevice>
{
	public:
		MyDeviceController(const std::string &devName)
		    : EngineDeviceController<SERIALIZER, MyDevice>(MyDevice::createID(devName, "")),
		      _data(DeviceIdentifier(*this))
		{}

		virtual void handleDeviceDataCallback(MyDevice &&data) override
		{
			this->_data.setScalar(data.getScalar());
			this->_data.setArray(data.getArray());
			this->_data.setVector(data.getVector());
		}

		virtual const MyDevice *getDeviceInformationCallback() override
		{
			return &this->_data;
		}

		MyDevice _data;
};