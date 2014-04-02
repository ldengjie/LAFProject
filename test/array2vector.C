{

    gROOT->ProcessLine("#include <vector>;");
/*
struct S{double b[5];};

vector<S> a;
S ss;
ss.b[0]=12;
ss.b[1]=12.1;
ss.b[2]=12.11;
ss.b[3]=12.111;
ss.b[4]=12.1111;
a.push_back(ss);
std::cout<<"a[0].b[0]  : "<<a[0].b[0]<<endl;
std::cout<<"a[0].b[1]  : "<<a[0].b[1]<<endl;
std::cout<<"a[0].b[2]  : "<<a[0].b[2]<<endl;
std::cout<<"a[0].b[3]  : "<<a[0].b[3]<<endl;
std::cout<<"a[0].b[4]  : "<<a[0].b[4]<<endl;
std::cout<<"a[0].b[0]-12  : "<<a[0].b[0]-12<<endl;
std::cout<<"a[0].b[1]-12.1  : "<<a[0].b[1]-12.1<<endl;
std::cout<<"a[0].b[2]-12.11  : "<<a[0].b[2]-12.11<<endl;
std::cout<<"a[0].b[3]-12.111  : "<<a[0].b[3]-12.111<<endl;
std::cout<<"a[0].b[4]-12.1111  : "<<a[0].b[4]-12.1111<<endl;
*/
/*
a.push_back(ss);

std::cout<<"a[0].ss[1]  : "<<a[0].ss[1]<<endl;
*/  
/*
    vector<int*> a;
    int b[3]={5,6,7};
    a.push_back(b);
    std::cout<<"a[0][0]  : "<<a[0][0]<<endl;
    std::cout<<"a[0][1]  : "<<a[0][1]<<endl;
    std::cout<<"a[0][2]  : "<<a[0][2]<<endl;

    */
    /*
    vector<float> time2Muon[2];
    time2Muon[0].push_back(12);
    time2Muon[0].push_back(23.);
    time2Muon[0].push_back(4);
    time2Muon[0].push_back(44.3);
    std::cout<<"time2Muon[0][0]  : "<<time2Muon[0][0]<<endl;
    std::cout<<"time2Muon[0][1]  : "<<time2Muon[0][1]<<endl;
    std::cout<<"time2Muon[0][2]  : "<<time2Muon[0][2]<<endl;
    std::cout<<"time2Muon[0][3]  : "<<time2Muon[0][3]<<endl;
    */
    vector<double> time2MuonVec;
    vector< vector<double> > time2MuonBuf[4];



    time2MuonVec.push_back(12.);
    time2MuonVec.push_back(12.1);
    time2MuonVec.push_back(12.11);
    time2MuonVec.push_back(12.111);
    //time2MuonVec.erase(time2MuonVec.begin(), time2MuonVec.end());
    time2MuonVec.clear();
    time2MuonVec.push_back(13.);
    time2MuonVec.push_back(13.1);
    time2MuonVec.push_back(13.11);
    time2MuonVec.push_back(13.111);
    time2MuonBuf[0].push_back(time2MuonVec);
    std::cout<<"time2MuonBuf.size()  : "<<time2MuonBuf.size()<<endl;
    std::cout<<"time2MuonBuf[0].size()  : "<<time2MuonBuf[0][0].size()<<endl;

    std::cout<<"time2MuonBuf[0][0][0]  : "<<time2MuonBuf[0][0][0]<<endl;
    std::cout<<"time2MuonBuf[0][0][1]  : "<<time2MuonBuf[0][0][1]<<endl;
    std::cout<<"time2MuonBuf[0][0][2]  : "<<time2MuonBuf[0][0][2]<<endl;
    std::cout<<"time2MuonBuf[0][0][3]  : "<<time2MuonBuf[0][0][3]<<endl;

    if( time2MuonBuf[0][0][1]==13.1 )
{
    std::cout<<"worik "<<endl;
    std::cout<<"time2MuonBuf[0][0][0]-12  : "<<time2MuonBuf[0][0][0]-12<<endl;
    std::cout<<"time2MuonBuf[0][0][1]-12.1  : "<<time2MuonBuf[0][0][1]-12.1<<endl;
    std::cout<<"time2MuonBuf[0][0][2]-12.11  : "<<time2MuonBuf[0][0][2]-12.11<<endl;
    std::cout<<"time2MuonBuf[0][0][3]-12.111  : "<<time2MuonBuf[0][0][3]-12.111<<endl;
}else
{
    std::cout<<"nout worik "<<endl;
}
    /*
    std::cout<<"time2MuonBuf[0][0]  : "<<time2MuonBuf[0][0]<<endl;
    std::cout<<"time2MuonBuf[0][1]  : "<<time2MuonBuf[0][1]<<endl;
    std::cout<<"time2MuonBuf[0][2]  : "<<time2MuonBuf[0][2]<<endl;
    std::cout<<"time2MuonBuf[0][3]  : "<<time2MuonBuf[0][3]<<endl;
    */
    
}
