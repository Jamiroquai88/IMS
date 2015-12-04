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
  (new CDefect(m_repairTime))->Activate(Time);
  double plan = Time + Exponential(m_freq);
  DBG_LOG("REPAIR TIME" << m_repairTime);
  Activate(plan);
}

