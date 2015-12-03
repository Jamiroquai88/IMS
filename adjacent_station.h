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

    // Inherit constructor
    using CStation::CStation;

    ~CAdjacentStation();
private:
};

#endif /* ADJACENT_STATION_H_ */
