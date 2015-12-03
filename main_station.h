/*
 * main_station.h
 *
 *  Created on: Nov 29, 2015
 *      Author: pavel
 */

#ifndef MAIN_STATION_H_
#define MAIN_STATION_H_

#include "station.h"
#include <vector>

class CAdjacentStation;

/**
 * \brief Singleton. Main train station of the simulation.
 */
class CMainStation : public CStation
{
public:
    typedef std::vector<CAdjacentStation*> AdjacentStations;

	~CMainStation();

	/**
	 * \brief Get singleton instance.
	 */
	static CMainStation& GetInstance();

	void SetTitle(const std::string& title);

	const AdjacentStations& GetAdjacentStations() const;
    const CAdjacentStation& GetAdjacentStation(size_t id) const;
    CAdjacentStation& GetAdjacentStation(size_t id);

    size_t AddAdjacentStations(const std::string& title);

private:
	CMainStation();

	CMainStation(CMainStation const&)    = delete;
    void operator=(CMainStation const&)  = delete;

    // Members
    AdjacentStations m_AdjacentStations;
};

#endif /* MAIN_STATION_H_ */
