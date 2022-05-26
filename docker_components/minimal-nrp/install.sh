cd ../..
NRP_ROOT=${PWD}
FILE_PATH="docker_components/minimal-nrp"
PYTHON_ENGINE="nrp_python_json_engine/nrp_python_json_engine/engine_server"
mkdir ${FILE_PATH}/remote_sim
cp ${PYTHON_ENGINE}/python_json_engine.py ${FILE_PATH}/remote_sim

sudo docker build -t nrp_minimal:v0 \
--build-arg FILE_PATH=${FILE_PATH} \
-f ${FILE_PATH}/Dockerfile .

rm ${FILE_PATH}/remote_sim/python_json_engine.py
