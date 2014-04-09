#define Ibd_cxx

#include "Ibd.h"
#include <TH2.h>
#include <TF1.h>
#include <TStyle.h>
#include  <iostream>
#include  <fstream>
#include  <sstream>
#include "iomanip.h"
#include  <TFitResult.h>

void Ibd::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   //TString DataVerTmp=option(3,4);
   //string DataVerTmp=option.substr(3);
   dataVer=option;
    dataVer=dataVer.substr(3);
   option=option(0,3);
   std::cout<<"dataVer  : "<<dataVer<<endl;
   int daqHistNum=0;
   if( dataVer.find("13")!=string::npos || dataVer.find("14")!=string::npos || dataVer.find("15")!=string::npos || dataVer.find("16")!=string::npos)
   {
       daqHistNum=5;
   } else
   {
       if( dataVer.find("12")!=string::npos || dataVer.find("11")!=string::npos)
       {
            daqHistNum=4;
       }
   }
   std::cout<<"option  : "<<option<<endl;
   //livetime={{85.7705,85.4113,0,0},{98.2744,0,0,0},{112.597,112.596,112.56,0}};
   //livetime0={{85.8167,85.4573,0,0},{98.3095,0,0,0},{112.61,112.609,112.572,0}};
   //double livetime={{85.7705,85.4113},{98.2744},{112.597,112.596,112.56}};
   //double livetime0={{85.8167,85.4573},{98.3095},{112.61,112.609,112.572}};
   
   for( int i=0 ; i<4 ; i++ )
   {
    UpperNum[i]=0.;
    LowerNum[i]=0.;
   }
   
   std::cout<<"start to read file "<<endl;
   for( int i=0 ; i<53 ; i++ )
   {
        for( int j=0 ; j<8 ; j++ )
        {
            t_result[i][j]=0.;    
        }
   }
	ifstream infile; 
    string infileName=dataVer+"/"+"result_temp_"+dataVer+".txt";
	infile.open(infileName.c_str(),ios::in);
	int inLineNum=0;
    std::cout<<"get value "<<endl;
	while(inLineNum<53)
	{
        for( int j=0 ; j<8 ; j++ )
        {
            infile>>t_result[inLineNum][j];
        }
		inLineNum++;
        //std::cout<<"inLineNuminLineNum  : "<<inLineNum<<endl;
	}
	infile.close();
    std::cout<<"done "<<endl;

   daqtime[0][0]=t_result[26][0];
   daqtime[0][1]=t_result[26][1];
   daqtime[1][0]=t_result[26][2];
   daqtime[1][1]=t_result[26][3];
   daqtime[2][0]=t_result[26][4];
   daqtime[2][1]=t_result[26][5];
   daqtime[2][2]=t_result[26][6];
   daqtime[2][3]=t_result[26][7];

   daqtime0[0][0]=t_result[2][0];
   daqtime0[0][1]=t_result[2][1];
   daqtime0[1][0]=t_result[2][2];
   daqtime0[1][1]=t_result[2][3];
   daqtime0[2][0]=t_result[2][4];
   daqtime0[2][1]=t_result[2][5];
   daqtime0[2][2]=t_result[2][6];
   daqtime0[2][3]=t_result[2][7];

   epsi_mu[0][0]=t_result[28][0];
   epsi_mu[0][1]=t_result[28][1];
   epsi_mu[1][0]=t_result[28][2];
   epsi_mu[1][1]=t_result[28][3];
   epsi_mu[2][0]=t_result[28][4];
   epsi_mu[2][1]=t_result[28][5];
   epsi_mu[2][2]=t_result[28][6];
   epsi_mu[2][3]=t_result[28][7];

   epsi_mu0[0][0]=t_result[3][0];
   epsi_mu0[0][1]=t_result[3][1];
   epsi_mu0[1][0]=t_result[3][2];
   epsi_mu0[1][1]=t_result[3][3];
   epsi_mu0[2][0]=t_result[3][4];
   epsi_mu0[2][1]=t_result[3][5];
   epsi_mu0[2][2]=t_result[3][6];
   epsi_mu0[2][3]=t_result[3][7];

   epsi_multi[0][0]=t_result[30][0];
   epsi_multi[0][1]=t_result[30][1];
   epsi_multi[1][0]=t_result[30][2];
   epsi_multi[1][1]=t_result[30][3];
   epsi_multi[2][0]=t_result[30][4];
   epsi_multi[2][1]=t_result[30][5];
   epsi_multi[2][2]=t_result[30][6];
   epsi_multi[2][3]=t_result[30][7];

   epsi_multi0[0][0]=t_result[4][0];
   epsi_multi0[0][1]=t_result[4][1];
   epsi_multi0[1][0]=t_result[4][2];
   epsi_multi0[1][1]=t_result[4][3];
   epsi_multi0[2][0]=t_result[4][4];
   epsi_multi0[2][1]=t_result[4][5];
   epsi_multi0[2][2]=t_result[4][6];
   epsi_multi0[2][3]=t_result[4][7];

   livetime[0][0]=daqtime[0][0]*epsi_mu[0][0]*epsi_multi[0][0];
   livetime[0][1]=daqtime[0][1]*epsi_mu[0][1]*epsi_multi[0][1];
   livetime[1][0]=daqtime[1][0]*epsi_mu[1][0]*epsi_multi[1][0];
   livetime[1][1]=daqtime[1][1]*epsi_mu[1][1]*epsi_multi[1][1];
   livetime[2][0]=daqtime[2][0]*epsi_mu[2][0]*epsi_multi[2][0];
   livetime[2][1]=daqtime[2][1]*epsi_mu[2][1]*epsi_multi[2][1];
   livetime[2][2]=daqtime[2][2]*epsi_mu[2][2]*epsi_multi[2][2];
   livetime[2][3]=daqtime[2][3]*epsi_mu[2][3]*epsi_multi[2][3];

   livetime0[0][0]=daqtime0[0][0]*epsi_mu0[0][0]*epsi_multi0[0][0];
   livetime0[0][1]=daqtime0[0][1]*epsi_mu0[0][1]*epsi_multi0[0][1];
   livetime0[1][0]=daqtime0[1][0]*epsi_mu0[1][0]*epsi_multi0[1][0];
   livetime0[1][1]=daqtime0[1][1]*epsi_mu0[1][1]*epsi_multi0[1][1];
   livetime0[2][0]=daqtime0[2][0]*epsi_mu0[2][0]*epsi_multi0[2][0];
   livetime0[2][1]=daqtime0[2][1]*epsi_mu0[2][1]*epsi_multi0[2][1];
   livetime0[2][2]=daqtime0[2][2]*epsi_mu0[2][2]*epsi_multi0[2][2];
   livetime0[2][3]=daqtime0[2][3]*epsi_mu0[2][3]*epsi_multi0[2][3];

   ADNum=daqHistNum-1;
   site=2;
	//if( option.EqualTo("EH1") || option.EqualTo("EH2"))
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
   for( int i=0 ; i<ADNum ; i++ )
   {
	   Num1[i]=0;
	   Numo[i]=0;
	   Num2[i]=0;
   }
   tNumo=0;
   tNum1=0;
   tNum2=0;
   option+="Ibd_";
   option+=dataVer;
   option+=".root";
   TString fileName=dataVer+"/"+option;
   file = new TFile(fileName,"RECREATE");

   // Binning	
   double rnorm = 1.;
   // calculate muon E bins
	const int nBins = 200;
	//const int nBins = 200;
	double xlow = 0.001, xup = 200.;
	BinWt = new  double[nBins+1];//so i can declare it in .h file 
	//double BinWt[nBins+1] = {0.0}; 
	double EBins[nBins+1] = {0.0};
	//EBins[0] = xlow;  //should be this ,but  zhangfh's EBins[0]=0
	double BinWidth = (log10(xup)-log10(xlow))/nBins;

	for(int i=1; i<=nBins; i++) {
		EBins[i] = xlow*pow(10, BinWidth*i);
		BinWt[i] = rnorm/(EBins[i]-EBins[i-1]);
	}
	tFnProEWithrpc=new TH1F("tFnProEWithrpc","Energy of promt signal with using rpc veto",nBins, EBins);
	tFnProEWithoutrpc=new TH1F("tFnProEWithoutrpc","Energy of promt signal without using rpc veto",nBins, EBins);
	tFnProEWithoutrpcUniX=new TH1F("tFnProEWithoutrpcUniX","Energy of promt signal without using rpc veto",198, 0.7,99.7);
   for(int i=0;i<ADNum;i++)
	{
		histname="AD";
		histname+=i+1;
		histname+="t2lastmuonWithrpc";
	   t2lastmuonWithrpc[i] = new TH1F(histname,"prompt time to last Muon with using rpc veto",5000,0.,50000.);
		histname="AD";
		histname+=i+1;
		histname+="t2lastmuonWithoutrpc";
	   t2lastmuonWithoutrpc[i] = new TH1F(histname,"prompt time to last Muon with using rpc veto",5000,0.,50000.);
		histname="AD";
		histname+=i+1;
		histname+="FnProEWithrpc";
		FnProEWithrpc[i]=new TH1F(histname,"Energy of promt signal with using rpc veto",nBins, EBins);
		histname="AD";
		histname+=i+1;
		histname+="FnProEWithoutrpc";
		FnProEWithoutrpc[i]=new TH1F(histname,"Energy of promt signal without using rpc veto",nBins, EBins);
		histname="AD";
		histname+=i+1;
		histname+="pxy";
		pxy[i] = new TH2F(histname,"pxy",30,-3.,3.,30,-3.,3.);
		histname+="0";
		pxy0[i] = new TH2F(histname,"pxy0",30,-3.,3.,30,-3.,3.);
		histname="AD";
		histname+=i+1;
		histname+="prz";
		prz[i] = new TH2F(histname,"prz",30,0,3.,30,-3.,3.);
		histname+="0";
		prz0[i] = new TH2F(histname,"prz0",30,0,3.,30,-3.,3.);
		histname="AD";
		histname+=i+1;
		histname+="pE";
		pE[i]= new TH1F(histname,"Energy of promt signal",30,0.7 ,12.0);
		histname+="0";
		pE0[i]= new TH1F(histname,"Energy of promt signal 0",30,0.7 ,12.0);
		histname="AD";
		histname+=i+1;
		histname+="dxy";
		dxy[i] = new TH2F(histname,"dxy",30,-3.,3.,30,-3.,3.);
		histname+="0";
		dxy0[i] = new TH2F(histname,"dxy0",30,-3.,3.,30,-3.,3.);
		histname="AD";
		histname+=i+1;
		histname+="drz";
		drz[i] = new TH2F(histname,"drz",30,0,3.,30,-3.,3.);
		histname+="0";
		drz0[i] = new TH2F(histname,"drz0",30,0,3.,30,-3.,3.);
		histname="AD";
		histname+=i+1;
		histname+="dE";
		dE[i]= new TH1F(histname,"Energy of delayed signal",60,6.0,12.0);
		histname+="0";
		dE0[i]= new TH1F(histname,"Energy of delayed signal 0",60,6.0,12.0);
		histname="AD";
		histname+=i+1;
		histname+="intervalT";
		intervalT[i]= new TH1F(histname,"interval time between prompt and delayed signal",400,0.,200.);
		histname+="0";
		intervalT0[i]= new TH1F(histname,"interval time between prompt and delayed signal 0",400,0.,200.);
	}
	std::cout<<"this is begin ,done. "<<endl;
}


