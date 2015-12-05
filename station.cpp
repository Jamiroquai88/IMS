#include "station.h"
#include "debug.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
CStation::CStation(const std::string& title)
  : m_Title(title)
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
const std::string& CStation::GetTitle() const
{
	return m_Title;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
CPublicTrainGenerator& CStation::AddTrain(const std::string& trainTitle,
    unsigned time,
    CStation& targetStation,
    CPublicTrainGenerator::Frequency frequency,
    unsigned targetStationArrival,
    unsigned averageDelay,
    bool bStopsInMainStation,
    unsigned mainStationArrival,
    unsigned mainStationDeparture)
{
    CPublicTrainGenerator* pTrainGenerator = new CPublicTrainGenerator(trainTitle, *this, targetStation,
        frequency, time, targetStationArrival, averageDelay, bStopsInMainStation,
        mainStationArrival, mainStationDeparture);

    //DBG_LOG("New train generator: " + pTrainGenerator->GetTrainTitle());

    pTrainGenerator->Activate(time);
    return *pTrainGenerator;
}
