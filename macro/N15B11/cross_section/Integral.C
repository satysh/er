TGraph* thetaCDFGr = NULL;
TGraph* thetaInvCDFGr = NULL;
Double_t ThetaCDF(Double_t *x, Double_t *par)
{
    return thetaCDFGr->Eval(x[0]);
}

Double_t ThetaInvCDF(Double_t *x, Double_t *par)
{
    return thetaInvCDFGr->Eval(x[0]);
}

void Integral(TString fileName = "", Int_t begin = 5, Int_t end = 35) {
  std::ifstream f;
  f.open(fileName.Data());
  if (!f.is_open()) {
    cerr << "Can't open file " << fileName << endl;
    return ;
  }

  Int_t nPoints = std::count(std::istreambuf_iterator<char>(f),
                         std::istreambuf_iterator<char>(), '\n');
  f.seekg(0, std::ios::beg);
  TVectorD tet(nPoints);
  TVectorD sigma(nPoints);
  cerr << "nPoints = " << nPoints << endl;
  Int_t i = 0;
  while (!f.eof()) {
    if (i == nPoints) break;
    f >> tet(i) >> sigma(i);
    //cerr << i << ": " << tet(i) << "\t" << sigma(i) << endl;
    i++;
  }
  f.close();
  
  thetaCDFGr = new TGraph(tet, sigma);
  thetaInvCDFGr = new TGraph(sigma, tet);

  TF1* fThetaCDF = new TF1("thetaCDF", ThetaCDF, 0., 180., 0);
  TF1* fThetaInvCDF = new TF1("thetaInvCDF", ThetaInvCDF, 0., 1., 0);

  Double_t dStep = 0.001;
  Double_t Integr = 0.;
  Double_t curAng = (Double_t)begin;
  Double_t limit = (Double_t)end;
  while (curAng <= limit) {
    Integr += fThetaCDF->Eval(curAng)*dStep;
    curAng += dStep;
  }
  cout.precision(12);
  cout << fileName << " Integral: " << Integr << endl;
}
