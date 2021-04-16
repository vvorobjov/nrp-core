#ifndef EXAMPLE_CONFIG_H
#define EXAMPLE_CONFIG_H

#include <string_view>
#include "nrp_general_library/utils/fixed_string.h"

struct ExampleConfigConst
{
	static constexpr FixedString EngineType = "example_engine";
    static constexpr FixedString EngineSchema = "https://neurorobotics.net/engines/engine_example.json#/EngineExample";
};

#endif // EXAMPLE_CONFIG_H
