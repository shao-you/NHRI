#include "control.h"

void merge_infer_ped(int** chr, int num_of_chrs)
{
	char pattern[CHAR_MAX_LENGTH+1]="";
	ifstream chr_ped[num_of_chrs];
	ifstream chr_dosage[num_of_chrs];
	ifstream original_fam;
	ofstream merge_ped;
	ofstream merge_dosage;
	
	char buffer[50];
	for(int i=0;i<num_of_chrs;i++) 
	{
		sprintf (buffer, "dir_%d/restore_chr%d.ped", chr[i][0], chr[i][0]);
		chr_ped[i].open(buffer,ios::in);

		sprintf (buffer, "dir_%d/restore_chr%d.dosage", chr[i][0], chr[i][0]);
		chr_dosage[i].open(buffer,ios::in);		
	}
	original_fam.open("all_digital.fam",ios::in);//not aligned
	merge_ped.open("merged_infer.ped",ios::out);
	merge_dosage.open("merged_infer.dosage",ios::out);
	
	while(original_fam.eof() == false)//an original person each time
	{
		original_fam.getline(pattern, CHAR_MAX_LENGTH+1);//until CHAR_MAX_LENGTH byte
		merge_ped<<pattern;
		merge_dosage<<pattern;
		char* tmp = strtok(pattern, " ");
	if(!tmp) break;	
		for(int i=0;i<num_of_chrs;i++) 
		{
			do{
				chr_ped[i].clear();
				chr_ped[i].getline(pattern, CHAR_MAX_LENGTH+1);
				merge_ped<<pattern;
			}while((chr_ped[i].rdstate() & std::ifstream::failbit ) != 0);
			
			do{
				chr_dosage[i].clear();
				chr_dosage[i].getline(pattern, CHAR_MAX_LENGTH+1);
				merge_dosage<<pattern;
			}while((chr_dosage[i].rdstate() & std::ifstream::failbit ) != 0);
		}
		merge_ped<<endl;
		merge_dosage<<endl;
	}
	
	for(int i=0;i<num_of_chrs;i++) {chr_ped[i].close();chr_dosage[i].close();}
	original_fam.close();
	merge_ped.close();
	merge_dosage.close();
}
/*int main()
{
	int** chr = new int*[3];
	for(int i=0;i<3;i++) chr[i] = new int[2];
	chr[0][0]=chr[1][0]=chr[2][0]=3; 
	chr[0][1]=chr[1][1]=chr[2][1]=1000;
	merge_infer_ped(chr,3);
}*/