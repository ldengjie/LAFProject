#include <iostream>
#include <sys/stat.h> //create dir :mkdir("bac",0755)
#include <dirent.h>//look DIR : opendir("adb")
#include <TCanvas.h>
#include <TMath.h>
#include <TLegend.h>
#include <TH1F.h>
#include <TFile.h>
#include <fstream>
#include <sstream>
#include "RooGlobalFunc.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooConstVar.h"
#include "RooFormulaVar.h"
#include "RooExponential.h"
#include "RooAddPdf.h"
#include "RooPlot.h"
#include "RooFitResult.h"
#include <string>
#include <TChain.h>
#include <TROOT.h>
#include <vector>
using namespace RooFit;

int anaHists(int siteNum,string dataVer)
{
    //gStyle->SetOptStat(0);
    //gSystem->Load("libRooFit");
    //gSystem->Load("libCintex");
    //gROOT->ProcessLine("using namespace RooFit;");

    //===>initialize variable
    bool anamuon=0;//
    bool anasingle=0;//accidentals bg
    bool anaLiHe=0;//He8/Li9 bg
    bool anaEffMulti=1;//efficiency of multiple cut
    bool genBinNumofRun=0;
    int ADNumOfSite[3]={0};
    int daqHistNum=5;
    if( dataVer.find("13")!=string::npos || dataVer.find("14")!=string::npos || dataVer.find("15")!=string::npos || dataVer.find("16")!=string::npos)
    {
        ADNumOfSite[0]=2;
        ADNumOfSite[1]=2;
        ADNumOfSite[2]=4;
        daqHistNum=5;
    } else
    {
        if( dataVer.find("11")!=string::npos || dataVer.find("12")!=string::npos )
        {
            ADNumOfSite[0]=2;
            ADNumOfSite[1]=1;
            ADNumOfSite[2]=3;
            daqHistNum=4;
        }
    }
    string site;
    if( siteNum==1 )
    {
        site="EH1";
    } else if(siteNum==2)
    {
        site="EH2";
    }else if (siteNum==3)
    {
        site="EH3";
    }else
    {
        site="EH1";
    }

    //===>initialize histograms
    //livetime
    int FirstInt=0;
    string runnum;
    TH1F* h[5];//the fifth is for daqtime
    TH1F* h0[4];
    TH1F* th[5];
    TH1F* th0[4];
    TH1F* h_200[4];
    TH1F* h0_200[4];
    TH1F* th_200[4];
    TH1F* th0_200[4];
    Double_t totalTime[5]={0.};
    Double_t totalTime0[4]={0.};

    //eff multi

    int effMultiHistInt=0;
    Double_t meanCaptureTime[3][4];
    meanCaptureTime[0][0]=33.55e-6;
    meanCaptureTime[0][1]=33.49e-6;
    meanCaptureTime[1][0]=33.51e-6;
    meanCaptureTime[1][1]=33.51e-6;
    meanCaptureTime[2][0]=33.05e-6;
    meanCaptureTime[2][1]=32.74e-6;
    meanCaptureTime[2][2]=33.46e-6;
    meanCaptureTime[2][3]=33.46e-6;
    TH1F* NumOfAll[4];
    TH1F* NumOfAll0[4];
    TH1F* tNumOfAll[4];
    TH1F* tNumOfAll0[4];
    TH1F* RateOfAll[4];
    TH1F* RateOfAll0[4];
    TH1F* SingleLiveTime[4];
    TH1F* SingleLiveTime0[4];
    TH1F* SingleLiveTime200[4];
    TH1F* SingleLiveTime2000[4];
    TH1F* tSingleLiveTime[4];
    TH1F* tSingleLiveTime0[4];
    TH1F* tSingleLiveTime200[4];
    TH1F* tSingleLiveTime2000[4];
    TH1F* NumOfSingleLiveTime200[4];
    TH1F* NumOfSingleLiveTime2000[4];
    TH1F* tNumOfSingleLiveTime200[4];
    TH1F* tNumOfSingleLiveTime2000[4];
    double effTime[4]={0.};
    double effTime0[4]={0.};

    //muon;
    int RateHistInt=0;
    TH1F* muonNum[7];
    TH1F* muonRate[7];
    TH1F* tmuonNum[7];
    Double_t totalNum[7]={0.};

    //singles
    int SingleHistInt=0;
    TH1F* NumN[4];
    TH1F* NumN0[4];
    TH1F* NumE[4];
    TH1F* NumE0[4];
    TH1F* NumE_600[4];
    TH1F* NumE0_600[4];
    TH1F* NRate[4];
    TH1F* NRate0[4];
    TH1F* ERate[4];
    TH1F* ERate0[4];
    TH1F* ERate_600[4];
    TH1F* ERate0_600[4];
    TH1F* tNumN[4];
    TH1F* tNumN0[4];
    TH1F* tNumE[4];
    TH1F* tNumE0[4];
    TH1F* tNumE_600[4];
    TH1F* tNumE0_600[4];
    Double_t NumAccBkg[4]={0.};
    Double_t NumAccBkg0[4]={0.};
    Double_t NumAccBkg_600[4]={0.};
    Double_t NumAccBkg0_600[4]={0.};
    Double_t totalN[4]={0.};
    Double_t totalN0[4]={0.};

    //Li9 He8
    int LiHistInt=0;
    double n98total0=0.;
    double in98total0=0.;
    double n98total=0.;
    double in98total=0.;
    double ifitn98total=0.;
    double n98Rate=0.;
    double in98Rate=0.;
    double tlivetime=0.;
    TH1F* showermuonNum[6]; 
    TH1F* showermuonNum0[6]; 
    TH1F* time2lastshowermuon[6];
    TH1F* time2lastshowermuon0[6];
    TH1F* tshowermuonNum[6]; 
    TH1F* tshowermuonNum0[6]; 
    TH1F* ttime2lastshowermuon[6];
    TH1F* ttime2lastshowermuon0[6];

    //===>get histograms from .root file for analyse
    ifstream runlist;
    string listname=dataVer+"/"+site+"run.list."+dataVer;
    cout<<"listname : "<<listname<<endl;
    runlist.open(listname.c_str());
    if( !runlist )
    {
        std::cout<<"Has no runlist : "<<listname<<endl;
        return 0;
    }
    string filename;

    if( genBinNumofRun )
    {
        ofstream outfile4runBinNumInit;
        string outfile4runBinNumInitName=dataVer+"/"+"binNumOfRun."+site+"."+dataVer;
        outfile4runBinNumInit.open(outfile4runBinNumInitName.c_str());
        outfile4runBinNumInit<<"";
        outfile4runBinNumInit.close();
    }

    while( getline(runlist,runnum) )
    {
        filename=runnum;
        std::cout<<"filename=  : "<<filename<<endl;
        if( filename=="" )
        {
            continue;
        }
        //std::cout<<"now is  : "<<runnum<<endl;
        /*
           if( runnum!="/afs/ihep.ac.cn/users/l/lidj/data/IBDSelForAcc/RpcBad/EH3/run22125_hists.root")
           {
        //std::cout<<"find 22125 "<<endl;
        continue;
        }
        std::cout<<"find 22125 "<<endl;
         */
        //filename="/afs/ihep.ac.cn/users/l/lidj/data/IBDSelForAcc/RpcGood/";
        //filename+=site;
        //filename+="/run";
        //filename+=runnum;
        //filename+="_hists.root";
        //cout<<"Processing "<<filename<<endl;
        TFile *f = new TFile(filename.c_str());
        if( f->IsZombie() )
        {
            cout<<"Can't open "<<site<<"/" <<runnum <<endl;
            //exit(-1);
            continue;
        }

        if( genBinNumofRun )
        {
            int runNumPos=runnum.rfind("_");
            string runNumShort=runnum.substr(runNumPos-5,5);
            ofstream outfile4runBinNum;
            string outfile4runBinNumName=dataVer+"/"+"binNumOfRun."+site+"."+dataVer;
            outfile4runBinNum.open(outfile4runBinNumName.c_str(),ios::app|ios::out);
            outfile4runBinNum<< runNumShort <<" : ";
            TH1F* h4runBinNum=(TH1F*)f->Get("LiveTime/DaqTime");
            TAxis *xaxis =h4runBinNum->GetXaxis();
            int binNum = xaxis->GetNbins();
            for( int ii=1 ; ii<binNum ; ii++ )
            {
                if( h4runBinNum->GetBinContent(ii)!=0 )
                {
                    outfile4runBinNum<<" "<<ii;
                }
            }
            outfile4runBinNum<<endl;
            outfile4runBinNum.close();
        }

        //livetime
        for( int i=0 ; i<5 ; i++ )
        {

            stringstream hname;
            if( i==4 )
            {
                hname<<"LiveTime/DaqTime";
            } else
            {
                hname<<"LiveTime/AD";
                hname<<i+1;
                hname<<"LiveTime";	
            }
            h[i] = (TH1F*)f->Get(hname.str().c_str());
            if( i!=4 )
            {
                h_200[i] = (TH1F*)f->Get((hname.str()+"4IbdSel").c_str());
                h0_200[i] = (TH1F*)f->Get((hname.str()+"4IbdSel0").c_str());
                hname<<"0";	
                h0[i] = (TH1F*)f->Get(hname.str().c_str());
            }
            if( !(h[i]) )
            {
                cout<<"Can not get Hist : "<<hname.str()<<" from "<<site<<"/"<<runnum<<" ."<<endl;
                //return true;
                continue;
            }
            if( FirstInt<5 )
            {
                th[i] = new TH1F(*h[i]); 
                if( i!=4 )
                {
                    th_200[i] = new TH1F(*h_200[i]); 
                    th0[i] = new TH1F(*h0[i]);
                    th0_200[i] = new TH1F(*h0_200[i]);
                }
                FirstInt++;
            }else
            {
                th[i]->Add(th[i],h[i],1,1);
                if( i!=4 )
                {
                    th_200[i]->Add(th_200[i],h_200[i],1,1);
                    th0[i]->Add(th0[i],h0[i],1,1);
                    th0_200[i]->Add(th0_200[i],h0_200[i],1,1);
                }
            }
        }

        //muon
        if( anamuon )
        {
            for( int i=0 ; i<7 ; i++ )
            {
                stringstream hname;
                //stringstream hratename;
                if( i<4 )
                {
                    hname<<"Muon/AD";
                    hname<<i+1;
                    hname<<"MuonNum";
                    //	hratename<<"Muon/AD";
                    //	hratename<<i+1;
                    //	hratename<<"MuonRate";
                } else if(i==4)
                {
                    hname<<"Muon/IwsMuonNum";	
                    //	hratename<<"Muon/IwsMuonRate";	
                }else if(i==5)
                {
                    hname<<"Muon/OwsMuonNum";	
                    //	hratename<<"Muon/OwsMuonRate";	
                }else if(i==6)
                {
                    hname<<"Muon/RpcMuonNum";	
                    //	hratename<<"Muon/RpcMuonRate";	
                }
                muonNum[i]=(TH1F*)f->Get(hname.str().c_str());
                totalNum[i] += muonNum[i]->GetEntries();
                if( RateHistInt<7 )
                {
                    tmuonNum[i] = new TH1F(*muonNum[i]);
                    muonRate[i] = new TH1F(*muonNum[i]);
                    /*
                       stringstream muonRname;
                       muonRname<<"AD";
                       muonRname<<i+1;
                       muonRname<<"muonRate";
                       muonRate[i]->SetName(muonRname.str().c_str());
                     */
                    muonRate[i]->Add(muonNum[i],muonNum[i],0,0);;
                    RateHistInt++;
                } else
                {
                    tmuonNum[i]->Add(tmuonNum[i],muonNum[i],1,1);
                }
            }
        }

        //singles
        if( anasingle )
        {
            for( int i=0 ; i<ADNumOfSite[siteNum-1] ; i++ )
            {
                stringstream hNname;
                stringstream hEname;
                hNname<<"lidj/AD";
                hNname<<i+1;
                hNname<<"NumN";
                NumN[i]=(TH1F*)f->Get(hNname.str().c_str());
                hNname<<"0";
                NumN0[i]=(TH1F*)f->Get(hNname.str().c_str());
                hEname<<"lidj/AD";
                hEname<<i+1;
                hEname<<"NumE";
                string str600="_600";
                NumE[i]=(TH1F*)f->Get(hEname.str().c_str());
                NumE_600[i]=(TH1F*)f->Get((hEname.str()+str600).c_str());
                hEname<<"0";
                NumE0[i]=(TH1F*)f->Get(hEname.str().c_str());
                //NumE0_600[i]=(TH1F*)f->Get(hEname.str().c_str()&"_600");
                NumE0_600[i]=(TH1F*)f->Get((hEname.str()+str600).c_str());
                totalN[i]+=NumN[i]->GetEntries();
                totalN0[i]+=NumN0[i]->GetEntries();
                if( SingleHistInt<4 )
                {
                    tNumN[i] = new TH1F(*NumN[i]);
                    tNumN0[i] = new TH1F(*NumN0[i]);
                    tNumE[i] = new TH1F(*NumE[i]);
                    tNumE0[i] = new TH1F(*NumE0[i]);
                    tNumE_600[i] = new TH1F(*NumE_600[i]);
                    tNumE0_600[i] = new TH1F(*NumE0_600[i]);
                    NRate[i] = new TH1F(*NumN[i]);
                    stringstream NRname;
                    NRname<<"AD";
                    NRname<<i+1;
                    NRname<<"NRate";
                    NRate[i]->SetName(NRname.str().c_str());
                    //NumN[i]->GetXaxis()->GetNbins()
                    //    std::cout<<"NRate[i]->GetXaxis()->GetNbins()  : "<<NRate[i]->GetXaxis()->GetNbins()<<endl;
                    NRate[i]->Add(NumN[i],NumN[i],0,0);;
                    NRate0[i] = new TH1F(*NumN0[i]);
                    NRname<<"0";
                    NRate0[i]->SetName(NRname.str().c_str());
                    NRate0[i]->Add(NumN0[i],NumN0[i],0,0);;
                    ERate[i] = new TH1F(*NumE[i]);
                    stringstream ERname;
                    ERname<<"AD";
                    ERname<<i+1;
                    ERname<<"ERate";
                    ERate[i]->SetName(ERname.str().c_str());
                    ERate[i]->Add(NumE[i],NumE[i],0,0);;
                    ERate_600[i] = new TH1F(*NumE_600[i]);
                    ERate_600[i]->SetName((ERname.str()+str600).c_str());
                    ERate_600[i]->Add(NumE_600[i],NumE_600[i],0,0);;
                    ERate0[i] = new TH1F(*NumN[i]);
                    ERname<<"0";
                    ERate0[i]->SetName(ERname.str().c_str());
                    ERate0[i]->Add(NumE0[i],NumE0[i],0,0);;
                    ERate0_600[i] = new TH1F(*NumE[i]);
                    ERate0_600[i]->SetName((ERname.str()+str600).c_str());
                    ERate0_600[i]->Add(NumE0_600[i],NumE0_600[i],0,0);;
                    SingleHistInt++;
                } else
                {
                    tNumN[i]->Add(tNumN[i],NumN[i],1,1);
                    tNumN0[i]->Add(tNumN0[i],NumN0[i],1,1);
                    tNumE[i]->Add(tNumE[i],NumE[i],1,1);
                    tNumE0[i]->Add(tNumE0[i],NumE0[i],1,1);
                    tNumE_600[i]->Add(tNumE_600[i],NumE_600[i],1,1);
                    tNumE0_600[i]->Add(tNumE0_600[i],NumE0_600[i],1,1);
                }
            }
        }

        //eff multi
        if( anaEffMulti )
        {
            for( int i=0 ; i<ADNumOfSite[siteNum-1] ; i++ )
            {
                stringstream hNname;
                hNname<<"lidj/AD";
                hNname<<i+1;
                hNname<<"NumOfAll";
                NumOfAll[i]=(TH1F*)f->Get(hNname.str().c_str());
                hNname<<"0";
                NumOfAll0[i]=(TH1F*)f->Get(hNname.str().c_str());
                stringstream htname;
                htname<<"LiveTime/AD";
                htname<<i+1;
                htname<<"SingleLiveTime";
                SingleLiveTime[i]=(TH1F*)f->Get(htname.str().c_str());
                htname<<"0";
                SingleLiveTime0[i]=(TH1F*)f->Get(htname.str().c_str());
                stringstream htname200;
                htname200<<"LiveTime/AD";
                htname200<<i+1;
                htname200<<"SingleLiveTime200";
                SingleLiveTime200[i]=(TH1F*)f->Get(htname200.str().c_str());
                htname200<<"0";
                SingleLiveTime2000[i]=(TH1F*)f->Get(htname200.str().c_str());
                stringstream hNname200;
                hNname200<<"LiveTime/AD";
                hNname200<<i+1;
                hNname200<<"NumOfSingleLiveTime200";
                NumOfSingleLiveTime200[i]=(TH1F*)f->Get(hNname200.str().c_str());
                hNname200<<"0";
                NumOfSingleLiveTime2000[i]=(TH1F*)f->Get(hNname200.str().c_str());
                //every binNum for each run
                /*
                   std::cout<< runnum<<" : ";
                   TAxis *xaxis = NumOfSingleLiveTime200[i]->GetXaxis();
                   int binNum = xaxis->GetNbins();
                   for( int ii=1 ; ii<binNum ; ii++ )
                   {
                   if( NumOfSingleLiveTime200[i]->GetBinContent(ii)!=0 )
                   {
                   std::cout<<" "<<ii;
                   }
                   }
                   std::cout<<" "<<endl;
                 */
                //
                if( effMultiHistInt<ADNumOfSite[siteNum-1] )
                {
                    tNumOfAll[i] = new TH1F(*NumOfAll[i]);
                    tNumOfAll0[i] = new TH1F(*NumOfAll0[i]);
                    tSingleLiveTime[i]=new TH1F(*SingleLiveTime[i]);
                    tSingleLiveTime0[i]=new TH1F(*SingleLiveTime0[i]);
                    tSingleLiveTime200[i]=new TH1F(*SingleLiveTime200[i]);
                    tSingleLiveTime2000[i]=new TH1F(*SingleLiveTime2000[i]);
                    tNumOfSingleLiveTime200[i]=new TH1F(*NumOfSingleLiveTime200[i]);
                    tNumOfSingleLiveTime2000[i]=new TH1F(*NumOfSingleLiveTime2000[i]);
                    RateOfAll[i]=new TH1F(*NumOfAll[i]);
                    RateOfAll[i]->Add(NumOfAll[i],NumOfAll[i],0,0);
                    RateOfAll0[i]=new TH1F(*NumOfAll0[i]);
                    RateOfAll0[i]->Add(NumOfAll0[i],NumOfAll0[i],0,0);

                    effMultiHistInt++;

                } else
                {
                    tNumOfAll[i]->Add(tNumOfAll[i],NumOfAll[i],1,1);
                    tNumOfAll0[i]->Add(tNumOfAll0[i],NumOfAll0[i],1,1);
                    tSingleLiveTime[i]->Add(tSingleLiveTime[i],SingleLiveTime[i],1,1);
                    tSingleLiveTime0[i]->Add(tSingleLiveTime0[i],SingleLiveTime0[i],1,1);
                    tSingleLiveTime200[i]->Add(tSingleLiveTime200[i],SingleLiveTime200[i],1,1);
                    tSingleLiveTime2000[i]->Add(tSingleLiveTime2000[i],SingleLiveTime2000[i],1,1);
                    tNumOfSingleLiveTime200[i]->Add(tNumOfSingleLiveTime200[i],NumOfSingleLiveTime200[i],1,1);
                    tNumOfSingleLiveTime2000[i]->Add(tNumOfSingleLiveTime2000[i],NumOfSingleLiveTime2000[i],1,1);
                }
            }

        }

        //Li9 He8
        if(anaLiHe)
        {
            for( int i=0 ; i<6 ; i++ )
            {
                TString hnameLi;
                hnameLi="lidj/showermuonNum";
                hnameLi+=i+1;
                showermuonNum[i]=(TH1F*)f->Get(hnameLi);
                hnameLi+="0";
                showermuonNum0[i]=(TH1F*)f->Get(hnameLi);
                hnameLi="lidj/time2lastshowermuon";
                hnameLi+=i+1;
                time2lastshowermuon[i]=(TH1F*)f->Get(hnameLi);
                hnameLi+="0";
                time2lastshowermuon0[i]=(TH1F*)f->Get(hnameLi);
                if( LiHistInt<6 )
                {
                    hnameLi="lidj/time2lastshowermuon";
                    hnameLi+=i+1;
                    ttime2lastshowermuon[i]=(TH1F*)f->Get(hnameLi);
                    hnameLi+="0";
                    ttime2lastshowermuon0[i]=(TH1F*)f->Get(hnameLi);
                    hnameLi="lidj/showermuonNum";
                    hnameLi+=i+1;
                    tshowermuonNum[i]=(TH1F*)f->Get(hnameLi);
                    hnameLi+="0";
                    tshowermuonNum0[i]=(TH1F*)f->Get(hnameLi);
                    LiHistInt++;
                }else
                {
                    ttime2lastshowermuon[i]->Add(ttime2lastshowermuon[i],time2lastshowermuon[i],1,1);
                    ttime2lastshowermuon0[i]->Add(ttime2lastshowermuon0[i],time2lastshowermuon0[i],1,1);
                    tshowermuonNum[i]->Add(tshowermuonNum[i],showermuonNum[i],1,1);
                    tshowermuonNum0[i]->Add(tshowermuonNum0[i],showermuonNum0[i],1,1);
                }
            }
        }
    }

    //===>analyse 
    TAxis *xaxis = th[0]->GetXaxis();
    int binNum = xaxis->GetNbins();
    int BinBound=th[0]->FindBin(1346428800);//2012.9.1 0:0:0
    //livetime
    if( BinBound!=0 )
    {
        if( site=="EH2" )
        {
            for(int j=1  ; j<=BinBound ; j++ )
            {
                th[1]->SetBinContent(j,0);
                th0[1]->SetBinContent(j,0);
                th_200[1]->SetBinContent(j,0);
                th0_200[1]->SetBinContent(j,0);
            }
        }
        if( site=="EH3" )
        {
            for(int j=1  ; j<=BinBound ; j++ )
            {
                th[3]->SetBinContent(j,0);
                th0[3]->SetBinContent(j,0);
                th_200[3]->SetBinContent(j,0);
                th0_200[3]->SetBinContent(j,0);
            }
        }
    }
    double totalTimeSpecical=0.;
    for( int j=BinBound ; j<=binNum ; j++ )
    {
        totalTimeSpecical+=th[4]->GetBinContent(j);
    }

    for( int i=0 ; i<5 ; i++ )
    {
        totalTime[i]=0.;
        for( int j=1 ; j<=binNum ; j++ )
        {
            totalTime[i]+=th[i]->GetBinContent(j);
            if( i!=4 )
            {
                totalTime0[i]+=th0[i]->GetBinContent(j);
            }
        }

    }
    //muon
    if( anamuon )
    {
        for( int i=0 ; i<7 ; i++ )
        {
            for( int j=1 ; j<=binNum ; j++ )
            {
                //muonRate[i]->SetBinContent(j,tmuonNum[i]->GetBinContent(j)/th[4]->GetBinContent(j));
                if(th[4]->GetBinContent(j)!=0)
                {
                    muonRate[i]->Fill(th[4]->GetBinCenter(j),tmuonNum[i]->GetBinContent(j)/th[4]->GetBinContent(j));
                }
            }
        }
    }
    //singles
    if( anasingle )
    {
        for( int i=0 ; i<ADNumOfSite[siteNum-1] ; i++ )
        {
            for( int j=1 ; j<=binNum ; j++ )
            {
                if( th[4]->GetBinContent(j)!=0 &&th[i]->GetBinContent(j)!=0&&th_200[i]->GetBinContent(j)!=0&&th0[i]->GetBinContent(j)!=0&&th0_200[i]->GetBinContent(j)!=0)
                {
                    NRate[i]->Fill(th[4]->GetBinCenter(j),tNumN[i]->GetBinContent(j)/th[i]->GetBinContent(j));
                    ERate[i]->Fill(th[4]->GetBinCenter(j),tNumE[i]->GetBinContent(j)/th_200[i]->GetBinContent(j));
                    ERate_600[i]->Fill(th[4]->GetBinCenter(j),tNumE_600[i]->GetBinContent(j)/th[i]->GetBinContent(j));
                    NumAccBkg[i]+=(tNumN[i]->GetBinContent(j))*(1-exp((ERate[i]->GetBinContent(j))*(-1)*(1.99e-4)));
                    NumAccBkg_600[i]+=(tNumN[i]->GetBinContent(j))*(1-exp((ERate_600[i]->GetBinContent(j))*(-1)*(1.99e-4)));
                    NRate0[i]->Fill(th[4]->GetBinCenter(j),tNumN0[i]->GetBinContent(j)/th0[i]->GetBinContent(j));
                    ERate0[i]->Fill(th[4]->GetBinCenter(j),tNumE0[i]->GetBinContent(j)/th0_200[i]->GetBinContent(j));
                    ERate0_600[i]->Fill(th[4]->GetBinCenter(j),tNumE0_600[i]->GetBinContent(j)/th0[i]->GetBinContent(j));
                    NumAccBkg0[i]+=(tNumN0[i]->GetBinContent(j))*(1-exp((ERate0[i]->GetBinContent(j))*(-1)*(1.99e-4)));
                    NumAccBkg0_600[i]+=(tNumN0[i]->GetBinContent(j))*(1-exp((ERate0_600[i]->GetBinContent(j))*(-1)*(1.99e-4)));
                }
            }

        }
    }

    //eff multi
    if( anaEffMulti )
    {
        for( int i=0 ; i<ADNumOfSite[siteNum-1] ; i++ )
        {
            int totalBin=0;
            int badBin=0;
            for( int j=1 ; j<=binNum ; j++ )
            {
                if( th[4]->GetBinContent(j)!=0&&th[i]->GetBinContent(j)!=0&&th0[i]->GetBinContent(j)!=0 )
                {
                    RateOfAll[i]->Fill(th[4]->GetBinCenter(j),tNumOfAll[i]->GetBinContent(j)/th[i]->GetBinContent(j));
                    RateOfAll0[i]->Fill(th[4]->GetBinCenter(j),tNumOfAll0[i]->GetBinContent(j)/th0[i]->GetBinContent(j));

                    /*
                       std::cout<<"1-RateOfAll[i]->GetBinContent(j)*2.e-4  : "<<1-RateOfAll[i]->GetBinContent(j)*2.e-4<<endl;
                       std::cout<<"(1-RateOfAll[i]->GetBinContent(j)*meanCaptureTime[siteNum-1][i])  : "<<(1-RateOfAll[i]->GetBinContent(j)*meanCaptureTime[siteNum-1][i])<<endl;
                       std::cout<<"(1-RateOfAll[i]->GetBinContent(j)*2.e-4)*tSingleLiveTime200[i]->GetBinContent(j)  : "<<(1-RateOfAll[i]->GetBinContent(j)*2.e-4)*tSingleLiveTime200[i]->GetBinContent(j)<<endl;
                       std::cout<<"RateOfAll[i]->GetBinContent(j)*2.e-4*1.e-4*tNumOfSingleLiveTime200[i]->GetBinContent(j)  : "<<RateOfAll[i]->GetBinContent(j)*2.e-4*1.e-4*tNumOfSingleLiveTime200[i]->GetBinContent(j)<<endl;
                       std::cout<<"tSingleLiveTime[i]->GetBinContent(j)  : "<<tSingleLiveTime[i]->GetBinContent(j)<<endl;
                       std::cout<<"RateOfAll[i]->GetBinContent(j)  : "<<RateOfAll[i]->GetBinContent(j)<<endl;
                       std::cout<<"meanCaptureTime[siteNum-1][i]  : "<<meanCaptureTime[siteNum-1][i]<<endl;
                       std::cout<<"tSingleLiveTime200[i]->GetBinContent(j)  : "<<tSingleLiveTime200[i]->GetBinContent(j)<<endl;
                       std::cout<<"tNumOfSingleLiveTime200[i]->GetBinContent(j): "<<tNumOfSingleLiveTime200[i]->GetBinContent(j)<<endl;
                       std::cout<<"tSingleLiveTime[i]->GetBinContent(j)  : "<<tSingleLiveTime[i]->GetBinContent(j)<<endl;
                     */

                    double tempTi=(1-RateOfAll[i]->GetBinContent(j)*2.e-4)
                        //effTime[i]+=(1-RateOfAll[i]->GetBinContent(j)*2.e-4)
                        *
                        (1-RateOfAll[i]->GetBinContent(j)*meanCaptureTime[siteNum-1][i])
                        *
                        ((1-RateOfAll[i]->GetBinContent(j)*2.e-4)*tSingleLiveTime200[i]->GetBinContent(j)
                         +
                         RateOfAll[i]->GetBinContent(j)*2.e-4*1.e-4*tNumOfSingleLiveTime200[i]->GetBinContent(j)
                         +
                         tSingleLiveTime[i]->GetBinContent(j)
                        );
                    effTime[i]+=tempTi;
                    /*
                       std::cout<<"tempTi/livetime  : "<<tempTi/th[i]->GetBinContent(j)<<endl;
                       std::cout<<"tSingleLiveTime>200  : "<<tSingleLiveTime200[i]->GetBinContent(j)<<endl;
                       std::cout<<"tSingleLiveTime<200  : "<<tSingleLiveTime[i]->GetBinContent(j)<<endl;
                       std::cout<<"livetime  : "<<th[i]->GetBinContent(j)<<endl;
                       std::cout<<"ratio   : "<<(tSingleLiveTime200[i]->GetBinContent(j)+tSingleLiveTime[i]->GetBinContent(j))/th[i]->GetBinContent(j)<<endl;
                     */
                    totalBin++;
                    if( tSingleLiveTime200[i]->GetBinContent(j)>th[i]->GetBinContent(j)+1 )
                    {
                        //if( j==2215 )
                        //{
                        //std::cout<<"binNum  : "<<j<<endl;
                        //std::cout<<"tempTi/livetime  : "<<tempTi/th[i]->GetBinContent(j)<<endl;
                        //std::cout<<"tSingleLiveTime>200  : "<<tSingleLiveTime200[i]->GetBinContent(j)<<endl;
                        //std::cout<<"tSingleLiveTime<200  : "<<tSingleLiveTime[i]->GetBinContent(j)<<endl;
                        //std::cout<<"livetime  : "<<th[i]->GetBinContent(j)<<endl;
                        //std::cout<<"ratio   : "<<(tSingleLiveTime200[i]->GetBinContent(j)+tSingleLiveTime[i]->GetBinContent(j))/th[i]->GetBinContent(j)<<endl;
                        //}
                        badBin++;
                    }
                    effTime0[i]+=(1-RateOfAll0[i]->GetBinContent(j)*2.e-4)
                        *
                        (1-RateOfAll0[i]->GetBinContent(j)*meanCaptureTime[siteNum-1][i])
                        *
                        ((1-RateOfAll0[i]->GetBinContent(j)*2.e-4)*tSingleLiveTime2000[i]->GetBinContent(j)
                         +
                         RateOfAll0[i]->GetBinContent(j)*2.e-4*1.e-4*tNumOfSingleLiveTime2000[i]->GetBinContent(j)
                         +
                         tSingleLiveTime0[i]->GetBinContent(j)
                        );
                    //effTime0[i]+=(1-RateOfAll0[i]->GetBinContent(j)*2.e-4)*(1-RateOfAll0[i]->GetBinContent(j)*meanCaptureTime[siteNum-1][i])*((1-RateOfAll0[i]->GetBinContent(j)*2.e-4*(1-1.e-4/tSingleLiveTime2000[i]->GetBinContent(j)))+(1-RateOfAll0[i]->GetBinContent(j)*tSingleLiveTime0[i]->GetBinContent(j)/2));
                    //std::cout<<"effTime[i]  : "<<effTime[i]<<endl;
                    //std::cout<<"effTime0[i]  : "<<effTime0[i]<<endl;
                    //std::cout<<" "<<endl;
                }
            }
            //std::cout<<"effTime[i]  : "<<effTime[i]<<endl;
            //std::cout<<"effTime0[i]  : "<<effTime0[i]<<endl;
            //std::cout<<"totalBin  : "<<totalBin<<endl;
            //std::cout<<"badBin  : "<<badBin<<endl;

        }

    }
    //Li He
    std::cout<<"begin to analyse Li "<<endl;
    if(anaLiHe)
    {
        //TCanvas* c = new TCanvas("c","c",2000,800) ; 
        //c->Divide(3,2);
        TString xTitle[6]={"0.02~0.5GeV","0.5~1.5GeV","1.5~2.5GeV","2.5~3.5GeV","3.5~4.5GeV",">4.5GeV"};
        double showerTh[7] = {0.02, 0.5, 1.5, 2.5, 3.5, 4.5, 5.0};
        TH1F* LiResult[5];
        LiResult[0]= new TH1F("LiResult", "All", 6, showerTh);
        LiResult[1]= new TH1F("LiResult", "AD1", 6, showerTh);
        LiResult[2]= new TH1F("LiResult", "AD2", 6, showerTh);
        LiResult[3]= new TH1F("LiResult", "AD3", 6, showerTh);
        LiResult[4]= new TH1F("LiResult", "AD4", 6, showerTh);
        double NumMuon[6]={0.};
        double NumMuon0[6]={0.};
        double RateMuon[6]={0.};
        double RateMuon0[6]={0.};
        double NumIbd[6]={0.};
        double NumIbd0[6]={0.};

        RooRealVar x("x","x",0,40., "s");
        RooRealVar tauLi9("tauLi9", "tauLi9", -0.257, "s");
        RooRealVar tauHe8("tauHe8", "tauHe8", -0.172, "s");
        RooRealVar rateMu("rateMu","rate of showermuon",-0.1,-10., 0.,"Hz");
        RooRealVar Li9Frac("Li9Frac","Li9's Frac", 0.0);//R
        RooRealVar N98("N98","total number of Li9 and He8",500.,0.,1.e6);
        //RooRealVar N98("N98","total number of Li9 and He8",3e2, 1e1, 2.e5);
        RooFormulaVar lambdaLi9("lambdaLi9","lambdaLi9","1/@0 + @1",RooArgList(tauLi9, rateMu));
        RooFormulaVar lambdaHe8("lambdaHe8","lambdaHe8","1/@0 + @1",RooArgList(tauHe8, rateMu));
        RooFormulaVar coeLi9("coeLi9","coeLi9","@0 * @1 ",RooArgList(N98,Li9Frac));
        RooFormulaVar coeHe8("coeHe8","coeHe8","@0 * ( 1 - @1 )",RooArgList(N98,Li9Frac));
        RooRealVar NIbd("NIbd","number of background",3e2, 1e1, 1.e6);
        RooExponential expLi9("expLi9", "Li9 distribution", x, lambdaLi9);
        RooExponential expHe8("expHe8", "He8 distribution", x, lambdaHe8);
        double Ibdrate[3]={0.0076,0.0067,0.00082};
        RooRealVar rateIbd("rateIbd","rateIbd",-Ibdrate[siteNum-1],-10,0.,"Hz");
        RooFormulaVar lambdaIbd("lambdaIbd","lambdaIbd","@0 + @1",RooArgList(rateIbd, rateMu));
        RooExponential expIbd("expIbd","Ibd distribution",x,rateMu);
        //RooExponential expIbd("expIbd","Ibd distribution",x,lambdaIbd);

        RooFitResult* fitres;
        RooAddPdf* sum;
        RooDataHist* data;
        RooPlot* mesframe[6];
        double n98[41]={0.};
        double in98[41]={0.};
        double nIbd[41]={0.};
        double inIbd[41]={0.};
        double minNLL[41]={0.};
        double rateMuValue[41]={0.};
        double rateMuErr[41]={0.};
        //double minNl[6]={0.};
        int minindex[6]={0};
        double n98fit[6]={0.};
        double in98fit[6]={0.};
        double rateMufit[6]={0.};
        int minindex0[6]={0};
        double n98fit0[6]={0.};
        double in98fit0[6]={0.};
        double binwidth=0.;
        //TString zfName="/publicfs/dyb/data/userdata/zhangfh/IbdAna/ana/HeLiAna/P13Etot/";
        //zfName+=site;
        //zfName+="/mergedHist.root";
        //TFile* zf=new TFile(zfName);
        //TH1F* zh[6];
        TString lfName="/workfs/dyw/lidj/IBDSel/job/";
        lfName+=dataVer+"/"+site;
        lfName+="Li_";
        lfName+=dataVer;
        lfName+=".root";
        //TFile* lf=new TFile(lfName,"RECREATE");
        TFile* lf=new TFile(lfName);
        if( lf->IsZombie() )
        {
            std::cout<<"LiHisto File doesn't exit ,please check :"<<lfName<<endl;
            return 0;
        }
        TH1F* lh[5][6];
        TString lname[5]={"","AD1","AD2","AD3","AD4"};
        int lcolor[5]={4,3,2,6,5};
        //for( int ihist=0 ; ihist<ADNumOfSite[siteNum-1]+1 ; ihist++ )
        for( int ihist=0 ; ihist<1 ; ihist++ )
        {
            double minNl[6]={0.};

            for( int j=0 ; j<6 ; j++ )
                //for( int j=0 ; j<1 ; j++ )
            {
                std::cout<<"now is  "<< j+1<<endl;
                //TString zhName="zhangfh/time2LastMuonRed";
                //zhName+=j+1;
                //zh[j]=(TH1F*)zf->Get(zhName);
                //zh[j]->Rebin(10);
                //ttime2lastshowermuon[j]->Rebin(5);
                TString lnameTmp=lname[ihist];
                lnameTmp+="time2lastshowermuon";
                lnameTmp+=j+1;

                lh[ihist][j]=(TH1F*)lf->Get(lnameTmp);
                lh[ihist][j]->Rebin(10);
                //data = new RooDataHist("data", "data", x, ttime2lastshowermuon[j]);
                //data = new RooDataHist("data", "data", x, zh[j]);
                data = new RooDataHist("data", "data", x, lh[ihist][j]);
                sum  = new RooAddPdf("sum","sum",RooArgList(expLi9, expHe8, expIbd),RooArgList(coeLi9, coeHe8, NIbd));
                for( int i=0 ; i<41 ; i++ )
                    //for( int i=35 ; i<36 ; i++ )
                {
                    Li9Frac.setVal(i*0.025);
                    fitres = sum->fitTo((*data),Save(),PrintLevel(-1),Extended(kTRUE));
                    //fitres->Print();
                    n98[i]=N98.getVal(0);
                    in98[i]=N98.getError();
                    nIbd[i]=NIbd.getVal(0);
                    inIbd[i]=NIbd.getError();
                    rateMuValue[i]=rateMu.getVal(0);
                    rateMuErr[i]=rateMu.getError();
                    minNLL[i] = fitres->minNll();
                    if( minNLL[i]<minNl[j] )
                    {
                        minNl[j]=minNLL[i];
                        minindex[j]=i;
                    }
                }
                //TString cName="c";
                //cName+=j+1;
                //TCanvas* c = new TCanvas(cName,cName,1200,400) ; 
                n98fit[j]=n98[minindex[j]];
                in98fit[j]=in98[minindex[j]];
                rateMufit[j]=rateMuValue[minindex[j]];
                //for drawing fit figure
                Li9Frac.setVal(minindex[j]*0.025);
                fitres = sum->fitTo((*data),Save(),PrintLevel(-1),Extended(kTRUE));
                //fitres->Print();
                /*
                   mesframe[j] = x.frame() ;
                   data->plotOn(mesframe[j]) ;
                   sum->plotOn(mesframe[j]);
                //sum->paramOn(mesframe[j]);
                sum->plotOn(mesframe[j],Components(expIbd),LineStyle(kDashed),LineColor(kGreen)) ;
                sum->plotOn(mesframe[j],Components(RooArgSet(expLi9,expHe8)),LineStyle(kDashed),LineColor(kRed)) ;
                xTitle[j]+="        time since last muon (s)";
                mesframe[j]->GetXaxis()->SetTitle(xTitle[j]);
                mesframe[j]->GetYaxis()->SetTitle("Entries");
                mesframe[j]->SetMarkerSize(0.2);

                //c->cd(j+1);
                //mesframe[j]->Draw();
                //c->SetLogx();
                 */
                /*
                   for( int i=0 ; i<41 ; i++ )
                   {
                   std::cout<<" i="<< i<< setprecision(5)<<setiosflags(ios::fixed)<<"   minNLL["<<i<<"] "<<minNLL[i]<<setprecision(6)<<"   n98["<<i<<"] "<<n98[i]<<"   nIbd["<<i<<"] "<<nIbd[i]<<"   rateMuValue["<<i<<"] "<<rateMuValue[i]<<endl;
                   }
                 */
            }
            /*
            //for( int j=0 ; j<6 ; j++ )
            for( int j=0 ; j<1 ; j++ )
            {

            rateMu.setVal(-RateMuon0[j]);
            //rateMu.setVal(-0.23);
            //NEvent.setVal(NumIbd[j]*binwidth);
            //NIbd.setRange(NumIbd[j]-1000,NumIbd[j]);
            //ttime2lastshowermuon[j]->Rebin(25);
            binwidth=ttime2lastshowermuon0[j]->GetBinWidth(1);
            //NEvent.setVal(NumIbd[j]*binwidth);
            data = new RooDataHist("data", "data", x, ttime2lastshowermuon0[j]);
            //	for( int i=0 ; i<41 ; i++ )
            for( int i=35 ; i<36 ; i++ )
            {
            Li9Frac.setVal(i*0.025);
            sum  = new RooAddPdf("sum","sum",RooArgList(expLi9, expHe8, expIbd),RooArgList(coeLi9, coeHe8, coeIbd));
            fitres = sum->fitTo((*data),Save());
            n98[i]=N98.getVal(0);
            in98[i]=N98.getError();
            minNLL[i] = fitres->minNll();
            if( minNLL[i]<minNl[j] )
            {
            minNl[j]=minNLL[i];
            minindex0[j]=i;
            }
            }
            n98fit0[j]=n98[minindex[j]]/binwidth;
            in98fit0[j]=in98[minindex[j]]/binwidth;
            }
             */

            n98total=(n98fit[0]+n98fit[1]+n98fit[2]+(n98fit[3]+n98fit[4]+n98fit[5])*exp(-1/0.257))/0.678;
            ifitn98total=sqrt(in98fit[0]*in98fit[0]+in98fit[1]*in98fit[1]+in98fit[2]*in98fit[2]+((in98fit[3]+in98fit[4]+in98fit[5])*exp(-1/0.257)*(in98fit[3]+in98fit[4]+in98fit[5])*exp(-1/0.257)))/0.678;
            if( ihist==0 )
            {
                for( int i=0 ; i<ADNumOfSite[siteNum-1] ; i++ )
                {
                    tlivetime+=totalTime[i];
                }
            }else
            {
                tlivetime=totalTime[ihist-1];
            }
            n98Rate=n98total/(tlivetime/(24*3600));
            in98Rate=sqrt((ifitn98total/(tlivetime/(24*3600)))*(ifitn98total/(tlivetime/(24*3600)))+(n98total/(tlivetime/(24*3600))/2)*(n98total/(tlivetime/(24*3600))/2));
            for( int j=0 ; j<6 ; j++ )
            {
                //std::cout<<" minindex="<< minindex[j]<<setprecision(5)<<setiosflags(ios::fixed)<<"   minNLL["<<minindex[j] <<"] "<<minNLL[minindex[j]]<<setprecision(6)<<"   n98["<<minindex[j]<<"] "<<n98[minindex[j]]<<"   nIbd["<<minindex[j]<<"] "<<nIbd[minindex[j]]<<"   rateMuValue["<<minindex[j]<<"] "<<rateMuValue[minindex[j]]<<endl;
                LiResult[ihist]->SetBinContent(j+1,n98fit[j]);
                LiResult[ihist]->SetBinError(j+1,in98fit[j]);
                for( int jbin=0 ; jbin<binNum ; jbin++ )
                {
                    NumMuon[j]+=tshowermuonNum[j]->GetBinContent(jbin);
                    NumMuon0[j]+=tshowermuonNum0[j]->GetBinContent(jbin);
                }
                RateMuon[j]=NumMuon[j]/totalTime[4]/ADNumOfSite[siteNum-1];
                RateMuon0[j]=NumMuon0[j]/totalTime[4]/ADNumOfSite[siteNum-1];
                //NumIbd[j]=ttime2lastshowermuon[j]->GetEntries();
                //NumIbd0[j]=ttime2lastshowermuon0[j]->GetEntries();
                NumIbd[j]=ttime2lastshowermuon[j]->Integral(1,ttime2lastshowermuon[j]->FindBin(40));
                NumIbd0[j]=ttime2lastshowermuon0[j]->GetEntries();
                //std::cout<<"RateMuon[j]  : "<<RateMuon[j]<<endl;
                //std::cout<<"RateMuon0[j]  : "<<RateMuon0[j]<<endl;
                std::cout<<"  R "<<minindex[j]*0.025<<"   n98["<<minindex[j]<<"] "<<n98fit[j]<<"   in98["<<minindex[j]<<"] "<< in98fit[j]<<"   fitRateMu["<<minindex[j]<<"] "<<rateMufit[j]<<"  realRateMu "<<RateMuon[j]<<"  nIbd["<<j<<"] "<<nIbd[j]<<"   NumIbd "<<NumIbd[j]<<endl;
            }
            std::cout<<"n98Num   : "<<n98total<<" +- "<<ifitn98total<<" Rate  : "<<n98Rate<<" +- "<< in98Rate<<endl;
        }

        /*
        //TCanvas* c4 = new TCanvas("c4", "c4", 600, 400);
        //gStyle->SetEndErrorSize(0.0);
        LiResult[0]->SetMarkerStyle(20);
        LiResult[0]->SetMarkerColor(lcolor[0]);
        LiResult[0]->SetLineColor(lcolor[0]);
        LiResult[0]->SetMarkerSize(1.0);
        LiResult[0]->SetMinimum(0);
        LiResult[0]->SetMaximum(LiResult[0]->GetMaximum()*1.5);
        LiResult[0]->GetXaxis()->SetTitle("Muon visible energy (GeV)");
        LiResult[0]->GetYaxis()->SetTitle("Fitted ^{9}Li events");
        LiResult[0]->SetTitle("");
        LiResult[0]->SetStats(kFALSE);
        LiResult[0]->Draw("EP");
        TLegend *legend=new TLegend(.6,.65,.79,.89);
        TString legendLabel=dataVer+" "+site+" "+"All";
        legend->AddEntry(LiResult[0],legendLabel,"lp");

        for( int ihist=1 ; ihist<ADNumOfSite[siteNum-1]+1; ihist++ )
        {
        LiResult[ihist]->SetMarkerStyle(20);
        LiResult[ihist]->SetMarkerColor(lcolor[ihist]);
        LiResult[ihist]->SetLineColor(lcolor[ihist]);
        LiResult[ihist]->SetMarkerSize(1.0);
        LiResult[ihist]->SetStats(kFALSE);
        LiResult[ihist]->Draw("EPsame");
        legend->AddEntry(LiResult[ihist],lname[ihist],"lp");
        }
        legend->SetFillColor(0);
        legend->Draw();
         */
        lf->Close();
    }	

    //===>write result .txt
    /*
       double i_title[8];//0
       for( int i=0 ; i<7 ; i++ )
       {
       i_title[i]=i+1;
       }

       double n_ibdNum[8]={0.};//[1]
       double n_daqTime[8]={0.};//[2]
       double n_epsi_mu[8]={0.};//[3 ]
       double n_epsi_multi[8]={0.};//[4]
       double n_accNum[8]={0.};//[5]
       double n_accNum_err[8]={0.};//[6]
       double n_accRate[8]={0.};//[7]
       double n_accRate_err[8]={0.};//[8]
       double n_accNum_600[8]={0.};//[9]
       double n_accNum_600_err[8]={0.};//[10]
       double n_accRate_600[8]={0.};//[11]
       double n_accRate_600_err[8]={0.};//[12]
       double n_fnNum[8]={0.};//[13]
       double n_fnNum_err[8]={0.};//[14]
       double n_fnBs[8]={0.};//[15]
       double n_fnBs_err[8]={0.};//[16]
       double n_fnRate[8]={0.};//[17]
       double n_fnRate_err[8]={0.};//[18]
       double i_newline[8]={0.};]
       double w_ibdNum[8]={0.};//[20]
       double w_ibdNum_ratio[8]={0.};//[21]
       double w_daqTime[8]={0.};//[22]
       double w_daqTime_ratio[8]={0.};//[23]
       double w_epsi_mu[8]={0.};//[24]
       double w_epsi_mu_ratio[8]={0.};//[25]
       double w_epsi_multi[8]={0.};//[26]
       double w_epsi_multi_ratio[8]={0.};//[27]
       double w_accNum[8]={0.};//[28]
       double w_accNum_err[8]={0.};//[29]
       double w_accNum_ratio[8]={0.};//[30]
       double w_accRate[8]={0.};//[31]
       double w_accRate_err[8]={0.};//[32]
       double w_accRate_ratio[8]={0.};//[33]
       double w_accNum_600[8]={0.};//[34]
       double w_accNum_600_err[8]={0.};//[35]
       double w_accNum_600_ratio[8]={0.};//[36]
       double w_accRate_600[8]={0.};//[37]
       double w_accRate_600_err[8]={0.};//[38]
       double w_accRate_600_ratio[8]={0.};//[39]
       double w_fnNum[8]={0.};//[40]
       double w_fnNum_err[8]={0.};//[41]
       double w_fnNum_ratio[8]={0.};//[42]
       double w_fnBs[8]={0.};//[43]
       double w_fnBs_err[8]={0.};//[44]
       double w_fnBs_ratio[8]={0.};//[45]
       double w_fnRate[8]={0.};//[46]
       double w_fnRate_err[8]={0.};//[47]
       double w_fnRate_ratio[8]={0.};//[48]
     */

    double t_result[53][8];
    //TString t_result[49][8];
    for( int i=0 ; i<53 ; i++ )
    {
        for( int j=0 ; j<8 ; j++ )
        {
            t_result[i][j]=0.;
        }

    }

    //std::cout<<"begin to read file "<<endl;
    ifstream infile; 
    string infileName=dataVer+"/"+"result_temp_"+dataVer+".txt";
    infile.open(infileName.c_str(),ios::in);
    int inLineNum=0;
    std::cout<<">>>>>>>>>>>>get value "<<endl;
    //while(!infile.eof())
    while(inLineNum<53)
    {
        for( int j=0 ; j<8 ; j++ )
        {
            infile>>t_result[inLineNum][j];
            //std::cout<<t_result[inLineNum][j]<<" " ;
        }
        //std::cout<<endl;
        inLineNum++;
    }
    //std::cout<<"finished  "<<endl;

    infile.close();
    int ii=0;
    if( siteNum==1 )
    {
        ii=0;
    } else if( siteNum==2 )
    {
        ii=2;
    }else if( siteNum==3 )
    {
        ii=4;
    }else
    {
        std::cout<<"error  : wrong siteNum"<<endl;    
    }
    t_result[0][0]=1;
    t_result[0][1]=2;
    t_result[0][2]=3;
    t_result[0][3]=8;
    t_result[0][4]=4;
    t_result[0][5]=5;
    t_result[0][6]=6;
    t_result[0][7]=7;
    for( int i=0 ; i<ADNumOfSite[siteNum-1] ; i++ )
    {
        double effOfMulti=effTime[i]/totalTime[i];
        double effOfMulti0=effTime0[i]/totalTime0[i];
        if( (site=="EH3"||site=="EH2")&&i==(ADNumOfSite[siteNum-1]-1) )
        {
            t_result[2][i+ii]=totalTimeSpecical/(3600*24);
            t_result[3][i+ii]=totalTime0[i]/totalTimeSpecical;
        }else
        {
            t_result[2][i+ii]=totalTime[4]/(3600*24);

            t_result[3][i+ii]=totalTime0[i]/totalTime[4];
        }
        t_result[4][i+ii]=effTime0[i]/totalTime0[i];
        t_result[5][i+ii]=NumAccBkg0[i];
        if( totalN0[i]==0 )
        {
            t_result[6][i+ii]=0;
            t_result[8][i+ii]=0;
            t_result[10][i+ii]=0;
            t_result[12][i+ii]=0;
        } else
        {
            t_result[6][i+ii]=NumAccBkg0[i]*sqrt(1/(double)totalN0[i]+(0.01*0.01));
            t_result[8][i+ii]=((NumAccBkg0[i]*sqrt(1/(double)totalN0[i]+(0.01*0.01)))/totalTime0[i])*24*3600;
            t_result[10][i+ii]=NumAccBkg0[i]*sqrt(1/(double)totalN0[i]+(0.01*0.01));
            t_result[12][i+ii]=(((NumAccBkg0[i]*sqrt(1/(double)totalN0[i]+(0.01*0.01))))/totalTime0[i])*24*3600;
        }
        t_result[7][i+ii]=(NumAccBkg0[i]/totalTime0[i])*24*3600;
        t_result[9][i+ii]=NumAccBkg0_600[i];
        t_result[11][i+ii]=(NumAccBkg0_600[i]/totalTime0[i])*24*3600;

        t_result[19][i+ii]=n98total;
        t_result[20][i+ii]=ifitn98total;
        t_result[21][i+ii]=n98Rate;
        t_result[22][i+ii]=in98Rate;

        t_result[26][i+ii]=totalTime[4]/(3600*24);
        t_result[27][i+ii]=0;
        t_result[28][i+ii]=totalTime[i]/totalTime[4];
        t_result[29][i+ii]=(totalTime[i]/totalTime[4]-totalTime0[i]/totalTime[4])/(totalTime0[i]/totalTime[4]);
        t_result[30][i+ii]=effTime[i]/totalTime[i];
        t_result[31][i+ii]=(effOfMulti-effOfMulti0)/effOfMulti;
        t_result[32][i+ii]=NumAccBkg[i];
        if( totalN[i]==0 )
        {
            t_result[33][i+ii]=0;
            t_result[36][i+ii]=0;
            t_result[39][i+ii]=0;
            t_result[42][i+ii]=0;
        } else
        {
            t_result[33][i+ii]=NumAccBkg[i]*sqrt(1/(double)totalN[i]+(0.01*0.01));
            t_result[36][i+ii]=((NumAccBkg[i]*sqrt(1/(double)totalN[i]+(0.01*0.01)))/totalTime[i])*24*3600;
            t_result[39][i+ii]=NumAccBkg[i]*sqrt(1/(double)totalN[i]+(0.01*0.01));
            t_result[42][i+ii]=(NumAccBkg[i]*sqrt(1/(double)totalN[i]+(0.01*0.01))/totalTime[i])*24*3600;
        }
        if( NumAccBkg0[i]==0 )
        {
            t_result[34][i+ii]=-0;
            t_result[37][i+ii]=-0;
            t_result[40][i+ii]=-0;
            t_result[43][i+ii]=-0;
        } else
        {
            t_result[34][i+ii]=-(NumAccBkg0[i]-NumAccBkg[i])/NumAccBkg0[i];
            t_result[37][i+ii]=-(NumAccBkg0[i]/totalTime0[i]-NumAccBkg[i]/totalTime[i])/(NumAccBkg0[i]/totalTime0[i]);
            t_result[40][i+ii]=-(NumAccBkg0_600[i]-NumAccBkg_600[i])/NumAccBkg0_600[i];
            t_result[43][i+ii]=-(NumAccBkg0_600[i]/totalTime0[i]-NumAccBkg_600[i]/totalTime[i])/(NumAccBkg0_600[i]/totalTime0[i]);
        }
        t_result[35][i+ii]=(NumAccBkg[i]/totalTime[i])*24*3600;
        t_result[38][i+ii]=NumAccBkg_600[i];
        t_result[41][i+ii]=(NumAccBkg_600[i]/totalTime[i])*24*3600;
    }

    //inLineNum=0;
    std::cout<<">>>>>>>after give value "<<endl;
    //while(inLineNum<53)
    //{
    //for( int j=0 ; j<8 ; j++ )
    //{
    //std::cout<<t_result[inLineNum][j]<<" " ;
    //}
    //std::cout<<endl;
    //inLineNum++;
    //}
    ofstream outfile;
    string outfileName=dataVer+"/"+"result_temp_"+dataVer+".txt";
    outfile.open(outfileName.c_str());
    int outLineNum=0;
    std::cout<<">>>>>>>>>>>>>>saved in file "<<endl;
    while( outLineNum<53 )
    {
        for( int i=0 ; i<8 ; i++ )
        {
            //std::cout<<t_result[outLineNum][i]<<" " ;
            outfile<< setw(13) <<t_result[outLineNum][i];
            //outfile<< setw(13) <<outStr[outLineNum][i];
            if( i==7 )
            {
                outfile<<endl;
            }
        }
        //std::cout<<endl;
        outLineNum++;
    }
    outfile.close();

    //===>print result
    std::cout<<""<<endl;
    std::cout<<site <<"'s infomation : "<<endl;
    std::cout<<""<<endl;
    if(anamuon)
    {
        std::cout<<"Total MuonNum : 1.AD1  2.AD2  3.AD3  4.AD4 5.IWS  6.OWS  7.RPC"<<endl;
        for( int i=0 ; i<7 ; i++ )
        {
            std::cout<<i+1<<". MuonNum :"<<totalNum[i]<<" Rate : "<<totalNum[i]/totalTime[4] <<" HZ"<<endl;
        }
        std::cout<<""<<endl;
    }
    for( int i=0 ; i<ADNumOfSite[siteNum-1] ; i++ )
    {
        std::cout<<"Total AD"<<i+1<<"LiveTime                         : "<<totalTime[i]/(24*3600)<<" ("<<(totalTime[i] -totalTime0[i] )/totalTime0[i] <<") day"<<endl;
        std::cout<<"Total AD"<<i+1<<"LiveTime(without rpc) : "<<totalTime0[i]/(24*3600)<<" day"<<endl;
    }
    std::cout<<"Total DaqTime : "<<totalTime[4]/(24*3600)<<" day" <<endl;
    std::cout<<""<<endl;
    if(anasingle)
    {
        for( int i=0 ; i<ADNumOfSite[siteNum-1] ; i++ )
        {
            std::cout<<"AD"<<i+1<<" accidental background "<<endl;
            //std::cout<<"	Num  of accbkg       : "<<NumAccBkg[i]<<" +- "<< NumAccBkg[i]/sqrt((double)totalN[i])<< " ("<<(NumAccBkg0[i]-NumAccBkg[i])/NumAccBkg0[i] <<")"<<endl;
            std::cout<<"NumAccBkg[i]  : "<<NumAccBkg[i]<<endl;
            std::cout<<"(double)totalN[i]  : "<<(double)totalN[i]<<endl;
            std::cout<<"	Num  of accbkg       : "<<NumAccBkg[i]<<" +- "<< NumAccBkg[i]*sqrt(1/(double)totalN[i]+(0.01*0.01))<< " ("<<-(NumAccBkg0[i]-NumAccBkg[i])/NumAccBkg0[i] <<")"<<endl;
            //std::cout<<"	Rate of accbkg(/day) : "<<(NumAccBkg[i]/totalTime[i])*24*3600<<" +- "<<((NumAccBkg[i]/sqrt((double)totalN[i]))/totalTime[i])*24*3600 <<" ("<<(NumAccBkg0[i]/totalTime0[i]-NumAccBkg[i]/totalTime[i])/(NumAccBkg0[i]/totalTime0[i]) <<")"<<endl;
            std::cout<<"	Rate of accbkg(/day) : "<<(NumAccBkg[i]/totalTime[i])*24*3600<<" +- "<<((NumAccBkg[i]*sqrt(1/(double)totalN[i]+(0.01*0.01)))/totalTime[i])*24*3600 <<" ("<<-(NumAccBkg0[i]/totalTime0[i]-NumAccBkg[i]/totalTime[i])/(NumAccBkg0[i]/totalTime0[i]) <<")"<<endl;
            std::cout<<"	Num  of accbkg(without rpc)       : "<<NumAccBkg0[i]<<" +- "<< NumAccBkg0[i]*sqrt(1/(double)totalN0[i]+(0.01*0.01))<<endl;
            std::cout<<"	Rate of accbkg(/day)(without rpc) : "<<(NumAccBkg0[i]/totalTime0[i])*24*3600<<" +- "<<((NumAccBkg0[i]*sqrt(1/(double)totalN0[i]+(0.01*0.01)))/totalTime0[i])*24*3600 <<endl;

            std::cout<<"	Num  of accbkg_600       : "<<NumAccBkg_600[i]<<" +- "<< NumAccBkg[i]*sqrt(1/(double)totalN[i]+(0.01*0.01))<< " ("<<-(NumAccBkg0_600[i]-NumAccBkg_600[i])/NumAccBkg0_600[i] <<")"<<endl;
            std::cout<<"	Rate of accbkg_600(/day) : "<<(NumAccBkg_600[i]/totalTime[i])*24*3600<<" +- "<<(NumAccBkg[i]*sqrt(1/(double)totalN[i]+(0.01*0.01))/totalTime[i])*24*3600 <<" ("<<-(NumAccBkg0_600[i]/totalTime0[i]-NumAccBkg_600[i]/totalTime[i])/(NumAccBkg0_600[i]/totalTime0[i]) <<")"<<endl;
            std::cout<<"	Num  of accbkg_600(without rpc)       : "<<NumAccBkg0_600[i]<<" +- "<< NumAccBkg0[i]*sqrt(1/(double)totalN0[i]+(0.01*0.01))<<endl;
            std::cout<<"	Rate of accbkg_600(/day)(without rpc) : "<<(NumAccBkg0_600[i]/totalTime0[i])*24*3600<<" +- "<<(((NumAccBkg0[i]*sqrt(1/(double)totalN0[i]+(0.01*0.01))))/totalTime0[i])*24*3600 <<endl;
        }
    }
    if( anaEffMulti )
    {
        for( int i=0 ; i<ADNumOfSite[siteNum-1] ; i++ )
        {
            double effOfMulti=effTime[i]/totalTime[i];
            double effOfMulti0=effTime0[i]/totalTime0[i];
            std::cout<<"AD "<<i+1<<" :"<<endl;
            std::cout<<"e-u                : "<<totalTime[i]/totalTime[4]<<" ("<<(totalTime[i]/totalTime[4]-totalTime0[i]/totalTime[4])/(totalTime0[i]/totalTime[4]) <<")"<<endl;
            std::cout<<"e-m                : "<<effOfMulti<<" ("<<(effOfMulti-effOfMulti0)/effOfMulti <<")"<<endl;
            std::cout<<"e-u  (without rpc) : "<<totalTime0[i]/totalTime[4]<<endl;
            std::cout<<"e-m  (without rpc) : "<<effOfMulti0<<endl;
        }

    }

    if(anaLiHe)
    {
        std::cout<<" "<<endl;
        /*
           std::cout<<"Li9/He8 "<<endl;
           std::cout<<"n98total  : "<<n98total <<" +- "<<in98total <<" Rate:"<<n98total/totalTime[0]*24*3600 <<" +- "<<in98total/totalTime[0]*24*3600 <<endl;
           std::cout<<"n98total0(without rpc)  : "<<n98total0 <<" +- "<<in98total0 <<" Rate:"<<n98total0/totalTime0[0]*24*3600 <<" +- "<<in98total0/totalTime0[0]*24*3600 <<endl;
         */
    }

    //===>write into .root file
    std::cout<<">>>>>>>>>>>>save to file "<<endl;
    string rootname=dataVer+"/"+site;
    rootname+="livetime_"+dataVer+".root";
    TFile* file = new TFile(rootname.c_str(),"RECREATE");
    for( int i=0 ; i<4 ; i++ )
    {
        th[i]->Write();
        th0[i]->Write();
        th_200[i]->Write();
        th0_200[i]->Write();
    }
    th[4]->Write();

    if( anaEffMulti )
    {
        for( int i=0 ; i<ADNumOfSite[siteNum-1] ; i++ )
        {
            //RateOfAll[i]->Write();
            //RateOfAll0[i]->Write();
            tNumOfSingleLiveTime200[i]->Write();
            tNumOfSingleLiveTime2000[i]->Write();
            tSingleLiveTime[i]->Write();
            tSingleLiveTime200[i]->Write();
            tSingleLiveTime0[i]->Write();
            tSingleLiveTime2000[i]->Write();
        }
    }
    if(anamuon)
    {
        for( int i=0 ; i<7 ; i++ )
        {
            muonRate[i]->Write();
            tmuonNum[i]->Write();
        }
    }
    if( anaLiHe )
    {
        //for( int i=0 ; i<ADNumOfSite[siteNum-1] ; i++ )
        for( int i=0 ; i<6 ; i++ )
        {
            ttime2lastshowermuon[i]->Write();
        }
    }
    if( anasingle )
    {
        for( int i=0 ; i<ADNumOfSite[siteNum-1] ; i++ )
        {
            //NumN[i]->Write();
            //NumN0[i]->Write();
            //NumE[i]->Write();
            //NumE0[i]->Write();
            //NumE_600[i]->Write();
            //NumE0_600[i]->Write();
            NRate[i]->Write();
            NRate0[i]->Write();
            ERate[i]->Write();
            ERate0[i]->Write();
            ERate_600[i]->Write();
            ERate0_600[i]->Write();
            tNumN[i]->Write();
            tNumN0[i]->Write();
            tNumE[i]->Write();
            tNumE0[i]->Write();
            tNumE_600[i]->Write();
            tNumE0_600[i]->Write();
        }
        //c->Write();

    }
    file->Close();
    return 0;
}

