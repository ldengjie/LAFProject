#define SingleTree_cxx

#include "SingleTree.h"
#include <TH2.h>
#include <TStyle.h>
#include  <THStack.h>
#include  <iostream>

void SingleTree::Begin(TTree * /*tree*/)
{
    TString option = GetOption();
    TString IsoModeTmp=option(7,3);
    genIsoSpec=1;
    genAmCNeu=0;
    genTimeFitDis=0;
    //IsoMode="C9";//Li8,C99,B12
    IsoMode=IsoModeTmp;//Li8,C99,B12
    for( int i=0 ; i<16 ; i++ )
    {
        T2lastMuon[i]=0.;
    }

    if( IsoMode=="B12" )
    {
        LowEdge=4.0;
        HighEdge=20.0;
        LowEdge4e=LowEdge;
        HighEdge4e=HighEdge;
        signalWinLow=0.002;
        signalWinHigh=0.06;
        offWinLow=0.502;
        offWinHigh=0.560;
    }else if( IsoMode=="Li8" )
    {
        LowEdge=14.0;//for N12 time fit,that for Li8 has been done while B12.
        HighEdge=20.0;
        LowEdge4e=4.0;
        HighEdge4e=20.0;
        signalWinLow=0.6;
        signalWinHigh=4.0;
        offWinLow=10.6;
        offWinHigh=14.0;
    } else if( IsoMode=="C99" )
    {
        LowEdge=12.0;
        HighEdge=20.0;
        LowEdge4e=LowEdge;
        HighEdge4e=HighEdge;
        signalWinLow=0.2;
        signalWinHigh=0.6;
        offWinLow=10.2;
        offWinHigh=10.6;
    }


    TString DataVerTmp=option(3,4);
    dataVer=DataVerTmp;
    option=option(0,3);
    std::cout<<"dataVer  : "<<dataVer<<endl;
    int daqHistNum;
    if( dataVer=="P12E"||dataVer=="P12C" )
    {
        daqHistNum=4;
    } else
    {
        daqHistNum=5;
    }
    std::cout<<"option  : "<<option<<endl;
    ADNum=daqHistNum-1;
    site=2;
    if( option=="EH1")
    {
        site=0;
        ADNum=2;
    }
    if( option=="EH2" )
    {
        site=1;
        ADNum=daqHistNum-3;
    }
    option+="iso_";
    option+=dataVer;
    option+=".root";
    option=dataVer+"/"+option;
    file = new TFile(option,"update");
    if( genTimeFitDis )
    {
        for( int i=0 ; i<5 ; i++ )
        {
            histname=Form("time2lastshowermuon%i_%0.1f_%0.1f",i+1,LowEdge,HighEdge);
            time2lastshowermuon[i]=new TH1F(histname,"time2lastshowermuon",99999,0.001,100);
            histname=Form("time2lastshowermuonNoRed%i_%0.1f_%0.1f",i+1,LowEdge,HighEdge);
            time2lastshowermuonNoRed[i]=new TH1F(histname,"time2lastshowermuon",99999,0.001,100);
        }

        histname=Form("time2Allmuon_%0.1f_%0.1f",LowEdge,HighEdge);
        time2Allmuon=new TH1F(histname,"time2Allmuon",99999,0.001,100);
        histname=Form("time2AllmuonNoRed_%0.1f_%0.1f",LowEdge,HighEdge);
        time2AllmuonNoRed=new TH1F(histname,"time2Allmuon",99999,0.001,100);

        for( int i=0 ; i<5 ; i++ )
        {
            time2lastmuon[i]=new TTree(Form("slice%i_%0.1f_%0.1f",i+1,LowEdge,HighEdge),"time2lastmuon");
            time2lastmuonNoRed[i]=new TTree(Form("sliceNoRed%i_%0.1f_%0.1f",i+1,LowEdge,HighEdge),"time2lastmuon NoRed");

            time2lastmuon[i]->Branch("xt",&T2lastMuon[4+i],"xt/F");
            time2lastmuonNoRed[i]->Branch("xt",&T2lastMuon[10+i],"xt/F");
        }
        time2lastmuon[5]=new TTree(Form("slice6_%0.1f_%0.1f",LowEdge,HighEdge),"time2lastmuon");
        time2lastmuon[5]->Branch("xt",&minT2allmuon,"xt/F");
        time2lastmuonNoRed[5]=new TTree(Form("sliceNoRed6_%0.1f_%0.1f",LowEdge,HighEdge),"time2lastmuon NoRed");
        time2lastmuonNoRed[5]->Branch("xt",&minT2allmuonNoRed,"xt/F");
    }
    if( genAmCNeu )
    {
        for( int i=0 ; i<4 ; i++ )
        {
            //histname=Form("singleSpecAD%i_%0.1f_%0.1f",i+1,LowEdge,HighEdge);
            //singleSpec[i]=new TH1F(histname,"spectra of singles",400,0,20);
            histname=Form("singleUpperAD%i_%0.1f_%0.1f",i+1,LowEdge,HighEdge);
            singleUpper[i]=new TH1F(histname,"single upper",400,0,20);
            histname=Form("singleLowerAD%i_%0.1f_%0.1f",i+1,LowEdge,HighEdge);
            singleLower[i]=new TH1F(histname,"single lower",400,0,20);
            histname=Form("AmCSpecAD%i_%0.1f_%0.1f",i+1,LowEdge,HighEdge);
            AmCSpec[i]=new TH1F(histname,"AmC spectrum",400,0,20);
        }

    }

    if( genIsoSpec )
    {
        for( int i=0 ; i<5 ; i++ )
        {
            sprintf(nameChar,"signalWindowSlice%i",i+1);
            signalWin[i]=new TH1F(nameChar,nameChar,80,0,20);
            sprintf(nameChar,"offWindowSlice%i",i+1);
            offWin[i]=new TH1F(nameChar,nameChar,80,0,20);
            sprintf(nameChar,"signalWindowNoRedSlice%i",i+1);
            signalWinNoRed[i]=new TH1F(nameChar,nameChar,80,0,20);
            sprintf(nameChar,"offWindowNoRedSlice%i",i+1);
            offWinNoRed[i]=new TH1F(nameChar,nameChar,80,0,20);
        }
        sprintf(nameChar,"signalWindow2AllMuon");
        signalWin[5]=new TH1F(nameChar,nameChar,80,0,20);
        sprintf(nameChar,"offWindow2AllMuon");
        offWin[5]=new TH1F(nameChar,nameChar,80,0,20);
        sprintf(nameChar,"signalWindow2AllMuonNoRed");
        signalWinNoRed[5]=new TH1F(nameChar,nameChar,80,0,20);
        sprintf(nameChar,"offWindow2AllMuonNoRed");
        offWinNoRed[5]=new TH1F(nameChar,nameChar,80,0,20);
    }

    std::cout<<"come here "<<endl;
}

