{
    gROOT->ProcessLine("#include <algorithm>");
string s = "12||345||56";
size_t p1 = s.rfind("5");
size_t p2 = s.rfind("5", p1-1);
cout <<p1 <<" "<<p2<<endl;
string s1=s.substr(p2);
cout<<s1<<endl;


	std::vector<std::string>  m_vFlist;
    bool m_notValid;
    std::string  fname;
    std::ifstream  fs("/workfs/dyw/lidj/runlist/P12C/RpcGood/GoodRun/EH1/21651.list");
    //while ( (fs>>fname).good() ) {
    while ( fs>>fname ) {
        int fpos=fname.rfind("_");
        std::string fnameNum=fname.substr(fpos);
	    m_vFlist.push_back(fnameNum);
    }
    std::cout<<"m_vFlist sieze  : "<<m_vFlist.size()<<endl;

	//std::vector<std::string>::iterator itf = std::find(m_vFlist.begin(), m_vFlist.end(), eventFilenameNum);
	std::vector<std::string>::iterator itf = std::find(m_vFlist.begin(), m_vFlist.end(), "_0008.root");
    m_notValid = (itf == m_vFlist.end()); 
    cout<<"m_notValid "<<m_notValid <<endl;
}
