#include "IBDSel.h"
#include "LafKernel/AlgFactory.h"
#include  <algorithm>
#include  <fstream>

DECLARE_ALGORITHM(IBDSel);

    IBDSel::IBDSel(const std::string& name)
: AlgBase(name)
{
    OptionParser::setOption(name,"PromptEnergyLow",promptELow);
    OptionParser::setOption(name,"PromptEnergyHigh",promptEHigh);
    OptionParser::setOption(name,"PromptEnergyHigh4Fn",promptEHigh4Fn);
    OptionParser::setOption(name,"DelayedEnergyLow",delayedELow);
    OptionParser::setOption(name,"DelayedEnergyHigh",delayedEHigh);
    OptionParser::setOption(name,"DelayedEnergyLow4Li",delayedEnergyLow4Li);
    OptionParser::setOption(name,"DelayedEnergyHigh4Li",delayedEnergyHigh4Li);
    OptionParser::setOption(name,"AdMuonELow",AdMuonELow);
    OptionParser::setOption(name,"AdMuonEHigh",AdMuonEHigh);
    OptionParser::setOption(name,"IbdTimeIntervalMin",IbdTimeIntervalMin);
    OptionParser::setOption(name,"IbdTimeIntervalMax",IbdTimeIntervalMax);
    OptionParser::setOption(name,"IbdDisInterval",IbdDisInterval);
    OptionParser::setOption(name,"Time2LastBufEvent",Time2LastBufEvent);
    //OptionParser::setOption(name,"Time2PreAdEvent",Time2PreAdEvent);
    //OptionParser::setOption(name,"Time2NextAdEvent",Time2NextAdEvent);
    OptionParser::setOption(name,"DelayedTrigTime2AdMuon",DelayedTrigTime2AdMuon=1.e-3);
    OptionParser::setOption(name,"DelayedTrigTime2IWpMuon",DelayedTrigTime2IWpMuon=6.e-4);
    OptionParser::setOption(name,"DelayedTrigTime2OWpMuon",DelayedTrigTime2OWpMuon=6.e-4);
    OptionParser::setOption(name,"DelayedTrigTime2RpcMuon",DelayedTrigTime2RpcMuon=6.e-4);
    OptionParser::setOption(name,"DelayedTrigTime2AdShowerMuon",DelayedTrigTime2AdShowerMuon=1.);
    OptionParser::setOption(name,"PromptTrigTime2AdShowerMuon",PromptTrigTime2AdShowerMuon=1.);
    OptionParser::setOption(name,"DelayedTrigTime2AdMuon4Li",DelayedTrigTime2AdMuon4Li=1.e-3);
    OptionParser::setOption(name,"DelayedTrigTime2IWpMuon4Li",DelayedTrigTime2IWpMuon4Li=6.e-4);
    OptionParser::setOption(name,"DelayedTrigTime2OWpMuon4Li",DelayedTrigTime2OWpMuon4Li=6.e-4);
    OptionParser::setOption(name,"DelayedTrigTime2RpcMuon4Li",DelayedTrigTime2RpcMuon4Li=6.e-4);
    OptionParser::setOption(name,"DelayedTrigTime2AdShowerMuon4Li",DelayedTrigTime2AdShowerMuon4Li=1.e-3);

    lastOwpMuonTrigtime.SetSec(0);
    lastOwpMuonTrigtime.SetNanoSec(0);
    lastIwpMuonTrigtime.SetSec(0);
    lastIwpMuonTrigtime.SetNanoSec(0);
    lastRpcMuonTrigtime.SetSec(0);
    lastRpcMuonTrigtime.SetNanoSec(0);

    for( int i=0 ; i<GlobalVar::NumADs ; i++ )
    {
        AdEvtBuf[i].clear();
        VetoedBuf[i].clear();
        time2MuonBuf[i].clear();
        for( int j=0 ; j<6 ; j++ )
        {
            lastshowermuonTrigtime[j][i].SetSec(0);
            lastshowermuonTrigtime[j][i].SetNanoSec(0);
            lastshowermuonTrigtime0[j][i].SetSec(0);

            lastshowermuonTrigtime0[j][i].SetNanoSec(0);
            lastshowermuonTrigtimeVec[j][i].clear();
            lastshowermuonTrigtime0Vec[j][i].clear();
        }

        lastAdMuonTrigtime[i].SetSec(0);
        lastAdMuonTrigtime[i].SetNanoSec(0);
        lastShowerMuonTrigtime[i].SetSec(0);
        lastShowerMuonTrigtime[i].SetNanoSec(0);
        lastNewAdMuonTrigtime[i].SetSec(0);
        lastNewAdMuonTrigtime[i].SetNanoSec(0);
        lastNewShowerMuonTrigtime[i].SetSec(0);
        lastNewShowerMuonTrigtime[i].SetNanoSec(0);
    }

    saveSingle=false;
    saveMuon=false;
    relatedTime=0.000002;//s
}

