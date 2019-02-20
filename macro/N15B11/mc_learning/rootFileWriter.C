void rootFileWriter(TString ionName = "", Int_t case_n = 0, Int_t BegAng = 0, Int_t EndAng = 0)
{
	if (ionName == "")
	{
		cerr << "ERROR: You have to use this macro by using run.sh script." << endl;
		return;
	}
	TString outFileName;
	outFileName.Form("output/"+ionName+"_case_%d.root", case_n);
	TFile* file = new TFile(outFileName, "recreate");


	// Read text files and tree filling
	for (Int_t i = BegAng; i <= EndAng; i++)
	{
        TString readTxtName;
        readTxtName.Form("input/all_interact_thetas_%d.00000000000000000000.txt", i);
        ifstream fin(readTxtName);
        if (!fin.is_open())
        {
        	cerr << "Can't open file: " << readTxtName << endl;
        	return;
        }

		TString treeName;
		treeName.Form("ang_%d", i);
		TTree* tree = new TTree(treeName, treeName);
        TString branchName;
        branchName.Form("ang_%d", i);
        Float_t curValue;
        TBranch* curBranch = tree->Branch(branchName, &curValue, "curValue/F");
        while (!fin.eof())
        {
        	fin >> curValue;
        	tree->Fill();
        }
        tree->Write();
        tree->TTree::ResetBranchAddress(curBranch);
        fin.clear();
        fin.close();
	}
	file->Close();
}
