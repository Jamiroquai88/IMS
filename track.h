/*
 * track.h
 *
 *  Created on: Dec 4, 2015
 *      Author: pavel
 */

#ifndef TRACK_H_
#define TRACK_H_

#include "simlib.h"

class CTrack
{
public:
    /**
     * \brief Constructor.
     * \param length Track length
     */
    CTrack(unsigned length);

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
    /** \} */

    /**
     * \brief   Add nested segment. Segment's parent is set to this. This object is the owner,
     *          deletes the segment in the destructor.
     * \param   pNestedSegment Pointer to CTrack object.
     */
    void AddNestedSegment(CTrack* pNestedSegment);

private:
    // length
    unsigned m_Length;
    // parent track
    CTrack* m_pParentTrack;
    // nested segment
    CTrack* m_pNestedSegment;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Public interface                                          //
////////////////////////////////////////////////////////////////////////////////////////////////////
inline CTrack* CTrack::GetNestedSegment() const
{
    return m_pNestedSegment;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void CTrack::AddNestedSegment(CTrack* pNestedSegment)
{
    m_pNestedSegment = pNestedSegment;
    m_pNestedSegment->m_pParentTrack = this;
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


#endif /* TRACK_H_ */
