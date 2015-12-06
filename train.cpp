/*
 * train.cpp
 *
 *  Created on: Dec 6, 2015
 *      Author: pavel
 */

#include "train.h"

CTrain::CTrain(const CTrainGenerator& generator)
  : m_Generator(generator),
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
