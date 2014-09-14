all:stage
Opt = -O3 
Warning = -w
#http://a7419.pixnet.net/blog/post/57931682-gcc%E5%8F%83%E6%95%B8%E8%A9%B3%E8%A7%A3
#http://stackoverflow.com/questions/9150397/warning-ignoring-return-value-of-system-c
stage:main.o frequency.o map_modify.o generate_dat.o count_chr_number.o do_map.o calculate_maxbit.o ftp.o count_people.o handle_chr.o fam_alignment.o format_imputed_dat_ped.o pick_up_main_cluster.o pedcut_fam_info.o IBD_sharing_Analysis.o check_disease_model.o restore_ped.o merge_ped.o
	g++ -o stage main.o frequency.o map_modify.o generate_dat.o count_chr_number.o do_map.o calculate_maxbit.o ftp.o count_people.o handle_chr.o fam_alignment.o format_imputed_dat_ped.o pick_up_main_cluster.o pedcut_fam_info.o IBD_sharing_Analysis.o check_disease_model.o restore_ped.o merge_ped.o -lpthread
main.o:main.cpp control.h
	g++ -c $(Opt)$(Warning) main.cpp
frequency.o:frequency.cpp control.h
	g++ -c $(Opt)$(Warning) frequency.cpp
map_modify.o:map_modify.cpp control.h
	g++ -c $(Opt)$(Warning) map_modify.cpp
generate_dat.o:generate_dat.cpp control.h
	g++ -c $(Opt)$(Warning) generate_dat.cpp
count_chr_number.o:count_chr_number.cpp control.h
	g++ -c $(Opt)$(Warning) count_chr_number.cpp
do_map.o:do_map.cpp control.h
	g++ -c $(Opt)$(Warning) do_map.cpp
calculate_maxbit.o:calculate_maxbit.cpp control.h
	g++ -c $(Opt)$(Warning) calculate_maxbit.cpp
ftp.o:ftp.cpp control.h
	g++ -c $(Opt)$(Warning) ftp.cpp
count_people.o:count_people.cpp control.h
	g++ -c $(Opt)$(Warning) count_people.cpp
handle_chr.o:handle_chr.cpp control.h
	g++ -c $(Opt)$(Warning) handle_chr.cpp
fam_alignment.o:fam_alignment.cpp control.h
	g++ -c $(Opt)$(Warning) fam_alignment.cpp
format_imputed_dat_ped.o:format_imputed_dat_ped.cpp control.h
	g++ -c $(Opt)$(Warning) format_imputed_dat_ped.cpp
pick_up_main_cluster.o:pick_up_main_cluster.cpp control.h
	g++ -c $(Opt)$(Warning) -std=c++11 pick_up_main_cluster.cpp
pedcut_fam_info.o:pedcut_fam_info.cpp control.h
	g++ -c $(Opt)$(Warning) -std=c++11 pedcut_fam_info.cpp
IBD_sharing_Analysis.o:IBD_sharing_Analysis.cpp control.h
	g++ -c $(Opt)$(Warning) IBD_sharing_Analysis.cpp
check_disease_model.o:check_disease_model.cpp control.h
	g++ -c $(Opt)$(Warning) check_disease_model.cpp
restore_ped.o:restore_ped.cpp control.h
	g++ -c $(Opt)$(Warning) restore_ped.cpp
merge_ped.o:merge_ped.cpp control.h
	g++ -c $(Opt)$(Warning) merge_ped.cpp
clean:
	rm *.o
	rm -rf stage
