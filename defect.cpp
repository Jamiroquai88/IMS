/*
 * defect.cpp
 *
 *  Created on: Dec 4, 2015
 *      Author: jose
 */

#include "defect.h"
#include "public_train.h"

CDefect::~CDefect()
{
  // TODO Auto-generated destructor stub
}

void CDefect::Behavior()
{
  CMainStation::Tracks tracks = CMainStation::GetInstance().GetTracks();

  SetDefectStartTime(Time);

  unsigned int index = ((int)round(simlib3::Random()*100)); // % tracks.size();
  index = index % tracks.size();

  // add rand direction
  m_dirFromMainStation = false;

  // multiple defects at the same time are disabled
  if(tracks[index]->GetDefect(0, m_dirFromMainStation) != NULL)
  {
      DBG_LOG("DEBUG: Two DEFECT");
      return;
  }
  m_distanceFromMainStation = ((int)round(simlib3::Random()*100));
  m_distanceFromMainStation = m_distanceFromMainStation % tracks[index]->GetLength();

  tracks[index]->SetDefect(*this, m_distanceFromMainStation);

  CTrack::Trains passingTrains;

  tracks[index]->GetComingTrains(m_distanceFromMainStation, m_dirFromMainStation, passingTrains);
  DBG_LOG("AFFECTED TRAINS START: " << passingTrains.size());
  DBG_LOG("STOPPING TRAINS: " << passingTrains.size());
  for(auto train : passingTrains)
  {
      // interrupt travelling trains
      if(train->GetTrackDuration())
      {
          DBG_LOG("INTERRUPT TRAIN WHEN DEFECT");
          train->Activate();
      }
  }

  double repair = Exponential(m_trainRepair);

  DBG_LOG("!!!");
  DBG_LOG_T("NEW DEFECT " << m_Id << ": "
    << "repair time: " << m_trainRepair
    << " track to: " << tracks[index]->GetAdjacentStation().GetTitle());
  DBG_LOG("!!!");

  CMainStation::GetInstance().GetDefectsHistogram()(repair);
  Wait(repair);

  tracks[index]->ClearDefect();

  DBG_LOG("DEFECT REMOVED (id: " << m_Id << ")");

  // move waiting trains
  // reload passing trains (new trains could have been generated and are waiting)
  passingTrains.clear();
  tracks[index]->GetComingTrains(m_distanceFromMainStation, m_dirFromMainStation, passingTrains);
  DBG_LOG("AFFECTED TRAINS END: " << passingTrains.size());
  for(auto train : passingTrains)
  {
      DBG_LOG_T("ACTIVATING TRAIN AFTER DEFECT");
      train->Activate();
  }
}

void CDefect::SetDefectStartTime(unsigned int time)
{
  m_defStartTime = time;
}

bool CDefect::GetDirection() const
{
    return m_dirFromMainStation;
}

unsigned int CDefect::GetLocation() const
{
    return m_distanceFromMainStation;
}

unsigned int CDefect::GetId() const
{
    return m_Id;
}
