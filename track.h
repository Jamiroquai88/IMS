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
class CDefect;

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
    const CAdjacentStation& GetAdjacentStation() const;
    /** \} */

    /**
     * \brief Get defect if there is any in front of the given location.
     * \param location              Distance from the main station
     * \param dirFromMainStation    Direction - from / to the main station
     */
    CDefect* GetDefect(unsigned location, bool dirFromMainStation) const;

    /**
     * \brief Get trains coming to the given point.
     * \param location          Distance from the main station
     * \param bFromMainStation  Direction of the train
     * \param trains            Output trains
     */
    void GetComingTrains(unsigned location, bool bFromMainStation, Trains& trains) const;

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
    void AddPassingTrain(CPublicTrain& train);

    /**
     * \brief Remove passing train.
     * \param train Train to be removed
     */
    void RemovePassingTrain(CPublicTrain& train);

    /**
     * \brief Set defect.
     * \param defect    Defect
     * \param location  Number of kilometers from the main station
     */
    void SetDefect(CDefect& defect, unsigned location);

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
inline const CAdjacentStation& CTrack::GetAdjacentStation() const
{
    return m_AdjacentStation;
}

#endif /* TRACK_H_ */
