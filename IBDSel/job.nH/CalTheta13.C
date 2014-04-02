{
    double DaqTime[3][4]={{3061.1*3600,3061.1*3600},{3057.1*3600},{3030.5*3600,3030.5*3600,3030.5*3600}};

    double Num_Ibd[3][4]={{},{},{}};
    double Epsilon_mu[3][4]={{},{},{}};
    double Epsilon_multi[3][4]={{},{},{}};
    double Rate_Acc[3][4]={{},{},{}};
    double Err_Acc[3][4]={{},{},{}};
    double Rate_Fn[3][4]={{},{},{}};
    double Err_Fn[3][4]={{},{},{}};
    double Rate_HeLi[3][4]={{},{},{}};
    double Err_HeLi[3][4]={{},{},{}};
    double Rate_AmC[3][4]={{},{},{}};
    double Err_AmC[3][4]={{},{},{}};
    double Rate_AlphaN[3][4]={{},{},{}};
    double Err_AlphaN[3][4]={{},{},{}};
    double Rate_Ibd[3][4]={{0},{0},{0}};
    double Err_Ibd[3][4]={{0},{0},{0}};

    double Num_Acc[3][4]={{},{},{}};
    double Num_Fn[3][4]={{},{},{}};
    double Num_HeLi[3][4]={{},{},{}};
    double Num_AmC[3][4]={{},{},{}};
    double Num_AlphaN[3][4]={{},{},{}};
    //double [3][4]={{},{},{}};

    for( int i=0 ; i<3 ; i++ )
    {
        for( int j=0 ; j<4 ; j++ )
        {
            Rate_Ibd[i][j]=Num_Ibd[i][j]/(DaqTime[i][j]*Epsilon_mu[i][j]*Epsilon_multi[i][j])-Rate_Acc[i][j]-Rate_Fn[i][j]-Rate_HeLi[i][j]-Rate_AmC[i][j]-Rate_AlphaN[i][j];
            Err_Ibd[i][j]=sqrt(Num_Ibd[i][j]/(DaqTime[i][j]*Epsilon_mu[i][j]*Epsilon_multi[i][j]*DaqTime[i][j]*Epsilon_mu[i][j]*Epsilon_multi[i][j])+Err_Acc[i][j]*Err_Acc[i][j]+Err_Fn[i][j]*Err_Fn[i][j]+Err_HeLi[i][j]*Err_HeLi[i][j]+Err_AmC[i][j]*Err_AmC[i][j]+Err_AlphaN[i][j]*Err_AlphaN[i][j]);
        }
        
    }
    
}
