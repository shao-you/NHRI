#include "control.h"

void calculate_reduced_marker(int chr, vector<int>& index_reduced_marker)
{
	char pattern[CHAR_MAX_LENGTH+1]="";
	ifstream infer_dat, original_dat;
	char buffer[50];
	//計算少了哪些marker
		sprintf (buffer, "dir_%d/CHR%d_infer.dat", chr, chr);
	infer_dat.open(buffer,ios::in);
		sprintf (buffer, "CHR%d.dat", chr);
	original_dat.open(buffer,ios::in);
	//if(infer_dat.is_open()) cout<<"OK1"<<endl;
	//if(original_dat.is_open()) cout<<"OK2"<<endl;
	
	original_dat.getline(pattern, CHAR_MAX_LENGTH+1);//ignore first line
	infer_dat.getline(pattern, CHAR_MAX_LENGTH+1);//ignore first line
	streamoff position_start1 = infer_dat.tellg();
	int which_marker = 1;
	while(original_dat.eof() == false)
	{
		original_dat.getline(pattern, CHAR_MAX_LENGTH+1);
		char* tmp = strtok(pattern, " ");
	if(!tmp) break;	
		string marker_name1 = strtok(NULL, " ");
		
		bool find_or_not = false;
		while(infer_dat.eof() == false)
		{
			infer_dat.getline(pattern, CHAR_MAX_LENGTH+1);
			char* tmp = strtok(pattern, " ");
		if(!tmp) break;		
			string marker_name2 = strtok(NULL, " ");
			if(marker_name1.compare(marker_name2) == 0) 
			{
				position_start1 = infer_dat.tellg();//store position
				find_or_not=true;
				break;
			}
		}
		if(find_or_not == false) 
		{
			index_reduced_marker.push_back(which_marker);
			infer_dat.clear();
			infer_dat.seekg(position_start1);
		}
		which_marker++;
	}
	infer_dat.close();
	original_dat.close();
}