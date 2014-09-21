#include "control.h"

bool check_marker(int marker, vector<int>& index_reduced_marker)
{
	int size = index_reduced_marker.size();
	for(int i=0;i<size;i++) if(index_reduced_marker[i] == marker) return true;
	return false;
}
string parse_ped(int total_people, ifstream& ped_file, int offset_count, char minor_allele, char major_allele)
{//minor_allele,major_allele不為'0'
	//check recessive & dominant models
	int missing_tolerance_people = (1-Missing_tolerance_threshold)*total_people;
	int model_tolerance_people = (1-Disease_model_threshold)*total_people;
	
	char pattern[CHAR_MAX_LENGTH+1]="";
	int recessive_count = 0;
	int dominant_count = 0;
	int missing_count = 0;
	
	bool flag_recessive = true;
	bool flag_dominant = true;
	
	while(ped_file.eof() == false)//check a person each time
	{
		streampos start = ped_file.tellg();
		ped_file.getline(pattern, CHAR_MAX_LENGTH+1);//until CHAR_MAX_LENGTH byte
		char* tmp = strtok(pattern, " ");
	if(!tmp) break;	
		for(int i=0;i<5;i++) tmp = strtok(NULL, " ");
		int affection = atoi(tmp);
		streamoff offset = ((tmp+strlen(tmp)+1) - pattern) + offset_count*4;
		ped_file.clear();
		ped_file.seekg(start);
		ped_file.seekg(offset, ped_file.cur);
		
		char first = ped_file.get();
		ped_file.get();//' '
		char second = ped_file.get();
		
		do{
			ped_file.clear();
			ped_file.getline(pattern, CHAR_MAX_LENGTH+1);
		}while((ped_file.rdstate() & std::ifstream::failbit ) != 0);//把剩下getline完(至少還剩' ')

		if(first=='0' || second=='0')//allele missing 
		{
			missing_count++;
			if(missing_count > missing_tolerance_people) return " m";
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
				if(recessive_count > model_tolerance_people) flag_recessive = false;//不再檢查recessive
			}
			//
			if( flag_dominant && 
				!((first==minor_allele && second==minor_allele) || (first==minor_allele && second==major_allele)) ) 
			{
				dominant_count++;
				if(dominant_count > model_tolerance_people) flag_dominant = false;//不再檢查dominant
			}
		}
		else if(affection == 1)
		{
			if( flag_recessive && 
				!((first==minor_allele && second==major_allele) || (first==major_allele && second==major_allele)) ) 
			{
				recessive_count++;
				if(recessive_count > model_tolerance_people) flag_recessive = false;//不再檢查recessive
			}
			//
			if(flag_dominant && !(first==major_allele && second==major_allele)) 
			{
				dominant_count++;
				if(dominant_count > model_tolerance_people) flag_dominant = false;//不再檢查dominant
			}
		}
		else//affection missing 
		{
			missing_count++;
			if(missing_count > missing_tolerance_people) return " m";
			continue;
		}
		if(flag_recessive==false && flag_dominant==false) return " n";
	}
	if(flag_recessive==true && flag_dominant==false) return " r";
	else if(flag_recessive==false && flag_dominant==true) return " d";
	else return " d r";//flag_recessive==true && flag_dominant==true
}
void check_disease_model(int total_people, int chr, vector<int>& index_reduced_marker)
{
	char pattern[CHAR_MAX_LENGTH+1]="";
	ifstream fre_result, ped_file;
	ofstream valid_marker;
	
	fre_result.open("fre_result.freq",ios::in);
	
	char buffer[50];
		if(Impute == 1) sprintf (buffer, "dir_%d/CHR%d_infer.ped", chr, chr);
		else if(Impute == 0) sprintf (buffer, "CHR%d.ped", chr);
	ped_file.open(buffer,ios::in);
		sprintf (buffer, "dir_%d/valid_marker_chr%d", chr, chr);
	valid_marker.open(buffer,ios::out);
	
	streamoff position_start;
	position_start = ped_file.tellg();
	int marker_count = 0;
	int offset_count = -1;
	
	while(fre_result.eof() == false)//check a marker each time
	{
		marker_count++;
		offset_count++;
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
		
		if(first_rate == 1.0)//don't need to do the checking 
		{
			if(Impute == 1)
				if(check_marker(marker_count,index_reduced_marker)) {offset_count--;continue;}
			valid_marker<<marker<<" u"<<endl;continue;
		}
		else
		{
			fre_result.getline(pattern, CHAR_MAX_LENGTH+1);
			tmp = strtok(pattern, " ");//A
			tmp = strtok(NULL, " ");
			second_allele = *tmp;
			tmp = strtok(NULL, " ");
			second_rate = atof(tmp);
			
			if(Impute == 1)
				if(check_marker(marker_count,index_reduced_marker)) {offset_count--;continue;}
		}
		
		if(first_rate < second_rate) {minor_allele = first_allele; major_allele = second_allele;}
		else if(first_rate > second_rate) {minor_allele = second_allele; major_allele = first_allele;}
		else if(first_rate == second_rate) {valid_marker<<marker<<" u"<<endl;continue;}//two alleles have the same rate
		
		if(first_allele == '0' || second_allele == '0') {valid_marker<<marker<<" u"<<endl;continue;}//missing allele

		string result = parse_ped(total_people,ped_file,offset_count,minor_allele,major_allele);
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
	vector<int> index_reduced_marker;
	calculate_reduced_marker(3,index_reduced_marker);
	cout<<"reduced number: "<<index_reduced_marker.size()<<endl;
	check_disease_model(1366,3,index_reduced_marker);
}*/