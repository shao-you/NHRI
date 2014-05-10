#include "control.h"
/********************************
This program is to adjust the fam file. Check whether the first 4 columns are digital or not.
Map<string, int> is used to store the mapping between STRING and assigned INT.
********************************/
bool check_digit(const char* buffer)
{
	bool flag = true;
	int length = strlen(buffer);
	for(int i=0; i!=length; i++)
	{
		if(!isdigit(buffer[i]))
		{ // 發現不是數字字元的話馬上跳出來
			flag = false;
			break;
		}
	}
	return flag;
}
size_t hash_value(const char* buffer)//char* -> string -> size_t
{
	string str1(buffer);
	std::tr1::hash<string> str_hash;
	//max 500000
	size_t value = str_hash(str1);
	return value;
}
size_t hash_value_digit(const size_t vle)//size_t -> string -> size_t
{
	stringstream str;
	str << vle;
	std::tr1::hash<string> str_hash;
	//max 500000
	size_t value = str_hash(str.str());
	return value;
}
int do_map(int** statistic, vector<int>* align_info)//return # of families
{
	char pattern[CHAR_MAX_LENGTH+1]="";
	
	ifstream input_fam;
	ofstream all_digital_fam;
	input_fam.open(FAM,ios::in);
	all_digital_fam.open("all_digital.fam",ios::out);
	//T2DG0800561 T2DG0800561 0 0 0 -9 
	
	map<string, int> map_family;
	map_family["0"] = 0;//不考慮family ID為0
	map<string, int>::iterator it_fam;
	
	map<string, int> map_ID;
	map_ID["0"] = 0;
	map<string, int>::iterator it;

	//===============================================
	int index_count_fam = 1, index_count = 1;
	int line = 0;
	while(input_fam.eof() == false)
	{
		int current_family;
		
		input_fam.getline(pattern, CHAR_MAX_LENGTH);
		char* tmp = strtok(pattern, " 	");
	if(!tmp) break;
		line++;
		//string str(tmp);
		it_fam = map_family.find(tmp);
		if(it_fam == map_family.end()) 
		{	
			current_family = index_count_fam;
			statistic[current_family] = new int [2];
			for(int j=0;j<2;j++) statistic[current_family][j] = 0;//initialization
		//align_info[current_family].push_back(0);//用0區隔founder/non-founder, --version1
			map_family[tmp] = index_count_fam;
			all_digital_fam<<index_count_fam<<" ";
			index_count_fam++;
		}
		else 
		{
			current_family = it_fam->second;
			all_digital_fam<<it_fam->second<<" ";
		}
		statistic[current_family][0]++;//add a member to the family	
		
		for(int i=0;i<1;i++)//ID
		{
			tmp = strtok(NULL, " 	");
			//str = tmp;
			it = map_ID.find(tmp);
			if(it == map_ID.end()) 
			{	
				map_ID[tmp] = index_count;
				all_digital_fam<<index_count<<" ";
				index_count++;
			}
			else all_digital_fam<<it->second<<" ";
		}
		
		int parents[2];
		for(int i=0;i<2;i++)//father/mother
		{
			tmp = strtok(NULL, " 	");
			//str = tmp;
			it = map_ID.find(tmp);
			if(it == map_ID.end()) 
			{	
				map_ID[tmp] = index_count;
				parents[i] = index_count;
				all_digital_fam<<index_count<<" ";
				index_count++;
			}
			else 
			{
				parents[i] = it->second;
				all_digital_fam<<it->second<<" ";
			}
		}
		if(parents[0]==0 && parents[1]==0)//founder
		{
		//vector<int>::iterator it = align_info[current_family].begin();//--version1
		//align_info[current_family].insert(it,line);//insert to the beginning, --version1
		align_info[current_family].push_back(line);//--version2
		}
		else 
		{
			statistic[current_family][1]++;//add a non-founder for the family
		//align_info[current_family].push_back(line);//store the location of "line", --version1
		align_info[current_family].push_back(line);//--version2
		}
		
		for(int i=0;i<2;i++)//gender/affection
		{
			tmp = strtok(NULL, " 	");
			if(atoi(tmp) < 0) all_digital_fam<<0<<" ";
			else all_digital_fam<<tmp<<" ";
			//getchar();
		}
		all_digital_fam<<endl;
		//all_digital_fam<<tmp+strlen(tmp)+1<<endl;
	}
	//===============================================
	//cout<<line<<"=============";getchar();
	input_fam.close();
	all_digital_fam.close();
	return index_count_fam-1;
}
