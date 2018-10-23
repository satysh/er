void eventDisplay()
{
  TString parFileName = "par.root";
  TString simFileName = "sim.root";
  TString outFileName = "eventDisplay.root";

  FairRunAna* fRun = new FairRunAna();
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  parIo1->open(parFileName);
  rtdb->setFirstInput(parIo1);
  //rtdb->print();

  fRun->SetInputFile(simFileName);
  fRun->SetOutputFile(outFileName);

  FairEventManager* fMan = new FairEventManager();
  FairMCTracks* Track = new FairMCTracks ("Monte-Carlo Tracks");
  FairMCPointDraw* Points = new FairMCPointDraw ("Gas_Layer_0_Det_2_Point", kOrange, kFullSquare);


  fMan->AddTask(Track);
  fMan->AddTask(Points);

  fMan->Init();
}
