#include "control.h"

void* handle_chr(void* para)
{
	struct parameter PARA = *((struct parameter*)para);
	int chr = (PARA.chr)[PARA.chr_index][0];
	int marker_num = (PARA.chr)[PARA.chr_index][1];
	//cout<<(*PARA.mapping).size()<<" == "<<chr<<" == "<<marker_num<<endl;//1367, 3, 1000
	//=============================
	char pattern[CHAR_MAX_LENGTH+1]="";
	
	ifstream input_fam, input_ped;
	ofstream output_ped;
	char buffer[300];
	
	input_fam.open("pedcut.fam",ios::in);
		
		sprintf (buffer, "metadata%d.ped", chr);
	input_ped.open(buffer,ios::in);//"metadataX.ped"
		
		sprintf (buffer, "CHR%d.ped", chr);
	output_ped.open(buffer,ios::out);//"CHRX.ped"
	
	streamoff position_start;
	position_start = input_ped.tellg();
	const int size = (*PARA.mapping).size();
	
	for(int i=0;i<size;i++)//each person from pedcut.fam
	{
		input_fam.getline(pattern, CHAR_MAX_LENGTH);
		output_ped<<pattern;
		
		int which_line = (*PARA.mapping)[i];
		if(which_line == -9)
		{
			for(int j=0;j<marker_num;j++) output_ped<<"0 0 ";
			output_ped<<endl;
		}
		else
		{
			int num_ahead_char = (which_line-1)*4;
			
			for(int j=0;j<marker_num;j++)
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
	cout<<"-----------0-----------"<<endl;
	//Generate cluster file
	extern char** Cluster_ext_result;
	if(strcmp(Cluster_ext_result[PARA.chr_index],"generate") == 0)
	{
		sprintf (buffer, "cd dir_%d && merlin -d ../CHR%d.dat -m ../new_map.map -f ../fre_result.freq -p ../CHR%d.ped --rsq 0.1 --cfreq --bits %d",chr,chr,chr,Maxbit);
		system(buffer);
		//output: merlin-clusters.freq, merlin-cluster-freqs.log
		sprintf (buffer, "cd dir_%d && mv merlin-clusters.freq CHR%d.clusters",chr,chr);//merlin-clusters.freq => CHR3.clusters
		system(buffer);
	}
	else
	{
		sprintf (buffer, "cd dir_%d && ln -s ../%s CHR%d.clusters",chr,Cluster_ext_result[PARA.chr_index],chr);
		system(buffer);
	}
	delete [] Cluster_ext_result[PARA.chr_index];
	//===============================================
	cout<<"-----------1-----------"<<endl;//time consuming
	//Imputation
	if(Impute == 1) 
	{
		sprintf (buffer, "cd dir_%d && merlin -d ../CHR%d.dat -m ../new_map.map -f ../fre_result.freq -p ../CHR%d.ped --bits %d --infer --clusters CHR%d.clusters",chr,chr,chr,Maxbit,chr);
		system(buffer);
		//output: merlin-infer.dat, merlin-infer.ped, merlin-clusters.log
		format_imputed_dat_ped(PARA.ID_affect,chr);//merlin-infer.ped => CHR3_infer.ped, merlin-infer.dat => CHR3_infer.dat
	}
	//===============================================
	cout<<"-----------2-----------"<<endl;
	//Linkage Analysis
	if(strcmp(Linkage,"") != 0)
	{
		//sprintf (buffer, "cd dir_%d && merlin -d CHR%d_infer.dat -m ../new_map.map -f ../fre_result.freq -p CHR%d_infer.ped --grid 1 --bits %d --clusters CHR%d.clusters %s --markerNames > CHR%d_linkage.txt",chr,chr,chr,Maxbit,chr,Linkage,chr);
		sprintf (buffer, "cd dir_%d && merlin -d ../CHR%d.dat -m ../new_map.map -f ../fre_result.freq -p ../CHR%d.ped --grid 1 --bits %d --clusters CHR%d.clusters %s --markerNames > CHR%d_linkage.txt",chr,chr,chr,Maxbit,chr,Linkage,chr);
		system(buffer);
		//output: CHR3_linkage.txt
	}
	//===============================================
	cout<<"-----------3-----------"<<endl;
	//IBD Sharing Analysis
	if(IBD_Sharing == 1)
	{
		//sprintf (buffer, "cd dir_%d && merlin -d CHR%d_infer.dat -m ../new_map.map -f ../fre_result.freq -p CHR%d_infer.ped --bits %d --extended --markerNames --grid 1",chr,chr,chr,Maxbit);
		sprintf (buffer, "cd dir_%d && merlin -d ../CHR%d.dat -m ../new_map.map -f ../fre_result.freq -p ../CHR%d.ped --bits %d --extended --markerNames --grid 1",chr,chr,chr,Maxbit);
		system(buffer);
		//output: merlin.s15
		IBD_sharing_Analysis(PARA.valid_pair_set,chr);
		//output: notable_grid_chr3
	}
	//===============================================
	cout<<"-----------4-----------"<<endl;
	vector<int> index_reduced_marker;//record markers erased by imputation
	vector<int> infer_marker_mapping;//record mapping of markers between CHR3.dat/CHR3_infer.dat
	if(Impute == 1) calculate_infer_marker_info(chr,index_reduced_marker,infer_marker_mapping);
	//Disease Model
	if(Disease_model == 1) 
	{
		//If imputation is performed, use CHR3_infer.ped as input; otherwise, use CHR3.ped
		check_disease_model(size,chr,index_reduced_marker,infer_marker_mapping);//check recessive & dominant models
		//output: valid_marker_chr3
	}
	//===============================================
	cout<<"-----------5-----------"<<endl;
	if(Impute == 1) restore_ped(PARA.mapping,chr,marker_num,infer_marker_mapping);
	//output: restore_chr3.ped
	//===============================================
	//sprintf (buffer, "rm -rf metadata%d.ped", chr);
	//system(buffer);
	//sprintf (buffer, "rm -rf CHR%d.ped", chr);
	//system(buffer);
	//sprintf (buffer, "rm -rf CHR%d.dat", chr);
	//system(buffer);
	
	//sprintf (buffer, "cd dir_%d && rm -rf CHR%d.clusters", chr, chr);
	//system(buffer);
	//sprintf (buffer, "cd dir_%d && rm -rf CHR%d_infer.ped", chr, chr);
	//system(buffer);
	//sprintf (buffer, "cd dir_%d && rm -rf CHR%d_infer.dat", chr, chr);
	//system(buffer);
	sprintf (buffer, "cd dir_%d && rm -rf merlin-cluster-freqs.log", chr);
	system(buffer);
	sprintf (buffer, "cd dir_%d && rm -rf merlin-clusters.log", chr);
	system(buffer);
	//sprintf (buffer, "cd dir_%d && rm -rf merlin-infer.ped", chr);
	//system(buffer);
	//sprintf (buffer, "cd dir_%d && rm -rf merlin-infer.dat", chr);
	//system(buffer);
	//sprintf (buffer, "cd dir_%d && rm -rf merlin.s15", chr);
	//system(buffer);
}