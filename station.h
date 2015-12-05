#ifndef STATION_H
#define STATION_H

#include "public_train_generator.h"
#include <string>

class CStation
{
public:
    CStation(const std::string& title = "");

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

protected:
	std::string m_Title;
};

#endif // STATION_H
