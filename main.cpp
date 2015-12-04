#include "unistd.h"
#include "train.h"
#include "schedule.h"
#include "station.h"
#include "adjacent_station.h"
#include "time_interval.h"
#include "main_station.h"
#include "debug.h"
#include "defect_generator.h"

// In days
#define SMALL_DEFECT_FREQ 24
#define MEDIUM_DEFECT_FREQ 41
#define LARGE_DEFECT_FREQ 11

// In minutes
#define SMALL_DEFECT_DELAY 2
#define MEDIUM_DEFECT_DELAY 19
#define LARGE_DEFECT_DELAY 80

/**
 * Main simulation program.
 *
 * Simulation time unit is ...
 */

int main(int argc, char *argv[])
{
    DBG_LOG("SIMULATION START - DEBUG");

    Init(0, CTimeInterval::TimeToMinutes(0,0, 100));

    // Main station
    CMainStation& mainStation = CMainStation::GetInstance();
    mainStation.SetTitle("Zilina");

    CSchedule schedule;
    if(!schedule.OpenConfigFile() || !schedule.ParseAndPlan(mainStation))
        return -1;

    CDefectGenerator smallDefects(CTimeInterval::TimeToMinutes(0,0, SMALL_DEFECT_FREQ),
      SMALL_DEFECT_DELAY);
    smallDefects.Activate(CTimeInterval::TimeToMinutes(0,0, SMALL_DEFECT_FREQ));

    CDefectGenerator mediumDefects(CTimeInterval::TimeToMinutes(0,0, MEDIUM_DEFECT_FREQ),
      MEDIUM_DEFECT_DELAY);
    mediumDefects.Activate(CTimeInterval::TimeToMinutes(0,0, MEDIUM_DEFECT_FREQ));

    CDefectGenerator largeDefects(CTimeInterval::TimeToMinutes(0,0, LARGE_DEFECT_FREQ),
      LARGE_DEFECT_DELAY);
    largeDefects.Activate(CTimeInterval::TimeToMinutes(0,0, LARGE_DEFECT_FREQ));

    Run();
    DBG_LOG(mainStation.GetAdjacentStations().size());
    return 0;
}
