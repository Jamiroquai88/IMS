/*
 * train_generator.h
 *
 *  Created on: Nov 29, 2015
 *      Author: pavel
 */

#ifndef PUBLIC_TRAIN_GENERATOR_H_
#define PUBLIC_TRAIN_GENERATOR_H_

#include "train_generator.h"

/**
 * \brief   Train generator - generating trains according to the schedule.
 */
class CPublicTrainGenerator : public CTrainGenerator
{
public:

    /**
     * \brief Constructor.
     */
    CPublicTrainGenerator(const std::string& trainTitle,
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
    ~CPublicTrainGenerator();

    void Behavior() override;
private:
};


#endif /* PUBLIC_TRAIN_GENERATOR_H_ */
