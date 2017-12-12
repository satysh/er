/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

// -------------------------------------------------------------------------
// -----                  ERNeuRadDigitizer header file                -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNEURADDIGITIZER_H
#define ERNEURADDIGITIZER_H

#include "FairTask.h" // mother class

#include <vector>

#include "TStopwatch.h"

class TF1;

class ERNeuRadPoint;
class ERNeuRadPhotoElectron;
class ERNeuRadPixelSignal;
class ERNeuRadSetup;

class ERNeuRadDigitizer : public FairTask
{
public:
  /** Default constructor **/
  ERNeuRadDigitizer();

  /** Constructor
  ** verbose: 1 - only standard log print, 2 - print digi information
  **/
  ERNeuRadDigitizer(Int_t verbose);

  /** Destructor **/
  ~ERNeuRadDigitizer();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();

  /** Modifiers **/
  inline void SetPixelJitter(const Double_t PixelJitter) {fPixelJitter = PixelJitter;}
  inline void SetPixelDelay(const Double_t PixelDelay)   {fPixelDelay = PixelDelay;}
  inline void SetScincilationTau(const Double_t tau)     {fScincilationTau = tau;}
  inline void SetUseCrosstalks(const Bool_t use)         {fUseCrosstalks = use;}

  /** Accessors **/
  Int_t PhotoElectronCount() const;
  Int_t PixelSignalCount() const;

protected:

  // Digitization parameters (not really)
  ERNeuRadSetup* fNeuRadSetup;

  // Input arrays
  TClonesArray *fNeuRadPoints;

  // Output arrays
  TClonesArray *fNeuRadPhotoElectron;
  TClonesArray *fNeuRadPixelSignal;

  // Event header variables
  /*Int_t fPECountF;
  Int_t fPECountB;
  Float_t fSumAmplitudeF;
  Float_t fSumAmplitudeB;*/

  // constants
  static const Double_t cSciFiLightYield; // [photons/MeV]
  static const Double_t cSpeedOfLight; // [cm/ns]
  static const Double_t cMaterialSpeedOfLight; // [cm/ns]
  static const Double_t cLightFractionInTotalIntReflection;
  // доля света захватываемая файбером в полное внутренне отражение в каждую сторону.
  static const Double_t cPixelDelay; // [ns]
  static const Double_t cPixelJitter; // [ns]
  static const Double_t cScincilationTau; // [ns]

  // Allow for user params
  Double_t fPixelJitter; // [ns]
  Double_t fPixelDelay; // [ns]
  Double_t fExcessNoiseFactor;
  Double_t fScincilationTau; // [ns]

  // Timers for debugging
  TStopwatch fPhotoElectronsCreatingTimer;
  Double_t fPhotoElectronsCreatingTime;
  TStopwatch fPixelSignalCreatingTimer;
  Double_t fPixelSignalCreatingTime;

  // 
  Bool_t fUseCrosstalks;

protected:

  ERNeuRadPhotoElectron* AddPhotoElectron(Int_t i_point,
                                          Int_t side, // 0 - front (smaller Z), 1 - back (bigger Z)
                                          Double_t lytime,
                                          Double_t cathode_time,
                                          Double_t anode_time,
                                          Int_t photon_count,
                                          Double_t amplitudes);

  virtual ERNeuRadPixelSignal* AddPixelSignal(Int_t iPMT,
                                              Int_t iChannel,
                                              Int_t numberOfPoints,
                                              Int_t side); // 0 - front (smaller Z), 1 - back (bigger Z)

  virtual void PhotoElectronsCreating(Int_t i_point,
                                      ERNeuRadPoint *point,
                                      std::vector<ERNeuRadPhotoElectron* >** pePerPixels,
                                      Int_t side, // 0 - front (smaller Z), 1 - back (bigger Z)
                                      Int_t& sumNphotoElectrons,
                                      Float_t& sumAmplitude);

  virtual void PixelSignalsCreating(Int_t iPMT, 
                                    Int_t iChannel,
                                    std::vector<ERNeuRadPhotoElectron* >** pePerPixels,
                                    Int_t side); // 0 - front (smaller Z), 1 - back (bigger Z)

  Int_t Crosstalks(Int_t pointModule, Int_t pointPixel, Int_t& peModule, Int_t& pePixel);

public:
  /** Single photoeclectron spectrum.
      This analytical function has been obtained by approximating
      the lab measurements of the MAPMT. **/
  static Double_t SPEfunc(Double_t *x, Double_t *par);

private:
  /** Single photoeclectron spectrum. ROOT-function object. **/
  TF1* fSPEfunc;

private:

  virtual void SetParContainers();

  ClassDef(ERNeuRadDigitizer, 1);
};

#endif // ERNEURADDIGITIZER_H
