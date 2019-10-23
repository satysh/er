/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
// ROOT
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
  LOG(DEBUG) << "ERDecayRootPhaseSpace::ERDecayRootPhaseSpace(" << name << ")" << FairLogger::endl;
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

  fParticle3He = TDatabasePDG::Instance()->GetParticle(fIon3He->GetName()); 
  if ( ! fParticle3He ) {
    LOG(FATAL) << "Can't find 3He " << FairLogger::endl;
    return kFALSE;
  }
  else {
    LOG(DEBUG) << "3He was found successful! It's mass is: " << fParticle3He->Mass() << " GeV" << FairLogger::endl; 
  }

  fParticle2H = TDatabasePDG::Instance()->GetParticle("Deuteron");
  if ( ! fParticle2H ) {
    LOG(FATAL) << "Can't find 2H " << FairLogger::endl;
    return kFALSE;
  }
  else {
  	LOG(DEBUG) << "2H was found successful! It's mass is: " << fParticle2H->Mass() << " GeV" << FairLogger::endl; 
  }

  fParticleNeutron = TDatabasePDG::Instance()->GetParticle("neutron");
  if ( ! fParticleNeutron ) {
    LOG(FATAL) << "Can't find neutron" << FairLogger::endl;
    return kFALSE;
  }
  else {
  	LOG(DEBUG) << "neutron was found successful! It's mass is: " << fParticleNeutron->Mass() << " GeV" << FairLogger::endl; 
  }

  fParticle7H = TDatabasePDG::Instance()->GetParticle(fUnstableIon7H->GetName()); 
  if ( ! fParticle7H ) {
    LOG(FATAL) << "Can't find 7H " << FairLogger::endl;
    return kFALSE;
  }
  else {
    LOG(DEBUG) << "7H was found successful! It's mass is: " << fParticle7H->Mass() << " GeV" << FairLogger::endl; 
  }

  LOG(INFO) << "In ERDecayRootPhaseSpace::Init() any particles were find successful!" << FairLogger::endl;
  return kTRUE;
}
//-------------------------------------------------------------------------------------------------
Bool_t ERDecayRootPhaseSpace::Stepping() {
  //TLorentzVector target(0., 0., 0., 0.);
 	if ( !fDecayFinish && TString(gMC->CurrentVolName()).Contains(fVolumeName) 
 		  	&& gMC->TrackPid() != fParticle8He->PdgCode()) {
    gMC->SetMaxStep(fStep);
 	  TLorentzVector curBeamPos;
	  gMC->TrackPosition(curBeamPos);
    fDecayPosZ = 0.; // TODO fix this one! Why It I can't set this one in macro?
    LOG(DEBUG) << "curBeamPosZ is " << curBeamPos.Z() << ", and fDecayPosZ is " << fDecayPosZ << FairLogger::endl;
    if (curBeamPos.Z() >= fDecayPosZ) {
      LOG(DEBUG) << "ERDecayRootPhaseSpace::Stepping(" << gMC->CurrentVolName() << ")" << FairLogger::endl;
      LOG(DEBUG) << "  curBeamPosZ is " << curBeamPos.Z() << FairLogger::endl;
      TLorentzVector target(0., 0., 0., fParticle2H->Mass());
      TLorentzVector curBeamMom;
      gMC->TrackMomentum(curBeamMom);
      TLorentzVector W = target + curBeamMom;
      TVector3 BoostVector = W.BoostVector();
      Double_t masses[2] = { fParticle3He->Mass(), fParticle7H->Mass() };

      /* Processing */
      fPhaseSpaceGenerator->SetDecay(W, 2, masses);
      Double_t weight = fPhaseSpaceGenerator->Generate();

      TLorentzVector* pNew3HeLV = fPhaseSpaceGenerator->GetDecay(0);
      TLorentzVector* pNew7HLV = fPhaseSpaceGenerator->GetDecay(1);

      pNew3HeLV->Boost(BoostVector);
      pNew7HLV->Boost(BoostVector);

      TLorentzVector new3HeLV = *pNew3HeLV;
      TLorentzVector new7HLV = *pNew7HLV;
      
      AddParticleToStack(fParticle3He->PdgCode(), curBeamPos, new3HeLV);
      AddParticleToStack(fParticle7H->PdgCode(), curBeamPos, new7HLV);

      fDecayFinish=kTRUE;
      gMC->SetMaxStep(10000.);
/*
      FairRunSim* run = FairRunSim::Instance();
      if (TString(run->GetMCEventHeader()->ClassName()).Contains("ERDecayMCEventHeader")){   
        ERDecayMCEventHeader* header = (ERDecayMCEventHeader*)run->GetMCEventHeader();
        header->Clear();
      }
*/
    }
 	}
  return kTRUE;
}

//-------------------------------------------------------------------------------------------------
/*
void ERDecayRootPhaseSpace::BeginEvent() {
	fDecayFinish=kFALSE;
	LOG(DEBUG) << "ERDecayRootPhaseSpace::BeginEvent()" << FairLogger::endl;
}
*/
//-------------------------------------------------------------------------------------------------
/*
void ERDecayRootPhaseSpace::FinishEvent() {
	LOG(DEBUG) << "ERDecayRootPhaseSpace::FinishEvent()" << FairLogger::endl;
	fDecayFinish=kTRUE;
	//gMC->StopTrack();
  gMC->SetMaxStep(10000.);
	FairRunSim* run = FairRunSim::Instance();
  if (TString(run->GetMCEventHeader()->ClassName()).Contains("ERDecayMCEventHeader")){   
    ERDecayMCEventHeader* header = (ERDecayMCEventHeader*)run->GetMCEventHeader();
    header->Clear();
  }
}
*/
//-------------------------------------------------------------------------------------------------
void ERDecayRootPhaseSpace::PhaseGenerator() {
}

ClassImp(ERDecayRootPhaseSpace)
