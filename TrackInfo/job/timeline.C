#include    "TFile.h"
#include    "TColor.h"
#include    "TTree.h"
#include    "TLine.h"
#include    "TMarker.h"
#include    "TCanvas.h"
#include    "TF1.h"
#include    "TH2.h"
#include  <sstream>
#include  <sstream>
#include  <iostream>
using namespace std;
void timeline()
{

        TFile *f=new TFile("/afs/ihep.ac.cn/users/l/lidj/largedata/TrackInfo/EH1/21344_track_wp.root");
        TTree *t = (TTree*)f->Get("t");

        float timeline[10][25][2];
        float timepoint[10][25][5];
        float charge[10][25];
        float hitcount[10][25];
        float energy;
        for( int i=0 ; i<10 ; i++ )
        {
            for( int j=0 ; j<25 ; j++ )
            {
                for( int k=0 ; k<2 ; k++ )
                {
                    timeline[i][j][k]=0.;
                }
                for( int l=0 ; l<5 ; l++ )
                {
                    timepoint[i][j][l]=0.;
                }
                charge[i][j]=0.;

            }
        }
        t->SetBranchAddress("timeline",timeline);
        t->SetBranchAddress("timepoint",timepoint);
        t->SetBranchAddress("charge",charge);
        t->SetBranchAddress("hitcount",hitcount);
        t->SetBranchAddress("energy",&energy);
        Int_t nentries =  t->GetEntries();
        std::cout<<"nentries  : "<<nentries<<endl;

        //for( int i=0 ; i<nentries ; i++ )
        for( int i1=0 ; i1<30; i1++ )
        {
            t->GetEntry(i1);
        
        
	        float mintime=0,maxtime=0;
	        bool initi=true;
	        for( int i=0 ; i<10 ; i++ )
	        {
	            for( int j=0 ; j<25 ; j++ )
	            {
	                if( (timeline[i][j][0]!=0)&&(initi) )
	                {
	                    mintime=timeline[i][j][0];
	                    maxtime=timeline[i][j][0];
	                    initi=false;
	                }
	                if( (timeline[i][j][0]!=0)&&(timeline[i][j][0]<mintime) )
	                {
	                    mintime=timeline[i][j][0];
	                }
	                if((timeline[i][j][0]!=0)&&( timeline[i][j][1]>maxtime) )
	                {
	                    maxtime=timeline[i][j][1];
	                }
	                //std::cout<<"timeline[i][j][0]  : "<<timeline[i][j][0]<<endl;
	            }
	        }
            /*	
	        //TFile *f=new TFile("/afs/ihep.ac.cn/users/l/lidj/file/TrackInfo/run/EH1/test.root","recreate")
	        TCanvas * myC1 = new TCanvas("myC1","A Canvas",10,10,1000,600);
	        TF1 *f1 = new TF1("func1","5",mintime-10,maxtime+10);
	        f1->SetLineColor(10);
	        f1->Draw();
	        TLine l[10][25];
	        TMarker m[10][25];
	        TMarker mpoint[10][25][5];
	        for( int i=0 ; i<10 ; i++ )
	        {
	            for( int j=0 ; j<25 ; j++ )
	            {
	                if(timeline[i][j][0]!=0.)
	                {
		                if( timeline[i][j][0]==timeline[i][j][1] )
		                {
		                    std::cout<<"make a marker "<<endl;
		                   m[i][j].SetX(timeline[i][j][0]);
		                   m[i][j].SetY(i+j*0.04);
		                   m[i][j].Draw();
		                }
	                    else
	                    {
		                    l[i][j].SetX1(timeline[i][j][0]);
		                    l[i][j].SetX2(timeline[i][j][1]);
		                    l[i][j].SetY1(i+j*0.04);
		                    l[i][j].SetY2(i+j*0.04);
		                    l[i][j].SetLineColor(i+1);
		                    l[i][j].SetLineWidth(1);
		                    l[i][j].Draw();
	                    }
	                }
	                else
	                {
	                    std::cout<<"!!! ==0 !!! "<<endl; 
	                }
	                for( int k=0 ; k<5 ; k++ )
	                {
	                    if( timepoint[i][j][k] != 0. )
	                    {
		                   mpoint[i][j][k].SetX(timepoint[i][j][k]);
		                   mpoint[i][j][k].SetY(i+j*0.04);
		                   mpoint[i][j][k].Draw();
	                    }
	                }
	                
	            }
	        
	        }
	
	        stringstream savename;
	        savename<<"wp_timeline";
	        savename<<i1;
	        savename<<".gif";
	        myC1->SaveAs(savename.str().c_str());
            */
	        TCanvas * myC2 = new TCanvas("myC2","A Canvas",10,10,1000,600);
            //TPaveLabel* label = new TPaveLabel(0,0,50,200,"Some text"); 
            //label->Draw();
	        TH2F* h2=new TH2F("charge","charge",25,0,25,10,0,10);
            double Red[3]    = { 1.00, 0.00, 0.00};
            double Green[3]  = { 0.00, 1.00, 0.00};
            //double Blue[3]   = { 1.00, 0.00, 1.00};
            double Blue[3]   = { 0.00, 0.00, 1.00};
            double Length[3] = { 0.00, 0.50, 1.00 };
            int nb=50;
            TColor::CreateGradientColorTable(3,Length,Blue,Green,Red,nb);
            h2->SetContour(nb);
	        for( int i=0 ; i<10 ; i++ )
	        {
	            for( int j=0 ; j<25 ; j++ )
	            {
                    if( charge[i][j]!=0 )
                    {
	                    h2->Fill(j,i,(int(charge[i][j])+float(hitcount[i][j]+1)/10));
                    }
	            }
	            
	        }
            stringstream xtitle;
            xtitle<<"energy = ";
            xtitle<<energy;
            h2->GetXaxis()->SetTitle(xtitle.str().c_str());
	        h2->Draw("COLZTEXT");
	        //h2->Draw("CONTZ");
	        stringstream savename2;
	        savename2<<"wp_charge";
	        savename2<<i1;
	        savename2<<".gif";
	        myC2->SaveAs(savename2.str().c_str());
        
        //myC1->SaveAs("myex33.gif");
        //f->Write();
        }
}
