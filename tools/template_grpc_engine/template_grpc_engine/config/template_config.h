#ifndef ${engine_name_uppercase}_CONFIG_H
#define ${engine_name_uppercase}_CONFIG_H

struct ${engine_name}GrpcConfigConst
{
    static constexpr char EngineType[] = "${engine_name_lowercase}_grpc_engine";
    static constexpr char EngineSchema[] = "json://nrp-core/engines/engine_comm_protocols.json#/engine_grpc";
};

#endif // ${engine_name_uppercase}_CONFIG_H

// EOF
