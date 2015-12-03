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
        unsigned targetStationArrival,
        unsigned mainStationArrival = 0,
        unsigned mainStationDeparture = 0);

    virtual ~CTrain();

    void Behavior();

private:
    const CTrainGenerator& m_Generator;

    // own schedule times (computed by generator)

    // stop at main station
    unsigned m_MainStationArrival;
    unsigned m_MainStationDeparture;
    // arrival at the target station
    unsigned m_TargetStationArrival;
};

#endif /* CTRAIN_H_ */
