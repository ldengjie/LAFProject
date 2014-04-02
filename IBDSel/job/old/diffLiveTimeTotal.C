/*
root -l
.L diffLiveTime.C+

diffLiveTime("EH121344")  LiveTime/AD1LiveTime
diffLiveTime("EH1213440") LiveTime/AD1LiveTime0
*/
#include  <iostream>
#include  <TMath.h>
#include  <TH1F.h>
#include  <TFile.h>
#include  <string>
int diffLiveTimeTotal(string run)
{
    //std::cout<<"run  : "<<run<<endl;

    string siteNum=run.substr(0,3);
    //std::cout<<"siteNum  : "<<siteNum<<endl; 
    //TString runNum=run.substr(3,5);
    //std::cout<<"runNum  : "<<runNum<<endl;

    //std::cout<<"length()  : "<<run.length()<<endl;
    TString hname="AD1LiveTime";
    if( run.length()==9 )
    {
        hname+="0";
    }
    TString f1name="/workfs/dyw/lidj/IBDSel/job/";
    f1name+=siteNum;
    f1name+="livetime.root";

    //return 0;
    TFile* f1= new TFile(f1name);
    TH1F* h1=(TH1F*)f1->Get(hname);
    


    TString f2name="/workfs/dyw/lidj/IBDSel/job/p12cOld/";
    f2name+=siteNum;
    f2name+="livetime.root";
    TFile* f2= new TFile(f2name);
    TH1F* h2=(TH1F*)f2->Get(hname);

    TH1F* h3=new TH1F(*h2);
    h3->Add(h1,h2,1,-1);

    TAxis *xaxis = h3->GetXaxis();
    int binNum = xaxis->GetNbins();
    //std::cout<<"binNu  : "<<binNum<<endl;
    double totalLiveTime=0.;
    for( int i=0 ; i<=binNum ; i++ )
    {
        if( h2->GetBinContent(i)!=0 )
        {
            std::cout<<" "<<endl;
            std::cout<<"binContent  : "<<h3->GetBinContent(i)<<endl;
            std::cout<<"h1  : "<<h1->GetBinContent(i)*1e0<<endl;
            std::cout<<"h2  : "<<h2->GetBinContent(i)*1e0<<endl;
            std::cout<<"h1-h2  : "<<h1->GetBinContent(i)-h2->GetBinContent(i)<<endl;
        }
        totalLiveTime+=h3->GetBinContent(i);
    }
    std::cout<<" "<<endl;
    std::cout<<"totalLiveTime  : "<<totalLiveTime<<endl;

    //h1->Draw();
    //h2->Draw("same");
}
