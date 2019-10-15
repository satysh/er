/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERDecayRootPhaseSpace_H
#define ERDecayRootPhaseSpace_H

#include "ERDecay.h" // mother class

#include "TString.h"

class TParticlePDG;

class ERDecayRootPhaseSpace : public ERDecay {
public:
  ERDecayRootPhaseSpace(TString name);
  ~ERDecayRootPhaseSpace();

  /*Modifiers*/
  void SetMinStep(Double_t minStep) { fMinStep = minStep; }
  void SetTargetThickness(Double_t targetThickness) { fTargetThickness = targetThickness; }
  void SetDecayVolumeName(TString volName) { fDecayVolumeName = volName; }
  /** @brief Body decay in phase space approach.
  **/
  void PhaseGenerator();
public:
  Bool_t Init();
  Bool_t Stepping();

  void BeginEvent();
  void FinishEvent();

private:
  TString fDecayVolumeName;

  Double_t fMinStep;
  Double_t fTargetThickness;

  // TODO fix this moment 8He is nesessary!
  TParticlePDG* fParticle6He;
  TParticlePDG* fParticle2H;
  TParticlePDG* fParticleNeutron;

  ClassDef(ERDecayRootPhaseSpace, 1)
};

#endif
