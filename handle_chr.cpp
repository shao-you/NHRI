#include "control.h"

void* handle_chr(void* para)
{
	struct parameter PARA = *((struct parameter*)para);
	//cout<<(*PARA.mapping).size()<<" == "<<(PARA.which_chr)<<" == "<<(PARA.marker_num)<<endl;//1367, 3, 1000
	//=============================
	char pattern[CHAR_MAX_LENGTH+1]="";
	
	ifstream input_fam, input_ped;
	ofstream output_ped;
	char buffer[300];
	
	input_fam.open("pedcut.fam",ios::in);
		
		sprintf (buffer, "metadata%d.ped", (PARA.which_chr));
		//cout<<buffer<<endl;getchar();
	input_ped.open(buffer,ios::in);//"metadataX.ped"
		
		sprintf (buffer, "CHR%d.ped", (PARA.which_chr));
		//cout<<buffer<<endl;getchar();
	output_ped.open(buffer,ios::out);//"CHRX.ped"
	
	streamoff position_start;
	position_start = input_ped.tellg();
	const int num_marker = (PARA.marker_num);
	const int size = (*PARA.mapping).size();
	
	for(int i=0;i<size;i++)//each person from pedcut.fam
	{
		input_fam.getline(pattern, CHAR_MAX_LENGTH);
		output_ped<<pattern;
		
		int which_line = (*PARA.mapping)[i];
		if(which_line == -999)
		{
			for(int j=0;j<num_marker;j++) output_ped<<"0 0 ";
			output_ped<<endl;
		}
		else
		{
			int num_ahead_char = (which_line-1)*4;
			
			for(int j=0;j<num_marker;j++)
			{
				input_ped.getline(pattern, CHAR_MAX_LENGTH);
				output_ped<<pattern[num_ahead_char]<<" "<<pattern[num_ahead_char+2]<<" ";
			}
			output_ped<<endl;
			
			input_ped.clear();
			input_ped.seekg(position_start);
		}
	}
	input_fam.close();
	input_ped.close();
	output_ped.close();
	//==============================
	int chr = (PARA.which_chr);
	sprintf (buffer, "mkdir dir_%d", chr);
	system(buffer);
	sprintf (buffer, "cd dir_%d && rm -rf *", chr);
	system(buffer);
	cout<<"---------------------"<<endl;
	//Generate cluster file
	sprintf (buffer, "cd dir_%d && merlin -d ../CHR%d.dat -m ../new_map.map -f ../fre_result.freq -p ../CHR%d.ped --rsq 0.1 --cfreq --bits %d",chr,chr,chr,Maxbit);
	system(buffer);
	//output: merlin-clusters.freq, merlin-cluster-freqs.log
	sprintf (buffer, "cd dir_%d && cp merlin-clusters.freq CHR%d.clusters",chr,chr);//merlin-clusters.freq => CHR3.clusters ??
	system(buffer);
	//===============================================
	cout<<"---------------------"<<endl;//time consuming
	//Imputation
	sprintf (buffer, "cd dir_%d && merlin -d ../CHR%d.dat -m ../new_map.map -f ../fre_result.freq -p ../CHR%d.ped --bits %d --infer --clusters CHR%d.clusters",chr,chr,chr,Maxbit,chr);
	system(buffer);
	//output: merlin-infer.dat, merlin-infer.ped, merlin-clusters.log
	format_imputed_dat_ped(PARA.ID_affect,chr);//merlin-infer.ped => CHR3_infer.ped, merlin-infer.dat => CHR3_infer.dat ??
	//===============================================
	cout<<"---------------------"<<endl;//use CHR3_infer.ped ??
	//Linkage Analysis
	sprintf (buffer, "cd dir_%d && merlin -d CHR%d_infer.dat -m ../new_map.map -f ../fre_result.freq -p CHR%d_infer.ped --grid 1 --bits %d --clusters CHR%d.clusters --pairs --npl --markerNames > CHR%d_linkage.txt",chr,chr,chr,Maxbit,chr,chr);
//system(buffer);
	//output: CHR3_linkage.txt
	//===============================================
	cout<<"---------------------"<<endl;//use CHR3_infer.ped ??
	//IBD sharing Analysis
	sprintf (buffer, "cd dir_%d && merlin -d CHR%d_infer.dat -m ../new_map.map -f ../fre_result.freq -p CHR%d_infer.ped --bits %d --extended --markerNames --grid 1",chr,chr,chr,Maxbit);
	system(buffer);
	//output: merlin.s15
	IBD_sharing_Analysis(PARA.valid_pair_set,chr);
	//output: notable_grid_chr3
	//===============================================
	cout<<"---------------------"<<endl;
	check_disease_model(size,chr);//check recessive & dominant models
	//output: valid_marker_chr3
	//===============================================
	cout<<"---------------------"<<endl;
	restore_ped(PARA.mapping,chr,num_marker);
	//output: restore_chr3.ped
	//===============================================
	//sprintf (buffer, "rm -rf metadata%d.ped",chr);
	//system(buffer);
	//sprintf (buffer, "rm -rf CHR%d.ped",chr);
	//system(buffer);
	//sprintf (buffer, "rm -rf CHR%d.dat",chr);
	//system(buffer);
	
	//sprintf (buffer, "cd dir_%d && rm -rf CHR%d.clusters", chr, chr);
	//system(buffer);
	//sprintf (buffer, "cd dir_%d && rm -rf CHR%d_infer.ped", chr, chr);
	//system(buffer);
	//sprintf (buffer, "cd dir_%d && rm -rf CHR%d_infer.ped", chr, chr);
	//system(buffer);
	//sprintf (buffer, "cd dir_%d && rm -rf merlin-clusters.freq", chr);
	//system(buffer);
	//sprintf (buffer, "cd dir_%d && rm -rf merlin-cluster-freqs.log", chr);
	//system(buffer);
	//sprintf (buffer, "cd dir_%d && rm -rf merlin-clusters.log", chr);
	//system(buffer);
	//sprintf (buffer, "cd dir_%d && rm -rf merlin-infer.ped", chr);
	//system(buffer);
	//sprintf (buffer, "cd dir_%d && rm -rf merlin-infer.dat", chr);
	//system(buffer);
	//sprintf (buffer, "cd dir_%d && rm -rf merlin.s15", chr);
	//system(buffer);
}