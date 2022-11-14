#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_general_library/utils/time_utils.h"

SimulationTime toSimulationTimeFromSeconds(double time)
{
    return std::chrono::duration_cast<SimulationTime>(std::chrono::duration<float, std::ratio<1>>(time));
}

#ifdef TIME_PROFILE

std::chrono::time_point<std::chrono::high_resolution_clock> TimeProfiler::start = std::chrono::high_resolution_clock::now();
std::map<std::string, std::ofstream> TimeProfiler::files = std::map<std::string, std::ofstream>();
std::string TimeProfiler::timeLogsDir = "time_logs/" + getTimestamp();

void TimeProfiler::setStartTime()
{ start = std::chrono::high_resolution_clock::now(); }

void TimeProfiler::recordTimePoint(const std::string& filename, const std::string& comment, bool newLine)
{ recordDuration(filename, std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start), comment, newLine); }

void TimeProfiler::recordDuration(const std::string& filename, const std::chrono::microseconds& duration, const std::string& comment, bool newLine)
{
    if(!std::filesystem::is_directory(timeLogsDir)) {
        std::filesystem::create_directory("time_logs");
        std::filesystem::create_directory(timeLogsDir);
    }

    if(!files.count(filename)) {
        files.emplace(filename, std::ofstream());
        files[filename].open(timeLogsDir+"/"+filename+".log", std::ios::out | std::ios::trunc);
    }

    if(!comment.empty())
        files[filename] << comment << ": ";

    files[filename] << duration.count();

    if(newLine)
        files[filename] << std::endl;
    else
        files[filename] << " ";
}

BlockProfiler::BlockProfiler(const std::string& filename, const std::string& comment) :
        _filename(filename),
        _comment(comment)
{ _start = std::chrono::high_resolution_clock::now(); }

BlockProfiler::~BlockProfiler()
{ TimeProfiler::recordDuration(_filename, std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - _start), _comment); }


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

std::string getTimestamp()
{
    const auto t = std::time(nullptr);
    const auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y%m%d-%H%M%S-") << getpid();

    return oss.str();
}