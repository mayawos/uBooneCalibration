#!/usr/bin/bash

################################
# setup: modify these parameters
################################

# sample name better match the name you gave to ntuple sample
sample="TestValidation"

# runs 4 and 5 are predefined below
# for custom date range enter manually here as "yyyy-mm-dd"
# ISO 8601, see GNU date command "date -I" or see below
start_date=""
end_date=""

run="4" # 4, 5, or MyCustomRun, etc.

if [[ $run == "4" ]]
then
    start_date=2018-09-17
    end_date=2019-07-06
elif [[ $run == "5" ]]
then
    start_date=2019-11-05
    end_date=2020-03-21
fi


# you shouldn't need to manually modify anything below this
#---------------------------------------------------------------------

###########################################
# makes output dirs if they don't exist yet
###########################################
basedir="/uboone/data/users/${USER}/uBooneCalibration/${sample}"
mkdir -p /uboone/data/users/${USER}/uBooneCalibration/${sample}/Ntuples
mkdir -p /uboone/data/users/${USER}/uBooneCalibration/${sample}/Figures
mkdir -p /uboone/data/users/${USER}/uBooneCalibration/${sample}/Histograms
mkdir -p /uboone/data/users/${USER}/uBooneCalibration/${sample}/Output
input=$basedir"/Ntuples/month"

######################################
# generates ROOT files with histograms
######################################

# loop from start to end using $date variable
date=$start_date
# while loop checks using linux time
while [[ $(date -d $date +%s) -le $(date -d $end_date +%s) ]]
do
    m=$(date -d $date +%-m) # month
    d=$(date -d $date +%-d) # day
    y=$(date -d $date +%Y)  # year
    echo root -l -b -q 'X_validation_DataDrivenMap_plane2.C("'$input$m'/daily/XYZcorrection_'$d'", 2,'$y','$m','$d','$d',"'$sample'")'
    date=$(date -I -d "$date + 1 day")
    break
done


#################################
# makes plots from the histograms
#################################

# more importantly the MakePlotsX.C macro adds entries to the output
# text file 'mcc9_1_globalcali_runX_plane...' which contains the
# entries for lifetime and slope day by day

y1=$(date -d $start_date +%Y)
m1=$(date -d $start_date +%-m)
y2=$(date -d $end_date +%Y)
m2=$(date -d $end_date +%-m )

echo $y1 $m1 $y2 $m2

# year 1 until december
echo root -l -b -q 'MakePlotsX.C("'$sample'",'$y1','$m1','12')'
echo root -l -b -q 'MakePlotsX.C("'$sample'",'$y1','$m1','12','false')'

# year 2 from january until m2
echo root -l -b -q 'MakePlotsX.C("'$sample'",'$y2','1','$m2')'
echo root -l -b -q 'MakePlotsX.C("'$sample'",'$y2','1','$m2','false')'


#####################
# makes lifetime plot
#####################

file1=mcc9_1_globalcali_run"$run"_plane2_lifetime_all_planes_efieldcorr_expo_month0plusShutdown_'$sample'.txt
file2=mcc9_1_globalcali_run"$run"_plane2_lifetime_all_planes_efieldcorr_pol1_month0plusShutdown_'$sample'.txt

echo root -l -b -q 'MakeLifetimePlotsFromTable.C("'$file1'","'$file2'.txt",2,false)'
echo mv lifetime_efieldcorr_plane2.pdf lifetime_efieldcorr_plane2_$sample.pdf
