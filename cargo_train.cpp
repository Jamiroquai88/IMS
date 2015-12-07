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
{
    CMainStation* pMainStation = &CMainStation::GetInstance();

    if(&generator.GetStartStation() == pMainStation)
    {
        m_pTrack = &pMainStation->GetTrack(static_cast<CAdjacentStation&>(generator.GetTargetStation()));
    }
    else
    {
        m_pTrack = &pMainStation->GetTrack(static_cast<CAdjacentStation&>(generator.GetStartStation()));
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
CCargoTrain::~CCargoTrain()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CCargoTrain::Behavior()
{
    DBG_LOG_T(m_Generator.GetTrainTitle() + ":\t\tGenerated in " + m_Generator.GetStartStation().GetTitle());

    // a track needed for the train
    m_Generator.GetStartStation().Enter(*this);

    // lock the track start
    m_Generator.GetStartStation().SeizeTrack(*this, m_pTrack, true);
    Wait(1);
    // leave the platform
    m_Generator.GetStartStation().Leave(*this);
    // unlock the track start
    m_Generator.GetStartStation().ReleaseTrack(*this, m_pTrack, true);

    Travel(m_ScheduledTargetStationArrival - m_ScheduledStartTime);

    // lock track end
    m_Generator.GetTargetStation().SeizeTrack(*this, m_pTrack, false);
    Wait(1);
    // enter the station
    m_Generator.GetTargetStation().Enter(*this);
    // unlock track end
    m_Generator.GetTargetStation().ReleaseTrack(*this, m_pTrack, false);

    // release the rail
    m_Generator.GetTargetStation().Leave(*this);
}
