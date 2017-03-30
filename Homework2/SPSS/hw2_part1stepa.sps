﻿* Encoding: UTF-8.


GET DATA
  /TYPE=XLSX
  /FILE=
    'C:\Users\oliviabnewton\Documents\GitHub\IDS6938-SimulationTechniques\Homework2\HW2_Part1_StepA.x'+
    'lsx'
  /SHEET=name 'Raw Results'
  /CELLRANGE=FULL
  /READNAMES=ON
  /DATATYPEMIN PERCENTAGE=95.0
  /HIDDEN IGNORE=YES.
EXECUTE.
DATASET NAME DataSet1 WINDOW=FRONT.

DATASET ACTIVATE DataSet1.
DESCRIPTIVES VARIABLES=Mersenne Knuth MinimalStandard Ranlux DefaultREngine Sobol
  /STATISTICS=MEAN STDDEV VARIANCE MIN MAX.

NONPAR CORR
  /VARIABLES=Mersenne Knuth MinimalStandard Ranlux DefaultREngine Sobol
  /PRINT=SPEARMAN TWOTAIL NOSIG
  /MISSING=PAIRWISE.



CORRELATIONS
  /VARIABLES=Mersenne Knuth MinimalStandard Ranlux DefaultREngine Sobol
  /PRINT=TWOTAIL NOSIG
  /STATISTICS DESCRIPTIVES
  /MISSING=PAIRWISE.

EXAMINE VARIABLES=Mersenne Knuth MinimalStandard Ranlux DefaultREngine Sobol
  /PLOT NPPLOT
  /STATISTICS DESCRIPTIVES
  /CINTERVAL 95
  /MISSING LISTWISE
  /NOTOTAL.