void Ibd::SlaveBegin(TTree * /*tree*/)
{

   TString option = GetOption();

}
void Ibd::FillIbd(int itype)
{
    if( itype==1)
    {
		pxy[det-1]->Fill(x[0]/1000,y[0]/1000);
		prz[det-1]->Fill(sqrt(x[0]*x[0]+y[0]*y[0])/1000,z[0]/1000);
		pE[det-1]->Fill(energy[0]);
		dxy[det-1]->Fill(x[1]/1000,y[1]/1000);
		drz[det-1]->Fill(sqrt(x[1]*x[1]+y[1]*y[1])/1000,z[1]/1000);
		dE[det-1]->Fill(energy[1]);
		intervalT[det-1]->Fill(timeInterval);
        
    }else
    {
		pxy0[det-1]->Fill(x[0]/1000,y[0]/1000);
		prz0[det-1]->Fill(sqrt(x[0]*x[0]+y[0]*y[0])/1000,z[0]/1000);
		pE0[det-1]->Fill(energy[0]);
		dxy0[det-1]->Fill(x[1]/1000,y[1]/1000);
		drz0[det-1]->Fill(sqrt(x[1]*x[1]+y[1]*y[1])/1000,z[1]/1000);
		dE0[det-1]->Fill(energy[1]);
		intervalT0[det-1]->Fill(timeInterval);
    
    }

}

