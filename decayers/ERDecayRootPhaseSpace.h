/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERDecayRootPhaseSpace_H
#define ERDecayRootPhaseSpace_H
// ROOT
#include "TString.h"
#include "TLorentzVector.h"
class TGenPhaseSpace;
class TParticlePDG;

// FAIRROOT
class FairIon;

// EXPERTROOT
#include "ERDecay.h" // mother class

class ERDecayRootPhaseSpace : public ERDecay {
public:
  ERDecayRootPhaseSpace(TString name);
  ~ERDecayRootPhaseSpace();

  /*Modifiers*/
  /* They wait to be written! */

public:
  Bool_t Init();
  Bool_t Stepping();

  void BeginEvent();
  void FinishEvent();

private:
  /** @brief Private method. Body decay in phase space approach.
  **/
  void RootPhaseGenerator(TLorentzVector& outLV1, TLorentzVector& outLV2);

  TGenPhaseSpace* fPhaseSpaceGenerator;

  TParticlePDG* fParticle8He;
  TParticlePDG* fParticle2H;
  TParticlePDG* fParticle3He;
  TParticlePDG* fParticle7H;
  TParticlePDG* fParticleNeutron;

  /* New Ions */
  FairIon* fUnstableIon7H;
  FairIon* fIon3He;

  ClassDef(ERDecayRootPhaseSpace, 1)
};

#endif
