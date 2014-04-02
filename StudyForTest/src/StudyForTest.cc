#include "StudyForTest.h"
#include "LafKernel/AlgFactory.h"

DECLARE_ALGORITHM(StudyForTest);

StudyForTest::StudyForTest(const std::string& name): AlgBase(name)
{
	//OptionParser::setOption(name,"PromptEnergyLow",promptELow);
    maxPE=0.;
    /*
    for( int i=0 ; i<8 ; i++ )
    {
        for( int j=0 ; j<10 ; j++ )
        {
            for( int k=0 ; k<10 ; k++ )
            {
                for( int l=0 ; l<10 ; l++ )
                {
                    obsPE[i][j][k][l]=0.;
                }
                
            }
            
        }
        
    }
    obsPE[0][0][0][0]=0.;
    for( int i=0 ; i<8 ; i++ )
    {
        for( int j=0 ; j<10 ; j++ )
        {
            for( int k=0 ; k<10 ; k++ )
            {
                for( int l=1 ; l<10 ; l++ )
                {
                    obsPE[i][j][k][l]=(i*1000+j*100+k*10+l)*TMath::Sqrt((TMath::Sqrt(1+8*TMath::Power(((i*1000+j*100+k*10+l)/SatPENum),alpha))-1)/(4*TMath::Power(((i*1000+j*100+k*10+l)/SatPENum),alpha)));
                    std::cout<<"obsPE["<<i <<"]["<<j <<"]["<<k <<"]["<<l <<"]  : "<<obsPE[i][j][k][l]<<endl;
                }
                
            }
            
        }
        
    }
    std::cout<<"obsPE 20000 : "<<
   (20000)*TMath::Sqrt((TMath::Sqrt(1+8*TMath::Power(((20000)/SatPENum),alpha))-1)/(4*TMath::Power(((20000)/SatPENum),alpha)))
   <<endl;
    std::cout<<"obsPE 25000 : "<<
   (25000)*TMath::Sqrt((TMath::Sqrt(1+8*TMath::Power(((25000)/SatPENum),alpha))-1)/(4*TMath::Power(((25000)/SatPENum),alpha)))
   <<endl;
    std::cout<<"obsPE 30000 : "<<
   (30000)*TMath::Sqrt((TMath::Sqrt(1+8*TMath::Power(((30000)/SatPENum),alpha))-1)/(4*TMath::Power(((30000)/SatPENum),alpha)))
   <<endl;
    std::cout<<"obsPE 35000 : "<<
   (35000)*TMath::Sqrt((TMath::Sqrt(1+8*TMath::Power(((35000)/SatPENum),alpha))-1)/(4*TMath::Power(((35000)/SatPENum),alpha)))
   <<endl;
    std::cout<<"obsPE 40000 : "<<
   (40000)*TMath::Sqrt((TMath::Sqrt(1+8*TMath::Power(((40000)/SatPENum),alpha))-1)/(4*TMath::Power(((40000)/SatPENum),alpha)))
   <<endl;
    */
}


