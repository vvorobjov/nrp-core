#!/bin/bash

set -e

if [[ $# -ne 1 ]] ; then
    echo 'This script requires a Docker service name as an argument.'
    echo 'Pick a service name from docker-compose.yaml to build and provide it as an argument.'
    exit 1
fi

if [ -f ".env" ]; then
    source .env
else
    echo ".env file not found. Skipping..."
fi

export NRP_DOCKER_REGISTRY="${NRP_DOCKER_REGISTRY:-nrp-local}"
export NRP_CORE_TAG="${NRP_CORE_TAG:-local}"

target_service_name=$1

docker_compose_file="docker-compose.yaml"

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

    # Opt-in reuse: when NRP_REUSE_EXISTING_IMAGES is set, skip (re)building
    # any *dependency* layer whose image is already present locally. CI
    # pre-seeds the *-env layers by pulling + tagging the prebuilt GHCR
    # images, so the multi-hour NEST/Gazebo/ROS env recompile is skipped.
    # The requested target service is always (re)built, and local dev /
    # the env-image workflow keep their always-rebuild behaviour when the
    # variable is unset.
    if [[ -n "${NRP_REUSE_EXISTING_IMAGES:-}" ]] \
        && [[ $target_service_name != $service_name ]] \
        && docker image inspect "$image_name" > /dev/null 2>&1; then
        echo "Image $image_name already present; reusing it for service $service_name (skipping build)"
        return
    fi

    if [[ $base_image == ${NRP_DOCKER_REGISTRY}/* ]]; then
        echo "Trying to build the base image $base_image"
        base_image_service_name=$(envsubst < "${docker_compose_file}" | awk -v RS= -v FS="\n" -v image_name="image: ${base_image}" '$0 ~ image_name {print $1}' | awk -F':' '{print $1}')
        echo "Looking for the service $base_image_service_name"
        build_service $docker_compose_file $base_image_service_name
    else
        echo "The base image $base_image needs no building"
    fi

    echo "Building the service $service_name from $service_file"
    docker compose -f $service_file build $service_name --progress=plain

    if [[ $target_service_name == $service_name ]]; then
        exit
    fi
}

if [ "$target_service_name" = "all" ]; then
    docker compose -f "${docker_compose_file}" build
else
    build_service $docker_compose_file $target_service_name
fi