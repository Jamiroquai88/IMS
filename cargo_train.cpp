/*
 * cargo_train.cpp
 *
 *  Created on: Dec 5, 2015
 *      Author: pavel
 */

#include "cargo_train.h"

CCargoTrain::CCargoTrain(const CTrainGenerator& generator,
        unsigned scheduledStartTime,
        unsigned scheduledTargetStationArrival,
        unsigned scheduledMainStationArrival,
        unsigned scheduledMainStationDeparture)
  : CTrain(generator, scheduledStartTime, scheduledTargetStationArrival, scheduledMainStationArrival,
          scheduledMainStationDeparture)
{}

CCargoTrain::~CCargoTrain()
{}

