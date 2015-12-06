/*
 * CTrain.h
 *
 *  Created on: Nov 25, 2015
 *      Author: jose
 */

#ifndef CTRAIN_H_
#define CTRAIN_H_

#include "train.h"
#include "station.h"
#include "track.h"

class CPublicTrainGenerator;

class CPublicTrain : public CTrain
{
public:
    /**
     * \brief Constructor
     * \param generator Train generator containing train settings (schedule, exceptions..)
     */
    CPublicTrain(const CPublicTrainGenerator& generator,
        unsigned scheduledStartTime,
        unsigned scheduledTargetStationArrival,
        unsigned scheduledMainStationArrival = 0,
        unsigned scheduledMainStationDeparture = 0);

    virtual ~CPublicTrain();

    void Behavior();

    unsigned GetRealMainStationdeparture() const;

    static unsigned GetBoardingTime();

private:
    /// time to wait for passengers to get on the train [minutes]
    static const unsigned BOARDING_TIME;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
inline unsigned CPublicTrain::GetBoardingTime()
{
    return BOARDING_TIME;
}

#endif /* CTRAIN_H_ */
