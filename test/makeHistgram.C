{
    const  int histNum=6;
    TH1F* h[histNum];
    TFile *f = new TFile("/afs/ihep.ac.cn/users/l/lidj/data/IBDSelForAcc/RpcBad/EH3/run22125_hists.root");
    h[0]=(TH1F*)f->Get("LiveTime/DaqTime");
    std::cout<<"h[0]->GetEntries()  : "<<h[0]->GetEntries()<<endl;
    h[1]=(TH1F*)f->Get("LiveTime/AD1LiveTime");
    std::cout<<"h[1]->GetEntries()  : "<<h[1]->GetEntries()<<endl;
    h[2]=(TH1F*)f->Get("LiveTime/AD2LiveTime");
    std::cout<<"h[2]->GetEntries()  : "<<h[2]->GetEntries()<<endl;
    h[3]=(TH1F*)f->Get("LiveTime/AD3LiveTime");
    std::cout<<"h[3]->GetEntries()  : "<<h[3]->GetEntries()<<endl;
    h[4]=(TH1F*)f->Get("LiveTime/AD3LiveTime");
    std::cout<<"h[4]->GetEntries()  : "<<h[4]->GetEntries()<<endl;
    h[5]=(TH1F*)f->Get("LiveTime/AD3LiveTime");
    std::cout<<"h[5]->GetEntries()  : "<<h[5]->GetEntries()<<endl;
    h[6]=(TH1F*)f->Get("LiveTime/AD3LiveTime");
}
