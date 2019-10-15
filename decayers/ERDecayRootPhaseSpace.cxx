/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERDecayRootPhaseSpace.h"

#include "FairLogger.h"

ERDecayRootPhaseSpace::ERDecayRootPhaseSpace():
  ERDecay("") {
  LOG(DEBUG) << "ERDecayRootPhaseSpace::ERDecayRootPhaseSpace()" << FairLogger::endl;
}

//-------------------------------------------------------------------------------------------------
ERDecayRootPhaseSpace::~ERDecayRootPhaseSpace() {
}
//-------------------------------------------------------------------------------------------------
Bool_t ERDecayRootPhaseSpace::Init() {
  LOG(DEBUG) << "ERDecayRootPhaseSpace::Init()" << FairLogger::endl;
  return kTRUE;
}

//-------------------------------------------------------------------------------------------------
Bool_t ERDecayRootPhaseSpace::Stepping() {
  LOG(DEBUG) << "ERDecayRootPhaseSpace::Stepping()" << FairLogger::endl;
  return kTRUE;
}

//-------------------------------------------------------------------------------------------------
void ERDecayRootPhaseSpace::BeginEvent() { 
}

//-------------------------------------------------------------------------------------------------
void ERDecayRootPhaseSpace::FinishEvent() {
}

//-------------------------------------------------------------------------------------------------
void ERDecayRootPhaseSpace::PhaseGenerator() {
}

ClassImp(ERDecayRootPhaseSpace)
