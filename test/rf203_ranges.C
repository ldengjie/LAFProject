/////////////////////////////////////////////////////////////////////////
//
// 'ADDITION AND CONVOLUTION' RooFit tutorial macro #203
// 
// Fitting and plotting in sub ranges
//
//
// 07/2008 - Wouter Verkerke 
//
/////////////////////////////////////////////////////////////////////////

#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooConstVar.h"
#include "RooPolynomial.h"
#include "RooAddPdf.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TH1.h"
using namespace RooFit ;


void rf203_ranges()
{
  // S e t u p   m o d e l 
  // ---------------------

  // Construct observables x
  RooRealVar x("x","x",-20,20) ;
  
  // Construct gaussx(x,mx,1)
  RooRealVar mx("mx","mx",0) ;
  RooGaussian gx("gx","gx",x,mx,RooConst(1)) ;

  // Construct px = 1 (flat in x)
  RooPolynomial px("px","px",x) ;

  // Construct model = f*gx + (1-f)px
  //RooRealVar f("f","f",0.5) ;
  //RooRealVar f("f","f",0.5,0,1) ;
  RooRealVar f("f","f",4000,0,10000) ;
  RooRealVar m("m","m",4000,0,10000) ;
  RooAddPdf model("model","model",RooArgList(gx,px),RooArgList(f,m)) ;

  // Generated 10000 events in (x,y) from p.d.f. model

  // F i t   f u l l   r a n g e 
  // ---------------------------

  // Fit p.d.f to all data
  //RooFitResult* r_full = model.fitTo(*modelData,Save(kTRUE)) ;


  // F i t   p a r t i a l   r a n g e 
  // ----------------------------------

  // Define "signal" range in x as [-3,3]
  //x.setRange("signal",-3,3) ;  

  // Fit p.d.f only to data in "signal" range

  //TF1 *fa1 = new TF1("fa1","0.4*exp(-0.5*x*x)",0,10);
  //TF1 *fa1 = new TF1("fa1","1.1",0,10);
  //RooAbsReal* rfa1 = bindFunction(fa1,x) ;



  // P l o t   /   p r i n t   r e s u l t s 
  // ---------------------------------------

  // Make plot frame in x and add data and fitted model

  //RooDataSet* modelData = model.generate(x,10000) ;
  //TH1F* h=modelData->createHistogram("h",x,Binning(100,-20,20));
  //TFile* f1= new TFile("rf203.root","RECREATE");
  //h->Write();
  TFile* f1= new TFile("rf203.root");
  TH1F* h=(TH1F*)f1->Get("h__x");
  //h->Draw();
  RooDataHist* hdata= new RooDataHist("hdata","hdata",x,h);
  //RooFitResult* r_sig1 = model.fitTo(*modelData,Save(kTRUE)) ;
  x.setRange(-5,5) ;
  RooPlot* frame = x.frame() ;
  //modelData->plotOn(frame) ;
  hdata->plotOn(frame);
  x.setRange("sig",-5,5) ;
  x.setRange("ref",-2,2) ;

  ////RooPlot* frame = x.frame(Title("Fitting a sub range")) ;
  //RooPlot* frame = x.frame(1000) ;
  //gx.plotOn(frame) ;
  //px.plotOn(frame) ;
  //rfa1->plotOn(frame) ;
  //gx.plotOn(frame,Normalization(1.0));
  //cout<<"getVal() : "<<gx.getVal()<<endl;
  //cout<<"getVal(x) : "<<gx.getVal(x=1)<<endl;
  //cout<<"getVal(mx) : "<<gx.getVal(mx)<<endl;
  //RooAbsReal* intGaussX = gx.createIntegral(x,NormSet(x)) ;
  //RooAbsReal* intGaussX = gx.createIntegral(x) ;
  //RooAbsReal* intGaussX = gx.createIntegral(x,NormSet(x),Range("sig")) ;
  //RooAbsReal* intGaussX = rfa1->createIntegral(x,Range("sig")) ;
  //std::cout<<"intGaussX  : "<<intGaussX->getVal()<<endl;
  //RooAbsReal* gx_cdf = gx.createCdf(x) ;
  //gx_cdf->plotOn(frame,LineColor(kGreen)) ;
  //intGaussX->Print("v");
  //intGaussX->plotOn(frame,LineColor(kRed)) ;
  //RooFitResult* r_sig = model.fitTo(*modelData,Save(kTRUE),Range("signal")) ;

  //modelData->plotOn(frame) ;
  //model.fixCoefRange("sig") ;
  //RooFitResult* r_sig = model.fitTo(*modelData,Save(kTRUE),Range("ref"),SumCoefRange("sig")) ;
  //RooFitResult* r_sig = model.fitTo(*modelData,Save(kTRUE),Range("sig"),SumCoefRange("sig")) ;
  //RooFitResult* r_sig = model.fitTo(*modelData,Save(kTRUE),SumCoefRange("sig")) ;
  //RooFitResult* r_sig = model.fitTo(*modelData,Save(kTRUE),Range("sig")) ;
  //RooFitResult* r_sig = model.fitTo(*modelData,Save(kTRUE)) ;
  //RooFitResult* r_sig = model.fitTo(*hdata,Save(kTRUE)) ;
  RooFitResult* r_sig = model.fitTo(*hdata,Save(kTRUE),SumCoefRange("sig")) ;
  //RooFitResult* r_sig = model.fitTo(*hdata,Save(kTRUE),Range("sig"),SumCoefRange("sig")) ;
  //RooFitResult* r_sig = model.fitTo(*hdata,Save(kTRUE),Range("sig")) ;
  model.plotOn(frame) ; // By default only fitted range is shown
  TString ratio="R = ";
  ratio+=f.getVal()/(m.getVal()+f.getVal());
  model.paramOn(frame,Label(ratio),Layout(0.55,0.85,0.9));
  model.plotOn(frame,Components(gx),LineColor(kRed),LineStyle(kDashed)) ; // By default only fitted range is shown
  model.plotOn(frame,Components(px),LineColor(kGreen),LineStyle(kDashed)) ; // By default only fitted range is shown
  
  // Print fit results 
  cout << "result of fit on all data " << endl ;
  //r_full->Print() ;  
  cout << "result of fit in in signal region (note increased error on signal fraction)" << endl ;
  //r_sig->Print() ;

  // Draw frame on canvas
  new TCanvas("rf203_ranges","rf203_ranges",600,600) ;
  gPad->SetLeftMargin(0.15) ; frame->GetYaxis()->SetTitleOffset(1.4) ; frame->Draw() ;

  return ;
  }