void SingleTree::SlaveBegin(TTree * /*tree*/)
{

    TString option = GetOption();

}

Bool_t SingleTree::Process(Long64_t entry)
{
    GetEntry(entry);
    if( entry%2000000==0 )
    {
        std::cout<<entry<<endl;
    }
    //isotopes spectrum
    if( genIsoSpec )
    {
        if( energy>LowEdge4e&&energy<HighEdge4e )
        {
            minT2allmuon=1000.;
            minT2allmuonNoRed=1000.;
            for( int j=0 ; j<5 ; j++ )
            {
                minT2allmuon=minT2allmuon<T2lastMuon[j+4]?minT2allmuon:T2lastMuon[j+4];
                minT2allmuonNoRed=minT2allmuonNoRed<T2lastMuon[j+10]?minT2allmuonNoRed:T2lastMuon[j+10];
                if( T2lastMuon[j+4]>signalWinLow &&T2lastMuon[j+4]<signalWinHigh )
                {
                    signalWin[j]->Fill(energy);
                }
                if( T2lastMuon[j+4]>offWinLow &&T2lastMuon[j+4]<offWinHigh )
                {
                    offWin[j]->Fill(energy);
                }
                if( T2lastMuon[j+10]>signalWinLow &&T2lastMuon[j+10]<signalWinHigh )
                {
                    signalWinNoRed[j]->Fill(energy);
                }
                if( T2lastMuon[j+10]>offWinLow &&T2lastMuon[j+10]<offWinHigh )
                {
                    offWinNoRed[j]->Fill(energy);
                }
            }
            if( minT2allmuon>signalWinLow &&minT2allmuon<signalWinHigh )
            {
                signalWin[5]->Fill(energy);
            }
            if( minT2allmuon>offWinLow &&minT2allmuon<offWinHigh )
            {
                offWin[5]->Fill(energy);
            }
            if( minT2allmuonNoRed>signalWinLow &&minT2allmuonNoRed<signalWinHigh )
            {
                signalWinNoRed[5]->Fill(energy);
            }
            if( minT2allmuonNoRed>offWinLow &&minT2allmuonNoRed<offWinHigh )
            {
                offWinNoRed[5]->Fill(energy);
            }
        }
    }
    if( energy<LowEdge || energy>HighEdge)
    {
        return 1; 
    }
    //AmC netron
    if( genAmCNeu )
    {
        if( z>0 )
        {
            singleUpper[det-1]->Fill(energy);
        }
        if( z<0 )
        {
            singleLower[det-1]->Fill(energy);
        }

    }
    if( genTimeFitDis )
    {
        for( int j=0 ; j<5 ; j++ )
        {
            time2lastshowermuon[j]->Fill(T2lastMuon[j+4]);
            time2lastshowermuonNoRed[j]->Fill(T2lastMuon[j+10]);

        }
        minT2allmuon=1000.;
        minT2allmuonNoRed=1000.;
        for( int j=0 ; j<5 ; j++ )
        {
            minT2allmuon=minT2allmuon<T2lastMuon[j+4]?minT2allmuon:T2lastMuon[j+4];
            minT2allmuonNoRed=minT2allmuonNoRed<T2lastMuon[j+10]?minT2allmuonNoRed:T2lastMuon[j+10];
        }
        //if( T2lastMuon[2+10]>0 )
        //{
        //std::cout<<"find one "<<T2lastMuon[2+10]<<endl;
        //}
        time2Allmuon->Fill(minT2allmuon);
        time2AllmuonNoRed->Fill(minT2allmuonNoRed);
        for( int j=0 ; j<6 ; j++ )
        {
            time2lastmuon[j]->Fill();
            time2lastmuonNoRed[j]->Fill();
        }


    }


    return kTRUE;
}

