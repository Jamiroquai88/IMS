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
{
    const CAdjacentStation& startStation =
            static_cast<const CAdjacentStation&>(generator.GetStartStation());

    if(CMainStation::GetInstance().HasTrack(startStation))
    {
        m_pTrack = &CMainStation::GetInstance()
            .GetTrack(startStation);
    }
    else
    {
        m_pTrack = &CMainStation::GetInstance()
            .GetTrack(static_cast<const CAdjacentStation&>(generator.GetTargetStation()));
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
CTrain::~CTrain()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CTrain::Behavior()
{
    // go to track
    m_pTrack->AddPassingTrain(*this);

    // check for defect
    if(m_pTrack->GetDefect() != NULL)
    {
        DBG_LOG_T("TRAIN AFFECTED BY DEFECT");
    }

    // travel to the main station
    DBG_LOG_T(m_Generator.GetTrainTitle() + ": Start in " + m_Generator.GetStartStation().GetTitle());

    // stops in the main station
    if( m_Generator.StopsInMainStation() )
    {
        unsigned timeToMainSt = m_ScheduledMainStationArrival - m_ScheduledStartTime;
        DBG_LOG_T(m_Generator.GetTrainTitle() + ": ride duration: " +
            CTimeInterval::MinutesToTime(timeToMainSt));

        DBG_LOG_T(m_Generator.GetTrainTitle() + ": Going to stop in the main station at: " +
            CTimeInterval::MinutesToTime(Time + timeToMainSt));

        Wait(timeToMainSt);
        // check for defect
        if(m_pTrack->GetDefect() != NULL)
        {
            DBG_LOG_T("TRAIN AFFECTED BY DEFECT");
        }

        // go off the track
        m_pTrack->RemovePassingTrain(*this);

        // in main station
        DBG_LOG(m_Generator.GetTrainTitle() + ": Entering the main station");
        if(Time > m_ScheduledMainStationArrival)
        {
            DBG_LOG(m_Generator.GetTrainTitle() +
                    ": delay " +
                    CTimeInterval::MinutesToTime(Time - m_ScheduledMainStationArrival));
        }

        Wait(m_ScheduledMainStationDeparture - m_ScheduledMainStationArrival);

        // go on the second track
        m_pTrack = &CMainStation::GetInstance()
            .GetTrack(static_cast<const CAdjacentStation&>(m_Generator.GetTargetStation()));
        m_pTrack->AddPassingTrain(*this);

        // leaving main station
        DBG_LOG(m_Generator.GetTrainTitle() + ": Leaving the main station");
    }

    // travel to the target station
    Wait(m_ScheduledTargetStationArrival - m_ScheduledMainStationDeparture);
    // check for defect
    if(m_pTrack->GetDefect() != NULL)
    {
        DBG_LOG_T("TRAIN AFFECTED BY DEFECT");
    }

    // in target station
    DBG_LOG_T(m_Generator.GetTrainTitle() + ": End in " + m_Generator.GetTargetStation().GetTitle());
    if(Time > m_ScheduledTargetStationArrival)
    {
        DBG_LOG(m_Generator.GetTrainTitle() +
                ": delay " +
                CTimeInterval::MinutesToTime(Time - m_ScheduledTargetStationArrival));
    }

    // leave the track
    m_pTrack->RemovePassingTrain(*this);
}

