#include "control.h"

void IBD_sharing_Analysis(set< pair<int, pair<int,int> > >* valid_pair_set)//notice thread race condition
{
	int num_of_pair = (*valid_pair_set).size();
	map<float, int> grid_pool;
	
	char pattern[CHAR_MAX_LENGTH+1]="";
	ifstream s15;
	
	s15.open("merlin.s15",ios::in);
	
	streamoff position_start;
	position_start = s15.tellg();
	
	s15.getline(pattern, CHAR_MAX_LENGTH);//first line is ignored
	while(s15.eof() == false)
	{
		s15.getline(pattern, CHAR_MAX_LENGTH);
		char* tmp = strtok(pattern, " ");
	if(!tmp) break;
		int this_fam = atoi(tmp);
		tmp = strtok(NULL, " ");
		int ID_1 = atoi(tmp);
		tmp = strtok(NULL, " ");
		int ID_2 = atoi(tmp);
		tmp = strtok(NULL, " ");
		float grid_position = atof(tmp);
		if(grid_pool.find(grid_position) == grid_pool.end()) grid_pool[grid_position]=0;//new position, initialize count
		
		pair<int, pair<int,int> > pair1 = make_pair(this_fam,make_pair(ID_1,ID_2));
		pair<int, pair<int,int> > pair2 = make_pair(this_fam,make_pair(ID_2,ID_1));
		
		//set< pair<int, pair<int,int> > >::iterator it;
		//it = (*valid_pair_set).find(pair1);
		if((*valid_pair_set).find(pair1) != (*valid_pair_set).end() 
			|| (*valid_pair_set).find(pair2) != (*valid_pair_set).end())
		{
			for(int i=0;i<15;i++) tmp = strtok(NULL, " ");
			if(atof(tmp) > IBD_threshold) grid_pool[grid_position]++;//add a pair with IBD sharing
		}
	}
	for(map<float, int>::iterator it=grid_pool.begin() ; it!=grid_pool.end() ; ++it)
		if(it->second > Sharing_threshold*num_of_pair) cout<<"marker: "<<it->first<<endl;
	
	s15.close();
}
