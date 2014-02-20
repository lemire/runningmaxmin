Fast running maximum-minimum filters implemented in C++.
========================================================== 


This code implements the algorithms described in the following paper:

Daniel Lemire, Streaming Maximum-Minimum Filter Using No More than 
Three Comparisons per Element. Nordic Journal of Computing, 13 (4), pages 328-339, 2006. 

A preprint is available there:

http://arxiv.org/abs/cs.DS/0610046

Usage
----- 

To reproduce the numbers from the paper, do the following:

  make
  
  ./runningmaxmin --sine 1000000 10000 --windowrange 4 100  --times 1
  
  ./runningmaxmin --white 1000000 --windowrange 4 100  --times 1


