{
	gROOT->ProcessLine("#include  <fstream>");
	gROOT->ProcessLine("#include  <string>");
	gROOT->ProcessLine("#include  <iostream>");
	gROOT->ProcessLine("#include  <sstream>");
	gROOT->ProcessLine("using namespace std;");

    TCanvas *c = new TCanvas("c","Graph2D example",0,0,700,600);
	TGraph2D *dt = new TGraph2D(); 
    //TH3D *pmtH = new TH3D("pmtPos","postion of PMT",13,-2400,2400,13,-2400,2400,8,-1800,1800);
	
    ifstream pmtposFile;
	pmtposFile.open("pmtPostion.txt");

	string pmtPos;
    int site,detetor,n;
    int site,detetor,i;
    //Double_t pmtPosX[192],pmtPosY[192],pmtPosZ[192];
    Double_t pmtPosX,pmtPosY,pmtPosZ;
    while( getline(pmtposFile,pmtPos) )
	{
        istringstream is(pmtPos);
        //is>>site>>detetor>>pmtPosX[i]>>pmtPosY[i]>>pmtPosZ[i];
        is>>site>>detetor>>pmtPosX>>pmtPosY>>pmtPosZ;
        //pmtPosX[i]=i+1;
        //pmtPosY[i]=i+1;
        //pmtPosZ[i]=i+1;
        std::cout<<"site  : "<<site<<" det  : "<<detetor <<" X  : "<<pmtPosX <<" Y  : "<<pmtPosY <<" Z  : "<<pmtPosZ <<endl;
        //dt->SetPoint(n,pmtPosX,pmtPosY,pmtPosZ);
        dt->SetPoint(n,pmtPosX,pmtPosY,pmtPosZ);
        //dt->SetPoint(n,n,n,n);
        //pmtH->Fill(pmtPosX,pmtPosY,pmtPosZ);
        n++;
        /*
		string newfilename;
		TFile *oldfile = new TFile(TString(pmtPos));
		TTree *oldtree = (TTree*)oldfile->Get("/Event/Rec/AdScaled");

		TFile *newfile = new TFile(TString(newfilename),"recreate");
		newfile->Write(); 
		delete oldfile;
		delete newfile;
        */
	}
	pmtposFile.close();
    //TGraph2D *dt = new TGraph2D(i,pmtPosX,pmtPosY,pmtPosZ);
    //dt->Draw("surf1");
    dt->Draw("ogl");
    //pmtH->Draw();
}
