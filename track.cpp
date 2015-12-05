/*
 * track.cpp
 *
 *  Created on: Dec 4, 2015
 *      Author: pavel
 */

#include "track.h"
#include "train.h"
#include "main_station.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
CTrack::CTrack(const CAdjacentStation& adjacentStation, unsigned length)
  : m_AdjacentStation(adjacentStation),
    m_Length(length),
    m_pParentTrack(NULL),
    m_pNestedSegment(NULL),
    m_pDefect(NULL)
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
CTrack::~CTrack()
{
    delete m_pNestedSegment;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
CDefect* CTrack::GetDefect()
{
    if(m_pDefect)
        return m_pDefect;
    else if(m_pNestedSegment)
        return m_pNestedSegment->GetDefect();
    else
        return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CTrack::GetPassingTrains(unsigned location, bool bFromMainStation, Trains& trains) const
{
    trains.clear();
    for(Trains::const_iterator itTrain = m_PassingTrains.begin();
        itTrain != m_PassingTrains.end();
        ++itTrain)
    {
        DBG_LOG("pass");
        // train approximate location
        double progressPercent = (*itTrain)->GetTraveledMinutes() / (double)(*itTrain)->GetTrackDuration();

        DBG_LOG("pass");

        DBG_LOG_T("Train progress " << (*itTrain)->GetGenerator().GetTrainTitle()
            << " Start - " << CTimeInterval::MinutesToTime((*itTrain)->GetRealStartTime())
            << " progress minutes: "
            << (*itTrain)->GetTraveledMinutes()
            << " - " << (progressPercent*100) << "%");

        // opposite direction - invert
        if(!bFromMainStation)
        {
            progressPercent = 1 - progressPercent;
        }

        //unsigned location = m_Length * progressPercent;

        trains.insert(*itTrain);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
CTrack& CTrack::AddNestedSegment(CTrack* pNestedSegment)
{
    m_pNestedSegment = pNestedSegment;
    m_pNestedSegment->m_pParentTrack = this;
    // register the nested segment in the main station (to the lookup table)
    CMainStation::GetInstance().AddTrackSegment(*pNestedSegment);

    return *pNestedSegment;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CTrack::AddPassingTrain(CTrain& train)
{
    m_PassingTrains.insert(&train);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CTrack::RemovePassingTrain(CTrain& train)
{
    DBG_LOG_T("Remove passing train " << train.GetGenerator().GetTrainTitle()
        << " Start - " << CTimeInterval::MinutesToTime(train.GetRealStartTime())
        << " progress minutes: "
        << train.GetTraveledMinutes());
    m_PassingTrains.erase(&train);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CTrack::SetDefect(CDefect& defect, unsigned location)
{
    // own defect
    if(!m_pNestedSegment || location > m_pNestedSegment->GetLength())
    {
        DBG_LOG_T("NEW DEFECT ");
        m_pDefect = &defect;
    }
    // place the defect in the proper segment
    else
    {
        m_pNestedSegment->SetDefect(defect, location);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CTrack::ClearDefect()
{
    DBG_LOG_T("CLEAR DEFECT");
    m_pDefect = NULL;
}
