#include "TemplateAlgPackage.h"
#include "LafKernel/AlgFactory.h"

DECLARE_ALGORITHM(TemplateAlgPackage);

TemplateAlgPackage::TemplateAlgPackage(const std::string& name): AlgBase(name)
{
	//OptionParser::setOption(name,"PromptEnergyLow",promptELow);
}

bool TemplateAlgPackage::initialize()
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

    return true;
}

bool TemplateAlgPackage::execute()
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
    CurEvent->isAD()
    CurEvent->isRpcMuon()
    CurEvent->isAdMuon()
    //Candidate functions
    muonVeto_l->updateVetoWindow(CurEvent)
    printEvt(CurEvent)
    */

    PhyEvent* CurEvent= EvtBuffer->curEvt();

    return true;
}

bool TemplateAlgPackage::finalize()
{
    return true;
}

bool TemplateAlgPackage::printEvt(PhyEvent* CurEvent)
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
