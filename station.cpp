#include "station.h"
#include "debug.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
CStation::CStation(const std::string& title)
  : m_Title(title)
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
const std::string& CStation::GetTitle() const {
	return m_Title;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
CTrainGenerator& CStation::AddTrain(const std::string& trainTitle,
        unsigned time,
        CStation& targetStation,
        CTrainGenerator::Frequency frequency,
        unsigned targetStationArrival,
        unsigned averageDelay,
        unsigned mainStationArrival,
        unsigned mainStationDeparture)
{
    CTrainGenerator* pTrainGenerator = new CTrainGenerator(trainTitle, *this, targetStation,
            frequency, targetStationArrival, averageDelay, mainStationArrival, mainStationDeparture);

    DBG_LOG("New train generator: " + pTrainGenerator->GetTrainTitle());

    pTrainGenerator->Activate(Time + time);
    return *pTrainGenerator;
}
