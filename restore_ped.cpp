#include "control.h"

void restore_ped(int chr)
{
	char pattern[CHAR_MAX_LENGTH+1]="";
	ifstream infer_ped, family_pedcut_info, original_ped, input_ped;//CHR3_infer.ped, family_pedcut_info, all_digit.fam
	ofstream restore_ped;//restore_chr3.ped

	char buffer[50];
		sprintf (buffer, "CHR%d_infer.ped", chr);
	infer_ped.open(buffer,ios::in);
	family_pedcut_info.open("family_pedcut_info",ios::in);
	original_ped.open("all_digit.fam",ios::in);
		sprintf (buffer, "metadata%d.ped", chr);
	input_ped.open(buffer,ios::in);
		sprintf (buffer, "restore_chr%d.ped", chr);
	restore_ped.open(buffer,ios::out);
	
	
	infer_ped.close();
	family_pedcut_info.close();
	original_ped.close();
	input_ped.close();
	restore_ped.close();
}
int main()
{
	restore_ped(3);
	cout<<"successfully"<<endl;
}