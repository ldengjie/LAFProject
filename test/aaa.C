#include  <iostream>
#include  <fstream>
#include  <string>
void aaa()
{
	int loop=0;
	double pmt[198];
	double gain[198];
	ifstream infile;
	infile.open("pmtDataTable_02.txt",ios::in);
	string a;
	string b[22];
	int count=0;	
	while( infile>>a )
	{
		if( count==22 )
		{
			loop++;
			count=0;
			pmt[loop]=loop;	
			cout<<b[7]<<endl;//
			gain[loop]=atof(b[7].c_str());
		}
		b[count]=a;
		count++;
	}
cout<<loop<<endl;
cout<<pmt[15]<<endl;
cout<<gain[15]<<endl;
//gr   = new TGraph(198,pmt,gain);
//gr->Draw();


}
