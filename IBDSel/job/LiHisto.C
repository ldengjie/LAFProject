#define LiHisto_cxx
// The class definition in LiHisto.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("LiHisto.C")
// Root > T->Process("LiHisto.C","some options")
// Root > T->Process("LiHisto.C+")
//

#include "LiHisto.h"
#include <TH2.h>
#include <TStyle.h>
#include  <iostream>


void LiHisto::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

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
   option+="Li_";
   option+=dataVer;
   option+=".root";
   TString fileName=dataVer+"/"+option;
   file = new TFile(fileName,"RECREATE");
    for( int j=0 ; j<6 ; j++ )
    {
        histname="time2lastshowermuon";
        histname+=j+1;
        t2lastshowermuon[4][j]=new TH1F(histname,"time since last showermuon",100000,0,1000);

    }
    
    for( int i=0 ; i<ADNum ; i++ )
    {
        histname="AD";
        histname+=i+1;
        histname+="time2lastshowermuon";
        for( int j=0 ; j<6 ; j++ )
        {
            TString histnameTmp=histname;
            histnameTmp+=j+1;
            t2lastshowermuon[i][j]=new TH1F(histnameTmp,"time since last showermuon",100000,0,1000);

        }
        
    }
    
}

void LiHisto::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t LiHisto::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either LiHisto::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.
    GetEntry(entry);
    if( isLi==1 )
    {
        for( int j=0 ; j<6 ; j++ )
        {
            t2lastshowermuon[4][j]->Fill(promptT2Muon[10+j]);
            //std::cout<<"promptT2Muon[10+j]  : "<<promptT2Muon[10+j]<<endl;
            t2lastshowermuon[det-1][j]->Fill(promptT2Muon[10+j]);
        }
    }
    



   return kTRUE;
}

void LiHisto::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void LiHisto::Terminate()
{
   // THE TERMINATE() FUNCTION IS THE LAST FUNCTION TO BE CALLED DURING
   // A QUERY. IT ALWAYS RUNS ON THE CLIENT, IT CAN BE USED TO PRESENT
   // THE RESULTS GRAPHICALLY OR SAVE THE RESULTS TO FILE.

    for( int j=0 ; j<6 ; j++ )
    {
        t2lastshowermuon[4][j]->Write();
        for( int i=0 ; i<ADNum ; i++ )
        {
            t2lastshowermuon[i][j]->Write();
        }
        
    }
    file->Close();
    
    
}
