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
    delete m_pParentTrack;
    delete m_pNestedSegment;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
CTrack& CTrack::AddNestedSegment(CTrack* pNestedSegment)
{
    m_pNestedSegment = pNestedSegment;
    m_pNestedSegment->m_pParentTrack = this;
    // register the nested segment in the main station (to the lookup table)
    CMainStation::GetInstance().AddTrack(pNestedSegment->m_AdjacentStation,
        pNestedSegment->GetLength());

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
    m_pDefect = &defect;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CTrack::ClearDefect()
{
    m_pDefect = NULL;
}
