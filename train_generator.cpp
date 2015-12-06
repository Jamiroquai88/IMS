/*
 * train_generator.cpp
 *
 *  Created on: Dec 6, 2015
 *      Author: pavel
 */

#include "train_generator.h"
#include "time_interval.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
CTrainGenerator::CTrainGenerator(const std::string& trainTitle,
    CStation& startStation,
    CStation& targetStation,
    CTrainGenerator::Frequency frequency,
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
CTrainGenerator::~CTrainGenerator()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CTrainGenerator::AddException(unsigned minutesStart, unsigned minutesEnd)
{
    m_ScheduleExceptions.push_back(new CTimeInterval(minutesStart, minutesEnd));
}

