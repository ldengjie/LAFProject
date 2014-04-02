{
    using namespace RooFit;
    int binNum=240;
    
    TH1F* h =new TH1F("h", "h", binNum, 0.5, 3.5);
    TF1 *fsig = new TF1 ("fsig", "3*exp(-3*x)", 0., 10.);
    TF1 *fb = new TF1 ("fb", "0.02*exp(-0.02*x)", 0., 10.);
    TF1 *fsum=new TF1 ("fsum", "0.7*fsig+0.3*fb", 0., 10.);
    for( int i=1 ; i<=binNum ; i++ )
    {
        //std::cout<<"h->GetBinLowEdge(i)  : "<<h->GetBinLowEdge(i)<<endl;
        h->SetBinContent(i,1.e8*fsum->Integral(h->GetBinLowEdge(i),h->GetBinLowEdge(i)+h->GetBinWidth(i)));
    }
    
    //h->Fit("pol2","L");
    //h->Fit("expo","L");
    //TF1 *fsumfit=new TF1 ("fsumfit", "1.e6*(0.7*[0]*exp(-[1]*x)+0.3*[2]*exp(-[3]*x))", 0.5, 3.5);
    //TF1 *fsumfit=new TF1 ("fsumfit", "0.7*[0]*exp(-[1]*x)+0.3*[2]*exp(-[3]*x)", 0.5, 3.5);
    /*
    TF1 *fsumfit=new TF1 ("fsumfit", "1.e6*0.7*[0]*exp(-[1]*x)+1.e6*0.3*[2]*exp(-[3]*x)", 0.5, 3.5);
    //fsumfit->SetParameters(2,2,38,38);
    fsumfit->SetParameters(2,2,38,38);
    TCanvas c0("c0", "c0", 400, 400);
    fsumfit->Draw();
    TCanvas c1("c1", "c1", 400, 400);
    h->Fit("fsumfit","L");
    h->Draw();
    c1->Draw();
    */
    //
    RooRealVar x ("x", "x", 0.5, 3.5);
    RooRealVar a ("a", "a", -2, -5,-0);
    //RooRealVar a ("a", "a", 0.0, 0,3.0);
    //RooRealVar a ("a", "a",-3.25,-5,0);
    RooRealVar b ("b", "b", -0.02, -1,0.0);
    //RooRealVar coe ("coe", "coe", 0.3, 0,1);
    RooRealVar coe1 ("coe1", "coe1", 1.e5,0,1.e8);
    RooRealVar coe2 ("coe2", "coe2", 1.e5,0,1.e8);
    //RooRealVar b ("b", "b", 0.75,50);
    //RooRealVar c ("c", "c", 0.0, -100,100.0);
    //RooPolynomial poly ("poly", "poly", x, RooArgList(a,b));
    //RooChebychev poly ("poly", "poly", x, RooArgList(a,b));
    RooExponential poly0("poly0", "poly0", x, a);
    RooExponential poly1("poly1", "poly1", x, b);
    RooAddPdf poly("poly", "poly", RooArgList(poly0,poly1), RooArgList(coe1,coe2));
    RooDataHist data ("data", "data", x, h);
    //pdf.fitTo(data);
    RooFitResult *fitr =poly.fitTo(data,Save());
    double fitnll = fitr->minNll(); 
    std::cout<<"fitnll  : "<<fitnll<<endl;
    std::cout<<"a  : "<<a->getVal()<<endl;
    std::cout<<"b  : "<<b->getVal()<<endl;
    std::cout<<"coe1  : "<<coe1->getVal()<<endl;
    std::cout<<"coe2  : "<<coe2->getVal()<<endl;
    //
    //RooChi2Var chi2 ("chi2", "chi2", pdf, data, Extended(kTRUE));
    //RooChi2Var chi2 ("chi2", "chi2", poly, data,DataError(RooAbsData::Poisson));
    std::cout<<">>>>>>>>>>>  begin chi2 "<<endl;
    RooChi2Var chi2 ("chi2", "chi2", poly, data);
    Double_t chi2_val = chi2.getVal();
    //
    TCanvas c2("c2", "c2", 400, 400);
    RooPlot* plot = x.frame();
    data.plotOn(plot, Name("data"));
    //pdf.plotOn(plot, Name("pdf"));
    poly.plotOn(plot, Name("pdf"));
    poly.plotOn(plot, Components(poly0), LineColor(kRed));
    poly.plotOn(plot, Components(poly1), LineColor(kGreen));
    //pdf.plotOn(plot, RooFit::Components(poly), RooFit::LineColor(2));
    plot->Draw();

    //TCanvas c3("c3", "c3", 400, 400);
    std::cout<<">>>>>>>>>>>  begin nll "<<endl;
    RooAbsReal* nll = poly.createNLL(data) ;
    //RooPlot* aframe = a.frame() ;
    //nll.plotOn(aframe);
    //aframe->Draw();
    //
    TCanvas c4("c4", "c4", 400, 400);
    std::cout<<">>>>>>>>>>>  begin to RooMinuit "<<endl;
    RooMinuit m(*nll) ; 
    std::cout<<">>>>>>>>>>>  begin to migrad "<<endl;
    m.migrad() ; 
    std::cout<<">>>>>>>>>>>  begin to hesse "<<endl;
    m.hesse() ;
    std::cout<<">>>>>>>>>>>  begin to contour "<<endl;
    RooPlot* p1 = m.contour(a,b,1,2,3) ; 
    //RooPlot* p1 = m.contour(a,b,1) ; 
    p1->Draw();

    std::cout<<"a  : "<<a->getVal()<<endl;
    std::cout<<"b  : "<<b->getVal()<<endl;
    std::cout<<"coe1  : "<<coe1->getVal()<<endl;
    std::cout<<"coe2  : "<<coe2->getVal()<<endl;

    std::cout<<">>>>>>>>>>>  begin to second fit "<<endl;
    poly.fitTo(data);
    TCanvas c5("c5", "c5", 400, 400);
    RooPlot* plot5 = x.frame();
    data.plotOn(plot5, Name("data"));
    //pdf.plotOn(plot, Name("pdf"));
    poly.plotOn(plot5, Name("pdf"));
    poly.plotOn(plot5, Components(poly0), LineColor(kRed));
    poly.plotOn(plot5, Components(poly1), LineColor(kGreen));
    //pdf.plotOn(plot, RooFit::Components(poly), RooFit::LineColor(2));
    plot5->Draw();
    std::cout<<"a  : "<<a->getVal()<<endl;
    std::cout<<"b  : "<<b->getVal()<<endl;
    std::cout<<"coe1  : "<<coe1->getVal()<<endl;
    std::cout<<"coe2  : "<<coe2->getVal()<<endl;
    TCanvas c6("c6", "c6", 400, 400);
    std::cout<<">>>>>>>>>>>  begin to second RooMinuit "<<endl;
    RooMinuit m2(*nll) ; 
    std::cout<<">>>>>>>>>>>  begin to second migrad "<<endl;
    m2.migrad() ; 
    std::cout<<">>>>>>>>>>>  begin to second hesse "<<endl;
    m2.hesse() ;
    std::cout<<">>>>>>>>>>>  begin to second contour "<<endl;
    //RooPlot* p12 = m2.contour(a,b,1,2,3) ; 
    RooPlot* p12 = m2.contour(coe1,coe2,1,2,3) ; 
    p12->Draw();

    std::cout<<"a  : "<<a->getVal()<<" +- "<<a->getError()<<endl;
    std::cout<<"b  : "<<b->getVal()<<" +- "<<b->getError()<<endl;
    std::cout<<"coe1  : "<<coe1->getVal()<<endl;
    std::cout<<"coe2  : "<<coe2->getVal()<<endl;
    cout << "plain chi2 from RooChi2Var = " << chi2_val<< endl;
    cout << "reduced chi2 from plot = " << plot->chiSquare("pdf", "data",1) << endl;
    std::cout<<"h->Integral  : "<<h->Integral(1,binNum)<<endl;
    //
    std::cout<<"h.GetNbinsX  : "<<h.GetNbinsX()<<endl;
    std::cout<<">>>>>>>>>>>  begin second chi2 "<<endl;
    RooChi2Var chi3 ("chi3", "chi3", poly, data);
    Double_t chi3_val = chi3.getVal();
    std::cout<<"chi3_val  : "<<chi3_val<<endl;
    //cout << "reduced chi2 from RooChi2Var = " << chi2_val/(h.GetNbinsX()-2.0) << endl;
    //cout << "reduced chi2 from plot = " << plot->chiSquare("pdf", "data", (h.GetNbinsX()-2.0)) << endl;
    /*
    double integral_lower = 0.0;
    double integral_upper = 50;
    RooRealVar poly_integral ("poly_integral", "poly_integral", 5,integral_lower, integral_upper);
    RooAddPdf pdf("pdf", "pdf", RooArgList(poly), RooArgList(poly_integral));
    RooFitResult *fitr1 =pdf.fitTo(data,Save());
    double fitnll1 = fitr1->minNll(); 
    std::cout<<"fitnll1  : "<<fitnll1<<endl;
    //
    //RooChi2Var chi2 ("chi2", "chi2", pdf, data, Extended(kTRUE));
    //RooChi2Var chi2 ("chi2", "chi2", pdf, data,DataError(RooAbsData::Poisson));
    RooChi2Var chi21 ("chi21", "chi21", pdf, data);
    Double_t chi2_val1 = chi21.getVal();
    cout << "plain chi2 from RooChi2Var1 = " << chi2_val1<< endl;
    //
    TCanvas c21("c21", "c21", 400, 400);
    RooPlot* plot1 = x.frame();
    data.plotOn(plot1, Name("data1"));
    //pdf.plotOn(plot, Name("pdf"));
    pdf.plotOn(plot1, Name("pdf1"));
    //pdf.plotOn(plot, RooFit::Components(pdf), RooFit::LineColor(2));
    plot1->Draw();
    cout << "reduced chi2 from plot1 = " << plot1->chiSquare("pdf1", "data1",1) << endl;

    TCanvas c31("c31", "c31", 400, 400);
    RooAbsReal* nll1 = pdf.createNLL(data) ;
    RooPlot* aframe1 = a.frame() ;
    nll1.plotOn(aframe1);
    aframe1->Draw();
    */
}
