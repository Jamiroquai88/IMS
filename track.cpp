/*
 * track.cpp
 *
 *  Created on: Dec 4, 2015
 *      Author: pavel
 */

#include "track.h"
#include "public_train.h"
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
CTrack* CTrack::GetTopParentTrack()
{
    if(m_pParentTrack)
        return m_pParentTrack->GetTopParentTrack();
    else
        return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
CDefect* CTrack::GetDefect(unsigned location, bool dirFromMainStation) const
{
    CDefect* pDefect = NULL;
    // MAIN STATION -> location -> TARGET STATION
    if(dirFromMainStation)
    {
        if(!m_pDefect)
        {
            if(m_pNestedSegment)
                pDefect = m_pNestedSegment->GetDefect(location, dirFromMainStation);
        }
        else if(m_pDefect->GetLocation() >= location)
            pDefect = m_pDefect;
    }
    // MAIN STATION <- location <- TARGET STATION
    else
    {
        if(!m_pDefect)
        {
            if(m_pParentTrack)
                pDefect = m_pParentTrack->GetDefect(location, dirFromMainStation);
        }
        else if(m_pDefect->GetLocation() <= location)
        {
            pDefect = m_pDefect;
        }
    }
    return pDefect;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CTrack::GetComingTrains(unsigned location, bool bFromMainStation, Trains& trains) const
{
    // location can reside within the nested segment
    if(m_pNestedSegment)
    {
        m_pNestedSegment->GetComingTrains(location, bFromMainStation, trains);
    }

    // add own trains
    for(Trains::const_iterator itTrain = m_PassingTrains.begin();
        itTrain != m_PassingTrains.end();
        ++itTrain)
    {
        // train approximate location
        unsigned trainLocation = (*itTrain)->GetDistanceFromMainStation();

        if( (bFromMainStation && trainLocation < location) ||
            (!bFromMainStation && trainLocation > location) )
        {
            trains.insert(*itTrain);
        }
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
void CTrack::AddPassingTrain(CPublicTrain& train)
{
    m_PassingTrains.insert(&train);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CTrack::RemovePassingTrain(CPublicTrain& train)
{
    m_PassingTrains.erase(&train);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CTrack::SetDefect(CDefect& defect, unsigned location)
{
    // own defect
    if(!m_pNestedSegment || location > m_pNestedSegment->GetLength())
    {
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
    if(m_pNestedSegment)
        m_pNestedSegment->ClearDefect();
}
