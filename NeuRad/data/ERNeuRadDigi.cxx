// -------------------------------------------------------------------------
// -----           ERNeuRadDigi source file                            -----
// -----           Created 11/12/15  by V.Schetinin                    -----
// -------------------------------------------------------------------------

#include "ERNeuRadDigi.h"

#include <iostream>

// -----   Default constructor   -------------------------------------------
ERNeuRadDigi::ERNeuRadDigi()
  :fID(-1),
  fFrontTDC(0.),
  fBackTDC(0.),
  fTDC(0.),
  fQDC(0.),
  fModuleIndex(-1),
  fFiberIndex(-1),
  fSide(-1)
{
}

ERNeuRadDigi::ERNeuRadDigi(Int_t id,
                           Double_t frontTDC,
                           Double_t backTDC,
                           Double_t tdc,
                           Double_t qdc,
                           Int_t bundle,
                           Int_t fiber,
                           Int_t side)
  :fID(id),
  fFrontTDC(frontTDC),
  fBackTDC(backTDC),
  fTDC(tdc),
  fQDC(qdc),
  fModuleIndex(bundle),
  fFiberIndex(fiber),
  fSide(side)
{
}

ERNeuRadDigi::ERNeuRadDigi(const ERNeuRadDigi& right)
  :fID(right.fID),
  fFrontTDC(right.fFrontTDC),
  fBackTDC(right.fBackTDC),
  fTDC(right.fTDC),
  fQDC(right.fQDC),
  fModuleIndex(right.fModuleIndex),
  fFiberIndex(right.fFiberIndex),
  fSide(right.fSide)
{
}

// -----   Destructor   ----------------------------------------------------
ERNeuRadDigi::~ERNeuRadDigi()
{
}

// -----   Public method Print   -------------------------------------------
void ERNeuRadDigi::Print(const Option_t* opt /* = 0 */) const
{
  std::cout << "-I- ERNeuRadDigi:  " << std::endl;
  std::cout << "    Fiber : " << fFiberIndex << std::endl;
  std::cout << "    Module : " << fModuleIndex << std::endl;
  std::cout << "    FrontTDC: " << fFrontTDC << " BackTDC " << fBackTDC << " TDC " << fTDC << std::endl;
  std::cout << "    QDC: " << fQDC << std::endl;
}
// -------------------------------------------------------------------------

ClassImp(ERNeuRadDigi)
