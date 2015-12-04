/*
 * defect_generator.h
 *
 *  Created on: Dec 4, 2015
 *      Author: jose
 */

#ifndef DEFECT_GENERATOR_H_
#define DEFECT_GENERATOR_H_

#include "simlib.h"
#include "defect.h"

class CDefectGenerator : public Event
{
  public:
    CDefectGenerator(unsigned int freq, unsigned int repairTime)
      : m_freq(freq), m_repairTime(repairTime) {};
    virtual ~CDefectGenerator();

    void Behavior();

  private:
    unsigned int m_freq;
    unsigned int m_repairTime;
};

#endif /* DEFECT_GENERATOR_H_ */