int anaTree(int siteNum,vector<string> dataVer)
{
    gROOT->ProcessLine(".L Ibd.C+");
    TChain chain("Tree/IbdTree");
    string site;
    if( siteNum==1 )
    {   
        site="EH1";
    } else if(siteNum==2)
    {   
        site="EH2";
    }else if (siteNum==3)
    {   
        site="EH3";
    }else
    {   
        site="EH1";
    } 

    string chainFile;

    for( vector<string>::iterator it=dataVer.begin() ; it<dataVer.end()-2 ; it++ )
    {
        chainFile="/afs/ihep.ac.cn/users/l/lidj/largedata/IBD/";
        chainFile+=*it;
        chainFile+=".nGd/";
        chainFile+=site;
        chainFile+="/*IBD.root";
        chain.Add(chainFile.c_str());
    }    

    //chain.Draw("isIbd");
    string  siteAndDataVer=site+dataVer[dataVer.size()-2];
    std::cout<<"DataVerAndSite  : "<<siteAndDataVer<<endl;
    chain.Process("Ibd",siteAndDataVer.c_str());
    return 0;
}

vector<string> checkdata(string dataVer)
{
    string dataIsGood="1";
    string verNum[6]={"11","12","13","14","15","16"};
    vector<string> dataVerReal;
    for( int i=0 ; i<6 ; i++ )
    {
        int pos=0;
        string TmpVer;
        TmpVer=dataVer.substr(pos);
        while( pos!=-1 )
        {
            pos=TmpVer.find(verNum[i]);
            if( pos!=-1 )
            {
                string subdataVer;
                subdataVer="P"+verNum[i];
                subdataVer+=toupper(*(TmpVer.substr(pos+2,1).c_str()));
                for( vector<string>::iterator it=dataVerReal.begin() ; it!=dataVerReal.end() ; ++it  )
                {
                    if( *it==subdataVer )
                    {
                        dataVerReal.erase(it);
                        it--;
                    }
                }
                dataVerReal.push_back(subdataVer);
                TmpVer=TmpVer.substr(pos+3);
            }
        }
    }
    sort(dataVerReal.begin(),dataVerReal.end());

    std::cout<<" "<<endl;
    std::cout<<"Find following data version will be analysed : "<<endl;
    std::cout<<" "<<endl;
    string runlistSiteNum[3]={"EH1","EH2","EH3"};
    vector<string> runlistFileContent[3];
    for( int i=0 ; i<3 ; i++ )
    {
        runlistFileContent[i].clear();
    }
    string verSuf;
    for( vector<string>::iterator it=dataVerReal.begin() ; it!=dataVerReal.end() ; ++it )
    {
        std::cout<<*it<<endl;
        verSuf+="_"+*it;
        //check data path
        string datadir="/afs/ihep.ac.cn/users/l/lidj/largedata/IBD/";
        DIR *dir=NULL;
        datadir+=*it;
        datadir+=".nGd";
        dir = opendir(datadir.c_str());
        if( dir==NULL )
        {
            std::cout<<" !!! data dir doesn't exist : "<<datadir<<endl;
            dataIsGood="0";
        } else
        {
            std::cout<<" data dir exist : "<<datadir<<endl;
            closedir(dir);
        }

        //check runlist
        for( int i=0 ; i<3 ; i++ )
        {
            fstream runlistFile;
            string runlistName=*it+"/"+runlistSiteNum[i]+"run.list.";
            runlistName+=*it;
            runlistFile.open(runlistName.c_str(),ios::in);
            if( !runlistFile )
            {
                std::cout<<" !!! runlist doesn't exist  : "<<runlistName<<endl;
                if( *it!="P12A" )
                {
                    dataIsGood="0";
                }
            } else
            {
                std::cout<<" runlist exist  : "<<runlistName<<endl;
                string histrunName;
                while( getline(runlistFile,histrunName) )
                {
                    if(histrunName=="") continue;
                    runlistFileContent[i].push_back(histrunName);
                }
            }
            runlistFile.close();
        }

        std::cout<<" "<<endl;
    }
    verSuf=verSuf.substr(1);
    std::cout<<"data version : "<<verSuf<<endl;

    //check list/result dir
    string listdirPath=verSuf;
    DIR *listdir=NULL;
    listdir = opendir(listdirPath.c_str());
    if( listdir==NULL )
    {
        std::cout<<" !!! list dir doesn't exist : "<<listdirPath<<endl;
        if( mkdir(verSuf.c_str(),0755)<0 )
        {
            std::cout<<" Failed to create listdir : "<<listdirPath<<endl;
        }else
        {
            std::cout<<" SUCCESSD to create listdir : "<<listdirPath<<endl;
        }
    } else
    {
        std::cout<<" list dir exist : "<<listdirPath<<endl;
        closedir(listdir);
    }
    //generate combined runlist 
    if( dataIsGood=="1" )
    {
        for( int i=0 ; i<3 ; i++ )
        {
            ofstream runlistInit;
            string runlistInitName=verSuf+"/"+runlistSiteNum[i]+"run.list."+verSuf;
            runlistInit.open(runlistInitName.c_str());
            for( vector<string>::iterator it=runlistFileContent[i].begin() ; it!=runlistFileContent[i].end() ; it++ )
            {
                runlistInit<<*it<<endl;
            }
            runlistInit.close();
        }
    }
    dataVerReal.push_back(verSuf);
    dataVerReal.push_back(dataIsGood);

    return dataVerReal; 
}
int anaAll(string dataVer,int siteNum=0)
{


    vector<string> dataVerVec=checkdata(dataVer);
    //string dataVer="P13A";
    /*
       if( !(dataVer=="P13A"||dataVer=="P12E"||dataVer=="P12C"||dataVer=="P12A") )
       {
       std::cout<<"  The dataVerion is wrong ,please check! "<<endl;
       return true;
       }
     */
    std::cout<<" "<<endl;
    std::cout<<" "<<endl;
    std::cout<<" "<<endl;
    std::cout<<" "<<endl;
    dataVer=dataVerVec[dataVerVec.size()-2];
    std::cout<<"dataVer  : "<<dataVer<<endl;
    if( dataVerVec[dataVerVec.size()-1]=="0" )
    {
        std::cout<<"  The dataVerion["<<dataVer<<"] or data is wrong ,please check! "<<endl;
        std::cout<<" "<<endl;
        std::cout<<" "<<endl;
        std::cout<<" "<<endl;
        std::cout<<" "<<endl;
        return 0;
    }

    if( siteNum==0 )
    {
        for( int i=1 ; i<=3 ; i++ )
        {

            std::cout<<"====> begin to analyse EH"<<i<<"'s DaqTime, epsilon_mu,epsilon_multi,accidentals,He8/Li9 "<<endl;
            std::cout<<"dataVersion  : "<<dataVer<<endl;
            //anaHists(i,dataVer);
            std::cout<<" "<<endl;
            std::cout<<" "<<endl;
            std::cout<<" "<<endl;
            std::cout<<" "<<endl;

            std::cout<<"====> begin to analyse EH"<<i<<"'s IbdNum ,fast neutron"<<endl;
            anaTree(i,dataVerVec);
            std::cout<<" "<<endl;
            std::cout<<" "<<endl;
            std::cout<<" "<<endl;
            std::cout<<" "<<endl;
        }

    }else if(siteNum>=1 && siteNum<=3)
    {

        std::cout<<"====> begin to analyse EH"<<siteNum<<"'s DaqTime, epsilon_mu,epsilon_multi,accidentals,He8/Li9 "<<endl;
        std::cout<<"dataVersion  : "<<dataVer<<endl;
        //anaHists(siteNum,dataVer);
        std::cout<<" "<<endl;
        std::cout<<" "<<endl;
        std::cout<<" "<<endl;
        std::cout<<" "<<endl;

        std::cout<<"====> begin to analyse EH"<<siteNum<<"'s IbdNum ,fast neutron"<<endl;
        anaTree(siteNum,dataVerVec);
        std::cout<<" "<<endl;
        std::cout<<" "<<endl;
        std::cout<<" "<<endl;
        std::cout<<" "<<endl;
    }
    std::cout<<"ALL DONE!!! "<<endl;
    return 0;
}