bool IBDSel::initialize()
{
    EvtBuffer = dynamic_cast<PhyEventBuf*>(service("Cycler"));
    liveTimeSvc = dynamic_cast<LiveTimeSvc*>(service("LiveTimeSvc"));
    muonVeto_l = MuonVeto::instance();

    //for( int i=0 ; i<7 ; i++ )
    //{
    //ADMuonNum[i]=0;
    //}

    for( int i=0 ; i<6 ; i++ )
    {
        histName="time2lastshowermuon";
        histName+=i+1;
        time2lastshowermuon[i] = new TH1F(histName,"time2lastshowermuon",10000,0.,100.); 
        ntupleSvc()->attach("FILE1/lidj",time2lastshowermuon[i]);
        histName+="0";
        time2lastshowermuon0[i] = new TH1F(histName,"time2lastshowermuon",10000,0.,100.);
        ntupleSvc()->attach("FILE1/lidj",time2lastshowermuon0[i]);
        histName="showermuonNum";
        histName+=i+1;
        showermuonNum[i] = new TH1F(histName,"number of AD showermuon",liveTimeSvc->nBins(),liveTimeSvc->startTime().AsDouble(),liveTimeSvc->endTime().AsDouble());
        ntupleSvc()->attach("FILE1/lidj",showermuonNum[i]);
        histName+="0";
        showermuonNum0[i] = new TH1F(histName,"number of AD showermuon",liveTimeSvc->nBins(),liveTimeSvc->startTime().AsDouble(),liveTimeSvc->endTime().AsDouble());
        ntupleSvc()->attach("FILE1/lidj",showermuonNum0[i]);

    }
    /*    
     */
    for( int i=0 ; i<GlobalVar::NumADs ; i++ )
    {
        histName="AD";
        histName+=i+1;
        histName+="NumOfAll";
        NumOfAll[i] = new TH1F(histName,"num of all triggers more than 1.5MeV",liveTimeSvc->nBins(),liveTimeSvc->startTime().AsDouble(),liveTimeSvc->endTime().AsDouble());
        ntupleSvc()->attach("FILE1/lidj",NumOfAll[i]);
        histName="AD";
        histName+=i+1;
        histName+="NumOfAll0";
        NumOfAll0[i] = new TH1F(histName,"num of all triggers more than 1.5MeV",liveTimeSvc->nBins(),liveTimeSvc->startTime().AsDouble(),liveTimeSvc->endTime().AsDouble());
        ntupleSvc()->attach("FILE1/lidj",NumOfAll0[i]);
        histName="AD";
        histName+=i+1;
        histName+="NumN";
        NumN[i] = new TH1F(histName,"num of n-like with RPC",liveTimeSvc->nBins(),liveTimeSvc->startTime().AsDouble(),liveTimeSvc->endTime().AsDouble());
        ntupleSvc()->attach("FILE1/lidj",NumN[i]);
        histName="AD";
        histName+=i+1;
        histName+="NumE";
        NumE[i] = new TH1F(histName,"num of n-like with RPC",liveTimeSvc->nBins(),liveTimeSvc->startTime().AsDouble(),liveTimeSvc->endTime().AsDouble());
        ntupleSvc()->attach("FILE1/lidj",NumE[i]);
        histName="AD";
        histName+=i+1;
        histName+="NumE_600";
        NumE_600[i] = new TH1F(histName,"num of n-like with RPC",liveTimeSvc->nBins(),liveTimeSvc->startTime().AsDouble(),liveTimeSvc->endTime().AsDouble());
        ntupleSvc()->attach("FILE1/lidj",NumE_600[i]);
        histName="AD";
        histName+=i+1;
        histName+="NumN0";
        NumN0[i] = new TH1F(histName,"num of n-like with RPC",liveTimeSvc->nBins(),liveTimeSvc->startTime().AsDouble(),liveTimeSvc->endTime().AsDouble());
        ntupleSvc()->attach("FILE1/lidj",NumN0[i]);
        histName="AD";
        histName+=i+1;
        histName+="NumE0";
        NumE0[i] = new TH1F(histName,"num of n-like with RPC",liveTimeSvc->nBins(),liveTimeSvc->startTime().AsDouble(),liveTimeSvc->endTime().AsDouble());
        ntupleSvc()->attach("FILE1/lidj",NumE0[i]);
        histName="AD";
        histName+=i+1;
        histName+="NumE0_600";
        NumE0_600[i] = new TH1F(histName,"num of n-like with RPC",liveTimeSvc->nBins(),liveTimeSvc->startTime().AsDouble(),liveTimeSvc->endTime().AsDouble());
        ntupleSvc()->attach("FILE1/lidj",NumE0_600[i]);
    }
    /*
       histName="RpcMuonRate";
       RpcMuonRate = new TH1F(histName,"Rpc muon rate",liveTimeSvc->nBins(),liveTimeSvc->startTime().AsDouble(),liveTimeSvc->endTime().AsDouble());
       histName="OWpMuonRate";
       OWpMuonRate = new TH1F(histName,"Outer wp muon rate",liveTimeSvc->nBins(),liveTimeSvc->startTime().AsDouble(),liveTimeSvc->endTime().AsDouble());
       histName="IWpMuonRate";
       IWpMuonRate = new TH1F(histName,"Inner wp muon rate",liveTimeSvc->nBins(),liveTimeSvc->startTime().AsDouble(),liveTimeSvc->endTime().AsDouble());

       ntupleSvc()->attach("FILE1/lidj",RpcMuonRate);
       ntupleSvc()->attach("FILE1/lidj",OWpMuonRate);
       ntupleSvc()->attach("FILE1/lidj",IWpMuonRate);

       for( int i=0 ; i<GlobalVar::NumADs ; i++ )
       {
       histName="AD";
       histName+=i+1;
       histName+="MuonRate";
       AdMuonRate[i] = new TH1F(histName,"Ad Muon Rate",liveTimeSvc->nBins(),liveTimeSvc->startTime().AsDouble(),liveTimeSvc->endTime().AsDouble());
       histName="AD";
       histName+=i+1;
       histName+="ShowerMuonRate";
       AdShowerMuonRate[i] = new TH1F(histName,"Shower Muon Rate",liveTimeSvc->nBins(),liveTimeSvc->startTime().AsDouble(),liveTimeSvc->endTime().AsDouble());


       histName="IbdRate";
       histName+="AD";
       histName+=i+1;
       IbdRate[i] = new TH1F(histName,"Ibd Rate",liveTimeSvc->nBins(),liveTimeSvc->startTime().AsDouble(),liveTimeSvc->endTime().AsDouble());
       histName="IbdPromptEnergy";
       histName+="AD";
       histName+=i+1;
       IbdPromptEnergy[i] = new TH1F(histName,"Ibd Prompt Energy",120,promptELow,promptEHigh);
       histName="IbdDelayedEnergy";
       histName+="AD";
       histName+=i+1;
       IbdDelayedEnergy[i] = new TH1F(histName,"Ibd Delayed Energy",120,delayedELow,delayedEHigh);
       histName="IbdDistance";
       histName+="AD";
       histName+=i+1;
       IbdDistance[i] = new TH1F(histName,"Ibd Distance",120,0.0,3.);
       histName="IbdTimeInterval";
       histName+="AD";
       histName+=i+1;
       IbdTimeInterval[i] = new TH1F(histName,"Ibd Time Interval",120,IbdTimeIntervalMin*1.e6,IbdTimeIntervalMax*1.e6);

       ntupleSvc()->attach("FILE1/lidj",AdMuonRate[i]);
       ntupleSvc()->attach("FILE1/lidj",AdShowerMuonRate[i]);

       ntupleSvc()->attach("FILE1/lidj",IbdRate[i]);
       ntupleSvc()->attach("FILE1/lidj",IbdPromptEnergy[i]);
       ntupleSvc()->attach("FILE1/lidj",IbdDelayedEnergy[i]);
       ntupleSvc()->attach("FILE1/lidj",IbdDistance[i]);
       ntupleSvc()->attach("FILE1/lidj",IbdTimeInterval[i]);

       }
     */
    if( saveMuon )
    {
        MuonTree = ntupleSvc()->bookTree("FILE2/Tree/MuonTree","MuonTree");
        MuonTree->Branch("det",&det,"det/I");
        MuonTree->Branch("trigTime",&trigTime,"trigTime/D");
        MuonTree->Branch("entry",&entry_l,"entry_l/L");
        MuonTree->Branch("fileNum",&fileNum_l,"fileNum_l/I");
        MuonTree->Branch("localEntry",&localEntry,"localEntry/L");
        MuonTree->Branch("muonTag",&muonTag,"muonTag/I");
        MuonTree->Branch("trigType",&trigType,"trigType/I");
        MuonTree->Branch("nPmt",&nPmt,"nPmt/I");
        MuonTree->Branch("trigNum",&trigNum_l,"trigNum_l/I");
        MuonTree->Branch("t2muon",t2muon,"t2muon[22]/D");
    }
    if( saveSingle )
    {
        SingleTree = ntupleSvc()->bookTree("FILE2/Tree/SingleTree","SingleTree");
        SingleTree->Branch("IsSingle",&IsSingle,"IsSingle/I");
        SingleTree->Branch("IsEle",&IsEle,"IsEle/I");
        SingleTree->Branch("IsEle_600",&IsEle_600,"IsEle_600/I");
        SingleTree->Branch("IsNeu",&IsNeu,"IsNeu/I");
        SingleTree->Branch("energy",&energy,"energy/F");
        SingleTree->Branch("x",&x,"x/F");
        SingleTree->Branch("y",&y,"y/F");
        SingleTree->Branch("z",&z,"z/F");
        SingleTree->Branch("det",&det,"det/I");
        SingleTree->Branch("trigTime",&trigTime,"trigTime/D");
        SingleTree->Branch("curLivetimeOld",&curLivetimeOld,"curLivetimeOld/D");
        SingleTree->Branch("curLivetime",&curLivetime,"curLivetime/D");
        SingleTree->Branch("curDaqtimeOld",&curDaqtimeOld,"curDaqtimeOld/D");
        SingleTree->Branch("curDaqtime",&curDaqtime,"curDaqtime/D");
        SingleTree->Branch("entry",&entry_l,"entry_l/L");
        SingleTree->Branch("fileNum",&fileNum_l,"fileNum_l/I");
        SingleTree->Branch("localEntry",&localEntry,"localEntry/L");
        SingleTree->Branch("trigNum",&trigNum_l,"trigNum_l/I");
        SingleTree->Branch("t2muon",t2muon,"t2muon[22]/D");
    }

    IbdTree = ntupleSvc()->bookTree("FILE2/Tree/IbdTree","IbdTree");
    IbdTree->Branch("isIbd",&isIbd_l,"isIbd_l/I");
    IbdTree->Branch("isLi",&isLi_l,"isLi_l/I");
    IbdTree->Branch("det",&det_l,"det_l/I");
    IbdTree->Branch("energy",energy_l,"energy_l[2]/F");
    IbdTree->Branch("x",x_l,"x_l[2]/F");
    IbdTree->Branch("y",y_l,"y_l[2]/F");
    IbdTree->Branch("z",z_l,"z_l[2]/F");
    IbdTree->Branch("fileNum",fileNum,"fileNum[2]/I");
    IbdTree->Branch("entry",entry,"entry[2]/L");
    IbdTree->Branch("trigNum",trigNum,"trigNum[2]/I");
    IbdTree->Branch("timeInterval",&timeInterval,"timeInterval/D");
    IbdTree->Branch("delayedTrigTimeSec",&delayedTrigTimeSec,"delayedTrigTimeSec/I");
    IbdTree->Branch("promptT2Muon",promptT2Muon,"promptT2Muon[22]/D");
    IbdTree->Branch("delayedT2Muon",delayedT2Muon,"delayedT2Muon[22]/D");

    //MutilTree = ntupleSvc()->bookTree("","");
    //MutilTree->Branch("",&,"");


    std::cout<<"IBDSel initializing..."<<endl;
    return true;
}

