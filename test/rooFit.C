{
    gSystem->Load("libRooFit") ;
    using namespace RooFit ;
    // --- Observable ---
    RooRealVar mes("mes","m_{ES} (GeV)",5.20,5.30) ;
     
    // --- Parameters ---
    RooRealVar sigmean("sigmean","B^{#pm} mass",5.28,5.20,5.30) ;
    RooRealVar sigwidth("sigwidth","B^{#pm} width",0.0027,0.001,1.) ;
     
    // --- Build Gaussian PDF ---
    RooGaussian signal("signal","signal PDF",mes,sigmean,sigwidth) ;
// --- Build Argus background PDF ---
    RooRealVar argpar("argpar","argus shape parameter",-20.0,-100.,-1.) ;
    RooArgusBG background("background","Argus PDF",mes,RooConst(5.291),argpar) ;
     
    // --- Construct signal+background PDF ---
    RooRealVar nsig("nsig","#signal events",100,0.,10000) ;
    RooRealVar nbkg("nbkg","#background events",100,0.,10000) ;
    RooAddPdf model("model","g+a",RooArgList(signal,background),RooArgList(nsig,nbkg)) ;
// --- Generate a toyMC sample from composite PDF ---
    RooDataSet *data = model.generate(mes,2000) ;

    TFile* f= new TFile("rooFit.root","RECREATE");
    TCanvas* cc = new TCanvas("cc","cc",800,600) ;
    //TH1F* h=(TH1F*)f->Get("histo__mes");
    //h->Rebin(2);
    
    //RooDataHist* data=new RooDataHist("data", "data", mes, h);

    //TFile* f= new TFile("rooFit.root","RECREATE");
    //TH1F* h=data->createHistogram("histo",mes,Binning(100,5.2,5.3));
    //h->Draw();
    //h->Write();
    //f->Close();
     
    // -F-- Perform extended ML fit of composite PDF to toy data ---
    RooFitResult* fitres;


    fitres = model.fitTo((*data),Save(),PrintLevel(-1),Extended(kTRUE)) ;
    double nsigNum,nbkgNum,sigmeanValue,sigwidthValue,argparValue;
    nsigNum=nsig.getVal(0);
    nbkgNum=nbkg.getVal(0);
    sigmeanValue=sigmean.getVal(0);
    sigwidthValue=sigwidth.getVal(0);
    argparValue=argpar.getVal(0);
    std::cout<<"nsigNum       : "<<nsigNum<<endl;
    std::cout<<"nbkgNum       : "<<nbkgNum<<endl;
    std::cout<<"sigmeanValue  : "<<sigmeanValue<<endl;
    std::cout<<"sigwidthValue : "<<sigwidthValue<<endl;
    std::cout<<"argparValue   : "<<argparValue<<endl;
    // --- Plot toy data and composite PDF overlaid ---
    RooPlot* mesframe = mes.frame() ;
    data->plotOn(mesframe) ;
    data->statOn(mesframe); 
    model.plotOn(mesframe) ;
    model.plotOn(mesframe,Components(background),LineStyle(kDashed),LineColor(kRed)) ;
    model.paramOn(mesframe);
    cc->cd();
    mesframe->Draw();
    //mesframe->getHist();
    //mesframe->Draw();
    //mesframe->SavePrimitive("2.png");
    //mesframe->Print();
    cc->Write("nihao");
    cc->cd();
    mesframe->Draw();
    cc->Write("nihaonihao");
    f->Close();

    
}
