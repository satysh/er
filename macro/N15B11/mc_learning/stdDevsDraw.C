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
		curGraph->SetMarkerStyle(8);
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
	TCanvas* canv = new TCanvas("canv", "canv", 1600, 1000);
	mg->Draw("ACP");
	gPad->SetGrid(2, 2);
	gPad->SetFrameBorderSize(5);
	gPad->SetFrameLineWidth(5);
	TAxis* mgX = (TAxis*)mg->GetXaxis();
	TAxis* mgY = (TAxis*)mg->GetYaxis();
	mgX->SetLabelSize(0.05);
	mgY->SetLabelSize(0.05);
	mgX->SetTitle("Theta Lab");
	mgY->SetTitle("StdDev");
	mgX->SetTitleSize(0.04);
	mgY->SetTitleSize(0.04);
	leg->Draw();
	leg->SetLineWidth(5);
	gPad->Update();
	TFile* outFile = new TFile("output/stdDev.root", "recreate");
	canv->Write();
	canv->Close();
	outFile->Close();
}
