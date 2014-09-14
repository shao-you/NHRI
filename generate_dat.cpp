#include "control.h"
/********************************
This program is to generate dat file of each chromosome.
********************************/

void generate_dat(int** chr, int num_of_chrs)//optimization: 可合併在count_chr_number.cpp
{
	char pattern[CHAR_MAX_LENGTH+1]="";
	ifstream input_map;
	input_map.open(BIM,ios::in);
	//===============================================
	char buffer[50];
	for(int i=0;i<num_of_chrs;i++)
	{
		ofstream output_dat;
		sprintf (buffer, "CHR%d.dat", chr[i][0]);
		//cout<<buffer<<endl;getchar();
		output_dat.open(buffer,ios::out);
		
		output_dat<<"A "<<"disease"<<endl;
		int sz = chr[i][1];
		for(int j=0;j<sz;j++)
		{
			input_map.getline(pattern, CHAR_MAX_LENGTH);
			char* tmp = strtok(pattern, " 	,");
		if(!tmp) break;
			tmp = strtok(NULL, " 	,");
			output_dat<<"M "<<tmp<<endl;
		}
		output_dat.close();
	}
	input_map.close();
}