// STD
#include <iostream>

// EXPERTROOT
#include "EROLVDetector.h"

#include "ERDetector.h"

#include "TGeoMatrix.h"
#include "TVirtualMC.h"

#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairRootManager.h"
#include "FairLogger.h"

#include "ERDetectorGeoPar.h"
//-------------------------------------------------------------------------------------------------
EROLVDetector::EROLVDetector()
: ERDetector("EROLVDetector", kTRUE, -1)
{
}
//-------------------------------------------------------------------------------------------------
EROLVDetector::EROLVDetector(const char* Name, Bool_t Active, Int_t DetId/*=0*/)
: ERDetector(Name, Active, DetId)
{
  fParticlePDG = 0;
  fNutronsNum = 0;
}

Bool_t EROLVDetector::ProcessHits(FairVolume* vol)
{
  if (gMC->IsTrackEntering())
  { // Return true if this is the first step of the track in the current volume
    StartNewPoint();
    if (gMC->TrackPid() == fParticlePDG)
    {
      fNutronsNum++;
      std::cout << "nutron: " << fNutronsNum << ", time: " << fTimeIn << std::endl;
    }
  }

  fELoss += gMC->Edep(); // GeV //Return the energy lost in the current step

  if (gMC->IsTrackExiting() || //true if this is the last step of the track in the current volume
      gMC->IsTrackStop()    || //true if the track energy has fallen below the threshold
      gMC->IsTrackDisappeared())
  {
      FinishNewPoint();
  }

  return kTRUE;
}

void EROLVDetector::StartNewPoint() {
  fELoss  = 0.;
  fLightYield = 0.;
  fEventID = gMC->CurrentEvent();
  gMC->TrackPosition(fPosIn);
  gMC->TrackMomentum(fMomIn);
  fTrackID  = gMC->GetStack()->GetCurrentTrackNumber();
  fTimeIn   = gMC->TrackTime() * 1.0e09;  //current time of flight of the track being transported
  fTrackLength = gMC->TrackLength(); //length of the current track from its origin (in cm)
  fMot0TrackID  = gMC->GetStack()->GetCurrentTrack()->GetMother(0);
  fMass = gMC->ParticleMass(gMC->TrackPid()); // GeV/c2
  Int_t curVolId, corOffVolId;
  curVolId = gMC->CurrentVolID(fVolNb);
  TGeoHMatrix matrix;
  gMC->GetTransformation(gMC->CurrentVolPath(), matrix);
  Double_t globalPos[3],localPos[3];
  fPosIn.Vect().GetXYZ(globalPos);
  matrix.MasterToLocal(globalPos,localPos);
  fPosInLocal.SetXYZ(localPos[0],localPos[1],localPos[2]);
}

void EROLVDetector::FinishNewPoint() {
  gMC->TrackPosition(fPosOut);
  gMC->TrackMomentum(fMomOut);
  fTimeOut = gMC->TrackTime() * 1.0e09;

  if (fELoss > 0.)
  {
    TClonesArray* points = fSenVolumes[gMC->CurrentVolName()];
    AddPoint(points);
    fFullEnergy+=fELoss;
    fFullLY+=fLightYield;

  }
}

ERPoint* EROLVDetector::AddPoint(TClonesArray* points) {
  TClonesArray& clref = *points;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERPoint(fEventID, fTrackID, fMot0TrackID, fVolNb,
    fMass, fPosIn.Vect(),fPosInLocal,fPosOut.Vect(),fMomIn.Vect(),fMomOut.Vect(),fTimeIn,
    fTimeOut,fTrackLength, fELoss, fLightYield,gMC->TrackPid(), gMC->TrackCharge());
}
ClassImp(EROLVDetector)
