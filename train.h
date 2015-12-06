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

    CTrain(const CTrainGenerator& generator);
    virtual ~CTrain();

    unsigned GetTrackDuration() const;
    unsigned GetTraveledMinutes() const;
    unsigned GetDistanceFromMainStation() const;
    const CTrainGenerator& GetGenerator() const;

protected:
    void Travel(unsigned duration);

    const CTrainGenerator& m_Generator;

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

#endif /* TRAIN_H_ */
