#include "ERNeuRadDigiPar.h"

#include "FairParamList.h"
#include "FairDetParIo.h"
#include "FairParIo.h"
#include "FairGenericParAsciiFileIo.h"
#include "FairGenericParRootFileIo.h"
#include "FairParAsciiFileIo.h"
#include "FairParRootFileIo.h"

#include "TString.h"
#include "TMath.h"

#include <iostream>

// -----   Standard constructor   ------------------------------------------
ERNeuRadDigiPar::ERNeuRadDigiPar(const char* name,
    const char* title,
    const char* context)
  : FairParGenericSet(name, title, context),
    fNofPixels(-1),
    fNofModules(-1),
    fUseCrosstalks(kFALSE),
    fRowNofFibers(-1),
    fPixelQuantumEfficiency(NULL),
    fPixelGain(NULL),
    fPixelSigma(NULL),
    fPixelCrosstalks(NULL),
    fFiberCrosstalks(NULL)
{
  std::cout << "ERNeuRadDigiPar" << std::endl;
}

// -----   Destructor   ----------------------------------------------------
ERNeuRadDigiPar::~ERNeuRadDigiPar()
{
  clear();
}

// -----   Public method clear   -------------------------------------------
void ERNeuRadDigiPar::clear()
{
  status = kFALSE;
  resetInputVersions();
}

// -----   Public method print ---------------------------------------
void ERNeuRadDigiPar::print()
{
  fRowNofFibers = (Int_t)TMath::Sqrt(fNofPixels);
  
  std::cout << "*****************************************" << std::endl;
  std::cout << "          ERNeuRadDigiPar                " << std::endl;
  std::cout << "*****************************************" << std::endl;
  std::cout << "   ERNeuRadNofModules: " <<  fNofModules <<  std::endl;
  std::cout << "   ERNeuRadNofPixels: " <<  fNofPixels <<  std::endl;
  std::cout << "   ERNeuRadPixelQuantumEfficiency: " <<  std::endl;
  for (Int_t iFiber = 0; iFiber < fRowNofFibers; iFiber++){
    std::cout << "     ";
    for (Int_t jFiber = 0; jFiber < fRowNofFibers; jFiber++)
      std::cout <<(*fPixelQuantumEfficiency)[iFiber*fRowNofFibers + jFiber] << "\t";
     std::cout << std::endl;
  }
  std::cout << "*****************************************" << std::endl;
  
  std::cout << "   ERNeuRadPixelGain: " <<  std::endl;
  for (Int_t iFiber = 0; iFiber < fRowNofFibers; iFiber++){
    std::cout << "     ";
    for (Int_t jFiber = 0; jFiber < fRowNofFibers; jFiber++)
      std::cout <<(*fPixelGain)[iFiber*fRowNofFibers + jFiber] << "\t";
     std::cout << std::endl;
  }
  std::cout << "*****************************************" << std::endl;
  
  std::cout << "   ERNeuRadPixelSigma: " <<  std::endl;
  for (Int_t iFiber = 0; iFiber < fRowNofFibers; iFiber++){
    std::cout << "     ";
    for (Int_t jFiber = 0; jFiber < fRowNofFibers; jFiber++)
      std::cout <<(*fPixelSigma)[iFiber*fRowNofFibers + jFiber] << "\t";
     std::cout << std::endl;
  }

  std::cout << "*****************************************" << std::endl;
}

//------------------------------------------------------
void ERNeuRadDigiPar::putParams(FairParamList* l)
{ 
}

//------------------------------------------------------
Bool_t ERNeuRadDigiPar::getParams(FairParamList* l)
{
  std::cout << "ERNeuRadDigiPar Filling ..." << std::endl;

  ERNeuRadSetup* setup = ERNeuRadSetup::Instance();
  fNofPixels = setup->RowNofPixels()*setup->RowNofPixels();
  fNofModules = setup->RowNofModules()*setup->RowNofModules();

  std::cout << "fNofPixels " << fNofPixels << std::endl;

  fPixelQuantumEfficiency = new TArrayF(fNofPixels);
  fPixelGain = new TArrayF(fNofPixels);
  fPixelSigma = new TArrayF(fNofPixels),
  fPixelCrosstalks = new TArrayF(fNofPixels*9);
  fFiberCrosstalks = new TArrayF(fNofPixels*9);

  if (!l) { return kFALSE; }
  if ( ! l->fill("ERNeuRadPixelQuantumEfficiency", fPixelQuantumEfficiency) ) { return kFALSE; }
  if ( ! l->fill("ERNeuRadPixelGain", fPixelGain) ) { return kFALSE; }
  if ( ! l->fill("ERNeuRadPixelSigma", fPixelSigma) ) { return kFALSE; }
  if ( ! l->fill("ERNeuRadPixelCrosstalks", fPixelCrosstalks) ) { 
    std::cerr << "ERNeuRadDigiPar: can`t find ERNeuRadPixelCrosstalks" << std::endl;
  } else {
    fUseCrosstalks = kTRUE;
  }
  if ( ! l->fill("ERNeuRadFiberCrosstalks", fFiberCrosstalks) ) { 
    std::cerr << "ERNeuRadDigiPar: can`t find ERNeuRadFiberCrosstalks" << std::endl;
  } else {
    fUseCrosstalks = kTRUE;
  }

  fPixelQuantumEfficiency->Set(fNofPixels);
  fPixelGain->Set(fNofPixels);
  fPixelSigma->Set(fNofPixels),
  fPixelCrosstalks->Set(fNofPixels*9);
  fFiberCrosstalks->Set(fNofPixels*9);
  std::cout << "fPixelQuantumEfficiency " << fPixelQuantumEfficiency->GetSize() << std::endl;
  return kTRUE;
}

//------------------------------------------------------
Bool_t ERNeuRadDigiPar::init(FairParIo* input){
	std::cout << input->getFilename() << std::endl;
  if ( TString(input->getFilename()).Contains(".digi")){
    FairGenericParAsciiFileIo* p=new FairGenericParAsciiFileIo(((FairParAsciiFileIo*)input)->getFile());
	  return p->init(this);
  }
  if ( TString(input->getFilename()).Contains(".root")){ 
    FairGenericParRootFileIo* p=new FairGenericParRootFileIo(((FairParRootFileIo*)input)->getParRootFile());
	  return p->init(this);
  }
  return kFALSE;
}

//------------------------------------------------------
void ERNeuRadDigiPar::Crosstalks(Int_t iPixel, TArrayF& crosstalks) const {
  //Возвращает матрицу три на три. Каждый элемент матрицы - кростолк к соответствующему соседу.
  //Центральая ячейка - вероятность фотонов, которые останутся в волокне.
  //Вне зависимости от того, что написано в файле параметров потом пересчитывается, чтобы суммарная вероятнсть была равна 1  
  crosstalks.Set(9);
  Int_t shift  = iPixel*9;
  for (Int_t i = 0; i < 9; i++){
    crosstalks[i] = (*fPixelCrosstalks)[shift + i]+(*fFiberCrosstalks)[shift + i];
  }
}

ClassImp(ERNeuRadDigiPar)
