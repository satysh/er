/* This macro is provided simulation process for H7 explore experiment. */
void sim(Int_t nEvents = 1, TString outDirName = "", Int_t seedIndex = 0)
{
  gRandom->SetSeed(seedIndex);
  //if (!gROOT->GetClass("TGenPhaseSpace")) gSystem->Load("libPhysics");

  /* Define main path */
  TString vmcWorkDir = gSystem->Getenv("VMCWORKDIR");

  /* Define: path + geometry files names */
  TString paramFileBeamDet = vmcWorkDir + "/db/BeamDet/BeamDetParts.xml";
  TString paramFileQTelescope = vmcWorkDir + "/db/QTelescope/QTelescopeParts.xml";
  TString targetGeoFileName = vmcWorkDir + "/geometry/target.h2.geo.root";
  TString magnetGeoFileName = vmcWorkDir + "/geometry/magnet.geo.root";
  TString gadastGeoFileName = vmcWorkDir + "/geometry/partOfGadast.v1.geo.root";

  /* Define output file names */
  TString outFile= outDirName + "/" + "sim.root";
  TString parFile= outDirName + "/" + "par.root";

  /* Create timer */
  TStopwatch timer;
  timer.Start();

  /* Create simulation run */
  ERRunSim* run = new ERRunSim();
  run->SetName("TGeant4"); // Set simulation engine
  run->SetOutputFile(outFile.Data()); // set output file

  /* Runtime database */
  FairRuntimeDb* rtdb = run->GetRuntimeDb();

  /* Create media */
  run->SetMaterials("media.geo");

//-------------------- Add experimetal modules for simulation ------------
  /* Add cave */
  FairModule* cave= new ERCave("CAVE");
  cave->SetGeometryFileName("cave.geo");
  run->AddModule(cave);

  /* Pramas for BeamDet */
  Double_t BeamDetLToF = 1234.8;     // [cm] 1234.8
  Double_t BeamDetPosZToF = -95.3;  // [cm]
  Double_t BeamDetPosZ1MWPC = -81.5; // [cm]
  Double_t BeamDetPosZ2MWPC = -27.;  // [cm]

  /* Craete Beam Detector geometry */
  ERBeamDetSetup* setupBeamDet = ERBeamDetSetup::Instance();
  setupBeamDet->SetXmlParametersFile(paramFileBeamDet);
  setupBeamDet->AddToF("ToF1", BeamDetPosZToF - BeamDetLToF);
  setupBeamDet->AddToF("ToF2", BeamDetPosZToF);
  setupBeamDet->AddMWPC("MWPC1", BeamDetPosZ1MWPC);
  setupBeamDet->SetMWPCnumberingInvOrderY();
  setupBeamDet->SetMWPCnumberingInvOrderX();
  setupBeamDet->AddMWPC("MWPC2", BeamDetPosZ2MWPC);

  Int_t verbose = 0;
  /* Create BeamDet detector providing point interface */
  ERBeamDet* beamdet= new ERBeamDet("ERBeamDet", kTRUE, verbose);
  run->AddModule(beamdet);

  /* Add target */
  FairModule* target = new ERTarget("targetH2", kTRUE, 1);
  target->SetGeometryFileName(targetGeoFileName);
  run->AddModule(target);


  /* Create Magnet */
  FairModule* magnet = new ERTarget("magnet", 1, kTRUE);
  magnet->SetGeometryFileName(magnetGeoFileName);
  run->AddModule(magnet);

  /* QTelescope SetUp geometry create*/
  ERQTelescopeSetup* setupQTelescope = ERQTelescopeSetup::Instance();
  setupQTelescope->SetXMLParametersFile(paramFileQTelescope);
  setupQTelescope->SetGeoName("QTelescopeTmp");

  /* Create Left Telescope geo */
  Double_t xPos, yPos, zPos;
  Double_t radius = 23.;
  TVector3 rotationT1(0., 12., 0.);
  xPos = radius * TMath::Sin(rotationT1.Y() * TMath::DegToRad());
  yPos = 0.;
  zPos = radius * TMath::Cos(rotationT1.Y() * TMath::DegToRad());
  ERGeoSubAssembly* assemblyT1 = new ERGeoSubAssembly("Left_telescope", TVector3(xPos, yPos, zPos), rotationT1);
  ERQTelescopeGeoComponentSingleSi* thinT1 = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SQ300",                                                                                 TVector3(0., 0., -5.36), TVector3(), "Y");
  ERQTelescopeGeoComponentDoubleSi* thickT1 = new ERQTelescopeGeoComponentDoubleSi("DoubleSi", "DoubleSi_SQ_L",
                                                                                    TVector3(0., 0., 0.), TVector3(), "X");
  ERQTelescopeGeoComponentCsI* csi1 = new ERQTelescopeGeoComponentCsI("CsI", "CsI_L", TVector3(0., 0., 5.), TVector3());

  assemblyT1->AddComponent(thinT1);
  assemblyT1->AddComponent(thickT1);
  assemblyT1->AddComponent(csi1);

  setupQTelescope->AddSubAssembly(assemblyT1);

  /* Create Right Telescope geo*/
  radius = 29.;
  TVector3 rotationT2(0., -8.27, 0.);
  xPos = radius * TMath::Sin(rotationT2.Y() * TMath::DegToRad());
  yPos = 0.;
  zPos = radius * TMath::Cos(rotationT2.Y() * TMath::DegToRad());
  ERGeoSubAssembly* assemblyT2 = new ERGeoSubAssembly("Right_telescope", TVector3(xPos, yPos, zPos), rotationT2);

  ERQTelescopeGeoComponentDoubleSi* thickT2 = new ERQTelescopeGeoComponentDoubleSi("DoubleSi", "DoubleSi_SQ_R",
                                                                                  TVector3(0., 0., 0.), TVector3(), "X");
  ERQTelescopeGeoComponentCsI* csi2 = new ERQTelescopeGeoComponentCsI("CsI", "CsI_R", TVector3(0., 0., 5.), TVector3());

  assemblyT2->AddComponent(thickT2);
  assemblyT2->AddComponent(csi2);

  setupQTelescope->AddSubAssembly(assemblyT2);


  /* Create QTelescope detector providing points */
  ERQTelescope* qtelescope= new ERQTelescope("ERQTelescope", kTRUE, verbose);
  run->AddModule(qtelescope);

  /* Create Part of Gadast */
  ERGadast* gadast = new ERGadast("PartOfGadast", kTRUE, 1);
  gadast->SetGeometryFileName(gadastGeoFileName);
  run->AddModule(gadast);
//-------------------------------------------------------------------------------

//-------------------- Add projectile genetarors -----------------------------
  /* Create generators collection */
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

  /* Atributes for projectile */
  TString ionName = "6He";
  Double_t  kinE_MevPerNucleon = 40.;
  Int_t Z = 2, A = 6, Q = 2, mult = 1;
  Double32_t kin_energy = kinE_MevPerNucleon * 1e-3 * (Double_t)A; //GeV
  Double_t beamStartPosition = -1600.;  // [cm] Beam start position

  /*Create mix generator */
  ERIonMixGenerator* generator = new ERIonMixGenerator(ionName, Z, A, Q, mult);
  generator->SetKinE(kin_energy);
  //generator->SetPRange(kin_energy, kin_energy);
  generator->SetPSigmaOverP(1);
  //Double32_t sigmaTheta = 0.004*TMath::RadToDeg();
  //generator->SetThetaSigma(0., sigmaTheta);
  generator->SetThetaRange(0., 0.);
  generator->SetPhiRange(0., 0.);
  generator->SetBoxXYZ(0., 0., 0., 0., beamStartPosition);
  generator->SpreadingOnTarget();

  primGen->AddGenerator(generator); // add mix generator to collection
  run->SetGenerator(primGen); // add collection to simulation
//-------------------------------------------------------------------------------
//-------------------- Add Decay -----------------------------------------------
  Double_t targetH2Thickness = 0.4;
  Double_t massH5 = 4.69036244;  // [GeV]
  ERDecayer* decayer = new ERDecayer();
  ERDecayRootPhaseSpace* targetDecay = new ERDecayRootPhaseSpace("6He+2H");
  targetDecay->SetDecayVolumeName("boxST");

  decayer->AddDecay(targetDecay);
  run->SetDecayer(decayer);

  run->SetStoreTraj(kTRUE);

  /* Set LOG verbosity */
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");

  /* Init simulations */
  run->Init();

  /* Runtime database */
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();

  //gMC->SetMaxNStep(nSteps);
  TString outGeoFileName = outDirName + "/" + "exp.setup.root";
  run->CreateGeometryFile(outGeoFileName);

  /* Run simulation */
  run->Run(nEvents);

  /* Finish */
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is sim.root" << endl;
  cout << "Parameter file is par.root" << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime
          << "s" << endl << endl;

  return;
}
