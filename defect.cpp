/*
 * defect.cpp
 *
 *  Created on: Dec 4, 2015
 *      Author: jose
 */

#include "defect.h"

CDefect::~CDefect()
{
  // TODO Auto-generated destructor stub
}

void CDefect::Behavior()
{
  DBG_LOG_T("Generated defect, takes " << m_trainRepair);
  CMainStation::Tracks tracks = CMainStation::GetInstance().GetTracks();

  SetDefectStartTime(Time);

  RandomSeed(42);
  unsigned int index = ((int)round(simlib3::Random()*100000)) % tracks.size();
  for(auto train : tracks[index]->GetPassingTrains())
  {
    train->Activate();
  }
}

void CDefect::SetDefectStartTime(unsigned int time)
{
  m_defStartTime = time;
}