bool IBDSel::execute()
{

    PhyEvent* CurEvent= EvtBuffer->curEvt();

    //printEvt(CurEvent);
    VetoedByRpc=0;
    /*
       if( CurEvent->m_entry%200000==0 )
       {
       std::cout<<"Entry = "<<CurEvent->m_entry<<endl;
    //printEvt(CurEvent);
    }
     */
    if( !CurEvent->m_good )
    {
        return true;
    }

    //if( CurEvent->isAdMuon() )
    //{
    //ADMuonNum[0]++;
    //}
    if( CurEvent->isMuon())
    {
        //need to delete this function "updateVetoWindow",in "NtupleAna/Algorithms/src/MuonTagAlg.cc" ,it has been executed once.If you don't use the project"MuonTagAlg",you should not delete this line.
        //muonVeto_l->updateVetoWindow(CurEvent);

        if( saveMuon )
        {
            if( CurEvent->isRpcMuon() )
            {
                CalTime2Muon(CurEvent);
                //single RpcMuon ,can't use [5] [6]!!!!didn't calculate these two times in function "CalTime2Muon()",default is 10.e6
                if( time2Muon[0]>relatedTime && time2Muon[1]>relatedTime && time2Muon[2]>relatedTime && time2Muon[3]>relatedTime && time2Muon[5]>relatedTime && time2Muon[6]>relatedTime)
                {
                    det=CurEvent->m_det;
                    trigTime=CurEvent->m_trigTime;
                    entry_l=CurEvent->m_entry;
                    fileNum_l=CurEvent->m_fileNum;
                    localEntry=CurEvent->m_localEntry;
                    muonTag=CurEvent->m_muonTag;
                    trigType=CurEvent->m_trigType;
                    nPmt=CurEvent->m_nPmt;
                    trigNum_l=CurEvent->m_trigNum;
                    for(int i=0;i<22;i++) t2muon[i]=time2Muon[i];
                    MuonTree->Fill();
                }
            }
        }
        if( CurEvent->isWpMuon() && CurEvent->m_det==5 )
        {
            //lastIwpMuonTrigtime=CurEvent->m_trigTime;
            lastIwpMuonTrigtime.SetSec(CurEvent->m_trigTime.GetSec());
            lastIwpMuonTrigtime.SetNanoSec(CurEvent->m_trigTime.GetNanoSec());
        } else if(CurEvent->isWpMuon() && CurEvent->m_det==6)
        {
            //lastOwpMuonTrigtime=CurEvent->m_trigTime;
            lastOwpMuonTrigtime.SetSec(CurEvent->m_trigTime.GetSec());
            lastOwpMuonTrigtime.SetNanoSec(CurEvent->m_trigTime.GetNanoSec());
        }else if(CurEvent->isRpcMuon())
        {
            //lastRpcMuonTrigtime=CurEvent->m_trigTime;
            lastRpcMuonTrigtime.SetSec(CurEvent->m_trigTime.GetSec());
            lastRpcMuonTrigtime.SetNanoSec(CurEvent->m_trigTime.GetNanoSec());
        }else
        {
            //
        }
        if( !(CurEvent->isAD()) )
        {
            return true;
        }
        if( CurEvent->isAdMuon() )
        {
            lastAdMuonTrigtime[CurEvent->m_det-1].SetSec(CurEvent->m_trigTime.GetSec());
            lastAdMuonTrigtime[CurEvent->m_det-1].SetNanoSec(CurEvent->m_trigTime.GetNanoSec());
        }else if(CurEvent->isAdMuonNew())
        {
            lastNewAdMuonTrigtime[CurEvent->m_det-1].SetSec(CurEvent->m_trigTime.GetSec());
            lastNewAdMuonTrigtime[CurEvent->m_det-1].SetNanoSec(CurEvent->m_trigTime.GetNanoSec());
        }else if(CurEvent->isShowerMuon())
        {
            lastShowerMuonTrigtime[CurEvent->m_det-1].SetSec(CurEvent->m_trigTime.GetSec());
            lastShowerMuonTrigtime[CurEvent->m_det-1].SetNanoSec(CurEvent->m_trigTime.GetNanoSec());
        }else if(CurEvent->isShowerMuonNew())
        {
            lastNewShowerMuonTrigtime[CurEvent->m_det-1].SetSec(CurEvent->m_trigTime.GetSec());
            lastNewShowerMuonTrigtime[CurEvent->m_det-1].SetNanoSec(CurEvent->m_trigTime.GetNanoSec());
        }else
        {
            // 
        }
        double AdMuonEnergy=0.;//CurEvent->energy();
        AdMuonEnergy=CurEvent->energy();

        if( AdMuonEnergy>=20. && AdMuonEnergy<500. )
        {
            if( CurEvent->isShowerMuon() || CurEvent->isAdMuon())
            {
                lastshowermuonTrigtime0Vec[0][CurEvent->m_det-1].push_back(CurEvent->m_trigTime);
            }
            lastshowermuonTrigtimeVec[0][CurEvent->m_det-1].push_back(CurEvent->m_trigTime);
            //ADMuonNum[1]++;

        } else if( AdMuonEnergy>=500. && AdMuonEnergy<1500. )
        {
            if( CurEvent->isShowerMuon() || CurEvent->isAdMuon() )
            {
                lastshowermuonTrigtime0Vec[1][CurEvent->m_det-1].push_back(CurEvent->m_trigTime);
            }
            lastshowermuonTrigtimeVec[1][CurEvent->m_det-1].push_back(CurEvent->m_trigTime);
            //ADMuonNum[2]++;
        } else if( AdMuonEnergy>=1500. && AdMuonEnergy<2500. )
        {
            if( CurEvent->isShowerMuon() || CurEvent->isAdMuon() )
            {
                lastshowermuonTrigtime0Vec[2][CurEvent->m_det-1].push_back(CurEvent->m_trigTime);
            }
            lastshowermuonTrigtimeVec[2][CurEvent->m_det-1].push_back(CurEvent->m_trigTime);
            //ADMuonNum[3]++;
        } else if( AdMuonEnergy>=2500. && AdMuonEnergy<3500. )
        {
            if( CurEvent->isShowerMuon() || CurEvent->isAdMuon())
            {
                lastshowermuonTrigtime0Vec[3][CurEvent->m_det-1].push_back(CurEvent->m_trigTime);
            }
            lastshowermuonTrigtimeVec[3][CurEvent->m_det-1].push_back(CurEvent->m_trigTime);
            //ADMuonNum[4]++;
        } else if( AdMuonEnergy>=3500. && AdMuonEnergy<4500. )
        {
            if( CurEvent->isShowerMuon() || CurEvent->isAdMuon())
            {
                lastshowermuonTrigtime0Vec[4][CurEvent->m_det-1].push_back(CurEvent->m_trigTime);
            }
            lastshowermuonTrigtimeVec[4][CurEvent->m_det-1].push_back(CurEvent->m_trigTime);
            //ADMuonNum[5]++;

        }else if( AdMuonEnergy>=4500. )
        {
            if( CurEvent->isShowerMuon() || CurEvent->isAdMuon())
            {
                lastshowermuonTrigtime0Vec[5][CurEvent->m_det-1].push_back(CurEvent->m_trigTime);
            }
            lastshowermuonTrigtimeVec[5][CurEvent->m_det-1].push_back(CurEvent->m_trigTime);

            //ADMuonNum[6]++;

        }else
        {
            //continue;
        }
        if( !(CurEvent->isAdMuonNew()||CurEvent->isShowerMuonNew()))
        {
            return true;
        } 
    }

    if( !CurEvent->isAD())
    {
        return true;
    }


    bool isNeulike=false;
    if( CurEvent->energy()>1.5)
    {
        isNeulike=true;
    } 
    //else if( CurEvent->energy()>1.8 && CurEvent->energy()<2.8 )
    //{
    //if( sqrt(CurEvent->m_x*CurEvent->m_x+CurEvent->m_y*CurEvent->m_y)>1.5 )
    //{
    //isNeulike=true;
    //}
    //}else
    //{
    ////
    //}
    if(isNeulike) 
    {

        //for( int i=0 ; i<6 ; i++ )
        for( int i=0 ; i<6 ; i++ )
        {
            if( !lastshowermuonTrigtimeVec[i][CurEvent->m_det-1].empty() )
            {
                vector<TTimeStamp>::iterator it=lastshowermuonTrigtimeVec[i][CurEvent->m_det-1].end();
                it--;
                for(; it>=lastshowermuonTrigtimeVec[i][CurEvent->m_det-1].begin();it--) 
                {
                    if( (CurEvent->m_trigTime-*it)>2.e-5 &&(CurEvent->m_trigTime-*it)<2.e-4)
                    {
                        lastshowermuonTrigtime[i][CurEvent->m_det-1].SetSec((*it).GetSec());
                        lastshowermuonTrigtime[i][CurEvent->m_det-1].SetNanoSec((*it).GetNanoSec());
                        showermuonNum[i]->Fill(lastshowermuonTrigtime[i][CurEvent->m_det-1]);
                        it=lastshowermuonTrigtimeVec[i][CurEvent->m_det-1].erase(it);
                    } else if( (CurEvent->m_trigTime-*it)>=2.e-4 )
                    {
                        it++;
                        lastshowermuonTrigtimeVec[i][CurEvent->m_det-1].erase(lastshowermuonTrigtimeVec[i][CurEvent->m_det-1].begin(),it); 
                        break;
                    } else
                    {
                        //
                    }
                }
            }
            if( !lastshowermuonTrigtime0Vec[i][CurEvent->m_det-1].empty() )
            {
                vector<TTimeStamp>::iterator it=lastshowermuonTrigtime0Vec[i][CurEvent->m_det-1].end();
                it--;
                for(; it>=lastshowermuonTrigtime0Vec[i][CurEvent->m_det-1].begin();it--) 
                {
                    if( (CurEvent->m_trigTime-*it)>1.e-5 &&(CurEvent->m_trigTime-*it)<2.e-4)
                    {
                        //lastshowermuonTrigtime0[i][CurEvent->m_det-1]=*it;
                        //showermuonNum0[i]->Fill(CurEvent->m_trigTime.AsDouble());
                        lastshowermuonTrigtime0[i][CurEvent->m_det-1].SetSec((*it).GetSec());
                        lastshowermuonTrigtime0[i][CurEvent->m_det-1].SetNanoSec((*it).GetNanoSec());
                        showermuonNum0[i]->Fill(lastshowermuonTrigtime0[i][CurEvent->m_det-1]);
                        it=lastshowermuonTrigtime0Vec[i][CurEvent->m_det-1].erase(it);
                    } else if( (CurEvent->m_trigTime-*it)>=2.e-4 )
                    {
                        it++;
                        lastshowermuonTrigtime0Vec[i][CurEvent->m_det-1].erase(lastshowermuonTrigtime0Vec[i][CurEvent->m_det-1].begin(),it); 
                        break;
                    } else
                    {
                        //
                    }
                }
            }
        }

    }


    int vetotag=muonVeto_l->veto(CurEvent);

    if( vetotag==1 )
    {
        return true;
    } else if(vetotag==2)
    {
        VetoedByRpc=1;
    }else//vetotag==0
    {
        VetoedByRpc=0;
    }

    if( !(CurEvent->energy()>1.5) )
    {
        return true;
    }
    if( VetoedByRpc==0 )
    {
        NumOfAll[CurEvent->m_det-1]->Fill(CurEvent->m_trigTime.AsDouble());
        NumOfAll0[CurEvent->m_det-1]->Fill(CurEvent->m_trigTime.AsDouble());
    }
    if( VetoedByRpc==1 )
    {
        NumOfAll0[CurEvent->m_det-1]->Fill(CurEvent->m_trigTime.AsDouble());
    }

    dump(CurEvent->m_det,0,CurEvent);

    AdEvtBuf[CurEvent->m_det-1].push_back(CurEvent->GrabInstance());
    VetoedBuf[CurEvent->m_det-1].push_back(VetoedByRpc);
    CalTime2Muon(CurEvent);
    /*
       if( CurEvent->m_fileNum==19 && CurEvent->m_localEntry ==452431 )
       {
       std::cout<<"find it! "<<endl;
       for( int i=0 ; i<22 ; i++ )
       {
       std::cout<<"time2MuonVec["<<i <<"]  : "<<time2MuonVec[i]<<endl;
       }

       }
     */
    time2MuonBuf[CurEvent->m_det-1].push_back(time2MuonVec);


    return true;

}

