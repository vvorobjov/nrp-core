#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_general_library/utils/time_utils.h"

#ifdef TIME_PROFILE

std::chrono::time_point<std::chrono::high_resolution_clock> TimeProfiler::start = std::chrono::high_resolution_clock::now();
std::map<std::string, std::ofstream> TimeProfiler::files = std::map<std::string, std::ofstream>();

void TimeProfiler::recordTimePoint(const std::string& filename, bool newLine)
{ recordDuration(filename, std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - TimeProfiler::start), newLine); }

void TimeProfiler::recordDuration(const std::string& filename, const std::chrono::microseconds& duration, bool newLine)
{
    if(!files.contains(filename)) {
        files.emplace(filename, std::ofstream());
        std::filesystem::create_directory("time_logs");
        files[filename].open("time_logs/"+filename+".log", std::ios::out | std::ios::trunc);
    }

    files[filename] << duration.count();
    if(newLine)
        files[filename] << std::endl;
    else
        files[filename] << " ";
}

BlockProfiler::BlockProfiler(const std::string& filename) :
        _filename(filename)
{ _start = std::chrono::high_resolution_clock::now(); }

BlockProfiler::~BlockProfiler()
{ TimeProfiler::recordDuration(_filename, std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - _start)); }


#endif

double getRoundedRunTimeMs(const SimulationTime runTime, const float simulationResolutionMs)
{
    // Convert SimulationTime to milliseconds

    const double timeStepMsDouble = fromSimulationTime<double, std::milli>(runTime);

    // Round the time step to account for simulation resolution

    const auto numSteps = std::round(timeStepMsDouble / simulationResolutionMs);

    if(numSteps == 0)
    {
        throw NRPException::logCreate("Simulation time step too small, step (ms): " +
                                      std::to_string(timeStepMsDouble)              +
                                      ", resolution (ms): "                         +
                                      std::to_string(simulationResolutionMs)        );
    }

    const double runTimeMsRounded = std::round(timeStepMsDouble / simulationResolutionMs) * simulationResolutionMs;

    return runTimeMsRounded;
}