#include "ERUnpack.h"

#include <iostream>
#include <vector>


#include "FairLogger.h"

#include "DetEventStation.h"
#include "DetMessage.h"

using namespace std;
//--------------------------------------------------------------------------------------------------
ERUnpack::ERUnpack(TString detName):
FairUnpack(0,0,0,0,0),
fDetName(detName),
fInited(kFALSE),
fUnpacked(kFALSE)
{

}
//--------------------------------------------------------------------------------------------------
ERUnpack::~ERUnpack(){

}
//--------------------------------------------------------------------------------------------------
Bool_t ERUnpack::Init(SetupConfiguration* setupConf){
    fSetupConfiguration = setupConf;

    if (fInited)
        return kFALSE;
    else
        fInited = kTRUE;

    return kTRUE;
}
//--------------------------------------------------------------------------------------------------
Bool_t ERUnpack::DoUnpack(Int_t* data, Int_t size){
    if (fUnpacked)
        return kFALSE;
    else
        fUnpacked = kTRUE;

    Reset();

    return kTRUE;
}
//--------------------------------------------------------------------------------------------------
void ERUnpack::Reset(){
    fUnpacked = kFALSE;
    for (auto itCol : fDigiCollections)
        itCol.second->Delete();
}
//--------------------------------------------------------------------------------------------------
void ERUnpack::UnpackAmpTimeStation(DetEventDetector* detEvent, TString ampStation, TString timeStation,
                                    std::map<Int_t, std::pair<Double_t, Double_t> >& valueMap,
                                    Bool_t skipAloneChannels/* = kTRUE*/){
    const std::map<TString, unsigned short> stList = fSetupConfiguration->GetStationList(fDetName);
    TString ampEventElement,timeEventElement;
    ampEventElement.Form("%s_%s",fDetName.Data(),ampStation.Data());
    timeEventElement.Form("%s_%s",fDetName.Data(),timeStation.Data());

    DetEventStation* ampStationEvent = (DetEventStation*)detEvent->GetChild(ampEventElement);
    DetEventStation* timeStationEvent = (DetEventStation*)detEvent->GetChild(timeEventElement);
    if (!ampStationEvent || !timeStationEvent){
        LOG(FATAL) << "Amplitude event element or time event element not found for " << ampStation << FairLogger::endl;
        return;
    }

    TClonesArray* ampMessages = ampStationEvent->GetDetMessages();
    TClonesArray* timeMessages = timeStationEvent->GetDetMessages();

    // just copy TClonesArray to map and vector
    map<Int_t,Double_t> time_map,amp_map;
    vector<Int_t> amp_channels, time_channels;
    for (Int_t iAmpMassage(0); iAmpMassage < ampMessages->GetEntriesFast(); ++iAmpMassage){
        DetMessage* ampMes = (DetMessage*)ampMessages->At(iAmpMassage);
        amp_map[ampMes->GetStChannel()] = ampMes->GetValue();
        amp_channels.push_back(ampMes->GetStChannel());
    }

    for (Int_t iTimeMessage(0); iTimeMessage < timeMessages->GetEntriesFast(); ++iTimeMessage){
        DetMessage* timeMes = (DetMessage*)timeMessages->At(iTimeMessage);
        time_map[timeMes->GetStChannel()] = timeMes->GetValue();
        time_channels.push_back(timeMes->GetStChannel());
    }

    // sort for intersection and difference algorithm
    sort(time_channels.begin(), time_channels.end());
    sort(amp_channels.begin(), amp_channels.end());

    // found intersection in amplitude and time channels
    vector<Int_t> intersection_channels;
    set_intersection(time_channels.begin(), time_channels.end(),
                     amp_channels.begin(), amp_channels.end(),
                     back_inserter(intersection_channels));
    
    // found alone time and amplitude channels
    vector<Int_t> time_dif_channels;
    vector<Int_t> amp_dif_channels;
    set_difference(time_channels.begin(), time_channels.end(),
                   intersection_channels.begin(), intersection_channels.end(), 
                   inserter(time_dif_channels, time_dif_channels.begin()));
    set_difference(amp_channels.begin(), amp_channels.end(),
                   intersection_channels.begin(), intersection_channels.end(), 
                   inserter(amp_dif_channels, amp_dif_channels.begin()));

    LOG(DEBUG) << "Time and amplitude coincidence channels number: " << intersection_channels.size() 
               << FairLogger::endl;
    LOG(DEBUG) << "Alone time channels number: " << time_dif_channels.size() 
               << FairLogger::endl;
    LOG(DEBUG) << "Alone amplitude channels number: " << amp_dif_channels.size() 
               << FairLogger::endl;
    LOG(DEBUG) << "Option skip alone channels is " << skipAloneChannels
               << FairLogger::endl;

    // save it
    for (auto iIntesectionChannel : intersection_channels)
        valueMap[iIntesectionChannel] = make_pair(amp_map[iIntesectionChannel],
                                                  time_map[iIntesectionChannel]);

    if (!skipAloneChannels){
        for (auto iAloneChannel : time_dif_channels)
            valueMap[iAloneChannel] = make_pair(-1.,time_map[iAloneChannel]);
        for (auto iAloneChannel : amp_dif_channels)
            valueMap[iAloneChannel] = make_pair(amp_map[iAloneChannel],-1.);
    }
}
//--------------------------------------------------------------------------------------------------
void ERUnpack::UnpackStation(DetEventDetector* detEvent, TString station, std::map<Int_t,Double_t>& valueMap){
    const std::map<TString, unsigned short> stList = fSetupConfiguration->GetStationList(fDetName);
    Double_t amp = 0.;
    Int_t channel = -1;
    TString eventElementName;
    eventElementName.Form("%s_%s",fDetName.Data(),station.Data());

    DetEventStation* stationEvent = (DetEventStation*)detEvent->GetChild(eventElementName);

    if (!stationEvent){
        LOG(FATAL) << "Event element not found for " << station << FairLogger::endl;
        return;
    }

    TClonesArray* ampMessages = stationEvent->GetDetMessages();

    for (Int_t iAmpMassage(0); iAmpMassage < ampMessages->GetEntriesFast(); ++iAmpMassage){
        DetMessage* ampMes = (DetMessage*)ampMessages->At(iAmpMassage);
        amp = ampMes->GetValue();
        channel = ampMes->GetStChannel();
        valueMap[channel] = amp;
    }
}

ClassImp(ERUnpack)