/*
 * train_generator.cpp
 *
 *  Created on: Nov 29, 2015
 *      Author: pavel
 */

#include "public_train_generator.h"
#include "public_train.h"
#include "time_interval.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      CTrainGenerator                                           //
////////////////////////////////////////////////////////////////////////////////////////////////////
CPublicTrainGenerator::CPublicTrainGenerator(const std::string& trainTitle,
    CStation& startStation,
    CStation& targetStation,
    CPublicTrainGenerator::Frequency frequency,
    unsigned scheduleStartTime,
    unsigned targetStationArrival,
    unsigned averageDelay,
    bool bStopsInMainStation,
    unsigned mainStationArrival,
    unsigned mainStationDeparture)
  : m_TrainTitle(trainTitle),
    m_StartStation(startStation),
    m_TargetStation(targetStation),
    m_Frequency(frequency),
    m_ScheduleStartTime(scheduleStartTime),
    m_ScheduleTargetStationArrival(targetStationArrival),
    m_AverageDelay(averageDelay),
    m_StopsInMainStation(bStopsInMainStation),
    m_ScheduleMainStationArrival(mainStationArrival),
    m_ScheduleMainStationDeparture(mainStationDeparture)
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
CPublicTrainGenerator::~CPublicTrainGenerator()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CPublicTrainGenerator::AddException(unsigned minutesStart, unsigned minutesEnd)
{
    m_ScheduleExceptions.push_back(new CTimeInterval(minutesStart, minutesEnd));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CPublicTrainGenerator::Behavior()
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
        unsigned targetStationArrival = Time + m_ScheduleTargetStationArrival - m_ScheduleStartTime + CPublicTrain::GetBoardingTime();
        unsigned mainStationArrival = Time + m_ScheduleMainStationArrival - m_ScheduleStartTime + CPublicTrain::GetBoardingTime();
        unsigned mainStationDeparture = Time + m_ScheduleMainStationDeparture - m_ScheduleStartTime + CPublicTrain::GetBoardingTime();

        /*
        DBG_LOG_T(GetTrainTitle() << " targetStationArrival: " << CTimeInterval::MinutesToTime(targetStationArrival));
        DBG_LOG_T(GetTrainTitle() << " mainStationArrival: " << CTimeInterval::MinutesToTime(mainStationArrival));
        DBG_LOG_T(GetTrainTitle() << " mainStationDeparture: " << CTimeInterval::MinutesToTime(mainStationDeparture));
        */

        // Generate train with delay
        (new CPublicTrain(*this, Time + CPublicTrain::GetBoardingTime(), targetStationArrival,
            mainStationArrival, mainStationDeparture))->Activate(Time + Exponential(m_AverageDelay));
    }

    // Repeat in future
    if(m_Frequency != FREQ_ONCE)
    {
        Activate(Time + m_Frequency);
    }
}
