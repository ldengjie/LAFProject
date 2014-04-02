{
    vector<int> a[5];
    a[2].push_back(1);
    for( int i=0 ;i<5  ; i++ )
    {
        std::cout<<"a["<<i <<"] size : "<<a[i].size()<<endl;
    }
    
    vector<int> b;
    b.push_back(1);
    b.push_back(1);
    b.push_back(1);
    b.push_back(1);
    b.push_back(1);
    b.push_back(1);
    b[3]=2;
    for( int j=0 ; j<(int)b.size() ; j++ )
    {
        std::cout<<"b["<< j<<"]  : "<<b[j]<<endl;
    }
    
}
