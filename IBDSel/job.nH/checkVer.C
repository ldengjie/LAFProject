{
    gROOT->ProcessLine("#include <unistd.h>");
    string dataVer="12e13a12e12a12a13a16a14g";
    string verNum[6]={"11","12","13","14","15","16"};
    vector<string> dataVerReal;
    for( int i=0 ; i<6 ; i++ )
    {
        int pos=0;
        string TmpVer;
        TmpVer=dataVer.substr(pos);
        while( pos!=-1 )
        {
            pos=TmpVer.find(verNum[i]);
            if( pos!=-1 )
            {
                string subdataVer=TmpVer.substr(pos+2,1);
                subdataVer="P"+verNum[i]+toupper(*(subdataVer.c_str()));
                for( vector<string>::iterator it=dataVerReal.begin() ; it!=dataVerReal.end() ; ++it  )
                {
                    if( *it==subdataVer )
                    {
                        dataVerReal.erase(it);
                        it--;
                    }
                }
                dataVerReal.push_back(subdataVer);
                TmpVer=TmpVer.substr(pos+3);
            }
        }
    }
    //sort(dataVerReal.begin(),dataVerReal.end());

    std::cout<<" "<<endl;
    std::cout<<"Find following data version will be analysed : "<<endl;
    std::cout<<" "<<endl;
    string runlistSiteNum[3]={"EH1","EH2","EH3"};
    vector<string> runlistFileContent[3];
    for( int i=0 ; i<3 ; i++ )
    {
        runlistFileContent[i].clear();
    }
    string verSuf;
    for( vector<string>::iterator it=dataVerReal.begin() ; it!=dataVerReal.end() ; ++it )
    {
        std::cout<<*it<<endl;
        verSuf+="_"+*it;
        //check data path
        string datadir="/afs/ihep.ac.cn/users/l/lidj/largedata/IBD/";
        DIR *dir=NULL;
        datadir+=*it;
        dir = opendir(datadir.c_str());
        if( dir==NULL )
        {
            std::cout<<" !!! data dir doesn't exist : "<<datadir<<endl;
        } else
        {
            std::cout<<" data dir exist : "<<datadir<<endl;
            closedir(dir);
        }

        //check runlist
        for( int i=0 ; i<3 ; i++ )
        {
            fstream runlistFile;
            string runlistName=runlistSiteNum[i]+"run.list.";
            runlistName+=*it;
            runlistFile.open(runlistName.c_str(),ios::in);
            if( !runlistFile )
            {
                std::cout<<" !!! runlist doesn't exist  : "<<runlistName<<endl;
            } else
            {
                std::cout<<" runlist exist  : "<<runlistName<<endl;
                string histrunName;
                while( getline(runlistFile,histrunName) )
                {
                    if(histrunName=="") continue;
                    runlistFileContent[i].push_back(histrunName);
                }
            }
            runlistFile.close();
        }
        
        std::cout<<" "<<endl;
    }
    verSuf=verSuf.substr(1);
    std::cout<<"verSuf  : "<<verSuf<<endl;

    //generate combined runlist 
    for( int i=0 ; i<3 ; i++ )
    {
	    ofstream runlistInit;
        string runlistInitName=runlistSiteNum[i]+"run.list."+verSuf;
	    runlistInit.open(runlistInitName.c_str());
        for( vector<string>::iterator it=runlistFileContent[i].begin() ; it!=runlistFileContent[i].end() ; it++ )
        {
            runlistInit<<*it<<endl;
        }
        runlistInit.close();
    }
    
}
