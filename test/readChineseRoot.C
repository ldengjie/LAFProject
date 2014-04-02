{
    TFile* f=new TFile("chineseRoot.root","read");
    TTree* t=(TTree*)f->Get("Topic");
    std::cout<<"Entries number  : "<<t->GetEntries()<<endl;
    int tnum=t->GetEntries();
    int id;
    //string* name;
    vector<string> *n=0;//=new vector<string>;
    TBranch *bn=0;
    //vector<string> *na;
    t->SetBranchAddress("id",&id);
    //t->SetBranchAddress("na",&n,&bn);
    t->SetBranchAddress("na",&n);
    for( int i=0 ; i<tnum ; i++ )
    {
        t->GetEntry(i);
        std::cout<<"id  : "<<id<<endl;
        for( int j=0 ; j<n->size() ; j++ )
        {
            std::cout<<"name  : "<<n->at(j)<<endl;
        }
        
    }
    
    f->Close();
}
