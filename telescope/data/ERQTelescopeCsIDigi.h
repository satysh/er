/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERQTelescopeCsIDigi_H
#define ERQTelescopeCsIDigi_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMultiLinkedData.h"

class ERQTelescopeCsIDigi : public FairMultiLinkedData
{
 public:
  /** Default constructor **/
  ERQTelescopeCsIDigi();
  
  /** Constructor with arguments
   **/
  ERQTelescopeCsIDigi(Int_t id, Float_t Edep, Double_t time, Int_t wallNb, Int_t blockNb);
                   
  /** Copy constructor **/
  ERQTelescopeCsIDigi(const ERQTelescopeCsIDigi&);

  /** Destructor **/
  virtual ~ERQTelescopeCsIDigi();

  ERQTelescopeCsIDigi& operator=(const ERQTelescopeCsIDigi&) { return *this; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

  /* Accessors */
  Int_t ID () const {return fID;}
  Int_t GetBlockNb() const {return fBlockNb;}
  Float_t GetEdep() const {return fEdep;}
  Double_t GetTime() const {return fTime;}

 protected:
  Int_t     fID;
  Int_t     fWallNb;
  Int_t     fBlockNb;
  Double_t  fTime;
  Float_t   fEdep;
  
  ClassDef(ERQTelescopeCsIDigi,1)

};

#endif
