# uBooneCalibration

This repository is meant to hold all of the standalone ROOT macros related to the dQ/dx calibration and validation with space charge correction.

A bash script, called <b>runYZvalidation.sh</b>, is provided to create all the necessary histograms and plots for the calibration once the crossing muons tuples are available. In order to run the script, there are 3 parameters used as an input parameters:
<ul>
<li> Version of validation. You can use "v1", "v2", "v3", etc. This is mostly done to ensure a proper bookkeeping for all the different studies/variations. </li>
<li> Output directory of the tuples. This is the directory where the tuples are located in. An example of how the direcory will look like is as follows: <pre>/pnfs/uboone/scratch/users/wospakrk/CrossingMuPitchCorr_take2/v08_00_00_12/CrossingMuPitchCorr@ </pre>
<li> Name of the output directory where you will keep your histogram files. You just need to supply a folder name. The directory itself will be created for you under the @/uboone/data/users/$USER directory@. Here as an example, I am using the "ExampleHistos" as the name of the folder.</li>
</ul>

Putting it all together, the command to run this:
<pre> . runYZvalidation.sh v1 /pnfs/uboone/scratch/users/wospakrk/CrossingMuPitchCorr_take2/v08_00_00_12/CrossingMuPitchCorr ExampleHistos </pre>

The main macros used to create the histograms are:
<ul>
<li>The macros used to create the YZ maps and correction factors map to get a uniform response across the YZ direction of the detector:
<dev> <b>Y_Z_calibration_DataDrivenMap.C </b> and its header file <b>Y_Z_calibration_DataDrivenMap.h </b> </dev></li>
<li>The macros used to create the dQ/dx as a function of drift direction and drift time. This is the main macros used to create the histograms for the daily lifetime:
<dev> <b>X_calibration_DataDrivenMap.C </b> and its header file <b>X_calibration_DataDrivenMap.h </b> </dev></li>
</ul>

Macros that are used to make all the calibration and validation plots:

<ul>
  <li> <b>MakePlotsYZ.C</b> -- creates the 2d YZ map plots and the 1d projection of the map.</li>
  <li> <b>MakePlotsX.C</b> -- creates the dQ/dx vs drift direction plots.</li>
  <li> <b>MakeLifetimePlotsFromTable.C</b> -- creates the daily lifetime plots from the table created while running the X_calibration_DataDrivenMap.C</li>
  <li> <b>MakeValidationPlots.C</b> -- create various plots to see the diffent shapes after applying the space and electric field corrections.</li>
  <li> <b>PlotOverlayTheta.C</b> -- create plot of median dQ/dx as a function of the drift direction separated by the angle of the crossing tracks</li>
  <li> <b>CreateNewRootTemplate.C</b> -- create a new root file containing the correction factors from all 3 planes. The database macros and overlay requires specific naming and this macro provides the correct histograms name formatting depending on data or MC.</li>
</ul>

Macros for dQ/dx YZ map overlay validation:
<ul>
  <li> <b>FillEmptyBins.C</b> -- fill in the empty bins in the YZ map with the average correction factors from the neighboring bins. Should be used after the <b>CreateNewRootTemplate.C</b></li>
  <li> <b>FillEmptyBinsWithOne.C</b> -- in case you want to fill the empty bins in the YZ map with 1 instead of the average correction factors from the neighboring bins.
</ul>
Macros for the systematics study:
<ul>
  <li> <b>TestMedianError.C</b> -- evaluate the median error assigned to the median dQ/dx. It uses toy data to evaluate how close the mean, median, and RMS with the current calculation for the error. 
</ul>
