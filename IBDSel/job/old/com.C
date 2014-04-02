{
    TCanvas *c =new TCanvas("d","d",800,600);
    TFile *f1 = new TFile("/afs/ihep.ac.cn/users/l/lidj/file/IBDSel/job/EH1Ibd.root");
    TH1F *h1=f1->Get("tFnProEWithoutrpc");
    //h1->Draw();
    //TH1F h6=0*(*h1);
    TH1F* h6=new TH1F(*h1);
    //TH1F* h6=new TH1F("","",200,0,200);
    //h6->SetBinContent(0,0);
    //h6->SetBinContent(1,0);
        
    //for( int i=2 ; i<=200 ; i++ )
    //{
    //    h6->SetBinContent(i,2);
    //}
    h6->Draw();
}
