{
    gROOT->ProcessLine("#include <vector>;");
    vector<double> a;
    a.push_back(1.1);
    a.push_back(2.1);
    a.push_back(3.1);
    a.push_back(4.1);
    a.push_back(5.1);
    a.push_back(6.1);
    std::cout<<"a.size()  : "<<a.size() <<endl;
    for(vector<double>::iterator it=a.begin(); it<a.end();it++) 
    {
        std::cout<<"  : "<<*it<<endl;
        if( *it==2.1||*it==3.1 )
        {
            //it=a.erase(it,a.end());
            it=a.erase(it);
            it--;//here, it is the same to --it;
        }
    }
    //a.erase(3);
    std::cout<<"a.size()  : "<<a.size() <<endl;
    for( int i=0 ; i<a.size()  ; i++ )
    {
        std::cout<<i<<"  : "<<a[i]<<endl;
    }
    
}
