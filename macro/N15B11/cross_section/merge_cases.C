void Draw_theory(TCanvas* cn, TLegend* leg);
void Draw_experiment(TCanvas* cn, TLegend* leg);

void merge_cases(Int_t n = 6, Int_t option = 1, Int_t start = 1)
{
    TCanvas* canv = new TCanvas("canv", "canv", 1000, 800);
    canv->SetLogy();
    TLegend* leg = new TLegend(1., 1., 0.80, 0.80);
    Draw_theory(canv, leg);
    gPad->SetGrid(4, 4);
    //Draw_experiment(canv, leg);
    leg->Draw();
    gPad->Update();
    Double_t** N15theta = new Double_t*[n];
    Double_t** B11theta = new Double_t*[n];
    Double_t** N15sigma = new Double_t*[n];
    Double_t** B11sigma = new Double_t*[n];
    TGraph** simN15Gr = new TGraph*[n]; 
    TGraph** simB11Gr = new TGraph*[n];
    for (Int_t i = start; i < n; i++)
    {
    	if (option == 1)
    	{
	    	cout << i << ": Press any button to continue: " << endl;
    		cin.get();	
    	}
    	
    	TString N15outFileName;
		N15outFileName.Form("cases/case_%d_N15_cross_and_theta.txt", i+1);
		TString B11ouFileName;
		B11ouFileName.Form("cases/case_%d_B11_cross_and_theta.txt", i+1);
		ifstream N15fin(N15outFileName);
		ifstream B11fin(B11ouFileName);
		if (!N15fin.is_open() || !B11fin.is_open())
		{
			cerr << i << ": " << "Can't open input file" << endl;
			return;
		}
		k = 31;
		N15theta[i] = new Double_t[k];
		N15sigma[i] = new Double_t[k];
		for (Int_t j = 0; j < k; j++)
		{
			N15fin >> N15theta[i][j];
			N15fin >> N15sigma[i][j];
			//cout << N15theta[i][j] << " " << N15sigma[i][j] << endl;
		}

    	canv->cd();
		simN15Gr[i] = new TGraph(k, N15theta[i], N15sigma[i]);
    	simN15Gr[i]->Draw("P");
    	simN15Gr[i]->SetMarkerStyle(8);
    	simN15Gr[i]->SetMarkerColor(i+1);

    	TString caseName;
    	caseName.Form("case_%d", i+1);
    	leg->AddEntry(simN15Gr[i], caseName + "_N15", "p");
    	
		B11theta[i] = new Double_t[k];
		B11sigma[i] = new Double_t[k];
		for (Int_t j = 0; j < k; j++)
		{
			B11fin >> B11theta[i][j];
			B11fin >> B11sigma[i][j];
			//cout << B11theta[i][j] << " " << B11sigma[i][j] << endl;
		}

		canv->cd();
		simB11Gr[i] = new TGraph(k, B11theta[i], B11sigma[i]);
    	simB11Gr[i]->Draw("P");
    	simB11Gr[i]->SetMarkerStyle(21);
    	simB11Gr[i]->SetMarkerColor(i+1);

    	leg->AddEntry(simB11Gr[i], caseName + "_B11", "p");
		N15fin.close();
		B11fin.close();
		gPad->Update();
    	leg->Draw();
    }

    canv->cd();
    leg->SetHeader("Log Scale");
    canv->SaveAs("result/all.pdf");
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
      return;
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
}
//-----------------------------------------------------------------------
void Draw_experiment(TCanvas* canv, TLegend* leg)
{
    ifstream fin("input/CS_15N_11B.txt");
    if (!fin.is_open())
    {
        cerr << "input/CS_15N_11B.txt read error\n";
        return ;
    }

    Int_t N15_B11_Points = 66;
    Int_t i;

    TVectorD sigN15_B11(N15_B11_Points);
    TVectorD thetaN15_B11(N15_B11_Points);
    for (i = 0; i < N15_B11_Points; i++)
    {
        Double_t theta;
        Double_t cs;
        fin >> theta;
        fin >> cs;
        thetaN15_B11(i) = theta;
        sigN15_B11(i) = cs;
        //cout << cs << ", " << theta << endl;
    }

    TGraph* N15_B11_Pgr = new TGraph(thetaN15_B11, sigN15_B11);
    canv->cd();
    N15_B11_Pgr->Draw("P");
    N15_B11_Pgr->SetMarkerStyle(22);
    N15_B11_Pgr->SetMarkerColor(8);
    leg->AddEntry(N15_B11_Pgr, "Experimental Points", "p");

    fin.clear();
    fin.close();
}
