#include "TrackInfo.h"
#include "LafKernel/AlgFactory.h"
#include  <vector>
#include    "TF1.h"
#include    "TMarker.h"
#include    "TLine.h"
#include    "TFile.h"

DECLARE_ALGORITHM(TrackInfo);

TrackInfo::TrackInfo(const std::string& name): AlgBase(name)
{
	//OptionParser::setOption(name,"PromptEnergyLow",promptELow);
}

bool TrackInfo::initialize()
{
    /*
    EvtBuffer = dynamic_cast<PhyEventBuf*>(service("Cycler"));
    liveTimeSvc = dynamic_cast<LiveTimeSvc*>(service("LiveTimeSvc"));
    muonVeto_l = MuonVeto::instance();

    //initialize histograms
    histName="time2lastshowermuon1";
    time2lastshowermuon1 = new TH1F(histName,"time2lastshowermuon 1.5Gev-2.5Gev",750,0.,30.);
    ntupleSvc()->attach("FILE2/lidj",time2lastshowermuon1);

    histName="showermuonNum1";
    showermuonNum1 = new TH1F(histName,"number of showermuon",liveTimeSvc->nBins(),liveTimeSvc->startTime().AsDouble(),liveTimeSvc->endTime().AsDouble());
    ntupleSvc()->attach("FILE2/lidj",showermuonNum1);
    
    //initialize trees
    m_IWSMuonTree = ntupleSvc()->bookTree("FILE2/IWSMuonTree/IWSMuonTree", "IWSMuonTree");
    m_IWSMuonTree->Branch("DetId",&m_detId,"DetId/I");
    */

    EvtBuffer = dynamic_cast<PhyEventBuf*>(service("Cycler"));
    
    h = new TH1F("h","h",5,0,5);
    ntupleSvc()->attach("FILE2/lidj",h);
    h1 = new TH1F("h1","h1",400,0,4000);
    ntupleSvc()->attach("FILE2/lidj",h1);
    t=ntupleSvc()->bookTree("FILE2/t","t");
    t->Branch("timeline",timeline,"timeline[10][25][2]/F");
    t->Branch("timepoint",timepoint,"timepoint[10][25][5]/F");
    t->Branch("charge",charge,"charge[10][25]/F");
    t->Branch("hitcount",hitcount,"hitcount[10][25]/F");
    t->Branch("energy",&energy,"energy/F");
    //t->Branch("",,"");
    //t->Branch("",,"");
    //t->Branch("",,"");
    ifcout=true;

    return true;
}

