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

int calculate_maxbit(int num_of_families, int** statistic, vector<int>& people_mapping, map< pair<int,int>, int >& ID_affect,
					vector<int>* align_info, vector<int>& one_cluster_info)
//two times the number of non-founders minus the number of founders
{	
	char pattern[CHAR_MAX_LENGTH+1]="";
	char pattern1[CHAR_MAX_LENGTH+1]="";
	ifstream one_cluster;
	ofstream pedcut_fam;
	
	one_cluster.open("one_cluster.fam",ios::in);
	pedcut_fam.open("pedcut.fam",ios::out);
	
	int maxbit = 20;//control_config
	double threshold = maxbit/2;
	int new_famID_naming = num_of_families + 1;
	//cout<<"num_of_families"<<num_of_families<<"====";getchar();
	int total_people = 0;
	//at least maxbit/2 people in a specific family, which may reach the maxbit value
	//===============================================
	for(int i=1;i<=num_of_families;i++)//同一family會連續出現
	{ 
		int num_member = statistic[i][0];//i == current family
		
		//if((2*statistic[i][1]-(num_member-statistic[i][1])) > maxbit)
			//cout<<(2*statistic[i][1]-(num_member-statistic[i][1]))<<" "<<i<<endl;
		if(num_member > threshold && 
			(2*statistic[i][1]-(num_member-statistic[i][1])) > maxbit)//this family needs pedcut
		{
			//未來可用thread同步進行多個pedcut(需處理相同檔名輸出問題)
			streamoff position_start;
			streamoff position_end;
			position_start = one_cluster.tellg();
			
			map<string, int> map_ID;
			map<string, int>::iterator it;
			map<string, int>::iterator it1;
			map<string, int>::iterator it_ID;
			
			vector<int> order;//index is the order of printing
			vector<int>::iterator ite;
			
			for(int j=0;j<num_member;j++)
			{
				one_cluster.getline(pattern, CHAR_MAX_LENGTH);
				char* tmp = strtok(pattern, " ");
				tmp = strtok(NULL, " ");
				it = map_ID.find(tmp);//actually, ID is without repetition
				if(it == map_ID.end()) map_ID[tmp] = j+1;//record (ID,line)
				
				order.push_back(j+1);//initialization
			}
			position_end = one_cluster.tellg();
			one_cluster.clear();
			one_cluster.seekg(position_start);//restore the position

			//re-ordering
			for(int j=0;j<num_member;j++)
			{
				one_cluster.getline(pattern, CHAR_MAX_LENGTH);
				char* tmp = strtok(pattern, " ");
				tmp = strtok(NULL, " ");//ID
				it_ID = map_ID.find(tmp);
				tmp = strtok(NULL, " ");//father
				it = map_ID.find(tmp);
				tmp = strtok(NULL, " ");//mother
				it1 = map_ID.find(tmp);
				
				if(it != map_ID.end() && it1 != map_ID.end())
				{
					if(it1->second > it_ID->second) 
					{
						ite = find(order.begin(),order.end(),it1->second);
						order.erase(ite);
						ite = find(order.begin(),order.end(),it_ID->second);
						order.insert(ite,it1->second);
					}
				}
				else if(it != map_ID.end() && it1 == map_ID.end()) 
				{
					ite = find(order.begin(),order.end(),it_ID->second);
					order.erase(ite);
					map_ID.erase(it_ID);//do not print out
				}
			}
			one_cluster.clear();
			one_cluster.seekg(position_start);//restore the position

			ofstream tmp_ped;
			tmp_ped.open("tmp.ped",ios::out);			
			for(int j=0;j<order.size();j++)
			{
				int which_line = order[j];
				for(int m=0;m<which_line;m++) one_cluster.getline(pattern, CHAR_MAX_LENGTH);
				strcpy(pattern1,pattern);//pattern1 is for strtok
				char* tmp = strtok(pattern1, " ");
				tmp = strtok(NULL, " ");//ID
				it_ID = map_ID.find(tmp);
				tmp = strtok(NULL, " ");//father
				it = map_ID.find(tmp);
				tmp = strtok(NULL, " ");//mother
				it1 = map_ID.find(tmp);
				if(it != map_ID.end() && it1 == map_ID.end()) map_ID.erase(it_ID);//do not print out
				else tmp_ped<<pattern<<endl;
				
				one_cluster.clear();
				one_cluster.seekg(position_start);//restore the position
			}
			tmp_ped.close();
			
			cout<<"family "<<i<<": NEED PEDCUT!!!"<<endl;//getchar();
			char buffer[50];
			sprintf (buffer, "pedcut -p tmp.ped -m %d", 20);
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
						people_mapping.push_back(align_info[i][corr-1]);//add the offset
						
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
				input_SubPedigrees.close();
				input_PedsSummary.close();
				system("sudo rm -rf SubPedigrees.csv");
				system("sudo rm -rf PedsSummary.csv");
				
				one_cluster.clear();
				one_cluster.seekg(position_end);
			}
			else 
			{
				//goto pedcut_is_futile;
				one_cluster.clear();
				one_cluster.seekg(position_end);
			}
		}
		else 
		{
		pedcut_is_futile:
			for(int j=0;j<num_member;j++)
			{
				one_cluster.getline(pattern, CHAR_MAX_LENGTH);
				pedcut_fam<<pattern<<endl;
				people_mapping.push_back(align_info[i][j]);
				
				char* tmp = strtok(pattern, " ");//fam
				int fam = atoi(tmp);//i
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
	system("sudo rm -rf tmp.ped");
	return total_people;
}
