#include "control.h"
/*******************
All Rights Reserved.
by Wu Shao-You, s54088.cs98@g2.nctu.edu.tw
*******************/
int main(int argc, char* argv[])
{
clock_t start, stop, exe_time;
start = clock(); //開始時間
	//===============================================
	cout<<"==========0=========="<<endl;
	int** chr = new int* [max_chr];//[chr name][# of marker], index is from 0 ~ num_of_chrs-1
exe_time = clock();	
	int num_of_chrs = count_chr_number(chr);
exe_time = clock() - exe_time;
cout << ((double)(exe_time)) / CLOCKS_PER_SEC <<endl;		
	//===============================================
	cout<<"==========1=========="<<endl;;
exe_time = clock();		
	get_ext_files(chr,num_of_chrs);//input: "chr3_1000.bim", store in "new_map.map"
exe_time = clock() - exe_time;
cout << ((double)(exe_time)) / CLOCKS_PER_SEC <<endl;	
	//===============================================	
	cout<<"==========2=========="<<endl;;
exe_time = clock();		
	map_modify();//input: "chr3_1000.bim", store in "new_map.map"
exe_time = clock() - exe_time;
cout << ((double)(exe_time)) / CLOCKS_PER_SEC <<endl;	
	//===============================================
	cout<<"==========3=========="<<endl;;
exe_time = clock();	
	int total_people = count_people();
exe_time = clock() - exe_time;
cout << ((double)(exe_time)) / CLOCKS_PER_SEC <<endl;	
	//===============================================
	cout<<"==========4=========="<<endl;;
	int** statistic = new int* [total_people];//[# of people][# of non-founder], index is from 1 ~ num_of_families
	vector<int> align_info[max_num_of_family+1];//儲存original fam和align fam的對應關係, index is from 1 ~ max_num_of_family
exe_time = clock();		
	int num_of_families = do_map(statistic,align_info);
exe_time = clock() - exe_time;
cout << ((double)(exe_time)) / CLOCKS_PER_SEC <<endl;		
	/*adjust all families, store in "all_digital.fam", 
	This program is to adjust the fam file. 
	Check whether the first 4 columns are digital or not.*/
	//===============================================
	cout<<"==========5=========="<<endl;;
exe_time = clock();	
	generate_dat(chr,num_of_chrs);//can be implemented in "handle_chr"
exe_time = clock() - exe_time;
cout << ((double)(exe_time)) / CLOCKS_PER_SEC <<endl;	
	//===============================================
	cout<<"==========6=========="<<endl;
exe_time = clock();		
	fam_alignment(align_info,num_of_families);//把同一family的人排在一起, store in "all_digital_fam_align.fam"
exe_time = clock() - exe_time;
cout << ((double)(exe_time)) / CLOCKS_PER_SEC <<endl;		
	//===============================================
	cout<<"==========7=========="<<endl;
	vector<int> one_cluster_info;//儲存original fam和one_cluster fam的對應關係
exe_time = clock();		
	pick_up_main_cluster(statistic, num_of_families, align_info, one_cluster_info);//store in "one_cluster.fam", statistic will be changed
exe_time = clock() - exe_time;
cout << ((double)(exe_time)) / CLOCKS_PER_SEC <<endl;		
	//===============================================	
	/*input: BIM/BED/ASN.freq
	output: fre_result.freq/metadataX.ped*/
	cout<<"==========8=========="<<endl;	
exe_time = clock();	
	frequency(total_people,chr,num_of_chrs);//store in "fre_result.freq"
exe_time = clock() - exe_time;
cout << ((double)(exe_time)) / CLOCKS_PER_SEC <<endl;	
	//===============================================
	/*output: pedcut.fam/family_pedcut_info*/
	cout<<"==========9=========="<<endl;
	vector<int> people_mapping;//儲存original fam和pedcut fam的對應關係
	map< pair<int,int>, int > ID_affect;//(family,ID) ==> affection
exe_time = clock();
	int total_people_after_pedcut = calculate_maxbit(num_of_families,statistic,people_mapping,ID_affect,one_cluster_info);
exe_time = clock() - exe_time;
cout << ((double)(exe_time)) / CLOCKS_PER_SEC <<endl;	
	cout<<"total_people_after_pedcut: "<<total_people_after_pedcut<<" == "<<people_mapping.size()<<endl;
	//for(int k=0;k<people_mapping.size();k++) cout<<people_mapping[k]<<" ";
	//===============================================
	cout<<"==========10=========="<<endl;
	set< pair<int, pair<int,int> > > valid_pair_set;
	pedcut_fam_info(total_people_after_pedcut,valid_pair_set);
	/*******************
	thread start!!!
	for each chromosome
	*******************/
	pthread_t tid[num_of_chrs];
	struct parameter para[num_of_chrs];
	for(int i=0;i<num_of_chrs;i++)
	{
		//cout<<chr[i][0]<<" == "<<chr[i][1]<<endl;
			para[i].mapping = &people_mapping;
			para[i].chr = chr;
			para[i].chr_index = i;	
			para[i].ID_affect = &ID_affect;
			para[i].valid_pair_set = &valid_pair_set;
		pthread_create(&tid[i], NULL, handle_chr, (void*)&para[i]);//default joinable ??
		//cout<<"tid: "<<tid[i]<<"==="<<endl;getchar();
		//pthread_join(tid[i],NULL);
	}
	for(int k=0;k<num_of_chrs;k++) pthread_join(tid[k],NULL);
	/*******************
	thread end!!!
	for each chromosome
	*******************/
	if(Impute == 1) merge_infer_ped(chr,num_of_chrs);
	//===============================================
	cout<<"==========11=========="<<endl;
	for(int i=1;i<=num_of_families;i++) delete [] statistic[i];
	delete [] statistic;
	for(int i=0;i<num_of_chrs;i++) delete [] chr[i];
	delete [] chr;
	
	extern char** Cluster_ext_result;
	delete [] Cluster_ext_result;
	cout<<"Successfully End!!"<<endl;
stop = clock(); //結束時間
cout <<"總共時間"<<((double)(stop - start)) / CLOCKS_PER_SEC <<endl;//getchar();
	return 0;
}