bool StudyForTest::initialize()
{
    /*
    EvtBuffer = dynamic_cast<PhyEventBuf*>(service("Cycler"));
    liveTimeSvc = dynamic_cast<LiveTimeSvc*>(service("LiveTimeSvc"));
    muonVeto_l = MuonVeto::instance();

    //initialize histograms
    histName="a";
    a = new TH1F(histName,"a",750,0.,30.);
    ntupleSvc()->attach("FILE2/lidj",a);

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
    /*
    EvtBuffer = dynamic_cast<PhyEventBuf*>(service("Cycler"));
    muonVeto_l = MuonVeto::instance();
    histName="AllTrigger";
    AllTrigger = new TH1F(histName,"AllTrigger",300,0.,12.);
    ntupleSvc()->attach("FILE2/lidj",AllTrigger);
    */
    EvtBuffer = dynamic_cast<PhyEventBuf*>(service("Cycler"));
    m_TriggerTree = ntupleSvc()->bookTree("FILE2/TriggerTree", "TriggerTree");
    m_TriggerTree->Branch("DetId",&DetId,"DetId/I");
    m_TriggerTree->Branch("x",&x,"x/F");
    m_TriggerTree->Branch("y",&y,"y/F");
    m_TriggerTree->Branch("z",&z,"z/F");
    m_TriggerTree->Branch("energy",&energy,"energy/F");


    histName="AdMuonEnergy";
    AdMuonEnergy = new TH1F(histName,"AdMuonEnergy",3000,0.,30000.);
    ntupleSvc()->attach("FILE2/lidj",AdMuonEnergy);
    histName="AdMuonEnergyAfterCor";
    AdMuonEnergyAfterCor = new TH1F(histName,"AdMuonEnergyAfterCor",3000,0.,30000.);
    ntupleSvc()->attach("FILE2/lidj",AdMuonEnergyAfterCor);
    histName="AdMuonEnergyVsAfterCor";
    AdMuonEnergyVsAfterCor = new TH2F(histName,"AdMuonEnergyVsAfterCor",3000,0.,30000.,3000,0.,30000.);
    ntupleSvc()->attach("FILE2/lidj",AdMuonEnergyVsAfterCor);


    return true;
}

