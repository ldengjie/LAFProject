{
    gROOT->ProcessLine("#include <iomanip.h>;");
    double a=12345.3496;
    cout<<setprecision(2)<<setiosflags(ios::fixed)<<a<<endl;

    std::cout<<"a*100 "<<a*100<<endl;
    std::cout<<"((int)(a*100))  : "<<((int)(a*100))<<endl;
    std::cout<<"(double)((int)(a*100))  : "<<(double)((int)(a*100))<<endl;
    cout<<(double)((int)(a*100))/100<<endl;

    std::cout<<"********************"<<endl;
    double amount = 22.0/7;
    cout <<amount <<endl;
    cout <<setprecision(0) <<setiosflags(ios::fixed)<<amount <<endl
         <<setprecision(1) <<setiosflags(ios::fixed)<<amount <<endl
         <<setprecision(2) <<setiosflags(ios::fixed)<<amount <<endl
         <<setprecision(3) <<setiosflags(ios::fixed)<<amount <<endl
         <<setprecision(4) <<setiosflags(ios::fixed)<<amount <<endl;

    cout <<setiosflags(ios::fixed);
    cout <<setprecision(8) <<amount <<endl;

    cout <<setiosflags(ios::scientific) 
         <<amount <<endl;

    cout <<setprecision(6); 
}
