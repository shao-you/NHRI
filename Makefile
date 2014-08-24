all:stage1 main.o frequency.o map_modify.o generate_dat.o count_chr_number.o do_map.o calculate_maxbit.o ftp.o count_people.o handle_chr.o fam_alignment.o format_imputed_dat_ped.o pick_up_main_cluster.o pedcut_fam_info.o IBD_sharing_Analysis.o check_disease_model.o
stage1:main.o frequency.o map_modify.o generate_dat.o count_chr_number.o do_map.o calculate_maxbit.o ftp.o count_people.o handle_chr.o fam_alignment.o format_imputed_dat_ped.o pick_up_main_cluster.o pedcut_fam_info.o IBD_sharing_Analysis.o check_disease_model.o
	g++ -g -o stage1 main.o frequency.o map_modify.o generate_dat.o count_chr_number.o do_map.o calculate_maxbit.o ftp.o count_people.o handle_chr.o fam_alignment.o format_imputed_dat_ped.o pick_up_main_cluster.o pedcut_fam_info.o IBD_sharing_Analysis.o check_disease_model.o -lpthread
main.o:main.cpp control.h
	g++ -g -c main.cpp
frequency.o:frequency.cpp control.h
	g++ -g -c frequency.cpp
map_modify.o:map_modify.cpp control.h
	g++ -g -c map_modify.cpp
generate_dat.o:generate_dat.cpp control.h
	g++ -g -c generate_dat.cpp
count_chr_number.o:count_chr_number.cpp control.h
	g++ -g -c count_chr_number.cpp
do_map.o:do_map.cpp control.h
	g++ -g -c do_map.cpp
calculate_maxbit.o:calculate_maxbit.cpp control.h
	g++ -g -c calculate_maxbit.cpp
ftp.o:ftp.cpp control.h
	g++ -g -c ftp.cpp
count_people.o:count_people.cpp control.h
	g++ -g -c count_people.cpp
handle_chr.o:handle_chr.cpp control.h
	g++ -g -c handle_chr.cpp
fam_alignment.o:fam_alignment.cpp control.h
	g++ -g -c fam_alignment.cpp
format_imputed_dat_ped.o:format_imputed_dat_ped.cpp control.h
	g++ -g -c format_imputed_dat_ped.cpp
pick_up_main_cluster.o:pick_up_main_cluster.cpp control.h
	g++ -g -c -std=c++11 pick_up_main_cluster.cpp
pedcut_fam_info.o:pedcut_fam_info.cpp control.h
	g++ -g -c -std=c++11 pedcut_fam_info.cpp
IBD_sharing_Analysis.o:IBD_sharing_Analysis.cpp control.h
	g++ -g -c IBD_sharing_Analysis.cpp
check_disease_model.o:check_disease_model.cpp control.h
	g++ -g -c check_disease_model.cpp
clean:
	rm *.o
	rm stage1
