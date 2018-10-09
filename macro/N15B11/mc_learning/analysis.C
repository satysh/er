void table_print(Double_t** arr, TString outDir);
//--------------------------------------------------------------------------------------------------------------------------
void analysis(Int_t caseNumBeg = 1, Int_t caseNumEnd = 1, Int_t thetBeg = 5, Int_t thetEnd = 35,
                TString inputDir = "input", TString outDir = "output_analysis")
{
    // arr[case][theta]
    Double_t** arr = new Double_t* [10];
    for(Int_t i = 0; i < 10; i++)
        arr[i] = new Double_t [31];
    for(Int_t i = 0; i < 10; i++)
        for(Int_t j = 0; j < 31; j++)
            arr[i][j] = 0.;

    // For out graphics
    TCanvas* canv = new TCanvas("canv", "canv", 1600, 1000);
    TLegend* leg = new TLegend(1., 1., 0.9, 0.7);
    leg->SetName("leg");
    TMultiGraph* mg = new TMultiGraph();

    // Fill graphics and table
    for(Int_t i = caseNumBeg; i <= caseNumEnd; i++)
    {
        TString rootFileName;
        rootFileName.Form("%s/%d_N.root", inputDir.Data(), i);
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

        Int_t thetNum = thetEnd - thetBeg + 1;
        // Data reading
        TVectorD thetaLab(thetNum);
        TVectorD sigma(thetNum);
        for(Int_t j = thetBeg; j <= thetEnd; j++)
        {
            thetaLab(j-thetBeg) = (Double_t)j;
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
            TCanvas* c1 = new TCanvas("test", "canv");
            c1->cd(1);
            tree->Draw(leafName);
            TH1F* histo = (TH1F*)gPad->GetListOfPrimitives()->FindObject("htemp");
            if(!histo)
            {
                cerr << " Histo in tree " << treeName << " hasn't been drawn" << endl;
                break;
            }
            else
                cout << " Histo in tree " << treeName << " has been drawn successfuly" << endl;

            Double_t curMean = histo->GetMean();
            Double_t curStdDev = histo->GetStdDev();
            cout << " Mean: " << curMean<< endl;
            cout << " StdDev: " << curStdDev << endl;
            c1->Close();
            sigma(j-thetBeg) = curStdDev;
            arr[i-1][j-5] = curStdDev; // fill arr[case][theta]
        }
        canv->cd();
        TString graphName;
        graphName.Form("case_%d", i);
        TGraph* gr = new TGraph(thetaLab, sigma);
        gr->SetName(graphName);
        //gr->Draw("ACP");
        gr->SetMarkerStyle(i);
        gr->SetMarkerColor(i);
        gr->SetLineColor(i);

        mg->Add(gr);
        TString legName;
        legName.Form("Case: %d", i);
        leg->AddEntry(gr, legName, "pl");
        file->Clear();
        file->Close();
    }

    table_print(arr, outDir);

    canv->cd();
    mg->Draw("ACP");
    mg->GetXaxis()->SetTitle("Theta Lab");
    mg->GetYaxis()->SetTitle("StdDev");
    leg->SetHeader("N15 CASES");
    leg->Draw();
    gPad->SetGrid(2, 2);
    TString canvFileName;
    canvFileName.Form("%s/canv.pdf", outDir.Data());
    canv->SaveAs(canvFileName);
    //canv->Close();

    TString outRootFileName = outDir + "/canv.root";
    TFile* outFile = new TFile(outRootFileName, "RECREATE");
    canv->Write();
}
//---------------------------------------------------------------------------------------------------------------
void table_print(Double_t** arr, TString outDir)
{
    // Output file opens
    TString outFileNameWithTable = outDir + "/" + "table.txt";
    ofstream fout(outFileNameWithTable);
    if(!fout.is_open())
    {
        cerr << "Can't open the table.txt" << endl;
        return;
    }
    fout.precision(4);
    fout.width(12);
    fout << "case|";
    for(Int_t i = 1; i < 10; i++)
    {
        fout.width(6);
        fout << i << "|";
    }
    fout << endl;
///////////////////////////////////////////
    fout << "_____|____________|______|______|______|______|______|______|______|______|" << endl;
//////////////////////////////////////////
    fout.width(6);
    fout << "theta|" << endl;
    for(Int_t i = 0; i < 31; i++)
    {
        fout.width(5);
        fout << i+5 << "|";
        for(Int_t j = 0; j < 9; j++)
        {
            if(j == 0)
                fout.width(12);
            else
                fout.width(6);
            fout << arr[j][i] << "|";
        }
        fout << endl;
///////////////////////////////////////////
    fout << "_____|____________|______|______|______|______|______|______|______|______|" << endl;
//////////////////////////////////////////
    }
    for(Int_t i = 0; i < 10; i++)
        delete []arr[i];
}
