NHRI
====
Install Pedcut (http://mga.bionet.nsc.ru/soft/)  
Install Merlin (http://www.sph.umich.edu/csg/abecasis/merlin/download/)  
(the names must be 'pedcut' and 'merlin')  

git init  
git clone https://github.com/shao-you/NHRI.git 

put the original file (bed/bim/fam) in the working directory
	(*)Every input/output should be Unix format.
	
configure the control file (control.h) for working details:  
	Freq_file
		"0", 不使用external freq。 
		"ftp", 若下載失敗，要求輸入"0" or "local_file"。若輸入"local_file"卻開檔失敗，不使用external freq。 
		"local_file", Ex: "ASN.freq"
	
	Cluster_file
		"generate", 由merlin產生cluster file。 
		"ftp", 若下載失敗，要求輸入"local_file"。若輸入"local_file"卻開檔失敗，由merlin產生cluster file。 
		"local_file", 表示分別為每條染色體輸入外部檔名。
		(*)一檔名僅供一染色體(a thread)使用，多條染色體時需多次輸入。
	
	Linkage 
		"", 不進行linkage。
		"--CMD1 --CMD2 ...", CMD could be "npl", "pairs", "qtl", "deviates", "vc"
	
	IBD_Sharing (0/1)
		IBD_threshold 0.5
		Sharing_threshold 0.8
	
	Disease_model (0/1)
		Disease_model_threshold 0.8//每一marker, 須符合人數比例
		Missing_tolerance_threshold 0.9//每一marker, 必須非missing人數比例

compile & run
	make ('make clean' is also supported)  
	./nhri (# of threads depends on # of chromosomes) 
	
outputs: (X: chromosome name)
	"original.fam" -> "all_digital.fam" -> "all_digital_align.fam" -> "one_cluster.fam" -> "pedcut.fam"
	"new_map.map"
	"fre_result.freq"
	"family_pedcut_info" (record the mapping of new and old family IDs while pedcut executes)
	"CHRX.dat", CHRX.ped"
	
	(*)some files will be stored in different directories (Eg: dir_X)
	"CHRX.clusters"
	Imputation: "CHRX_infer.ped", "CHRX_infer.dat" 
		(*)Imputation is time consuming. 
	Linkage Analysis: "CHRX_linkage.txt" 
	IBD Sharing Analysis: "notable_grid_chrX" 
	Disease Model: "valid_marker_chrX" 
		m:   affection/allele missing 
		n:   d/r皆不符合 
		u:   忽略 
		d:   d符合 
		r:   r符合 
		d r: d/r皆符合 
		(*)If imputation is performed, the program uses "CHRX_infer.ped" as input; otherwise, uses "CHRX.ped". 
	Merge imputed file (imputation is performed): "merged_infer.ped"

Reference:  
	http://www.sph.umich.edu/csg/abecasis/merlin/tour/input_files.html  
	http://pngu.mgh.harvard.edu/~purcell/plink/data.shtml  

Pending issues:  
	time calculating  
	process how many markers each time in 'frequency.cpp'  
	marker name: 'chr3:12345'  
	only find chrX in EXTERNAL.freq  
	put EXTERNAL.freq file into memory for optimization  

