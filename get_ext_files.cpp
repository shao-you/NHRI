#include "control.h"
char Freq_ext_result[150];
char** Cluster_ext_result;

void get_ext_files(int** chr, int num_of_chrs)
{	
	for(int i=0;i<num_of_chrs;i++)
	{
		char buffer[300];
		sprintf (buffer, "mkdir dir_%d", chr[i][0]);
		system(buffer);
		sprintf (buffer, "cd dir_%d && rm -rf *", chr[i][0]);
		system(buffer);
	}
	//
	Cluster_ext_result = new char* [num_of_chrs];	
	for(int i=0;i<num_of_chrs;i++)
	{
		Cluster_ext_result[i] = new char[150];
		if(strcmp(Cluster_file,"generate") == 0) strcpy(Cluster_ext_result[i],"generate");
		else if(strcmp(Cluster_file,"ftp") == 0)
		{
			cout<<"Download EXTERNAL.clusters for chromosome "<<chr[i][0]<<": "<<endl;
			char file_name[150];
			bool ftp_get_file = ftp(file_name);
			if(ftp_get_file) 
			{
				strcpy(Cluster_ext_result[i],file_name);
			}
			else
			{
				cout<<"Download failed! Enter \"local clusters file name\" for chromosome "<<chr[i][0]<<": "<<endl;
				//fgets(file_name,sizeof(file_name),stdin);//include '\n'
				cin.getline(file_name,sizeof(file_name));
	
				ifstream input_cluster;
				input_cluster.open(file_name,ios::in);//local file
				if(input_cluster.is_open()) 
				{
					input_cluster.close();
					strcpy(Cluster_ext_result[i],file_name);
				}
				else 
				{
					cout<<"Open local file failed! Merlin will generate a cluster file for chromosome "<<chr[i][0]<<"! "<<endl;
					strcpy(Cluster_ext_result[i],"generate");
				}
			}
		}
		else
		{
			cout<<"Enter \"local clusters file name\" for chromosome "<<chr[i][0]<<": "<<endl;
			char file_name[150];
			//fgets(file_name,sizeof(file_name),stdin);//include '\n'
			cin.getline(file_name,sizeof(file_name));
			
			ifstream input_cluster;
			input_cluster.open(file_name,ios::in);//local file
			if(input_cluster.is_open()) 
			{
				input_cluster.close();
				strcpy(Cluster_ext_result[i],file_name);
			}
			else 
			{
				cout<<"Open local file failed! Merlin will generate a cluster file for chromosome "<<chr[i][0]<<"! "<<endl;
				strcpy(Cluster_ext_result[i],"generate");
			}
		}
	}
	//
	if(strcmp(Freq_file,"0") == 0) strcpy(Freq_ext_result,"0");
	else if(strcmp(Freq_file,"ftp") == 0)
	{
		cout<<"Download EXTERNAL.freq: "<<endl;
		char file_name[150];
		bool ftp_get_file = ftp(file_name);
		if(ftp_get_file) 
		{
			strcpy(Freq_ext_result,file_name);
		}
		else 
		{
			cout<<"Download failed! Enter \"local freq file name\" or \"0\" to skip EXTERNAL.freq: "<<endl;
			//fgets(file_name,sizeof(file_name),stdin);//include '\n'
			cin.getline(file_name,sizeof(file_name));
			if(strcmp(file_name,"0") == 0) strcpy(Freq_ext_result,"0");
			else
			{
				ifstream input_fre;
				input_fre.open(file_name,ios::in);//local file
				if(input_fre.is_open()) 
				{
					input_fre.close();
					strcpy(Freq_ext_result,file_name);
				}
				else 
				{
					cout<<"Open local file failed! Skip EXTERNAL.freq!"<<endl;
					strcpy(Freq_ext_result,"0");
				}
			}
		}
	}
	else
	{
		ifstream input_fre;
		input_fre.open(Freq_file,ios::in);//local file
		if(input_fre.is_open()) 
		{
			input_fre.close();
			strcpy(Freq_ext_result,Freq_file);
		}
		else 
		{
			cout<<"Open local file failed! Skip EXTERNAL.freq!"<<endl;
			strcpy(Freq_ext_result,"0");
		}
	}
}