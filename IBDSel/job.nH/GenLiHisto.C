#include  <iostream>
#include  <TChain.h>
#include  <TROOT.h>
#include  <string>
int anaTree(int siteNum,string dataVer)
{
	gROOT->ProcessLine(".L LiHisto.C+");
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
	
    chainFile="/afs/ihep.ac.cn/users/l/lidj/largedata/IBD/";
    chainFile+=dataVer;
    //chainFile+="bak2/";
    chainFile+=".nH/";
    chainFile+=site;
    chainFile+="/*IBD.root";
    chain.Add(chainFile.c_str());
    if( dataVer=="P12E" )
    {
        chainFile="/afs/ihep.ac.cn/users/l/lidj/largedata/IBD/";
        chainFile+="P12A.nH";
        chainFile+="/";
        //chainFile+="bak2/";
        chainFile+=site;
        chainFile+="/*IBD.root";
        chain.Add(chainFile.c_str());
    }
    string  siteAndDataVer=site+dataVer;
    std::cout<<"siteAndDataVer  : "<<siteAndDataVer<<endl;
	chain.Process("LiHisto",siteAndDataVer.c_str());
	return 0;
}
int GenLiHisto(string dataVer)
{
    //string dataVer="P13A";
    if( !(dataVer=="P13A"||dataVer=="P12E"||dataVer=="P14A") )
    {
        std::cout<<"  The dataVerion is wrong ,please check! "<<endl;
    }
    anaTree(1,dataVer);
    anaTree(2,dataVer);
    anaTree(3,dataVer);
    return 0;
}