void SingleTree::SlaveTerminate()
{

}

void SingleTree::Terminate()
{
    std::cout<<"Now is in Terminate() "<<endl;
    if( genIsoSpec )
    {
        //TCanvas* c[6];
        for( int i=0 ; i<6 ; i++ )
        {
            std::cout<<"1 "<<endl;
            //histname=Form("%sSpecColor",IsoMode.c_str());
            //c[i]=new TCanvas(histname,"IsoSpec",600,400);
            signalWin[i]->SetLineColor(kRed);
            std::cout<<"1.1 "<<endl;
            //histname=Form("%sSpecColor",IsoMode.c_str());
            signalWin[i]->GetXaxis()->SetTitle("Energy(MeV)");
            std::cout<<"1.2 "<<endl;
            signalWin[i]->GetYaxis()->SetTitle("Entries");
            std::cout<<"1.3 "<<endl;
            signalWin[i]->SetStats(kFALSE); 
            std::cout<<"1.4 "<<endl;
            //signalWin[i]->Draw();
            offWin[i]->SetLineColor(kGreen);
            std::cout<<"1.5 "<<endl;
            offWin[i]->SetStats(kFALSE); 
            std::cout<<"1.6 "<<endl;
            //offWin[i]->Draw("same");
            histname=Form("%sSpecSlice%i_%0.1f_%0.1f",IsoMode.c_str(),i+1,LowEdge4e,HighEdge4e);
            std::cout<<"2 "<<endl;
            isoSpec[i]=new TH1F(histname,"IsoSpec",80,0,20);
            std::cout<<"3 "<<endl;
            isoSpec[i]->Sumw2();
            //isoSpec[i]->Add(signalWin[i],offWin[i],1,-0.5);
            std::cout<<"4 "<<endl;
            isoSpec[i]->Add(signalWin[i],offWin[i],1,-1);
            std::cout<<"5 "<<endl;
            isoSpec[i]->SetLineColor(kBlue);
            isoSpec[i]->SetStats(kFALSE);
            isoSpec[i]->SetMarkerStyle(20);
            isoSpec[i]->SetMarkerSize(0.7);
            isoSpec[i]->SetMarkerColor(kBlue);
            isoSpec[i]->SetOption("E");
            //isoSpec[i]->Draw("sameE");
            //TLegend *legend=new TLegend(.4,.65,.79,.89);
            //histname=Form("signalWin[i] : %0.3f ~ %0.3f",signalWin[i]Low,signalWin[i]High);
            //legend->AddEntry(signalWin[i],histname,"lp");
            //histname=Form("offWin[i] : %0.3f ~ %0.3f",offWin[i]Low,offWin[i]High);
            //legend->AddEntry(offWin[i],histname,"lp");
            //histname=Form("%sSpec",IsoMode.c_str());
            //legend->AddEntry(isoSpec[i],histname,"lp");
            //legend->SetFillColor(0);
            //legend->Draw("same");

            //THStack* hs = new THStack("hs","");
            //hs->Add(signalWin[i]);
            //hs->Add(offWin[i]);
            //hs->Draw();
            //isoSpec[i]->Draw("same");
            //legend->Draw("same");
            isoSpec[i]->Write();
            //c[i]->Write();

            signalWinNoRed[i]->SetLineColor(kRed);
            signalWinNoRed[i]->GetXaxis()->SetTitle("Energy(MeV)");
            signalWinNoRed[i]->GetYaxis()->SetTitle("Entries");
            signalWinNoRed[i]->SetStats(kFALSE); 
            //signalWinNoRed[i]->Draw();
            offWinNoRed[i]->SetLineColor(kGreen);
            offWinNoRed[i]->SetStats(kFALSE); 
            //offWinNoRed[i]->Draw("same");
            histname=Form("%sSpecNoRedSlice%i_%0.1f_%0.1f",IsoMode.c_str(),i+1,LowEdge4e,HighEdge4e);
            isoSpecNoRed[i]=new TH1F(histname,"IsoSpecNoRed",80,0,20);
            isoSpecNoRed[i]->Sumw2();
            //isoSpecNoRed[i]->Add(signalWinNoRed[i],offWinNoRed[i],1,-0.5);
            isoSpecNoRed[i]->Add(signalWinNoRed[i],offWinNoRed[i],1,-1);
            isoSpecNoRed[i]->SetLineColor(kBlue);
            isoSpecNoRed[i]->SetStats(kFALSE);
            isoSpecNoRed[i]->SetMarkerStyle(20);
            isoSpecNoRed[i]->SetMarkerSize(0.7);
            isoSpecNoRed[i]->SetMarkerColor(kBlue);
            isoSpecNoRed[i]->SetOption("E");
            isoSpecNoRed[i]->Write();
            //c[i]
        }

    }

    if( genAmCNeu )
    {

        TCanvas *c1=new TCanvas("AmCNeuSpec","AmCNeuSpec",1200,600);
        c1->Divide(2,2);
        for( int i=0 ; i<4 ; i++ )
        {
            c1->cd(i+1);
            singleUpper[i]->SetLineColor(kRed);
            singleUpper[i]->GetXaxis()->SetTitle("Energy(MeV)");
            singleUpper[i]->GetYaxis()->SetTitle("Entries/0.05Mev");
            singleUpper[i]->SetStats(kFALSE);
            //singleUpper[i]->Draw();
            singleLower[i]->SetLineColor(kGreen);
            singleLower[i]->SetStats(kFALSE);
            //singleLower[i]->Draw("same");
            AmCSpec[i]->Sumw2();
            AmCSpec[i]->Add(singleUpper[i],singleLower[i],1,-0.95);
            AmCSpec[i]->SetLineColor(kBlue);
            AmCSpec[i]->SetStats(kFALSE);
            //AmCSpec[i]->Draw("same");
            THStack* hs = new THStack("hs","");
            hs->Add(singleUpper[i]);
            hs->Add(singleLower[i]);
            //hs->Add(AmCSpec[i]);
            hs->Draw();
            AmCSpec[i]->Draw("same");
            TLegend *legend=new TLegend(.4,.65,.79,.89);
            //std::cout<<"singleUpper[i]->FindBin(LowEdge)  : "<<singleUpper[i]->FindBin(LowEdge)<<endl;
            //std::cout<<"singleUpper[i]->FindBin(HighEdge)  : "<<singleUpper[i]->FindBin(HighEdge)<<endl;
            double upperNum=singleUpper[i]->Integral(singleUpper[i]->FindBin(LowEdge),singleUpper[i]->FindBin(HighEdge));
            double lowNum=singleLower[i]->Integral(singleLower[i]->FindBin(LowEdge),singleLower[i]->FindBin(HighEdge));
            histname=Form("Single Upper : %0.0f",upperNum);
            legend->AddEntry(singleUpper[i],histname,"lp");
            histname=Form("Single Lower : %0.0f",lowNum);
            legend->AddEntry(singleLower[i],histname,"lp");
            histname=Form("AmC number : %0.0f",upperNum-lowNum*0.95);
            legend->AddEntry(AmCSpec[i],histname,"lp");
            histname=Form("   %0.1f ~ %0.1f MeV",LowEdge,HighEdge);
            legend->SetHeader(histname);
            legend->SetFillColor(0);
            legend->Draw("same");
            /*
               for( int j=81 ; j<401 ; j++ )
               {
               if( singleUpper[i]->GetBinContent(j)==0 )
               {
               continue;
               }
               std::cout<<"Upper "<<i<<"  : "<<singleUpper[i]->GetBinContent(j)<<endl;
               std::cout<<"Lower "<<i<<"  : "<<singleLower[i]->GetBinContent(j)<<endl;
               std::cout<<"AmC   "<<i<<"  : "<<AmCSpec[i]->GetBinContent(j)<<endl;
               std::cout<<"U-L   "<<i<<"  : "<<singleUpper[i]->GetBinContent(j)-singleLower[i]->GetBinContent(j)*0.95<<endl;
               }
             */
        }

        for( int i=0 ; i<4 ; i++ )
        {
            singleUpper[i]->Write();
            singleLower[i]->Write();
            AmCSpec[i]->Write();
        }
        c1->Write();
    }
    if( genTimeFitDis )
    {
        for( int i=0 ; i<5 ; i++ )
        {
            time2lastshowermuon[i]->Write();
            time2lastshowermuonNoRed[i]->Write();
        }
        time2Allmuon->Write();
        time2AllmuonNoRed->Write();
        for( int j=0 ; j<6 ; j++ )
        {
            time2lastmuon[j]->Write();
            time2lastmuonNoRed[j]->Write();
        }

    }

    //c1->Write();
    file->Close();
}
