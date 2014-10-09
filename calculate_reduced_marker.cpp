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
	
	map<string, int> name_seq;
	map<string, int>::iterator it;
	
	int which_marker = 1;
	while(original_dat.eof() == false)
	{
		original_dat.getline(pattern, CHAR_MAX_LENGTH+1);
		char* tmp = strtok(pattern, " ");
	if(!tmp) break;	
		string marker_name = strtok(NULL, " ");
		name_seq[marker_name] = which_marker;
		which_marker++;
	}
	
	while(infer_dat.eof() == false)
	{
		infer_dat.getline(pattern, CHAR_MAX_LENGTH+1);
		char* tmp = strtok(pattern, " ");
	if(!tmp) break;	
		string marker_name = strtok(NULL, " ");
		name_seq.erase(marker_name);
	}
	
	for (it=name_seq.begin(); it!=name_seq.end(); ++it) index_reduced_marker.push_back(it->second);
	infer_dat.close();
	original_dat.close();
}