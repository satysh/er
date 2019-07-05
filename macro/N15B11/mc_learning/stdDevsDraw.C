Int_t GetMarker(Int_t i);
void stdDevsDraw(TString ionName = "", Int_t startCase = 1, Int_t endCase = 8, Int_t startAng = 5, Int_t endAng = 35)
{
	if (ionName == "")
	{
		cerr << "You have to run this macro by stdRun.sh script." << endl;
		return;
	}

	TMultiGraph* mg = new TMultiGraph();

	//Angles array form
	TVectorF angles(endAng-startAng+1);
	for (Int_t i =0; i < endAng-startAng+1; i++)
		angles(i) = (Float_t)(i+startAng);
	// Legend form
	TLegend* leg = new TLegend(1., 1., 0.9, 0.7);
	leg->SetName("Legend");

	cout << "Ion: " << ionName << endl;
	for (Int_t i = startCase; i < endCase+1; i++)
	{
		// Array for stdDevs for any case
		TVectorF stdDev(endAng-startAng+1);

		cout << "case: " << i << endl;
		TString curFileName;
		curFileName.Form("cases/%s_case_%d.root", ionName.Data(), i);
		TFile* file = new TFile(curFileName, "read");
		if (file->IsZombie())
		{
			cerr << "Can't open file " << curFileName << "." << endl;
			return;
		}

		for (Int_t j = startAng; j < endAng+1; j++)
		{
			cout << " ang: " << setw(2) << j << ", StdDev = ";
			TString curTreeName;
			curTreeName.Form("ang_%d", j);
			TTree* tree = (TTree*)file->Get(curTreeName);
			if (!tree)
			{
				cerr << curTreeName << " can't be open in file " << curFileName << "." << endl;
				return;
			}
			TCanvas* curCanv = new TCanvas("canv1", "canv1");
			curCanv->cd();
			tree->Draw(curTreeName);
			TH1F* histo = (TH1F*)gPad->GetListOfPrimitives()->FindObject("htemp");;
			if (!histo)
			{
				cerr << "Can't find histogram in tree " << curTreeName << " which is placed in file " << curFileName << "." << endl;
				return;
			}
			Float_t curStdDev = (Float_t)histo->GetStdDev();
			cout << curStdDev << endl;
			curCanv->Close();
			stdDev(j-startAng) = curStdDev;
		}
		// Form graph for current case
		TGraph* curGraph = new TGraph(angles, stdDev);
		TString graphName;
		graphName.Form("case_%d", i);
		curGraph->SetName(graphName);
		//curGraph->Draw("ACP");
		curGraph->SetMarkerStyle(GetMarker(i));
		curGraph->SetMarkerColor(i);
		curGraph->SetMarkerSize(1.5);
		curGraph->SetLineColor(i);
		curGraph->SetLineWidth(3);
		mg->Add(curGraph);

		TString legName;
		legName.Form("Case: %d", i);
		leg->AddEntry(curGraph, legName, "pl");

		file->Close();
	}
	// ============ 11B Draw ===============
	for (Int_t i = 0; i < 2; i++) {


	  TVectorF stdDev(endAng-startAng+1);
	  TString curFileName;
	  Int_t num;
	  if (i == 0)
        num = 1;
      else num = 6;
	  curFileName.Form("cases/B11_case_%d.root", num);
	  TFile* file = new TFile(curFileName, "read");
	  if (file->IsZombie())
	  {
	    cerr << "Can't open file " << curFileName << "." << endl;
		return;
	  }

		for (Int_t j = startAng; j < endAng+1; j++)
		{
			cout << " ang: " << setw(2) << j << ", StdDev = ";
			TString curTreeName;
			curTreeName.Form("ang_%d", j);
			TTree* tree = (TTree*)file->Get(curTreeName);
			if (!tree)
			{
				cerr << curTreeName << " can't be open in file " << curFileName << "." << endl;
				return;
			}
			TCanvas* curCanv = new TCanvas("canv1", "canv1");
			curCanv->cd();
			tree->Draw(curTreeName);
			TH1F* histo = (TH1F*)gPad->GetListOfPrimitives()->FindObject("htemp");;
			if (!histo)
			{
				cerr << "Can't find histogram in tree " << curTreeName << " which is placed in file " << curFileName << "." << endl;
				return;
			}
			Float_t curStdDev = (Float_t)histo->GetStdDev();
			cout << curStdDev << endl;
			curCanv->Close();
			stdDev(j-startAng) = curStdDev;
		}
		TGraph* curGraph = new TGraph(angles, stdDev);
		TString graphName;
		graphName.Form("caseB11_%d", i);
		curGraph->SetName(graphName);
		//curGraph->Draw("ACP");
		curGraph->SetMarkerStyle(8);
		curGraph->SetMarkerColor(1);
		curGraph->SetMarkerSize(1.5);
		curGraph->SetLineColor(1);
		curGraph->SetLineWidth(3);
		mg->Add(curGraph);
	}
		// ============ 11B Draw ===============
	TCanvas* canv = new TCanvas("canv", "canv", 1600, 1000);
	mg->Draw("ACP");
	gPad->SetGrid(2, 2);
	gPad->SetFrameBorderSize(5);
	gPad->SetFrameLineWidth(5);
	TAxis* mgX = (TAxis*)mg->GetXaxis();
	TAxis* mgY = (TAxis*)mg->GetYaxis();
	mgX->SetLabelSize(0.05);
	mgY->SetLabelSize(0.05);
	//mgX->SetTitle("Theta Lab");
	mgY->SetTitle("RMS");
	mgX->SetTitleSize(0.04);
	mgY->SetTitleSize(0.04);
	leg->Draw();
	leg->SetHeader(ionName);
	leg->SetLineWidth(5);
	gPad->Update();

	TLatex latex;
    latex.SetTextAlign(12);
    latex.SetTextSize(0.05);
    latex.DrawLatex(33.2, 0.1, "#theta_{Lab}");
    latex.SetTextSize(0.03);
    latex.DrawLatex(35., 0.1, "[deg]");

	TFile* outFile = new TFile("output/stdDev.root", "recreate");
	canv->Write();
	canv->Close();
	outFile->Close();
}

Int_t GetMarker(Int_t i) {
	switch(i) {
		case 1 : return 4;
		case 2 : return 34;
		case 3 : return 23;
		case 4 : return 33;
		case 5 : return 29;
		case 6 : return 20;
		case 7 : return 21;
		case 8 : return 22;
		default : return 8;
	}
}