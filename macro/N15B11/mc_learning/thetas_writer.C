void thetas_writer(Double_t angle = 0., Double_t endAngle = 0., TString resultDir = "result")
{
/*
    Int_t anglesNumber;
    if (!(cin >> anglesNumber))
    {
        cerr << "anglesNumber read error" << endl;
        return;
    }
    cout << "Curent angle = " << angle << ", n = " << anglesNumber << endl;
*/
    Int_t branchesNumber = (Int_t)(endAngle - angle + 1);
    if (angle > endAngle)
        branchesNumber = 1;
    TString path = TString(gSystem->Getenv("VMCWORKDIR")) + "/macro/N15B11/mc_learning/";
    TString ofilename;
    ofilename.Form("%s%s/thetas.root", path.Data(), resultDir.Data());
    TFile file(ofilename,"recreate");
    if (file.IsZombie())
    {
        cerr << "Can't opened .root file" << endl;
        return;
    }
    cout << "Output *.root file created" << endl;


    Double_t* dataTheta = new Double_t [branchesNumber];
    Int_t i = 0;
    for (i = 0; i < branchesNumber; i++)
    {
        TString treeName;
        treeName.Form("tree_%d", (Int_t)(angle+i));
        TTree* tree = new TTree(treeName, "Thetas");
        cout << "For(" << i << ")" << endl;
        TString readFileName;
        readFileName.Form("interact_thetas_%d.00000000000000000000.txt", (Int_t)(angle+i));
        TString pathRaedFileName = path + "result/thetas/" + readFileName;
        cout << pathRaedFileName << "has been opened" << endl;
        ifstream fin(pathRaedFileName, ios_base::in);
        if (!fin.is_open())
        {
            cerr << "Can't open " << pathRaedFileName << endl;
            return;
        }
        cout << "   Input file" << readFileName << " opened" << endl;
        TString branchName;
        branchName.Form("theta_%d", (Int_t)(angle+i));
        TBranch* br = (TBranch*)tree->Branch(branchName.Data(), &dataTheta[i], TString(branchName + "/D").Data());
        cerr << "data adress: " << &dataTheta[i] << endl;
        Int_t entries = 0;
        while(!fin.eof())
        {
            fin >> dataTheta[i];
            //br->Fill();
            tree->Fill();
            //cerr << dataTheta << endl;
            entries++;
        }
        cout << "   Entries wrote, nubmer: " << entries << endl;
        //tree->Fill();
        fin.clear();
        fin.close();
        //tree->Fill();
        Int_t testInt = br->GetEntries();
        cerr << testInt << endl;
        cout << endl;
        br->Print();
        tree->Write();
    }
}
