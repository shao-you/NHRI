#include "control.h"

void format_imputed_dat_ped(map< pair<int,int>, int >* ID_affect, int chr)//merlin-infer.ped => CHR1_infer.ped
{
	char pattern[CHAR_MAX_LENGTH+1]="";
	ifstream merlin_ped, merlin_dat;
	ofstream infer_ped, infer_dat;
	
	merlin_ped.open("merlin-infer.ped",ios::in);
	merlin_dat.open("merlin-infer.dat",ios::in);
	char buffer[50];
		sprintf (buffer, "CHR%d_infer.ped", chr);
	infer_ped.open(buffer,ios::out);
		sprintf (buffer, "CHR%d_infer.dat", chr);
	infer_dat.open(buffer,ios::out);
	
	infer_dat<<"A "<<"disease"<<endl;
	int num_marker = 0;
	while(merlin_dat.eof() == false)
	{
		merlin_dat.getline(pattern, CHAR_MAX_LENGTH);
		char* tmp = strtok(pattern," ");
		if(!tmp) break;
		if(pattern[0] == 'M') 
		{
			num_marker++;
			tmp = strtok(NULL," ");
			infer_dat<<"M "<<tmp<<endl;
		}
	}
	//cout<<num_marker<<endl;getchar();
	
	while(merlin_ped.eof() == false)
	{
		streamoff position_start;
		position_start = merlin_ped.tellg();
		
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
		
		merlin_ped.clear();
		merlin_ped.seekg(position_start);//restore the position
		
		int length;
		merlin_ped.getline(pattern, CHAR_MAX_LENGTH, '/');
		length = strlen(pattern);
		infer_ped<<pattern[length-1]<<" ";//first
		for(int k=0;k<num_marker-1;k++)
		{
			merlin_ped.getline(pattern, CHAR_MAX_LENGTH, '/');
			infer_ped<<pattern[0]<<" ";
			length = strlen(pattern);
			infer_ped<<pattern[length-1]<<" ";
		}
		merlin_ped.getline(pattern, CHAR_MAX_LENGTH);//last, delim is '\n'
		infer_ped<<pattern[0]<<" ";
			
		/*tmp = strtok(NULL," /");
		while(tmp)//一組四個為單位
		{
			infer_ped<<tmp<<" ";
			tmp = strtok(NULL," ");
			infer_ped<<tmp<<" ";
			for(int i=0;i<4;i++) tmp = strtok(NULL," ");
			tmp = strtok(NULL," /");
		}*/
		infer_ped<<endl;
	}
	merlin_ped.close();
	merlin_dat.close();
	infer_ped.close();
	infer_dat.close();
}