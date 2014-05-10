#include "control.h"
/********************************
Calculate total people.
********************************/
int count_people()
{
	char pattern[CHAR_MAX_LENGTH+1]="";
	ifstream input_fam;
	input_fam.open(FAM,ios::in);
	int total_people = 0;
	
	while(input_fam.eof() == false)
	{
		input_fam.getline(pattern, CHAR_MAX_LENGTH);
		if(!strtok(pattern," 	,")) break;
		total_people++;
	}
	//cout<<total_people;getchar();//1756,959
	input_fam.close();
	return total_people;
}