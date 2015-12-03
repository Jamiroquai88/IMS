#include "unistd.h"
#include "train.h"
#include "schedule.h"
#include "station.h"
#include "adjacent_station.h"
#include "time_interval.h"
#include "main_station.h"
#include "debug.h"

/**
 * Main simulation program.
 *
 * Simulation time unit is ...
 */

int main(int argc, char *argv[])
{
    DBG_LOG("SIMULATION START - DEBUG");

    Init(0, CTimeInterval::TimeToMinutes(10,0,0));
    // Main station
    CMainStation& mainStation = CMainStation::GetInstance();
    mainStation.SetTitle("Zilina");

    CSchedule schedule;
    if(!schedule.OpenConfigFile() || !schedule.ParseAndPlan(mainStation))
        return -1;



    Run();
    DBG_LOG(mainStation.GetAdjacentStations().size());
    return 0;
}
