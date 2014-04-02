{
    TGraph2D *dt = new TGraph2D();
    Double_t x,y,z;
    for( Int_t n=0 ; n<20 ; n++ )
    {
        x=n;
        y=n;
        z=n;
       //dt->SetPoint(n,n,n,n); 
       dt->SetPoint(n,x,y,z); 
    }
    gStyle->SetPalette(1);
    //dt->Draw("tri1 p0");
    dt->Draw("surf1");
    
}
