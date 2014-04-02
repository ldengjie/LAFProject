{
    TH1F* h= new TH1F("h","h",10,1,11);
    std::cout<<"h->FindBin(1)  : "<<h->FindBin(1)<<endl;
    std::cout<<"h->FindBin(11)  : "<<h->FindBin(11)<<endl;
    std::cout<<"h->FindBin(111)  : "<<h->FindBin(111)<<endl;
}
