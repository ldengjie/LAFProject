{
    //TCanvas *c = new TCanvas("c","Graph2D example",0,0,700,600);
    TH3F *pmtH = new TH3F("pmtPos","postion of PMT",20,-5,5,20,-10,10,20,-20,20);

    for( int i=0 ; i<20 ; i++ )
    {
        //pmtH->SetBinContent(i,i,i,2);
        pmtH->Fill(i/4,i/2,19);
    }
    pmtH->Draw(); 
}
