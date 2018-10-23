void histo()
{
    ifstream fin("nu.txt");
    if (!fin.is_open())
    {
        cerr << "file open error" << endl;
        return;
    }

    TH1D* h = new TH1D("h", "Title", 100, 0., 500.);
    TString test = "time:";

    TVectorD tim(1497);
    TVectorD num(1497);
    Double_t timAr[1497];
    Int_t i = 0;
    Int_t k = 0;
    while (!fin.eof())
    {
        TString curTest;
        fin >> curTest;
        if (curTest == test)
        {
            Double_t curD;
            fin >> curD;
            //cout << curD << endl;
            //h->Fill(curD);
            timAr[i] = curD;
            i++;
        }
        else if (curTest == "nutron:")
        {
            fin >> num(k);
            k++;
        }

    }
    fin.clear();
    fin.close();
    //h->Draw();
/*
    for (Int_t j = 0; j < 1497; j++)
    {
        cout << num(j) << " " << tim(j) << endl;
    }
*/
    TGraph* gr = new TGraph(num, tim);
    gr->Draw("APL");
}
