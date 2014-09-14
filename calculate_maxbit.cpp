#include "control.h"
/********************************
This program is to check whether the family needs to do pedcut 
inputs: SubPedigrees.csv/PedsSummary.csv/one_cluster_align.fam
outputs: pedcut.fam
tmp file: tmp.ped
********************************/
int find_corresponding(char* ID, map<string, int>& map_ID)
{
	map<string, int>::iterator it;
	it = map_ID.find(ID);
	return it->second;
}

int calculate_maxbit(int num_of_families, int** statistic, vector<int>& people_mapping, 
					map< pair<int,int>, int >& ID_affect, vector<int>& one_cluster_info)
//two times the number of non-founders minus the number of founders
{	
	char pattern[CHAR_MAX_LENGTH+1]="";
	char pattern1[CHAR_MAX_LENGTH+1]="";
	ifstream one_cluster;
	ofstream pedcut_fam, family_pedcut_info;
	
	one_cluster.open("one_cluster.fam",ios::in);
	pedcut_fam.open("pedcut.fam",ios::out);
	family_pedcut_info.open("family_pedcut_info",ios::out);
	
	double threshold = Maxbit/2;
	int start_from = 0;//從哪一個人開始(依順序)
	int new_famID_naming = num_of_families + 1;
	//cout<<"num_of_families"<<num_of_families<<"====";getchar();
	int total_people = 0;
	//at least Maxbit/2 people in a specific family, which may reach the Maxbit value
	//===============================================
	for(int i=1;i<=num_of_families;i++)//同一family會連續出現
	{ 
		int num_member = statistic[i][0];//i == current family
		
		//if((2*statistic[i][1]-(num_member-statistic[i][1])) > Maxbit)
			//cout<<(2*statistic[i][1]-(num_member-statistic[i][1]))<<" "<<i<<endl;
		if(num_member > threshold && 
			(2*statistic[i][1]-(num_member-statistic[i][1])) > Maxbit)//this family needs pedcut
		{
			//未來可用thread同步進行多個pedcut(需處理相同檔名輸出問題)
			streamoff position_start;
			streamoff position_end;
			position_start = one_cluster.tellg();
			
			map<string, int> map_ID;
			map<string, int>::iterator it;
			
			ofstream tmp_ped;
			tmp_ped.open("tmp.ped",ios::out);
			int family_name;			
			for(int j=0;j<num_member;j++)
			{
				one_cluster.getline(pattern, CHAR_MAX_LENGTH);
				tmp_ped<<pattern<<endl;
				
				char* tmp = strtok(pattern, " ");//fam
				family_name = atoi(tmp);
				tmp = strtok(NULL, " ");
				it = map_ID.find(tmp);//actually, ID is without repetition
				if(it == map_ID.end()) map_ID[tmp] = j+1;//record (ID,line)
			}
			family_pedcut_info<<family_name<<": ";
			position_end = one_cluster.tellg();
			tmp_ped.close();

			cout<<"family "<<i<<": NEED PEDCUT!!!"<<endl;//getchar();
			char buffer[50];
			sprintf (buffer, "pedcut -p tmp.ped -m %d", Maxbit);
			system(buffer);
			/*pedcut only supports one family, check control file
			produce many fake families in a file ("SubPedigrees.csv")*/
			//===============================================
			ifstream input_SubPedigrees, input_PedsSummary;
			input_SubPedigrees.open("SubPedigrees.csv",ios::in);
			input_PedsSummary.open("PedsSummary.csv",ios::in);
			if(input_SubPedigrees.is_open()==true && input_PedsSummary.is_open()==true)
			{
				input_PedsSummary.getline(pattern, CHAR_MAX_LENGTH);//first line is information
				while(input_PedsSummary.eof() == false)
				{
					input_PedsSummary.getline(pattern, CHAR_MAX_LENGTH);
					//cout<<pattern<<endl;getchar();
					char* tmp = strtok(pattern, ",");
				if(!tmp) break;
					
					tmp = strtok(NULL, ",");
					int num_people = atoi(tmp);
					total_people += num_people;
					family_pedcut_info<<new_famID_naming<<" ";
					for(int ii=0;ii<num_people;ii++)
					{
						cout<<"===aaa==";
						pedcut_fam<<new_famID_naming<<" ";
						cout<<new_famID_naming<<" ";
						input_SubPedigrees.getline(pattern, CHAR_MAX_LENGTH);
						tmp = strtok(pattern, ",");
						tmp = strtok(NULL, ",");//ID
						pedcut_fam<<tmp<<" ";
						cout<<tmp<<" ";
						int ID = atoi(tmp);
						int corr = find_corresponding(tmp,map_ID);
						people_mapping.push_back(one_cluster_info[start_from+corr-1]);//add the offset
						
						for(int jj=0;jj<4;jj++)
						{
							tmp = strtok(NULL, ",");
							pedcut_fam<<tmp<<" ";
							cout<<tmp<<" ";
						}
						
						int affection = atoi(tmp);
						ID_affect[make_pair(new_famID_naming,ID)] = affection;
						
						cout<<"===bbb==";
						pedcut_fam<<endl;	
						cout<<endl;
					}
					cout<<"=================================="<<endl;
					//cout<<num_people<<" people in this new family"<<endl;getchar();
					new_famID_naming++;
				}
				start_from += num_member;
				input_SubPedigrees.close();
				input_PedsSummary.close();
				system("rm -rf SubPedigrees.csv");
				system("rm -rf PedsSummary.csv");
				
				one_cluster.clear();
				one_cluster.seekg(position_end);
			}
			else 
			{
				//goto pedcut_is_futile;
				start_from += num_member;
				one_cluster.clear();
				one_cluster.seekg(position_end);
			}
			family_pedcut_info<<endl;
		}
		else//no need pedcut
		{
		pedcut_is_futile:
			for(int j=0;j<num_member;j++)
			{
				one_cluster.getline(pattern, CHAR_MAX_LENGTH);
				pedcut_fam<<pattern<<endl;
				
				people_mapping.push_back(one_cluster_info[start_from]);
				start_from++;
				
				char* tmp = strtok(pattern, " ");//fam
				int fam = atoi(tmp);
				tmp = strtok(NULL, " ");//ID
				int ID = atoi(tmp);
				for(int k=0;k<4;k++) tmp = strtok(NULL, " ");//affection
				int affection = atoi(tmp);
				ID_affect[make_pair(fam,ID)] = affection;
			}
			total_people += num_member;
		}
	}
	pedcut_fam.close();
	one_cluster.close();
	family_pedcut_info.close();
	system("rm -rf tmp.ped");
	return total_people;
}
