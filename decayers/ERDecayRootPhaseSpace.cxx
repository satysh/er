/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
// STD
#include <iostream>

// ROOT
#include "TROOT.h" // for gROOT using
#include "TVirtualMC.h" // for gMC using
#include "TVector3.h"
#include "TParticlePDG.h"
#include "TGenPhaseSpace.h"


// FAIRROOT
#include "FairRunSim.h"
#include "FairLogger.h"

// EXPERTROOT
#include "ERDecayRootPhaseSpace.h"

//-------------------------------------------------------------------------------------------------
ERDecayRootPhaseSpace::ERDecayRootPhaseSpace(TString name):
  ERDecay(name)
   {
  LOG(INFO) << "ERDecayRootPhaseSpace::ERDecayRootPhaseSpace(" << name << ")" << FairLogger::endl;
  if (!gROOT->GetClass("TGenPhaseSpace")) gSystem->Load("libPhysics");
  fPhaseSpaceGenerator = new TGenPhaseSpace();
  FairRunSim* run = FairRunSim::Instance();
  fUnstableIon7H = new FairIon("7H",  1, 7, 1);
  fIon3He        = new FairIon("3He", 2, 3, 2);
  run->AddNewIon(fUnstableIon7H);
  run->AddNewIon(fIon3He);
}
//-------------------------------------------------------------------------------------------------
ERDecayRootPhaseSpace::~ERDecayRootPhaseSpace() {
  if (fPhaseSpaceGenerator) {
    delete fPhaseSpaceGenerator;
  }
  if (fUnstableIon7H) {
    delete fUnstableIon7H;
  }
  if (fIon3He) {
    delete fIon3He;
  }
}
//-------------------------------------------------------------------------------------------------
Bool_t ERDecayRootPhaseSpace::Init() {
  LOG(DEBUG) << "ERDecayRootPhaseSpace::Init()" << FairLogger::endl;
  fParticle8He = TDatabasePDG::Instance()->GetParticle("8He");
  if ( ! fParticle8He ) {
    LOG(FATAL) << "Can't find 8He " << FairLogger::endl;
    return kFALSE;
  }
  else {
  	LOG(DEBUG) << "8He was found successful! It's mass is: " << fParticle8He->Mass() << " GeV" << FairLogger::endl;
  }

  fParticle2H = TDatabasePDG::Instance()->GetParticle("Deuteron");
  if ( ! fParticle2H ) {
    LOG(FATAL) << "Can't find 2H " << FairLogger::endl;
    return kFALSE;
  }
  else {
  	LOG(DEBUG) << "2H was found successful! It's mass is: " << fParticle2H->Mass() << " GeV" << FairLogger::endl;
  }

  fParticle3He = TDatabasePDG::Instance()->GetParticle(fIon3He->GetName());
  if ( ! fParticle3He ) {
    LOG(FATAL) << "Can't find 3He " << FairLogger::endl;
    return kFALSE;
  }
  else {
    LOG(DEBUG) << "3He was found successful! It's mass is: " << fParticle3He->Mass() << " GeV" << FairLogger::endl;
  }

  fParticle7H = TDatabasePDG::Instance()->GetParticle(fUnstableIon7H->GetName());
  if ( ! fParticle7H ) {
    LOG(FATAL) << "Can't find 7H " << FairLogger::endl;
    return kFALSE;
  }
  else {
    LOG(DEBUG) << "7H was found successful! It's mass is: " << fParticle7H->Mass() << " GeV" << FairLogger::endl;
  }

  fParticleNeutron = TDatabasePDG::Instance()->GetParticle("neutron");
  if ( ! fParticleNeutron ) {
    LOG(FATAL) << "Can't find neutron" << FairLogger::endl;
    return kFALSE;
  }
  else {
  	LOG(DEBUG) << "neutron was found successful! It's mass is: " << fParticleNeutron->Mass() << " GeV" << FairLogger::endl;
  }


  LOG(INFO) << "In ERDecayRootPhaseSpace::Init() any particles were find successful!" << FairLogger::endl;
  return kTRUE;
}
//-------------------------------------------------------------------------------------------------
Bool_t ERDecayRootPhaseSpace::Stepping() {
 	if ( !fDecayFinish && TString(gMC->CurrentVolName()).Contains(fVolumeName)
 		  	&& gMC->TrackPid() == fParticle8He->PdgCode()) {
    gMC->SetMaxStep(fStep);
 	  TLorentzVector curBeamPos;
	  gMC->TrackPosition(curBeamPos);
    if (curBeamPos.Z() >= fDecayPosZ) {
      std::cout << "[Stepping] " << "ERDecayRootPhaseSpace::Stepping(" << gMC->CurrentVolName() << ")" << std::endl;
      std::cout << "[Stepping] " << "curBeamPos: " << "X = " << curBeamPos.X() << ", Y = " << curBeamPos.Y()
                << ", Z = " << curBeamPos.Z() << ", Time = " << curBeamPos.T() << std::endl;

      TLorentzVector new3HeLV(0., 0., 0., 0.);
      TLorentzVector new7HLV(0., 0., 0., 0.);
      RootPhaseGenerator(new3HeLV, new7HLV); // Filling of the LV's
      Int_t projectileTrackNb = gMC->GetStack()->GetCurrentTrackNumber();
      Int_t He3TrackNb, H7TrackNb;
      gMC->GetStack()->PushTrack(1, projectileTrackNb, fParticle3He->PdgCode(),
                                 new3HeLV.Px(), new3HeLV.Py(), new3HeLV.Pz(),
                                 new3HeLV.E(), curBeamPos.X(), curBeamPos.Y(), curBeamPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, He3TrackNb, fParticle3He->Mass(), 0);
      gMC->GetStack()->PushTrack(1, projectileTrackNb, fParticle7H->PdgCode(),
                                 new7HLV.Px(), new7HLV.Py(), new7HLV.Pz(),
                                 new7HLV.E(), curBeamPos.X(), curBeamPos.Y(), curBeamPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, H7TrackNb, fParticle7H->Mass(), 0);
      fDecayFinish=kTRUE; // It'is impossble to do It here! Why is It so?
      gMC->StopTrack();
      gMC->SetMaxStep(10000.);
    }
 	}
  return kTRUE;
}

