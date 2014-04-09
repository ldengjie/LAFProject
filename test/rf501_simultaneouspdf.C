//////////////////////////////////////////////////////////////////////////
//
// 'ORGANIZATION AND SIMULTANEOUS FITS' RooFit tutorial macro #501
// 
// Using simultaneous p.d.f.s to describe simultaneous fits to multiple
// datasets
//
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
#include "RooChebychev.h"
#include "RooAddPdf.h"
#include "RooSimultaneous.h"
#include "RooCategory.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
using namespace RooFit ;


void rf501_simultaneouspdf()
{
  // C r e a t e   m o d e l   f o r   p h y s i c s   s a m p l e
  // -------------------------------------------------------------

  // Create observables
  RooRealVar x("x","x",-8,8) ;

  // Construct signal pdf
  RooRealVar mean("mean","mean",0,-8,8) ;
  RooRealVar sigma("sigma","sigma",0.3,0.1,10) ;
  RooGaussian gx("gx","gx",x,mean,sigma) ;

  x.setRange("test",-0,0.51);
  RooAbsReal* tTmpCoe =gx->createIntegral(x,NormSet(x),Range("test"));
  //RooAbsReal* tTmpCoe =gx->createIntegral(x,NormSet(x),Range(0,2));
        std::cout<<"tTmpCoe  : "<<tTmpCoe->getVal()<<endl;
        return ;

  // Construct background pdf
  RooRealVar a0("a0","a0",-0.1,-1,1) ;
  RooRealVar a1("a1","a1",0.004,-1,1) ;
  RooChebychev px("px","px",x,RooArgSet(a0,a1)) ;

  // Construct composite pdf
  RooRealVar f("f","f",0.2,0.,1.) ;
  RooAddPdf model("model","model",RooArgList(gx,px),f) ;



  // C r e a t e   m o d e l   f o r   c o n t r o l   s a m p l e
  // --------------------------------------------------------------

  // Construct signal pdf. 
  // NOTE that sigma is shared with the signal sample model
  RooRealVar mean_ctl("mean_ctl","mean_ctl",-3,-8,8) ;
  RooGaussian gx_ctl("gx_ctl","gx_ctl",x,mean_ctl,sigma) ;

  // Construct the background pdf
  RooRealVar a0_ctl("a0_ctl","a0_ctl",-0.1,-1,1) ;
  RooRealVar a1_ctl("a1_ctl","a1_ctl",0.5,-0.1,1) ;
  RooChebychev px_ctl("px_ctl","px_ctl",x,RooArgSet(a0_ctl,a1_ctl)) ;

  // Construct the composite model
  RooRealVar f_ctl("f_ctl","f_ctl",0.5,0.,1.) ;
  RooAddPdf model_ctl("model_ctl","model_ctl",RooArgList(gx_ctl,px_ctl),f_ctl) ;



  // G e n e r a t e   events   f o r   b o t h   s a m p l e s 
  // ---------------------------------------------------------------

  // Generate 1000 events in x and y from model
  RooDataSet *data = model.generate(RooArgSet(x),100) ;
  RooDataSet *data_ctl = model_ctl.generate(RooArgSet(x),2000) ;
  // C r e a t e   i n d e x   c a t e g o r y   a n d   j o i n   s a m p l e s 
  // ---------------------------------------------------------------------------
  // Define category to distinguish physics and control samples events
  RooCategory sample("sample","sample") ;
  sample.defineType("physics") ;
  sample.defineType("control") ;
  RooDataSet combData("combData","combined data",x,Index(sample),Import("physics",*data),Import("control",*data_ctl)) ;
  RooSimultaneous simPdf("simPdf","simultaneous pdf",sample) ;
  simPdf.addPdf(model,"physics") ;
  simPdf.addPdf(model_ctl,"control") ;
  simPdf.fitTo(combData) ;
  // Plot all data tagged as physics sample
  RooPlot* frame1 = x.frame(Bins(30),Title("Physics sample")) ;
  combData.plotOn(frame1,Cut("sample==sample::physics")) ;
  simPdf.plotOn(frame1,Slice(sample,"physics"),ProjWData(sample,combData)) ;
  simPdf.plotOn(frame1,Slice(sample,"physics"),Components("px"),ProjWData(sample,combData),LineStyle(kDashed)) ;
  // The same plot for the control sample slice
  RooPlot* frame2 = x.frame(Bins(30),Title("Control sample")) ;
  combData.plotOn(frame2,Cut("sample==sample::control")) ;
  simPdf.plotOn(frame2,Slice(sample,"control"),ProjWData(sample,combData)) ;
  simPdf.plotOn(frame2,Slice(sample,"control"),Components("px_ctl"),ProjWData(sample,combData),LineStyle(kDashed)) ;
  TCanvas* c = new TCanvas("rf501_simultaneouspdf","rf403_simultaneouspdf",800,400) ;
  c->Divide(2) ;
  c->cd(1) ; gPad->SetLeftMargin(0.15) ; frame1->GetYaxis()->SetTitleOffset(1.4) ; frame1->Draw() ;
  c->cd(2) ; gPad->SetLeftMargin(0.15) ; frame2->GetYaxis()->SetTitleOffset(1.4) ; frame2->Draw() ;


}
