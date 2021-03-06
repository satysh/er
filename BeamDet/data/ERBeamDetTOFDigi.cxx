// -------------------------------------------------------------------------
// -----                       ERBeamDetTOFDigi source file                -----
// -----           			   Created   by        			   -----
// -------------------------------------------------------------------------

#include "ERBeamDetTOFDigi.h"
#include<iostream>

using namespace std;

// -----   Default constructor   -------------------------------------------
ERBeamDetTOFDigi::ERBeamDetTOFDigi()
  :fID(-1),
  fEdep(0.)
{
}

ERBeamDetTOFDigi::ERBeamDetTOFDigi(Int_t id, Float_t Edep, Double_t time, Int_t tofNb)
  :fID(id),
  fEdep(Edep),
  fTime(time),
  fToFNb(tofNb)
{
}

ERBeamDetTOFDigi::ERBeamDetTOFDigi(const ERBeamDetTOFDigi& right)
  :fID(right.fID),
  fEdep(right.fEdep),
  fTime(right.fTime),
  fToFNb(right.fToFNb)
{
}



// -----   Destructor   ----------------------------------------------------
ERBeamDetTOFDigi::~ERBeamDetTOFDigi()
{
}

// -----   Public method Print   -------------------------------------------
void ERBeamDetTOFDigi::Print(const Option_t* opt /* = 0 */) const
{
  std::cout << "-I- ERBeamDetTOFDigi:  " << endl;
  std::cout << "    Edep : " << fEdep << endl;
}
// -------------------------------------------------------------------------

ClassImp(ERBeamDetTOFDigi)
