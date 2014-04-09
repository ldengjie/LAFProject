{
    gROOT->ProcessLine("#include <iomanip.h>;");
    string dataVer="P14A";
    int ADNUM;
    if( dataVer=="P12E" ||dataVer=="P12A"||dataVer=="P12B"||dataVer=="P12C")
    {
        ADNUM=6;
    }else
    {
        ADNUM=8;
    }

    double outStr[49][8];
    ifstream infile4out; 
    string infileoutName=dataVer+"/result_temp_"+dataVer+".txt";
    infile4out.open(infileoutName.c_str(),ios::in);
    int inLineNum4out=0;
    while(inLineNum4out<49)
    {
        for( int j=0 ; j<8 ; j++ )
        {
            infile4out>>outStr[inLineNum4out][j];
        }
        inLineNum4out++;
    }
    infile4out.close();

    ofstream outfile4out;
    string outfileoutName=dataVer+"/result_"+dataVer+"_LatexTable.txt";
    outfile4out.open(outfileoutName.c_str());

    if( ADNUM==8 )
    {
        outfile4out<<""<<endl;
        outfile4out<<"IBD for 8ADs "<<endl;
        outfile4out<<" "<<endl;
        outfile4out<<"    \\begin{tabular}{c|p{22pt}|p{22pt}|p{22pt}|p{22pt}|p{22pt}|p{22pt}|p{22pt}|p{22pt}} "<<endl;
        outfile4out<<"     \\toprule "<<endl;
        outfile4out<<"     \\textcolor{red}{"<< dataVer<<"} & AD1 & AD2 & AD3 &AD8& AD4 & AD5 & AD6&AD7 \\\\ "<<endl;
        outfile4out<<"     \\hline "<<endl;
        outfile4out<<"      IBD candidates &"<<outStr[1][0] <<"&"<<outStr[1][1] <<"&"<<outStr[1][2] <<"&"<<outStr[1][3] <<"&"<<outStr[1][4] <<"&"<<outStr[1][5] <<"&"<<outStr[1][6] <<"&"<<outStr[1][7] <<"\\\\ "<<endl;
        outfile4out<<"     \\hline "<<endl;
        outfile4out<<"      DAQ live time(day) &"<<outStr[2][0] <<"&"<<outStr[2][1] <<"&"<<outStr[2][2]<<"&"<<outStr[2][3]<<"&"<<outStr[2][4] <<"&"<<outStr[2][5]<<"&"<<outStr[2][6]<<"&"<<outStr[2][7] <<" \\\\ "<<endl;
        outfile4out<<"     \\hline "<<endl;
        outfile4out<<"      $\\varepsilon_{\\mu}$ "<<"&"<<setprecision(4)<<setiosflags(ios::fixed)<<outStr[3][0] <<"&"<<outStr[3][1] <<"&"<<outStr[3][2] <<"&"<<outStr[3][3] <<"&"<<outStr[3][4] <<"&"<<outStr[3][5] <<"&"<<outStr[3][6] <<"&"<<outStr[3][7] <<"\\\\ "<<endl;
        //outfile4out<<"     \\hline "<<endl;
        //outfile4out<<"      $\\varepsilon_m$"<<"&"<<outStr[4][0] <<"&"<<outStr[4][1] <<"&"<<outStr[4][2] <<"&"<<outStr[4][3] <<"&"<<outStr[4][4] <<"&"<<outStr[4][5] <<"&"<<outStr[4][6] <<"&"<<outStr[4][7] <<"\\\\ "<<endl;
        //outfile4out<<"     \\hline "<<endl;
        //outfile4out<<"      AccRate(/day)"<<"&"<<setprecision(2)<<setiosflags(ios::fixed)<<outStr[7][0] <<"$\\pm$"<<outStr[8][0] <<"&"<<outStr[7][1] <<"$\\pm$"<<outStr[8][1] <<"&"<<outStr[7][2] <<"$\\pm$"<<outStr[8][2] <<"&"<<outStr[7][3] <<"$\\pm$"<<outStr[8][3] <<"&"<<outStr[7][4] <<"$\\pm$"<< outStr[8][4]<<"&"<<outStr[7][5] <<"$\\pm$"<<outStr[8][5] <<"&"<<outStr[7][6] <<"$\\pm$"<<outStr[8][6] <<"&"<<outStr[7][7] <<"$\\pm$"<<outStr[8][7] <<"\\\\ "<<endl;
        outfile4out<<"     \\bottomrule "<<endl;
        outfile4out<<"    \\end{tabular} "<<endl;

        outfile4out<<""<<endl;
        outfile4out<<"fast neutron for 8ADs"<<endl;
        outfile4out<<""<<endl;
        outfile4out<<"    \\begin{tabular}{ c|c|c|c|c|c|c|c|c} "<<endl;
        outfile4out<<"      \\toprule "<<endl;
        outfile4out<<"      \\textcolor{red}{"<< dataVer<<"} & AD1 & AD2 & AD3 & AD8 & AD4 & AD5 & AD6 & AD7  \\\\ "<<endl;
        outfile4out<<"      \\hline "<<endl;
        outfile4out<<"      total number & "<<endl;
        outfile4out<<"       \\multicolumn{2}{c|}{"<<setprecision(2)<<setiosflags(ios::fixed)<<outStr[13][0] <<"$\\pm$"<<outStr[14][0] <<"} & "<<endl;
        outfile4out<<"       \\multicolumn{2}{c|}{"<<outStr[13][2] <<"$\\pm$"<<outStr[14][2] <<"} & "<<endl;
        outfile4out<<"       \\multicolumn{4}{c}{"<<outStr[13][4]<<"$\\pm$"<<outStr[14][4] <<" } \\\\ "<<endl;
        outfile4out<<"      \\hline "<<endl;
        outfile4out<<"      B/S & "<<endl;
        outfile4out<<"       \\multicolumn{2}{c|}{"<<setprecision(5)<<outStr[15][0] <<"} & "<<endl;
        outfile4out<<"       \\multicolumn{2}{c|}{ "<<outStr[15][2] <<"}& "<<endl;
        outfile4out<<"       \\multicolumn{4}{c}{"<<outStr[15][4] <<"} \\\\ "<<endl;
        outfile4out<<"      \\hline "<<endl;
        outfile4out<<"      Rate(/day) & "<<endl;
        outfile4out<<"       \\multicolumn{2}{c|}{"<<setprecision(2)<<setiosflags(ios::fixed)<<outStr[17][0] <<"$\\pm$"<<outStr[18][0] <<"} & "<<endl;
        outfile4out<<"       \\multicolumn{2}{c|}{"<<outStr[17][2] <<"$\\pm$"<<outStr[18][2] <<"}& "<<endl;
        outfile4out<<"       \\multicolumn{4}{c}{"<<outStr[17][4] <<"$\\pm$"<<outStr[18][4]<<"} \\\\ "<<endl;
        outfile4out<<"      \\bottomrule "<<endl;
        outfile4out<<"    \\end{tabular} "<<endl;

        outfile4out<<""<<endl;
        outfile4out<<"He8/Li9 for 8ADs"<<endl;
        outfile4out<<""<<endl;
        outfile4out<<"    \\begin{tabular}{ c|c|c|c|c|c|c|c|c} "<<endl;
        outfile4out<<"      \\toprule "<<endl;
        outfile4out<<"      \\textcolor{red}{"<< dataVer<<"} & AD1 & AD2 & AD3 & AD8 & AD4 & AD5 & AD6 & AD7  \\\\ "<<endl;
        outfile4out<<"      \\hline "<<endl;
        outfile4out<<"      total number & "<<endl;
        outfile4out<<"       \\multicolumn{2}{c|}{"<<setprecision(2)<<setiosflags(ios::fixed)<<outStr[19][0] <<"$\\pm$"<<outStr[20][0] <<"} & "<<endl;
        outfile4out<<"       \\multicolumn{2}{c|}{"<<outStr[19][2] <<"$\\pm$"<<outStr[20][2] <<"} & "<<endl;
        outfile4out<<"       \\multicolumn{4}{c}{"<<outStr[19][4]<<"$\\pm$"<<outStr[20][4] <<" } \\\\ "<<endl;
        outfile4out<<"      \\hline "<<endl;
        outfile4out<<"      Rate(/day) & "<<endl;
        outfile4out<<"       \\multicolumn{2}{c|}{"<<setprecision(2)<<setiosflags(ios::fixed)<<outStr[21][0] <<"$\\pm$"<<outStr[22][0] <<"} & "<<endl;
        outfile4out<<"       \\multicolumn{2}{c|}{"<<outStr[21][2] <<"$\\pm$"<<outStr[22][2] <<"}& "<<endl;
        outfile4out<<"       \\multicolumn{4}{c}{"<<outStr[21][4] <<"$\\pm$"<<outStr[22][4]<<"} \\\\ "<<endl;
        outfile4out<<"      \\bottomrule "<<endl;
        outfile4out<<"    \\end{tabular} "<<endl;
    }

    if( ADNUM==6 )
    {

        outfile4out<<""<<endl;
        outfile4out<<"fast neutron for 6ADs"<<endl;
        outfile4out<<""<<endl;
        outfile4out<<"    \\begin{tabular}{ c|c|c|c|c|c|c} "<<endl;
        outfile4out<<"      \\toprule "<<endl;
        outfile4out<<"      \\textcolor{red}{"<< dataVer<<"} & AD1 & AD2 & AD3 & AD4 & AD5 & AD6 \\\\ "<<endl;
        outfile4out<<"      \\hline "<<endl;
        outfile4out<<"      total number & "<<endl;
        outfile4out<<"       \\multicolumn{2}{c|}{"<<setprecision(2)<<setiosflags(ios::fixed)<<outStr[13][0] <<"$\\pm$"<<outStr[14][0] <<"} & "<<endl;
        outfile4out<<"                            "<<outStr[13][2] <<"$\\pm$"<<outStr[14][2] <<" & "<<endl;
        outfile4out<<"       \\multicolumn{3}{c}{"<<outStr[13][4]<<"$\\pm$"<<outStr[14][4] <<" } \\\\ "<<endl;
        outfile4out<<"      \\hline "<<endl;
        outfile4out<<"      B/S & "<<endl;
        outfile4out<<"       \\multicolumn{2}{c|}{"<<setprecision(5)<<outStr[15][0] <<"} & "<<endl;
        outfile4out<<"                         "<<outStr[15][2] <<"& "<<endl;
        outfile4out<<"       \\multicolumn{3}{c}{"<<outStr[15][4] <<"} \\\\ "<<endl;
        outfile4out<<"      \\hline "<<endl;
        outfile4out<<"      Rate(/day) & "<<endl;
        outfile4out<<"       \\multicolumn{2}{c|}{"<<setprecision(2)<<setiosflags(ios::fixed)<<outStr[17][0] <<"$\\pm$"<<outStr[18][0] <<"} & "<<endl;
        outfile4out<<"                           "<<outStr[17][2] <<"$\\pm$"<<outStr[18][2] <<"& "<<endl;
        outfile4out<<"       \\multicolumn{3}{c}{"<<outStr[17][4] <<"$\\pm$"<<outStr[17][4]<<"} \\\\ "<<endl;
        outfile4out<<"      \\bottomrule "<<endl;
        outfile4out<<"    \\end{tabular} "<<endl;

        outfile4out<<""<<endl;
        outfile4out<<"fast neutron for 6ADs (with Rpc Muon Veto)"<<endl;
        outfile4out<<""<<endl;

        int startline=40;
        outfile4out<<"    \\begin{tabular}{c|c|c|c|c|c|c} "<<endl;
        outfile4out<<"      \\toprule "<<endl;
        outfile4out<<"          \\textcolor{red}{with RPC muon veto} & AD1 & AD2 & AD3 & AD4 & AD5 & AD6 \\\\ "<<endl;
        outfile4out<<"                \\hline "<<endl;
        outfile4out<<"                  \\multirow{2}{*}{total number} &  "<<endl;
        outfile4out<<"                  \\multicolumn{2}{c|}{"<<outStr[startline][0] <<" $\\pm$ "<<outStr[startline+1][0] <<"} &  "<<endl;
        outfile4out<<"                  "<<outStr[startline][2] <<" $\\pm$ "<<outStr[startline+1][2] <<" &  "<<endl;
        outfile4out<<"                  \\multicolumn{3}{c}{"<<outStr[startline][4] <<" $\\pm$ "<<outStr[startline+1][4] <<"} \\\\ "<<endl;
        outfile4out<<"                \\cline{2-7}&  "<<endl;
        outfile4out<<"                  \\multicolumn{2}{c|}{\\textcolor{red}{"<<outStr[startline+2][0]*100 <<"\\%}} &  "<<endl;
        outfile4out<<"                  \\textcolor{red}{"<<outStr[startline+2][2]*100 <<"\\%} &  "<<endl;
        outfile4out<<"                  \\multicolumn{3}{c}{\\textcolor{red}{"<<outStr[startline+2][4]*100 <<"\\%}} \\\\ "<<endl;
        outfile4out<<"                \\hline "<<endl;
        outfile4out<<"                  \\multirow{2}{*}{B/S} &  "<<endl;
        outfile4out<<"                  \\multicolumn{2}{c|}{"<<setprecision(5)<<setiosflags(ios::fixed)<<outStr[startline+3][0] <<"} &  "<<endl;
        outfile4out<<"                  "<<outStr[startline+3][2] <<" &  "<<endl;
        outfile4out<<"                  \\multicolumn{3}{c}{"<<outStr[startline+3][4] <<"} \\\\ "<<endl;
        outfile4out<<"                \\cline{2-7}&  "<<endl;
        outfile4out<<"                  \\multicolumn{2}{c|}{\\textcolor{red}{"<<setprecision(2)<<setiosflags(ios::fixed)<<outStr[startline+5][0]*100 <<"\\%}} &  "<<endl;
        outfile4out<<"                  \\textcolor{red}{"<<outStr[startline+5][2]*100 <<"\\%} &  "<<endl;
        outfile4out<<"                  \\multicolumn{3}{c}{\\textcolor{red}{"<<outStr[startline+5][4]*100 <<"\\%}} \\\\ "<<endl;
        outfile4out<<"                \\hline "<<endl;
        outfile4out<<"                  \\multirow{2}{*}{Rate(/day)} &  "<<endl;
        outfile4out<<"                  \\multicolumn{2}{c|}{"<<outStr[startline+6][0] <<"$\\pm$ "<<outStr[startline+7][0] <<"} &  "<<endl;
        outfile4out<<"                  "<<outStr[startline+6][2] <<" $\\pm$ "<<outStr[startline+7][2] <<" &  "<<endl;
        outfile4out<<"                  \\multicolumn{3}{c}{"<<outStr[startline+6][4] <<" $\\pm$ "<<outStr[startline+6][4] <<"} \\\\ "<<endl;
        outfile4out<<"                \\cline{2-7}&  "<<endl;
        outfile4out<<"                  \\multicolumn{2}{c|}{\\textcolor{red}{"<<outStr[startline+8][0]*100 <<"\\%}} &  "<<endl;
        outfile4out<<"                  \\textcolor{red}{"<<outStr[startline+8][2]*100 <<"\\%} &  "<<endl;
        outfile4out<<"                  \\multicolumn{3}{c}{\\textcolor{red}{"<<outStr[startline+8][4]*100 <<"\\%}} \\\\ "<<endl;
        outfile4out<<"      \\bottomrule "<<endl;
        outfile4out<<"    \\end{tabular} "<<endl;

    }

}

