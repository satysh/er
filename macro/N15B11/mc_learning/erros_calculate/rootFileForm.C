void rootFileForm(TString ionName = "", TString outDir = "output", Int_t case_n = 1, Int_t run_n = 1,
				Int_t ang = 5)
{
	if (ionName == "")
	{
		cerr << "ERROR: this macro can be using only by script run_sim.sh." << endl;
		return;
	}
	
	TString outFileName;
	outFileName.Form("%s/%s_case_%d.root", outDir.Data(),ionName.Data(), case_n);
	TFile* file = new TFile(outFileName, "recreate");
	for (Int_t i = 0; i < run_n; i++)
	{
		TString readTxtName;
		readTxtName.Form("%s/run_%d_thetas.txt", outDir.Data(), i);
		ifstream fin(readTxtName);
		if (!fin.is_open())
		{
			cerr << "Can't open file " << readTxtName << endl;
			continue;
		}

		TString treeName;
		treeName.Form("run_%d", i);
		TTree* tree = new TTree(treeName, treeName);
		TString branchName;
		branchName.Form("ang_%d", ang);
		Float_t curValue;
		TBranch* curBr = (TBranch*)tree->Branch(branchName, &curValue, "curValue/F");
		while (!fin.eof())
		{
			fin >> curValue;
			tree->Fill();
		}
		tree->Write();
		tree->TTree::ResetBranchAddress(curBr);
		fin.clear();
		fin.close();
	}

	file->Close();
}