/*
 * train.h
 *
 *  Created on: Dec 6, 2015
 *      Author: pavel
 */

#ifndef TRAIN_H_
#define TRAIN_H_

#include "train_generator.h"
#include "track.h"
#include <simlib.h>
#include <assert.h>

class CTrain : public simlib3::Process
{
public:
    /**
     * \brief Event that periodically activates the train, letting it update its progress.
     */
    class CProgressUpdateEvent : public Event
    {
    public:
        static const unsigned FREQUENCY;

        CProgressUpdateEvent(CTrain& train);

        void Behavior();
    private:
        CTrain& m_Train;
    };

    CTrain(CTrainGenerator& generator,
        unsigned scheduledStartTime,
        unsigned scheduledTargetStationArrival,
        unsigned scheduledMainStationArrival = 0,
        unsigned scheduledMainStationDeparture = 0);
    virtual ~CTrain();

    unsigned GetTrackDuration() const;
    unsigned GetTraveledMinutes() const;
    unsigned GetDistanceFromMainStation();
    bool IsGoingFromMainStation() const;
    CTrainGenerator& GetGenerator();

protected:
    void Travel(unsigned duration);

    CTrainGenerator& m_Generator;

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
inline unsigned CTrain::GetDistanceFromMainStation()
{
    // only defined if the train is on a track
    assert(m_pTrack);

    unsigned distance = 0;

    if(m_TrackDuration > 0)
    {
        double percent = m_TraveledMinutes / (double)m_TrackDuration;
        distance = m_pTrack->GetLength() * percent;
    }
    // revert
    if(!m_DirFromMainStation)
    {
        distance = m_pTrack->GetLength() - distance;
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
inline bool CTrain::IsGoingFromMainStation() const
{
    return m_DirFromMainStation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline CTrainGenerator& CTrain::GetGenerator()
{
    return m_Generator;
}

#endif /* TRAIN_H_ */
