{
    //TFile* f=new TFile("/afs/ihep.ac.cn/users/l/lidj/largedata/IBD/P12C/EH1/run21660_hists.root");
    //TFile* f=new TFile("/afs/ihep.ac.cn/users/l/lidj/largedata/IBD/P12C/EH1/run21653_hists.root");
    TFile* f=new TFile("EH2livetime.root");
    //TH1F* h=(TH1F*)f->Get("LiveTime/AD1SingleLiveTime2000");
    TH1F* h=(TH1F*)f->Get("AD1SingleLiveTime0");
    TAxis *xaxis = h->GetXaxis();
    int binNum = xaxis->GetNbins();
    for( int i=0 ; i<=binNum ; i++ )
    //for( int i=910 ; i<=950 ; i++ )
    {
        std::cout<<i<<" bincontent  : "<<h->GetBinContent(i)<<endl;
    }
    
    //h->Draw();
}
