#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#define max_row 3000
#define max_col 2000
#define CHAR_MAX_LENGTH 9999
using namespace std;

int main(int argc, char* argv[]) 
{
	cout<<"input: [map] [ped] [frequency]"<<endl;
	ifstream input_map, input_ped, input_fre;
	ofstream output_fre;
	
	input_map.open(argv[1],ios::in);
	input_ped.open(argv[2],ios::in);
	//input_fre.open(argv[3],ios::in);
	
	output_fre.open("final_frequency.freq",ios::out);
	char pattern[CHAR_MAX_LENGTH]="";
        //patternIN.getline (pattern, CHAR_MAX_LENGTH);
        //stringIN.getline  (string , CHAR_MAX_LENGTH);
	int col = 0, row;
	char** allele;
		//1 2 4 8 16
		//0 A T C G 
	while(input_ped.eof() == false)
	{		
		input_ped.getline(pattern, CHAR_MAX_LENGTH);
		//cout<<pattern;
		int count = strlen(pattern)+1;
		
		char* tmp = strtok(pattern, " 	");
		if(!tmp) break;
		for(int i=0;i<5;i++) tmp = strtok(NULL, " 	");
		tmp = strtok(NULL, " 	");//first allele
		
		if(col == 0)//first time for calculating row
		{
			row = (count-(tmp-pattern))/2;
			//cout<<row<<" ";//2000 alleles
			allele = new char* [row];
			for(int i=0;i<row;i++) allele[i] = new char [max_col];//enough here!!
		}
		int index = 0;
		while(tmp!=NULL)
		{
			//cout<<tmp<<" ";
			allele[index][col] = *tmp;//a char
			tmp = strtok(NULL, " 	");
			index++;
		} 
		//cout<<index;
		//getchar();
		col++;
	}
	cout<<"total people: "<<col<<endl;
	//==================================================
	int ASN_start_line = 1;
	for(int k=0;k<row-1;k+=2)//a marker
	{
		double allele_count[6]={0};//A T C G 0 NAME
		//for(int m=0;m<5;m++) cout<< allele_count[m]<<" ";
		for(int j=0;j<col;j++) if(allele[k][j]=='0' || allele[k+1][j]=='0') allele[k][j]=allele[k+1][j]='0';//avoid only one missing allele
		
		for(int i=k;i<k+2;i++) 
			for(int j=0;j<col;j++) 
			{
				switch(allele[i][j])
				{
					case 'A':
						allele_count[0]++;
						break;
					case 'T':
						allele_count[1]++;
						break;
					case 'C':
						allele_count[2]++;
						break;
					case 'G':
						allele_count[3]++;
						break;
					case '0':
						allele_count[4]++;
						break;
					default:
						cout<<"some errors occur"<<endl;
						break;
				}
			}
		if((allele_count[0]+allele_count[1]+allele_count[2]+allele_count[3]+allele_count[4]) != col*2) 
			cout<<"some errors occur"<<endl;
		//cout<<allele_count[0]<<" "<<allele_count[1]<<" "<<allele_count[2]<<" "<<allele_count[3]<<" "<<allele_count[4]<<endl;
		
		int check_valid = 0;
		int allele_sample_space = col*2-allele_count[4];
		for(int m=0;m<4;m++) 
		{
			if(allele_count[m]!=0) 
			{	
				check_valid++;
				allele_count[m] = allele_count[m]/allele_sample_space;
			}
		}
		if(check_valid > 2) cout<<check_valid<<" :some errors occur"<<endl;
		
		char* tmp;
		//while(input_map.eof() == false)
		{
			input_map.getline(pattern, CHAR_MAX_LENGTH);
			tmp = strtok(pattern, " 	");
			//cout<<tmp<<"=="<<strlen(tmp)<<endl;
			tmp = strtok(NULL, " 	");
			//cout<<tmp<<"=="<<strlen(tmp)<<endl;
			allele_count[5] = atoi(tmp);
			//cout<<atoi(tmp)<<" "<<endl;
		}
		{
			input_fre.open(argv[3],ios::in);
			for(int i=0;i<ASN_start_line;i++) input_fre.getline(pattern, CHAR_MAX_LENGTH);//first line is useless
			int marker_name;
			do{
				ASN_start_line++;
				input_fre.getline(pattern, CHAR_MAX_LENGTH);
				tmp = strtok(pattern, " 	");
				tmp = strtok(NULL, " 	");
				marker_name = atoi(tmp);
			}while(allele_count[5]!=marker_name && allele_count[5]>marker_name);
			if(allele_count[5]==marker_name)
			{
				double A_T_C_G[4]={0};
				char* tmp_A1 = strtok(NULL, " 	");
				char* tmp_A2 = strtok(NULL, " 	");
				tmp = strtok(NULL, " 	");//MAF
				double MAF = atof(tmp);
				switch(*tmp_A1)
				{
					case 'A':
						A_T_C_G[0]=MAF;
						break;
					case 'T':
						A_T_C_G[1]=MAF;
						break;
					case 'C':
						A_T_C_G[2]=MAF;
						break;
					case 'G':
						A_T_C_G[3]=MAF;
						break;
					default:
						cout<<"some errors occur"<<endl;
						break;					
				}
				switch(*tmp_A2)
				{
					case 'A':
						A_T_C_G[0]=1-MAF;
						break;
					case 'T':
						A_T_C_G[1]=1-MAF;
						break;
					case 'C':
						A_T_C_G[2]=1-MAF;
						break;
					case 'G':
						A_T_C_G[3]=1-MAF;
						break;
					default:
						cout<<"some errors occur"<<endl;
						break;					
				}	
				bool match_valid = true;
				for(int i=0;i<4;i++) 
					if((allele_count[i]==0 && A_T_C_G[i]!=0) || (allele_count[i]!=0 && A_T_C_G[i]==0)) {match_valid=false;break;}
				if(!match_valid) 
				{
					match_valid = true;//another check
					swap(A_T_C_G[0],A_T_C_G[1]);
					swap(A_T_C_G[2],A_T_C_G[3]);
					for(int i=0;i<4;i++)
						if((allele_count[i]==0 && A_T_C_G[i]!=0) || (allele_count[i]!=0 && A_T_C_G[i]==0)) {match_valid=false;break;}
				}
				if(match_valid)//marker is matched in ASN_freq file
				{
					double n1 = allele_sample_space/2;//n1
					tmp = strtok(NULL, " 	");
					double n2 = atoi(tmp);//n2
					for(int i=0;i<4;i++)
						if(allele_count[i]!=0) allele_count[i] = allele_count[i]*(n1/(n1+n2)) + A_T_C_G[i]*(n2/(n1+n2));
				}
				else
				{
					//frequency.snp
				}
			}
			else ASN_start_line--;//no matched marker in ASN_freq file
			
			output_fre<<"M "<<allele_count[5]<<endl;
			for(int i=0;i<4;i++) 
			{
				if(allele_count[i]!=0) 
				{
					switch(i)
					{
						case 0: output_fre<<"A A ";break;
						case 1: output_fre<<"A T ";break;
						case 2: output_fre<<"A C ";break;
						case 3: output_fre<<"A G ";break;
					}
					output_fre<<allele_count[i]<<endl;
				}
			}		
			input_fre.close();		
		}
		//getchar();
	}
	//==================================================
	for(int i=0;i<row;i++) delete [] allele[i];
	delete [] allele;
	
	input_map.close();
	input_ped.close();
	output_fre.close();
	return 0;
}
