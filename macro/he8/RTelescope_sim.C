void RTelescope_sim(Int_t nEvents = 1000){
  //---------------------Files-----------------------------------------------
  TString outFile= "sim.root";
  TString parFile= "par.root";
  // ------------------------------------------------------------------------

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------
 
  // -----   Create simulation run   ----------------------------------------
  FairRunSim* run = new FairRunSim();
  /** Select transport engine
  * TGeant3
  * TGeant4
  **/
  run->SetName("TGeant4");              // Transport engine
  run->SetOutputFile(outFile.Data());          // Output file
  // ------------------------------------------------------------------------
  
  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  // ------------------------------------------------------------------------
  
  // -----   Create media   -------------------------------------------------
  run->SetMaterials("media.geo");       // Materials
  // ------------------------------------------------------------------------

  // -----   Create detectors  ----------------------------------------------	
  FairModule* cave= new ERCave("CAVE");
  cave->SetGeometryFileName("cave.geo");
  run->AddModule(cave);
	
  // ER NeuRad definition
  /* Select verbosity level
   * 1 - only standard logs
   * 2 - Print points after each event
   * 3 - - GEANT Step information
  */
  Int_t verbose = 1;
  ERRTelescope* RTelescope= new ERRTelescope("ERRTelescope", kTRUE,verbose);
  RTelescope->SetGeometryFileName("RTelescope.v2.geo.root");
  run->AddModule(RTelescope);
  // ------------------------------------------------------------------------
	
  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  Int_t pdgId = 2212; // proton  beam
  Double32_t theta1 = 20;  // polar angle distribution
  Double32_t theta2 = 30;
  Double32_t kin_energy = .5; //GeV
  Double_t mass = TDatabasePDG::Instance()->GetParticle(pdgId)->Mass();
  Double32_t momentum = TMath::Sqrt(kin_energy*kin_energy + 2.*kin_energy*mass); //GeV
  FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, 1);
  boxGen->SetThetaRange(theta1, theta2);
  boxGen->SetPRange(momentum, momentum);
  boxGen->SetPhiRange(0, 360);
  boxGen->SetBoxXYZ(0.,0,0.0,0.0,0.);

  primGen->AddGenerator(boxGen);
  run->SetGenerator(primGen);
  // ------------------------------------------------------------------------
	
  //-------Set visualisation flag to true------------------------------------
  run->SetStoreTraj(kTRUE);
	
  //-------Set LOG verbosity  ----------------------------------------------- 
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  
  // -----   Initialize simulation run   ------------------------------------
  run->Init();
  Int_t nSteps = -15000;
  //gMC->SetMaxNStep(nSteps);
	
  // -----   Runtime database   ---------------------------------------------
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();
  // ---------------------------------------------------------
  
  // -----   Run simulation  ------------------------------------------------
  run->Run(nEvents);
  
  // -----   Finish   -------------------------------------------------------
  //neuRad->WriteHistos();
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
