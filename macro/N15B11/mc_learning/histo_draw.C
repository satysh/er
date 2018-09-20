void histo_draw(Double_t angle = 5.)
{
    Int_t anglesNumber;

    if (!(cin >> anglesNumber))
    {
        cerr << "anglesNumber read error" << endl;
        return;
    }
    cout << "Curent angle = " << angle << ", n = " << anglesNumber << endl;

    TString path = TString(gSystem->Getenv("VMCWORKDIR")) + "/macro/N15B11/mc_learning/output/interact_thetas.txt";
    ifstream fin(path, ios_base::in);
    if (!fin.is_open())
    {
        cerr << "Can't open " << path << endl;
        return;
    }
    cout << path << "has been opened" << endl;

    TString histTitle;
    histTitle.Form("Theta CM distribution, angle in Lab %5.2f", angle);
    Int_t i = 0;
    TH1F* hist = new TH1F("hist", histTitle, 100, 0., 180.);
    while (!fin.eof() && i <= anglesNumber)
    {
        Double_t curReadsAngle;
        fin >> curReadsAngle;
        hist->Fill(curReadsAngle);
        i++;
    }

    fin.clear();
    fin.close();

    TCanvas* canv = new TCanvas("canv", "canv", 800, 800);
    canv->cd();
    hist->Draw();

    TString pathToCanv = TString(gSystem->Getenv("VMCWORKDIR")) + "/macro/N15B11/mc_learning/output";
    TString saveCanvFileName;
    saveCanvFileName.Form("%s/histomram_ang_%5.2f.pdf", pathToCanv.Data(), angle);
    canv->SaveAs(saveCanvFileName);
    canv->Close();
}
