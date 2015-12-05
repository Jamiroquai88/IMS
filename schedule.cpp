/*
 * schedule.cpp
 *
 *  Created on: Nov 26, 2015
 *      Author: jose
 */

#include "schedule.h"
#include "time_interval.h"

CSchedule::CSchedule()
{

}

bool CSchedule::OpenConfigFile(std::string file)
{
  m_ConfigFile.open(file.c_str());
  if(!m_ConfigFile.is_open())
  {
    std::cerr << "ERROR: Can not open configuration file!\n";
    return false;
  }
  else
    return true;
}

bool CSchedule::GetLink(STrainProcess& unit)
{
  if(!m_ConfigFile.is_open())
  {
    std::cerr << "ERROR: Call method CSchedule::OpenConfigFile() first!\n";
    return false;
  }

  std::string line;
  unsigned int i = 0;


  while(getline(m_ConfigFile, line))
  {
    if(!ParseLine(line, i, unit))
    {
      std::cerr << "ERROR: Syntax error in configuration file on line: " << i + 1 << std::endl;
      return false;
    }
    if(i == 4)
    {
      if(unit.through != "")
      {
        getline(m_ConfigFile, line);
      }
      return true;
    }
    i++;
  }
  unit.name = "EOF";
  return true;
}

bool CSchedule::ParseLine(std::string line, unsigned int counter, STrainProcess& conf_struct)
{
  unsigned int line_num = counter % NUM_OF_LINES;
  switch(line_num)
  {
    case 0:
      if(line[0] == '[' && line[line.length() - 1] == ']')
      {
        conf_struct.name = line.substr(1, line.length() - 2);
      }
      else
        return false;
      break;

    case 1:
      if(line.substr(0, 6) == "From: ")
      {
        conf_struct.from = line.substr(6, line.find('-') - 7);
        conf_struct.appears = atoi(line.substr(line.find('-') + 2, 2).c_str()) * 60
            + atoi(line.substr(line.find('-') + 5, 2).c_str());
      }
      else
        return false;
      break;

    case 2:
      if(line.substr(0, 4) == "To: ")
      {
        conf_struct.through = "";
        conf_struct.to = line.substr(4, line.find('-') - 5);
        conf_struct.disappears = atoi(line.substr(line.find('-') + 2, 2).c_str()) * 60
            + atoi(line.substr(line.find('-') + 5, 2).c_str());
        conf_struct.comes = 0;
        conf_struct.leaves = 0;
      }
      else
      {
        if(line.substr(0, 15) == "Through: Zilina")
        {
          conf_struct.through = "Zilina";
          conf_struct.comes = atoi(line.substr(line.find('-') + 2, 2).c_str()) * 60
              + atoi(line.substr(line.find('-') + 5, 2).c_str());
          conf_struct.leaves = atoi(line.substr(line.find(',') + 2, 2).c_str()) * 60
              + atoi(line.substr(line.find(',') + + 5, 2).c_str());
        }
        else
          return false;
      }
      break;

    case 3:
      if(line.substr(0, 4) == "To: ")
      {
        conf_struct.to = line.substr(4, line.find('-') - 5);
        conf_struct.disappears = atoi(line.substr(line.find('-') + 2, 2).c_str()) * 60
            + atoi(line.substr(line.find('-') + 5, 2).c_str());
        conf_struct.late = 0;
      }
      else
      {
        if(line.substr(0, 6) == "Late: ")
        {
          conf_struct.late = atoi(line.substr(line.find(':') + 2, line.length() - 6).c_str());
        }
        else
          return false;
      }
      break;

    case 4:
      if(line.substr(0, 6) == "Late: ")
      {
        conf_struct.late = atoi(line.substr(line.find(':') + 2, line.length() - 6).c_str());
      }
      else
      {
        if(line.length() == 0)
          break;
        else
          return false;
      }
      break;

    case 5:
      break;

    default:
      return false;
  }
  return true;
}

bool CSchedule::ParseAndPlan(CMainStation& mainStation)
{
  CSchedule::STrainProcess unit;
  unit.name = "";

  while(1)
  {
    if(GetLink(unit))
    {
      if(unit.name == "EOF")
        return true;

      CStation* pStationFrom;
      CStation* pStationTo;
      if(unit.from != "Zilina")
      {
          pStationFrom = &mainStation.GetAdjacentStation(unit.from);
      }
      else
      {
          pStationFrom = &mainStation;
      }

      if(unit.to != "Zilina")
      {
          pStationTo = &mainStation.GetAdjacentStation(unit.to);
      }
      else
      {
          pStationTo = &mainStation;
      }

      // travelling over midnight - move to next day
      if(!unit.through.empty() )
      {
        // comes
        if(unit.comes < unit.appears)
        {
            unit.comes += 60*24;
            unit.leaves += 60*24;
            unit.disappears += 60*24;
            DBG_LOG("SHIFT " << unit.name);
        }

        // leaves
        if(unit.leaves < unit.comes)
        {
            unit.leaves += 60*24;
            unit.disappears += 60*24;
            DBG_LOG("SHIFT " << unit.name);
        }

        // disappears
        if(unit.disappears < unit.leaves)
        {
            unit.disappears += 60*24;
            DBG_LOG("SHIFT " << unit.name);
        }
      }
      // no stop in main station
      else if(unit.disappears < unit.appears)
      {
          unit.disappears += 60*24;
          DBG_LOG("SHIFT " << unit.name);
      }


      CTrainGenerator::Frequency freq;
      if(unit.name[unit.name.length()-1] == 'W')
        freq = CTrainGenerator::FREQ_WEEKLY;
      else
        freq = CTrainGenerator::FREQ_DAILY;

      pStationFrom->AddTrain(unit.name,
        unit.appears, *pStationTo,
        freq, unit.disappears, unit.late,
        !unit.through.empty(), unit.comes, unit.leaves);

      std::cout  << "Train " << unit.name << " schedule: \t\t"
            << CTimeInterval::MinutesToTime(unit.appears)
            << ", from " << unit.from
            << " to " << unit.to << std::endl;
      continue;
    }
    else
      return false;
  }
  return true;
}

CSchedule::~CSchedule()
{
  // TODO Auto-generated destructor stub
}

