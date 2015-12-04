/*
 * CTrain.cpp
 *
 *  Created on: Nov 25, 2015
 *      Author: jose
 */

#include "train.h"
#include "train_generator.h"
#include "main_station.h"
#include "time_interval.h"
#include "debug.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////////////
CTrain::CTrain(const CTrainGenerator& generator,
    unsigned scheduledStartTime,
    unsigned scheduledTargetStationArrival,
    unsigned scheduledMainStationArrival,
    unsigned scheduledMainStationDeparture)
  : m_Generator(generator),
    m_ScheduledStartTime(scheduledStartTime),
    m_ScheduledMainStationArrival(scheduledMainStationArrival),
    m_ScheduledMainStationDeparture(scheduledMainStationDeparture),
    m_ScheduledTargetStationArrival(scheduledTargetStationArrival)
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
CTrain::~CTrain()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CTrain::Behavior()
{
    // travel to the main station
    DBG_LOG_T(m_Generator.GetTrainTitle() + ": Start in " + m_Generator.GetStartStation().GetTitle());

    // stops in the main station
    if( m_ScheduledMainStationArrival )
    {
        unsigned timeToMainSt = m_ScheduledMainStationArrival - m_ScheduledStartTime;
        DBG_LOG_T(m_Generator.GetTrainTitle() + ": ride duration: " +
            CTimeInterval::MinutesToTime(timeToMainSt));

        DBG_LOG_T(m_Generator.GetTrainTitle() + ": Going to stop in the main station at: " +
            CTimeInterval::MinutesToTime(Time + timeToMainSt));

        Wait(timeToMainSt);

        // in main station
        DBG_LOG(m_Generator.GetTrainTitle() + ": Entering the main station");
        if(Time > m_ScheduledMainStationArrival)
        {
            DBG_LOG(m_Generator.GetTrainTitle() +
                    ": delay " +
                    CTimeInterval::MinutesToTime(Time - m_ScheduledMainStationArrival));
        }

        Wait(m_ScheduledMainStationDeparture - m_ScheduledMainStationArrival);

        // leaving main station
        DBG_LOG(m_Generator.GetTrainTitle() + ": Leaving the main station");
    }

    // travel to the target station
    Wait(m_ScheduledTargetStationArrival - m_ScheduledMainStationDeparture);

    // in target station
    DBG_LOG_T(m_Generator.GetTrainTitle() + ": End in " + m_Generator.GetTargetStation().GetTitle());
    if(Time > m_ScheduledTargetStationArrival)
    {
        DBG_LOG(m_Generator.GetTrainTitle() +
                ": delay " +
                CTimeInterval::MinutesToTime(Time - m_ScheduledTargetStationArrival));
    }

}

