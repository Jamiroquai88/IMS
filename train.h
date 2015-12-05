/*
 * CTrain.h
 *
 *  Created on: Nov 25, 2015
 *      Author: jose
 */

#ifndef CTRAIN_H_
#define CTRAIN_H_

#include "simlib.h"
#include "station.h"
#include "track.h"

class CTrainGenerator;

class CTrain : public Process
{
public:

    class CProgressUpdateEvent : public Event
    {
    public:
        static const unsigned FREQUENCY;

        CProgressUpdateEvent(CTrain& train);
        void Behavior();
    private:
        CTrain& m_Train;
    };

    /**
     * \brief Constructor
     * \param generator Train generator containing train settings (schedule, exceptions..)
     */
    CTrain(const CTrainGenerator& generator,
        unsigned scheduledStartTime,
        unsigned scheduledTargetStationArrival,
        unsigned scheduledMainStationArrival = 0,
        unsigned scheduledMainStationDeparture = 0);

    virtual ~CTrain();

    void Behavior();

    unsigned GetRealMainStationdeparture() const;
    unsigned GetTrackDuration() const;
    unsigned GetTraveledMinutes() const;
    unsigned GetDistanceFromMainStation() const;
    const CTrainGenerator& GetGenerator() const;

private:
    void Travel(unsigned duration);

    const CTrainGenerator& m_Generator;

    // own schedule times (computed by generator)

    // start time
    unsigned m_ScheduledStartTime;

    // stop at main station
    unsigned m_ScheduledMainStationArrival;
    unsigned m_ScheduledMainStationDeparture;

    // arrival at the target station
    unsigned m_ScheduledTargetStationArrival;

    // actual track duration according to the schedule
    unsigned m_TrackDuration;
    // minutes already traveled at the actual track
    unsigned m_TraveledMinutes;

    CTrack* m_pTrack;
    bool m_DirFromMainStation;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
inline unsigned CTrain::GetTrackDuration() const
{
    return m_TrackDuration;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline unsigned CTrain::GetTraveledMinutes() const
{
    return m_TraveledMinutes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline unsigned CTrain::GetDistanceFromMainStation() const
{
    // only defined if the train is on a track
    assert(m_pTrack);

    unsigned distance = 0;
    if(m_TraveledMinutes > 0)
    {
        assert(m_TrackDuration > 0);

        double percent = m_TraveledMinutes / (double)m_TrackDuration;
        distance = m_pTrack->GetLength() * percent;
    }

    DBG_LOG_T(GetGenerator().GetTrainTitle()
            << ":\tTrain Location: "
            << CTimeInterval::MinutesToTime(m_ScheduledStartTime)
            << " - "
            << CTimeInterval::MinutesToTime(m_ScheduledTargetStationArrival)
            << '\t' << CTimeInterval::MinutesToTime(m_ScheduledTargetStationArrival - m_ScheduledStartTime)
            << '\t' << distance << " / " << m_pTrack->GetLength());

    return distance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const CTrainGenerator& CTrain::GetGenerator() const
{
    return m_Generator;
}

#endif /* CTRAIN_H_ */
