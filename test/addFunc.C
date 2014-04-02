{
/*
int func
{
    for( int i=0 ; i<8 ; i++ )
    {
        std::cout<<"nihao "<<endl;
    }
    return 3;
}
*/
//f=ax+2
TF1* f[3];
TString totalfstr="";
int fnum=3;
int a[3]={1,2,3};
for( int i=0; i<fnum ; i++ )
{
    /*
    stringstream a;
    a<<a[i];

    stringstream ii;
    ii<<i;
    */
    std::cout<<"a[i]  : "<<a[i]<<endl;
    TString fstr="";
    fstr+=a[i];
    fstr+="*x+2";
    TString fname="f";
    fname+=i;
    std::cout<<"fstr  : "<<fstr<<endl;
    std::cout<<"fname  : "<<fname<<endl;
    f[i]= new TF1(fname,fstr,-1,1);
    if( i<fnum-1 )
    {
        totalfstr+=fname+"+";
    }else
    {
        totalfstr+=fname;
    }
}
std::cout<<"totalfstr  : "<<totalfstr<<endl;
TF1* totalf = new TF1("totalf",totalfstr,-1,1);
totalf.Draw();
}
