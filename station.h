#ifndef STATION_H
#define STATION_H

#include "public_train_generator.h"
#include <string>

class CPublicTrain;
class CCargoTrain;
class CTrack;
class CTrain;

class CStation
{
public:
    CStation(const std::string& title = "");
    virtual ~CStation();

	const std::string& GetTitle() const;

	/**
     * \brief   Add public train
     */
    CPublicTrainGenerator& AddPublicTrain(const std::string& trainTitle,
            unsigned time,
            CStation& targetStation,
            CPublicTrainGenerator::Frequency frequency,
            unsigned targetStationArrival,
            unsigned averageDelay = 0,
            bool bStopsInMainStation = false,
            unsigned mainStationArrival = 0,
            unsigned mainStationDeparture = 0);

    /**
     * \brief   Add cargo train
     */
    CTrainGenerator& AddCargoTrain(const std::string& trainTitle,
            unsigned time,
            CStation& targetStation,
            CPublicTrainGenerator::Frequency frequency,
            unsigned targetStationArrival,
            unsigned averageDelay = 0,
            bool bStopsInMainStation = false,
            unsigned mainStationArrival = 0,
            unsigned mainStationDeparture = 0);


    /**
     * \brief   Train enters the station
     */
    virtual void Enter(CPublicTrain& train) {};
    virtual void Enter(CCargoTrain& train) {};

    /**
     * \brief   Train leaves the station
     */
    virtual void Leave(CPublicTrain& train) {};
    virtual void Leave(CCargoTrain& train) {};

    virtual void SeizeTrack(CTrain& train, CTrack* pTrack, bool bStart) {};
    virtual void ReleaseTrack(CTrain& train, CTrack* pTrack, bool bStart) {};

protected:
	std::string m_Title;
};

#endif // STATION_H
