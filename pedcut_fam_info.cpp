#include "control.h"

struct _IND_ {
	_IND_(int fam, int id, int father_id, int mother_id, int sex, int affection, int which_line, _IND_* next):
		fam(fam),id(id),father_id(father_id),mother_id(mother_id),
		sex(sex),affection(affection),which_line(which_line),next(next){}
	
	int fam;
	int id;
	int father_id;//only one
	int mother_id;//only one
	int sex;
	int affection;
	
	int which_line;
	_IND_* next;
	
	_IND_* father = NULL;
	_IND_* mother = NULL;
	_IND_* spouse[5] = {NULL};// assume a person can have up to 5 spouses
	_IND_* child[15] = {NULL};// assume a person can have up to 15 children
	int num_spouse = 0;
	int num_child = 0;
	bool picked = false;
	//_IND_* sib = NULL;
	_IND_* go_through = NULL;//link all people in the same cluster
};
bool check_common_ancector(_IND_* A, _IND_* B, const int max_people)
{
	//cout<<max_people<<endl;
	//check父母關係 (需非父母關係, 隔代可)
	if((A->father == B || A->mother == B) || (B->father == A || B->mother == A)) return false;
	//check同源 (需同源)
	vector<_IND_*> ancector_for_A(max_people);
	vector<_IND_*> ancector_for_B(max_people);
	int A_index = 0, B_index = 0;
	
	ancector_for_A[A_index++] = A;
	ancector_for_B[B_index++] = B;

	int count = 0;
	while(count != A_index)
	{
		_IND_* ptr1 = ancector_for_A[count]->father;
		_IND_* ptr2 = ancector_for_A[count]->mother;
		if(ptr1 != NULL && ptr2 != NULL) 
		{
			ancector_for_A[A_index] = ptr1;
			ancector_for_A[A_index+1] = ptr2;
			A_index += 2;
		}
		++count;
	}

	count = 0;
	while(count != B_index)
	{
		_IND_* ptr1 = ancector_for_B[count]->father;
		_IND_* ptr2 = ancector_for_B[count]->mother;
		if(ptr1 != NULL && ptr2 != NULL) 
		{
			ancector_for_B[B_index] = ptr1;
			ancector_for_B[B_index+1] = ptr2;
			B_index += 2;
		}
		++count;
	}
	
	for(int i=0;i<A_index;i++)
		for(int j=0;j<B_index;j++) if(ancector_for_A[i]==ancector_for_B[j]) return true;
	
	/*for(vector<_IND_*>::iterator it1=ancector_for_A.begin()+1;it1!=ancector_for_A.end();it1++)
		for(vector<_IND_*>::iterator it2=ancector_for_B.begin()+1;it2!=ancector_for_B.end();it2++)
			if(*it1 == *it2) return true;*/
	return false;
}
void pedcut_fam_info(int total_people, set< pair<int, pair<int,int> > >& valid_pair_set)
{
	char pattern[CHAR_MAX_LENGTH+1]="";
	ifstream pedcut_fam;
	
	pedcut_fam.open("pedcut.fam",ios::in);
	
	streamoff position_start;
	position_start = pedcut_fam.tellg();
	
	int statistic[total_people][2];//index is from 1 ~ num_of_families
	for(int i=0;i<total_people;i++) statistic[i][0]=statistic[i][1]=0;

	int num_of_families = 0;
	int current_family = 0;
	while(pedcut_fam.eof() == false)
	{
		pedcut_fam.getline(pattern, CHAR_MAX_LENGTH);
		char* tmp = strtok(pattern, " 	");
	if(!tmp) break;
		int this_fam = atoi(tmp);
		tmp = strtok(NULL, " 	");//ID
		tmp = strtok(NULL, " 	");//father
		int father = atoi(tmp);
		tmp = strtok(NULL, " 	");//mother
		int mother = atoi(tmp);
		
		if(current_family == this_fam)
		{
			statistic[num_of_families][0]++;
		}
		else//new family
		{
			current_family = this_fam;
			num_of_families++;
			statistic[num_of_families][0]++;
		}		
		if(!(father==0 && mother==0)) statistic[num_of_families][1]++;//non-founder
	}
	pedcut_fam.clear();
	pedcut_fam.seekg(position_start);
	//======================================
	_IND_* const_head;
	int offset = 0;
	
	for(int ii=1;ii<=num_of_families;ii++)//同一family會連續出現
	{ 
		int num_member = statistic[ii][0];
		int non_founder = statistic[ii][1];
		int founder = num_member-non_founder;
		int which_line = 0;
		map<int, int> map_ID;
		map_ID[0] = 0;
		map<int, int>::iterator it;

		_IND_* head = NULL;
		for(int j=0;j<num_member;j++)
		{
			pedcut_fam.getline(pattern, CHAR_MAX_LENGTH);
			char* tmp = strtok(pattern, " ");
				int fam = atoi(tmp);
			tmp = strtok(NULL, " ");
				int id = atoi(tmp);			
				it = map_ID.find(id);//actually, ID is without repetition
				if(it == map_ID.end()) map_ID[id] = j+1;//record (ID,line)
			tmp = strtok(NULL, " ");
				int father_id = atoi(tmp);
			tmp = strtok(NULL, " ");
				int mother_id = atoi(tmp);
			tmp = strtok(NULL, " ");
				int sex = atoi(tmp);
			tmp = strtok(NULL, " ");
				int affection = atoi(tmp);
			head = new _IND_(fam,id,father_id,mother_id,sex,affection,which_line,head);
			which_line++;
		}
		const_head = head;
		
		//add some vacant people
		int extra_people = 0;
		int new_line_from = num_member+1;
		while(head)
		{
			int pa = head->father_id;
			int ma = head->mother_id;
			if(!(pa==0 && ma==0))//non-founder, (1 = male, 2 = female)
			{
				it = map_ID.find(pa);
				if(it == map_ID.end()) 
				{
					const_head = new _IND_(head->fam,pa,0,0,1,-999,-999,const_head);
					extra_people++;
					map_ID[pa] = new_line_from;
					new_line_from++;
				}
				
				it = map_ID.find(ma);
				if(it == map_ID.end()) 
				{
					const_head = new _IND_(head->fam,ma,0,0,2,-999,-999,const_head);
					extra_people++;
					map_ID[ma] = new_line_from;
					new_line_from++;
				}
			}
			head = head->next;
		}
		head = const_head;//refresh pointer
		
		//initialization
		const int total_founder = founder + extra_people;
		const int total_member  = num_member + extra_people;
		
		//find spouses/children of each individual
		_IND_* head2 = const_head;
		while(head2)
		{
			int individual_id = head2->id;
			while(head)
			{
				int pa = head->father_id;
				int ma = head->mother_id;
				if(pa == individual_id || ma == individual_id)//不考慮爸媽性別---------------
				{
					int id, sex;
					if(pa == individual_id) {id = ma;sex = 2;}//當爸爸
					else {id = pa;sex = 1;}//當媽媽
					
					_IND_* tmp = const_head;
					while(tmp)//find the spouse of head2
					{
						if(tmp->id == id)//考慮只有一邊超過5----------------
						{
							bool flag = true;
							int num = tmp->num_spouse;
							for(int k=0;k<num;k++) if(head2==tmp->spouse[k]) {flag = false;break;}
							
							if(flag)//new spouse found
							{
								if(num<5) {tmp->spouse[num] = head2;(tmp->num_spouse)++;}
								num = head2->num_spouse;
								if(num<5) {head2->spouse[num] = tmp;(head2->num_spouse)++;}
							}
							break;
						}
						tmp = tmp->next;
					}
					if(head->father==NULL && head->mother==NULL)
					{
						//set parents
						if(sex == 2) {head->father = head2;head->mother = tmp;}
						else {head->father = tmp;head->mother = head2;}
						//record the children
						int num_child = head2->num_child;
						if(num_child<15) {head2->child[num_child] = head;(head2->num_child)++;}
						
						num_child = tmp->num_child;
						if(num_child<15) {tmp->child[num_child] = head;(tmp->num_child)++;}
					}
				}
				head = head->next;
			}
			head = const_head;//restore pointer
			head2 = head2->next;
		}
		
		//link people belong to same cluster (construct all clusters)
		int which_cluster = 0;
		int count = 0;//count accumulated people
		
		_IND_* head3 = const_head;
		_IND_* go_through_start[total_founder];//max is total_founder
		int number_of_each_cluster[total_founder];
		int non_founder_of_each_cluster[total_founder];
		while(count < total_member)//a round for a cluster
		{
			int num_of_people = 0;
			int num_of_founder = 0;
			_IND_* go_through_end = NULL;
			while(head3)
			{
				if(head3->picked == false)//not yet linked
				{
					go_through_start[which_cluster] = go_through_end = head3;
					if(go_through_end->father_id==0 && go_through_end->mother_id==0) num_of_founder++;//founder
					go_through_end->picked = true;
					num_of_people++;
					break;
				}
				head3 = head3->next;
			}
			head3 = const_head;
			
			while(go_through_end)
			{
				if(go_through_end->father_id==0 && go_through_end->mother_id==0) {}//founder
				else//non-founder, link parents
				{
				    if((go_through_end->father)->picked == false)
					{
						if((go_through_end->father)->father_id==0 && (go_through_end->father)->mother_id==0) num_of_founder++;
						(go_through_end->father)->picked = true;
						(go_through_end->father)->go_through = go_through_end->go_through;
						go_through_end->go_through = go_through_end->father;
						num_of_people++;
					}
					if((go_through_end->mother)->picked == false)
					{
						if((go_through_end->mother)->father_id==0 && (go_through_end->mother)->mother_id==0) num_of_founder++;
						(go_through_end->mother)->picked = true;
						(go_through_end->mother)->go_through = go_through_end->go_through;
						go_through_end->go_through = go_through_end->mother;
						num_of_people++;
					}
				}
				int num = go_through_end->num_spouse;
				for(int i=0;i<num;i++)//link spouses
				{
					if((go_through_end->spouse[i])->picked == false)
					{
						if((go_through_end->spouse[i])->father_id==0 && (go_through_end->spouse[i])->mother_id==0) num_of_founder++;
						(go_through_end->spouse[i])->picked = true;
						(go_through_end->spouse[i])->go_through = go_through_end->go_through;
						go_through_end->go_through = go_through_end->spouse[i];
						num_of_people++;
					}
				}
				num = go_through_end->num_child;
				for(int i=0;i<num;i++)//link children
				{
					if((go_through_end->child[i])->picked == false)
					{
						if((go_through_end->child[i])->father_id==0 && (go_through_end->child[i])->mother_id==0) num_of_founder++;
						(go_through_end->child[i])->picked = true;
						(go_through_end->child[i])->go_through = go_through_end->go_through;
						go_through_end->go_through = go_through_end->child[i];
						num_of_people++;
					}
				}
				go_through_end = go_through_end->go_through;
			}
			number_of_each_cluster[which_cluster] = num_of_people;
			non_founder_of_each_cluster[which_cluster] = num_of_people-num_of_founder;
			count += num_of_people;
			which_cluster++;
		}
		if(count != total_member) {cout<<"===!!error2!!==="<<endl;getchar();}
		
		//choice the cluster with most people
		int max = -1;
		int which_one;
		for(int i=0;i<which_cluster;i++) 
			if(number_of_each_cluster[i] > max) {max = number_of_each_cluster[i];which_one = i;}
		
		//print out for checking
		//if(extra_people != 0) 
		//if(num_member != max)
			//{cout<<const_head->fam<<"=="<<num_member<<"==="<<max<<"==="<<(num_member-max);offset+=(num_member-max);getchar();}
		//cout<<num_member<<" "<<extra_people<<" "<<which_cluster<<" "<<max<<endl;//getchar();
		
		//output new fam file
		_IND_* ptr = go_through_start[which_one];
		//align_info, one_cluster_info
		_IND_* affected_people[max];
		int affected_count = 0;
		while(ptr)
		{
			/*if(ptr->which_line != -999) one_cluster_info.push_back(align_info[ii][ptr->which_line]);
			else one_cluster_info.push_back(-999);
			
			one_cluster<<ptr->fam<<" "<<ptr->id<<" "<<ptr->father_id<<" "<<ptr->mother_id<<" "
					   <<ptr->sex<<" "<<ptr->affection<<" "<<endl;*/
			/*
			-9 missing 
			0 missing
			1 unaffected
			2 affected
			*/
			if(ptr->affection == 2)//affected
			{
				affected_people[affected_count] = ptr;
				affected_count++;
			}			
			ptr = ptr->go_through;
		}
		
		int n=affected_count;
		for(int i=1;i<n;i++)//只有一個affected person不會存進set
		{
			for(int j=i+1;j<=n;j++)
			{
				//cout<<"("<<i<<","<<j<<") "<<endl;
				bool result = check_common_ancector(affected_people[i-1],affected_people[j-1],total_member);
				if(result) valid_pair_set.insert( 
						make_pair(affected_people[i-1]->fam,make_pair(affected_people[i-1]->id,affected_people[j-1]->id)) );
			}
		}

		//reset statistic
		statistic[ii][0] = number_of_each_cluster[which_one];//add "extra_people" num_member, non_founder is not changing
		statistic[ii][1] = non_founder_of_each_cluster[which_one];
		//delete individuals
		while(const_head)
		{
			_IND_* tmp = const_head;
			const_head = const_head->next;
			delete tmp;
		}
	}
	//cout<<offset<<endl;
	pedcut_fam.close();
}
/*int main()
{
	set< pair<int, pair<int,int> > > valid_pair_set;
	pedcut_fam_info(1367,valid_pair_set);
	cout<<"successfully"<<endl;
}*/