bool IBDSel::finalize()
{
    PhyEvent* CurEvent=0;
    for( int i=1 ; i<5 ; i++ )
    {
        dump(i,1,CurEvent);
    }

    std::cout<<"IBDSel finalize..."<<endl;

    return true;
}

bool IBDSel::FillIbd(vector<PhyEvent*> EvtGroup,int tag)
{
    if( time2MuonBuf[EvtGroup[0]->m_det-1].size()!=2 )
    {
        std::cout<<"time2MuonBuf size is wrong in FillIbd(),size is "<<time2MuonBuf[EvtGroup[0]->m_det-1].size()<<endl;
    }

    double disInterval=sqrt((EvtGroup[0]->m_x-EvtGroup[1]->m_x)*(EvtGroup[0]->m_x-EvtGroup[1]->m_x)+(EvtGroup[0]->m_y-EvtGroup[1]->m_y)*(EvtGroup[0]->m_y-EvtGroup[1]->m_y)+(EvtGroup[0]->m_z-EvtGroup[1]->m_z)*(EvtGroup[0]->m_z-EvtGroup[1]->m_z));
    double _IbdTimeInterval=EvtGroup[1]->m_trigTime-EvtGroup[0]->m_trigTime;
    double _promptE=EvtGroup[0]->energy();
    double _delayedE=EvtGroup[1]->energy();

    if( !((_IbdTimeInterval>=IbdTimeIntervalMin)&&(_IbdTimeInterval<=IbdTimeIntervalMax)&&disInterval<IbdDisInterval) )
    {
        return true;
    }

    if( !((_promptE>=promptELow) && (_promptE<=promptEHigh4Fn)) )
    {
        return true;
    }

    for( int i=0 ; i<22 ; i++ )
    {
        delayedT2Muon[i]=time2MuonBuf[EvtGroup[1]->m_det-1][1][i];	
        promptT2Muon[i]=time2MuonBuf[EvtGroup[0]->m_det-1][0][i];	
    }

    IsLi=7;
    if( (_promptE<promptEHigh)&&(_promptE>3.5)&&(_delayedE>=delayedEnergyLow4Li)&&(_delayedE<=delayedEnergyHigh4Li))
    {
        if( tag==2 )
        {
            IsLi=2;
        } else if(tag==1)
        {
            IsLi=1;
        }else
        {
            IsLi=0;
        }
        if( IsLi==0||IsLi==1 )
        {
            for( int i=0 ; i<6 ; i++ )
            {
                if( time2MuonBuf[EvtGroup[0]->m_det-1][0][i+16]<100. )
                {
                    time2lastshowermuon0[i]->Fill(time2MuonBuf[EvtGroup[0]->m_det-1][0][i+16]);
                }
            }
        }
        if( IsLi==2||IsLi==1 )
        {
            for( int i=0 ; i<6 ; i++ )
            {
                if( time2MuonBuf[EvtGroup[0]->m_det-1][0][i+10]<100. )
                {
                    time2lastshowermuon[i]->Fill(time2MuonBuf[EvtGroup[0]->m_det-1][0][i+10]);
                }
            }
        }
    }

    if( !((_delayedE>=delayedELow)&&(_delayedE<=delayedEHigh)&&(promptT2Muon[1]>=PromptTrigTime2AdShowerMuon) ))
    {
        if( IsLi==7 )
        {
            return true;
        }
        IsIbd=7;
    }else
    {
        if( tag==2 )
        {
            IsIbd=2;
        } else if(tag==1)
        {
            IsIbd=1;
        }else
        {
            IsIbd=0;
        }
    }

    if( EvtGroup[0]->energy()>promptEHigh )
    {
        IsIbd+=3;
    }

    //fill IBD tree AND histograms
    isIbd_l = IsIbd;
    isLi_l=IsLi;
    det_l = EvtGroup[1]->m_det;
    timeInterval=(EvtGroup[1]->m_trigTime-EvtGroup[0]->m_trigTime)*1.e6;
    delayedTrigTimeSec=EvtGroup[1]->m_trigTime.GetSec();
    for( int i=0 ; i<(int)EvtGroup.size() ; i++ )
    {
        energy_l[i]=EvtGroup[i]->energy();
        x_l[i]=EvtGroup[i]->m_x;
        y_l[i]=EvtGroup[i]->m_y;
        z_l[i]=EvtGroup[i]->m_z;
        fileNum[i]=EvtGroup[i]->m_fileNum;
        entry[i]=EvtGroup[i]->m_localEntry;
        trigNum[i]=EvtGroup[i]->m_trigNum;
    }
    if( IsIbd==1 )
    {
        std::cout<<"_IbdTimeInterval "<<_IbdTimeInterval<<endl;
        std::cout<<"_promptE  : "<<_promptE<<endl;
        std::cout<<"_delayedE  : "<<_delayedE<<endl;
        std::cout<<"disInterval  : "<<disInterval<<endl;
        std::cout<<"PromptTrigTime2AdShowerMuon  : "<<promptT2Muon[1]<<endl;
        std::cout<<"PromptTrig2AdMuon  : "<<promptT2Muon[0]<<endl;
        std::cout<<"PromptTrig2IwsMuon  : "<<promptT2Muon[2]<<endl;
        std::cout<<"PromptTrig2OwsMuon  : "<<promptT2Muon[3]<<endl;
    }

    IbdTree->Fill();

    return true;
}


