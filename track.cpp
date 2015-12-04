/*
 * track.cpp
 *
 *  Created on: Dec 4, 2015
 *      Author: pavel
 */

#include "track.h"
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
    m_PassingTrains.erase(&train);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CTrack::SetDefect(CDefect& defect)
{
  DBG_LOG_T("NEW DEFECT ");
    m_pDefect = &defect;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CTrack::ClearDefect()
{
  DBG_LOG_T("CLEAR DEFECT");
    m_pDefect = NULL;
}
