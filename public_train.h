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
    CPublicTrain(CPublicTrainGenerator& generator,
        unsigned scheduledStartTime,
        unsigned scheduledTargetStationArrival,
        unsigned scheduledMainStationArrival = 0,
        unsigned scheduledMainStationDeparture = 0,
        std::string waitForTrain = "");

    virtual ~CPublicTrain();

    void Behavior();

    unsigned GetRealMainStationdeparture() const;

    const std::string& GetWaitForTrainName() const;

    static unsigned GetBoardingTime();

private:
    /// time to wait for passengers to get on the train [minutes]
    static const unsigned BOARDING_TIME;

    // name of the train, this train is waiting for
    std::string m_WaitingForTrain;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const std::string& CPublicTrain::GetWaitForTrainName() const
{
    return m_WaitingForTrain;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline unsigned CPublicTrain::GetBoardingTime()
{
    return BOARDING_TIME;
}

#endif /* CTRAIN_H_ */
