/*
 * CTrain.cpp
 *
 *  Created on: Nov 25, 2015
 *      Author: jose
 */

#include "train.h"
#include "train_generator.h"
#include "main_station.h"
#include "time_interval.h"
#include "debug.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////////////
CTrain::CTrain(const CTrainGenerator& generator,
    unsigned targetStationArrival,
    unsigned mainStationArrival,
    unsigned mainStationDeparture)
  : m_Generator(generator),
    m_TargetStationArrival(targetStationArrival),
    m_MainStationArrival(mainStationArrival),
    m_MainStationDeparture(mainStationDeparture)
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
CTrain::~CTrain()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CTrain::Behavior()
{
    // travel to the main station
    DBG_LOG_T(m_Generator.GetTrainTitle() + ": Start in " + m_Generator.GetStartStation().GetTitle());

    // stops in the main station
    if( m_MainStationArrival )
    {
        DBG_LOG(m_Generator.GetTrainTitle() + ": Going to stop in the main station");

        Wait(m_MainStationArrival - Time);

        // in main station
        DBG_LOG(m_Generator.GetTrainTitle() + ": Entering the main station");

        Wait(m_MainStationDeparture - Time);

        // leaving main station
        DBG_LOG(m_Generator.GetTrainTitle() + ": Leaving the main station");
    }

    DBG_LOG(CTimeInterval::MinutesToTime(Time) +
            ": Scheduling arrival to " +
            CTimeInterval::MinutesToTime(m_TargetStationArrival));

    // travel to the target station

    Wait(m_TargetStationArrival - Time);

    // in target station
    DBG_LOG_T(m_Generator.GetTrainTitle() + ": End in " + m_Generator.GetTargetStation().GetTitle());
}

