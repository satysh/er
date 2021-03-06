/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERGeoSubAssembly.h"

#include "ERSetup.h"
#include "ERGeoComponent.h"

using namespace std;
  
std::map<TString, Int_t>* ERGeoSubAssembly::fComponentTypes = new std::map<TString, Int_t>(); 

//--------------------------------------------------------------------------------------------------
ERGeoSubAssembly::ERGeoSubAssembly() {
}
//--------------------------------------------------------------------------------------------------
ERGeoSubAssembly::ERGeoSubAssembly(TString name) 
: TNamed(name, name),
  fComponents(new TObjArray()),
  fComponentNames(new std::vector<TString>()),
  fPosition(new TVector3()),
  fRotation(new TGeoRotation())
{
}
//--------------------------------------------------------------------------------------------------
ERGeoSubAssembly::ERGeoSubAssembly(TString name, TVector3 position, TVector3 rotation)
: TNamed(name, name),
  fComponents(new TObjArray()),
  fComponentNames(new std::vector<TString>()),
  fPosition(new TVector3(position))
{
  fRotation = new TGeoRotation();
  fRotation->RotateX(rotation.X());
  fRotation->RotateY(rotation.Y());
  fRotation->RotateZ(rotation.Z());
}
//--------------------------------------------------------------------------------------------------
ERGeoSubAssembly::~ERGeoSubAssembly() {
}
//--------------------------------------------------------------------------------------------------
void ERGeoSubAssembly::SetPosition(TVector3 position) {
	fPosition = new TVector3(position);
}
//--------------------------------------------------------------------------------------------------
void ERGeoSubAssembly::SetRotation(TVector3 rotation) {
  fRotation = new TGeoRotation();
  fRotation->RotateX(rotation.X());
  fRotation->RotateY(rotation.Y());
  fRotation->RotateZ(rotation.Z());
}
//--------------------------------------------------------------------------------------------------
void ERGeoSubAssembly::AddComponent(TObject* component) {  
  fComponents->AddLast(component); 
}
//--------------------------------------------------------------------------------------------------
void ERGeoSubAssembly::AddComponent(TObject* component, TVector3 position, TVector3 rotation) {
  ((ERGeoComponent*)component)->SetPosition(position);
  ((ERGeoComponent*)component)->SetRotation(rotation);  
  fComponents->AddLast(component);
}
//--------------------------------------------------------------------------------------------------
void ERGeoSubAssembly::ConstructGeometryVolume() {
  fVolume = new TGeoVolumeAssembly(fName);
  TIter itComponent(fComponents);
  ERGeoComponent *component;
  while(component = (ERGeoComponent*)(itComponent.Next())){
    component->SetVolumeNamePrefix(TString(this->GetName()) + "_");
    component->ConstructGeometryVolume();
    TGeoVolume*   volume = component->GetVolume(); 
    TGeoRotation* rotation = component->GetRotation();
    TVector3*     trans = component->GetPosition();
    fComponentNames->push_back(component->GetVolumeName());
    if (fComponentTypes->find(component->GetType()) == fComponentTypes->end()) {
      fComponentTypes->emplace(make_pair(component->GetType(), 0));
    } else {
      fComponentTypes->at(component->GetType())++;
    }
    fVolume->AddNode(volume, fComponentTypes->at(component->GetType()), 
                     new TGeoCombiTrans(trans->X() ,trans->Y(), trans->Z(), rotation)); 
  }
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERGeoSubAssembly)

