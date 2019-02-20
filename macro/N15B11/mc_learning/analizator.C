void analizator(TString ionName = "", Int_t case_n = 0, Int_t BegAng = 0, Int_t EndAng = 0)
{
	if (ionName == "")
	{
		cerr << "You have to use this macro by using run.sh script" << endl;
		return;
	}
	TString outFileName;
	outFileName.Form("output/"+ionName+"_case_%d.root", case_n);
	TFile* file = new TFile(outFileName, "recreate");

	TString treeName;
	treeName.Form("case_%d", case_n);
	TTree* tree = new TTree(treeName, "tree");

	// Read text files and tree filling
	for (Int_t i = BegAng; i <= EndAng; i++)
	{

	}
}