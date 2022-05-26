cd ../..
NRP_ROOT=${PWD}
FILE_PATH="docker_components/full-nrp"

sudo docker build -t nrp_env:v0 .
sudo docker kill $(sudo docker ps -q)
sudo docker rm $(sudo docker ps -a -q)


sudo docker build -t nrp:v0 -f ${FILE_PATH}/Dockerfile .
