/*
 * defect_generator.cpp
 *
 *  Created on: Dec 4, 2015
 *      Author: jose
 */

#include "defect_generator.h"

CDefectGenerator::~CDefectGenerator()
{
  // TODO Auto-generated destructor stub
}

void CDefectGenerator::Behavior()
{
  assert(m_freq);
  (new CDefect(m_repairTime))->Activate(Time);
  double plan = Time + Exponential(m_freq);
  DBG_LOG("DEFECT PLANNED ON " << CTimeInterval::MinutesToTime(plan));
  DBG_LOG("REPAIR TIME" << CTimeInterval::MinutesToTime(m_repairTime));
  Activate(plan);
}

