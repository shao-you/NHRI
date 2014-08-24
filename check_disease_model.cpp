#include "control.h"

string parse_ped(int total_people, ifstream& ped_file, long int offset, char minor_allele, char major_allele)
{
	//check recessive & dominant models
	int missing_tolerace_people = (1-Missing_tolerance_threshold)*total_people;
	int model_tolerace_people = (1-Disease_model_threshold)*total_people;
	
	char pattern[CHAR_MAX_LENGTH+1]="";
	int recessive_count = 0;
	int dominant_count = 0;
	int missing_count = 0;
	
	bool flag_recessive = true;
	bool flag_dominant = true;
	
	int getline_round = 1;
	if(offset >= CHAR_MAX_LENGTH) getline_round = (offset/CHAR_MAX_LENGTH)+1;
	int remainder = offset%CHAR_MAX_LENGTH;	
	
	while(ped_file.eof() == false)//check a person each time
	{
		ped_file.getline(pattern, CHAR_MAX_LENGTH+1);//until CHAR_MAX_LENGTH byte
		char* tmp = strtok(pattern, " ");
	if(!tmp) break;	
		for(int i=0;i<5;i++) tmp = strtok(NULL, " ");
		int affection = atoi(tmp);
		
		for(int i=1;i<getline_round;i++)//getline if needed
		{
			ped_file.clear();	
			ped_file.getline(pattern, CHAR_MAX_LENGTH+1);//until CHAR_MAX_LENGTH byte	
		}
		char first = pattern[remainder];
		char second = pattern[remainder+2];
		
		if(first=='0' || second=='0')//allele missing 
		{
			missing_count++;
			if(missing_count > missing_tolerace_people) return " m";
			continue;
		}
			/*
			-9 missing 
			0 missing
			1 unaffected
			2 affected
			*/
		if(affection == 2)
		{
			if(flag_recessive && !(first==minor_allele && second==minor_allele)) 
			{
				recessive_count++;
				if(recessive_count > model_tolerace_people) flag_recessive = false;//不再檢查recessive
			}
			//
			if( flag_dominant && 
				!((first==minor_allele && second==minor_allele) || (first==minor_allele && second==major_allele)) ) 
			{
				dominant_count++;
				if(dominant_count > model_tolerace_people) flag_dominant = false;//不再檢查dominant
			}
		}
		else if(affection == 1)
		{
			if( flag_recessive && 
				!((first==minor_allele && second==major_allele) || (first==major_allele && second==major_allele)) ) 
			{
				recessive_count++;
				if(recessive_count > model_tolerace_people) flag_recessive = false;//不再檢查recessive
			}
			//
			if(flag_dominant && !(first==major_allele && second==major_allele)) 
			{
				dominant_count++;
				if(dominant_count > model_tolerace_people) flag_dominant = false;//不再檢查dominant
			}
		}
		else//affection missing 
		{
			missing_count++;
			if(missing_count > missing_tolerace_people) return " m";
		}
		if(flag_recessive==false && flag_dominant==false) return " n";
	}
	if(flag_recessive==true && flag_dominant==false) return " r";
	else if(flag_recessive==false && flag_dominant==true) return " d";
	else return " d r";//flag_recessive==true && flag_dominant==true
}
void check_disease_model(int total_people, int chr)
{
	char pattern[CHAR_MAX_LENGTH+1]="";
	ifstream fre_result, ped_file;
	ofstream valid_marker;
	
	fre_result.open("fre_result.freq",ios::in);
	
	char buffer[50];
		sprintf (buffer, "CHR%d.ped", chr);
	ped_file.open(buffer,ios::in);
		sprintf (buffer, "valid_marker_chr%d", chr);
	valid_marker.open(buffer,ios::out);
	
	streamoff position_start;
	position_start = ped_file.tellg();
	long int offset = 12;//first 6 columns
	int marker_count = -1;

	while(fre_result.eof() == false)//check a marker each time
	{
		marker_count++;
		char first_allele, second_allele;
		double first_rate, second_rate;
		char minor_allele, major_allele;
		fre_result.getline(pattern, CHAR_MAX_LENGTH+1);//until CHAR_MAX_LENGTH byte
		char* tmp = strtok(pattern, " ");//M
	if(!tmp) break;	
		tmp = strtok(NULL, " ");
		string marker = tmp;
	
		fre_result.getline(pattern, CHAR_MAX_LENGTH+1);
		tmp = strtok(pattern, " ");//A
		tmp = strtok(NULL, " ");
		first_allele = *tmp;
		tmp = strtok(NULL, " ");
		first_rate = atof(tmp);
		
		if(first_rate == 1.0) {valid_marker<<marker<<" u"<<endl;continue;}//don't need to do the checking
		else
		{
			fre_result.getline(pattern, CHAR_MAX_LENGTH+1);
			tmp = strtok(pattern, " ");//A
			tmp = strtok(NULL, " ");
			second_allele = *tmp;
			tmp = strtok(NULL, " ");
			second_rate = atof(tmp);
		}
		
		if(first_rate < second_rate) {minor_allele = first_allele; major_allele = second_allele;}
		else if(first_rate > second_rate) {minor_allele = second_allele; major_allele = first_allele;}
		else if(first_rate == second_rate) {valid_marker<<marker<<" u"<<endl;continue;}//two alleles have the same rate
		
		if(first_allele == '0' || second_allele == '0') {valid_marker<<marker<<" u"<<endl;continue;}//missing allele
		
		offset += marker_count*4;
		string result = parse_ped(total_people,ped_file,offset,minor_allele,major_allele);
		valid_marker<<marker<<result<<endl;
		
		ped_file.clear();
		ped_file.seekg(position_start);
	}
	
	fre_result.close();
	ped_file.close();
	valid_marker.close();
}
/*int main()
{
	check_disease_model(1378,3);
}*/