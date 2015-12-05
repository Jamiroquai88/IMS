/*
 * train_generator.h
 *
 *  Created on: Nov 29, 2015
 *      Author: pavel
 */

#ifndef TRAIN_GENERATOR_H_
#define TRAIN_GENERATOR_H_

#include "simlib.h"
#include <iostream>
#include <vector>

class CStation;
class CTimeInterval;

/**
 * \brief   Train generator - generating trains according to the schedule.
 */
class CPublicTrainGenerator : public Event
{
public:

    enum Frequency
    {
        FREQ_ONCE = 0,
        FREQ_HOURLY = 60,
        FREQ_DAILY = 24 * FREQ_HOURLY,
        FREQ_WEEKLY = 7 * FREQ_DAILY
    };

    typedef std::vector<CTimeInterval*> ScheduleExceptions;

    /**
     * \brief Constructor.
     */
    CPublicTrainGenerator(const std::string& trainTitle,
        CStation& startStation,
        CStation& targetStation,
        Frequency frequency,
        unsigned scheduleStartTime,
        unsigned scheduleTargetStationArrival,
        unsigned averageDelay = 0,
        bool bStopsInMainStation = false,
        unsigned scheduleMainStationArrival = 0,
        unsigned scheduleMainStationDeparture = 0);

    /**
     * \brief Destructor.
     */
    ~CPublicTrainGenerator();

    /**
     * \brief Add interval when the train is not generated
     */
    void AddException(unsigned minutesStart, unsigned minutesEnd);

    void Behavior() override;

    /**
     * \{
     * \brief Getter.
     */
    const std::string& GetTrainTitle() const;
    unsigned GetAverageDelay() const;
    Frequency GetFrequency() const;
    bool StopsInMainStation() const;
    unsigned GetMainStationArrival() const;
    unsigned GetMainStationDeparture() const;
    const ScheduleExceptions& GetScheduleExceptions() const;
    const CStation& GetStartStation() const;
    const CStation& GetTargetStation() const;
    unsigned GetTargetStationArrival() const;
    /** \} */

private:
    // train title
    std::string m_TrainTitle;

    CStation& m_StartStation;
    CStation& m_TargetStation;
    Frequency m_Frequency;

    // start time
    unsigned m_ScheduleStartTime;

    // arrival at the target station
    unsigned m_ScheduleTargetStationArrival;

    // average delay
    unsigned m_AverageDelay;

    // stops in main station
    bool m_StopsInMainStation;

    // stop at main station
    unsigned m_ScheduleMainStationArrival;
    unsigned m_ScheduleMainStationDeparture;

    // schedule exceptions
    ScheduleExceptions m_ScheduleExceptions;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Public interface                                          //
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const std::string& CPublicTrainGenerator::GetTrainTitle() const
{
    return m_TrainTitle;
}

inline unsigned CPublicTrainGenerator::GetAverageDelay() const
{
    return m_AverageDelay;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline CPublicTrainGenerator::Frequency CPublicTrainGenerator::GetFrequency() const
{
    return m_Frequency;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool CPublicTrainGenerator::StopsInMainStation() const
{
    return m_StopsInMainStation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline unsigned CPublicTrainGenerator::GetMainStationArrival() const
{
    return m_ScheduleMainStationArrival;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline unsigned CPublicTrainGenerator::GetMainStationDeparture() const
{
    return m_ScheduleMainStationDeparture;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const CPublicTrainGenerator::ScheduleExceptions& CPublicTrainGenerator::GetScheduleExceptions() const
{
    return m_ScheduleExceptions;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const CStation& CPublicTrainGenerator::GetStartStation() const
{
    return m_StartStation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const CStation& CPublicTrainGenerator::GetTargetStation() const
{
    return m_TargetStation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline unsigned CPublicTrainGenerator::GetTargetStationArrival() const
{
    return m_ScheduleTargetStationArrival;
}


#endif /* TRAIN_GENERATOR_H_ */
