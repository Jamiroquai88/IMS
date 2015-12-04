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
#include <vector>
#include <map>

class CAdjacentStation;

/**
 * \brief Singleton. Main train station of the simulation.
 */
class CMainStation : public CStation
{
public:
    // title => station
    typedef std::map<std::string, CAdjacentStation*> AdjacentStations;
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
    void AddTrack(const std::string& adjStationTitle);
    const Tracks& GetTracks() const;

    CTrack& AddTrack(const CAdjacentStation& adjStation, unsigned length);

private:
	CMainStation();

	CMainStation(CMainStation const&)    = delete;
    void operator=(CMainStation const&)  = delete;

    // Members
    AdjacentStations m_AdjacentStations;
    Tracks m_Tracks;
};

inline const CMainStation::Tracks& CMainStation::GetTracks() const
{
    return m_Tracks;
}

#endif /* MAIN_STATION_H_ */
