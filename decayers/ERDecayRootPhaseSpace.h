/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERDecayRootPhaseSpace_H
#define ERDecayRootPhaseSpace_H

#include "TString.h"
class TGenPhaseSpace;

class FairIon;

#include "ERDecay.h" // mother class

class TParticlePDG;

class ERDecayRootPhaseSpace : public ERDecay {
public:
  ERDecayRootPhaseSpace(TString name);
  ~ERDecayRootPhaseSpace();

  /*Modifiers*/
  void SetTargetThickness(Double_t targetThickness) { fTargetThickness = targetThickness; }

  /** @brief Body decay in phase space approach.
  **/
  void PhaseGenerator(); // TODO!! Remove or implement this one!
public:
  Bool_t Init();
  Bool_t Stepping();

  void BeginEvent();
  void FinishEvent();

private:
  TGenPhaseSpace* fPhaseSpaceGenerator;
  Double_t fTargetThickness;

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
