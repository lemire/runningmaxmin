Fast running maximum-minimum filters implemented in C++.
========================================================== 


This code implements the algorithms described in the following paper:

Daniel Lemire, Streaming Maximum-Minimum Filter Using No More than 
Three Comparisons per Element. Nordic Journal of Computing, 13 (4), pages 328-339, 2006. 

A preprint is available there:

http://arxiv.org/abs/cs.DS/0610046


Contributors: Daniel Lemire, Kai Wolf

Usage
----- 

To reproduce the numbers from the paper, do the following:

  make
  
  ./runningmaxmin --sine 1000000 10000 --windowrange 4 100  --times 1
  
  ./runningmaxmin --white 1000000 --windowrange 4 100  --times 1

Suitability 
------------

The new algorithm introduced in the manuscript is most suitable for piecewise monotonic
data or when low-latency is required. Otherwise, Gil-Kimmel and van Herk
are good choices.

See also
---------

For an application of this idea to rolling statistics in JavaScript, see

https://github.com/shimondoodkin/efficient-rolling-stats

For an application in Go, please see 

https://github.com/notnot/movingminmax
