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
#include <ctime>
#include <tr1/functional>
#include <pthread.h>
#include <vector>
#include <algorithm>

#define max_num_of_family 2000
#define max_chr 46//23 pairs
#define CHAR_MAX_LENGTH 9999//max accommodate 40000 people
using namespace std;

//original files
#define BED 					"chr3_1000.bed"//"chr135_merge.bed"//
#define BIM 					"chr3_1000.bim"//"chr135_merge.bim"//
#define FAM 					"chr3_1000.fam"//"chr135_merge.fam"//
#define FREQ 					"ASN_freq.txt"

//generated files
#define _freq       			"fre_result.freq"
//#define _meta 					"metadadaX.ped"
#define _new_map 				"new_map.map"
#define _dat 					"map.dat"
#define _pedcut_fam 			"pedcut.fam"
#define _all_digital_fam 		"all_digital.fam"
#define _tmp_ped 				"tmp.ped"
#define _SubPedigrees 			"SubPedigrees.csv"
#define _PedsSummary 			"PedsSummary.csv"
//#define _chr_ped					"CHRX.ped"
#define _all_digital_align_fam 	all_digital_fam_align.fam

struct parameter
{
	vector<int>* mapping;
	int which_chr;
	int marker_num;
	map< pair<int,int>, int >* ID_affect;
};

//count_people.cpp
int count_people();
//frequency.cpp
//void switch_allele(char* str);
void frequency(int total_people, int** chr, int num_of_chrs);
//map_modify.cpp
int map_modify();
//generate_dat.cpp
int generate_dat(int** chr, int num_of_chrs);
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
int calculate_maxbit(int num_of_families, int** statistic, vector<int>& people_mapping, map< pair<int,int>, int >& ID_affect);
//ftp.cpp
int ftp();
//handle_chr.cpp
void* handle_chr(void* para);
//format_imputed_ped.cpp
void format_imputed_dat_ped(map< pair<int,int>, int >* ID_affect, int chr);
