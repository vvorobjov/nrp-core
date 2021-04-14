/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020 Michael Zechmair
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project has received funding from the European Union’s Horizon 2020
 * Framework Programme for Research and Innovation under the Specific Grant
 * Agreement No. 945539 (Human Brain Project SGA3).
 */

#ifndef DUMMY_DEVICE_H
#define DUMMY_DEVICE_H

#include "nrp_general_library/device_interface/device.h"

// Helper structure, for cleaner code

struct MyDeviceConsts
{
    using MyArray  = std::array<float, 2>; // Helper type definition for our array
    using MyVector = std::vector<int>;     // Helper type definition for our vector

    static constexpr FixedString Scalar        = "scalar"; // Property name
    static constexpr float       DefaultScalar = 0.0f;     // Default value

    static constexpr FixedString Array         = "array";
    static constexpr MyArray     DefaultArray  = { 0.0f, 0.0f };

    static constexpr FixedString Vector        = "vector"; // No default value here, because it can't be a compile time constant

    // Aggregate property names into single structure PropNames
    // This will be passed to PropertyTemplates constructor

    using MyDevicePropertyNames = PropNames<Scalar, Array, Vector>;
}; //

// Actual device class
// Derives from Device, which in turn derives from PropertyTemplates
// Also derives from DummyDevicePropSpec for convenience

class MyDevice
    : public MyDeviceConsts,
      // Here we need to pass property names and specify property types
      public Device<MyDevice, "MyDevice", MyDeviceConsts::MyDevicePropertyNames, float, MyDeviceConsts::MyArray, MyDeviceConsts::MyVector>
{
    public:
        MyDevice(DeviceIdentifier    &&deviceId,
                 property_template_t &&properties = property_template_t(DefaultScalar, DefaultArray, MyVector()))
            : Device(std::move(deviceId), std::move(properties))
        {
            // Do nothing
        }

        // Getter and setter for "Scalar" property

        float getScalar() const
        {
            return getPropertyByName<Scalar>();
        }

        void setScalar(float value)
        {
            getPropertyByName<Scalar>() = value;
        }

        // Getter and setter for "Array" property

        const MyArray & getArray() const
        {
            return getPropertyByName<Array>();
        }

        void setArray(const MyArray & value)
        {
            getPropertyByName<Array>() = value;
        }

        // Getter and setter for "Vector" property

        const MyVector & getVector() const
        {
            return getPropertyByName<Vector>();
        }

        void setVector(const MyVector & value)
        {
            getPropertyByName<Vector>() = value;
        }
};

#endif // DUMMY_DEVICE_H
