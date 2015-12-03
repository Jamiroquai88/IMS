/*
 * schedule.h
 *
 *  Created on: Nov 26, 2015
 *      Author: jose
 */

#ifndef SCHEDULE_H_
#define SCHEDULE_H_

#define CONFIG_FILE "schedule.conf"
#define NUM_OF_LINES 6

#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>

#include "main_station.h"
#include "train_generator.h"
#include "adjacent_station.h"

class CSchedule
{
  public:
    typedef struct STrainProcess
    {
      std::string name;
      std::string from;
      std::string through;
      std::string to;
      unsigned int appears;
      unsigned int disappears;
      unsigned int comes;
      unsigned int leaves;
      unsigned int late;
    }STrainProcess;

    CSchedule();
    virtual ~CSchedule();

    bool OpenConfigFile(std::string file = CONFIG_FILE);
    bool ParseAndPlan(CMainStation& mainStation);
    bool GetLink(STrainProcess& unit);

  private:
    bool ParseLine(std::string line, unsigned int counter, STrainProcess& conf_struct);

    typedef std::tuple<size_t, std::string> SAdjStationsTuple;
    std::ifstream m_ConfigFile;
    std::vector<STrainProcess> m_Configuration;
};

#endif /* SCHEDULE_H_ */
