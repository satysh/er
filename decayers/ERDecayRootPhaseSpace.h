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

class ERDecayRootPhaseSpace : public ERDecay {

public:
  ERDecayRootPhaseSpace();
  ~ERDecayRootPhaseSpace();

  /*Modifiers*/
  void SetMinStep(Double_t minStep) {;}
  void SetTargetThickness(Double_t targetThickness) {;}

  /** @brief Body decay in phase space approach.
  **/
  void PhaseGenerator();  
public:
  Bool_t Init();
  Bool_t Stepping();

  void BeginEvent();
  void FinishEvent();

private:

  ClassDef(ERDecayRootPhaseSpace,1)
};

#endif