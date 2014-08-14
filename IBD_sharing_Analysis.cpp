#include "control.h"

void IBD_sharing_Analysis(set< pair<int, pair<int,int> > >* valid_pair_set)
{
	(*valid_pair_set).size();
	
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
		int ID_2 = ati(tmp);
		tmp = strtok(NULL, " ");
		int grid_position = atoi(tmp);
		
		pair<int, pair<int,int> > pair1 = make_pair(this_fam,make_pair(ID_1,ID_2));
		pair<int, pair<int,int> > pair2 = make_pair(this_fam,make_pair(ID_2,ID_1));
		
		set< pair<int, pair<int,int> > >::iterator it;
		it = (*valid_pair_set).find(pair1);
		it = (*valid_pair_set).find(pair2);
	}
	
	s15.close();
}
