/*
 * main_station.h
 *
 *  Created on: Nov 29, 2015
 *      Author: pavel
 */

#ifndef MAIN_STATION_H_
#define MAIN_STATION_H_

#include "station.h"
#include "track.h"
#include <cassert>
#include <vector>
#include <map>

class CAdjacentStation;
class CTrack;

/**
 * \brief Singleton. Main train station of the simulation.
 */
class CMainStation : public CStation
{
public:
    // title => station
    typedef std::map<std::string, CAdjacentStation*> AdjacentStations;
    // all tracks segments lookup
    typedef std::map<const CAdjacentStation*, CTrack*> AllTracksMap;
    // root tracks (from main station to the last station)
    typedef std::vector<CTrack*> Tracks;


	~CMainStation();

	/**
	 * \brief Get singleton instance.
	 */
	static CMainStation& GetInstance();

	void SetTitle(const std::string& title);

	const AdjacentStations& GetAdjacentStations() const;
    const CAdjacentStation& GetAdjacentStation(const std::string& title) const;
    CAdjacentStation& GetAdjacentStation(const std::string& title);

    void AddAdjacentStation(const std::string& title);

    bool HasTrack(const CAdjacentStation& adjStation) const;

    const Tracks& GetTracks() const;
    const CTrack& GetTrack(const CAdjacentStation& adjStation) const;
    CTrack& GetTrack(const CAdjacentStation& adjStation);

    CTrack& AddCoreTrack(const CAdjacentStation& adjStation, unsigned length);
    void AddTrackSegment(CTrack& track);

    Histogram& GetDelayHistogram();
    Histogram& GetDefectsHistogram();

private:
	CMainStation();

	CMainStation(CMainStation const&)    = delete;
    void operator=(CMainStation const&)  = delete;

    // Members
    AdjacentStations m_AdjacentStations;
    // Root tracks
    Tracks m_Tracks;
    // Tracks segments lookup
    AllTracksMap m_TracksMap;

    // Histogram for delays
    Histogram m_Delayhistogram;
    // Histogram for defects
    Histogram m_DefectsHistogram;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool CMainStation::HasTrack(const CAdjacentStation& adjStation) const
{
    return m_TracksMap.find(&adjStation) != m_TracksMap.end();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const CMainStation::Tracks& CMainStation::GetTracks() const
{
    return m_Tracks;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const CTrack& CMainStation::GetTrack(const CAdjacentStation& adjStation) const
{
    assert(m_TracksMap.find(&adjStation) != m_TracksMap.end());
    return *m_TracksMap.find(&adjStation)->second;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline CTrack& CMainStation::GetTrack(const CAdjacentStation& adjStation)
{
    assert(m_TracksMap.find(&adjStation) != m_TracksMap.end());
    return *m_TracksMap[&adjStation];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Histogram& CMainStation::GetDelayHistogram()
{
  return m_Delayhistogram;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Histogram& CMainStation::GetDefectsHistogram()
{
  return m_DefectsHistogram;
}

#endif /* MAIN_STATION_H_ */
