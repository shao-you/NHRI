NHRI
====
Install Pedcut (http://mga.bionet.nsc.ru/soft/)  
Install Merlin (http://www.sph.umich.edu/csg/abecasis/merlin/download/)  
(the names must be 'pedcut' and 'merlin')  

git init  
git clone https://github.com/shao-you/NHRI.git 

put the original file (bed/bim/fam) in the working directory  
set the control file (control.h) for working details  

make ('make clean' is also supported)  
./nhri  

Reference:  
http://www.sph.umich.edu/csg/abecasis/merlin/tour/input_files.html  
http://pngu.mgh.harvard.edu/~purcell/plink/data.shtml  


Pending issues:  
time calculating  
process how many markers each time in 'frequency.cpp'  
marker name: 'chr3:12345'  
only find chrX in EXTERNAL.freq  
put EXTERNAL.freq file into memory for optimization  

