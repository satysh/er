void sim_test(Int_t nEvents = 100, Int_t index = 0)
{
  gRandom->SetSeed(index);

  //---------------------Files-----------------------------------------------
  TString outFile = "sim.root";
  TString parFile = "par.root";
  // ------------------------------------------------------------------------

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();

  FairRunSim* run = new FairRunSim();
  run->SetName("TGeant4");              // Transport engine
  run->SetOutputFile(outFile.Data());          // Output file
  run->SetMaterials("N15.media.geo");       // Materials

  FairModule* cave= new ERCave("CAVE");
  cave->SetGeometryFileName("cave.geo");
  run->AddModule(cave);

  EROLVDetector* detector = new EROLVDetector("OVLdetector", kTRUE);
  detector->SetGeometryFileName("OLV_detector.geo.root");
  detector->AddSensetive("vCub");
  detector->SetParticlePDG(2112); // nutron 2112
  run->AddModule(detector);

  Int_t pdgId = 2212; // proton 2212 // electron 11
  Double32_t momentum = 10.;
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, 1);
  boxGen->SetPRange(momentum, momentum);
  boxGen->SetThetaRange(0., 0.); // 0-90
  boxGen->SetPhiRange(0., 0.); // 0-360
  boxGen->SetBoxXYZ(0., 0., 0., 0., -5.); // xmin, ymin, xmax, ymax, z
  primGen->AddGenerator(boxGen);

  primGen->AddGenerator(boxGen);
  run->SetGenerator(primGen);

  run->SetStoreTraj(kTRUE); // kFALSE

  run->Init();

  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();

  // -----   Run simulation  ------------------------------------------------
  run->Run(nEvents);

  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is sim.root" << endl;
  cout << "Parameter file is par.root" << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime
                  << "s" << endl << endl;
}
