void Draw_theory(TCanvas* cn, TLegend* leg);
void Draw_simulation(TCanvas* cn, TLegend* leg, TString fileName = "");
void Draw_experiment(TCanvas* cn, TLegend* leg);

void merge_cases()
{
    TCanvas* canv = new TCanvas("canv", "canv", 1000, 800);
    canv->SetLogy();
    TLegend* leg = new TLegend(1., 1., 0.80, 0.80);
    Draw_theory(canv, leg);

    gPad->SetGrid(4, 4);
}

// ----------Globals--------------------------------------
TGraph* sigmaGr = NULL;

Double_t Sigma(Double_t *x, Double_t *par)
{
  return sigmaGr->Eval(x[0]);
}
//---------------------------------------------------------
void Draw_theory(TCanvas* cn, TLegend* leg)
{
    TString fileName = "input/nrv.txt";
    std::ifstream f;
    f.open(fileName.Data());
    if (!f.is_open()) {
      cout << "Can't open file " << fileName << endl;
      return kFALSE;
    }

    Int_t anglesNumbers = std::count(std::istreambuf_iterator<char>(f),
                               std::istreambuf_iterator<char>(), '\n');
    f.seekg(0, std::ios::beg);
    TVectorD tet(anglesNumbers);
    TVectorD sigma(anglesNumbers);

    Int_t i = 0;
    while (!f.eof())
    {
      if (i == anglesNumbers) break;
      f >> tet(i) >> sigma(i);
      i++;
    }

    sigmaGr = new TGraph(tet, sigma);

    cn->cd();
    TF1* sigmaFun = new TF1("Cross-Section", Sigma, 4.5, 180., 0);
    sigmaFun->Draw("C");
    sigmaFun->GetXaxis()->SetTitle("theta");
    sigmaFun->GetYaxis()->SetTitle("cross-section");
    sigmaFun->SetLineWidth(4);
    sigmaFun->SetMarkerStyle(8);
    sigmaFun->SetMinimum(1.0e-11);
    leg->AddEntry(sigmaFun, "Theory", "l");
    f.close();
    return kTRUE;
}
