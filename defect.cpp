/*
 * defect.cpp
 *
 *  Created on: Dec 4, 2015
 *      Author: jose
 */

#include "defect.h"
#include "public_train.h"

Histogram CDefect::m_StoppedTrainsHistogram("Trains stopped by a defect", 0, 2, 30);
Histogram CDefect::m_RestartedTrainsHistogram("Trains restarted by a defect repair", 0, 2, 30);

CDefect::~CDefect()
{
  // TODO Auto-generated destructor stub
}

void CDefect::Behavior()
{
  DBG_LOG_T("DEFECT generated");
  CMainStation::Tracks tracks = CMainStation::GetInstance().GetTracks();

  SetDefectStartTime(Time);

  unsigned int index = ((int)round(simlib3::Random()*100)); // % tracks.size();
  index = index % tracks.size();

  // add rand direction
  m_dirFromMainStation = false;

  double repair = Exponential(m_trainRepair);
    DBG_LOG_T("NEW DEFECT " << m_Id << ": "
      << "repair time: " << repair
      << " track to: " << tracks[index]->GetAdjacentStation().GetTitle()
      << " direction: " << (m_dirFromMainStation ? "from " : "to ") << "main station");

  // multiple defects at the same time are disabled
  if(tracks[index]->GetDefect(0, m_dirFromMainStation) != NULL)
  {
      DBG_LOG_T("DEBUG: Two DEFECT");
      return;
  }
  m_distanceFromMainStation = ((int)round(simlib3::Random()*100));
  m_distanceFromMainStation = m_distanceFromMainStation % tracks[index]->GetLength();

  tracks[index]->SetDefect(*this, m_distanceFromMainStation);
  DBG_LOG_T("DEFECT SET");

  CTrack::Trains passingTrains;
  tracks[index]->GetTopParentTrack()->GetComingTrains(m_distanceFromMainStation, m_dirFromMainStation, passingTrains);
  DBG_LOG("AFFECTED TRAINS START: " << passingTrains.size());
  DBG_LOG("STOPPING TRAINS: " << passingTrains.size());
  unsigned stoppedTrains = 0;
  for(auto train : passingTrains)
  {
      // interrupt travelling trains
      if(train->GetTrackDuration())
      {
          DBG_LOG("INTERRUPT TRAIN WHEN DEFECT");
          train->Activate();
          stoppedTrains++;
      }
  }
  m_StoppedTrainsHistogram(stoppedTrains);

  CMainStation::GetInstance().GetDefectsHistogram()(repair);
  Wait(repair);

  tracks[index]->ClearDefect();

  DBG_LOG("DEFECT REMOVED (id: " << m_Id << ")");

  // move waiting trains
  // reload passing trains (new trains could have been generated and are waiting)
  passingTrains.clear();
  tracks[index]->GetTopParentTrack()->GetComingTrains(m_distanceFromMainStation, m_dirFromMainStation, passingTrains);
  DBG_LOG("AFFECTED TRAINS END: " << passingTrains.size());
  for(auto train : passingTrains)
  {
      DBG_LOG_T("ACTIVATING TRAIN AFTER DEFECT");
      train->Activate();
  }
  m_RestartedTrainsHistogram(passingTrains.size());
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
