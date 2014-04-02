{
    using namespace RooFit;
    TH1F h("h", "h", 3, 0.5, 3.5);
    for (int i=0;i<300;i++) {
        h.Fill(1.0);
    }
    for (int i=0;i<500;i++) {
        h.Fill(2.0);
    }
    for (int i=0;i<400;i++) {
        h.Fill(3.0);
    }
    TCanvas c1("c1", "c1", 400, 400);
    //h->Fit("pol2","L");
    h->Fit("pol2");
    h->Draw();
    c1->Draw();
    //
    RooRealVar x ("x", "x", 0.5, 3.5);
    RooRealVar a ("a", "a", 0.0, -1000,1000.0);
    //RooRealVar a ("a", "a", 0.0, 0,3.0);
    //RooRealVar a ("a", "a",-3.25,-5,0);
    RooRealVar b ("b", "b", 0.0, -1000,1000.0);
    //RooRealVar b ("b", "b", 0.75,0,2);
    //RooRealVar c ("c", "c", 0.0, -100,100.0);
    RooPolynomial poly ("poly", "poly", x, RooArgList(a,b));
    //RooChebychev poly ("poly", "poly", x, RooArgList(a,b));
    RooDataHist data ("data", "data", x, &h);
    //pdf.fitTo(data);
    RooFitResult *fitr =poly.fitTo(data,Save());
    double fitnll = fitr->minNll(); 
    std::cout<<"fitnll  : "<<fitnll<<endl;
    //
    //RooChi2Var chi2 ("chi2", "chi2", pdf, data, Extended(kTRUE));
    //RooChi2Var chi2 ("chi2", "chi2", poly, data,DataError(RooAbsData::Poisson));
    RooChi2Var chi2 ("chi2", "chi2", poly, data);
    Double_t chi2_val = chi2.getVal();
    cout << "plain chi2 from RooChi2Var = " << chi2_val<< endl;
    //
    TCanvas c2("c2", "c2", 400, 400);
    RooPlot* plot = x.frame();
    data.plotOn(plot, Name("data"));
    //pdf.plotOn(plot, Name("pdf"));
    poly.plotOn(plot, Name("pdf"));
    //pdf.plotOn(plot, RooFit::Components(poly), RooFit::LineColor(2));
    plot->Draw();
    cout << "reduced chi2 from plot = " << plot->chiSquare("pdf", "data",1) << endl;

    //TCanvas c3("c3", "c3", 400, 400);
    RooAbsReal* nll = poly.createNLL(data) ;
    //RooPlot* aframe = a.frame() ;
    //nll.plotOn(aframe);
    //aframe->Draw();
    //
    TCanvas c4("c4", "c4", 400, 400);
    RooMinuit m(*nll) ; 
    m.migrad() ; 
    m.hesse() ;
    //RooPlot* p1 = m.contour(a,b,1,2,3,4) ; 
    RooPlot* p1 = m.contour(a,b,1,2,3) ; 
    ////RooPlot* p1 = m.contour(a,b,1) ; 
    p1->Draw();


    //
    std::cout<<"h.GetNbinsX  : "<<h.GetNbinsX()<<endl;
    //cout << "reduced chi2 from RooChi2Var = " << chi2_val/(h.GetNbinsX()-2.0) << endl;
    //cout << "reduced chi2 from plot = " << plot->chiSquare("pdf", "data", (h.GetNbinsX()-2.0)) << endl;
    /*
    double integral_lower = 0.0;
    double integral_upper = 5000;
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
