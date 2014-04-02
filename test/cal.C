{
    int geshu=20;
    double fenzi[geshu+1],fenmu[geshu+1];
    fenzi[0]=1.;
    fenmu[0]=2.;
    for( int i=1 ; i<geshu ; i++ )
    {
        fenzi[i]=fenzi[i-1]+fenmu[i-1];
        fenmu[i]=fenzi[i-1];
    }
    double gongbeishu=1.;
    for( int i=0 ; i<geshu ; i++ )
    {
        bool xiaoqu=false;
        for( int j=i+1 ; j<geshu+1 ; j++ )
        {
            if( (fenmu[j]/fenmu[i]-(int)(fenmu[j]/fenmu[i]))>0 )
            {
                xiaoqu=true;
            }
        }
        if( !xiaoqu )
        {
            gongbeishu=gongbeishu*fenmu[i];
        }
    }
    double totalfenmu=gongbeishu;
    double totalfenzi;
    for( int i=0 ; i<geshu ; i++ )
    {
        totalfenzi+=totalfenmu/fenzi[i];
    }
    std::cout<<"result : "<<totalfenzi<<"/"<<totalfenmu <<endl;
}
