#include "control.h"
/*******************
All Rights Reserved.
by Wu Shao-You, s54088.cs98@g2.nctu.edu.tw
*******************/
int main(int argc, char* argv[])
{
clock_t start, stop, exe_time;
start = clock(); //開始時間
	ifstream control_file;
	control_file.open("control_config",ios::in);
	//parse_control_file();//------------------------------------------------------
	//===============================================
	cout<<"==========0=========="<<endl;
exe_time = clock();	
	int total_people = count_people();
exe_time = clock() - exe_time;
cout << ((double)(exe_time)) / CLOCKS_PER_SEC <<endl;
	//cout<<total_people<<"====";getchar();//1756,959
	//===============================================
	cout<<"==========1=========="<<endl;;
exe_time = clock();		
	map_modify();//input: "chr3_1000.bim", store in "new_map.map"
exe_time = clock() - exe_time;
cout << ((double)(exe_time)) / CLOCKS_PER_SEC <<endl;	
	//===============================================
	cout<<"==========2=========="<<endl;;
	int** chr = new int* [max_chr];//[chr name][# of marker], index is from 0 ~ num_of_chrs-1
exe_time = clock();	
	int num_of_chrs = count_chr_number(chr);
exe_time = clock() - exe_time;
cout << ((double)(exe_time)) / CLOCKS_PER_SEC <<endl;		
	//===============================================
	cout<<"==========3=========="<<endl;;
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
	cout<<"==========4=========="<<endl;;
exe_time = clock();	
	generate_dat(chr,num_of_chrs);//can be implemented in "handle_chr"
exe_time = clock() - exe_time;
cout << ((double)(exe_time)) / CLOCKS_PER_SEC <<endl;	
	//===============================================
	cout<<"==========5=========="<<endl;
exe_time = clock();		
	fam_alignment(align_info,num_of_families);//把同一family的人排在一起, store in "all_digital_fam_align.fam"
exe_time = clock() - exe_time;
cout << ((double)(exe_time)) / CLOCKS_PER_SEC <<endl;		
	//===============================================
	cout<<"==========6=========="<<endl;
	vector<int> one_cluster_info;//儲存original fam和one_cluster fam的對應關係
exe_time = clock();		
	pick_up_main_cluster(statistic, num_of_families, align_info, one_cluster_info);//store in "one_cluster.fam", statistic will be changed
exe_time = clock() - exe_time;
cout << ((double)(exe_time)) / CLOCKS_PER_SEC <<endl;		
	//===============================================	
	/*input: BIM/BED/ASN_freq.txt
	output: fre_result.freq/metadataX.ped*/
	cout<<"==========7=========="<<endl;	
exe_time = clock();	
	//frequency(total_people,chr,num_of_chrs);//store in "fre_result.freq"
exe_time = clock() - exe_time;
cout << ((double)(exe_time)) / CLOCKS_PER_SEC <<endl;	
	//===============================================
	cout<<"==========8=========="<<endl;
	vector<int> people_mapping;//儲存original fam和pedcut fam的對應關係
	map< pair<int,int>, int > ID_affect;//(family,ID) ==> affection
exe_time = clock();
	int total_people_after_pedcut = calculate_maxbit(num_of_families,statistic,people_mapping,ID_affect,one_cluster_info);
exe_time = clock() - exe_time;
cout << ((double)(exe_time)) / CLOCKS_PER_SEC <<endl;	
	cout<<"total_people_after_pedcut: "<<total_people_after_pedcut<<" == "<<people_mapping.size()<<endl;
	//for(int k=0;k<people_mapping.size();k++) cout<<people_mapping[k]<<" ";
	//===============================================
	cout<<"==========9=========="<<endl;
	//read or generate cluster file
	cout<<"Create cluster file (1), "<<endl
		<<"Use the local cluster file type (2): "<<endl;
	//char option = getc(stdin);
	//cout<<option<<"====="<<endl;getchar();
	//ftp connection here, and list the cluster files for user to choose
	//ftp();//選定並下載 XXX.clusters
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
			para[i].which_chr = chr[i][0];
			para[i].marker_num = chr[i][1];	
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
	//merge_files();
	//===============================================
	cout<<"==========11=========="<<endl;
	for(int i=1;i<=num_of_families;i++) delete [] statistic[i];
	delete [] statistic;
	for(int i=0;i<num_of_chrs;i++) delete [] chr[i];
	delete [] chr;
	cout<<"Successfully End!!"<<endl;
	//system("ls");
	control_file.close();
stop = clock(); //結束時間
cout <<"總共時間"<<((double)(stop - start)) / CLOCKS_PER_SEC <<endl;//getchar();
	return 0;
}