/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef EROLVDetector_H
#define EROLVDetector_H

// EXPERTROOT
#include "ERDetector.h"

class EROLVDetector : public ERDetector
{

public:

    EROLVDetector();

    EROLVDetector(const char* Name, Bool_t Active, Int_t DetId=0);
    Int_t GetParticlePDG() { return fParticlePDG; }
    void SetParticlePDG(Int_t pdg_int = 0) { fParticlePDG = pdg_int; }
    virtual ~EROLVDetector() {;}

    virtual Bool_t ProcessHits(FairVolume* vol = 0);
private:
    void StartNewPoint();
    void FinishNewPoint();
    ERPoint* AddPoint(TClonesArray* points);

    Int_t fParticlePDG;
    Int_t fNutronsNum;

    ClassDef(EROLVDetector,1)
};

#endif /* EROLVDetector_H */

