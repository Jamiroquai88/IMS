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

    CDefect(unsigned int repair) : m_trainRepair(repair) {};
    virtual ~CDefect();

    void Behavior();

  private:
    unsigned int m_trainRepair;
};

#endif /* DEFECT_H_ */
