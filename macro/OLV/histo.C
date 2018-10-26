void histo()
{
    TFile* file = new TFile("sim.root", "READ");
    if (file->IsZombie())
    {
        cerr << "File read error" << endl;
        return;
    }

    TTree* tree = (TTree*)file->Get("er");
    if (!tree)
    {
        cerr << "Tree read error" << endl;
        return;
    }

    Int_t nBins = 100;
    Double_t binMin = 0.;
    Double_t binMax = 2400.;
    TCanvas* c = new TCanvas("c", "canv", 1000, 1200);
    c->cd(1);
    tree->Draw("OVLdetectorvCubPoint.fTimeIn");
    TH1F* histo = (TH1F*)gPad->GetListOfPrimitives()->FindObject("htemp");
    if (!histo)
    {
        cerr << "Histo TimeIn Draw error" << endl;
        return;
    }
    histo->SetBins(nBins, binMin, binMax);
    
    TCanvas* canv = new TCanvas("canv", "Time", 1000, 1200);
    canv->Divide(1, 2);
    TH1F* inHis = new TH1F();
    *inHis = *histo;
    c->Close();
    inHis->SetName("inHis");
    inHis->SetTitle("Time in");
    canv->cd(1);
    inHis->Draw();

    c = new TCanvas("c", "canv", 1000, 1200);
    c->cd(1);
    tree->Draw("OVLdetectorvCubPoint.fTimeOut");
    histo = (TH1F*)gPad->GetListOfPrimitives()->FindObject("htemp");
    if (!histo)
    {
        cerr << "Histo TimeOut Draw error" << endl;
        return;
    }
    histo->SetBins(nBins, binMin, binMax);

    TH1F* outHis = new TH1F();
    *outHis = *histo;
    c->Close();
    outHis->SetName("outHis");
    outHis->SetTitle("Time Out");
    canv->cd(2);
    outHis->Draw();

    Int_t entriesIn = inHis->GetEntries();
    Int_t entriesOut = outHis->GetEntries();
    Int_t maxI = max(entriesIn, entriesOut);
    cout << "in Entries: " << entriesIn << ", out Entries: " << entriesOut << endl;	
/*
    // Testing bincontent
    Int_t test = 0;
    for (Int_t i = 0; i <= nBins; i++)
    {
    	cout << inHis->GetBinContent(i) << endl;
    	test += inHis->GetBinContent(i);
    }
    cout << "full: " << test << endl;

    test = 0;
    for (Int_t i = 0; i <= nBins; i++)
    {
    	cout << outHis->GetBinContent(i) << endl;
    	test += outHis->GetBinContent(i);
    }
    cout << "full: " << test << endl;
*/

    TCanvas* resCanv = new TCanvas("res", "resCanv", 1000, 1200);
    TH1F* resHist = new TH1F("resHist", "Nutronst num vs. Time", nBins, binMin, binMax);
    Int_t memNum = 0;
    Double_t step = (Double_t)(binMax-binMin)/nBins;
    Double_t time = 0.;
    for (Int_t i = 0; i <= nBins; i++)
    {
    	Int_t curentNumIn = (Int_t)inHis->GetBinContent(i); // curent number for borning nutrons 
    	Int_t curentNumOut = (Int_t)outHis->GetBinContent(i); // curent cumber for out nutrons
    	Int_t curentNum = memNum + curentNumIn;
    	memNum = curentNum - curentNumOut;
    	cout << "cn: " << setw(5) << curentNum << ", born: " 
    			<< setw(5) << curentNumIn << ", out: " 
    			<< setw(5) << curentNumOut << ", mem: " 
    			<< setw(5) << memNum << endl;

    	for (Int_t j = 0; j < curentNum; j++)
    	{
    		resHist->Fill(time);
    	}
    	time += i*step;
    }
    resCanv->cd(1);
    resHist->Draw();

    canv->Close();
    //resCanv->Close();
}
