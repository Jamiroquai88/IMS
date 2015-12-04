/*
 * main_station.cpp
 *
 *  Created on: Nov 29, 2015
 *      Author: pavel
 */

#include "main_station.h"
#include "adjacent_station.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
CMainStation::CMainStation()
{}

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
    return *m_AdjacentStations.find(title)->second;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
CAdjacentStation& CMainStation::GetAdjacentStation(const std::string& title)
{
    return *m_AdjacentStations[title];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CMainStation::AddAdjacentStation(const std::string& title)
{
    m_AdjacentStations[title] = new CAdjacentStation(title);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
CTrack& CMainStation::AddTrack(const CAdjacentStation& adjStation, unsigned length)
{
    CTrack* pTrack = new CTrack(adjStation, length);
    m_Tracks.push_back(pTrack);
    return *pTrack;
}
