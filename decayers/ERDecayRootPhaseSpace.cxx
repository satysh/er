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
#include "TROOT.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TParticlePDG.h"
#include "TVirtualMC.h"
#include "TGenPhaseSpace.h"


// FAIRROOT
#include "FairRunSim.h"
#include "ERDecayMCEventHeader.h"
#include "FairLogger.h"

// EXPERTROOT
#include "ERDecayRootPhaseSpace.h"

//-------------------------------------------------------------------------------------------------
ERDecayRootPhaseSpace::ERDecayRootPhaseSpace(TString name):
  ERDecay(name),
  fTargetThickness(0.)
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
      TLorentzVector target(0., 0., 0., fParticle2H->Mass());
      TLorentzVector curBeamMom;
      gMC->TrackMomentum(curBeamMom);
      std::cout << "[Stepping] " << "8He mass is " << fParticle8He->Mass() << ", 2H mass is " << fParticle2H->Mass() << std::endl;
      std::cout << "[Stepping] " << "Before Boost Beam Momentum of 8He: " << "Px = " << curBeamMom.X() << ", Py = " << curBeamMom.Y()
                << ", Pz = " << curBeamMom.Z() << ", E = " << curBeamMom.T() << " GeV" << std::endl;
      Double_t pM = fParticle8He->Mass();
      Double_t pM2= pM*pM;
      Double_t projectileIonIonT = sqrt(pow(curBeamMom.P(), 2)+pM2) - pM;
      std::cout << "[Stepping] " << "8He T is " << projectileIonIonT << " GeV" << std::endl;
      TLorentzVector W = target + curBeamMom;
      TVector3 BoostVector = W.BoostVector();
      std::cout << "[Stepping] " << "3He mass is " << fParticle3He->Mass() << ", 7H mass is " << fParticle7H->Mass() << std::endl;

      /* Processing */
      Double_t masses[3] = { fParticle8He->Mass()/8., fParticle3He->Mass()/3., fParticle7H->Mass()/7.};
      fPhaseSpaceGenerator->SetDecay(W, 3, masses);
      Double_t weight = fPhaseSpaceGenerator->Generate();
      std::cout << "[Stepping] " << "weight of the reaction is " << weight << std::endl;

      TLorentzVector* p8HeLV = fPhaseSpaceGenerator->GetDecay(0);
      TLorentzVector* pNew3HeLV = fPhaseSpaceGenerator->GetDecay(1);
      TLorentzVector* pNew7HLV = fPhaseSpaceGenerator->GetDecay(2);

      pNew3HeLV->Boost(BoostVector);
      pNew7HLV->Boost(BoostVector);

      TLorentzVector new3HeLV = *p8HeLV + *pNew3HeLV;
      TLorentzVector new7HLV = *p8HeLV + *pNew7HLV;

      std::cout << "[Stepping] " << "curBeamPos: " << "X = " << curBeamPos.X() << ", Y = " << curBeamPos.Y()
                << ", Z = " << curBeamPos.Z() << ", Time = " << curBeamPos.T() << std::endl;
      std::cout << "[Stepping] " << "After Boost Beam Momentum of 3He: " << "Px = " << new3HeLV.X() << ", Py = " << new3HeLV.Y()
                << ", Pz = " << new3HeLV.Z() << ", E = " << new3HeLV.T() << " GeV" << std::endl;
      std::cout << "[Stepping] " << "After Boost Beam Momentum of 7H: " << "Px = " << new7HLV.X() << ", Py = " << new7HLV.Y()
                << ", Pz = " << new7HLV.Z() << ", E = " << new7HLV.T() << " GeV" << std::endl;

      Int_t projectileTrackNb = gMC->GetStack()->GetCurrentTrackNumber();
      Int_t He3TrackNb, H7TrackNb;
      std::cout << "[Stepping] " << "projectileTrackNb is " << projectileTrackNb << std::endl;

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

      std::cout << "[Stepping] " << "7H trackNb is " << H7TrackNb << std::endl;

      fDecayFinish=kTRUE; // It'is impossble to do It here! Why is It so?
      gMC->StopTrack();
      gMC->SetMaxStep(10000.);
/*
      FairRunSim* run = FairRunSim::Instance();
      if (TString(run->GetMCEventHeader()->ClassName()).Contains("ERDecayMCEventHeader")){
        ERDecayMCEventHeader* header = (ERDecayMCEventHeader*)run->GetMCEventHeader();
        header->SetDecayPos(curBeamPos.Vect());
        header->SetInputIon(projectileTrackNb);
        header->AddOutputParticle(He3TrackNb);
        header->AddOutputParticle(H7TrackNb);
      }
*/
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
/*
	FairRunSim* run = FairRunSim::Instance();

  if (TString(run->GetMCEventHeader()->ClassName()).Contains("ERDecayMCEventHeader")){
    ERDecayMCEventHeader* header = (ERDecayMCEventHeader*)run->GetMCEventHeader();
    header->Clear();
  }
*/
}

//-------------------------------------------------------------------------------------------------
void ERDecayRootPhaseSpace::PhaseGenerator() {
  /* TODO! If It isn't nesessary so It has to get removed */
}

ClassImp(ERDecayRootPhaseSpace)
