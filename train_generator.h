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
class CTrainGenerator : public Event
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
     * \brief Constructor
     */
    CTrainGenerator(const std::string& trainTitle,
        CStation& startStation,
        CStation& targetStation,
        Frequency frequency,
        unsigned targetStationArrival,
        unsigned averageDelay = 0,
        unsigned mainStationArrival = 0,
        unsigned mainStationDeparture = 0);

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

    // arrival at the target station
    unsigned m_TargetStationArrival;

    // average delay
    unsigned m_AverageDelay;

    // stop at main station
    unsigned m_MainStationArrival;
    unsigned m_MainStationDeparture;

    // schedule exceptions
    ScheduleExceptions m_ScheduleExceptions;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Public interface                                          //
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const std::string& CTrainGenerator::GetTrainTitle() const
{
    return m_TrainTitle;
}

inline unsigned CTrainGenerator::GetAverageDelay() const
{
    return m_AverageDelay;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline CTrainGenerator::Frequency CTrainGenerator::GetFrequency() const
{
    return m_Frequency;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline unsigned CTrainGenerator::GetMainStationArrival() const
{
    return m_MainStationArrival;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline unsigned CTrainGenerator::GetMainStationDeparture() const
{
    return m_MainStationDeparture;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const CTrainGenerator::ScheduleExceptions& CTrainGenerator::GetScheduleExceptions() const
{
    return m_ScheduleExceptions;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const CStation& CTrainGenerator::GetStartStation() const
{
    return m_StartStation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const CStation& CTrainGenerator::GetTargetStation() const
{
    return m_TargetStation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline unsigned CTrainGenerator::GetTargetStationArrival() const
{
    return m_TargetStationArrival;
}


#endif /* TRAIN_GENERATOR_H_ */
