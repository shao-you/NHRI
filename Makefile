all:stage1 main.o frequency.o map_modify.o generate_dat.o count_chr_number.o do_map.o calculate_maxbit.o ftp.o count_people.o handle_chr.o fam_alignment.o format_imputed_dat_ped.o
stage1:main.o frequency.o map_modify.o generate_dat.o count_chr_number.o do_map.o calculate_maxbit.o ftp.o count_people.o handle_chr.o fam_alignment.o format_imputed_dat_ped.o
	g++ -g -o stage1 main.o frequency.o map_modify.o generate_dat.o count_chr_number.o do_map.o calculate_maxbit.o ftp.o count_people.o handle_chr.o fam_alignment.o format_imputed_dat_ped.o -lpthread
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
	g++ -g -c  calculate_maxbit.cpp
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
clean:
	rm *.o
	rm stage1
