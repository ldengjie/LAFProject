#include  <iostream>
#include  <string>
#include  <TChain.h>
#include  <TROOT.h>
int anaTree(int siteNum)
{
	gROOT->ProcessLine(".L Ibd.C+");
	TChain chain("Tree/IbdTree");
	string site;
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

	string chainFile;
	
    /*
	chainFile="/afs/ihep.ac.cn/users/l/lidj/data/IBDSelForAcc/RpcGood/";
	chainFile+=site;
	chainFile+="/*IBD.root";
	chain.Add(chainFile.c_str());
	
	chainFile="/afs/ihep.ac.cn/users/l/lidj/data/IBDSelForAcc/RpcBad/";
	chainFile+=site;
	chainFile+="/*IBD.root";
	chain.Add(chainFile.c_str());
	*/
    /*
    chainFile="/afs/ihep.ac.cn/users/l/lidj/largedata/IBD/P12C/";
    chainFile+=site;
    chainFile+="/*IBD.root";
    chain.Add(chainFile.c_str());
    */
    chainFile="/afs/ihep.ac.cn/users/l/lidj/largedata/IBD/P13A/";
    chainFile+=site;
    chainFile+="/*IBD.root";
    chain.Add(chainFile.c_str());
    /*
    chainFile="/afs/ihep.ac.cn/users/l/lidj/largedata/IBD/P12E/";
    chainFile+=site;
    chainFile+="/*IBD.root";
    chain.Add(chainFile.c_str());
    */
	/*		
	cout<<"total IBD Number : "<<chain.GetEntries("isIbd==2")+chain.GetEntries("isIbd==1")<<endl;
	cout<<"total IBD Number(without using rpc veto) : "<<chain.GetEntries("isIbd==0")+chain.GetEntries("isIbd==1")<<endl;
	std::cout<<"total Num  : "<<chain.GetEntries()<<endl;
	std::cout<<"isIbd=3  : "<<chain.GetEntries("isIbd==3")<<endl;
	std::cout<<"isIbd=4  : "<<chain.GetEntries("isIbd==4")<<endl;
	std::cout<<"isIbd=5  : "<<chain.GetEntries("isIbd==5")<<endl;
	
	std::cout<<"start to process . "<<endl;
	*/
	chain.Process("Ibd",site.c_str());
	return 0;
}