//-------------------------------------------------------------------------------------------------

void ERDecayRootPhaseSpace::BeginEvent() {
	fDecayFinish=kFALSE;
	LOG(DEBUG) << "ERDecayRootPhaseSpace::BeginEvent()" << FairLogger::endl;
  fDecayPosZ = fRnd->Uniform(fUniformA, fUniformB);
  LOG(DEBUG) << "Rnd(" << fUniformA << ", " << fUniformB << ") => DecayPosZ is " << fDecayPosZ << FairLogger::endl;
}

//-------------------------------------------------------------------------------------------------
void ERDecayRootPhaseSpace::FinishEvent() {
	LOG(DEBUG) << "ERDecayRootPhaseSpace::FinishEvent()" << FairLogger::endl;
}

//-------------------------------------------------------------------------------------------------
void ERDecayRootPhaseSpace::RootPhaseGenerator(TLorentzVector &outLV1, TLorentzVector &outLV2) {
  TLorentzVector target(0., 0., 0., fParticle2H->Mass());
  TLorentzVector curBeamMom;
  gMC->TrackMomentum(curBeamMom);
  std::cout << "[RootPhaseGenerator] " << "8He mass is " << fParticle8He->Mass() << ", 2H mass is " << fParticle2H->Mass() << std::endl;
  std::cout << "[RootPhaseGenerator] " << "LV of 8He: " << "Px = " << curBeamMom.X() << ", Py = " << curBeamMom.Y()
            << ", Pz = " << curBeamMom.Z() << ", E = " << curBeamMom.T() << " GeV" << std::endl;

  Double_t pM = fParticle8He->Mass();
  Double_t pM2= pM*pM;
  Double_t projectileIonIonT = sqrt(pow(curBeamMom.P(), 2)+pM2) - pM;
  std::cout << "[RootPhaseGenerator] " << "8He T is " << projectileIonIonT << " GeV" << std::endl;

  TLorentzVector W = target + curBeamMom;
  TVector3 BoostVector = W.BoostVector();
  std::cout << "[RootPhaseGenerator] " << "3He mass is " << fParticle3He->Mass() << ", 7H mass is " << fParticle7H->Mass() << std::endl;

  /* Processing */
  Double_t masses[3] = { fParticle8He->Mass()/8., fParticle3He->Mass()/3., fParticle7H->Mass()/7.};
  fPhaseSpaceGenerator->SetDecay(W, 3, masses);
  Double_t weight = fPhaseSpaceGenerator->Generate();
  TLorentzVector* p8HeLV = fPhaseSpaceGenerator->GetDecay(0);
  TLorentzVector* pNew3HeLV = fPhaseSpaceGenerator->GetDecay(1);
  TLorentzVector* pNew7HLV = fPhaseSpaceGenerator->GetDecay(2);
  std::cout << "[RootPhaseGenerator] " << "weight of the reaction is " << weight << std::endl;

  /* Write results Moments to the LV's */
  outLV1 = *p8HeLV + *pNew3HeLV;
  outLV2 = *p8HeLV + *pNew7HLV;

  std::cout << "[RootPhaseGenerator] " << "Before Boost LV of 3He: " << "Px = " << outLV1.X() << ", Py = " << outLV1.Y()
            << ", Pz = " << outLV1.Z() << ", E = " << outLV1.T() << " GeV" << std::endl;
  std::cout << "[RootPhaseGenerator] " << "Before Boost LV of 7H: " << "Px = " << outLV2.X() << ", Py = " << outLV2.Y()
            << ", Pz = " << outLV2.Z() << ", E = " << outLV2.T() << " GeV" << std::endl;

  outLV1.Boost(BoostVector);
  outLV2.Boost(BoostVector);

  std::cout << "[RootPhaseGenerator] " << "After Boost LV of 3He: " << "Px = " << outLV1.X() << ", Py = " << outLV1.Y()
            << ", Pz = " << outLV1.Z() << ", E = " << outLV1.T() << " GeV" << std::endl;
  std::cout << "[RootPhaseGenerator] " << "After Boost LV of 7H: " << "Px = " << outLV2.X() << ", Py = " << outLV2.Y()
            << ", Pz = " << outLV2.Z() << ", E = " << outLV2.T() << " GeV" << std::endl;

  /* Physics checking */
  std::cout << "[RootPhaseGenerator] " << "8He P is " << curBeamMom.P() << std::endl;
  std::cout << "[RootPhaseGenerator] " << "3He + 7H P is " << sqrt(pow(outLV1.P(), 2)+pow(outLV2.P(), 2)) << std::endl;
  std::cout << "[RootPhaseGenerator] " << "8He E is " << curBeamMom.E() << std::endl;
  std::cout << "[RootPhaseGenerator] " << "3He E + 7H E is " << outLV1.E()+outLV2.E() << std::endl;
}

ClassImp(ERDecayRootPhaseSpace)
