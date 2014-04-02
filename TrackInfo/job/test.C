{   
       //TCanvas *c = new TCanvas("c","Graph2D example",0,0,700,600); 
       Double_t x, y, z, P = 6.;  
       //Int_t np = 200; 
       TGraph2D *dt = new TGraph2D(); 
       //TRandom *r = new TRandom(); 
                    
       //for (Int_t N=0; N<np; N++) { 
       for (Int_t N=0; N<20; N++) { 
       // x = 2*P*(r->Rndm(N))-P; 
        //y = 2*P*(r->Rndm(N))-P; 
        //z = (sin(x)/x)*(sin(y)/y)+0.2; 
        x=N;
        y=N;
        z=N;
        dt->SetPoint(N,x,y,z); 
       } 
       gStyle->SetPalette(1); 
       dt->Draw("surf1");       // use “surf1” to generate the left picture  
       //dt->Draw("tri1 p0");       // use “surf1” to generate the left picture  
} 
