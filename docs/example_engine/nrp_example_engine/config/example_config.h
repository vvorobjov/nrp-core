#ifndef EXAMPLE_CONFIG_H
#define EXAMPLE_CONFIG_H

#include <string_view>
#include "nrp_general_library/utils/fixed_string.h"

struct ExampleConfigConst
{
    static constexpr char EngineType[] = "example_engine";
    static constexpr char EngineSchema[] = "json://nrp-core/engines/engine_example.json#/EngineExample";
};

#endif // EXAMPLE_CONFIG_H
