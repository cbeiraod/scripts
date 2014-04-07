scripts
=======

Several scripts I've written over time for several small tasks

lamda24:
  Scripts associated with the measurement of angular parameters lamda2 and lamda4 in a two body decay
  -bgs_study_old.cc: Background study
  -bgs_study.cc: Started to rewrite previous code, with some improvements and in oo style. Not finished yet
  -spin_study.cc: Generate plots for some of the variables

LHEtoAODSIM:
  Scripts for producing AODSIM output from LHE files, created when working on the stau-stau analysis

MyValidator:
  A module for CMSSW to plot some basic quatities from AODSIM files.
  Also a set of macros to export these plots to images and combine several of the plots in a single histogram
