#include "control.h"
/********************************
This program is to generate MERLIN map file from bim file.
********************************/

void map_modify()
{
	char pattern[CHAR_MAX_LENGTH+1]="";
	ifstream input_map;
	ofstream output_new_map;
	
	input_map.open(BIM,ios::in);
	output_new_map.open("new_map.map",ios::out);
	//===============================================
	while(input_map.eof() == false)
	{
		input_map.getline(pattern, CHAR_MAX_LENGTH);
		char* tmp = strtok(pattern, " 	,");
	if(!tmp) break;
		output_new_map<<tmp<<" ";
		tmp = strtok(NULL, " 	,");
		output_new_map<<tmp<<" ";
		tmp = strtok(NULL, " 	,");
		tmp = strtok(NULL, " 	,");
		output_new_map<<atof(tmp)/1000000<<endl;/**/
	}
	
	input_map.close();
	output_new_map.close();
}