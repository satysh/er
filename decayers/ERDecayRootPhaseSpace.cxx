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
  fDecayVolumeName(""),
  fMinStep(0.),
  fTargetThickness(0.)
   {
  LOG(DEBUG) << "ERDecayRootPhaseSpace::ERDecayRootPhaseSpace(" << name << ")" << FairLogger::endl;
}
//-------------------------------------------------------------------------------------------------
ERDecayRootPhaseSpace::~ERDecayRootPhaseSpace() {
}
//-------------------------------------------------------------------------------------------------
Bool_t ERDecayRootPhaseSpace::Init() {
  LOG(DEBUG) << "ERDecayRootPhaseSpace::Init()" << FairLogger::endl;
  fParticle6He = TDatabasePDG::Instance()->GetParticle("6He"); 
  if ( ! fParticle6He ) {
    LOG(FATAL) << "Can't find 8He " << FairLogger::endl;
    return kFALSE;
  }
  else {
  	LOG(DEBUG) << "8He was found successful! It's mass is: " << fParticle6He->Mass() << " GeV" << FairLogger::endl; 
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

  LOG(INFO) << "In ERDecayRootPhaseSpace::Init() any particles were find successful!" << FairLogger::endl;
  return kTRUE;
}
//-------------------------------------------------------------------------------------------------
Bool_t ERDecayRootPhaseSpace::Stepping() {
  //TLorentzVector target(0., 0., 0., 0.);
 	if ( !fDecayFinish && TString(gMC->CurrentVolName()).Contains(fDecayVolumeName) 
 		  	&& gMC->TrackPid() != fParticle6He->PdgCode()) {
  	LOG(DEBUG) << "ERDecayRootPhaseSpace::Stepping(" << gMC->CurrentVolName() << ")" << FairLogger::endl;
 	  TLorentzVector curBeamPos;
	  gMC->TrackPosition(curBeamPos);
 	  if (curBeamPos.Z() >= 0.) {
  		//gMC->SetMaxStep(fMinStep);
 			TLorentzVector target(0., 0., 0., fParticle2H->Mass());
  	  TLorentzVector curBeamMom;
    	gMC->TrackMomentum(curBeamMom);
    	TLorentzVector W = target + curBeamMom;
    	TVector3 BoostVector = W.BoostVector();
    	Double_t masses[2] = { fParticle6He->Mass(), fParticle2H->Mass() };
 	  }
 	}
  return kTRUE;
}

//-------------------------------------------------------------------------------------------------
void ERDecayRootPhaseSpace::BeginEvent() {
	fDecayFinish=kFALSE;
	LOG(DEBUG) << "ERDecayRootPhaseSpace::BeginEvent()" << FairLogger::endl;
}

//-------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------
void ERDecayRootPhaseSpace::PhaseGenerator() {
}

ClassImp(ERDecayRootPhaseSpace)
