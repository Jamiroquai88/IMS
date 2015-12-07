/*
 * main_station.cpp
 *
 *  Created on: Nov 29, 2015
 *      Author: pavel
 */

#include "main_station.h"
#include "adjacent_station.h"
#include "public_train.h"
#include "cargo_train.h"
#include "debug.h"
#include "assert.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
CMainStation::CMainStation()
	: m_Delayhistogram("Delays durations in system", 0, 5, 30),
	  m_DefectsHistogram("Defects in system", 0, 5, 30),
	  m_PublicRailsStore("Station Transport Rails", 1),
	  m_CargoRailsStore("Station Cargo Rails", 1)
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
void CMainStation::SetTransportRailsNumber(unsigned count)
{
    m_PublicRailsStore.SetCapacity(count);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CMainStation::SetCargoRailsNumber(unsigned count)
{
    m_CargoRailsStore.SetCapacity(count);
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
CAdjacentStation* CMainStation::AddAdjacentStation(const std::string& title)
{
    CAdjacentStation* pStation = new CAdjacentStation(title);
    m_AdjacentStations[title] = pStation;
    return pStation;
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

////////////////////////////////////////////////////////////////////////////////////////////////////
void CMainStation::Enter(CPublicTrain& train)
{
    m_PublicRailsStore.Enter(dynamic_cast<Entity*>(&train), 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CMainStation::Enter(CCargoTrain& train)
{
    DBG_LOG_T(train.GetGenerator().GetTrainTitle() << "\t\tCargo entering the main station");
    m_CargoRailsStore.Enter(dynamic_cast<Entity*>(&train), 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CMainStation::Leave(CPublicTrain& train)
{
    m_PublicRailsStore.Leave(1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CMainStation::Leave(CCargoTrain& train)
{
    DBG_LOG_T(train.GetGenerator().GetTrainTitle() << "\t\tCargo leaving the main station");
    m_CargoRailsStore.Leave(1);
}
