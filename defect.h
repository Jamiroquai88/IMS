/*
 * defect.h
 *
 *  Created on: Dec 4, 2015
 *      Author: jose
 */

#ifndef DEFECT_H_
#define DEFECT_H_

#include "simlib.h"
#include "debug.h"

class CDefect : public Process
{
  public:

    CDefect(unsigned int repair) : m_trainRepair(repair), m_defStartTime(0) {};
    virtual ~CDefect();

    void Behavior();
    inline unsigned int GetDelay() {return m_trainRepair;}
    unsigned int GetDefectStartTime() {return m_defStartTime;}

  private:
    void SetDefectStartTime(unsigned int time);

    unsigned int m_trainRepair;
    unsigned int m_defStartTime;
};

#endif /* DEFECT_H_ */
