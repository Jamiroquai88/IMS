/*
 * cargo_train.cpp
 *
 *  Created on: Dec 5, 2015
 *      Author: pavel
 */

#include "cargo_train.h"
#include "cargo_train_generator.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
CCargoTrain::CCargoTrain(const CCargoTrainGenerator& generator,
        unsigned scheduledStartTime,
        unsigned scheduledTargetStationArrival,
        unsigned scheduledMainStationArrival,
        unsigned scheduledMainStationDeparture)
  : CTrain(generator, scheduledStartTime, scheduledTargetStationArrival, scheduledMainStationArrival,
          scheduledMainStationDeparture)
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
CCargoTrain::~CCargoTrain()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CCargoTrain::Behavior()
{
    // create update progress event
    CProgressUpdateEvent& progressUpdateEvent = *(new CProgressUpdateEvent(*this));
    progressUpdateEvent.Activate(Time + CProgressUpdateEvent::FREQUENCY);

    // go to the platform
    m_Generator.GetStartStation().Enter(*this);
}
