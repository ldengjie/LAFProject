#include "FindEvent.h"
#include "LafKernel/AlgFactory.h"

DECLARE_ALGORITHM(FindEvent);

FindEvent::FindEvent(const std::string& name)
: AlgBase(name)
{
    OptionParser::setOption(name,"PromptEnergyLow",promptELow);
    OptionParser::setOption(name,"PromptEnergyHigh",promptEHigh);
    OptionParser::setOption(name,"DelayedEnergyLow",delayedELow);
    OptionParser::setOption(name,"DelayedEnergyHigh",delayedEHigh);
    OptionParser::setOption(name,"IbdTimeIntervalMin",IbdTimeIntervalMin);
    OptionParser::setOption(name,"IbdTimeIntervalMax",IbdTimeIntervalMax);
	OptionParser::setOption(name,"AdMuonELow",AdMuonELow);
	OptionParser::setOption(name,"AdMuonEHigh",AdMuonEHigh);
    OptionParser::setOption(name,"DelayedTrigTime2AdMuon",DelayedTrigTime2AdMuon);
	OptionParser::setOption(name,"DelayedTrigTime2IWpMuon",DelayedTrigTime2IWpMuon);
	OptionParser::setOption(name,"DelayedTrigTime2OWpMuon",DelayedTrigTime2OWpMuon);
	OptionParser::setOption(name,"DelayedTrigTime2AdShowerMuon",DelayedTrigTime2AdShowerMuon);
    OptionParser::setOption(name,"Time2LastBufEvent",Time2LastBufEvent);
    OptionParser::setOption(name,"PromptFileNum",PromptFileNum);
    OptionParser::setOption(name,"PromptLocalEntry",PromptLocalEntry);
    OptionParser::setOption(name,"PromptTrigNum",PromptTrigNum);
	for( int i=0 ; i<GlobalVar::NumADs ; i++ )
	{
		AdEvtBuf[i].clear();
	}
	AdBuf.clear();
//zhangfh:  ['22492', '0', '0', '1', '73234', '73235', '961850', '961851', '3.01', '6.34', '8.25']
//zhangfh:  ['21221', '22', '21', '2', '48606', '48609', '3288423', '3288424', '97.5', '8.26', '67.9']
//PromptFileNum=100;
//PromptLocalEntry=683069;
    //PromptTrigNum=23194126;
	DelayedFileNum=PromptFileNum;
	DelayedLocalEntry=PromptLocalEntry+1;
	DelayedTrigNum=PromptTrigNum+1;

}

bool FindEvent::initialize()
{
	EvtBuffer = dynamic_cast<PhyEventBuf*>(service("Cycler"));
	muonVeto_l = MuonVeto::instance();
	std::cout<<"FindEvent initializing..."<<endl;
	return true;
}

bool FindEvent::execute()
{
	
	PhyEvent* CurEvent= EvtBuffer->curEvt();
    if(CurEvent->m_fileNum<PromptFileNum || CurEvent->m_localEntry<PromptLocalEntry-100 )
    {
        return true;
    }
    if( CurEvent->m_localEntry>DelayedLocalEntry+100 )
    {
        return false;
    }
    std::cout<<"m_localEntry  : "<<CurEvent->m_localEntry<<endl;
	
    if( CurEvent->m_localEntry==PromptLocalEntry || CurEvent->m_localEntry==DelayedLocalEntry )
    {
        printEvt(CurEvent);
    }
	if( CurEvent->m_entry%200000==0)
	{
		std::cout<<"Entry = "<<CurEvent->m_entry<<endl;
	}
	
    if( !CurEvent->m_good )
	{
	    return true;
	}

	if( CurEvent->isMuon())
    {
        muonVeto_l->updateVetoWindow(CurEvent);
	    return true;
    }

	if( !CurEvent->isAD())
	{
	    return true;
	}
	
	if( muonVeto_l->veto(CurEvent) )
	{
	    return true;
	}
	if(!(CurEvent->energy()>0.7) )
	{
		return true;
	}
    std::cout<<"begin function. "<<endl;

	PreEvtInfo(CurEvent);
	NextEvtInfo(CurEvent);
	FindRunInfo(CurEvent);
	IsIbd(CurEvent);

	return true;

}

