#!/bin/bash

# first hadd the tuples
# no SCE in Sim
if [[ $5 != "calibration" && $4 == "mc" ]]; then
  echo "creating directory to hadd the tuples for noSCE in simulation sample"
  if [[ ! -d /uboone/data/users/${USER}/dQdxCrossingMu_noSCE ]]; then
    mkdir /uboone/data/users/${USER}/dQdxCrossingMu_noSCE
  fi
  if [[ ! -d /uboone/data/users/${USER}/dQdxCrossingMu_noSCE/$4 ]]; then
    mkdir /uboone/data/users/${USER}/dQdxCrossingMu_noSCE/$4/$1
  fi
  if [[ ! -d /uboone/data/users/${USER}/dQdxCrossingMu_noSCE/$4/$1 ]]; then
    mkdir /uboone/data/users/${USER}/dQdxCrossingMu_noSCE/$4/$1
  fi
  if [[ ! -f /uboone/data/users/${USER}/dQdxCrossingMu_noSCE/$4/$1/XYZcorrection.root ]]; then
    hadd -f /uboone/data/users/${USER}/dQdxCrossingMu_noSCE/$4/$1/XYZcorrection.root $2*/*/XYYcorrection.root
  fi
fi

## SCE in Sim
echo "creating directory to hadd the tuples for SCE in simulation sample"
if [[ ! -d /uboone/data/users/${USER}/dQdxCrossingMu ]]; then
  mkdir /uboone/data/users/${USER}/dQdxCrossingMu
fi
if [[ ! -d /uboone/data/users/${USER}/dQdxCrossingMu/$1 ]]; then
  mkdir /uboone/data/users/${USER}/dQdxCrossingMu/$1
fi
if [[ ! -d /uboone/data/users/${USER}/dQdxCrossingMu/$4/$1 ]]; then
  mkdir /uboone/data/users/${USER}/dQdxCrossingMu/$4/$1
fi
if [[ ! -f /uboone/data/users/${USER}/dQdxCrossingMu/$4/$1/XYZcorrection.root ]]; then
  hadd -f /uboone/data/users/${USER}/dQdxCrossingMu/$4/$1/XYZcorrection.root $2*/*/XYZcorrection.root
fi

if [[ $5 == "calibration" ]]; then
  echo "only creating the Full SCE corrected sample..." 
  root -b -q 'Y_Z_calibration_DataDrivenMap.C("/uboone/data/users/'${USER}'/dQdxCrossingMu/'${4}'/'${1}'/XYZcorrection", 2, 2016, "'${3}'", "'${4}'", "'${5}'")';
  root -b -q 'X_calibration_DataDrivenMap.C("/uboone/data/users/'${USER}'/dQdxCrossingMu/'${4}'/'${1}'/XYZcorrection", 2, 2016, "'${3}'", "'${4}'", "'${5}'")';
fi


if [[ $5 == "validation" ]]; then
  echo "creating all the SCE correction variations samples..." 
  if [[ $4 == "mc" ]]; then
    root -b -q 'Y_Z_calibration_DataDrivenMap.C("/uboone/data/users/'${USER}'/dQdxCrossingMu_noSCE/'${4}'/'${1}'/XYZcorrection", 3, 2016, "'${3}'", "'${4}'", "'${5}'")';
  fi
  root -b -q 'Y_Z_calibration_DataDrivenMap.C("/uboone/data/users/'${USER}'/dQdxCrossingMu/'${4}'/'${1}'/XYZcorrection", 0, 2016, "'${3}'", "'${4}'", "'${5}'")';
  root -b -q 'Y_Z_calibration_DataDrivenMap.C("/uboone/data/users/'${USER}'/dQdxCrossingMu/'${4}'/'${1}'/XYZcorrection", 1, 2016, "'${3}'", "'${4}'", "'${5}'")';
  root -b -q 'Y_Z_calibration_DataDrivenMap.C("/uboone/data/users/'${USER}'/dQdxCrossingMu/'${4}'/'${1}'/XYZcorrection", 2, 2016, "'${3}'", "'${4}'", "'${5}'")';
  if [[ $4 == "mc" ]]; then
    root -b -q 'X_calibration_DataDrivenMap.C("/uboone/data/users/'${USER}'/dQdxCrossingMu_noSCE/'${4}'/'${1}'/XYZcorrection", 3, 2016, "'${3}'", "'${4}'", "'${5}'")';
  fi
  root -b -q 'X_calibration_DataDrivenMap.C("/uboone/data/users/'${USER}'/dQdxCrossingMu/'${4}'/'${1}'/XYZcorrection", 0, 2016, "'${3}'", "'${4}'", "'${5}'")';
  root -b -q 'X_calibration_DataDrivenMap.C("/uboone/data/users/'${USER}'/dQdxCrossingMu/'${4}'/'${1}'/XYZcorrection", 1, 2016, "'${3}'", "'${4}'", "'${5}'")';
  root -b -q 'X_calibration_DataDrivenMap.C("/uboone/data/users/'${USER}'/dQdxCrossingMu/'${4}'/'${1}'/XYZcorrection", 2, 2016, "'${3}'", "'${4}'", "'${5}'")';
fi


#Now create the validation plots:
root -b -q 'MakeValidationPlots.C("'${3}'")'
root -b -q 'PlotOverlayTheta.C("'${3}'")'

## can add more macros below
# ......
# .....
# ....
# ...
