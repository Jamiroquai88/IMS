/*
 * CTrain.h
 *
 *  Created on: Nov 25, 2015
 *      Author: jose
 */

#ifndef CTRAIN_H_
#define CTRAIN_H_

#include "simlib.h"
#include "station.h"

class CTrainGenerator;

class CTrain : public Process
{
public:
    /**
     * \brief Constructor
     * \param generator Train generator containing train settings (schedule, exceptions..)
     */
    CTrain(const CTrainGenerator& generator,
        unsigned scheduledStartTime,
        unsigned scheduledTargetStationArrival,
        unsigned scheduledMainStationArrival = 0,
        unsigned scheduledMainStationDeparture = 0);

    virtual ~CTrain();

    void Behavior();

private:
    const CTrainGenerator& m_Generator;

    // own schedule times (computed by generator)

    // start time
    unsigned m_ScheduledStartTime;

    // stop at main station
    unsigned m_ScheduledMainStationArrival;
    unsigned m_ScheduledMainStationDeparture;

    // arrival at the target station
    unsigned m_ScheduledTargetStationArrival;
};

#endif /* CTRAIN_H_ */
