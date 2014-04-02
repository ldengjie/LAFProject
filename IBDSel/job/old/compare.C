{
    TCanvas *c =new TCanvas("d","d",800,600);
    TFile *f1 = new TFile("/afs/ihep.ac.cn/users/l/lidj/file/IBDSel/job/EH1Ibd.root");
    TH1F *h1=f1->Get("tFnProEWithoutrpc");
    
    h1->SetLineColor(2);
    std::cout<<"h1 num 12-100  : "<<h1->Integral(155,200,"width")<<endl; 
    std::cout<<"h1 num 0.7-12  : "<<h1->Integral(108,154,"width")<<endl; 
    std::cout<<"h1 num  : "<<h1->Integral(1,200,"width")<<endl; 
    //h1->Draw();
    TFile *f2 = new TFile("/publicfs/dyb/data/userdata/zhangfh/IbdAna/ana/P12E/defaultCharge/EH1/mergedHist.root");
    TH1F *h2=f2->Get("zhangfh/allPromptEnergyAD1");
    TH1F *h22=f2->Get("zhangfh/allPromptEnergyAD2");
    TH1F* h2t=new TH1F(*h1);
    h2t->Add(h2,h22,1,1);

    std::cout<<"h2t num 12-100  : "<<h2t->Integral(155,200,"width")<<endl; 
    //std::cout<<"h2 num 0.7-12  : "<<h2->Integral(108,154,"width")<<endl; 
    std::cout<<"h2t num 0.7-12  : "<<h2t->Integral(1,h2t->FindBin(12),"width")<<endl; 
    std::cout<<"h2t num  : "<<h2t->Integral(1,200,"width")<<endl; 
    //h2t->Draw("same");
    
    TH1F* h3=new TH1F(*h1);
    h3->Add(h1,h2t,-1,1);
    std::cout<<"h3 num 12-100  : "<<h3->Integral(155,200,"width")<<endl; 
    std::cout<<"h3 num 0.7-12  : "<<h3->Integral(1,h3->FindBin(12),"width")<<endl; 
    std::cout<<"h3 num  : "<<h3->Integral(1,200,"width")<<endl; 
    //h3->Draw();

    //TH1F h5=0*(*h1);
    TH1F* h5=new TH1F(*h1);
    h5->Add(h1,h1,0,0);
        
    for( int i=2 ; i<=200 ; i++ )
    {
        h5->SetBinContent(i,(h3->Integral(1,i,"width")-h3->Integral(1,i-1,"width")));
    }
    //h5->Draw("same");
    h5->Draw();
}
