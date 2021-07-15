#!/usr/bin/env bash

# Get the root directory of the repo

repo_root=$(git rev-parse --show-toplevel)

cd "$repo_root"/build || exit 1;

# Set up the environment

source "$HOME"/.bashrc

NPROC=$(nproc)

# By default, limit jobs by 70% of cores,
# or up to 100% if we have few cores:
#    if 70% is less than 4, then use 4 parallel jobs
#    if the $NPROC is less than 4, use -j${NPROC}

J_LIM=$(python3 -c "print(max(${NPROC} * 7 // 10, min(${NPROC}, 4)))")

# Limit job scheduling by 100% of CPU load
# if 1min or 5min CPU load is higher than 100%, wait with job scheduling

L_LIM="${NPROC}"

echo "Running make with -j${J_LIM} -l${L_LIM}"
make -j"${J_LIM}" -l"${L_LIM}"
make install

# EOF
