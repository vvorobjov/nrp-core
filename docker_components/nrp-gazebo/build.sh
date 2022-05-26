cd nrp-core
NRP_INSTALL_PATH=/root/.local/nrp
[ -e build ] && rm -r build
mkdir build

cd build

[ -e $NRP_INSTALL_PATH ] && rm -r $NRP_INSTALL_PATH
mkdir -p $NRP_INSTALL_PATH

cmake .. -DCMAKE_INSTALL_PREFIX=$NRP_INSTALL_PATH \
	-DENABLE_TESTING=OFF \
	-DENABLE_ROS=OFF -DENABLE_MQTT=OFF -DENABLE_SPINNAKER=OFF \
	-DENABLE_NEST=OFF -DBUILD_RST=OFF
make -j4
make install