bool IBDSel::FillSingle(PhyEvent* Evt,int tag)
{
    if( time2MuonBuf[Evt->m_det-1].size()!=1 )
    {
        std::cout<<"wrong !! time2MuonBuf["<<Evt->m_det-1<<"].size()="<<time2MuonBuf[Evt->m_det-1].size() <<endl;
    }
    energy=Evt->energy();
    IsSingle=tag;
    if( energy>=delayedEHigh)
    {
        return true;
    }
    if( IsSingle==0 )
    {
        NumE0[Evt->m_det-1]->Fill(Evt->m_trigTime.AsDouble());
    } else if(IsSingle==1)
    {
        NumE[Evt->m_det-1]->Fill(Evt->m_trigTime.AsDouble());
        NumE0[Evt->m_det-1]->Fill(Evt->m_trigTime.AsDouble());
    }else
    {
        NumE[Evt->m_det-1]->Fill(Evt->m_trigTime.AsDouble());
    }
    IsEle_600=0;
    IsNeu=0; 
    //CalTime2Muon(Evt);
    //num_total++;

    if( ((time2MuonBuf[Evt->m_det-1][0][0]>=DelayedTrigTime2AdMuon)&&(time2MuonBuf[Evt->m_det-1][0][2]>=DelayedTrigTime2IWpMuon)&&(time2MuonBuf[Evt->m_det-1][0][1]>=DelayedTrigTime2AdShowerMuon)&&(time2MuonBuf[Evt->m_det-1][0][3]>=DelayedTrigTime2OWpMuon)) )
    {
        //num_temp++;
        IsEle_600=1;
        if( tag==0 )
        {
            /*
               if( time2Muon[4]<DelayedTrigTime2RpcMuon )
               {
               return true;
               }
             */
        } else if( tag==1 )
        {
            if( time2Muon[4]<DelayedTrigTime2RpcMuon || time2Muon[8]<DelayedTrigTime2AdMuon || time2Muon[9]<DelayedTrigTime2AdShowerMuon)
            {
                IsSingle=0;
            } else
            {
                IsSingle=1;
            }
        }else
        {
            if( time2Muon[4]<DelayedTrigTime2RpcMuon || time2Muon[8]<DelayedTrigTime2AdMuon || time2Muon[9]<DelayedTrigTime2AdShowerMuon)
            {
                //num_temp_2++;
                return true;
            }
        }

        if( IsSingle==0 )
        {
            if( energy>delayedELow)
            {
                NumN0[Evt->m_det-1]->Fill(Evt->m_trigTime.AsDouble());
                IsNeu=1; 
            }
            NumE0_600[Evt->m_det-1]->Fill(Evt->m_trigTime.AsDouble());
        } else if(IsSingle==2)
        {
            if( energy>delayedELow)
            {
                NumN[Evt->m_det-1]->Fill(Evt->m_trigTime.AsDouble());
                IsNeu=1; 
            }
            NumE_600[Evt->m_det-1]->Fill(Evt->m_trigTime.AsDouble());
        }else
        {
            //num_temp_N0++;
            if( energy>delayedELow)
            {
                NumN0[Evt->m_det-1]->Fill(Evt->m_trigTime.AsDouble());
                NumN[Evt->m_det-1]->Fill(Evt->m_trigTime.AsDouble());
                IsNeu=1; 
            }
            NumE0_600[Evt->m_det-1]->Fill(Evt->m_trigTime.AsDouble());
            NumE_600[Evt->m_det-1]->Fill(Evt->m_trigTime.AsDouble());
        }
    }

    if( saveSingle )
    {
        IsEle=1;
        det=Evt->m_det;
        if( IsSingle==0 )
        {
            //curLivetimeOld=liveTimeSvc->liveTimeOld(det)+Evt->m_trigTime-muonVeto_l->m_vetoWindowPostOld[det-1].AsDouble();
            curLivetimeOld=liveTimeSvc->liveTimeOld(det);
            curDaqtimeOld=liveTimeSvc->daqTime();
        } else if(IsSingle==2)
        {
            curLivetime=liveTimeSvc->liveTime(det);
            curDaqtime=liveTimeSvc->daqTime();
        }else
        {
            curLivetimeOld=liveTimeSvc->liveTimeOld(det);
            curLivetime=liveTimeSvc->liveTime(det);
            curDaqtimeOld=liveTimeSvc->daqTime();
            curDaqtime=liveTimeSvc->daqTime();
        }
        x=Evt->m_x;
        y=Evt->m_y;
        z=Evt->m_z;
        det=Evt->m_det;
        trigTime=Evt->m_trigTime;
        entry_l=Evt->m_entry;
        fileNum_l=Evt->m_fileNum;
        localEntry=Evt->m_localEntry;
        trigNum_l=Evt->m_trigNum;
        //CalTime2Muon(Evt);
        for(int i=0;i<22;i++) t2muon[i]=time2Muon[i];
        SingleTree->Fill();
        curLivetime=0.;
        curDaqtime=0.;
        curDaqtimeOld=0.;
        curLivetimeOld=0.;
    }
    return true;
}

