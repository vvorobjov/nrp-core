#ifndef ${engine_name_uppercase}_CONFIG_H
#define ${engine_name_uppercase}_CONFIG_H

#include <string_view>
#include "nrp_general_library/utils/fixed_string.h"

struct ${engine_name}JSONConfigConst
{
	static constexpr FixedString EngineType = "${engine_name_lowercase}_json_engine";
    static constexpr FixedString EngineSchema = "https://neurorobotics.net/engines/engine_comm_protocols.json#/engine_json";
};

#endif // ${engine_name_uppercase}_CONFIG_H

// EOF