bool TrackInfo::execute()
{
    /*
    PhyEvent* CurEvent= EvtBuffer->curEvt();
    //application of muonVeto_l , 5Iwp 6Owp 7Rpc
    PhyEvent* preAdMu=muonVeto_l->preAdMuon(CurEvent,AdMuonELow,AdMuonEHigh);
    PhyEvent* preAdShowerMuon=muonVeto_l->preShowerMuon(CurEvent);
    PhyEvent* preIWMuon=muonVeto_l->preMuon(CurEvent,5);
    PhyEvent* nextIWMuon=muonVeto_l->nextMuon(CurEvent,5);
    //Candidate select conditions
    CurEvent->m_good
    CurEvent->isMuon() 
    CurEvent->isPool()
    CurEvent->isRpcMuon()
    CurEvent->isAdMuon()
    //Candidate functions
    muonVeto_l->updateVetoWindow(CurEvent)
    printEvt(CurEvent)
    */

//std::cout<<"now is execute "<<endl;
    PhyEvent* CurEvent= EvtBuffer->curEvt();
    if( !CurEvent->m_good )
    {
        return true;
    }
    if( CurEvent->isWpMuon() && CurEvent->m_det==5)
    {
        energy=CurEvent->m_energy;
        h1->Fill(CurEvent->m_energy);
        //std::cout<<"m_entry  : "<<CurEvent->m_entry<<endl;
        //if(!ifcout) return true;
        CalibReadout* calib = CurEvent->get<CalibReadout>("Event/CalibReadout/CalibReadoutHeader");
        
        if( calib->nHitsPool == 0 )
        {
            std::cout<<"someting is wrong,admuon's nHitsPool ==0 "<<endl;
        }
        for( int i=0 ; i<10 ; i++ )
        {
            for( int j=0 ; j<25 ; j++ )
            {
                charge[i][j]=0.;
                hitcount[i][j]=0.;
                for( int k=0 ; k<2 ; k++ )
                {
                    timeline[i][j][k]=0.;
                }
                for( int l=0 ; l<5 ; l++ )
                {
                    timepoint[i][j][l]=0.;
                }
                
            }
        }
        //float mintime,maxtime;
       // mintime=calib->timePool.at(0);
       // maxtime=calib->timePool.at(0);
        for( unsigned int i=0 ; i<calib->nHitsPool ; i++ )
        {
            if(timeline[calib->wallNumber.at(i)-1][calib->wallSpot.at(i)-1][0]==0)
            {
                timeline[calib->wallNumber.at(i)-1][calib->wallSpot.at(i)-1][0]=calib->timePool.at(i);
                timeline[calib->wallNumber.at(i)-1][calib->wallSpot.at(i)-1][1]=calib->timePool.at(i);
            }
            timeline[calib->wallNumber.at(i)-1][calib->wallSpot.at(i)-1][1]=calib->timePool.at(i);
            timepoint[calib->wallNumber.at(i)-1][calib->wallSpot.at(i)-1][calib->hitCountPool.at(i)]=calib->timePool.at(i);
            charge[calib->wallNumber.at(i)-1][calib->wallSpot.at(i)-1]+=calib->chargePool.at(i);
            hitcount[calib->wallNumber.at(i)-1][calib->wallSpot.at(i)-1]=calib->hitCountPool.at(i);
            h->Fill(calib->hitCountPool.at(i));
            /*
            if( calib->timePool.at(i)<mintime )
            {
                mintime=calib->timePool.at(i);
            }
            if( calib->timePool.at(i)>maxtime )
            {
                maxtime=calib->timePool.at(i);
            }
            */
            //if(ifcout) std::cout<<"timePool  : "<<calib->timePool.at(i)<<endl;
        }
        t->Fill();
        /*
        for( int i=0 ; i<10 ; i++ )
        {
            for( int j=0 ; j<25 ; j++ )
            {
                for( int k=0 ; k<2 ; k++ )
                {
                    //h->SetBinContent(((i)*25+j+1)*2-1+k,timeline[i][j][k]);
                    if( timeline[i][j][k]==0 )
                    {
                        std::cout<<" !!! timeline[i][j][k]==0 !!! "<<endl;
                        ifcout=true;
                    }
                }
                
            }
            
        }

        if(ifcout) std::cout<<"!!! new one !!! "<<endl;
        ifcout=false;
        */
        
    }

    return true;
}

bool TrackInfo::finalize()
{
    return true;
}

bool TrackInfo::printEvt(PhyEvent* CurEvent)
{
	std::cout<<"===> info <==="<<endl;
	std::cout<<"entry = "<<CurEvent->m_entry<<endl;
	std::cout<<"fileNum = "<<CurEvent->m_fileNum<<endl;
	std::cout<<"localentry = "<<CurEvent->m_localEntry<<endl;
	std::cout<<"trigtime = "<<CurEvent->m_trigTime<<endl;
	std::cout<<"trigType = "<<CurEvent->m_trigType<<endl;
	std::cout<<"det = "<<CurEvent->m_det<<endl;
	std::cout<<"energy = "<<CurEvent->m_energy<<endl;
	std::cout<<"x = "<<CurEvent->m_x<<endl;
	std::cout<<"y = "<<CurEvent->m_y<<endl;
	std::cout<<"z = "<<CurEvent->m_z<<endl;
	std::cout<<"rawEvis = "<<CurEvent->m_rawEvis<<endl;
	std::cout<<"energy() = "<<CurEvent->energy()<<endl;
	std::cout<<"nPmt = "<<CurEvent->m_nPmt<<endl;
	std::cout<<"flasherTag = "<<CurEvent->m_flasherTag<<endl;
	std::cout<<"forceTrigTag = "<<CurEvent->m_forceTrigTag<<endl;
	std::cout<<"crossTrigTag = "<<CurEvent->m_crossTrigTag<<endl;
	std::cout<<"rpcNoiseTag = "<<CurEvent->m_rpcNoiseTag<<endl;
	std::cout<<"adLowEnergyTag = "<<CurEvent->m_adLowEnergyTag<<endl;
	std::cout<<"goodevent = "<<CurEvent->m_good<<endl;
	std::cout<<"muontag = "<<CurEvent->m_muonTag<<endl;
	return true;
}
