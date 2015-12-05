#ifndef STATION_H
#define STATION_H

#include "public_train_generator.h"
#include <string>

class CPublicTrain;

class CStation
{
public:
    CStation(const std::string& title = "");
    virtual ~CStation();

	const std::string& GetTitle() const;

	/**
     * \brief   Add train
     */
    CPublicTrainGenerator& AddTrain(const std::string& trainTitle,
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
    virtual void Enter(CPublicTrain& train) const;

    /**
     * \brief   Train leaves the station
     */
    virtual void Leave(CPublicTrain& train) const;

protected:
	std::string m_Title;
};

#endif // STATION_H