Bool_t Ibd::Process(Long64_t entry)
{
	GetEntry(entry);

    //std::cout<<"Numo["<<det-1<<"]  : "<<Numo[det-1]<<endl;
    //TODO::4 is not enough,spesically for newAdMuon and newShowerMuon ,maybe need to modify this .
	double t2lastmuonWoutrpc=promptT2Muon[0];
	for( int i=0 ; i<4 ; i++ )
	{
		if( t2lastmuonWoutrpc>promptT2Muon[i] )
		{
			t2lastmuonWoutrpc=promptT2Muon[i];
		}
	}
	double t2lastmuonWrpc=promptT2Muon[4];
	if( t2lastmuonWrpc>t2lastmuonWoutrpc)
	{
		t2lastmuonWrpc=t2lastmuonWoutrpc;
	}
	int bin = FnProEWithrpc[det-1]->FindBin(energy[0]);
	double wt=BinWt[bin];

	if( isIbd==0 )
	{
		Numo[det-1]++;
		//Numo[det-1]++;
        //if( Numo[det-1]-Numo0>100 )
        //{
        //    std::cout<<"Numo0  : "<<Numo0<<endl;
        //    std::cout<<"Numo[det-1]  : "<<Numo[det-1]<<endl;
        //}
		tNumo++;
        FillIbd(0);
		t2lastmuonWithoutrpc[det-1]->Fill(t2lastmuonWoutrpc*1e6);
		FnProEWithoutrpc[det-1]->Fill(energy[0],wt);
		tFnProEWithoutrpc->Fill(energy[0],wt);
		tFnProEWithoutrpcUniX->Fill(energy[0]);
        if( z[0]>0. )
        {
            UpperNum[det-1]++;
        }else
        {
            LowerNum[det-1]++;
        }
		
	} else if( isIbd==1 )
	{
		Num1[det-1]++;
		tNum1++;
        FillIbd(0);
        FillIbd(1);
		t2lastmuonWithoutrpc[det-1]->Fill(t2lastmuonWoutrpc*1e6);
		t2lastmuonWithrpc[det-1]->Fill(t2lastmuonWrpc*1e6);
		FnProEWithoutrpc[det-1]->Fill(energy[0],wt);
		FnProEWithrpc[det-1]->Fill(energy[0],wt);
		tFnProEWithoutrpc->Fill(energy[0],wt);
		tFnProEWithoutrpcUniX->Fill(energy[0]);
		tFnProEWithrpc->Fill(energy[0],wt);
        if( z[0]>0. )
        {
            UpperNum[det-1]++;
        }else
        {
            LowerNum[det-1]++;
        }
	} else if( isIbd==2 )
	{
		Num2[det-1]++;
		tNum2++;
        FillIbd(1);
		t2lastmuonWithrpc[det-1]->Fill(t2lastmuonWrpc*1e6);
		FnProEWithrpc[det-1]->Fill(energy[0],wt);
		tFnProEWithrpc->Fill(energy[0],wt);
	} else if( isIbd==3 )
	{
		FnProEWithoutrpc[det-1]->Fill(energy[0],wt);
		tFnProEWithoutrpc->Fill(energy[0],wt);
		tFnProEWithoutrpcUniX->Fill(energy[0]);
	} else if( isIbd==4 )
	{
		FnProEWithoutrpc[det-1]->Fill(energy[0],wt);
		FnProEWithrpc[det-1]->Fill(energy[0],wt);
		tFnProEWithoutrpc->Fill(energy[0],wt);
		tFnProEWithoutrpcUniX->Fill(energy[0]);
		tFnProEWithrpc->Fill(energy[0],wt);
	} else if (isIbd==5)
	{
		FnProEWithrpc[det-1]->Fill(energy[0],wt);
		tFnProEWithrpc->Fill(energy[0],wt);
	}else
	{
		//std::cout<<"isIbd is wrong ,please check data. "<<endl;	
	}
	//std::cout<<"this is process ,done. "<<endl;

   return kTRUE;
}

