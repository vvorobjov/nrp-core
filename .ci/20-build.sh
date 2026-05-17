#!/usr/bin/env bash

# Get the root directory of the script

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

cd "$SCRIPT_DIR"/../build || exit 1;

# Set up the environment

source "$HOME"/.bashrc

NPROC=$(nproc)
# Available memory in GB (rounded down). Each heavy nrp-core C++
# compilation unit can peak around ~2.5 GB — run enough jobs in
# parallel to keep the CPU busy, but not so many that the kernel OOM-
# kills cc1plus. EBR2-27's ubuntu22 build hit this on a 30 GB /
# 16-core machine with the old j=11 setting.
MEM_GB=$(awk '/MemAvailable/ {printf "%d", $2/1024/1024}' /proc/meminfo)

# By default, limit jobs by 70% of cores,
# or up to 100% if we have few cores:
#    if 70% is less than 4, then use 4 parallel jobs
#    if the $NPROC is less than 4, use -j${NPROC}

J_LIM_CPU=$(python3 -c "print(max(${NPROC} * 7 // 10, min(${NPROC}, 4)))")
# Memory-based cap: one parallel job per ~10 GB of available RAM.
# python_module.cpp on jammy/Humble peaks above 8 GB during the
# gazebo image build at -O0 (Boost 1.74 + rclcpp transitive headers
# + Gazebo 11 headers + Humble's typesupport machinery). Outside
# the gazebo image build the typical NRPCore TU peaks closer to
# 2-3 GB, so reserve one parallel job per 4 GB of available RAM —
# fast on dev hosts, still safe on the 16-32 GB CI agents. The
# previous 10 GB/job heuristic was tuned for the worst-case
# python_module.cpp during image rebuilds and made -j2 the
# routine on a 24 GB host (slow).
J_LIM_MEM=$(python3 -c "print(max(int(${MEM_GB} // 4), 1))")
J_LIM=$(python3 -c "print(min(${J_LIM_CPU}, ${J_LIM_MEM}))")
echo "[20-build.sh] NPROC=${NPROC} MEM_GB=${MEM_GB} J_LIM_CPU=${J_LIM_CPU} J_LIM_MEM=${J_LIM_MEM} -> -j${J_LIM}"

# Limit job scheduling by 100% of CPU load
# if 1min or 5min CPU load is higher than 100%, wait with job scheduling

L_LIM="${NPROC}"

echo "Running make with -j${J_LIM} -l${L_LIM}"
make -j"${J_LIM}" -l"${L_LIM}"
make install

# EOF
