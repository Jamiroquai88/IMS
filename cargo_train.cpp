/*
 * cargo_train.cpp
 *
 *  Created on: Dec 5, 2015
 *      Author: pavel
 */

#include "cargo_train.h"
#include "cargo_train_generator.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
CCargoTrain::CCargoTrain(CCargoTrainGenerator& generator,
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

    DBG_LOG_T(m_Generator.GetTrainTitle() + ":\t\tGenerated in " + m_Generator.GetStartStation().GetTitle());

    // leaving the station
    m_Generator.GetStartStation().Enter(*this);
    DBG_LOG(m_Generator.GetTrainTitle() << "\t\tCargo leaving from " << m_Generator.GetStartStation().GetTitle());
    Wait(10);
    m_Generator.GetStartStation().Leave(*this);

    Travel(m_ScheduledTargetStationArrival - m_ScheduledStartTime);

    // coming to the station
    m_Generator.GetTargetStation().Enter(*this);
    DBG_LOG(m_Generator.GetTrainTitle() << "\t\tCargo coming to " << m_Generator.GetTargetStation().GetTitle());
    Wait(10);
    m_Generator.GetTargetStation().Leave(*this);

    progressUpdateEvent.Cancel();
}
