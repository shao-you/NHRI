#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <bitset>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <functional>
#include <ctype.h>
#include <sstream>
#include <map>
#include <set>
#include <ctime>
#include <tr1/functional>
#include <pthread.h>
#include <vector>
#include <algorithm>

#define max_num_of_family 2000
#define max_chr 46//23 pairs
#define CHAR_MAX_LENGTH 10000//max accommodate 40000 people
using namespace std;

#define Maxbit 20
//original files
#define BED 					"chr3_1000.bed"//"chr135_merge.bed"//
#define BIM 					"chr3_1000.bim"//"chr135_merge.bim"//
#define FAM 					"chr3_1000.fam"//"chr135_merge.fam"//

#define IBD_threshold 0.5
#define Sharing_threshold 0.8

#define Disease_model_threshold 0.8//每一marker, 須符合人數比例
#define Missing_tolerance_threshold 0.9//每一marker, 非missing人數比例

struct parameter
{
	vector<int>* mapping;
	int which_chr;
	int marker_num;
	map< pair<int,int>, int >* ID_affect;
	set< pair<int, pair<int,int> > >* valid_pair_set;
};

//count_people.cpp
int count_people();
//frequency.cpp
//void switch_allele(char* str);
void frequency(int total_people, int** chr, int num_of_chrs);
//map_modify.cpp
void map_modify();
//generate_dat.cpp
void generate_dat(int** chr, int num_of_chrs);
//count_chr_number.cpp
int count_chr_number(int** chr);//return # of chr
//do_map.cpp
//bool check_digit(const char* buffer);
//size_t hash_value(const char* buffer);
//size_t hash_value_digit(const size_t vle);
int do_map(int** statistic, vector<int>* align_info);
//fam_alignment.cpp
void fam_alignment(vector<int>* align_info, int num_of_families);
//calculate_maxbit.cpp
//int find_corresponding(char* ID, map<string, int>& map_ID);
int calculate_maxbit(int num_of_families, int** statistic, vector<int>& people_mapping,
					map< pair<int,int>, int >& ID_affect, vector<int>& one_cluster_info);
//ftp.cpp
void ftp();
//handle_chr.cpp
void* handle_chr(void* para);
//format_imputed_ped.cpp
void format_imputed_dat_ped(map< pair<int,int>, int >* ID_affect, int chr);
//pick_up_main_cluster.cpp
void pick_up_main_cluster(int** statistic, int num_of_families, vector<int>* align_info, vector<int>& one_cluster_info);
//pedcut_fam_info.cpp
void pedcut_fam_info(int total_people, set< pair<int, pair<int,int> > >& valid_pair_set);
//IBD_sharing_Analysis.cpp
void IBD_sharing_Analysis(set< pair<int, pair<int,int> > >* valid_pair_set, int chr);
//check_disease_model.cpp
void check_disease_model(int total_people, int chr);
//restore_ped.cpp
void restore_ped(vector<int>* mapping, int chr, int num_marker);
//merge_ped.cpp
void merge_ped(int** chr, int num_of_chrs);
