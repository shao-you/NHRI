#include "control.h"

struct _IND_ {
	_IND_(int fam, int id, int father_id, int mother_id, int sex, int affection, _IND_* next):
		fam(fam),id(id),father_id(father_id),mother_id(mother_id),
		sex(sex),affection(affection),next(next){}
	
	int fam;
	int id;
	int father_id;//only one
	int mother_id;//only one
	int sex;
	int affection;
	_IND_* next;
	
	_IND_* father = NULL;
	_IND_* mother = NULL;
	_IND_* spouse[50] = {NULL};// assume a person can have up to 5 spouses
	_IND_* child[100] = {NULL};// assume a person can have up to 10 children
	int num_spouse = 0;
	int num_child = 0;
	bool picked = false;
	//_IND_* sib = NULL;
	_IND_* go_through = NULL;//link all people in the same cluster
};

/*struct _FAM_ {
	string fam_id;
	//_IND_ *ANS;		// the first ancestor
	_IND_ *founders;
	//_IND_ *parents;	// all parents in the family except founders
	_IND_ *members;	// all members in the family except founders
	_IND_ *probands;
	int total_founders;
	int total_members;
};*/

void pick_up_main_cluster(int** statistic, int num_of_families)//statistic will be changed (人數會增減)
{
	char pattern[CHAR_MAX_LENGTH+1]="";
	ifstream all_digital_fam;
	ofstream one_cluster;
	
	all_digital_fam.open("all_digital_fam_align.fam",ios::in);
	one_cluster.open("one_cluster.fam",ios::out);
	_IND_* const_head;
	
	for(int i=1;i<=num_of_families;i++)//同一family會連續出現
	{ 
		int num_member = statistic[i][0];
		int non_founder = statistic[i][1];
		int founder = num_member-non_founder;
		
		map<int, int> map_ID;
		map<int, int>::iterator it;

		_IND_* head = NULL;
		for(int j=0;j<num_member;j++)
		{
			all_digital_fam.getline(pattern, CHAR_MAX_LENGTH);
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
			head = new _IND_(fam,id,father_id,mother_id,sex,affection,head);
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
					const_head = new _IND_(head->fam,pa,0,0,1,-999,const_head);
					extra_people++;
					map_ID[pa] = new_line_from;
					new_line_from++;
				}
				
				it = map_ID.find(ma);
				if(it == map_ID.end()) 
				{
					const_head = new _IND_(head->fam,ma,0,0,2,-999,const_head);
					extra_people++;
					map_ID[ma] = new_line_from;
					new_line_from++;
				}
			}
			head = head->next;
		}
		head = const_head;//refresh pointer
		//num_member += extra_people;
		
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
						if(num_child<10) {head2->child[num_child] = head;(head2->num_child)++;}
						
						num_child = tmp->num_child;
						if(num_child<10) {tmp->child[num_child] = head;(tmp->num_child)++;}
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
		while(count < total_member)//a round for a cluster
		{
			int num_of_people = 0;
			_IND_* go_through_end = NULL;
			while(head3)
			{
				if(head3->picked == false)//not yet linked
				{
					go_through_start[which_cluster] = go_through_end = head3;
					go_through_end->picked = true;
					num_of_people++;
					break;
				}
				head3 = head3->next;
			}
			head3 = const_head;
			
			while(go_through_end)
			{
				if(!(go_through_end->father_id==0 && go_through_end->mother_id==0))//non-founder, link parents
				{
					if((go_through_end->father)->picked==false && (go_through_end->mother)->picked==false)
					{
						(go_through_end->father)->picked = (go_through_end->mother)->picked = true;
						(go_through_end->father)->go_through = go_through_end->mother;
						go_through_end->go_through = go_through_end->father;
						num_of_people += 2;
					}
				}
				int num = go_through_end->num_spouse;
				for(int i=0;i<num;i++)//link spouses
				{
					if((go_through_end->spouse[i])->picked == false)
					{
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
						(go_through_end->child[i])->picked = true;
						(go_through_end->child[i])->go_through = go_through_end->go_through;
						go_through_end->go_through = go_through_end->child[i];
						num_of_people++;
					}
				}
				go_through_end = go_through_end->go_through;
			}
			number_of_each_cluster[which_cluster] = num_of_people;
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
		if(num_member != max)
			{cout<<const_head->fam<<"=="<<num_member<<"==="<<max;getchar();}
		//cout<<num_member<<" "<<extra_people<<" "<<which_cluster<<" "<<max<<endl;//getchar();
		
		//output new fam file
		_IND_* ptr = go_through_start[which_one];
		while(ptr)
		{
			one_cluster<<ptr->fam<<" "<<ptr->id<<" "<<ptr->father_id<<" "<<ptr->mother_id<<" "
					   <<ptr->sex<<" "<<ptr->affection<<endl;
			ptr = ptr->go_through;
		}
		
		//reset statistic
		statistic[i][0];
		statistic[i][1];

		//delete individuals
		while(const_head)
		{
			_IND_* tmp = const_head;
			const_head = const_head->next;
			delete tmp;
		}
	}
	all_digital_fam.close();
	one_cluster.close();
}
