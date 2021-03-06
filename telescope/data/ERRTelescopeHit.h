// -------------------------------------------------------------------------
// -----                        ERRTelescopeHit header file                   -----
// -----                     Created 03/16  by V.Schetinin             -----
// -------------------------------------------------------------------------

#ifndef ERRTelescopeHit_H
#define ERRTelescopeHit_H

#include "FairHit.h"

class ERRTelescopeHit : public FairHit{
private:
	Int_t fID;
	Float_t fEloss;
	Float_t fTime;
public:
	ERRTelescopeHit(){}
	ERRTelescopeHit(Int_t id, Int_t detID, TVector3& pos, TVector3& dpos, Int_t point_index, Float_t eloss, Float_t time);

	Int_t ID() const {return fID;}
	Float_t Eloss() const {return fEloss;}
	Float_t Time() const {return fTime;}


	ClassDef(ERRTelescopeHit, 1)

};

#endif

