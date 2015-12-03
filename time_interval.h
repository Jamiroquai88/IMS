/*
 * time_interval.h
 *
 *  Created on: Nov 29, 2015
 *      Author: pavel
 */

#ifndef TIME_INTERVAL_H_
#define TIME_INTERVAL_H_

#include <string>

/**
 * \brief   Exception in a schedule (interval, when the train doesn't go)
 */
class CTimeInterval
{
public:
    /**
     * \brief Constructor
     * \param minutesStart Interval start in minutes from the beginning of the simulation.
     * \param minutesEnd   Interval end in minutes from the beginning of the simulation.
     */
    CTimeInterval(unsigned minutesStart, unsigned minutesEnd);

    /**
     * \brief Test whether the time is in the exception interval.
     * \param minutes Time in minutes from the beginning of the simulation.
     */
    bool IsInInterval(unsigned minutes);

    /**
     * \brief Compute minutes from time
     */
    static unsigned TimeToMinutes(unsigned hours,
        unsigned minutes,
        unsigned days = 0);

    /**
     * \brief format times in minutes (d h:m)
     */
    static std::string MinutesToTime(unsigned minutes);

private:
    unsigned m_MinutesStart;
    unsigned m_MinutesEnd;
};

#endif /* TIME_INTERVAL_H_ */
