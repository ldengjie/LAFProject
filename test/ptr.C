{
    TFile f("13.root");
    if( f.IsZombie() )
    {

        std::cout<<"don't exists "<<endl;
    }
    TFile* f1=new TFile("23.root");
    if( !f1)
    {
        std::cout<<"23 doesn't exists "<<endl;
    }else
    {
        std::cout<<"ptr is not NULL "<<endl;
    }
    TH1F* h=(TH1F*)f1->Get("ni");
    if( !h )
    {
        std::cout<<"h doesn't exists "<<endl;
    }
    delete h;
    h=NULL;

    delete f1;
    f1=NULL;

}
