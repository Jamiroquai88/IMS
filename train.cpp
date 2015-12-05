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

// update progress every 5 minutes
const unsigned CTrain::CProgressUpdateEvent::FREQUENCY = 10;

////////////////////////////////////////////////////////////////////////////////////////////////////
CTrain::CTrain(const CTrainGenerator& generator,
    unsigned scheduledStartTime,
    unsigned scheduledTargetStationArrival,
    unsigned scheduledMainStationArrival,
    unsigned scheduledMainStationDeparture)
  : m_Generator(generator),
    m_ScheduledStartTime(scheduledStartTime),
    m_RealStartTime(0),
    m_ScheduledMainStationArrival(scheduledMainStationArrival),
    m_ScheduledMainStationDeparture(scheduledMainStationDeparture),
    m_RealMainStationdeparture(0),
    m_ScheduledTargetStationArrival(scheduledTargetStationArrival),
    m_TrackDuration(0),
    m_TraveledMinutes(0)
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
    // create update progress event
    CProgressUpdateEvent& progressUpdateEvent = *(new CProgressUpdateEvent(*this));
    progressUpdateEvent.Activate(Time + CProgressUpdateEvent::FREQUENCY);

    m_RealStartTime = Time;
    DBG_LOG_T(m_Generator.GetTrainTitle() + ": Generated in " + m_Generator.GetStartStation().GetTitle());

    // go to track
    m_pTrack->AddPassingTrain(*this);

    // check for defect
    if(m_pTrack->GetDefect() != NULL)
    {
        DBG_LOG_T("S TRAIN AFFECTED BY DEFECT");
        Passivate();
        assert(m_pTrack->GetDefect() == NULL);
    }

    // start
    DBG_LOG_T(m_Generator.GetTrainTitle() + ": Start in " + m_Generator.GetStartStation().GetTitle());

    // time to target station
    unsigned timeToTargetStation = m_ScheduledTargetStationArrival - m_ScheduledStartTime;

    // stops in the main station
    if( m_Generator.StopsInMainStation() )
    {
        unsigned timeToMainSt = m_ScheduledMainStationArrival - m_ScheduledStartTime;
        DBG_LOG_T(m_Generator.GetTrainTitle() + ": ride start: "
                  << CTimeInterval::MinutesToTime(m_ScheduledStartTime)
                  << m_Generator.GetTrainTitle() + ": ride main station arrival: "
                  << CTimeInterval::MinutesToTime(m_ScheduledMainStationArrival));

        DBG_LOG_T(m_Generator.GetTrainTitle() + ": ride duration: " +
            CTimeInterval::MinutesToTime(timeToMainSt));

        DBG_LOG_T(m_Generator.GetTrainTitle() + ": Going to stop in the main station at: " +
            CTimeInterval::MinutesToTime(Time + timeToMainSt));

        // travel to the main station
        Travel(timeToMainSt);

        // go off the track
        m_pTrack->RemovePassingTrain(*this);

        // in main station
        DBG_LOG_T(m_Generator.GetTrainTitle() + ": Entering the main station");
        if(Time > m_ScheduledMainStationArrival)
        {
            DBG_LOG(m_Generator.GetTrainTitle() +
                    ": delay " +
                    CTimeInterval::MinutesToTime(Time - m_ScheduledMainStationArrival));
        }

        Wait(m_ScheduledMainStationDeparture - m_ScheduledMainStationArrival);

        m_RealMainStationdeparture = Time;

        // go on the second track
        m_pTrack = &CMainStation::GetInstance()
            .GetTrack(static_cast<const CAdjacentStation&>(m_Generator.GetTargetStation()));
        m_pTrack->AddPassingTrain(*this);

        // leaving main station
        DBG_LOG_T(m_Generator.GetTrainTitle() + ": Leaving the main station");

        // update time to target station
        timeToTargetStation = m_ScheduledTargetStationArrival - m_ScheduledMainStationDeparture;
    }

    // travel to the target station
    Travel(timeToTargetStation);

    // leave the track
    m_pTrack->RemovePassingTrain(*this);

    // in target station
    DBG_LOG_T(m_Generator.GetTrainTitle() + ": End in " + m_Generator.GetTargetStation().GetTitle());
    if(Time > m_ScheduledTargetStationArrival)
    {
    	double delayTime = Time - m_ScheduledTargetStationArrival;
        DBG_LOG(m_Generator.GetTrainTitle() +
                ": delay " + CTimeInterval::MinutesToTime(delayTime));
        CMainStation::GetInstance().GetDelayHistogram()
        		(delayTime);
    }

    // delete event
    progressUpdateEvent.Cancel();
}

/**
 * \brief Travel on actual track. Set track duration, update trabeled minutes when interrupted.
 * \param duration Scheduled track duration (not including delays caused by defects)
 */
void CTrain::Travel(unsigned duration)
{
    unsigned expectedArrivalTime = Time + duration;
    unsigned timeToTarget = duration;

    // update track duration & location
    m_TrackDuration = duration;
    m_TraveledMinutes = 0;

    Wait(timeToTarget);

    // interrupts
    while(Time < expectedArrivalTime)
    {
        // interrupted
        timeToTarget = expectedArrivalTime - Time;

        // update progress
        m_TraveledMinutes = duration - timeToTarget;

        //DBG_LOG_T(m_Generator.GetTrainTitle() << " Progress: " << m_TraveledMinutes << " / " << duration);

        // wait for activation (e.g. defect fix)
        if(m_pTrack->GetDefect() != NULL)
        {
            DBG_LOG_T("TRAIN AFFECTED BY DEFECT");
            Passivate();
        }

        // defect fixed - wait for the rest of the journey
        expectedArrivalTime = Time + timeToTarget;

        Wait(timeToTarget);
    }

    // update track duration & location
    m_TrackDuration = 0;
    m_TraveledMinutes = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
CTrain::CProgressUpdateEvent::CProgressUpdateEvent(CTrain& train)
  : m_Train(train)
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CTrain::CProgressUpdateEvent::Behavior()
{
    // only ask for update when the train is traveling without any active defect
    if(m_Train.m_pTrack->GetDefect() == NULL && m_Train.GetTrackDuration())
    {
        //DBG_LOG("PROGRESS REQ");
        m_Train.Activate();
    }
    Activate(Time + FREQUENCY);
}
