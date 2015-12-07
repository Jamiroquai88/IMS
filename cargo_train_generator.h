/*
 * cargo_train_generator.h
 *
 *  Created on: Dec 6, 2015
 *      Author: pavel
 */

#ifndef CARGO_TRAIN_GENERATOR_H_
#define CARGO_TRAIN_GENERATOR_H_

#include "train_generator.h"

class CCargoTrainGenerator : public CTrainGenerator
{
public:
    /**
    * \brief Constructor.
    */
    CCargoTrainGenerator(const std::string& trainTitle,
        CStation& startStation,
        CStation& targetStation,
        Frequency frequency,
        unsigned scheduleStartTime,
        unsigned scheduleTargetStationArrival,
        unsigned averageDelay = 0,
        bool bStopsInMainStation = false,
        unsigned scheduleMainStationArrival = 0,
        unsigned scheduleMainStationDeparture = 0);

    /**
    * \brief Destructor.
    */
    ~CCargoTrainGenerator();

    void Behavior() override;
};

#endif /* CARGO_TRAIN_GENERATOR_H_ */
