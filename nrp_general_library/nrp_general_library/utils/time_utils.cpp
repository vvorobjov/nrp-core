#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_general_library/utils/time_utils.h"

namespace nrpTimeUtils
{
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
}