bool FindEvent::PreEvtInfo(PhyEvent* CurEvent)
{
	if( CurEvent->m_fileNum==PromptFileNum && CurEvent->m_localEntry==PromptLocalEntry && CurEvent->m_trigNum==PromptTrigNum )
	{
		std::cout<<""<<endl;
		PhyEvent* preEvt=0;
		DataBuffer<PhyEvent>::Iterator preit=EvtBuffer->find(CurEvent);
		if(preit==EvtBuffer->begin())
		{
			std::cout<<"preit==EvtBuffer->begin()"<<endl;
		}else
		{
			while(preit>EvtBuffer->begin())
			{
				preit--;
				if((*preit)->m_good&&(*preit)->isAD()&&((*preit)->m_det==CurEvent->m_det)&&((*preit)->energy()>0.7)&&(!(muonVeto_l->veto(CurEvent))))
				{
					std::cout<<"find pre event! "<<endl;
					preEvt=*preit;
					break;
				}
			}
		}
		if( preEvt )
		{
			printEvt(preEvt);
			std::cout<<"CurEvent->m_trigTime-preEvt->m_trigTime : "<<CurEvent->m_trigTime-preEvt->m_trigTime<<endl;
			Time2PreMuon(preEvt);	
		} else
		{
			std::cout<<"Can nou find pre event."<<endl;
		}

	}
	return true;
}

bool FindEvent::NextEvtInfo(PhyEvent* CurEvent)
{

	if( CurEvent->m_fileNum==DelayedFileNum && CurEvent->m_localEntry==DelayedLocalEntry && CurEvent->m_trigNum==DelayedTrigNum )
	{
		std::cout<<""<<endl;
		PhyEvent* nextEvt=0;
		DataBuffer<PhyEvent>::Iterator nextit=EvtBuffer->find(CurEvent);
		if(nextit==EvtBuffer->end())
		{
			std::cout<<"nextit==EvtBuffer->end()"<<endl;
		}else
		{
			nextit++;
			while(nextit<EvtBuffer->end())
			{
				if((*nextit)->m_good&&(*nextit)->isAD()&&((*nextit)->m_det==CurEvent->m_det)&&((*nextit)->energy()>0.7)&&(!(muonVeto_l->veto(CurEvent))))
				{
					std::cout<<"find next event! "<<endl;
					nextEvt=*nextit;
					break;
				}
				nextit++;
			}
		}
		if( nextEvt )
		{
			printEvt(nextEvt);
			std::cout<<"nextEvt->m_trigTime-CurEvent->m_trigTime : "<<nextEvt->m_trigTime-CurEvent->m_trigTime<<endl;
			Time2PreMuon(nextEvt);	
		} else
		{
			std::cout<<"Can nou find next event."<<endl;
		}

	}
	return true;

}

bool FindEvent::printEvt(PhyEvent* CurEvent)
{
		std::cout<< "------entry=" << CurEvent->m_entry<<endl;
		std::cout<< "fileNum=" << CurEvent->m_fileNum<<endl;
		std::cout<< "local entry=" << CurEvent->m_localEntry<<endl;
		std::cout<< "trigtime=" << CurEvent->m_trigTime<<endl;
		std::cout<< "trigType=" << CurEvent->m_trigType<<endl;
		std::cout<< "det=" << CurEvent->m_det<<endl;
		std::cout<< "energy=" << CurEvent->m_energy<<endl;
		std::cout<< "rawEvis=" << CurEvent->m_rawEvis<<endl;
		std::cout<< "energy()=" << CurEvent->energy()<<endl;
		std::cout<< "nPmt=" << CurEvent->m_nPmt<<endl;
		std::cout<< "flasherTag=" << CurEvent->m_flasherTag<<endl; 
		std::cout<< "forceTrigTag=" << CurEvent->m_forceTrigTag<<endl;
		std::cout<< "crossTrigTag=" << CurEvent->m_crossTrigTag<<endl;
		std::cout<< "rpcNoiseTag=" << CurEvent->m_rpcNoiseTag<<endl;
		std::cout<< "adLowEnergyTag=" << CurEvent->m_adLowEnergyTag<<endl;
		std::cout<< "goodevent=" << CurEvent->m_good<<endl;
		std::cout<< "muontag=" << CurEvent->m_muonTag<<endl;
		return true;
}

