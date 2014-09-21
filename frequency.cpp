#include "control.h"

void switch_allele(char* str)
{
	switch(*str)
	{
		case 'A':
			*str = 'T';
			break;
		case 'T':
			*str = 'A';
			break;
		case 'C':
			*str = 'G';
			break;
		case 'G':
			*str = 'C';
			break;
		default:
			cout<<"some errors occur"<<endl;
			break;					
	}
}

void frequency(int total_people, int** chr, int num_of_chrs)
{//http://pngu.mgh.harvard.edu/~purcell/plink/binary.shtml BED file format
	char pattern[CHAR_MAX_LENGTH+1]="";char pattern1[CHAR_MAX_LENGTH+1]="";
	
	ifstream input_bim, input_fre;
	ofstream output_fre;
	input_bim.open(BIM,ios::in);
	output_fre.open("fre_result.freq",ios::out);
	
	bool external_freq_or_not;
	streamoff position;
	
	extern char Freq_ext_result[150];
	if(strcmp(Freq_ext_result,"0") == 0) 
	{
		//cout<<"--"<<Freq_ext_result<<"--"<<endl;getchar();
		external_freq_or_not = false;
	}
	else 
	{
		//cout<<"=="<<Freq_ext_result<<"=="<<endl;getchar();
		external_freq_or_not = true;
		input_fre.open(Freq_ext_result,ios::in);		
		input_fre.getline(pattern, CHAR_MAX_LENGTH);//first line is useless
		position = input_fre.tellg();
	}
	//===============================================set propriate pre-fetch array size
	int remain_people = total_people%4;
	int boundary;//for last byte
	int const_need_per_marker;//byte
		if(remain_people != 0) 
		{
			const_need_per_marker = total_people/4 + 1;
			boundary = remain_people*2;
		}
		else
		{		
			const_need_per_marker = total_people/4;
			boundary = 8;
		}
	//===============================================calculate total markers
	int total_marker = 0;
	FILE* input_bed = fopen(BED,"r");
	bitset<8> a_byte;//represent 4 people
	fgets(pattern, 3+1, input_bed);//first 3 bytes is for special use

	for(int i=0;i<num_of_chrs;i++) 
	{
		char buffer[50];
		sprintf (buffer, "metadata%d.ped", chr[i][0]);
		ofstream output_ped;
		output_ped.open(buffer,ios::out);
		
		int total_marker_for_chr = chr[i][1];
		total_marker += chr[i][1];
		//cout<<num_of_chrs;getchar();
		//cout<<total_marker_for_chr;getchar();
		//===============================================
		int num_pre_fetch_marker = 1;//decide_prefetch_chrs_once(chr,num_of_chrs,const_need_per_marker);//一次處理幾個marker
		int pre_fetch_size = num_pre_fetch_marker*const_need_per_marker;
		while(total_marker_for_chr)//共幾個marker
		{
			memset(pattern,0,sizeof(pattern));
			fread((void*)pattern, 1, pre_fetch_size, input_bed);
			for(int h=0;h<num_pre_fetch_marker;h++)//依序處理marker 
			{			
				char* _tmp;
				string string1;
				char* _tmp_A[2];
				{
					input_bim.getline(pattern1, CHAR_MAX_LENGTH);
					//if(!strtok(pattern," 	,")) break;
					_tmp = strtok(pattern1, " 	");
					//cout<<_tmp<<"=="<<strlen(_tmp)<<endl;
					_tmp = strtok(NULL, " 	");
					//cout<<_tmp<<"=="<<strlen(_tmp)<<endl;
					//allele_count[3] = atoi(_tmp);//NAME
					string1 = _tmp; 
					//cout<<string1;getchar();
					//cout<<atoi(_tmp)<<" "<<endl;
					_tmp = strtok(NULL, " 	");
					_tmp = strtok(NULL, " 	");
					//cout<<_tmp;getchar();
					_tmp_A[0] = strtok(NULL, " 	");
					_tmp_A[1] = strtok(NULL, " 	");
					//cout<<_tmp_A[0]<<" "<<endl;cout<<_tmp_A[1]<<" "<<endl;getchar();
				}
				//===================
				int start = const_need_per_marker*h;
				int end = start + const_need_per_marker -1;
				double allele_count[4]={0};//two alleles: (A,T,C,G,1,2), missing, NAME
				{
					//leave last byte to another handle
					for(int k=start;k<end;k++)
					{
						a_byte = pattern[k];
						for(int ii=0;ii<8;ii+=2) 
						{
							if(a_byte[ii]==1 && a_byte[ii+1]==0) 
							{
								allele_count[2]+=2;
								output_ped<<"0 0 ";
							}
							else for(int jj=ii;jj<ii+2;jj++) 
							{
								allele_count[a_byte[jj]]++;
								output_ped<<_tmp_A[a_byte[jj]]<<" ";
							}
						}
					}
					//for last byte
					a_byte = pattern[end];
					for(int ii=0;ii<boundary;ii+=2) 
					{
						if(a_byte[ii]==1 && a_byte[ii+1]==0) 
						{
							allele_count[2]+=2;
							output_ped<<"0 0 ";
						}
						else for(int jj=ii;jj<ii+2;jj++) 
						{
							allele_count[a_byte[jj]]++;
							output_ped<<_tmp_A[a_byte[jj]]<<" ";
						}
					}
					output_ped<<endl;//end of a marker
				}
				//cout<<allele_count[0]<<" "<<allele_count[1]<<" "<<allele_count[2]<<endl;
				if(allele_count[0]+allele_count[1]+allele_count[2] != total_people*2) cout<<"some errors occur!!"<<endl;
				
				int allele_sample_space = total_people*2-allele_count[2];
				if(allele_sample_space == 0)
				{
					allele_count[0] = allele_count[1] = 0;
				}
				else
				{
					allele_count[0] = allele_count[0]/allele_sample_space;
					allele_count[1] = 1-allele_count[0];
				}
				//===================
				if(external_freq_or_not)//use external freq file or not
				{
					char* tmp;
					string string2;
					char* tmp_A[2];
					input_fre.clear();
					input_fre.seekg(position);
					do{
						input_fre.getline(pattern, CHAR_MAX_LENGTH);
						tmp = strtok(pattern, " 	");
						if(!tmp) break;
						tmp = strtok(NULL, " 	");
						string2 = tmp;
						//cout<<string2;getchar();
					}
					while(string1.compare(string2)!=0);
					if(string1.compare(string2)==0)
					{
						position = input_fre.tellg();
						//cout<<"hit!!!!!!!!!!!!!!!!!!!!!"<<endl;getchar();
						tmp_A[0] = strtok(NULL, " 	");
						tmp_A[1] = strtok(NULL, " 	");
						
						int match_valid;
						if(strcmp(_tmp_A[0],tmp_A[0]) == 0 && strcmp(_tmp_A[1],tmp_A[1]) == 0) match_valid = 1;//ok
						else if(strcmp(_tmp_A[0],tmp_A[1]) == 0 && strcmp(_tmp_A[1],tmp_A[0]) == 0) match_valid = 2;//ok
						else 
						{
							switch_allele(tmp_A[0]);
							switch_allele(tmp_A[1]);
							match_valid = 0;
							if(strcmp(_tmp_A[0],tmp_A[0]) == 0 && strcmp(_tmp_A[1],tmp_A[1]) == 0) match_valid = 1;//ok
							else if(strcmp(_tmp_A[0],tmp_A[1]) == 0 && strcmp(_tmp_A[1],tmp_A[0]) == 0) match_valid = 2;//ok
						}
						if(match_valid)//marker is matched in ASN.freq
						{
							tmp = strtok(NULL, " 	");//MAF
							double MAF = atof(tmp);
							
							double n1 = allele_sample_space/2;//n1
							tmp = strtok(NULL, " 	");
							double n2 = atoi(tmp);//n2
							
							if(match_valid == 1)
							{
								allele_count[0] = allele_count[0]*(n1/(n1+n2)) + MAF*(n2/(n1+n2));
								allele_count[1] = allele_count[1]*(n1/(n1+n2)) + (1-MAF)*(n2/(n1+n2));
							}
							else if(match_valid == 2)
							{
								allele_count[0] = allele_count[0]*(n1/(n1+n2)) + (1-MAF)*(n2/(n1+n2));
								allele_count[1] = allele_count[1]*(n1/(n1+n2)) + (MAF)*(n2/(n1+n2));					
							}
						}
						else
						{
							//frequency.snp
						}
					}
					else {}//no matched marker in ASN.freq
				}
				//===================
				//output_fre<<"M "<<allele_count[3]<<endl;
				output_fre<<"M "<<string1<<endl;
				//cout      <<"M "<<string1<<endl;
				for(int i=0;i<2;i++)
				{
					if(allele_count[i]>0) 
					{
						output_fre<<"A "<<(_tmp_A[i])<<" "<<allele_count[i]<<endl;
						//cout      <<"A "<<(_tmp_A[i])<<" "<<allele_count[i]<<endl;
					}		
				}
			}
			//cout<<total_marker_for_chr<<endl;getchar();
			total_marker_for_chr -= num_pre_fetch_marker;
		}
		output_ped.close();
	}
	//cout<<total_marker<<endl;getchar();//1000,4737983
	if(external_freq_or_not) input_fre.close();	
	//===============================================
	fclose(input_bed);
	input_bim.close();
	output_fre.close();
}