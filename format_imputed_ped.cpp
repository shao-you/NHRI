#include "control.h"

void format_imputed_ped(map< pair<int,int>, int >* ID_affect, int chr)//merlin-infer.ped => CHR1_infer.ped
{
	char pattern[CHAR_MAX_LENGTH+1]="";
	ifstream merlin_ped;
	ofstream infer_ped;
	
	merlin_ped.open("merlin-infer.ped",ios::in);
	char buffer[50];
		sprintf (buffer, "CHR%d_infer.ped", chr);
	infer_ped.open(buffer,ios::out);
	
	while(merlin_ped.eof() == false)
	{
		merlin_ped.getline(pattern, CHAR_MAX_LENGTH);
		char* tmp = strtok(pattern, " 	");
	if(!tmp) break;
		int fam = atoi(tmp);
		tmp = strtok(NULL, " 	");
		int ID = atoi(tmp);
		
		infer_ped<<fam<<" "<<ID<<" ";
		for(int k=0;k<3;k++)
		{
			tmp = strtok(NULL, " 	");
			infer_ped<<tmp<<" ";
		}
		infer_ped<<(*ID_affect)[make_pair(fam,ID)]<<" ";
		
		tmp = strtok(NULL," /");
		while(tmp)//一組四個為單位
		{
			infer_ped<<tmp<<" ";
			tmp = strtok(NULL," ");
			infer_ped<<tmp<<" ";
			for(int i=0;i<4;i++) tmp = strtok(NULL," ");
			tmp = strtok(NULL," /");
		}
		infer_ped<<endl;
	}
	merlin_ped.close();
	infer_ped.close();
	//system("sudo rm -rf merlin-infer.ped");
}