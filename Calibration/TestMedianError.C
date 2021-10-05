#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TProfile.h>
#include <TStyle.h>
#include <iostream>
#include <TF1.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TMath.h>
#include <TTree.h>
#include <fstream>
#include <vector>
#include <cmath>
#include <math.h> 
#include <string>
#include <TImage.h>
#include <iomanip>


float GetMedianError( float median, std::vector<float> value );
void TestMedianError( bool verbose=false );

void TestMedianError( bool verbose ) {

  TCanvas *c1 = new TCanvas("c1","The Median Error",200,10,600,400);
  c1->SetGrid();
  
  // Initialize sets and mpv
  std::vector<int> sets = {5,10,100,1000,10000,100000};
  //std::vector<int> sets = {5};
  float true_mpv = 220.;
  int n_population = 1e8;
  int n_universe = 100;

  // Create some histograms.
  std::vector<TH1F*> histos,deltas,errors;
  TString histoName, histoTitle;

  // New root file:
  TFile *file = new TFile("/uboone/data/users/wospakrk/TestError3.root","recreate");

  // Population histogram
  TH1F* population_histo = new TH1F("dQdx_LandauDistribution","Landau Distribution",500,0.,1000.);

  for ( uint k=0; k < sets.size(); k++ ){  
    histoName = Form("Histo_%d_entries", sets[k]);
    histoTitle = Form("dQ/dx Median of %d entries", sets[k]);
    TH1F *histo = new TH1F(histoName,histoTitle,250,0.,500.);  
    histos.push_back(histo); 
    // Delta histogram for each set
    TH1F* delta_histo = new TH1F(Form("Delta_Median_sets_of_%d",sets[k]),Form("Delta Median Sets of %d",sets[k]),100,-20.,20.);
    deltas.push_back(delta_histo);
    // Delta histogram for each set
    TH1F* error_histo = new TH1F(Form("My_MedianErrors_sets_of_%d",sets[k]),Form("Sample Median Errors, Sets of %d",sets[k]),100,-20.,20.);
    errors.push_back(error_histo);
  }
  
  // Fill histograms randomly
  gRandom->SetSeed();
  std::vector<float> prandoms,srandoms;
  std::vector<std::vector<float> > srandom_vec;
  std::vector<std::vector<std::vector<float> > > srandom_unis;

  //Fill Landau distribution with random values
  for ( uint i=0; i < n_population; i++) {
     float r = gRandom->Landau(true_mpv,25);
     prandoms.push_back(r);
     population_histo->Fill(r);
  }

  // Loop over n universes each!
  // Loop over sets
  // sample random values from the population histos
  for ( int n=0; n < n_universe; n++ ){  
    for ( uint k=0; k < sets.size(); k++ ){  
      for ( uint i=0; i < sets[k]; i++) {
	float r = population_histo->GetRandom();
	srandoms.push_back(r);
	histos[k]->Fill(r);
      }
      srandom_vec.push_back(srandoms);
      srandoms.clear();
    }
    srandom_unis.push_back(srandom_vec);
    srandom_vec.clear();
  }
  
  // Test the median:
  for ( uint k=0; k < sets.size(); k++ ){  

    float mean, sum_stderr;
    float population_median = TMath::Median(prandoms.size(),&prandoms[0]);   

    for ( int n=0; n < n_universe; n++ ){

      float median = TMath::Median(srandom_unis[n][k].size(),&srandom_unis[n][k][0]);
      float error_median = GetMedianError(median,srandom_unis[n][k]);
      float error_pop = GetMedianError(population_median,prandoms);
      if(verbose) cout << "sets of " << sets[k] << " : " << "median, population median, (median - population_median), standard error = " <<  median << ", " << population_median << ", " << (median-population_median) << ", " << error_pop << ", " << error_median << std::endl;
      mean+=(median-population_median)*(median-population_median);
      sum_stderr+=error_median;

      //histogram for s_median - p_median
      deltas[k]->Fill(median-population_median);
      errors[k]->Fill(error_median);

    }

    float stddev_pop=std::sqrt(mean/(n_universe-1));
    float stderr_pop=stddev_pop/std::sqrt(n_universe);
    float avg_error_median=sum_stderr/n_universe/n_universe;
    float shift=avg_error_median/stderr_pop;
    cout << "sets of " << sets[k] << " --- " << "population_median, rms (median of set - population median of dist.), rms (standard error of median), median standard error shift from the population median distribution  = " << population_median << ", " << stderr_pop << ", " << avg_error_median << ", " << shift << std::endl;

  } 

  //Write the histogram:
  file->Write();
  file->Close();
  
  //Plot the histograms
  
}

float GetMedianError( float median, std::vector<float> value ){
  
  float sum_sd=0.;
  
  for( std::vector<float>::iterator it = value.begin(); it != value.end(); ++it ){
    float sd = *it - median;
    float sd_sqr = sd*sd;
    sum_sd += sd_sqr;
    //cout << "it, median, sd, sd_sqr, sum_sd = " << *it << ", " << median << ", " << sd << ", " << sd_sqr << ", " <<sum_sd << endl;
  } 
  float sum_sd_n=sum_sd/((float)value.size()-1.0);
  float sum_sq_n_sqrt=sqrt( sum_sd_n );
  float error=sum_sq_n_sqrt/(sqrt((float)value.size()));
  //cout << "sum_sd_n, sum_sq_n_sqrt, error = " << sum_sd_n << ", " <<sum_sq_n_sqrt << ", " << error << std::endl; 
  return error;
}
