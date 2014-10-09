#include "control.h"

void compare_genotype(vector<char>& result, int line, ifstream& infer_ped)
{
	int flag;
	if(result.size() == 0) flag = 0;//first time 
	else flag = 1;
	
	char pattern[CHAR_MAX_LENGTH+1]="";
	for(int i=0;i<line-1;i++)
	{
		do{
			infer_ped.clear();
			infer_ped.getline(pattern, CHAR_MAX_LENGTH+1);
		}while((infer_ped.rdstate() & std::ifstream::failbit ) != 0);
	}
	{//targeted line
		int index = 0;
		int round = 1;
		do{
			infer_ped.clear();
			infer_ped.getline(pattern, CHAR_MAX_LENGTH+1);
			if(round == 1)
			{
				char* tmp = strtok(pattern, " ");
				for(int i=0;i<6;i++) tmp = strtok(NULL, " ");
				while(tmp)
				{
					if(flag == 0) result.push_back(tmp[0]);
					else//flag == 1 
					{
						if(result[index] != tmp[0]) result[index] = '.';//., A, T, C, G, 1, 2
						index++;
					}
					tmp = strtok(NULL, " ");
				}
				round++;
			}
			else
			{
				char* tmp = strtok(pattern, " ");
				while(tmp)
				{
					if(flag == 0) result.push_back(tmp[0]);
					else//flag == 1  
					{
						if(result[index] != tmp[0]) result[index] = '.';//., A, T, C, G, 1, 2
						index++;						
					}
					tmp = strtok(NULL, " ");
				}
			}
		}while((infer_ped.rdstate() & std::ifstream::failbit ) != 0);
		//The failbit flag is set if the function extracts no characters, or if the delimiting 
		//character is not found once (n-1) characters have already been written to s.
	}
}
void restore_ped(vector<int>* mapping, int chr, int num_marker, vector<int>& infer_marker_mapping)
{
	char pattern[CHAR_MAX_LENGTH+1]="";
	ifstream infer_ped, original_fam, input_ped;//CHR3_infer.ped, all_digital.fam
	ofstream restore_ped;//restore_chr3.ped
	
	char buffer[50];
	//把infer_ped內容與original fam結合
		sprintf (buffer, "dir_%d/CHR%d_infer.ped", chr, chr);
	infer_ped.open(buffer,ios::in);
	original_fam.open("all_digital.fam",ios::in);//not aligned
		sprintf (buffer, "metadata%d.ped", chr);
	input_ped.open(buffer,ios::in);
		sprintf (buffer, "dir_%d/restore_chr%d.ped", chr, chr);
	restore_ped.open(buffer,ios::out);
	//if(infer_ped.is_open()) cout<<"OK3"<<endl;
	//if(original_fam.is_open()) cout<<"OK4"<<endl;
	//if(restore_ped.is_open()) cout<<"OK5"<<endl;
	streamoff position_start2 = input_ped.tellg();
	streamoff position_start3 = infer_ped.tellg();
	
	const int total_people_after_pedcut = (*mapping).size();
	int which_line = 1;
	while(original_fam.eof() == false)//an original person each time
	{
		//cout<<which_line<<endl;
		original_fam.getline(pattern, CHAR_MAX_LENGTH+1);//until CHAR_MAX_LENGTH byte
		//restore_ped<<pattern;
		char* tmp = strtok(pattern, " ");
	if(!tmp) break;	
		vector<char> result;
		for(int i=0;i<total_people_after_pedcut;i++) 
			if((*mapping)[i] == which_line) 
			{
				compare_genotype(result,i+1,infer_ped);
				infer_ped.clear();
				infer_ped.seekg(position_start3);
			}
		
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
			const int total_allele_num = 2*num_marker;
			char reorder_result[total_allele_num];
			for(int i=0;i<total_allele_num;i++) reorder_result[i]='0';
			
			int SZ = infer_marker_mapping.size();
			if(infer_marker_mapping.size()*2 != result.size()) cout<<"some errors occur!!"<<endl;
			for(int i=0;i<SZ;i++) 
			{
				int position = (infer_marker_mapping[i]-1)*2;
				reorder_result[position] = result[2*i];
				reorder_result[position+1] = result[2*i+1];
			}
			
			for(int i=0;i<total_allele_num;i++) 
			{
				if(reorder_result[i] == '.') restore_ped<<"0 ";
				else restore_ped<<reorder_result[i]<<" ";
			}
		}
		restore_ped<<endl;
		which_line++;
	}
	infer_ped.close();
	original_fam.close();
	input_ped.close();
	restore_ped.close();
}
/*int main()
{
	int size = 2060;
	srand(time(NULL));
	
	vector<int> mapping;
	for(int i=0;i<size;i++) mapping.push_back((rand()%size)+1);
	
	vector<int> index_reduced_marker;
	vector<int> infer_marker_mapping;
	calculate_infer_marker_info(1,index_reduced_marker,infer_marker_mapping);
	restore_ped(&mapping,1,2793,infer_marker_mapping);
}*/