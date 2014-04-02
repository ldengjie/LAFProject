#define RpcAna_cxx

#include "RpcAna.h"
using namespace std;
void RpcAna::Begin(TTree * /*tree*/)
{
   TString option = GetOption();
	option+="AnaRpc.root";
	file = new TFile(option,"RECREATE");
	for( int i=0 ; i<4 ; i++ )
	{
	   wholeNum[i]=0;
	   IbdNum[i]=0;
	   vetoedNum[i]=0;
	}
	
   for( int j=0 ; j<4 ; j++ )
   {
		for( int i=0 ; i<3 ; i++ )
		{
		   histname="AD";
		   histname+=j+1;
		   histname+="pt2rpc";
		   histname+=i+1;
		   pt2Rpc[j][i] = new TH1F(histname,"prompt time to Rpc Muon",5000,0.,50000.);//us
		   histname="AD";
		   histname+=j+1;
		   histname+="dt2rpc";
		   histname+=i+1;
		   dt2Rpc[j][i] = new TH1F(histname,"delayed time to Rpc Muon",5000,0.,50000.);//us
		   histname="AD";
		   histname+=j+1;
		   histname+="promptEnergy";
		   histname+=i+1;
		   pE[j][i] = new TH1F(histname,"Energy of promt signal",120,0.7 ,12.0);
		   histname="AD";
		   histname+=j+1;
		   histname+="delayedEnergy";
		   histname+=i+1;
		   dE[j][i] = new TH1F(histname,"Energy of delayed signal",60,6.0,12.0);
		   histname="AD";
		   histname+=j+1;
		   histname+="pxy";
		   histname+=i+1;
		   pxy[j][i] = new TH2F(histname,"pxy",30,-3.,3.,30,-3.,3.);
		   histname="AD";
		   histname+=j+1;
		   histname+="dxy";
		   histname+=i+1;
		   dxy[j][i] = new TH2F(histname,"dxy",30,-3.,3.,30,-3.,3.);
		   histname="AD";
		   histname+=j+1;
		   histname+="prz";
		   histname+=i+1;
		   prz[j][i] = new TH2F(histname,"prz",30,0,3.,30,-3.,3.);
		   histname="AD";
		   histname+=j+1;
		   histname+="drz";
		   histname+=i+1;
		   drz[j][i] = new TH2F(histname,"drz",30,0,3.,30,-3.,3.);
		}
	   
   }
   
}

void RpcAna::SlaveBegin(TTree * /*tree*/)
{

   TString option = GetOption();

}

Bool_t RpcAna::Process(Long64_t entry)
{
	GetEntry(entry);
	/*
	if( entry%10000==0 )
	{
		//std::cout<<"Now is "<<entry<<"  "<<entry/chain.GetEntries()<<endl;
		std::cout<<"Now is "<<entry<<endl;
	}
	*/
	pt2Rpc[det-1][0]->Fill(promptT2Muon[4]*1e6);
	dt2Rpc[det-1][0]->Fill(delayedT2Muon[4]*1e6);
	pE[det-1][0]->Fill(energy[0]);
	dE[det-1][0]->Fill(energy[1]);
	pxy[det-1][0]->Fill(x[0]/1000,y[0]/1000);
	dxy[det-1][0]->Fill(x[1]/1000,y[1]/1000);
	prz[det-1][0]->Fill(sqrt(x[0]*x[0]+y[0]*y[0])/1000,z[0]/1000);
	drz[det-1][0]->Fill(sqrt(x[1]*x[1]+y[1]*y[1])/1000,z[1]/1000);
	wholeNum[det-1]++;
	if( isIbd==1 )
	{
		pt2Rpc[det-1][2]->Fill(promptT2Muon[4]*1e6);
		dt2Rpc[det-1][2]->Fill(delayedT2Muon[4]*1e6);
		pE[det-1][2]->Fill(energy[0]);
		dE[det-1][2]->Fill(energy[1]);
		pxy[det-1][2]->Fill(x[0]/1000,y[0]/1000);
		dxy[det-1][2]->Fill(x[1]/1000,y[1]/1000);
		prz[det-1][2]->Fill(sqrt(x[0]*x[0]+y[0]*y[0])/1000,z[0]/1000);
		drz[det-1][2]->Fill(sqrt(x[1]*x[1]+y[1]*y[1])/1000,z[1]/1000);
		IbdNum[det-1]++;

	}else
	{
		pt2Rpc[det-1][1]->Fill(promptT2Muon[4]*1e6);
		dt2Rpc[det-1][1]->Fill(delayedT2Muon[4]*1e6);
		pE[det-1][1]->Fill(energy[0]);
		dE[det-1][1]->Fill(energy[1]);
		pxy[det-1][1]->Fill(x[0]/1000,y[0]/1000);
		dxy[det-1][1]->Fill(x[1]/1000,y[1]/1000);
		prz[det-1][1]->Fill(sqrt(x[0]*x[0]+y[0]*y[0])/1000,z[0]/1000);
		drz[det-1][1]->Fill(sqrt(x[1]*x[1]+y[1]*y[1])/1000,z[1]/1000);
		vetoedNum[det-1]++;
	}


   return kTRUE;
}

void RpcAna::SlaveTerminate()
{

}

void RpcAna::Terminate()
{
	for( int i=0 ; i<4 ; i++ )
	{
		std::cout<<""<<endl;
		std::cout<<"AD"<<i+1<<" wholeNum : "<<wholeNum[i] <<endl;
		std::cout<<"AD"<<i+1<<"IbdNum : "<<IbdNum[i] <<endl;
		std::cout<<"AD"<<i+1<<"vetoedNum : "<<vetoedNum[i] <<endl;
	}
	
	for( int j=0 ; j<4 ; j++ )
	{
		for( int i=0 ; i<3 ; i++ )
		{
			pt2Rpc[j][i]->Write();
			pE[j][i]->Write();
			pxy[j][i]->Write();
			prz[j][i]->Write();
			dt2Rpc[j][i]->Write();
			dE[j][i]->Write();
			dxy[j][i]->Write();
			drz[j][i]->Write();
		}
	}
	//file->Write();
	file->Close();
}
