#include  <iostream>
#include  <TMath.h>
#include  <TH1F.h>
#include  <TFile.h>
#include  <fstream>
#include  <sstream>
#include "RooGlobalFunc.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooConstVar.h"
#include "RooFormulaVar.h"
#include "RooExponential.h"
#include "RooAddPdf.h"
#include "RooFitResult.h"
using namespace RooFit;
int anaHists(int siteNum)
{
	//gStyle->SetOptStat(0);
	//gSystem->Load("libRooFit");
	//gSystem->Load("libCintex");
	//gROOT->ProcessLine("using namespace RooFit;");
	bool anamuon=false;
	bool anasingle=true;
	bool anaLiHe=false;
    bool anaEffMulti=true;
    int ADNumOfSite[3]={0};
    ADNumOfSite[0]=2;
    ADNumOfSite[1]=2;
    ADNumOfSite[2]=4;
	//livetime
	int FirstInt=0;
	string runnum;
	string site;
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
	TH1F* muonNum[6];
	TH1F* muonRate[6];
	TH1F* tmuonNum[6];
	Double_t totalNum[6]={0.};
	
	//singles
	int SingleHistInt=0;
	double n98total0=0.;
	double in98total0=0.;
	double n98total=0.;
	double in98total=0.;
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
	TH1F* showermuonNum1; 
	TH1F* showermuonNum2;
	TH1F* showermuonNum3;
	TH1F* time2lastshowermuon1;
	TH1F* time2lastshowermuon2;
	TH1F* time2lastshowermuon3;
	TH1F* time2lastshowermuon10;
	TH1F* time2lastshowermuon20;
	TH1F* time2lastshowermuon30;
	TH1F* tshowermuonNum1; 
	TH1F* tshowermuonNum2;
	TH1F* tshowermuonNum3;
	TH1F* ttime2lastshowermuon[3];
	TH1F* ttime2lastshowermuon0[3];
	
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
	ifstream runlist;
	string listname=site+"run.list.P13A";
	cout<<"listname : "<<listname<<endl;
	runlist.open(listname.c_str());
	string filename;
	
	while( getline(runlist,runnum) )
	{
		filename=runnum;
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
			return true;
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
				return true;
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
        //th[4]->Draw();
        //break;

        //std::cout<<"niaho "<<endl;
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

		//Li9 He8
		if(anaLiHe)
		{
		showermuonNum1=(TH1F*)f->Get("lidj/showermuonNum1");
		showermuonNum2=(TH1F*)f->Get("lidj/showermuonNum2");
		showermuonNum3=(TH1F*)f->Get("lidj/showermuonNum3");
		time2lastshowermuon1=(TH1F*)f->Get("lidj/time2lastshowermuon1");
		time2lastshowermuon2=(TH1F*)f->Get("lidj/time2lastshowermuon2");
		time2lastshowermuon3=(TH1F*)f->Get("lidj/time2lastshowermuon3");
		time2lastshowermuon10=(TH1F*)f->Get("lidj/time2lastshowermuon10");
		time2lastshowermuon20=(TH1F*)f->Get("lidj/time2lastshowermuon20");
		time2lastshowermuon30=(TH1F*)f->Get("lidj/time2lastshowermuon30");
		if( LiHistInt<1 )
			{
				ttime2lastshowermuon[0]=(TH1F*)f->Get("lidj/time2lastshowermuon1");
				ttime2lastshowermuon[1]=(TH1F*)f->Get("lidj/time2lastshowermuon2");
				ttime2lastshowermuon[2]=(TH1F*)f->Get("lidj/time2lastshowermuon3");
				ttime2lastshowermuon0[0]=(TH1F*)f->Get("lidj/time2lastshowermuon10");
				ttime2lastshowermuon0[1]=(TH1F*)f->Get("lidj/time2lastshowermuon20");
				ttime2lastshowermuon0[2]=(TH1F*)f->Get("lidj/time2lastshowermuon30");
				tshowermuonNum1=(TH1F*)f->Get("lidj/showermuonNum1");
				tshowermuonNum2=(TH1F*)f->Get("lidj/showermuonNum2");
				tshowermuonNum3=(TH1F*)f->Get("lidj/showermuonNum3");
				LiHistInt++;
			}else
			{
				ttime2lastshowermuon[0]->Add(ttime2lastshowermuon[0],time2lastshowermuon1,1,1);
				ttime2lastshowermuon[1]->Add(ttime2lastshowermuon[1],time2lastshowermuon2,1,1);
				ttime2lastshowermuon[2]->Add(ttime2lastshowermuon[2],time2lastshowermuon3,1,1);
				ttime2lastshowermuon0[0]->Add(ttime2lastshowermuon0[0],time2lastshowermuon10,1,1);
				ttime2lastshowermuon0[1]->Add(ttime2lastshowermuon0[1],time2lastshowermuon20,1,1);
				ttime2lastshowermuon0[2]->Add(ttime2lastshowermuon0[2],time2lastshowermuon30,1,1);
				tshowermuonNum1->Add(tshowermuonNum1,showermuonNum1,1,1);
				tshowermuonNum2->Add(tshowermuonNum2,showermuonNum2,1,1);
				tshowermuonNum3->Add(tshowermuonNum3,showermuonNum3,1,1);
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

	}
	
	TAxis *xaxis = th[0]->GetXaxis();
//    std::cout<<"haha "<<endl;
	int binNum = xaxis->GetNbins();
	//livetime
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
				if( th[4]->GetBinContent(j)!=0 )
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
            for( int j=1 ; j<=binNum ; j++ )
            {
                if( th[4]->GetBinContent(j)!=0 )
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
                    effTime[i]+=(1-RateOfAll[i]->GetBinContent(j)*2.e-4)
                        *
                        (1-RateOfAll[i]->GetBinContent(j)*meanCaptureTime[siteNum-1][i])
                        *
                        ((1-RateOfAll[i]->GetBinContent(j)*2.e-4)*tSingleLiveTime200[i]->GetBinContent(j)
                         +
                         RateOfAll[i]->GetBinContent(j)*2.e-4*1.e-4*tNumOfSingleLiveTime200[i]->GetBinContent(j)
                         +
                         tSingleLiveTime[i]->GetBinContent(j)
                         );
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
            
        }
        
    }

	//Li He
		if(anaLiHe)
		{
		double NumMuon[3]={0.};
		double RateMuon[3]={0.};
		double NumIbd[3]={0.};
		double NumIbd0[3]={0.};
		for( int j=0 ; j<binNum ; j++ )
		{
			NumMuon[0]+=tshowermuonNum1->GetBinContent(j);
			NumMuon[1]+=tshowermuonNum2->GetBinContent(j);
			NumMuon[2]+=tshowermuonNum3->GetBinContent(j);
		}
		for( int i=0 ; i<3 ; i++ )
		{
			RateMuon[i]=NumMuon[i]/totalTime[4];
			NumIbd[i]=ttime2lastshowermuon[i]->GetEntries();
			NumIbd0[i]=ttime2lastshowermuon0[i]->GetEntries();
			/*
			std::cout<<" "<<endl;
			std::cout<<"RateMuon[i]  : "<<RateMuon[i]<<endl;
			std::cout<<"NumMuon[i]  : "<<NumMuon[i]<<endl;
			std::cout<<"totalTime[4]  : "<<totalTime[4]<<endl;
			std::cout<<"NumIbd[i]  : "<<NumIbd[i]<<endl;
			std::cout<<"NumIbd0[i]  : "<<NumIbd0[i]<<endl;
			*/
		}
		
		RooRealVar   x("x","x",0.,30., "s");
		RooRealVar tauLi9("tauLi9", "tauLi9", -0.257, "s");
		RooRealVar tauHe8("tauHe8", "tauHe8", -0.172, "s");
		RooRealVar rateMu("rateMu","rate of showermuon",-0.1,"Hz");
		RooRealVar Li9Frac("Li9Frac","Li9's Frac", 0.0);
		RooRealVar N98("N98","total number of Li9 and He8",500.,0.,1000.);
		RooRealVar NEvent("NEvent","number of events",0.);
		RooRealVar one("one","one", -1.);
		//RooRealVar NIbd("NIbd","number of background",0.);
		//RooRealVar NIbd("NIbd","number of background",NumIbd[i]-1000.,0.,NumIbd[i]);
		RooFormulaVar NIbd("NIbd","NIbd","@0 - @1",RooArgList(NEvent, N98));
		RooFormulaVar lambdaLi9("lambdaLi9","lambdaLi9","1/@0 + @1",RooArgList(tauLi9, rateMu));
		RooFormulaVar lambdaHe8("lambdaHe8","lambdaHe8","1/@0 + @1",RooArgList(tauHe8, rateMu));
		RooFormulaVar coeLi9("coeLi9","coeLi9","@0*@1*@2*@3",RooArgList(N98,Li9Frac,lambdaLi9,one));
		RooFormulaVar coeHe8("coeHe8","coeHe8","@0*@1*@2*@3",RooArgList(N98,Li9Frac,lambdaHe8,one));
		RooFormulaVar coeIbd("coeIbd","coeIbd","@0*@1*@2",RooArgList(NIbd,rateMu,one));
		RooExponential expLi9("expLi9", "Li9 distribution", x, lambdaLi9);
		RooExponential expHe8("expHe8", "He8 distribution", x, lambdaHe8);
		RooExponential expIbd("expIbd","Ibd distribution",x,rateMu);
		
		RooFitResult* fitres;
		RooAddPdf* sum;
		RooDataHist* data;
		double n98[41]={0.};
		double in98[41]={0.};
		//double nIbd[41]={0.};
		//double inIbd[41]={0.};
		double minNLL[41]={0.};
		double minNl[3]={0.};
		int minindex[3]={0};
		double n98fit[3]={0.};
		double in98fit[3]={0.};
		int minindex0[3]={0};
		double n98fit0[3]={0.};
		double in98fit0[3]={0.};
		double binwidth=0.;
		for( int j=0 ; j<3 ; j++ )
		//for( int j=0 ; j<1 ; j++ )
		{
	
			rateMu.setVal(-RateMuon[j]);
			//rateMu.setVal(-0.23);
			//NEvent.setVal(NumIbd[j]*binwidth);
			//NIbd.setRange(NumIbd[j]-1000,NumIbd[j]);
			//ttime2lastshowermuon[j]->Rebin(25);
			binwidth=ttime2lastshowermuon[j]->GetBarWidth();
			//NEvent.setVal(NumIbd[j]*binwidth);
			data = new RooDataHist("data", "data", x, ttime2lastshowermuon[j]);
			for( int i=0 ; i<41 ; i++ )
			//for( int i=35 ; i<36 ; i++ )
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
					minindex[j]=i;
				}
			}
			n98fit[j]=n98[minindex[j]]/binwidth;
			in98fit[j]=in98[minindex[j]]/binwidth;
		}
		for( int j=0 ; j<3 ; j++ )
		//for( int j=0 ; j<1 ; j++ )
		{
	
			rateMu.setVal(-RateMuon[j]);
			//rateMu.setVal(-0.23);
			//NEvent.setVal(NumIbd[j]*binwidth);
			//NIbd.setRange(NumIbd[j]-1000,NumIbd[j]);
			//ttime2lastshowermuon[j]->Rebin(25);
			binwidth=ttime2lastshowermuon[j]->GetBarWidth();
			//NEvent.setVal(NumIbd[j]*binwidth);
			data = new RooDataHist("data", "data", x, ttime2lastshowermuon0[j]);
			for( int i=0 ; i<41 ; i++ )
			//for( int i=35 ; i<36 ; i++ )
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
		/*
		for( int j=0 ; j<3 ; j++ )
		{
			std::cout<<"minindex  : "<<minindex[j]<<endl;
			std::cout<<"n98  : "<<n98fit[j] <<" +- "<<in98fit[j] <<endl;
		}
		*/
		n98total=(n98fit[0]+n98fit[1]+n98fit[2])/0.678;
		in98total=sqrt(in98fit[0]*in98fit[0]+in98fit[1]*in98fit[1]+in98fit[2]*in98fit[2])/0.678;
		//std::cout<<"n98total  : "<<n98total <<" +- "<<in98total <<" Rate:"<<n98total/totalTime[0]*24*3600 <<" +- "<<in98total/totalTime[0]*24*3600 <<endl;
		/*
		for( int j=0 ; j<3 ; j++ )
		{
			std::cout<<"minindex0  : "<<minindex0[j]<<endl;
			std::cout<<"n980  : "<<n98fit0[j] <<" +- "<<in98fit0[j] <<endl;
		}
		*/
		n98total0=(n98fit0[0]+n98fit0[1]+n98fit0[2])/0.678;
		in98total0=sqrt(in98fit0[0]*in98fit0[0]+in98fit0[1]*in98fit0[1]+in98fit0[2]*in98fit0[2])/0.678;
		//std::cout<<"n98total  : "<<n98total0 <<" +- "<<in98total0 <<" Rate:"<<n98total0/totalTime0[0]*24*3600 <<" +- "<<in98total0/totalTime0[0]*24*3600 <<endl;
	}	
	

	//print result
    /*
    double i_title[8];//0
    for( int i=0 ; i<7 ; i++ )
    {
        i_title[i]=i+1;
    }
    
	double n_ibdNum[8]={0.};//1
	double n_daqTime[8]={0.};//2
	double n_epsi_mu[8]={0.};//3 
	double n_epsi_multi[8]={0.};//4
	double n_accNum[8]={0.};//5
	double n_accNum_err[8]={0.};//6
	double n_accRate[8]={0.};//7
	double n_accRate_err[8]={0.};//8
	double n_accNum_600[8]={0.};//9
	double n_accNum_600_err[8]={0.};//10
	double n_accRate_600[8]={0.};//11
	double n_accRate_600_err[8]={0.};//12
	double n_fnNum[8]={0.};//13
	double n_fnNum_err[8]={0.};//14
	double n_fnBs[8]={0.};//15
	double n_fnBs_err[8]={0.};//16
	double n_fnRate[8]={0.};//17
	double n_fnRate_err[8]={0.};//18
    double i_newline[8]={0.};
	double w_ibdNum[8]={0.};//20
	double w_ibdNum_ratio[8]={0.};//21
	double w_daqTime[8]={0.};//22
	double w_daqTime_ratio[8]={0.};//23
	double w_epsi_mu[8]={0.};//24
	double w_epsi_mu_ratio[8]={0.};//25
	double w_epsi_multi[8]={0.};//26
	double w_epsi_multi_ratio[8]={0.};//27
	double w_accNum[8]={0.};//28
	double w_accNum_err[8]={0.};//29
	double w_accNum_ratio[8]={0.};//30
	double w_accRate[8]={0.};//31
	double w_accRate_err[8]={0.};//32
	double w_accRate_ratio[8]={0.};//33
	double w_accNum_600[8]={0.};//34
	double w_accNum_600_err[8]={0.};//35
	double w_accNum_600_ratio[8]={0.};//36
	double w_accRate_600[8]={0.};//37
	double w_accRate_600_err[8]={0.};//38
	double w_accRate_600_ratio[8]={0.};//39
	double w_fnNum[8]={0.};//40
	double w_fnNum_err[8]={0.};//41
	double w_fnNum_ratio[8]={0.};//42
	double w_fnBs[8]={0.};//43
	double w_fnBs_err[8]={0.};//44
	double w_fnBs_ratio[8]={0.};//45
	double w_fnRate[8]={0.};//46
	double w_fnRate_err[8]={0.};//47
	double w_fnRate_ratio[8]={0.};//48
    */

    double t_result[49][8];
    //TString t_result[49][8];
    for( int i=0 ; i<49 ; i++ )
    {
        for( int j=0 ; j<8 ; j++ )
        {
            t_result[i][j]=0.;
        }
        
    }
    
    std::cout<<"begin to read file "<<endl;
	ifstream infile; 
	infile.open("result_temp_P13A.txt",ios::in);
	int inLineNum=0;
    std::cout<<"get value "<<endl;
	//while(!infile.eof())
	while(inLineNum<49)
	{
        for( int j=0 ; j<8 ; j++ )
        {
            infile>>t_result[inLineNum][j];
        }
		inLineNum++;
	}
    std::cout<<"finished  "<<endl;

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
    for( int i=0 ; i<ADNumOfSite[siteNum-1] ; i++ )
    {
            double effOfMulti=effTime[i]/totalTime[i];
            double effOfMulti0=effTime0[i]/totalTime0[i];
		    t_result[0][i+ii]=i+ii+1;
		    t_result[2][i+ii]=totalTime[4]/(3600*24);
		    t_result[3][i+ii]=totalTime0[i]/totalTime[4];
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
            t_result[22][i+ii]=totalTime[4]/(3600*24);
            t_result[23][i+ii]=0;
            t_result[24][i+ii]=totalTime[i]/totalTime[4];
            t_result[25][i+ii]=(totalTime[i]/totalTime[4]-totalTime0[i]/totalTime[4])/(totalTime0[i]/totalTime[4]);
            t_result[26][i+ii]=effTime[i]/totalTime[i];
            t_result[27][i+ii]=(effOfMulti-effOfMulti0)/effOfMulti;
            t_result[28][i+ii]=NumAccBkg[i];
            if( totalN[i]==0 )
            {
                t_result[29][i+ii]=0;
                t_result[32][i+ii]=0;
                t_result[35][i+ii]=0;
                t_result[38][i+ii]=0;
            } else
            {
                t_result[29][i+ii]=NumAccBkg[i]*sqrt(1/(double)totalN[i]+(0.01*0.01));
                t_result[32][i+ii]=((NumAccBkg[i]*sqrt(1/(double)totalN[i]+(0.01*0.01)))/totalTime[i])*24*3600;
                t_result[35][i+ii]=NumAccBkg[i]*sqrt(1/(double)totalN[i]+(0.01*0.01));
                t_result[38][i+ii]=(NumAccBkg[i]*sqrt(1/(double)totalN[i]+(0.01*0.01))/totalTime[i])*24*3600;
            }
            if( NumAccBkg0[i]==0 )
            {
                t_result[30][i+ii]=-0;
                t_result[33][i+ii]=-0;
                t_result[36][i+ii]=-0;
                t_result[39][i+ii]=-0;
            } else
            {
                t_result[30][i+ii]=-(NumAccBkg0[i]-NumAccBkg[i])/NumAccBkg0[i];
                t_result[33][i+ii]=-(NumAccBkg0[i]/totalTime0[i]-NumAccBkg[i]/totalTime[i])/(NumAccBkg0[i]/totalTime0[i]);
                t_result[36][i+ii]=-(NumAccBkg0_600[i]-NumAccBkg_600[i])/NumAccBkg0_600[i];
                t_result[39][i+ii]=-(NumAccBkg0_600[i]/totalTime0[i]-NumAccBkg_600[i]/totalTime[i])/(NumAccBkg0_600[i]/totalTime0[i]);
            }
            t_result[31][i+ii]=(NumAccBkg[i]/totalTime[i])*24*3600;
            t_result[34][i+ii]=NumAccBkg_600[i];
            t_result[37][i+ii]=(NumAccBkg_600[i]/totalTime[i])*24*3600;
    }
	
	ofstream outfile;
	outfile.open("result_temp_P13A.txt");
    int outLineNum=0;
    while( outLineNum<49 )
    {
	    for( int i=0 ; i<8 ; i++ )
	    {
		    outfile<< setw(13) <<t_result[outLineNum][i];
		    //outfile<< setw(13) <<outStr[outLineNum][i];
            if( i==7 )
            {
                outfile<<endl;
            }
	    }
        outLineNum++;
    }
	outfile.close();


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
	for( int i=0 ; i<4 ; i++ )
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
	if(anaLiHe)
	{
	    std::cout<<" "<<endl;
	    std::cout<<"Li9/He8 "<<endl;
	    std::cout<<"n98total  : "<<n98total <<" +- "<<in98total <<" Rate:"<<n98total/totalTime[0]*24*3600 <<" +- "<<in98total/totalTime[0]*24*3600 <<endl;
	    std::cout<<"n98total0(without rpc)  : "<<n98total0 <<" +- "<<in98total0 <<" Rate:"<<n98total0/totalTime0[0]*24*3600 <<" +- "<<in98total0/totalTime0[0]*24*3600 <<endl;
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



	string rootname=site;
	rootname+="livetime_P13A.root";
	TFile* file = new TFile(rootname.c_str(),"RECREATE");
	th[0]->Write();
	th[1]->Write();
	th[2]->Write();
	th[3]->Write();
	th[4]->Write();
	th0[0]->Write();
	th0[1]->Write();
	th0[2]->Write();
	th0[3]->Write();
	th_200[0]->Write();
	th_200[1]->Write();
	th_200[2]->Write();
	th_200[3]->Write();
	th0_200[0]->Write();
	th0_200[1]->Write();
	th0_200[2]->Write();
	th0_200[3]->Write();
    for( int i=0 ; i<ADNumOfSite[siteNum-1] ; i++ )
    {
        tSingleLiveTime[i]->Write();
        tSingleLiveTime200[i]->Write();
        tSingleLiveTime0[i]->Write();
        tSingleLiveTime2000[i]->Write();
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
		for( int i=0 ; i<ADNumOfSite[siteNum-1] ; i++ )
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
        
    }
	file->Close();
	return 0;
}