void Ibd::SlaveTerminate()
{

}

void Ibd::Terminate0()
{

}
void Ibd::Terminate()
{
	for( int i=0 ; i<ADNum ; i++ )
	{
        std::cout<<"start to write "<<endl;
		t2lastmuonWithrpc[i]->Write();
		t2lastmuonWithoutrpc[i]->Write();
		FnProEWithrpc[i]->Write();
		FnProEWithoutrpc[i]->Write();
		pxy[i]->Write();
		prz[i]->Write();
		pE[i]->Write();
		dxy[i]->Write();
		drz[i]->Write();
		dE[i]->Write();
		intervalT[i]->Write();
		pxy0[i]->Write();
		prz0[i]->Write();
		pE0[i]->Write();
		dxy0[i]->Write();
		drz0[i]->Write();
		dE0[i]->Write();
		intervalT0[i]->Write();
        std::cout<<"done "<<endl;
		std::cout<<""<<endl;
		std::cout<<"AD"<<i+1<<" Num of IBD                          : ["<<Num2[i]+Num1[i]<<" ("<<((double)(Num2[i]+Num1[i]) -(double)(Numo[i]+Num1[i]) )/(double)(Numo[i]+Num1[i])<<")]  "<<(Num2[i]+Num1[i])/livetime[site][i] <<" /day"<<endl;
		std::cout<<"    Num2: "<<Num2[i]<<" Num1: "<<Num1[i]<<endl;
		std::cout<<"AD"<<i+1<<" Num of IBD (without using rpc veto) : ["<<Numo[i]+Num1[i]<<"]  "<<(Numo[i]+Num1[i])/livetime0[site][i] <<" /day"<<endl;
		std::cout<<"    Numo: "<<Numo[i]<<" Num1: "<<Num1[i]<<endl;
		std::cout<<" "<<endl;
	}

	//fn
	
	for( int i=1 ; i<=200 ; i++ )
	{
		double nEvt = tFnProEWithrpc->GetBinContent(i);
		tFnProEWithrpc->SetBinError(i, sqrt(nEvt*BinWt[i]));
		double nEvt0 = tFnProEWithoutrpc->GetBinContent(i);
		double nEvt0UniX = tFnProEWithoutrpcUniX->GetBinContent(i);
		tFnProEWithoutrpc->SetBinError(i, sqrt(nEvt0*BinWt[i]));
		tFnProEWithoutrpcUniX->SetBinError(i, sqrt(nEvt0UniX));
	}
	
    TFile* f=new TFile("fnSpec.root","read");
    if( f->IsZombie() )
    {
        std::cout<<"Can't find fnSpec.root ... "<<endl;
    }
    TH1F* h=(TH1F*)f->Get("h1");
    if( !h )
    {
        std::cout<<"Can't find h1 in fnSpec.root ... "<<endl;
    }
    TCanvas* c=new TCanvas(Form("%sfnSpec",dataVer.c_str()),"c",800,600);
    //tFnProEWithoutrpcUniX->SetLineColor();
    tFnProEWithoutrpcUniX->Draw("e");
    h->SetLineColor(kRed);
    h->Draw("same");
    c->SaveAs(Form("%s/%sfnSpec.eps",dataVer.c_str(),dataVer.c_str()));
    file->cd();
    c->Write();
    delete h;
    delete c;
    delete f;

	//usr rpc veto
	//pol1
	TF1* f1= new TF1("f1","pol1",12.,60.);
	tFnProEWithrpc->Fit(f1,"R");
	double par1[2],ipar1[2];
	f1->GetParameters(&par1[0]);
	ipar1[0]=f1->GetParError(0);
	ipar1[1]=f1->GetParError(1);
	int binlow=tFnProEWithrpc->FindBin(0.7);
	int binup=tFnProEWithrpc->FindBin(12.);
	std::cout<<"binlow  : "<<binlow<<endl;
	std::cout<<"binup  : "<<binup<<endl;
    double bincenter=0.;
	double NFn1=0.;
	double iNFnsquare1=0.;
    for( int j=binlow ; j<=binup ; j++ )
    {
        bincenter=tFnProEWithrpc->GetBinCenter(j);
        NFn1+=(par1[0]+par1[1]*bincenter)/BinWt[j];
        iNFnsquare1+=(bincenter*bincenter*ipar1[1]*ipar1[1]+ipar1[0]*ipar1[0])/(BinWt[j]*BinWt[j]);
    }
    //NFn1=(par1[0]*2+12.7*par1[1])*(12-0.7)/2;
    //iNFnsquare1=(ipar1[0]*2+12.7*ipar1[1])*(12-0.7)/2; //this is not appropriate ,from zhangfh.

	//NFn1=f1->Integral(0.7,12.0);
	//pol0
	TF1* f0= new TF1("f0","pol0",12.,60.);
	tFnProEWithrpc->Fit(f0,"R+");
	double par0,ipar0;
	f0->GetParameters(&par0);
	ipar0=f0->GetParError(0);
	double NFn0=0.;
	double iNFnsquare0=0.;
	
    for( int j=binlow ; j<=binup ; j++ )
    {
        bincenter=tFnProEWithrpc->GetBinCenter(j);
        NFn0+=par0/BinWt[j];
        iNFnsquare0+=ipar0*ipar0/(BinWt[j]*BinWt[j]);
    }
    //NFn0=tFnProEWithrpc->Integral(tFnProEWithrpc->FindBin(12),tFnProEWithrpc->FindBin(100),"width")*(12-0.7)/(100-12);
    //iNFnsquare0=sqrt(tFnProEWithrpc->Integral(tFnProEWithrpc->FindBin(12),tFnProEWithrpc->FindBin(100),"width"))*(12-0.7)/(100-12);
    ////NFn00=f00->Integral(0.7,12.0);

	//NFn0=f0->Integral(0.7,12.0);
	//without using rpc veto
	//pol1
	TF1* f10= new TF1("f10","pol1",12.,60.);
	TFitResultPtr r=tFnProEWithoutrpcUniX->Fit(f10,"RS");
    double chi2=r->Chi2();
    double ndf=r->Ndf();
	double par10[2],ipar10[2];
	f10->GetParameters(&par10[0]);
	ipar10[0]=f10->GetParError(0);
	ipar10[1]=f10->GetParError(1);
	double NFn10=0.;
	double iNFnsquare10=0.;
    for( int j=binlow ; j<=binup ; j++ )
    {
        bincenter=tFnProEWithoutrpcUniX->GetBinCenter(j);
        NFn10+=(par10[0]+par10[1]*bincenter);
        iNFnsquare10+=(bincenter*bincenter*ipar10[1]*ipar10[1]+ipar10[0]*ipar10[0]);
    }
    //NFn10=(par10[0]*2+12.7*par10[1])*(12-0.7)/2;
    //iNFnsquare10=(ipar10[0]*2+12.7*ipar10[1])*(12-0.7)/2;
    //
	//pol0
	TF1* f00= new TF1("f00","pol0",12.,60.);
	TFitResultPtr r0=tFnProEWithoutrpcUniX->Fit(f00,"R+S");
    double chi20=r0->Chi2();
    double ndf0=r0->Ndf();
	double par00,ipar00;
	f00->GetParameters(&par00);
	ipar00=f00->GetParError(0);
	double NFn00=0.;
	double iNFnsquare00=0.;
    for( int j=binlow ; j<=binup ; j++ )
    {
        bincenter=tFnProEWithoutrpcUniX->GetBinCenter(j);
        NFn00+=par00;
        iNFnsquare00+=ipar00*ipar00;
    }
    //NFn00=tFnProEWithoutrpc->Integral(tFnProEWithoutrpc->FindBin(12),tFnProEWithoutrpc->FindBin(100),"width")*(12-0.7)/(100-12);
    //iNFnsquare00=sqrt(tFnProEWithoutrpc->Integral(tFnProEWithoutrpc->FindBin(12),tFnProEWithoutrpc->FindBin(100),"width"))*(12-0.7)/(100-12);
	//NFn00=f00->Integral(0.7,12.0);
	std::cout<<"tNumo tNum1 tNum2  : "<<tNumo<<" " <<tNum1<<" "<<tNum2 <<endl;
	std::cout<<"number of Fn  : "<<endl;
	std::cout<<"NFn0  : "<<NFn0<<endl;
	std::cout<<"NFn1  : "<<NFn1<<endl;
    double totallivetime0=0.;
    for( int i=0 ; i<ADNum ; i++ )
    {
        totallivetime0+=livetime0[site][i];
    }
    double totallivetime=0.;
    for( int i=0 ; i<ADNum ; i++ )
    {
        totallivetime+=livetime[site][i];
    }
	std::cout<<"	total  : "<<(NFn0+NFn1)/2<<" +- "<<(sqrt((iNFnsquare0+iNFnsquare1)/4+(NFn1-NFn0)*(NFn1-NFn0))) \
		 <<"("<< ((NFn0+NFn1)/2 -(NFn00+NFn10)/2 )/((NFn00+NFn10)/2)<<")   B/S : "\
		 <<((NFn0+NFn1)/(tNum2+tNum1))/2 \
		 <<"("<<(((NFn0+NFn1)/(tNum2+tNum1))/2 -(NFn00+NFn10)/2/(tNumo+tNum1) )/((NFn00+NFn10)/2/(tNumo+tNum1)) <<") rate : "\
         <<(NFn0+NFn1)/2/totallivetime<<" +- "<< (sqrt((iNFnsquare0+iNFnsquare1)/4+(NFn1-NFn0)*(NFn1-NFn0)))/totallivetime<<" /day ("\
		 <<( (NFn0+NFn1)/2/totallivetime-(NFn00+NFn10)/2/totallivetime0 )/((NFn00+NFn10)/2/totallivetime0) <<")" <<endl;

	std::cout<<"number of Fn (without using rpc veto) : "<<endl;
    std::cout<<"first order a+b*x  : a = "<<par10[0]<<"+-"<<ipar10[0]<<" b = "<<par10[1]<<"+-"<<ipar10[1]<<" chi2/ndf="<<chi2<<"/"<<ndf<<endl;
    std::cout<<"zero  order a      : a = "<<par00<<"+-"<<ipar00<<" chi2/ndf="<<chi20<<"/"<<ndf0<<endl;
    //std::cout<<"zero  order a zfj  : a = "<<tFnProEWithoutrpc->Integral(tFnProEWithoutrpc->FindBin(12),tFnProEWithoutrpc->FindBin(100),"width")/(100-12)<<"+-"<<sqrt(tFnProEWithoutrpc->Integral(tFnProEWithoutrpc->FindBin(12),tFnProEWithoutrpc->FindBin(100),"width"))/(100-12) <<endl;

	std::cout<<"NFn00  : "<<NFn00<<endl;
	std::cout<<"NFn10  : "<<NFn10<<endl;
	std::cout<<"	total0  : "<<(NFn00+NFn10)/2<<" +- "<<(sqrt((iNFnsquare00+iNFnsquare10)/4+(NFn10-NFn00)*(NFn10-NFn00))) \
		     <<"   B/S : "<<(NFn00+NFn10)/2/(tNumo+tNum1) \
		 <<" rate : "<<(NFn00+NFn10)/2/totallivetime0<<" +- "<< (sqrt((iNFnsquare00+iNFnsquare10)/4+(NFn10-NFn00)*(NFn10-NFn00)))/totallivetime0<<" /day"<<endl;
	tFnProEWithrpc->Write();
	tFnProEWithoutrpc->Write();

	file->Close();	

    //write to result_temp.txt
    int ii=0;
    if( site==0 )
    {
        ii=0;
    } else if( site==1 )
    {
        ii=2;
    }else if( site==2 )
    {
        ii=4;
    }else
    {
        std::cout<<"error  : wrong siteNum"<<endl;    
    }
    for( int i=0 ; i<ADNum ; i++ )
    {
		    t_result[1][i+ii]=Numo[i]+Num1[i];
		    t_result[13][i+ii]=(NFn00+NFn10)/2;
		    t_result[14][i+ii]=(sqrt((iNFnsquare00+iNFnsquare10)/4+(NFn10-NFn00)*(NFn10-NFn00)));
		    t_result[15][i+ii]=(NFn00+NFn10)/2/(tNumo+tNum1);
		    t_result[16][i+ii]=UpperNum[i]/LowerNum[i];
		    t_result[17][i+ii]=(NFn00+NFn10)/2/totallivetime0;
		    t_result[18][i+ii]=(sqrt((iNFnsquare00+iNFnsquare10)/4+(NFn10-NFn00)*(NFn10-NFn00)))/totallivetime0;
            /*
		    t_result[24][i+ii]=Num2[i]+Num1[i];
            if( Num2[i]+Num1[i]==0 )
            {
                t_result[25][i+ii]=0;
            } else
            {
                t_result[25][i+ii]=((double)(Num2[i]+Num1[i])-(double)(Numo[i]+Num1[i]) )/(double)(Numo[i]+Num1[i]);
            }
            */
            t_result[24][i+ii]=UpperNum[i];
            t_result[25][i+ii]=LowerNum[i];
		    t_result[44][i+ii]=(NFn0+NFn1)/2;
		    t_result[45][i+ii]=(sqrt((iNFnsquare0+iNFnsquare1)/4+(NFn1-NFn0)*(NFn1-NFn0)));
		    t_result[46][i+ii]=((NFn0+NFn1)/2 -(NFn00+NFn10)/2 )/((NFn00+NFn10)/2);
		    t_result[47][i+ii]=((NFn0+NFn1)/(tNum2+tNum1))/2;
		    t_result[48][i+ii]=0;
		    t_result[49][i+ii]=(((NFn0+NFn1)/(tNum2+tNum1))/2 -(NFn00+NFn10)/2/(tNumo+tNum1) )/((NFn00+NFn10)/2/(tNumo+tNum1));
		    t_result[50][i+ii]=(NFn0+NFn1)/2/totallivetime;
		    t_result[51][i+ii]=(sqrt((iNFnsquare0+iNFnsquare1)/4+(NFn1-NFn0)*(NFn1-NFn0)))/totallivetime;
		    t_result[52][i+ii]=( (NFn0+NFn1)/2/totallivetime-(NFn00+NFn10)/2/totallivetime0 )/((NFn00+NFn10)/2/totallivetime0);
    }
	
	ofstream outfile;
    string outfileName=dataVer+"/"+"result_temp_"+dataVer+".txt";
	outfile.open(outfileName.c_str());
    int outLineNum=0;
    while( outLineNum<53 )
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

    TString outStr[54][9];
	ifstream infile4out; 
    string infileoutName=dataVer+"/"+"result_temp_"+dataVer+".txt";
	infile4out.open(infileoutName.c_str(),ios::in);
	int inLineNum4out=0;
	while(inLineNum4out<53)
	{
        for( int j=1 ; j<9 ; j++ )
        {
            infile4out>>outStr[inLineNum4out][j];
        }
		inLineNum4out++;
	}
	infile4out.close();

	outStr[0][0]="====NoRpc====";//0
	outStr[1][0]="ibdNum";//1
	outStr[2][0]="daqTime";//2
	outStr[3][0]="epsi_mu";//3 
	outStr[4][0]="epsi_multi";//4
	outStr[5][0]="accNum";//5
	outStr[6][0]="+-";//6
	outStr[7][0]="accRate";//7
	outStr[8][0]="+-";//8
	outStr[9][0]="accNum_600";//9
	outStr[10][0]="+-";//10
	outStr[11][0]="accRate_600";//11
	outStr[12][0]="+-";//12
	outStr[13][0]="fnNum";//13
	outStr[14][0]="+-";//14
	outStr[15][0]="fnBs";//15
	outStr[16][0]="U/L";//16
	outStr[17][0]="fnRate";//17
	outStr[18][0]="+-";//18

	outStr[19][0]="LiNum";//18
	outStr[20][0]="+-";//18
	outStr[21][0]="LiRate";//18
	outStr[22][0]="+-";//18

	outStr[23][0]="===WithRpc===";//19
    //outStr[24][0]="ibdNum";//20
    //outStr[25][0]="(-)";//21
	outStr[24][0]="UpperNum";//20
	outStr[25][0]="LowerNum";//21
	outStr[26][0]="daqTime";//22
	outStr[27][0]="(-)";//23
	outStr[28][0]="epsi_mu";//24
	outStr[29][0]="(-)";//25
	outStr[30][0]="epsi_multi";//26
	outStr[31][0]="(-)";//27
	outStr[32][0]="accNum";//28
	outStr[33][0]="+-";//29
	outStr[34][0]="(-)";//30
	outStr[35][0]="accRate";//31
	outStr[36][0]="+-";//32
	outStr[37][0]="(-)";//33
	outStr[38][0]="accNum_600";//34
	outStr[39][0]="+-";//35
	outStr[40][0]="(-)";//36
	outStr[41][0]="accRate_600";//37
	outStr[42][0]="+-";//38
	outStr[43][0]="(-)";//39
	outStr[44][0]="fnNum";//40
	outStr[45][0]="+-";//41
	outStr[46][0]="(-)";//42
	outStr[47][0]="fnBs";//43
	outStr[48][0]="+-";//44
	outStr[49][0]="(-)";//45
	outStr[50][0]="fnRate";//46
	outStr[51][0]="+-";//47
	outStr[52][0]="(-)";//48
    
	ofstream outfile4out;
    string outfileoutName=dataVer+"/"+"result_"+dataVer+".txt";
	outfile4out.open(outfileoutName.c_str());
    int outLineNum4out=0;
    while( outLineNum4out<53 )
    {
	    for( int i=0 ; i<9 ; i++ )
	    {
		    outfile4out<< setw(13) <<outStr[outLineNum4out][i];
            if( i==8 )
            {
                outfile4out<<endl;
            }
	    }
        outLineNum4out++;
    }
	outfile4out.close();
}
