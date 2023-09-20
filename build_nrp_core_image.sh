#!/bin/bash

set -e

if [[ $# -ne 1 ]] ; then
    echo 'This script requires a Docker service name as an argument.'
    echo 'Pick a service name from docker-compose-nrp.yaml to build and provide it as an argument.'
    exit 1
fi

export NRP_DOCKER_REGISTRY="${NRP_DOCKER_REGISTRY:-}"
export NRP_CORE_TAG="${NRP_CORE_TAG:-}"

target_service_name=$1

service_env_file="docker-compose-env.yaml"
service_nrp_file="docker-compose-nrp.yaml"

function build_service {
    local service_file=$1
    local service_name=$2

    local line_number=$(grep -n " ${service_name}:" $service_file | cut -d: -f1)

    if [[ -z $line_number ]]; then
        echo "Couldn't find service ${service_name} in ${service_file}"
        return
    fi

    # Get the build information until the first line that only contains whitespace.
    local build_info=$(awk -v ln=$line_number 'NR>ln && /^[[:space:]]*$/{exit} NR>ln' $service_file)
    # Substitute environment variables
    local build_info=$(echo "$build_info" | envsubst)

    local base_image=$(echo "$build_info" | grep BASE_IMAGE | awk -F 'BASE_IMAGE:' '{print $2}' | xargs)
    local image_name=$(echo "$build_info" | grep image | awk -F 'image:' '{print $2}' | xargs)

    if [[ $base_image == ${NRP_DOCKER_REGISTRY}nrp-core* ]]; then
        echo "Trying to build the base image $base_image"
        base_image_service_name=$(awk -v RS= -v FS="\n" -v image_name="${base_image}" '$0 ~ image_name {print $1}' "${service_env_file}" | awk -F':' '{print $1}')
        build_service $service_env_file $base_image_service_name
    else
        echo "The base image $base_image needs no building"
    fi

    echo "Building the service $service_name from $service_file"
    docker compose -f $service_file build $service_name

    if [[ $target_service_name == $service_name ]]; then
        exit
    fi
}

# Define Docker Compose files
compose_files=("docker-compose-nrp.yaml" "docker-compose-env.yaml")

# Build service
for compose_file in "${compose_files[@]}"; do
    build_service $compose_file $target_service_name
done
