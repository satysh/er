void rootFileRead(TString ionName = "", TString outDir = "output", Int_t case_n = 1, Int_t run_n = 1,
				Int_t ang = 5)
{
	if (ionName == "")
	{
		cerr << "ERROR: this macro can be using only by script run_sim.sh." << endl;
		return;
	}
	TString outFileName;
	outFileName.Form("%s/%s_case_%d.root", outDir.Data(),ionName.Data(), case_n);
	TFile* file = new TFile(outFileName, "READ");
	if (file->IsZombie())
	{
		cerr << "Read file error." << endl;
		return;	
	}
	TString outTxtFileName;
	outTxtFileName.Form("%s/stdDevs.txt", outDir.Data());
	ofstream fout(outTxtFileName);
	if (!fout.is_open())
	{
		cerr << "Can't create output txt file." << endl;
		return;
	}
	for (Int_t i = 0; i < run_n; i++)
	{
		TString treeName;
		treeName.Form("run_%d", i);
		TString branchName;
		branchName.Form("ang_%d", ang);
		TTree* tree = (TTree*)file->Get(treeName);
		if (!tree)
		{
			cerr << treeName << " tree can't be found in file " << outFileName << "." << endl;
			return;
		}
		TCanvas* curCanv = new TCanvas("canv1", "canv1");
		curCanv->cd();
		tree->Draw(branchName);
		TH1F* histo = (TH1F*)gPad->GetListOfPrimitives()->FindObject("htemp");;
		if (!histo)
		{
			cerr << "Can't find histogram in tree " << treeName << " which is placed in file " << outFileName << "." << endl;
			return;
		}
		Float_t curStdDev = (Float_t)histo->GetStdDev();
		cout << "curStdDev: " << curStdDev << endl;
		fout << curStdDev << endl;
		curCanv->Close();
	}
	file->Close();
}