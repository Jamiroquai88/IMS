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

    mainStation.AddAdjacentStation("Bohumin");
    mainStation.AddAdjacentStation("Brodno");
    mainStation.AddAdjacentStation("Bytca");
    mainStation.AddAdjacentStation("Cadca");
    mainStation.AddAdjacentStation("Cesky Tesin");
    mainStation.AddAdjacentStation("Horny Hricov");
    mainStation.AddAdjacentStation("Kralovany");
    mainStation.AddAdjacentStation("Kysucke Nove Mesto");
    mainStation.AddAdjacentStation("Povazska Bystrica");
    mainStation.AddAdjacentStation("Rudina");
    mainStation.AddAdjacentStation("Ruzomberok");
    mainStation.AddAdjacentStation("Trencin");
    mainStation.AddAdjacentStation("Vrutky");
    mainStation.AddAdjacentStation("Zilina Solinky");
    mainStation.AddAdjacentStation("Zilina Zariecie");

    // Track 127 - Zilina ------> Cadca
    mainStation.AddTrack(mainStation.GetAdjacentStation("Bohumin"), 94)
        .AddNestedSegment(new CTrack(mainStation.GetAdjacentStation("Cesky Tesin"), 65))
        .AddNestedSegment(new CTrack(mainStation.GetAdjacentStation("Cadca"), 30))
        .AddNestedSegment(new CTrack(mainStation.GetAdjacentStation("Kysucke Nove Mesto"), 10))
        .AddNestedSegment(new CTrack(mainStation.GetAdjacentStation("Rudina"), 7))
        .AddNestedSegment(new CTrack(mainStation.GetAdjacentStation("Brodno"), 5));

    // Track 126 - Zilina ------> Rajec
    mainStation.AddTrack(mainStation.GetAdjacentStation("Zilina Solinky"), 5)
        .AddNestedSegment(new CTrack(mainStation.GetAdjacentStation("Zilina Zariecie"), 2));

    // Track 120 - Bratislava ------> Zilina
    mainStation.AddTrack(mainStation.GetAdjacentStation("Trencin"), 179)
        .AddNestedSegment(new CTrack(mainStation.GetAdjacentStation("Povazska Bystrica"), 32))
        .AddNestedSegment(new CTrack(mainStation.GetAdjacentStation("Bytca"), 17))
        .AddNestedSegment(new CTrack(mainStation.GetAdjacentStation("Horny Hricov"), 7));

    // Track 180 - Zilina ------> Kosice
    mainStation.AddTrack(mainStation.GetAdjacentStation("Ruzomberok"), 57)
        .AddNestedSegment(new CTrack(mainStation.GetAdjacentStation("Kralovany"), 39))
        .AddNestedSegment(new CTrack(mainStation.GetAdjacentStation("Vrutky"), 21));

    // Small Defects Generator
    CDefectGenerator smallDefects(CTimeInterval::TimeToMinutes(0,0, SMALL_DEFECT_FREQ),
      SMALL_DEFECT_DELAY);
    smallDefects.Activate(CTimeInterval::TimeToMinutes(0,0, SMALL_DEFECT_FREQ));

    // Medium Defects Generator
    CDefectGenerator mediumDefects(CTimeInterval::TimeToMinutes(0,0, MEDIUM_DEFECT_FREQ),
      MEDIUM_DEFECT_DELAY);
    mediumDefects.Activate(CTimeInterval::TimeToMinutes(0,0, MEDIUM_DEFECT_FREQ));

    // Large Defects Generator
    CDefectGenerator largeDefects(CTimeInterval::TimeToMinutes(0,0, LARGE_DEFECT_FREQ),
      LARGE_DEFECT_DELAY);
    largeDefects.Activate(CTimeInterval::TimeToMinutes(0,0, LARGE_DEFECT_FREQ));

    Run();
    DBG_LOG(mainStation.GetAdjacentStations().size());
    return 0;
}