bool FindEvent::Time2PreMuon(PhyEvent* CurEvent)
{
	//AD Muon
	PhyEvent* preAdMu=muonVeto_l->preAdMuon(CurEvent,AdMuonELow,AdMuonEHigh);
	if( preAdMu )
	{
		std::cout<<"[CurEvent->m_trigTime - preAdMu->m_trigTime] : "<<CurEvent->m_trigTime - preAdMu->m_trigTime <<endl;
	} else
	{
		std::cout<<"==> Can't find preAdMu "<<endl;
	}
	//ADshower muon
	PhyEvent* preAdShowerMuon=muonVeto_l->preShowerMuon(CurEvent);
	if( preAdShowerMuon )
	{
		std::cout<<"[CurEvent->m_trigTime - preAdShowerMuon->m_trigTime] : "<<CurEvent->m_trigTime - preAdShowerMuon->m_trigTime <<endl;
	} else
	{
		std::cout<<"==> Can't find preAdShowerMuon "<<endl;
	}
	//Iwp muon
	PhyEvent* preIWMuon=muonVeto_l->preMuon(CurEvent,5);
	if( preIWMuon )
	{
		std::cout<<"[CurEvent->m_trigTime - preIWMuon->m_trigTime] : "<<CurEvent->m_trigTime - preIWMuon->m_trigTime <<endl;
	} else
	{
		std::cout<<"==> Can't find preIWMuon "<<endl;
	}
	//Owp muon
	PhyEvent* preOWMuon=muonVeto_l->preMuon(CurEvent,6);
	if( preOWMuon )
	{
		std::cout<<"[CurEvent->m_trigTime - preOWMuon->m_trigTime] : "<<CurEvent->m_trigTime - preOWMuon->m_trigTime <<endl;
	} else
	{
		 std::cout<<"==> Can't find preOWMuon "<<endl;
	}

	//Iwp muon
	PhyEvent* nextIWMuon=muonVeto_l->nextMuon(CurEvent,5);
	if( nextIWMuon )
	{
		std::cout<<"[nextIWMuon->m_trigTime-CurEvent->m_trigTime] : "<<nextIWMuon->m_trigTime-CurEvent->m_trigTime <<endl;
	} else
	{
		std::cout<<"==> Can't find nextIWMuon "<<endl;
	}
	//Owp muon
	PhyEvent* nextOWMuon=muonVeto_l->nextMuon(CurEvent,6);
	if( nextOWMuon )
	{
		std::cout<<"[nextOWMuon->m_trigTime-CurEvent->m_trigTime] : "<<nextOWMuon->m_trigTime-CurEvent->m_trigTime <<endl;
	} else
	{
		 std::cout<<"==> Can't find nextOWMuon "<<endl;
	}
	return true;
}
bool FindEvent::IsIbd(PhyEvent* CurEvent)
{
	if(( CurEvent->m_fileNum==PromptFileNum && CurEvent->m_localEntry==PromptLocalEntry && CurEvent->m_trigNum==PromptTrigNum ) )
	{
		std::cout<<""<<endl;
		std::cout<<"!!!!!!!! Find the prompt signal !!!!!!!!!!!!!"<<endl;
		printEvt(CurEvent);
        Time2PreMuon(CurEvent);
	    
		if( muonVeto_l->veto(CurEvent) )
	    {
            std::cout<<"muonVeto_l->veto(CurEvent)  : "<<muonVeto_l->veto(CurEvent)<<endl;
            std::cout<<"CurEvent->isMuon()  : "<<CurEvent->isMuon() <<endl;
			std::cout<<"==> Is vetod By MuonVeto Program."<<endl;
	    }else
		{
            std::cout<<"muonVeto_l->veto(CurEvent)  : "<<muonVeto_l->veto(CurEvent)<<endl;
            std::cout<<"CurEvent->isMuon()  : "<<CurEvent->isMuon() <<endl;
			std::cout<<"==> Not be vetod By MuonVeto Program."<<endl;
		}
		AdBuf.push_back(CurEvent->GrabInstance());

		return true;

	}
	
	if(!( CurEvent->m_fileNum==DelayedFileNum && CurEvent->m_localEntry==DelayedLocalEntry && CurEvent->m_trigNum==DelayedTrigNum ) )
	{
		return true;
	}
	std::cout<<""<<endl;
	std::cout<<"!!!!!!!! Find the delayed signal !!!!!!!!!!!!!"<<endl;
	printEvt(CurEvent);
    Time2PreMuon(CurEvent);
	
	if( muonVeto_l->veto(CurEvent) )
	{
            std::cout<<"muonVeto_l->veto(CurEvent)  : "<<muonVeto_l->veto(CurEvent)<<endl;
            std::cout<<"CurEvent->isMuon()  : "<<CurEvent->isMuon() <<endl;
		std::cout<<"==> Is vetod By MuonVeto Program."<<endl;
	}else
	{
            std::cout<<"muonVeto_l->veto(CurEvent)  : "<<muonVeto_l->veto(CurEvent)<<endl;
            std::cout<<"CurEvent->isMuon()  : "<<CurEvent->isMuon() <<endl;
		std::cout<<"==> Not be  vetod By MuonVeto Program."<<endl;
	}
	AdBuf.push_back(CurEvent->GrabInstance());		

	std::cout<<""<<endl;
	if(!( AdBuf.size()==2) )
	{
		std::cout<<"size of AdBuf has something wrong!"<<endl;
	}else
	{
		std::cout<<"size of AdBuf is right!"<<endl;
	}

	//ADMuon
	PhyEvent* preAdMu=muonVeto_l->preAdMuon(AdBuf[1],AdMuonELow,AdMuonEHigh);
	if( preAdMu )
	{
		DelayedTrigTime2AdMuon_l=AdBuf[1]->m_trigTime - preAdMu->m_trigTime;
	} else
	{
		DelayedTrigTime2AdMuon_l=DelayedTrigTime2AdMuon*2;
		std::cout<<"==> Can't find preAdMu "<<endl;
	}
	
	//ADshower muon
	PhyEvent* preAdShowerMuon=muonVeto_l->preShowerMuon(AdBuf[1]);
	if( preAdShowerMuon )
	{
		DelayedTrigTime2AdShowerMuon_l=AdBuf[1]->m_trigTime - preAdShowerMuon->m_trigTime; 
		//std::cout<<"preAdShowerMuon->m_trigTime : "<<preAdShowerMuon->m_trigTime <<endl;
	} else
	{
		DelayedTrigTime2AdShowerMuon_l=DelayedTrigTime2AdShowerMuon*2;
		std::cout<<"==> Can't find preAdShowerMuon "<<endl;
	}

	//Iwp muon
	PhyEvent* preIWMuon=muonVeto_l->preMuon(AdBuf[1],5);
	if( preIWMuon )
	{
		DelayedTrigTime2IWpMuon_l=AdBuf[1]->m_trigTime - preIWMuon->m_trigTime;
	} else
	{
		DelayedTrigTime2IWpMuon_l=DelayedTrigTime2IWpMuon*2;
		std::cout<<"==> Can't find preIWMuon "<<endl;
	}
	
	//Owp muon
	PhyEvent* preOWMuon=muonVeto_l->preMuon(AdBuf[1],6);
	if( preOWMuon )
	{
		DelayedTrigTime2OWpMuon_l=AdBuf[1]->m_trigTime - preOWMuon->m_trigTime;
	} else
	{
		DelayedTrigTime2OWpMuon_l=DelayedTrigTime2OWpMuon*2;
		std::cout<<"==> Can't find preOWMuon "<<endl;
	}
	//std::cout<<"Time2PreAdEvent_l : "<<Time2PreAdEvent_l <<endl; 

	//std::cout<<"Time2NextAdEvent_l : "<<Time2NextAdEvent_l <<endl; 
	std::cout<<"DelayedTrigTime2AdMuon_l : "<<DelayedTrigTime2AdMuon_l <<endl; 
	std::cout<<"DelayedTrigTime2AdShowerMuon_l : "<<DelayedTrigTime2AdShowerMuon_l <<endl;
	//std::cout<<"AdBuf[1]->m_trigTime : "<<AdBuf[1]->m_trigTime <<endl;
	//std::cout<<"preAdShowerMuon->m_trigTime : "<<preAdShowerMuon->m_trigTime <<endl;
	std::cout<<"DelayedTrigTime2IWpMuon_l : "<<DelayedTrigTime2IWpMuon_l <<endl; 
	std::cout<<"DelayedTrigTime2OWpMuon_l : "<<DelayedTrigTime2OWpMuon_l <<endl; 

    if( !((DelayedTrigTime2AdMuon_l>DelayedTrigTime2AdMuon)&&(DelayedTrigTime2IWpMuon_l>DelayedTrigTime2IWpMuon)&&(DelayedTrigTime2AdShowerMuon_l>DelayedTrigTime2AdShowerMuon)&&(DelayedTrigTime2OWpMuon_l>DelayedTrigTime2OWpMuon)) )
    {
		std::cout<<"==> Is voted by Muon ! "<<endl;
    }else
	{
		std::cout<<"==> Not be  voted by Muon ! "<<endl;
	}

    if( !(((AdBuf[1]->m_trigTime-AdBuf[0]->m_trigTime)>IbdTimeIntervalMin)&&((AdBuf[1]->m_trigTime-AdBuf[0]->m_trigTime)<IbdTimeIntervalMax)) )
	{   
		std::cout<<"==> The time interval is wrong:"<<endl; 
	}else
	{
	
		std::cout<<"==> The time interval is right"<<endl; 
	}

	std::cout<<"AdBuf[1]->m_trigTime-AdBuf[0]->m_trigTime : "<<AdBuf[1]->m_trigTime-AdBuf[0]->m_trigTime<<endl;

	if( !((AdBuf[0]->energy()>promptELow) && (AdBuf[0]->energy()<promptEHigh)&&(AdBuf[1]->energy()>delayedELow)&&(AdBuf[1]->energy()<delayedEHigh)) )
	{   
		std::cout<<"==> The energy has something wrong :"<<endl; 
		
	}else
	{
		std::cout<<"==> The energy is  right :"<<endl; 
	}

	std::cout<<"The prompt energy  : "<<AdBuf[0]->energy() <<endl; 
	std::cout<<"The delayed energy : "<<AdBuf[1]->energy() <<endl; 

	return true;
}