bool StudyForTest::execute()
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
    //singleTrigger4Li(CurEvent);
    muonEnergyCorrection(CurEvent);


    return true;
}
bool StudyForTest::muonEnergyCorrection(PhyEvent* CurEvent)
{
    if( !CurEvent->m_good )
    {
        return true;
    }
    if( CurEvent->isAdMuon()||CurEvent->isShowerMuon() )
    {

        AdMuonEnergy->Fill(CurEvent->energy()); 
        CalibReadout* calib = CurEvent->get<CalibReadout>("Event/CalibReadout/CalibReadoutHeader");
        double chargeADtotal=0.;
        double chargeADtotalAfterCor=0.;
        SatPENum=1000.;
        alpha=2.;
        //std::cout<<"calib->nHitsAD  : "<<calib->nHitsAD<<endl;
        //double SaturationCoefficient=0.;
        for( unsigned int i=0 ; i<calib->nHitsAD ; i++ )
        {
            if( calib->ring.at(i)>8 || calib->column.at(i)>24 )
            {
                std::cout<<"wrong "<<endl;
            }
            if( calib->hitCountAD.at(i)==0 )
            {
                if( calib->chargeAD.at(i)>maxPE )
                {
                    maxPE=calib->chargeAD.at(i);
                }
                chargeADtotal+=calib->chargeAD.at(i);
                //SaturationCoefficient=TMath::Sqrt((TMath::Sqrt(1+8*TMath::Power((calib->chargeAD.at(i)/SatPENum),alpha))-1)/(4*TMath::Power((calib->chargeAD.at(i)/SatPENum),alpha)));
                double chargeADAfterCor=0.;
                /*
                int posi,posj,posk,posl;
                for( posi=0 ; posi<8 ; posi++ )
                {
                    if( calib->chargeAD.at(i)<obsPE[posi][0][0][0] )
                    {
                        break;
                    }
                }
                for( posj=0 ; posj<10 ; posi++ )
                {
                    if( calib->chargeAD.at(i)<obsPE[posi-1][posj][0][0] )
                    {
                        break;
                    }
                }
                for( posk=0 ; posk<10 ; posk++ )
                {
                    if( calib->chargeAD.at(i)<obsPE[posi-1][posj-1][posk][0] )
                    {
                        break;
                    }
                }
                for( posl=0 ; posl<10 ; posl++ )
                {
                    if( calib->chargeAD.at(i)<obsPE[posi-1][posj-1][posk-1][posl] )
                    {
                        break;
                    }
                }
                */
                //chargeADAfterCor=calib->chargeAD.at(i)*TMath::Sqrt(2*calib->chargeAD.at(i)*calib->chargeAD.at(i)/(1000000)+1);
                //chargeADAfterCor=obsPE[posi-1][posj-1][posk-1][posl-1]+(calib->chargeAD.at(i)-obsPE[posi-1][posj-1][posk-1][posl-1])/(obsPE[posi-1][posj-1][posk-1][posl]-obsPE[posi-1][posj-1][posk-1][posl-1]);
                //chargeADtotalAfterCor+=chargeADAfterCor;
                chargeADtotalAfterCor+=calib->chargeAD.at(i);
                
                //std::cout<<"calib->chargeAD.at("<<i<<")  : "<<calib->chargeAD.at(i)<<endl;
            }
        }
        double EnergyRecCoefficient=chargeADtotal/CurEvent->m_rawEvis;
        double HeterogeneityCoefficient=CurEvent->m_enrec/CurEvent->m_rawEvis;

        double EnergyAfterSatCor=chargeADtotalAfterCor/EnergyRecCoefficient;
        double EnergyAfterSatAndHetCor=EnergyAfterSatCor*HeterogeneityCoefficient;

        AdMuonEnergyVsAfterCor->Fill(CurEvent->energy(),EnergyAfterSatAndHetCor);
        AdMuonEnergyAfterCor->Fill(EnergyAfterSatAndHetCor);
        
        //std::cout<<"CurEvent->m_rawEvis  : "<<CurEvent->m_rawEvis<<endl;
        //std::cout<<"EnergyAfterSatCor  : "<<EnergyAfterSatCor<<endl;
        //std::cout<<"CurEvent->m_enrec  : "<<CurEvent->m_enrec<<endl;
        //std::cout<<"EnergyAfterSatAndHetCor  : "<<EnergyAfterSatAndHetCor<<endl;

        /*
        //std::cout<<"CurEvent->energy()  : "<<CurEvent->energy()<<endl;
        std::cout<<"CurEvent->m_enrec  : "<<CurEvent->m_enrec <<endl;
        std::cout<<"CurEvent->m_rawEvis  : "<<CurEvent->m_rawEvis<<endl;
        //std::cout<<"CurEvent->m_eprec  : "<<CurEvent->m_eprec<<endl;
        std::cout<<"chargeADtotal  : "<<chargeADtotal<<endl;
        std::cout<<"HeterogeneityCoefficient  : "<<HeterogeneityCoefficient<<endl;
        std::cout<<"chargeADtotal/162  : "<<chargeADtotal/HeterogeneityCoefficient<<endl;
        //std::cout<<"energy()/(chargeADtotal/162)  : "<<CurEvent->energy()/(chargeADtotal/HeterogeneityCoefficient)<<endl;
        std::cout<<"CurEvent->m_rawEvis/(chargeADtotal/162)  : "<<CurEvent->m_rawEvis/(chargeADtotal/HeterogeneityCoefficient)<<endl;
        std::cout<<" "<<endl;
        */

    }
    return true;
    
}
bool StudyForTest::singleTrigger4Li(PhyEvent* CurEvent)
{

    if( !CurEvent->m_good )
    {
        return true;
    }
    if( CurEvent->isMuon() )
    {
        return true;
    }
    if( !CurEvent->isAD() )
    {
        return true;
    }
    if( CurEvent->energy()>0.7 && CurEvent->energy()<12. )
    {
        DetId=CurEvent->m_det;
        x=CurEvent->m_x;
        y=CurEvent->m_y;
        z=CurEvent->m_z;
        energy=CurEvent->energy();
        m_TriggerTree->Fill();
        
    }
    return true;
}

bool StudyForTest::finalize()
{
	std::cout<<"StudyForTest finalize..."<<endl;
    //AdMuonEnergyAfterCor->SetLineColor(kRed);
    //AdMuonEnergyAfterCor->Draw();
    //AdMuonEnergy->SetLineColor(kBlue);
    //AdMuonEnergy->Draw("same");
    std::cout<<"maxPE  : "<<maxPE<<endl;

    return true;
}

bool StudyForTest::printEvt(PhyEvent* CurEvent)
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
