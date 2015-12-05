/*
 * CTrain.cpp
 *
 *  Created on: Nov 25, 2015
 *      Author: jose
 */

#include "public_train.h"
#include "public_train_generator.h"
#include "main_station.h"
#include "time_interval.h"
#include "debug.h"
#include <iostream>

// update progress every 5 minutes
const unsigned CPublicTrain::CProgressUpdateEvent::FREQUENCY = 10;

////////////////////////////////////////////////////////////////////////////////////////////////////
CPublicTrain::CPublicTrain(const CPublicTrainGenerator& generator,
    unsigned scheduledStartTime,
    unsigned scheduledTargetStationArrival,
    unsigned scheduledMainStationArrival,
    unsigned scheduledMainStationDeparture)
  : m_Generator(generator),
    m_ScheduledStartTime(scheduledStartTime),
    m_ScheduledMainStationArrival(scheduledMainStationArrival),
    m_ScheduledMainStationDeparture(scheduledMainStationDeparture),
    m_ScheduledTargetStationArrival(scheduledTargetStationArrival),
    m_TrackDuration(0),
    m_TraveledMinutes(0)
{
    const CAdjacentStation& startStation =
            static_cast<const CAdjacentStation&>(generator.GetStartStation());

    // (Target <- ) Main Station <- Start Station
    if(CMainStation::GetInstance().HasTrack(startStation))
    {
        m_pTrack = &CMainStation::GetInstance()
            .GetTrack(startStation);
        m_DirFromMainStation = false;
    }
    // Main Station -> Target Station
    else
    {
        m_pTrack = &CMainStation::GetInstance()
            .GetTrack(static_cast<const CAdjacentStation&>(generator.GetTargetStation()));
        m_DirFromMainStation = true;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
CPublicTrain::~CPublicTrain()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CPublicTrain::Behavior()
{
    // create update progress event
    CProgressUpdateEvent& progressUpdateEvent = *(new CProgressUpdateEvent(*this));
    progressUpdateEvent.Activate(Time + CProgressUpdateEvent::FREQUENCY);

    unsigned realStartTime = Time;
    unsigned initialDelay = realStartTime - m_ScheduledStartTime;

    DBG_LOG_T(m_Generator.GetTrainTitle() + ":\t\tGenerated in " + m_Generator.GetStartStation().GetTitle());

    // go to track
    m_pTrack->AddPassingTrain(*this);

    // start
    DBG_LOG_T(m_Generator.GetTrainTitle()
        << ":\t\tStart in " + m_Generator.GetStartStation().GetTitle()
        << " with initial delay: " << initialDelay << "minutes");

    // check for defect
    CDefect* pDefect = m_pTrack->GetDefect(0, m_DirFromMainStation);
    if(pDefect != NULL)
    {
        unsigned defectId = pDefect->GetId();
        DBG_LOG_T(m_Generator.GetTrainTitle()
            << ":\t\tCan't start because of a defect (id: " << defectId << ")");

        Passivate();
        // defect must be fixed now
        assert(m_pTrack->GetDefect(GetDistanceFromMainStation(), m_DirFromMainStation) == NULL);

        DBG_LOG_T(m_Generator.GetTrainTitle()
            << ":\t\tStarting after a defect removal (id: " << defectId << ")");
    }

    // time to target station
    unsigned timeToTargetStation = m_ScheduledTargetStationArrival - m_ScheduledStartTime;

    // stops in the main station
    if( m_Generator.StopsInMainStation() )
    {
        unsigned timeToMainSt = m_ScheduledMainStationArrival - m_ScheduledStartTime;

        DBG_LOG_T(m_Generator.GetTrainTitle() + ":\t\tMain station arrival scheduled at: " +
            CTimeInterval::MinutesToTime(Time + timeToMainSt));

        // travel to the main station
        Travel(timeToMainSt);

        // go off the track
        m_pTrack->RemovePassingTrain(*this);

        // in main station
        DBG_LOG_T(m_Generator.GetTrainTitle()
            << ":\t\tEntering the main station with delay: "
            << ((unsigned)Time - m_ScheduledMainStationArrival) << " minutes");

        if((unsigned)Time > m_ScheduledMainStationArrival)
        {
            DBG_LOG(m_Generator.GetTrainTitle()
                << ":\t\tadditional delay: "
                << ((unsigned)Time - m_ScheduledMainStationArrival) << " minutes");
        }

        // wait at the main station
        Wait(m_ScheduledMainStationDeparture - m_ScheduledMainStationArrival);

        // going from the main station
        m_DirFromMainStation = !m_DirFromMainStation;

        // go on the second track
        m_pTrack = &CMainStation::GetInstance()
            .GetTrack(static_cast<const CAdjacentStation&>(m_Generator.GetTargetStation()));
        m_pTrack->AddPassingTrain(*this);

        // leaving main station
        DBG_LOG_T(m_Generator.GetTrainTitle() + ":\t\tLeaving the main station");

        // check for defect
        CDefect* pDefect = m_pTrack->GetDefect(0, m_DirFromMainStation);
        if(pDefect != NULL)
        {
            unsigned defectId = pDefect->GetId();
            DBG_LOG_T(m_Generator.GetTrainTitle()
                << ":\t\tCan't leave main station because of a defect (id: " << defectId << ")");

            Passivate();
            // defect must be fixed now
            assert(m_pTrack->GetDefect(GetDistanceFromMainStation(), m_DirFromMainStation) == NULL);

            DBG_LOG_T(m_Generator.GetTrainTitle()
                << ":\t\tLeaving station after a defect removal (id: " << defectId << ")");
        }

        // update time to target station
        timeToTargetStation = m_ScheduledTargetStationArrival - m_ScheduledMainStationDeparture;
    }

    // travel to the target station
    Travel(timeToTargetStation);

    // leave the track
    m_pTrack->RemovePassingTrain(*this);

    // in target station
    DBG_LOG_T(m_Generator.GetTrainTitle() + ":\t\tEnd in " + m_Generator.GetTargetStation().GetTitle());
    if(Time > m_ScheduledTargetStationArrival)
    {
    	unsigned delayTime = Time - m_ScheduledTargetStationArrival;
        DBG_LOG(m_Generator.GetTrainTitle()
            << ":\t\tTotal delay: "
            << delayTime << " minutes");

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
void CPublicTrain::Travel(unsigned duration)
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
        CDefect* pDefect = m_pTrack->GetDefect(GetDistanceFromMainStation(), m_DirFromMainStation);
        if(pDefect != NULL)
        {
            DBG_LOG_T(m_Generator.GetTrainTitle()
                << ":\t\tStopping because of defect (id: "
                << pDefect->GetId() << ")");
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
CPublicTrain::CProgressUpdateEvent::CProgressUpdateEvent(CPublicTrain& train)
  : m_Train(train)
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CPublicTrain::CProgressUpdateEvent::Behavior()
{
    // only ask for update when the train is traveling without any active defect
    if(m_Train.m_pTrack &&
       m_Train.m_pTrack->GetDefect(m_Train.GetDistanceFromMainStation(), m_Train.m_DirFromMainStation) == NULL &&
       m_Train.GetTrackDuration())
    {
        //DBG_LOG("PROGRESS REQ");
        m_Train.Activate();
    }
    Activate(Time + FREQUENCY);
}