/*
   bool IBDSel::FillMulti(vector<PhyEvent*> EvtGroup)
   {
   return true;
   }
 */

bool IBDSel::CalTime2Muon(PhyEvent* event)
{
    //can not be used in dump().must only be used for current event,because these last*muonTrigtimes are latest muons!!!
    time2MuonVec.clear();
    for(int i=0;i<22;i++) time2Muon[i]=10.e6;
    //pre ADMuon
    if( lastAdMuonTrigtime[event->m_det-1].GetSec()!=0. )
    {
        //time2Muon[0]=event->m_trigTime.AsDouble()-lastAdMuonTrigtime[event->m_det-1];
        time2Muon[0]=event->m_trigTime-lastAdMuonTrigtime[event->m_det-1];
    }
    //pre ADshower muon
    if( lastShowerMuonTrigtime[event->m_det-1].GetSec()!=0. )
    {
        time2Muon[1]=event->m_trigTime - lastShowerMuonTrigtime[event->m_det-1];
    }
    //pre ADMuonNew
    if( lastNewAdMuonTrigtime[event->m_det-1].GetSec()!=0. )
    {
        time2Muon[8]=event->m_trigTime - lastNewAdMuonTrigtime[event->m_det-1];
    }
    //pre ADshower muonNew
    if( lastNewShowerMuonTrigtime[event->m_det-1].GetSec()!=0.  )
    {
        time2Muon[9]=event->m_trigTime - lastNewShowerMuonTrigtime[event->m_det-1];
    }
    //for He8/Li9
    for( int i=0 ; i<6 ; i++ )
    {
        if( lastshowermuonTrigtime[i][event->m_det-1].GetSec()!=0. )
        {
            time2Muon[i+10]=event->m_trigTime - lastshowermuonTrigtime[i][event->m_det-1];
        }
        if( lastshowermuonTrigtime0[i][event->m_det-1].GetSec()!=0. )
        {
            time2Muon[i+16]=event->m_trigTime - lastshowermuonTrigtime0[i][event->m_det-1];
        }
    }


    //pre Iwp muon
    /*
       PhyEvent* preIWMuon=muonVeto_l->preMuon(event,5);
       if( preIWMuon )
       {
       time2Muon[2]=event->m_trigTime - preIWMuon->m_trigTime;
       } 
     */
    if( lastIwpMuonTrigtime.GetSec()!=0. )
    {
        time2Muon[2]=event->m_trigTime - lastIwpMuonTrigtime;
    }
    //pre Owp muon
    /*
       PhyEvent* preOWMuon=muonVeto_l->preMuon(event,6);
       if( preOWMuon )
       {
       time2Muon[3]=event->m_trigTime - preOWMuon->m_trigTime;
       } 
     */
    if( lastOwpMuonTrigtime.GetSec()!=0. )
    {
        time2Muon[3]=event->m_trigTime - lastOwpMuonTrigtime;
    }
    //pre Rpc Muon
    /*
       PhyEvent* preRpcMuon=muonVeto_l->preMuon(event,7);
       if( preRpcMuon )
       {
       time2Muon[4]=event->m_trigTime - preRpcMuon->m_trigTime;
       }
     */
    if( lastRpcMuonTrigtime.GetSec()!=0. )
    {
        time2Muon[4]=event->m_trigTime - lastRpcMuonTrigtime;
    }
    /*
    //next Iwp muon
    PhyEvent* nextIWMuon=muonVeto_l->nextMuon(event,5);
    if( nextIWMuon )
    {
    time2Muon[5]=nextIWMuon->m_trigTime - event->m_trigTime;
    } 

    //next Owp muon
    PhyEvent* nextOWMuon=muonVeto_l->nextMuon(event,6);
    if( nextOWMuon )
    {
    time2Muon[6]=nextOWMuon->m_trigTime - event->m_trigTime;
    } 

    //next Rpc Muon
    PhyEvent* nextRpcMuon=muonVeto_l->nextMuon(event,7);
    if( nextRpcMuon )
    {
    time2Muon[7]=nextRpcMuon->m_trigTime - event->m_trigTime;
    }
     */
    for( int i=0 ; i<22 ; i++ )
    {
        time2MuonVec.push_back(time2Muon[i]);
    }
    return true;
}


