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
#include "main_station.h"
#include <math.h>

class CDefect : public Process
{
  public:

    CDefect(unsigned int repair)
      : m_trainRepair(repair), m_defStartTime(0),
        m_dirFromMainStation(true), m_distanceFromMainStation(0),
        m_Id(Time)
    {};

    virtual ~CDefect();

    void Behavior();
    inline unsigned int GetDelay() {return m_trainRepair;}
    unsigned int GetDefectStartTime() {return m_defStartTime;}

    bool GetDirection() const;
    unsigned int GetLocation() const;
    unsigned int GetId() const;

  private:
    void SetDefectStartTime(unsigned int time);

    unsigned int m_trainRepair;
    unsigned int m_defStartTime;
    bool m_dirFromMainStation;
    unsigned int m_distanceFromMainStation;
    unsigned int m_Id;
};

#endif /* DEFECT_H_ */
