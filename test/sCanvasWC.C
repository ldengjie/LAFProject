#include <TCanvas.h>
#include <TH1.h>
#include <TRandom3.h>
#include <TFile.h>
#include <THStack.h>
#include <TRint.h>

#include <iostream>

using namespace std;

int sCanvasWC(){

      TFile* f = new TFile("sCanvasWC.root","RECREATE");

        TH1F* h = new TH1F("h","",100,0,1);
          TRandom3 rand;
            for(int i = 0; i != 10000; ++i){
                    h->Fill(rand.Rndm());
                      }
              h->SetFillColor(kRed+1);

                TCanvas* can = new TCanvas("c1");
                  h->Draw();
                    can->Write();
                    h->Write();
                      THStack* hs = new THStack("hs","");
                        hs->Add(h);
                          hs->Draw();
                            can->SetName("c2");
                            h->SetName("h2");
                              can->Write();
                              h->Write();
                                f->Close();

                                //f = TFile::Open("test.root", "READ");
                                //
                                //std::cout << "Start root session" << std::endl;
                                    //TRint* rint = new TRint("theApp",0,0);
                                    //TCanvas* c1 = (TCanvas*) f->Get("c1");
                                    //TCanvas* c2 = (TCanvas*) f->Get("c2");
                                    //c1->ls();
                                    //c1->Draw();
                                    //c2->ls();
                                    //c2->Draw();
                                    //rint->Run(0);

}

