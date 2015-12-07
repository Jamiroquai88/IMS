/*
 * cargo_train_generator.cpp
 *
 *  Created on: Dec 6, 2015
 *      Author: pavel
 */

#include "cargo_train_generator.h"
#include "cargo_train.h"
#include "time_interval.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
CCargoTrainGenerator::CCargoTrainGenerator(const std::string& trainTitle,
    CStation& startStation,
    CStation& targetStation,
    CCargoTrainGenerator::Frequency frequency,
    unsigned scheduleStartTime,
    unsigned targetStationArrival,
    unsigned averageDelay,
    bool bStopsInMainStation,
    unsigned mainStationArrival,
    unsigned mainStationDeparture)
  : CTrainGenerator(trainTitle, startStation, targetStation, frequency, scheduleStartTime,
          targetStationArrival, averageDelay, bStopsInMainStation, mainStationArrival,
          mainStationDeparture)
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
CCargoTrainGenerator::~CCargoTrainGenerator()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CCargoTrainGenerator::Behavior()
{
    // Check for exceptions
    bool bExceptionalTime = false;
    for(ScheduleExceptions::const_iterator it = m_ScheduleExceptions.begin();
        it != m_ScheduleExceptions.end();
        ++it)
    {
        if((*it)->IsInInterval(Time))
        {
            bExceptionalTime = true;
            break;
        }
    }

    // Generate train
    if(!bExceptionalTime)
    {
        // Compute concrete arrival and departure times
        unsigned targetStationArrival = Time + m_ScheduleTargetStationArrival - m_ScheduleStartTime;
        unsigned mainStationArrival = Time + m_ScheduleMainStationArrival - m_ScheduleStartTime;
        unsigned mainStationDeparture = Time + m_ScheduleMainStationDeparture - m_ScheduleStartTime;

        // Generate train with delay
        (new CCargoTrain(*this, Time, targetStationArrival,
            mainStationArrival, mainStationDeparture))->Activate(Time + Exponential(m_AverageDelay));
    }

    // Repeat in future
    if(m_Frequency != FREQ_ONCE)
    {
        Activate(Time + m_Frequency);
    }
}
