/*
 * track.h
 *
 *  Created on: Dec 4, 2015
 *      Author: pavel
 */

#ifndef TRACK_H_
#define TRACK_H_

#include "simlib.h"
#include "defect.h"
#include "adjacent_station.h"
#include <set>

class CTrain;

class CTrack
{
public:
    typedef std::set<CTrain*> Trains;

    /**
     * \brief Constructor.
     * \param srcStation    Adjacent train station (destination is always the main station)
     * \param length        Track length
     */
    CTrack(const CAdjacentStation& adjacentStation, unsigned length);

    /**
     * \brief Destructor.
     */
    virtual ~CTrack();

    /**
     * \{
     * \brief Getter.
     */
    CTrack* GetNestedSegment() const;
    CTrack* GetParentTrack() const;
    unsigned GetLength() const;
    unsigned GetNestedSegmentLength() const;
    const Trains& GetPassingTrains() const;
    /** \} */

    /**
     * \brief   Add nested segment. Segment's parent is set to this. This object is the owner,
     *          deletes the segment in the destructor.
     * \param   pNestedSegment Pointer to CTrack object.
     */
    CTrack& AddNestedSegment(CTrack* pNestedSegment);

    /**
     * \brief Add passing train. The train will be notified about accidents/defects.
     * \param train Train to be added
     */
    void AddPassingTrain(CTrain& train);

    /**
     * \brief Remove passing train.
     * \param train Train to be removed
     */
    void RemovePassingTrain(CTrain& train);

    /**
     * \brief Set defect.
     * \param defect Defect
     */
    void SetDefect(CDefect& defect);

    /**
     * \brief Remove defect.
     */
    void ClearDefect();

private:
    // adjacent station
    const CAdjacentStation& m_AdjacentStation;
    // length
    unsigned m_Length;
    // parent track
    CTrack* m_pParentTrack;
    // nested segment
    CTrack* m_pNestedSegment;
    // passing trains
    Trains m_PassingTrains;
    // defect
    CDefect* m_pDefect;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Public interface                                          //
////////////////////////////////////////////////////////////////////////////////////////////////////
inline CTrack* CTrack::GetNestedSegment() const
{
    return m_pNestedSegment;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline CTrack* CTrack::GetParentTrack() const
{
    return m_pParentTrack;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline unsigned CTrack::GetLength() const
{
    return m_Length;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline unsigned CTrack::GetNestedSegmentLength() const
{
    return m_pNestedSegment ? m_pNestedSegment->GetLength() : 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const CTrack::Trains& CTrack::GetPassingTrains() const
{
    return m_PassingTrains;
}


#endif /* TRACK_H_ */
