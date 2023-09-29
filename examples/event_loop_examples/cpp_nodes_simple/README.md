In order to run this example "test.h" file must be compiled into a FN Factory module. This can be done by invoking the command below from this folder:

build_fn_factory_module.sh test.h

This will generate the FN Factory module "libFNFactoryModule.so". Afterwards the experiment can be run normally with NRPCoreSim:

NRPCoreSim -c simulation_config.json
