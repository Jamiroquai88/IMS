/*
 * defect.cpp
 *
 *  Created on: Dec 4, 2015
 *      Author: jose
 */

#include "defect.h"
#include "train.h"

CDefect::~CDefect()
{
  // TODO Auto-generated destructor stub
}

void CDefect::Behavior()
{
  DBG_LOG_T("Generated defect, takes " << m_trainRepair);
  CMainStation::Tracks tracks = CMainStation::GetInstance().GetTracks();

  SetDefectStartTime(Time);

  unsigned int index = ((int)round(simlib3::Random()*100)); // % tracks.size();
  index = index % tracks.size();

  // multiple defects at the same time are disabled
  if(tracks[index]->GetDefect())
      return;

  DBG_LOG("Defect on Track generated");

  unsigned location = ((int)round(simlib3::Random()*100));
  location = location % tracks[index]->GetLength();

  tracks[index]->SetDefect(*this, location);

  DBG_LOG("Index of track " << index << location);

  CTrack::Trains passingTrains;
  tracks[index]->GetPassingTrains(location, true, passingTrains);
  DBG_LOG("PASSING TRAINS: " << passingTrains.size());
  for(auto train : passingTrains)
  {
      // interrupt travelling trains
      if(train->GetTrackDuration())
      {
          train->Activate();
      }
  }

  double repair = Exponential(m_trainRepair);
  DBG_LOG("Defect repair time: " << m_trainRepair);
  CMainStation::GetInstance().GetDefectsHistogram()(repair);
  Wait(repair);
  tracks[index]->ClearDefect();

  // reload passing trains (new trains could have been generated and are waiting)
  passingTrains.clear();
  tracks[index]->GetPassingTrains(location, true, passingTrains);
  DBG_LOG("PASSING TRAINS: " << passingTrains.size());
  for(auto train : passingTrains)
  {
      train->Activate();
  }
}

void CDefect::SetDefectStartTime(unsigned int time)
{
  m_defStartTime = time;
}

