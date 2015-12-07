/*
 * adjacent_station.h
 *
 *  Created on: Nov 29, 2015
 *      Author: pavel
 */

#ifndef ADJACENT_STATION_H_
#define ADJACENT_STATION_H_

#include "station.h"
#include "simlib.h"
#include <vector>

/**
 * \brief   Class representing a station adjacent to the main station.
 *          No statistics are collected, only trains are generated.
 */
class CAdjacentStation : public CStation
{
public:

    CAdjacentStation(const std::string& title = "");

    virtual ~CAdjacentStation();

    /**
     * \brief   Train enters the station
     */
    virtual void Enter(CPublicTrain& train);
    virtual void Enter(CCargoTrain& train);

    /**
     * \brief   Train leaves the station
     */
    virtual void Leave(CPublicTrain& train);
    virtual void Leave(CCargoTrain& train);
private:
};

#endif /* ADJACENT_STATION_H_ */
