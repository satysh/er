
/** Configuration macro for setting common cuts and processes for G3, G4 and Fluka (M. Al-Turany 27.03.2008)
    specific cuts and processes to g3 or g4 should be set in the g3Config.C, g4Config.C or flConfig.C

*/

void SetCuts()
{
  cout << "SetCuts Macro: Setting Processes.." <<endl;
   
  // ------>>>> IMPORTANT!!!!
  // For a correct comparison between GEANE and MC (pull distributions) 
  // or for a simulation without the generation of secondary particles:
  // 1. set LOSS = 2, DRAY = 0, BREM = 1
  // 2. set the following cut values: CUTGAM, CUTELE, CUTNEU, CUTHAD, CUTMUO = 1 MeV or less
  //                                  BCUTE, BCUTM, DCUTE, DCUTM, PPCUTM = 10 TeV
  // (For an explanation of the chosen values, please refer to the GEANT User's Guide
  // or to message #5362 in the PandaRoot Forum >> Monte Carlo Engines >> g3Config.C thread)
  // 
  // The default settings refer to a complete simulation which generates and follows also the secondary particles.


  gMC->SetProcess("PAIR",1); /** pair production*/
  gMC->SetProcess("COMP",1); /**Compton scattering*/
  gMC->SetProcess("PHOT",1); /** photo electric effect */
  gMC->SetProcess("PFIS",0); /**photofission*/
  gMC->SetProcess("DRAY",0); /**delta-ray*/
  gMC->SetProcess("ANNI",1); /**annihilation*/
  gMC->SetProcess("BREM",1); /**bremsstrahlung*/
  gMC->SetProcess("HADR",5); /**hadronic process*/
  gMC->SetProcess("MUNU",1); /**muon nuclear interaction*/
  gMC->SetProcess("DCAY",1); /**decay*/
  gMC->SetProcess("LOSS",3); /**energy loss*/
  gMC->SetProcess("MULS",1); /**multiple scattering*/
 
  Double_t cut1 = 0.;         // MeV --> 1 keV
  //Double_t cut1 = 1.0E-6;
  Double_t cut2 = 0.;
  //Double_t cut2 = 1.0E-6;         // MeV --> 100 keV
  Double_t cutb = 1.0E4;          // GeV --> 10 TeV
  Double_t tofmax = 1.E10;        // seconds
  cout << "SetCuts Macro: Setting cuts.." <<endl;
  
  gMC->SetCut("CUTGAM",cut2);   /** gammas (GeV)*/
  gMC->SetCut("CUTGAM",cut1);   /** gammas (GeV)*/
  gMC->SetCut("CUTELE",cut1);   /** electrons (GeV)*/
  gMC->SetCut("CUTELE",cut1);   /** electrons (GeV)*/
  gMC->SetCut("CUTNEU",cut1);   /** neutral hadrons (GeV)*/
  gMC->SetCut("CUTHAD",cut1);   /** charged hadrons (GeV)*/
  gMC->SetCut("CUTMUO",cut2);   /** muons (GeV)*/
  gMC->SetCut("BCUTE",cut2);    /** electron bremsstrahlung (GeV)*/
  gMC->SetCut("BCUTM",cut2);    /** muon and hadron bremsstrahlung(GeV)*/ 
  gMC->SetCut("DCUTE",cut2);    /** delta-rays by electrons (GeV)*/
  gMC->SetCut("DCUTM",cut2);    /** delta-rays by muons (GeV)*/
  gMC->SetCut("PPCUTM",cut2);   /** direct pair production by muons (GeV)*/
  gMC->SetCut("TOFMAX",tofmax); /**time of flight cut in seconds*/
  
   
}
