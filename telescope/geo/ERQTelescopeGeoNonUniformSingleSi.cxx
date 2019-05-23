/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERQTelescopeGeoNonUniformSingleSi.h"

#include "TH2D.h"
#include "TGeoBBox.h"
#include "TGeoCompositeShape.h"

#include "FairLogger.h"

#include "ERQTelescopeSetup.h"

using namespace std;

//--------------------------------------------------------------------------------------------------
ERQTelescopeGeoNonUniformSingleSi::ERQTelescopeGeoNonUniformSingleSi() {
}
//--------------------------------------------------------------------------------------------------
ERQTelescopeGeoNonUniformSingleSi::ERQTelescopeGeoNonUniformSingleSi(TString xmlTypeSingleSi, 
                                                                     TString id1,
                                                                     TString xmlTypeSecondSt2, 
                                                                     TString id2, TString plane2,
                                                                     TString pathFullMap,
                                                                     TString pathFrontDeadMap,
                                                                     TString pathBackDeadMap)
: ERGeoComponent(xmlTypeSingleSi/*+"_"+typeFromXML2*/,/*TString("Pseudo_") + */id1/*+"_"+id2*/)
{
  fComponentId1 = id1;
  fComponentId2 = id2;
  fVolumeName += "_X"; // first implementation is for X-strips only
  fPathFullMap = pathFullMap;
  fPathFrontDeadMap = pathFrontDeadMap;
  fPathBackDeadMap = pathBackDeadMap;
  fPlane2 = plane2;
}

