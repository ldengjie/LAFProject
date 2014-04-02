#include  <iostream>
#include  <fstream>
#include  <string>
void readtxt()
{
	ifstream infile;
	infile.open("pmtDataTable_02.txt",ios::in);
	string a;
	string b[22];
	int count=0;	
	while( infile>>a )
	{
		if( count==22 )
		{
			count=0;
			cout<<b[7]<<endl;//
		}
		b[count]=a;
		count++;
	}
}
