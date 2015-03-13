// -------------------------------------------------------------------------
// -----                        ExpertNeuRad source file               -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------
#include "ExpertNeuRad.h"
#include "FairRootManager.h"
#include "TParticle.h"
#include "TClonesArray.h"
#include "TVirtualMC.h"
#include "TString.h"
using std::cout;
using std::cerr;
using std::endl;


// -----   Default constructor   -------------------------------------------
ExpertNeuRad::ExpertNeuRad() : FairDetector("ExpertNeuRad", kTRUE){
  ResetParameters();
  fNeuRadCollection = new TClonesArray("ExpertNeuRadPoint");
  fPosIndex = 0;
  kGeoSaved = kFALSE;
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 0;
  fVersion = 1;

}
// -------------------------------------------------------------------------



// -----   Standard constructor   ------------------------------------------
ExpertNeuRad::ExpertNeuRad(const char* name, Bool_t active) 
  : FairDetector(name, active) {
  ResetParameters();
  fNeuRadCollection = new TClonesArray("ExpertNeuRadPoint");
  fPosIndex = 0;
  kGeoSaved = kFALSE;
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 0;
  fVersion = 1;
}

ExpertNeuRad::~ExpertNeuRad() {
  if (fNeuRadCollection) {
    fNeuRadCollection->Delete();
    delete fNeuRadCollection;
  }
}

void ExpertNeuRad::Initialize()
{
  FairDetector::Initialize();
}


Bool_t ExpertNeuRad::ProcessHits(FairVolume* vol) {
  if ( gMC->IsTrackEntering() ) { // Return true if this is the first step of the track in the current volume
    fELoss  = 0.;
    fTime   = gMC->TrackTime() * 1.0e09;  // Return the current time of flight of the track being transported
    fLength = gMC->TrackLength(); // Return the length of the current track from its origin (in cm)
    gMC->TrackPosition(fPosIn);
    gMC->TrackMomentum(fMomIn);
  }
    
  fELoss += gMC->Edep() * 1E+6; // keV //Return the energy lost in the current step
  
	if (gMC->IsTrackExiting()    || //Return true if this is the last step of the track in the current volume 
	    gMC->IsTrackStop()       || //Return true if the track energy has fallen below the threshold
	    gMC->IsTrackDisappeared()) 
	{ 
	  fEventID = gMC->CurrentEvent();
      fTrackID  = gMC->GetStack()->GetCurrentTrackNumber();
      fMot0TrackID  = gMC->GetStack()->GetCurrentTrack()->GetMother(0);
      fMass = gMC->ParticleMass(gMC->TrackPid()); // GeV/c2
      
      gMC->TrackPosition(fPosOut);
      gMC->TrackMomentum(fMomOut);
	  
	  if (fELoss > 0.){
      AddHit(fEventID, fTrackID, fMot0TrackID, fMass,
           TVector3(fPosIn.X(),   fPosIn.Y(),   fPosIn.Z()),
           TVector3(fPosOut.X(),  fPosOut.Y(),  fPosOut.Z()),
           TVector3(fMomIn.Px(),  fMomIn.Py(),  fMomIn.Pz()),
           TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
           fTime, fLength, fELoss);
    }
	}
  return kTRUE;
}

// -----   Public method EndOfEvent   -----------------------------------------
void ExpertNeuRad::BeginEvent() {

}


void ExpertNeuRad::EndOfEvent() {

}


// -----   Public method Register   -------------------------------------------
void ExpertNeuRad::Register() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman)
	Fatal("Init", "IO manager is not set");	
  ioman->Register("NeuRadPoints","NeuRad", fNeuRadCollection, kTRUE);
}
// ----------------------------------------------------------------------------

// -----   Public method GetCollection   --------------------------------------
TClonesArray* ExpertNeuRad::GetCollection(Int_t iColl) const {
  if (iColl == 0) 
    return fNeuRadCollection;
  else 
    return NULL;
}
// ----------------------------------------------------------------------------



// -----   Public method Print   ----------------------------------------------
void ExpertNeuRad::Print(Option_t *option) const
{

}
// ----------------------------------------------------------------------------



// -----   Public method Reset   ----------------------------------------------
void ExpertNeuRad::Reset() {
  fNeuRadCollection->Clear();
  ResetParameters();
}
// ----------------------------------------------------------------------------



// -----   Public method CopyClones   -----------------------------------------
void ExpertNeuRad::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {

}

// -----   Private method AddHit   --------------------------------------------
ExpertNeuRadPoint* ExpertNeuRad::AddHit(Int_t eventID, Int_t trackID,
				    Int_t mot0trackID,
				    Double_t mass,
				    TVector3 posIn,
				    TVector3 posOut, TVector3 momIn,
				    TVector3 momOut, Double_t time,
				    Double_t length, Double_t eLoss) {
  TClonesArray& clref = *fNeuRadCollection;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ExpertNeuRadPoint(eventID, trackID, mot0trackID, mass,
					  posIn, posOut, momIn, momOut, time, length, eLoss);
	
}
// -----   Public method ConstructGeometry   ----------------------------------
void ExpertNeuRad::ConstructGeometry() {
  TString fileName = GetGeometryFileName();
  if(fileName.EndsWith(".root")) {
    LOG(INFO) << "Constructing ExpertNeuRad geometry from ROOT file " << fileName.Data() << FairLogger::endl;
    ConstructRootGeometry();
  } else {
    LOG(FATAL) << "Geometry file name is not set" << FairLogger::endl;
  }
}

Bool_t ExpertNeuRad::CheckIfSensitive(std::string name)
{
  TString volName = name;
  if(volName.Contains("module")) {
    return kTRUE;
  }
  return kFALSE;
}

void ExpertNeuRad::ResetParameters() {
  fEventID = fTrackID = -1;
  fMot0TrackID = -1;
  fPosIn.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fPosOut.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fMomIn.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fMomOut.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fTime = fLength = fELoss = fMass = 0;
  fPosIndex = 0;
};


ClassImp(ExpertNeuRad)
