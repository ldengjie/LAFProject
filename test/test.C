{
	TFile* f=new TFile("test.root","recreate");
	TH1F* h = new TH1F("test","test",100,0,100);
	for( int i=1 ; i<=100 ; i++ )
	{
		h->SetBinContent(i,i-0.5);
	}
	h->Draw();
	TF1* f1= new TF1("f1","pol1",0.,100.);
	h->Fit(f1,"R");
	double par1[2];
	f1->GetParameters(&par1[0]);
	std::cout<<"par1[0]  : "<<par1[0]<<endl;
	std::cout<<"par1[1]  : "<<par1[1]<<endl;
	std::cout<<"y= "<<par1[1]<<"*x + "<< par1[0]<<endl;
	std::cout<<" "<<endl;
	TH1F* h1=h->Rebin(2);
	h1->Draw("same");
	h1->Fit(f1,"R+");
	f1->GetParameters(&par1[0]);
	std::cout<<"par1[0]  : "<<par1[0]<<endl;
	std::cout<<"par1[1]  : "<<par1[1]<<endl;
	std::cout<<"y= "<<par1[1]<<"*x + "<< par1[0]<<endl;
	
}
