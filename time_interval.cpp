/*
 * time_interval.cpp
 *
 *  Created on: Nov 29, 2015
 *      Author: pavel
 */

#include "time_interval.h"
#include <sstream>

////////////////////////////////////////////////////////////////////////////////////////////////////
//                             CTrainGenerator::ScheduleException                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////
CTimeInterval::CTimeInterval(unsigned minutesStart, unsigned minutesEnd)
  : m_MinutesStart(minutesStart),
    m_MinutesEnd(minutesEnd)
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTimeInterval::IsInInterval(unsigned minutes)
{
    return (minutes > m_MinutesStart && minutes < m_MinutesEnd);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned CTimeInterval::TimeToMinutes(unsigned hours,
    unsigned minutes,
    unsigned days)
{
    return minutes + hours*60 + days*24*60;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
std::string CTimeInterval::MinutesToTime(unsigned minutes)
{
    std::stringstream ss;
    ss << "day " << (minutes / (24*60)) << ", " << ((minutes % (24*60)) / 60) << ":" << (minutes % 60);
    return ss.str();
}
