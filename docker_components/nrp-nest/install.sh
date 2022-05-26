cd ../..
NRP_ROOT=${PWD}
FILE_PATH="docker_components/nrp-nest"

sudo docker build -t nrp_nest:v0 \
--build-arg FILE_PATH=${FILE_PATH} \
-f ${FILE_PATH}/Dockerfile .
