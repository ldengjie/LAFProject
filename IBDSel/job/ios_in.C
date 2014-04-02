{
    ifstream runlist;
    string listname="EH1run.list.P17A";
    cout<<"listname : "<<listname<<endl;
    runlist.open(listname.c_str());
    if( !runlist )
    {
        std::cout<<"no "<<endl;
    }else
    {
        std::cout<<"yes "<<endl;
    }

}
