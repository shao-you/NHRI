#include "control.h"

void fam_alignment(vector<int>* align_info, int num_of_families)
{	
	//int num_of_families = align_info.size();
	//cout<<"# of families: "<<num_of_families;getchar();
	char pattern[CHAR_MAX_LENGTH+1]="";
	
	ofstream all_digital_fam_align;
	all_digital_fam_align.open("all_digital_fam_align.fam",ios::out);
	
	ifstream all_digital_fam;
	all_digital_fam.open("all_digital.fam",ios::in);
	
	/*for(int i=1;i<=num_of_families;i++)
	{
		int size = align_info[i].size();
		for(int j=0;j<size;j++) cout<<align_info[i][j]<<" ";
		cout<<"---------------------"<<endl;
	}
	cout<<"============================="<<endl;getchar();*/
	/*//--version1
	for(int i=1;i<=num_of_families;i++)
	{
		all_digital_fam.clear();
		all_digital_fam.seekg(0, all_digital_fam.beg);

		int size = align_info[i].size();
		int pivot, last_line = 0;
		for(pivot=0;align_info[i][pivot] != 0;pivot++);

		for(int j=pivot-1;j>=0;j--)
		{
			int line = (align_info[i][j])-last_line;
			for(int k=0;k<line;k++) all_digital_fam.getline(pattern, CHAR_MAX_LENGTH);
			all_digital_fam_align<<pattern<<endl;
			last_line = align_info[i][j];
		}
		
		all_digital_fam.clear();
		all_digital_fam.seekg(0, all_digital_fam.beg);
		last_line = 0;
		for(int j=pivot+1;j<=size-1;j++)
		{
			int line = (align_info[i][j])-last_line;
			for(int k=0;k<line;k++) all_digital_fam.getline(pattern, CHAR_MAX_LENGTH);
			all_digital_fam_align<<pattern<<endl;
			last_line = align_info[i][j];
		}
	}
	*/
	
	//--version2
	for(int i=1;i<=num_of_families;i++)
	{
		all_digital_fam.clear();
		all_digital_fam.seekg(0, all_digital_fam.beg);

		int size = align_info[i].size();
		int last_line = 0;

		for(int j=0;j<size;j++)
		{
			int line = (align_info[i][j])-last_line;
			for(int k=0;k<line;k++) all_digital_fam.getline(pattern, CHAR_MAX_LENGTH);
			all_digital_fam_align<<pattern<<endl;
			last_line = align_info[i][j];
		}
	}
	all_digital_fam_align.close();
	all_digital_fam.close();
}
