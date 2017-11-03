#include "ERRTelescopeRawConverter.h"

#include "TVector3.h"
#include "TGeoMatrix.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TFile.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>

using namespace std;

#include "ERDetectorList.h"

// ----------------------------------------------------------------------------
ERRTelescopeRawConverter::ERRTelescopeRawConverter()
    : FairTask("ER rtelescope convertation"),
    fRTelescope1Si1DigiR(NULL),
    fRTelescope1Si1DigiS(NULL),
    fRTelescope1Si2DigiS(NULL),
    fRTelescope1CsIDigi(NULL),
    fRTelescope2Si1DigiR(NULL),
    fRTelescope2Si1DigiS(NULL),
    fRTelescope2Si2DigiS(NULL),
    fRTelescope2CsIDigi(NULL),
    fAculRaw(NULL),
    fSiCalFile(""),
    fCsICalFile("")
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERRTelescopeRawConverter::ERRTelescopeRawConverter(Int_t verbose)
    : FairTask("ER rtelescope convertation ", verbose),
    fRTelescope1Si1DigiR(NULL),
    fRTelescope1Si1DigiS(NULL),
    fRTelescope1Si2DigiS(NULL),
    fRTelescope1CsIDigi(NULL),
    fRTelescope2Si1DigiR(NULL),
    fRTelescope2Si1DigiS(NULL),
    fRTelescope2Si2DigiS(NULL),
    fRTelescope2CsIDigi(NULL),
    fAculRaw(NULL),
    fSiCalFile(""),
    fCsICalFile("")
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERRTelescopeRawConverter::~ERRTelescopeRawConverter()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERRTelescopeRawConverter::SetParContainers()
{
    // Get run and runtime database
    FairRunAna* run = FairRunAna::Instance();
    if ( ! run ) Fatal("SetParContainers", "No analysis run");

    FairRuntimeDb* rtdb = run->GetRuntimeDb();
    if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERRTelescopeRawConverter::Init()
{
    if (fSiCalFile == "") Fatal("Init", "Silicon calibration file not defined");
    if (fCsICalFile == "") Fatal("Init", "CsI calibration file not defined");
    // Get input array
    FairRootManager* ioman = FairRootManager::Instance();
    if ( ! ioman ) Fatal("Init", "No FairRootManager");

    fRTelescope1Si1DigiS = (TClonesArray*) ioman->GetObject("RTelescope1Si1DigiS");
    if ( ! fRTelescope1Si1DigiS) Fatal("Init", "Can`t find collection RTelescope1Si1DigiS!");

    fRTelescope1Si1DigiR = (TClonesArray*) ioman->GetObject("RTelescope1Si1DigiR");
    if ( ! fRTelescope1Si1DigiR) Fatal("Init", "Can`t find collection RTelescope1Si1DigiR!");
    
    fRTelescope1Si2DigiS = (TClonesArray*) ioman->GetObject("RTelescope1Si2DigiS");
    if ( ! fRTelescope1Si2DigiS) Fatal("Init", "Can`t find collection RTelescope1Si2DigiS!");
    
    fRTelescope1CsIDigi = (TClonesArray*) ioman->GetObject("RTelescope1CsIDigi");
    if ( ! fRTelescope1CsIDigi) Fatal("Init", "Can`t find collection RTelescope1CsIDigi!");

    
    fRTelescope2Si1DigiS = (TClonesArray*) ioman->GetObject("RTelescope2Si1DigiS");
    if ( ! fRTelescope2Si1DigiS) Fatal("Init", "Can`t find collection RTelescope2Si1DigiS!");

    fRTelescope2Si1DigiR = (TClonesArray*) ioman->GetObject("RTelescope2Si1DigiR");
    if ( ! fRTelescope2Si1DigiR) Fatal("Init", "Can`t find collection RTelescope2Si1DigiR!");
    
    fRTelescope2Si2DigiS = (TClonesArray*) ioman->GetObject("RTelescope2Si2DigiS");
    if ( ! fRTelescope2Si2DigiS) Fatal("Init", "Can`t find collection RTelescope2Si2DigiS!");
    
    fRTelescope2CsIDigi = (TClonesArray*) ioman->GetObject("RTelescope2CsIDigi");
    if ( ! fRTelescope2CsIDigi) Fatal("Init", "Can`t find collection RTelescope2CsIDigi!");

    // Register output array fRTelescopeHits
    fAculRaw = new AculRaw();
    ioman->Register("RTelescopeAculRaw.", "RTelescope Acul Raw", fAculRaw, kTRUE);

    GetParameters();

  /*fRTelescopeSetup = ERRTelescopeSetup::Instance();
    fRTelescopeSetup->Print();*/

    return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERRTelescopeRawConverter::Exec(Option_t *opt)
{
    Reset();

    vector<Int_t>               SiDigi;
    vector<Int_t>::iterator     it_Nb;

    TClonesArray *SiBranch = NULL;

    for(Int_t i = 0; i < 6; ++i)
    {
        switch(i)
        {
            case 0: SiBranch = fRTelescope1Si1DigiS;
                    break;
            case 1: SiBranch = fRTelescope1Si1DigiR;
                    break;
            case 2: SiBranch = fRTelescope1Si2DigiS;
                    break;
            case 3: SiBranch = fRTelescope2Si1DigiS;  
                    break;                   
            case 4: SiBranch = fRTelescope2Si1DigiR;
                    break;
            case 5: SiBranch = fRTelescope2Si2DigiS;
                    break;
        }

        for(Int_t iDigi = 0; iDigi < SiBranch->GetEntriesFast(); ++iDigi)
        {
            ERRTelescopeSiDigi *digi = (ERRTelescopeSiDigi *)SiBranch->At(iDigi);
            SiDigi.push_back(iDigi);
        }

        for(it_Nb = SiDigi.begin(); it_Nb < SiDigi.end(); ++it_Nb)
        {
            LOG(DEBUG) << "Silicon digi: " << *it_Nb << FairLogger::endl;

            ERRTelescopeSiDigi *si_digi = (ERRTelescopeSiDigi*)SiBranch->At(*it_Nb);
            Int_t  telescopeNb = si_digi->TelescopeNb();
            Int_t  detectorNb = si_digi->DetectorNb();
            Int_t  Side = si_digi->Side();                   // 1 - sector, 0 - ring
            Int_t  Nb = si_digi->Nb(); 
            Double_t  edep = si_digi->Edep()*1e3;

            Int_t  j = 80*(telescopeNb - 1) + 16*(detectorNb - 1) + 16*(Side == 1 ? 0 : 1) + Nb;
            Int_t  StationNb = fParametersSi[j][0];
            Int_t  ChanelNb = fParametersSi[j][1];
            Double_t  a = fParametersSi[j][3];
            Double_t  b = fParametersSi[j][4];
            Int_t  ACP = (Int_t)((edep - b) / a);

            LOG(DEBUG) << "telescope " << telescopeNb << ", detector " << detectorNb << ", Side " << Side << FairLogger::endl;
            LOG(DEBUG) << "parameter raw " << j << ", a = " << a << ", b = " << b << FairLogger::endl;
            LOG(DEBUG) << "edep " << edep << ", ACP " << ACP << FairLogger::endl;

            fAculRaw->C3[StationNb][ChanelNb] = ACP;
        }

        SiDigi.clear();
    }

    vector<Int_t>    CsIDigi;

    TClonesArray *CsIBranch = NULL;

    for(Int_t i = 0; i < 2; ++i)
    {
        if (!i)
            CsIBranch = fRTelescope1CsIDigi;
        else
            CsIBranch = fRTelescope2CsIDigi;

        for(Int_t iDigi = 0; iDigi < CsIBranch->GetEntriesFast(); ++iDigi)
        {
            ERRTelescopeCsIDigi *digi = (ERRTelescopeCsIDigi *)CsIBranch->At(iDigi);
            CsIDigi.push_back(iDigi);
        }

        for(it_Nb = CsIDigi.begin(); it_Nb < CsIDigi.end(); ++it_Nb)
        {
            LOG(DEBUG) << "CsI digi: " << *it_Nb << FairLogger::endl;
            ERRTelescopeCsIDigi *csi_digi = (ERRTelescopeCsIDigi*)(CsIBranch->At(*it_Nb));
            Int_t telescopeNb = csi_digi->TelescopeNb();
            Int_t Nb = csi_digi->CrystallNB(); 
            Double_t edep = csi_digi->Edep()*1e3;

            Int_t j = 16 * (telescopeNb - 1) + Nb;
            Int_t StationNb = fParametersCsI[j][0];
            Int_t ChanelNb = fParametersCsI[j][1];
            Double_t a = fParametersCsI[j][3];
            Double_t b = fParametersCsI[j][4];
            Double_t p = fParametersCsI[j][5];
            Int_t x0 = 500;
            Int_t N1 = x0; // N1 = x0 = 500 
            Int_t ACP = 0;
            Double_t c2 = a*(x0 - p) / (a*x0 + b);
            Double_t c1 = (a*x0 + b) / pow(x0-p, c2);

            if (edep > 0 && edep < c1 * pow(N1-p, c2)){
                ACP = (Int_t)(pow( edep/c1, c2));
                LOG(DEBUG) << "telescope " << telescopeNb << FairLogger::endl;
                LOG(DEBUG) << "parameter raw " << j << ", c1 = " << c1 << ", b = " << b << FairLogger::endl;
                LOG(DEBUG) << "edep " << edep << ", ACP " << ACP << FairLogger::endl;
            }
            else{
                ACP = (Int_t)((edep - b) / a);
                LOG(DEBUG) << "telescope " << telescopeNb << FairLogger::endl;
                LOG(DEBUG) << "parameter raw " << j << ", a = " << a << ", b = " << b << FairLogger::endl;
                LOG(DEBUG) << "edep " << edep << ", ACP " << ACP << FairLogger::endl;
            }

            fAculRaw->C3[StationNb][ChanelNb] = ACP;


        }

        CsIDigi.clear();
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERRTelescopeRawConverter::GetParameters()
{
    Int_t i = 0;
    ifstream     SiFile;
    ifstream     CsIFile;

    SiFile.open(fSiCalFile);
    if (!SiFile.is_open()) Fatal("GetParameters", "Can`t open file silicon calibration file!");
    CsIFile.open(fCsICalFile);
    if (!CsIFile.is_open()) Fatal("GetParameters", "Can`t open file CsI calibration file!");

    LOG(DEBUG2) << "Get silicon calibration params" << FairLogger::endl;
    while ( !SiFile.eof()) // check it
    {
        SiFile >> fParametersSi[i][0];
        SiFile >> fParametersSi[i][1];
        SiFile >> fParametersSi[i][2];
        SiFile >> fParametersSi[i][3];
        SiFile >> fParametersSi[i][4];
        SiFile >> fParametersSi[i][5];
        LOG(DEBUG2) << fParametersSi[i][0] << " " << fParametersSi[i][1] << " " << fParametersSi[i][2] << FairLogger::endl;
        i++;
    }

    i = 0;
    LOG(DEBUG2) << "Get CsI calibration params" << FairLogger::endl;
    while ( !CsIFile.eof())
    {   
        CsIFile >> fParametersCsI[i][0];
        CsIFile >> fParametersCsI[i][1];
        CsIFile >> fParametersCsI[i][2];
        CsIFile >> fParametersCsI[i][3];
        CsIFile >> fParametersCsI[i][4];
        CsIFile >> fParametersCsI[i][5];
        CsIFile >> fParametersCsI[i][6];
        LOG(DEBUG2) << fParametersCsI[i][0] << " " << fParametersCsI[i][1] << " " << fParametersCsI[i][2] << FairLogger::endl;
        i++;
    }

    SiFile.close();
    CsIFile.close();
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERRTelescopeRawConverter::Reset()
{
    if (fAculRaw) 
        fAculRaw->Delete();
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void ERRTelescopeRawConverter::Finish() 
{ 
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ClassImp(ERRTelescopeRawConverter)
