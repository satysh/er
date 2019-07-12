void formula()
{
    ifstream fin;
    TString workDir = "archive/case_9/N15/";
    Double_t fDetThetaWidth = 0.262822833;
    Double_t ratio = 1.3624844;
    Double_t ratio2 = ratio*ratio;
    Double_t Radius = 218.;

    Int_t i = 5;
    Double_t fDetPos = 5.;
    while (fDetPos <= 35.) {
      Double_t rAng = fDetPos*TMath::DegToRad();
      Double_t fDetThetaWRad = fDetThetaWidth*TMath::DegToRad(); // Detectors fDetThetaWRad

      Double_t fTheta1 = TMath::RadToDeg()*acos( -ratio*sin(rAng-fDetThetaWRad)*sin(rAng-fDetThetaWRad)
                    + cos(rAng-fDetThetaWRad)*sqrt(1.-ratio2*sin(rAng-fDetThetaWRad)*sin(rAng-fDetThetaWRad)) );
      Double_t fTheta2 = TMath::RadToDeg()*acos( -ratio*sin(rAng+fDetThetaWRad)*sin(rAng+fDetThetaWRad)
                    + cos(rAng+fDetThetaWRad)*sqrt(1.-ratio2*sin(rAng+fDetThetaWRad)*sin(rAng+fDetThetaWRad)) );

      TString fileName;
      fileName.Form("%sall_interact_thetas_%d.00000000000000000000.txt", workDir.Data(), i);

      fin.open(fileName);
      if (!fin.is_open()) {
        cerr << fileName << " read error!" << endl;
        return;
      }

      Double_t lMin = 1000.;
      Double_t rMax = -1000.;
      Double_t curVar;
      while (!fin.eof()) {
        fin >> curVar;
        if (curVar < lMin) {
          lMin = curVar;
        }
        if (curVar > rMax) {
          rMax = curVar;
        }
      }
      fin.clear();
      fin.close();

      cout << "Detectors position: " << fDetPos << endl;
      cout << " formula.: (min: " << fTheta1 << ", max: " << fTheta2 << ") " << endl;
      cout << " simulat.: (min: " << lMin << ", max: " << rMax << ") " << endl;

      fDetPos += 1.;
      i++;
    }

    return;
}
