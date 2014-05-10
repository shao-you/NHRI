#include "control.h"

int count_chr_number(int** chr)//return # of chr
{
	char pattern[CHAR_MAX_LENGTH+1]="";
	
	//chr = new int* [max_chr];
	//[#chr][# of marker]
	ifstream input_bim;
	input_bim.open(BIM,ios::in);
	//===============================================
	int current_chr = -1;
	int index = -1;
	while(input_bim.eof() == false)//假設同一chr連續出現
	{		
		input_bim.getline(pattern, CHAR_MAX_LENGTH);
		char* tmp = strtok(pattern, " 	");
	if(!tmp) break;
		int __chr = atoi(tmp);
		if(current_chr != __chr)//new chr appears
		{
			index++;
			current_chr = __chr;
			chr[index] = new int [2];
			chr[index][0] = current_chr;//initialize #chr
			chr[index][1] = 1;//initialize # of marker
		}
		else chr[index][1]++;
	}	
	//int check_total_markers = 0;
	//for(int i=0;i<index+1;i++) check_total_markers += chr[i][1];
	//cout<<index+1<<"  "<<check_total_markers;getchar();
	input_bim.close();
	return index+1;
}