//--------------------------------------------------------------------------------------------------
ERQTelescopeGeoNonUniformSingleSi::~ERQTelescopeGeoNonUniformSingleSi() {
}
void ERQTelescopeGeoNonUniformSingleSi::ConstructGeometryVolume(void) {
  ParseXmlParameters();
  // --------------------------------------------------------------------------
  // Create a global translation
  Float_t global_X = 0.;
  Float_t global_Y = 0.;
  Float_t global_Z = 0.;
  // Create a zero rotation
  TGeoRotation *fZeroRotation = new TGeoRotation();
  fZeroRotation->RotateX(0.);
  fZeroRotation->RotateY(0.);
  fZeroRotation->RotateZ(0.);
  // Create a 90 degree rotation around Z axis
  TGeoRotation *f90ZRotation = new TGeoRotation();
  f90ZRotation->RotateX(0.);
  f90ZRotation->RotateY(0.);
  f90ZRotation->RotateZ(90.);

  TGeoManager*   gGeoMan = NULL;
  // -------   Load media from media file   -----------------------------------
  FairGeoLoader* geoLoad = FairGeoLoader::Instance();//
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  TString geoPath = gSystem->Getenv("VMCWORKDIR");
  TString medFile = geoPath + "/geometry/media.geo";
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  gGeoMan = gGeoManager;
  // --------------------------------------------------------------------------
  // -------   Geometry file name (output)   ----------------------------------
  TString geoFileName = geoPath + "/geometry/QTelescope.temp.root";
  // --------------------------------------------------------------------------
  // -----------------   Get and create the required media    -----------------
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();
  // ----- Create media for Double Si -----------------------------------------
  FairGeoMedium* mDoubleSi;
  TGeoMedium*    pMed; 
  mDoubleSi = geoMedia->getMedium(fMedia);

  if ( ! mDoubleSi ) Fatal("Main", "Medium for DoubleSi not found");
  geoBuild->createMedium(mDoubleSi);
  pMed = gGeoMan->GetMedium(fMedia);
  if ( ! pMed ) Fatal("Main", "Medium for DoubleSi not found");
  LOG(DEBUG) << "Created double Si media" << FairLogger::endl;

  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  // ----------------- DoubleSi -----------------------------------------------
  // fVolume = new TGeoVolumeAssembly(fName);
  vector<TGeoVolume*> stripAss;
  vector<TGeoVolume*> strip;
  // vector<TGeoVolume*> boxSens;
  vector<TGeoBBox*> boxSens;
  vector<TGeoVolume*> boxDeadFront;
  vector<TGeoVolume*> boxDeadBack;
  fVolume = new TGeoVolumeAssembly(this->GetVolumeName());
  //------------------ Silicon strip   ---------------------------------------
  // Double_t stripX = fSensX / fStripPlaneCnt_1;
  // Double_t stripY = fSensY;
  Double_t boxX = fSensX / fStripPlaneCnt_1;  
  Double_t boxY = fSensY / fStripPlaneCnt_2; 
  // read thikness maps
  auto file = TFile::Open(fPathFullMap.Data());
  auto histInhom = (TH2D*)file->Get(file->GetListOfKeys()->At(0)->GetName());
  file = TFile::Open(fPathFrontDeadMap.Data());
  auto histFrontDead = (TH2D*)file->Get(file->GetListOfKeys()->At(0)->GetName());
  file = TFile::Open(fPathBackDeadMap.Data());
  auto histBackDead = (TH2D*)file->Get(file->GetListOfKeys()->At(0)->GetName());
  
  for (Int_t iStripX = 0; iStripX < fStripPlaneCnt_1; iStripX++) {
    stripAss.push_back(new TGeoVolumeAssembly("pseudoSiStrip" + fOrientAroundZ + "_" 
                                            + TString::Itoa(iStripX, 10)));
    for (Int_t iStripY = 0; iStripY < fStripPlaneCnt_2; iStripY++) {
      Double_t sensThckss = histInhom->GetBinContent(iStripX + 1, iStripY + 1);
      Double_t deadFrontThckss = histFrontDead->GetBinContent(iStripX + 1, iStripY + 1);
      Double_t deadBackThckss = histBackDead->GetBinContent(iStripX + 1, iStripY + 1);
      LOG(DEBUG) << "ERQTelescopeGeoNonUniformSingleSi::sensThckss " << sensThckss << FairLogger::endl;
      LOG(DEBUG) << "ERQTelescopeGeoNonUniformSingleSi::deadFrontThckss " << deadFrontThckss << FairLogger::endl;
      LOG(DEBUG) << "ERQTelescopeGeoNonUniformSingleSi::deadBackThckss " << deadBackThckss << FairLogger::endl;
      TString sensBoxId = "X" + TString::Itoa(iStripX, 10) + "_Y" + TString::Itoa(iStripY, 10);
      boxSens.push_back(new TGeoBBox(sensBoxId + "SensitivePixelSiBox", boxX / 2, boxY / 2, sensThckss / 2));
      boxDeadFront.push_back(gGeoManager->MakeBox("DeadFrontPixelSiBox_" 
                                                  + fOrientAroundZ + "_X" 
                                                  + TString::Itoa(iStripX, 10) + "_Y" 
                                                  + TString::Itoa(iStripY, 10), 
                                                  pMed, 
                                                  boxX / 2, 
                                                  boxY / 2, 
                                                  deadFrontThckss / 2));
      boxDeadFront.back()->SetLineColor(kRed);
      boxDeadFront.back()->SetTransparency(60);
      boxDeadBack.push_back(gGeoManager->MakeBox("DeadBackPixelSiBox_" 
                                                 + fOrientAroundZ + "_X" 
                                                 + TString::Itoa(iStripX, 10) + "_Y" 
                                                 + TString::Itoa(iStripY, 10), 
                                                 pMed, 
                                                 boxX / 2, 
                                                 boxY / 2, 
                                                 deadBackThckss / 2));
      boxDeadBack.back()->SetLineColor(kGreen);
      boxDeadBack.back()->SetTransparency(60);
    }

    // dividing strip into non-uniform boxes
    TString sumVolsStrip = "";
    vector<TGeoTranslation*> sensBoxTrans;
    for (Int_t iStripY = 0; iStripY < fStripPlaneCnt_2; iStripY++) {
      int boxSensNb = iStripX * fStripPlaneCnt_2 + iStripY;
      Double_t transY = (fSensY / 2) - boxY / 2 - boxY * iStripY;
      TString transName = "tr_" + TString::Itoa(iStripY, 10);
      auto trans = new TGeoTranslation(transName, 0., transY, 0.);
      trans->RegisterYourself();
      sensBoxTrans.push_back(trans);
      sumVolsStrip = sumVolsStrip + boxSens[boxSensNb]->GetName()
                   + ":" + transName + "+";
    }
    sumVolsStrip.Remove(sumVolsStrip.Length() - 1);
    TString stripName = "SensitiveNonUniformSingleSiStrip" 
                      + fOrientAroundZ + "_" 
                      + TString::Itoa(iStripX, 10);
    TGeoCompositeShape *stripBool = new TGeoCompositeShape("stripX", sumVolsStrip.Data());
    strip.push_back(new TGeoVolume(stripName, stripBool, pMed));
    // strip.back()->SetLineColor(kGreen);
    strip.back()->SetTransparency(60);
  }
  //------------------ STRUCTURE  ---------------------------------------------
  //----------------------- Double Si structure -------------------------------
  //------------------ Add fibers to station  along x ----------------------- 
  for (Int_t iStripX = 0; iStripX < fStripPlaneCnt_1; iStripX++) {
    for (Int_t iStripY = 0; iStripY < fStripPlaneCnt_2; iStripY++) {
      Double_t sensThckss = histInhom->GetBinContent(iStripX + 1, iStripY + 1);
      Double_t deadFrontThckss = histFrontDead->GetBinContent(iStripX + 1, iStripY + 1);
      Double_t deadBackThckss = histBackDead->GetBinContent(iStripX + 1, iStripY + 1);
      Int_t boxNb = iStripX * fStripPlaneCnt_2 + iStripY;
      Double_t translateY = (fSensY / 2) 
                          - boxY / 2 - boxY * iStripY;
      Double_t transFrontDeadZ = - (deadFrontThckss + sensThckss) / 2;
      Double_t transBackDeadZ = (deadBackThckss + sensThckss) / 2;
      stripAss[iStripX]->AddNode(boxDeadFront[boxNb], boxNb, 
                              new TGeoCombiTrans(0, translateY, transFrontDeadZ, fZeroRotation));
      // gasVol[i]->AddNode(gasPlane[i], 1, new TGeoCombiTrans(0, 0, -fDistBetweenXandY[i] / 2, fZeroRotation));
      stripAss[iStripX]->AddNode(boxDeadBack[boxNb], boxNb, 
                              new TGeoCombiTrans(0, translateY, transBackDeadZ, fZeroRotation));
    }
    stripAss[iStripX]->AddNode(strip[iStripX], iStripX, 
                               new TGeoCombiTrans(0., 0., 0, fZeroRotation));
  }
  for (Int_t iStripX = 0; iStripX < fStripPlaneCnt_1; iStripX++) {
    Double_t translateX = fSensX / 2 
                        - boxX *(iStripX)-(boxX / 2);
    fVolume->AddNode(stripAss[iStripX], iStripX, new TGeoCombiTrans(translateX, 0, 0, fZeroRotation));
  }
  if (fOrientAroundZ.Contains("Y")) {
    fRotation->RotateZ(90.);
  }
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeGeoNonUniformSingleSi::ParseXmlParameters() {
  TString xmlFile = ERQTelescopeSetup::Instance()->GetXMLParametersFile();
  TDOMParser *domParser;//
  //gROOT->ProcessLine(".O 0");
  domParser = new TDOMParser;
  domParser->SetValidate(false); // do not validate with DTD

  Int_t parsecode = domParser->ParseFile(xmlFile);
  if (parsecode < 0) {
     LOG(FATAL) << domParser->GetParseCodeMessage(parsecode) << FairLogger::FairLogger::endl;
    return ;
  }
  // the first pass trought the xml-database file for the reading of thin detector parameters
  TXMLNode *rootNode = domParser->GetXMLDocument()->GetRootNode();
  TXMLNode *detPartNode = rootNode->GetChildren();
  TXMLNode *SiTypeNodes;
  LOG(DEBUG) << "fComponentId1 " << fComponentId1 << FairLogger::endl;
  LOG(DEBUG) << "fComponentId2 " << fComponentId2 << FairLogger::endl;
  for ( ; detPartNode; detPartNode = detPartNode->GetNextNode()) { // detector's part
    if (!strcasecmp(detPartNode->GetNodeName(), "SiTypes")) {
      SiTypeNodes = detPartNode->GetChildren();
      for ( ; SiTypeNodes; SiTypeNodes = SiTypeNodes->GetNextNode()) {
        if (!strcasecmp(SiTypeNodes->GetNodeName(), "singleSiTypes")) {
          TXMLNode* curNode = SiTypeNodes->GetChildren()->GetNextNode();
          for(; curNode; curNode = curNode->GetNextNode()) {
            TList *attrList;
            TXMLAttr *attr = 0;
            if (curNode->HasAttributes()){
              attrList = curNode->GetAttributes();
              TIter next(attrList);
              while ((attr=(TXMLAttr*)next())) {
                if (!strcasecmp("id", attr->GetName())) { 
                  break;
                }
              }
            }
            else {
              continue;
            }
            if(!strcasecmp(fComponentId1, attr->GetValue())) {
              TXMLNode* curNode2 = curNode->GetChildren();
              for(; curNode2; curNode2 = curNode2->GetNextNode()) {
                // if(!strcasecmp(curNode2->GetNodeName(), "singleSiSize")) {
                //   attrList = curNode2->GetAttributes();
                //   attr = 0;
                //   TIter nextSizeAttr(attrList);
                //   // while ((attr=(TXMLAttr*)nextSizeAttr())) {
                //   //   if (!strcasecmp("singleSiX", attr->GetName())) {
                //   //     fSizeX = atof(attr->GetValue());
                //   //   }
                //   //   if (!strcasecmp("singleSiY", attr->GetName())) {
                //   //     fSizeY = atof(attr->GetValue());
                //   //   }
                //   //   if (!strcasecmp("singleSiZ", attr->GetName())) {
                //   //     fSizeZ = atof(attr->GetValue());
                //   //   }
                //   // }
                // }
                if(!strcasecmp(curNode2->GetNodeName(), "singleSiSensAreaSize")) {
                  attrList = curNode2->GetAttributes();
                  attr = 0;
                  TIter nextSensSizeAttr(attrList);
                  while ((attr=(TXMLAttr*)nextSensSizeAttr())) {
                    if (!strcasecmp("singleSiSensX", attr->GetName())) {
                      fSensX = atof(attr->GetValue());
                    }
                    if (!strcasecmp("singleSiSensY", attr->GetName())) {
                      fSensY = atof(attr->GetValue());
                    }
                    if (!strcasecmp("singleSiSensZ", attr->GetName())) {
                      fSensZ = atof(attr->GetValue());
                    }
                  }
                }
                if(!strcasecmp(curNode2->GetNodeName(), "stripCount")) {
                  fStripPlaneCnt_1 = atof(curNode2->GetText());
                }
                if(!strcasecmp(curNode2->GetNodeName(), "singleSiMedia")) {
                  fMedia = curNode2->GetText();
                }
              }
            }
          }
        }
      }
    }
  }
  // the second pass trought the xml-database file for the reading of thick detector parameters
  rootNode = domParser->GetXMLDocument()->GetRootNode();
  detPartNode = rootNode->GetChildren();
  for ( ; detPartNode; detPartNode = detPartNode->GetNextNode()) { // detector's part
    if (!strcasecmp(detPartNode->GetNodeName(), "SiTypes")) {
      SiTypeNodes = detPartNode->GetChildren();
      for ( ; SiTypeNodes; SiTypeNodes = SiTypeNodes->GetNextNode()) {
        if (!strcasecmp(SiTypeNodes->GetNodeName(), "doubleSiTypes")) {
          TXMLNode* curNode = SiTypeNodes->GetChildren()->GetNextNode();
          for(; curNode; curNode = curNode->GetNextNode()) {
            TList *attrList;
            TXMLAttr *attr = 0;
            if (curNode->HasAttributes()){
              attrList = curNode->GetAttributes();
              TIter next(attrList);
              while ((attr=(TXMLAttr*)next())) {
                if (!strcasecmp("id", attr->GetName())) { 
                  break;
                }
              }
            }
            else {
              continue;
            }
            if(!strcasecmp(fComponentId2, attr->GetValue())) {
              TXMLNode* curNode2 = curNode->GetChildren();
              for(; curNode2; curNode2 = curNode2->GetNextNode()) {
                if(fPlane2 == "X" && !strcasecmp(curNode2->GetNodeName(), "stripCountX")) {
                  fStripPlaneCnt_2 = atof(curNode2->GetText());
                }
                if(fPlane2 == "Y" && !strcasecmp(curNode2->GetNodeName(), "stripCountY")) {
                  fStripPlaneCnt_2 = atof(curNode2->GetText());
                }
              }
            }
          }
        }
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERGeoComponent)

