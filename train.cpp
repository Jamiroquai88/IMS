/*
 * train.cpp
 *
 *  Created on: Dec 6, 2015
 *      Author: pavel
 */

#include "train.h"

CTrain::CTrain(CTrainGenerator& generator,
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
    m_TraveledMinutes(0),
    m_pTrack(NULL),
    m_DirFromMainStation(0)
{
    // TODO Auto-generated constructor stub

}

CTrain::~CTrain()
{
    // TODO Auto-generated destructor stub
}

/**
 * \brief Travel on actual track. Set track duration, update trabeled minutes when interrupted.
 * \param duration Scheduled track duration (not including delays caused by defects)
 */
void CTrain::Travel(unsigned duration)
{
    unsigned time = (unsigned)Time;
    unsigned scheduledArrivalTime = time + duration;
    unsigned expectedArrivalTime = scheduledArrivalTime;
    unsigned timeToTarget = duration;

    // update track duration & location
    m_TrackDuration = duration;
    m_TraveledMinutes = 0;

    Wait(timeToTarget);
    time = (unsigned)Time;

    // interrupts
    while(time < expectedArrivalTime)
    {
        // interrupted
        timeToTarget = expectedArrivalTime - time;

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
            time = (unsigned)Time;
        }

        // defect fixed - wait for the rest of the journey
        expectedArrivalTime = time + timeToTarget;

        if(expectedArrivalTime - scheduledArrivalTime > 30)
        {
            DBG_LOG_T(m_Generator.GetTrainTitle()
                << "\t\tTravel delay: "
                << (expectedArrivalTime - scheduledArrivalTime)
                << "sched: " << scheduledArrivalTime
                << "exp:" << expectedArrivalTime
                << " [WARNING]");
        }

        Wait(timeToTarget);
        time = (unsigned)Time;
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
    if(m_Train.m_pTrack &&
       m_Train.m_pTrack->GetDefect(m_Train.GetDistanceFromMainStation(), m_Train.m_DirFromMainStation) == NULL &&
       m_Train.GetTrackDuration())
    {
        //DBG_LOG("PROGRESS REQ");
        m_Train.Activate();
    }
    Activate(Time + FREQUENCY);
}
