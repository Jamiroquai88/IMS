/*
 * train_generator.cpp
 *
 *  Created on: Nov 29, 2015
 *      Author: pavel
 */

#include "train_generator.h"
#include "train.h"
#include "time_interval.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      CTrainGenerator                                           //
////////////////////////////////////////////////////////////////////////////////////////////////////
CTrainGenerator::CTrainGenerator(const std::string& trainTitle,
    CStation& startStation,
    CStation& targetStation,
    CTrainGenerator::Frequency frequency,
    unsigned targetStationArrival,
    unsigned averageDelay,
    bool bStopsInMainStation,
    unsigned mainStationArrival,
    unsigned mainStationDeparture)
  : m_TrainTitle(trainTitle),
    m_StartStation(startStation),
    m_TargetStation(targetStation),
    m_Frequency(frequency),
    m_TargetStationArrival(targetStationArrival),
    m_AverageDelay(averageDelay),
    m_StopsInMainStation(bStopsInMainStation),
    m_MainStationArrival(mainStationArrival),
    m_MainStationDeparture(mainStationDeparture)
{
std::cout << "TITLEEEEEEEEEEEEEEEEEEEE" << m_TrainTitle << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
CTrainGenerator::~CTrainGenerator()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CTrainGenerator::AddException(unsigned minutesStart, unsigned minutesEnd)
{
    m_ScheduleExceptions.push_back(new CTimeInterval(minutesStart, minutesEnd));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CTrainGenerator::Behavior()
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
        unsigned targetStationArrival = Time + m_TargetStationArrival;
        unsigned mainStationArrival = m_MainStationArrival ? Time + m_MainStationArrival : 0;
        unsigned mainStationDeparture = m_MainStationDeparture ? Time + m_MainStationDeparture : 0;

        // Generate train with delay
        (new CTrain(*this, Time, targetStationArrival, mainStationArrival,
                mainStationDeparture))->Activate(Time + Exponential(m_AverageDelay));
    }

    // Repeat in future
    if(m_Frequency != FREQ_ONCE)
    {
        Activate(Time + m_Frequency);
    }
}
