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

// boarding time
const unsigned CPublicTrain::BOARDING_TIME = 5;
// update progress every 5 minutes
const unsigned CPublicTrain::CProgressUpdateEvent::FREQUENCY = 60;

////////////////////////////////////////////////////////////////////////////////////////////////////
CPublicTrain::CPublicTrain(CPublicTrainGenerator& generator,
    unsigned scheduledStartTime,
    unsigned scheduledTargetStationArrival,
    unsigned scheduledMainStationArrival,
    unsigned scheduledMainStationDeparture)
  : CTrain(generator, scheduledStartTime, scheduledTargetStationArrival, scheduledMainStationArrival,
          scheduledMainStationDeparture)
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

    // go to the platform
    m_Generator.GetStartStation().Enter(*this);

    // wait for passengers to board
    Wait(CPublicTrain::GetBoardingTime());

    // lock the track start
    m_Generator.GetStartStation().SeizeTrack(*this, m_pTrack, true);
    Wait(STATION_LEAVING_TIME);
    // leave the platform
    m_Generator.GetStartStation().Leave(*this);

    unsigned realStartTime = Time;
    unsigned initialDelay = realStartTime - m_ScheduledStartTime;

    DBG_LOG_T(m_Generator.GetTrainTitle() + ":\t\tGenerated in " + m_Generator.GetStartStation().GetTitle());

    // go to track
    m_pTrack->AddPassingTrain(*this);

    // unlock the track start
    m_Generator.GetStartStation().ReleaseTrack(*this, m_pTrack, true);


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

        // lock track end
        CMainStation::GetInstance().SeizeTrack(*this, m_pTrack, false);
        Wait(STATION_ENTERING_TIME);
        // enter main station
        CMainStation::GetInstance().Enter(*this);
        // unlock track end
        CMainStation::GetInstance().ReleaseTrack(*this, m_pTrack, false);

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

        // lock track start
        CMainStation::GetInstance().SeizeTrack(*this, m_pTrack, true);
        Wait(STATION_LEAVING_TIME);
        // leave the station
        CMainStation::GetInstance().Leave(*this);
        // unlock track start
        CMainStation::GetInstance().ReleaseTrack(*this, m_pTrack, true);

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

    // lock track end
    m_Generator.GetTargetStation().SeizeTrack(*this, m_pTrack, false);
    Wait(STATION_ENTERING_TIME);
    // enter the station
    m_Generator.GetTargetStation().Enter(*this);
    // unlock track end
    m_Generator.GetTargetStation().ReleaseTrack(*this, m_pTrack, false);


    // in target station
    DBG_LOG_T(m_Generator.GetTrainTitle() + ":\t\tEnd in " + m_Generator.GetTargetStation().GetTitle());
    unsigned absDelay = Time - m_ScheduledTargetStationArrival - STATION_ENTERING_TIME;
    unsigned relDelay = absDelay - initialDelay;
    if(absDelay)
    {
        DBG_LOG(m_Generator.GetTrainTitle()
            << ":\t\tTotal delay: "
            << absDelay << " minutes");

        CMainStation::GetInstance().GetDelayHistogram()(relDelay);

        DELAY_LOG(relDelay, absDelay);
    }

    // release platform
    m_Generator.GetTargetStation().Leave(*this);

    // delete event
    progressUpdateEvent.Cancel();
}
