void analysis(Int_t caseNumBeg = 1, Int_t caseNumEnd = 1, TString inputDir = "input", TString dictionaryOffilesNames = "dofn.txt")
{
    TString dofn = inputDir + "/" + dictionaryOffilesNames;
    ifstream fin(dofn);
    if(!fin.is_open())
    {
        cerr << "Can't open the dictionary of files names." << endl;
        return;
    }

    for(Int_t i = caseNumBeg; i <= caseNumEnd; i++)
    {
        // File name reading for curent case i
        TString rootFileName;
        while(!fin.eof())
        {
            TString readName;
            fin >> readName;
            //cout << i << ": " << readName << endl;
            TString caseNum;
            caseNum.Form("%d", i);
            if(readName.Contains(caseNum) && readName.Contains("root"))
            {
                rootFileName = inputDir + "/" + readName;

                cout << "Case: " << i << "\n " << rootFileName << " has been found successfuly" << endl;
                fin.clear();
                break;
            }

            if (fin.eof())
            {
                cerr << "Case: " << i << "\n " << "Can't found file to case " << i << endl;
                fin.clear();
                fin.close();
                return;
            }
        }

        // The root file is opening
        TFile* file = new TFile(rootFileName, "read");
        if (file->IsZombie())
        {
            cerr << "Case: " << i << "\n " << "Can't open " << rootFileName << endl;
            file->Clear();
            file->Close();
            continue;
        }
        else
            cout << " " << rootFileName << " has been opened successfuly" << endl;

        // Data reading
        TVectorD thetaLab(31);
        TVectorD sigma(31);
        for(Int_t j = 5; j <= 35; j++)
        {
            thetaLab(j-5) = (Double_t)j;
            TString treeName;
            treeName.Form("tree_%d", j);
            TTree* tree = (TTree*)file->Get(treeName);
            if(!tree)
            {
                cerr << " Can't found branch " << treeName << endl;
                break;
            }
            else
                cout << " Tree " << treeName << " has been opened successfuly" << endl;
            TString leafName;
            leafName.Form("theta_%d", j);
            TLeaf* leaf = (TLeaf*)tree->FindLeaf(leafName);
            if(!leaf)
            {
                cerr << "  Can't found leaf " << leafName << endl;
                break;
            }
            else
                cout << "  Leaf " << leafName << " has been opened successfuly" << endl;
        }
        file->Clear();
        file->Close();
    }
    fin.clear();
    fin.close();
}