bool IBDSel::printEvt(PhyEvent* CurEvent)
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

void IBDSel::dump(int i_det,bool IsFinal,PhyEvent* CurEvent)
{
    if( !AdEvtBuf[i_det-1].empty() )
    {
        if( !(AdEvtBuf[i_det-1].size()==VetoedBuf[i_det-1].size()) )
        {
            std::cout<<"AdEvtBuf's size != VetoedBuf's size , sth is wrong . "<<endl;
        }
        PhyEvent* LastEvent=AdEvtBuf[i_det-1][AdEvtBuf[i_det-1].size()-1];
        if( IsFinal || (CurEvent->m_trigTime-LastEvent->m_trigTime)>Time2LastBufEvent )
        {
            /*
               switch ( (int)AdEvtBuf[i_det-1].size() )
               {
               case 1 :
               FillSingle(AdEvtBuf[CurEvent->m_det][0]);
               break;
               case 2 :
            //std::cout<<"case 2"<<endl;
            FillIbd(AdEvtBuf[i_det-1]);
            break;
            default :
            //FillMulti(AdEvtBuf[CurEvent->m_det]);
            break;
            }
             */
            if( AdEvtBuf[i_det-1].size()==1 )
            {
                if( VetoedBuf[i_det-1][0]==0 )
                {
                    FillSingle(AdEvtBuf[i_det-1][0],1);
                }else
                {
                    FillSingle(AdEvtBuf[i_det-1][0],0);
                }
            } else if(AdEvtBuf[i_det-1].size()==2)
            {
                if( VetoedBuf[i_det-1][0]==0 && VetoedBuf[i_det-1][1]==0)
                {
                    FillIbd(AdEvtBuf[i_det-1],1);
                } else if(VetoedBuf[i_det-1][0]==1 && VetoedBuf[i_det-1][1]==1)
                {
                    FillIbd(AdEvtBuf[i_det-1],0);	
                }else
                {
                    if( VetoedBuf[i_det-1][0]==0 )
                    {
                        FillSingle(AdEvtBuf[i_det-1][0],2);
                        FillIbd(AdEvtBuf[i_det-1],0);
                    }else
                    {
                        FillSingle(AdEvtBuf[i_det-1][1],2);
                        FillIbd(AdEvtBuf[i_det-1],0);
                    }
                }
            }else
            {
                int RealNum=0;
                vector<PhyEvent*> TempAdEvtBuf[4];

                for( int i=0 ; i<(int)AdEvtBuf[i_det-1].size() ; i++ )
                {
                    if( VetoedBuf[i_det-1][i]==0 )
                    {
                        RealNum++;
                        TempAdEvtBuf[i_det-1].push_back(AdEvtBuf[i_det-1][i]->GrabInstance());
                    }else
                    {
                        time2MuonBuf[i_det-1][i].clear();
                    }
                }

                for( vector< vector<double> >::iterator it=time2MuonBuf[i_det-1].begin(); it<time2MuonBuf[i_det-1].end();it++ )
                {
                    if( (*it).size()==0 )
                    {
                        it=time2MuonBuf[i_det-1].erase(it);
                        --it;
                    }
                }

                if( RealNum==1 )
                {
                    FillSingle(TempAdEvtBuf[i_det-1][0],2);
                } else if(RealNum==2)
                {
                    FillIbd(TempAdEvtBuf[i_det-1],2);
                }else
                {
                    //FillMulti(AdEvtBuf[i_det-1]);
                }
                for( int i=0 ; i<(int)TempAdEvtBuf[i_det-1].size();i++)
                {
                    TempAdEvtBuf[i_det-1][i]->ReleaseInstance();
                }
                TempAdEvtBuf[i_det-1].clear(); 
            }
            for( int i=0 ; i<(int)AdEvtBuf[i_det-1].size();i++)
            {
                AdEvtBuf[i_det-1][i]->ReleaseInstance();
            }

            AdEvtBuf[i_det-1].clear(); 
            VetoedBuf[i_det-1].clear(); 
            time2MuonBuf[i_det-1].clear();
        }
    } 

}
