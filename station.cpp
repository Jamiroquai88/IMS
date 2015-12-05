#include "station.h"
#include "public_train.h"
#include "debug.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
CStation::CStation(const std::string& title)
  : m_Title(title)
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
CStation::~CStation()
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

    // decrease the time by the boarding time
    pTrainGenerator->Activate(time - CPublicTrain::GetBoardingTime());
    return *pTrainGenerator;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CStation::Enter(CPublicTrain& train) const
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CStation::Leave(CPublicTrain& train) const
{}
