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
    unsigned GetRealStartTime() const;
    unsigned GetTrackDuration() const;
    unsigned GetTraveledMinutes() const;
    const CTrainGenerator& GetGenerator() const;

private:
    void Travel(unsigned duration);

    const CTrainGenerator& m_Generator;

    // own schedule times (computed by generator)

    // start time
    unsigned m_ScheduledStartTime;
    unsigned m_RealStartTime;

    // stop at main station
    unsigned m_ScheduledMainStationArrival;
    unsigned m_ScheduledMainStationDeparture;
    unsigned m_RealMainStationdeparture;

    // arrival at the target station
    unsigned m_ScheduledTargetStationArrival;

    // actual track duration according to the schedule
    unsigned m_TrackDuration;
    // minutes already traveled at the actual track
    unsigned m_TraveledMinutes;

    CTrack* m_pTrack;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
inline unsigned CTrain::GetRealMainStationdeparture() const
{
    return m_RealMainStationdeparture;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline unsigned CTrain::GetRealStartTime() const
{
    return m_RealStartTime;
}

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
inline const CTrainGenerator& CTrain::GetGenerator() const
{
    return m_Generator;
}

#endif /* CTRAIN_H_ */
