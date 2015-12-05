/*
 * main_station.cpp
 *
 *  Created on: Nov 29, 2015
 *      Author: pavel
 */

#include "main_station.h"
#include "adjacent_station.h"
#include "debug.h"
#include "assert.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
CMainStation::CMainStation()
	: m_Delayhistogram("Delays durations in system", 0, 5, 30),
	  m_DefectsHistogram("Defects in system", 0, 5, 30),
	  m_RailsStore("Station Rails", 7)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////
CMainStation::~CMainStation()
{
    // Delete adjacent stations
	for(AdjacentStations::iterator it = m_AdjacentStations.begin();
        it != m_AdjacentStations.end();
        ++it)
	{
	    delete it->second;
	}

	// Delete tracks
	for(Tracks::iterator it = m_Tracks.begin();
        it != m_Tracks.end();
        ++it)
	{
	    delete *it;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
CMainStation& CMainStation::GetInstance()
{
    static CMainStation mainStation;
    return mainStation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CMainStation::SetTitle(const std::string& title)
{
    m_Title = title;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
const CMainStation::AdjacentStations& CMainStation::GetAdjacentStations() const
{
    return m_AdjacentStations;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
const CAdjacentStation& CMainStation::GetAdjacentStation(const std::string& title) const
{
    assert(m_AdjacentStations.find(title) != m_AdjacentStations.end());
    return *m_AdjacentStations.find(title)->second;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
CAdjacentStation& CMainStation::GetAdjacentStation(const std::string& title)
{
    assert(m_AdjacentStations.find(title) != m_AdjacentStations.end());
    return *m_AdjacentStations[title];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CMainStation::AddAdjacentStation(const std::string& title)
{
    m_AdjacentStations[title] = new CAdjacentStation(title);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
CTrack& CMainStation::AddCoreTrack(const CAdjacentStation& adjStation, unsigned length)
{
    CTrack* pTrack = new CTrack(adjStation, length);
    m_Tracks.push_back(pTrack);
    AddTrackSegment(*pTrack);
    return *pTrack;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CMainStation::AddTrackSegment(CTrack& track)
{
    m_TracksMap[&track.GetAdjacentStation()] = &track;
}
