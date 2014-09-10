#include "control.h"

void compare_genotype(vector<char>& result, int line, ifstream& infer_ped)
{
	char pattern[CHAR_MAX_LENGTH+1]="";
	for(int i=0;i<line;i++) infer_ped.getline(pattern, CHAR_MAX_LENGTH+1);//buffer size may overflow ?
	
	if(result.size() == 0)//first time 
	{	
		char* tmp = strtok(pattern, " ");
		while(tmp)
		{
			result.push_back(tmp[0]);
			tmp = strtok(NULL, " ");
		}
	}
	else
	{
		int index = 0;
		char* tmp = strtok(pattern, " ");
		while(tmp)
		{
			if(result[index] != tmp[0]) result[index] = '.';//., A, T, C, G, 1, 2
			tmp = strtok(NULL, " ");
			index++;
		}
	}
}
void restore_ped(vector<int>* mapping, int chr, int num_marker)
{
	char pattern[CHAR_MAX_LENGTH+1]="";
	ifstream infer_ped, original_fam, input_ped,
			 infer_dat, original_dat;//CHR3_infer.ped, all_digit.fam
	ofstream restore_ped;//restore_chr3.ped
	
	char buffer[50];
	//計算少了哪些marker
		sprintf (buffer, "CHR%d_infer.dat", chr);
	infer_dat.open(buffer,ios::in);
		sprintf (buffer, "CHR%d.dat", chr);
	original_dat.open(buffer,ios::in);
	
	vector<int> index_reduced_marker;
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
	
	//把infer_ped內容與original fam結合
		sprintf (buffer, "CHR%d_infer.ped", chr);
	infer_ped.open(buffer,ios::in);
	original_fam.open("all_digit.fam",ios::in);//not aligned
		sprintf (buffer, "metadata%d.ped", chr);
	input_ped.open(buffer,ios::in);
		sprintf (buffer, "restore_chr%d.ped", chr);
	restore_ped.open(buffer,ios::out);
	
	streamoff position_start2 = input_ped.tellg();
	streamoff position_start3 = infer_ped.tellg();
	
	const int total_people_after_pedcut = (*mapping).size();
	int which_line = 1;
	while(original_fam.eof() == false)//an original person each time
	{
		original_fam.getline(pattern, CHAR_MAX_LENGTH+1);//until CHAR_MAX_LENGTH byte
		restore_ped<<pattern;
		char* tmp = strtok(pattern, " ");
	if(!tmp) break;	
		vector<char> result;
		for(int i=0;i<total_people_after_pedcut;i++) 
			if((*mapping)[i] == which_line) compare_genotype(result,i+1,infer_ped);
		
		if(result.size() == 0)//依照原來內容填入(不存在於pedcut fam的人)
		{
			int num_ahead_char = (which_line-1)*4;
			for(int j=0;j<num_marker;j++)
			{
				input_ped.getline(pattern, CHAR_MAX_LENGTH+1);
				restore_ped<<pattern[num_ahead_char]<<" "<<pattern[num_ahead_char+2]<<" ";
			}
			input_ped.clear();
			input_ped.seekg(position_start2);
		}
		else
		{
			int reduced_num = index_reduced_marker.size();
			for(int i=0;i<reduced_num;i++) 
			{
				int offset = (index_reduced_marker[i]-1)*2;
				vector<char>::iterator it = result.begin();
				result.insert(it+offset, 2, '0');
			}
			int size = result.size();
			for(int i=0;i<size;i++) 
			{
				if(result[i] == '.') restore_ped<<"0 ";
				else restore_ped<<result[i]<<" ";
			}
		}
		restore_ped<<endl;
		infer_ped.clear();
		infer_ped.seekg(position_start3);
		which_line++;
	}
	infer_ped.close();
	original_fam.close();
	input_ped.close();
	restore_ped.close();
}
