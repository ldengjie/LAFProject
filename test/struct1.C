#include  <iostream>
#include  <TCanvas.h>
#include  <TMath.h>
#include  <TLegend.h>
#include  <TH1F.h>
#include  <TFile.h>
#include  <fstream>
#include  <sstream>
#include  <TStyle.h>
#include "RooGlobalFunc.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooConstVar.h"
#include "RooFormulaVar.h"
#include    "RooChi2Var.h"
#include "RooExponential.h"
#include    "RooPolynomial.h"
#include    "RooMinuit.h"
#include "RooAddPdf.h"
#include    "RooPlot.h"
#include "RooFitResult.h"
#include  <string>
#include  <TChain.h>
#include  <TROOT.h>
#include  <vector>
#include "RooHistPdf.h"
#include    "RooExtendPdf.h"
using namespace RooFit;
TString nameStr,nameStr2;
	typedef struct isoItems
	{
	    string isoName;
	    double tau;
	    double tFitLowEdge;
	    double tFitHighEdge;
	    double N_value;
	    double N_val_low;
	    double N_val_high;
	    RooRealVar* fitTau;
	    RooRealVar* fitNum;
	    double fitNum_value;
	    double fitNum_err;
	    RooFormulaVar* fitLambda;
	    RooExponential* fitExpPdf;
	    RooExtendPdf* fitExtendPdf;
	    RooHistPdf* fitHistPdf;
	}isoItem;
RooRealVar xe("xe","x for spectrum fit",2,6) ;
RooRealVar xt("xt","x for time fit",1,8, "s");
RooRealVar rateMu("rateMu","rate of showermuon",-0.003);

int genIsoPdf(isoItem& iso)
{
    std::cout<<"0.01 "<<endl;
    nameStr=Form("tau%s",iso.isoName.c_str());
    std::cout<<"nameStr  : "<<nameStr<<endl;
    iso.fitTau = new RooRealVar(nameStr, nameStr, -iso.tau, "s");
    std::cout<<"0.02 "<<endl;
    nameStr=Form("N_%s",iso.isoName.c_str());
    nameStr2=Form("total number of %s",iso.isoName.c_str());
    iso.fitNum=new RooRealVar(nameStr,nameStr2,iso.N_value,iso.N_val_low,iso.N_val_high);
    nameStr=Form("lambda%s",iso.isoName.c_str());
    iso.fitLambda=new RooFormulaVar(nameStr,nameStr,"1/@0 + @1",RooArgList(*iso.fitTau, rateMu));
    std::cout<<"0.03 "<<endl;
    nameStr=Form("exp%s",iso.isoName.c_str());
    nameStr2=Form("%s distribution",iso.isoName.c_str());
    iso.fitExpPdf=new RooExponential(nameStr,nameStr2, xt, *iso.fitLambda);
    nameStr=Form("ExtendPdf%s",iso.isoName.c_str());
    iso.fitExtendPdf=new RooExtendPdf(nameStr,nameStr,*iso.fitExpPdf,*iso.fitNum) ;
    std::cout<<"0.04 "<<endl;
    TFile* f=new TFile("IsoTheoreticalSpec.root");
    nameStr=Form("%sSpectra",iso.isoName.c_str());
    TH1F* h=(TH1F*)f->Get(nameStr);
    RooDataHist* hd =new RooDataHist("hd","hd",xe,h);
    nameStr=Form("%shpdf",iso.isoName.c_str());
    iso.fitHistPdf=new RooHistPdf(nameStr,nameStr,xe,*hd,2) ;
    return 0;
}


int struct1()
{
    std::cout<<"0 "<<endl;
    isoItem isoB12={"B12",0.0291,0.001,0.501,2.e4,0,1.e6};genIsoPdf(isoB12);
    std::cout<<"0.1 "<<endl;
    isoItem isoN12={"N12",0.0159,0.001,0.501,2.e4,0,1.e6};genIsoPdf(isoN12);
    std::cout<<"0.2 "<<endl;
    isoItem isoLi8={"Li8",1.21  ,0.8  ,10.0 ,2.e4,0,1.e6};genIsoPdf(isoLi8);
    std::cout<<"0.3 "<<endl;
    isoItem isoB8 ={"B8" ,1.11  ,0.8  ,10.0 ,2.e4,0,1.e6};genIsoPdf(isoB8);
    std::cout<<"0.4 "<<endl;
    isoItem isoC9 ={"C9" ,0.1825,0.15 ,2.0  ,2.e4,0,1.e6};genIsoPdf(isoC9);
    std::cout<<"0.5 "<<endl;
    isoItem isoLi9={"Li9",0.1717,0.   ,0.   ,2.e4,0,1.e6};genIsoPdf(isoLi9);
    std::cout<<"0.6 "<<endl;
    isoItem isoHe8={"He8",0.2572,0.   ,0.   ,2.e4,0,1.e6};genIsoPdf(isoHe8);
    std::cout<<"0.7 "<<endl;
    //	
    std::cout<<"1 "<<endl;
    map<string,isoItem> iso;
    iso.insert(map<string,isoItem>::value_type("B12",isoB12));
    iso.insert(map<string,isoItem>::value_type("N12",isoN12));
    iso.insert(map<string,isoItem>::value_type("Li8",isoLi8));
    iso.insert(map<string,isoItem>::value_type("B8",isoB8));
    iso.insert(map<string,isoItem>::value_type("C9",isoC9));
    iso.insert(map<string,isoItem>::value_type("Li9",isoLi9));
    iso.insert(map<string,isoItem>::value_type("He8",isoHe8));

    std::cout<<"2 "<<endl;
    string timeFitCom[10]={"B12","N12","C9"};
    RooArgList timeFitComList;
    for( int i=0 ; i<10 ; i++ )
    {
        if( timeFitCom[i]!="" )
        {
            timeFitComList.add(*(iso[timeFitCom[i].c_str()].fitExtendPdf));
        }
    }
    
    RooAddPdf model("model","model",timeFitComList) ;
    model.Print("t");
    return 0;
}
