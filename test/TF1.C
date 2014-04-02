{
    TF1 *f1 = new TF1("func1","4",0,20);
    f1->SetLineColor(10);
    f1->Draw();
    TLine l[8][24];
    for( int i=0 ; i<8 ; i++ )
    {
        for( int j=0 ; j<24 ; j++ )
        {
            l[i][j].SetX1(1);
            l[i][j].SetX2(4);
            l[i][j].SetY1(i+j*0.04);
            l[i][j].SetY2(i+j*0.04);
            l[i][j].SetLineColor(i+1);
            l[i][j].SetLineWidth(1);
            l[i][j].Draw();
        }
        
    }
    TMarker m(4,4,7);
    m.Draw();
}
