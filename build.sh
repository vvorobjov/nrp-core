NRP_INSTALL_PATH=$HOME/.local/nrp
[ -e build ] && sudo rm -r build
mkdir build

cd build

[ -e $NRP_INSTALL_PATH ] && sudo rm -r $NRP_INSTALL_PATH
mkdir -p $NRP_INSTALL_PATH

export C=/usr/bin/gcc-10; export CXX=/usr/bin/g++-10
cmake .. -DCMAKE_INSTALL_PREFIX=$NRP_INSTALL_PATH
make -j4
make install

