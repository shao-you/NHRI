#include "control.h"

void* handle_chr(void* para)
{
	struct parameter PARA = *((struct parameter*)para);
	//cout<<(*PARA.mapping).size()<<" == "<<(PARA.which_chr)<<" == "<<(PARA.marker_num)<<endl;//1666
	//=============================
	char pattern[CHAR_MAX_LENGTH+1]="";
	
	ifstream input_fam, input_ped;
	ofstream output_ped;
	char buffer[200];
		
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
	input_fam.close();
	input_ped.close();
	output_ped.close();
	//==============================
	int chr = (PARA.which_chr);
	cout<<"---------------------"<<endl;
	//Generate cluster file
	sprintf (buffer, "merlin -d CHR%d.dat -m new_map.map -f fre_result.freq -p CHR%d.ped --grid 1 --rsq 0.1 --cfreq --bits %d",chr,chr,20);
	system(buffer);
	//output: merlin-clusters.freq, merlin-cluster-freqs.log, merlin-clusters.log
	sprintf (buffer, "sudo cp merlin-clusters.freq CHR%d.clusters",chr);//merlin-clusters.freq => CHR3.clusters ??
	system(buffer);
	//===============================================
	cout<<"---------------------"<<endl;
	//Imputation
	sprintf (buffer, "merlin -d CHR%d.dat -m new_map.map -f fre_result.freq -p CHR%d.ped --bits %d --infer --clusters CHR%d.clusters",chr,chr,20,chr);
	system(buffer);
	//output: merlin-infer.dat, merlin-infer.ped
	format_imputed_dat_ped(PARA.ID_affect,chr);//merlin-infer.ped => CHR3_infer.ped, merlin-infer.dat => CHR3_infer.dat ??
	//===============================================
	cout<<"---------------------"<<endl;//use CHR3_infer.ped ??
	//Linkage Analysis
	sprintf (buffer, "merlin -d CHR%d_infer.dat -m new_map.map -f fre_result.freq -p CHR%d_infer.ped --bits %d --clusters CHR%d.clusters --pairs --npl --markerNames > CHR%d_linkage.txt",chr,chr,20,chr,chr);
	system(buffer);
	//output: CHR3_linkage.txt
	//===============================================
	cout<<"---------------------"<<endl;//use CHR3_infer.ped ??
	//IBD sharing Analysis
	sprintf (buffer, "merlin -d CHR%d_infer.dat -m new_map.map -f fre_result.freq -p CHR%d_infer.ped --bits %d --extended --markerNames --grid 1",chr,chr,20);
	system(buffer);
	//output: merlin.s15
	//===============================================
	//sprintf (buffer, "sudo rm -rf metadata%d.ped",chr);
	//system(buffer);
	//sprintf (buffer, "sudo rm -rf CHR%d.ped",chr);
	//system(buffer);
	//system("sudo rm -rf merlin-clusters.freq");
	//system("sudo rm -rf merlin-cluster-freqs.log");
	//system("sudo rm -rf merlin-clusters.log");
	//system("sudo rm -rf merlin-infer.ped");
	//system("sudo rm -rf merlin-infer.dat");
}