bool FindEvent::FindRunInfo(PhyEvent* CurEvent)
{
	if(( CurEvent->m_fileNum==PromptFileNum && CurEvent->m_localEntry==PromptLocalEntry && CurEvent->m_trigNum==PromptTrigNum )  )
	{		
		std::cout<<""<<endl;
		std::cout<<"====find the prompt signal , now: ===="<<endl;
		if( !AdEvtBuf[CurEvent->m_det-1].empty() )
		{
			std::cout<<"==> AdEvtBuf[CurEvent->m_det-1] is NOT empty "<<endl;
			std::cout<<"AdEvtBuf[CurEvent->m_det-1].size() : "<<AdEvtBuf[CurEvent->m_det-1].size() <<endl;
			PhyEvent*LastEvent=AdEvtBuf[CurEvent->m_det-1][AdEvtBuf[CurEvent->m_det-1].size()-1];
			if( (CurEvent->m_trigTime-LastEvent->m_trigTime)>Time2LastBufEvent )
			{
				std::cout<<"(CurEvent->m_trigTime-LastEvent->m_trigTime)>Time2LastBufEvent , start to clear buffer ,be the first signal in buffer ."<<endl;
				for(int i=0;i<(int)AdEvtBuf[CurEvent->m_det-1].size();i++)
				{
					AdEvtBuf[CurEvent->m_det-1][i]->ReleaseInstance();
				}
				AdEvtBuf[CurEvent->m_det-1].clear();
				
			} else
			{
				std::cout<<"(CurEvent->m_trigTime-LastEvent->m_trigTime) < Time2LastBufEvent , push_back in buffer , is the " << AdEvtBuf[CurEvent->m_det-1].size()+1<<"th signal in buffer."<<endl;
				for( int i=0 ; i<(int)AdEvtBuf[CurEvent->m_det-1].size() ; i++ )
				{
					std::cout<<"following is info of the "<<i+1 <<"th signal in buffer : "<<endl;
					printEvt( AdEvtBuf[CurEvent->m_det-1][i]);
					Time2PreMuon(AdEvtBuf[CurEvent->m_det-1][i]);
				}
			}

		} else
		{
			std::cout<<"==> AdEvtBuf[CurEvent->m_det-1] is empty "<<endl;
		}
		
		AdEvtBuf[CurEvent->m_det-1].push_back(CurEvent->GrabInstance());
		return true;
		
	}

	if(CurEvent->m_fileNum==DelayedFileNum && CurEvent->m_localEntry==DelayedLocalEntry && CurEvent->m_trigNum==DelayedTrigNum )
	{
		std::cout<<""<<endl;
		std::cout<<"====find the delayed signal , now: ===="<<endl;
		if( !AdEvtBuf[CurEvent->m_det-1].empty() )
		{
			std::cout<<"==> AdEvtBuf[CurEvent->m_det-1] is NOT empty "<<endl;
			std::cout<<"AdEvtBuf[CurEvent->m_det-1].size() : "<<AdEvtBuf[CurEvent->m_det-1].size() <<endl;
			PhyEvent*LastEvent=AdEvtBuf[CurEvent->m_det-1][AdEvtBuf[CurEvent->m_det-1].size()-1];
			if( (CurEvent->m_trigTime-LastEvent->m_trigTime)>Time2LastBufEvent )
			{
				std::cout<<"(CurEvent->m_trigTime-LastEvent->m_trigTime)>Time2LastBufEvent , start to clear buffer ,be the first signal in buffer ."<<endl;
				for(int i=0;i<(int)AdEvtBuf[CurEvent->m_det-1].size();i++)
				{
					AdEvtBuf[CurEvent->m_det-1][i]->ReleaseInstance();
				}
				AdEvtBuf[CurEvent->m_det-1].clear();
				
			} else
			{
				std::cout<<"(CurEvent->m_trigTime-LastEvent->m_trigTime) < Time2LastBufEvent , push_back in buffer , is the " << AdEvtBuf[CurEvent->m_det-1].size()+1<<"th signal in buffer."<<endl;
				for( int i=0 ; i<(int)AdEvtBuf[CurEvent->m_det-1].size() ; i++ )
				{
					std::cout<<"following is info of the "<<i+1 <<"th signal in buffer : "<<endl;
					printEvt( AdEvtBuf[CurEvent->m_det-1][i]);
					Time2PreMuon(AdEvtBuf[CurEvent->m_det-1][i]);
				}

			}

		} else
		{
			std::cout<<"==> AdEvtBuf[CurEvent->m_det-1] is empty "<<endl;
		}
		
		AdEvtBuf[CurEvent->m_det-1].push_back(CurEvent->GrabInstance());
		return true;
		
	}

	if(!AdEvtBuf[CurEvent->m_det-1].empty())
	{
		PhyEvent*LastEvent=AdEvtBuf[CurEvent->m_det-1][AdEvtBuf[CurEvent->m_det-1].size()-1];
		if((CurEvent->m_trigTime-LastEvent->m_trigTime)>Time2LastBufEvent)
		{
			for(int i=0;i<(int)AdEvtBuf[CurEvent->m_det-1].size();i++)
			{
				AdEvtBuf[CurEvent->m_det-1][i]->ReleaseInstance();
			}
			AdEvtBuf[CurEvent->m_det-1].clear();
		}
	}
	AdEvtBuf[CurEvent->m_det-1].push_back(CurEvent->GrabInstance());

	return true;
}

bool FindEvent::finalize()
{
	std::cout<<"FindEvent finalize..."<<endl;
	return true;
}
