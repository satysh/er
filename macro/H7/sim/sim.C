/* This macro is provided simulation process for H7 explore experiment. */
void sim(Int_t nEvents = 1, TString outDirName = "")
{
  /* Define main path */
  TString vmcWorkDir = gSystem->Getenv("VMCWORKDIR");

  /* Define: path + geometry files names */
  TString targetGeoFileName = vmcWorkDir + "/geometry/target.h2.geo.root";
  TString gadastGeoFileName = vmcWorkDir + "/geometry/partOfGadast.v1.geo.root";
  TString magnetGeoFileName = vmcWorkDir + "/geometry/magnet.geo.root";

  /* Define output files names */
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

  /* Add detectors */
  FairModule* cave= new ERCave("CAVE");
  cave->SetGeometryFileName("cave.geo");
  run->AddModule(cave);

  /* Add target */
  FairModule* target = new ERTarget("targetH2", kTRUE, 1);
  target->SetGeometryFileName(targetGeoFileName);
  run->AddModule(target);

  /* TODO Here I have to difine BeamDetectot */

  /* Create generators collection */
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

  /* Atributes */
  TString ionName = "6He";
  Double_t  kinE_MevPerNucleon = 40.;
  Int_t Z = 2, A = 6, Q = 2;
  Double32_t kin_energy = kinE_MevPerNucleon * 1e-3 * A; //GeV
  Double_t startZforBeam = -1000.;

  /*Create mix generator */
  ERIonMixGenerator* generator = new ERIonMixGenerator(ionName, Z, A, Q, 1);
  generator->SetKinE(kin_energy);
  generator->SetPSigmaOverP(0);
  Double32_t sigmaTheta = 0.004*TMath::RadToDeg();
  generator->SetThetaSigma(0, 0);
 // generator->SetThetaRange(0., 5.);
 // generator->SetPhiRange(0, 360);
  generator->SetBoxXYZ(0, 0, 0, 0, startZforBeam);
  generator->SpreadingOnTarget();

  primGen->AddGenerator(generator); // add mix generator to collection
  run->SetGenerator(primGen); // add collection to simulation

  run->SetStoreTraj(kTRUE);

  /* Set LOG verbosity */
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

  /* Init simulations */
  run->Init();

  /* Runtime database */
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();

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
