#include "control.h"

void merge_infer_ped(int** chr, int num_of_chrs)
{
	char pattern[CHAR_MAX_LENGTH+1]="";
	ifstream chr_ped[num_of_chrs];
	ifstream original_fam;
	ofstream merge_ped;
	
	char buffer[50];
	for(int i=0;i<num_of_chrs;i++) 
	{
		sprintf (buffer, "dir_%d/restore_chr%d.ped", chr[i][0], chr[i][0]);
		chr_ped[i].open(buffer,ios::in);	
	}
	original_fam.open("all_digital.fam",ios::in);//not aligned
	merge_ped.open("merged_infer.ped",ios::out);
	
	while(original_fam.eof() == false)//an original person each time
	{
		original_fam.getline(pattern, CHAR_MAX_LENGTH+1);//until CHAR_MAX_LENGTH byte
		merge_ped<<pattern;
		char* tmp = strtok(pattern, " ");
	if(!tmp) break;	
		for(int i=0;i<num_of_chrs;i++) 
		{
			do{
				chr_ped[i].clear();
				chr_ped[i].getline(pattern, CHAR_MAX_LENGTH+1);
				merge_ped<<pattern;
			}while((chr_ped[i].rdstate() & std::ifstream::failbit ) != 0);
		}
		merge_ped<<endl;
	}
	
	for(int i=0;i<num_of_chrs;i++) chr_ped[i].close();
	original_fam.close();
	merge_ped.close();
}
/*int main()
{
	int** chr = new int*[3];
	for(int i=0;i<3;i++) chr[i] = new int[2];
	chr[0][0]=chr[1][0]=chr[2][0]=3; 
	chr[0][1]=chr[1][1]=chr[2][1]=1000;
	merge_infer_ped(chr,3);
}*/