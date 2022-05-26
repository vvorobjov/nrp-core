git clone https://@bitbucket.org/hbpneurorobotics/models.git
mkdir exp_models
cp -r --parent $(cat model_list.txt) exp_models
cp models/create-symlinks.sh exp_models/models
sudo rm -r models

cd ../..
NRP_ROOT=${PWD}
FILE_PATH="docker_components/nrp-gazebo"

sudo docker build -t nrp_gazebo:v0 \
--build-arg FILE_PATH=${FILE_PATH} \
-f ${FILE_PATH}/Dockerfile .

rm -r ${FILE_PATH}/exp_models
