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

  DBG_LOG("Defect on Track generated");

  unsigned int index = ((int)round(simlib3::Random()*100)); // % tracks.size();
  index = index % tracks.size();

  unsigned location = ((int)round(simlib3::Random()*100));
  location = location % tracks[index]->GetLength();

  tracks[index]->SetDefect(*this, location);

  DBG_LOG("Index of track " << index << location);

  CTrack::Trains passingTrains;
  tracks[index]->GetPassingTrains(location, true, passingTrains);
  for(auto train : passingTrains)
  {
    train->Activate();
  }

  double repair = Exponential(m_trainRepair);
  CMainStation::GetInstance().GetHistogram()(repair);
  Wait(repair);
  tracks[index]->ClearDefect();

  tracks[index]->GetPassingTrains(location, true, passingTrains);
  for(auto train : passingTrains)
  {
    train->Activate();
  }
}

void CDefect::SetDefectStartTime(unsigned int time)
{
  m_defStartTime = time;
}

