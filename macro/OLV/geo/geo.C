void geo()
{
    Double_t dx = 150.;
    Double_t dy = 150.;
    Double_t dz = 150.;
    // Create a zero rotation
    TGeoRotation* ggZeroRotation = new TGeoRotation("ggZeroRotation");

    // -------   Load media from media file   -----------------------------------
    FairGeoLoader*    geoLoad = new FairGeoLoader("TGeo","FairGeoLoader");
    FairGeoInterface* geoFace = geoLoad->getGeoInterface();
    TString geoPath = gSystem->Getenv("VMCWORKDIR");
    TString medFile = geoPath + "/geometry/N15.media.geo";
    geoFace->setMediaFile(medFile);
    geoFace->readMedia();
    // --------------------------------------------------------------------------

    // -------   Geometry file name (output)   ----------------------------------
    TString geoFileName = geoPath + "/geometry/OLV_detector.geo.root";
    // --------------------------------------------------------------------------

    // --------------   Create geometry and top volume  -------------------------
    gGeoManager = (TGeoManager*)gROOT->FindObject("FAIRGeom");
    gGeoManager->SetName("DETgeom");
    TGeoVolume* top = new TGeoVolumeAssembly("TOP");
    // --------------------------------------------------------------------------


    // Materials and media -------------------------------------------------------------
    TGeoMaterial* cubMat = new TGeoMaterial("CsI", 1., 1., 1.);
    TGeoMedium* cubMed = new TGeoMedium("cubMed", 1, cubMat);

    // Shapes and volumes ---------------------------------------------------------------

    TGeoBBox* sCub = new TGeoBBox("sCub", dx, dy, dz);
    TGeoVolume* vCub = new TGeoVolume("vCub", sCub, cubMed);

    // Container for 1 detector
    TGeoVolumeAssembly* vDetContainer = new TGeoVolumeAssembly("vDetContainer");

    // Volume hierarchy -----------------------------------------------------------------
    TGeoTranslation* tSemi = new TGeoTranslation("tSemi", 0., 0., 20.);
    vDetContainer->AddNode(vCub, 1);

    top->AddNode(vDetContainer, 1);

    // ---------------   Finish   -----------------------------------------------
    gGeoManager->SetTopVolume(top);
    gGeoManager->CloseGeometry();
    gGeoManager->CheckOverlaps(0.001);
    gGeoManager->PrintOverlaps();
    gGeoManager->CheckGeometry();
    //gGeoManager->CheckGeometryFull();

    //gGeoManager->GetTopVolume()->Draw("ogl");

    TFile* geoFile = new TFile(geoFileName, "RECREATE");
    top->Write();
    geoFile->Close();
    // --------------------------------------------------------------------------
}
