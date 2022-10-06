#define X_validation_DataDrivenMap_plane2_cxx
#include "X_validation_DataDrivenMap_plane2.h"
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
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <fstream>
#include <vector>
#include <cmath>
#include <math.h> 
#include <string>
#include <TImage.h>
#include <iomanip>

#ifdef WIN32
  #define unsigned_long_long unsigned __int256
  #define long_long __int256
#else // gcc. Might not work on other compilers!
  #define unsigned_long_long unsigned long long int
  #define long_long long long int
#endif

//helper function
float GetMedianError( float median, std::vector<float> dqdx );
void SetHistoContent( std::vector<std::vector<float> > value, std::vector<float>& all_value, TH1F* hist );
void SetCorrHistoContent( std::vector<std::vector<float> > value, float global_median, TH1F* histerr, TH1F* histcorr );
TString getDir( const std::string& subdir );

void X_validation_DataDrivenMap_plane2::Loop(int corr, int i_year, int i_month, int i_date, int f_date, std::string sample)
{
  std::cout << "///////////////////////////////////// Generating X correction histogrmas for different dates and all planes ///////////////////////////////////" << std::endl;
  
  ifstream infile;
  infile.open("goodruns_intersec_all_runs_mcc9_run4.txt");
  std::vector<int> good_run_vec;
  std::vector<long_long> good_evt_vec;
  long_long good_runs;
  infile>>good_runs;
  while(good_runs!=-1){
    good_run_vec.push_back(good_runs);
    infile>>good_runs;
  }
  infile.close();
  const double t_bin_size=100.0; 
  const double hittime_bin_size=100.0; 
  const double x_bin_size=10.0; 
  const double y_bin_size=5.0; 
  const double z_bin_size=5.0; 
  
  for(int plane_number=2; plane_number<3; plane_number++){
    std::cout  << "********** MONTH : " << i_month << " ******************" << std::endl;
    for( int my_date=i_date; my_date < f_date+1; my_date++ ){
      std::vector<TH1F*> dq_dx_t_hists, dq_dx_t_error_hists, corrected_dq_dx_t_hists;
      std::vector<TH1F*> dq_dx_hittime_hists, dq_dx_hittime_error_hists, corrected_dq_dx_hittime_hists;
      std::vector<TH1F*> dq_dx_x_hists, dq_dx_x_error_hists, corrected_dq_dx_x_hists;
      std::vector<TH1F*> theta_xz_x_hists, theta_yz_x_hists;
      
      //create new directory if it hasn't existed already
      //TString subdir = getDir( Form("/uboone/data/users/wospakrk/uBooNECalibration/DataMaps/DataMap_%s", sample.c_str()) );
      TString subdir = getDir( "/uboone/data/users/wospakrk/uBooNECalibration/DataMaps/DataMap_FinalMCC9CalibRun3" );
      TString histdir;
      if( corr==0 ) histdir = getDir( Form("%s/final_calibration_root_files_datadrivenmap_nocorr_month0",subdir.Data()) );
      else if( corr==1 ) histdir = getDir( Form("%s/final_calibration_root_files_datadrivenmap_spacecorr_month0",subdir.Data()) );
      else if( corr==2 ) histdir = getDir( Form("%s/final_calibration_root_files_datadrivenmap_efieldcorr_month0",subdir.Data()) );
      else if( corr==6 ) histdir = getDir( Form("%s/final_calibration_root_files_datadrivenmap_efieldcorr_month0",subdir.Data()) );
      else{
	cout << "YOU NEED TO PROVIDE CORRECTION!" << endl;
	return 1;
      }
      
      TFile my_file(Form("%s/YZ_correction_factors_2016_month_0_plane_%d.root",histdir.Data(),plane_number));
      TH2F *month_hist  = (TH2F*)my_file.Get(Form("error_dq_dx_z_vs_y_hist_plane%d",plane_number));
      
      TString subdirx = getDir( Form("/uboone/data/users/wospakrk/DataMap_%s_plane%d", sample.c_str(),plane_number) );
      TString histdirx;
      if( corr==0 ) histdirx = getDir( Form("%s/final_calibration_root_files_datadrivenmap_nocorr_month%d",subdirx.Data(),i_month) );
      else if( corr==1 ) histdirx = getDir( Form("%s/final_calibration_root_files_datadrivenmap_spacecorr_month%d",subdirx.Data(),i_month) );
      else if( corr==2 ) histdirx = getDir( Form("%s/final_calibration_root_files_datadrivenmap_efieldcorr_month%d",subdirx.Data(),i_month) );
      else if( corr==6 ) histdirx = getDir( Form("%s/final_calibration_root_files_datadrivenmap_efieldcorr_nospatial_month%d",subdirx.Data(),i_month) );
      else{
	cout << "YOU NEED TO PROVIDE CORRECTION!" << endl;
	return 1;
      }
      
      TFile *x_file = new TFile( Form("%s/X_correction_factors_%d_month_%d_%d_plane_%d.root",histdirx.Data(),i_year,i_month,my_date,plane_number),"recreate");
      cout << Form("%s/X_correction_factors_%d_month_%d_%d_plane_%d.root",histdirx.Data(),i_year,i_month,my_date,plane_number) << endl;     
      
      TH1F *dq_dx_x_hist = new TH1F(Form("dq_dx_x_hist_plane%d",plane_number),Form("plane_%d",plane_number),26,0,260);
      dq_dx_x_hist->GetXaxis()->SetTitle("X Coordinate(cm)");
      dq_dx_x_hist->SetStats(0);
      
      TH1F *dq_dx_x_error_hist = new TH1F(Form("dq_dx_x_error_hist_plane%d",plane_number),Form("plane_%d",plane_number),26,0,260);
      dq_dx_x_error_hist->GetXaxis()->SetTitle("X Coordinate (cm)");
      dq_dx_x_error_hist->SetStats(0);
      
      TH1F *corrected_dq_dx_x_hist = new TH1F(Form("corrected_dq_dx_x_hist_plane%d",plane_number),Form("plane_%d",plane_number),26,0,260);
      corrected_dq_dx_x_hist->GetXaxis()->SetTitle("X Coordinate (cm)");
      corrected_dq_dx_x_hist->SetStats(0);
      
      //drift time
      TH1F *dq_dx_t_hist = new TH1F(Form("dq_dx_t_hist_plane%d",plane_number),Form("plane %d",plane_number),24,0,2400);
      dq_dx_t_hist->GetXaxis()->SetTitle("Drift time (#mus)");
      dq_dx_t_hist->SetStats(0);
      
      TH1F *dq_dx_t_error_hist = new TH1F(Form("dq_dx_t_error_hist_plane%d",plane_number),Form("plane %d",plane_number),24,0,2400);
      dq_dx_t_error_hist->GetXaxis()->SetTitle("Drift time (#mus)");
      dq_dx_t_error_hist->SetStats(0);
      
      TH1F *corrected_dq_dx_t_hist = new TH1F(Form("corrected_dq_dx_t_hist_plane%d",plane_number),Form("plane %d",plane_number),24,0,2400);
      corrected_dq_dx_t_hist->GetXaxis()->SetTitle("Drift time (#mus)");
      corrected_dq_dx_t_hist->SetStats(0);
      
      //local hit time
      TH1F *dq_dx_hittime_hist = new TH1F(Form("dq_dx_hittime_hist_plane%d",plane_number),Form("plane %d",plane_number),26,0,2600);
      dq_dx_hittime_hist->GetXaxis()->SetTitle("Local Hit Time (#mus)");
      dq_dx_hittime_hist->SetStats(0);
      
      TH1F *dq_dx_hittime_error_hist = new TH1F(Form("dq_dx_hittime_error_hist_plane%d",plane_number),Form("plane %d",plane_number),26,0,2600);
      dq_dx_hittime_error_hist->GetXaxis()->SetTitle("Local Hit Time (#mus)");
      dq_dx_hittime_error_hist->SetStats(0);
      
      TH1F *corrected_dq_dx_hittime_hist = new TH1F(Form("corrected_dq_dx_hittime_hist_plane%d",plane_number),Form("plane %d",plane_number),26,0,2600);
      corrected_dq_dx_hittime_hist->GetXaxis()->SetTitle("Local Hit Time (#mus)");
      corrected_dq_dx_hittime_hist->SetStats(0);
      
      //as a function of drift time
      for( int i=0; i < 30; i++ ){
	dq_dx_t_hists.push_back( new TH1F(Form("dq_dx_t_hist_bin%d_plane%d",i+1,plane_number),Form("plane %d, drift time bin %d",plane_number,i+1),100,0,1500.));
	dq_dx_t_hists.back()->GetXaxis()->SetTitle("drift time(#mus)");
	//dq_dx_t_hists.back()->SetStats(0);
      }
      for( int i=0; i < 30; i++ ){
	dq_dx_hittime_hists.push_back( new TH1F(Form("dq_dx_hittime_hist_bin%d_plane%d",i+1,plane_number),Form("plane %d, drift time bin %d",plane_number,i+1),100,0,1500.));
	dq_dx_hittime_hists.back()->GetXaxis()->SetTitle("local hit time(#mus)");
	//dq_dx_t_hists.back()->SetStats(0);
      }
      //as a function of drift distance
      for( int i=0; i < 30; i++ ){
	dq_dx_x_hists.push_back( new TH1F(Form("dq_dx_x_hist_bin%d_plane%d",i+1,plane_number),Form("plane %d, drift distance bin %d",plane_number,i+1),100,0,1000.));
	dq_dx_x_hists.back()->GetXaxis()->SetTitle("dQ/dx (ADC)");
	//dq_dx_x_hists.back()->SetStats(0);
      }
      //as a function of drift distance
      for( int i=0; i < 26; i++ ){
	theta_xz_x_hists.push_back( new TH1F(Form("theta_xz_x_hist_bin%d_plane%d",i+1,plane_number),Form("plane %d, drift distance bin %d",plane_number,i+1),64,-3.2,3.2));
	theta_xz_x_hists.back()->GetXaxis()->SetTitle("#theta_{XZ} (rad)");
	//theta_xz_x_hists.back()->SetStats(0);
      }
      //as a function of drift distance
      for( int i=0; i < 26; i++ ){
	theta_yz_x_hists.push_back( new TH1F(Form("theta_yz_x_hist_bin%d_plane%d",i+1,plane_number),Form("plane %d, drift distance bin %d",plane_number,i+1),64,-3.2,3.2));
	theta_yz_x_hists.back()->GetXaxis()->SetTitle("#theta_{YZ} (rad)");
	//theta_yz_x_hists.back()->SetStats(0);
      }
      
      TH1F *histo_dq_dx_x = new TH1F(Form("histos_dq_dx_plane%d",plane_number),Form("plane %d, drift distance",plane_number),100,0,1000.);
      histo_dq_dx_x->GetXaxis()->SetTitle("dQ/dx (ADC)");

	//dq_dx_x_hists.back()->SetStats(0);
      TH1F *res_spatialcorr =  new TH1F(Form("Res_dQdx_spatialcorr_plane%d",plane_number),Form("plane %d",plane_number),200,100.,500.);
      res_spatialcorr->GetXaxis()->SetTitle("dQdx_{spatial} - dQdx_{no corr}");
      
      TH1F *res_efieldcorr =  new TH1F(Form("Res_dQdx_efieldcorr_plane%d",plane_number),Form("plane %d",plane_number),200,100.,500.);
      res_efieldcorr->GetXaxis()->SetTitle("dQdx_{E field} - dQdx_{no corr}");
	
      TH1F *res_efield_spatial_corr =  new TH1F(Form("Res_dQdx_efield_spatial_corr_plane%d",plane_number),Form("plane %d",plane_number),200,100.,500.);
      res_efield_spatial_corr->GetXaxis()->SetTitle("dQdx_{E field} - dQdx_{spatial}");
	
      TH1F *fracres_spatialcorr =  new TH1F(Form("FracRes_dQdx_spatialcorr_plane%d",plane_number),Form("plane %d",plane_number),200,100.,500.);
      fracres_spatialcorr->GetXaxis()->SetTitle("(dQdx_{spatial} - dQdx_{no corr})/dQdx_{spatial}");
      
      TH1F *fracres_efieldcorr =  new TH1F(Form("FracRes_dQdx_efieldcorr_plane%d",plane_number),Form("plane %d",plane_number),200,100.,500.);
      fracres_efieldcorr->GetXaxis()->SetTitle("(dQdx_{E field} - dQdx_{no corr})/dQdx_{E field}");
      
      TH1F *fracres_efield_spatial_corr =  new TH1F(Form("FracRes_dQdx_efield_spatial_corr_plane%d",plane_number),Form("plane %d",plane_number),200,100.,500.);
      fracres_efield_spatial_corr->GetXaxis()->SetTitle("(dQdx_{E field} - dQdx_{spatial})/dQdx_{E field}");
      
      float upper_angle;
      float lower_angle;
      
      if(plane_number==2){
	upper_angle=1.745;lower_angle=1.396;
      }
      else if(plane_number==1){
	upper_angle=0.873;lower_angle=0.175;
      }
      else if(plane_number==0){
	upper_angle=2.792;lower_angle=2.007;
      }
      
      vector<vector<float>> dq_dx_value, dq_dx_t_value, dq_dx_hittime_value;
      vector<float> all_dq_dx_value, all_dq_dx_t_value, all_dq_dx_hittime_value;
      dq_dx_value.resize(30);
      dq_dx_t_value.resize(30);
      dq_dx_hittime_value.resize(30);
      
      TH1D* h_trkhitxSCE = new TH1D("trkhitxSCE","trkhitxSCE",250,0.,250.);

      int ang_cross=0;
      if(fChain == 0) return;
      Long64_t nentries = fChain->GetEntriesFast();
      Long64_t nbytes = 0, nb = 0;
      for(Long64_t jentry=0; jentry<nentries;jentry++) {
	Long64_t ientry = LoadTree(jentry);
	if (ientry < 0) break;
	nb=fChain->GetEntry(jentry);   nbytes += nb;
	bool run_good = (std::find( good_run_vec.begin(), good_run_vec.end(), run) != good_run_vec.end())? true : false;
	long_long evt = (run*1e9)+(subrun*1e5)+event;
	bool evt_good = (std::find( good_evt_vec.begin(), good_evt_vec.end(), evt) != good_evt_vec.end())? true : false;
        if(jentry%1000==0) std::cout << jentry << "/" << nentries << std::endl;
	int year=year_month_date/10000;
	int month_date=year_month_date%10000;
	int month=month_date/100;
	int date=month_date%100;
	if(!run_good) continue;
	if(year != i_year ) continue;
	if( month != i_month ) continue;
        if(date != my_date ) continue; 
	
	for(int k=0; k<cross_trks; ++k){
	      
          if( xprojectedlen[k] < 250.) continue; 
	  if(!((TMath::Abs(trackthetaxz[k])>1.31) && (TMath::Abs(trackthetaxz[k])<1.83))){
	    
	    if(!((TMath::Abs(trackthetayz[k])<upper_angle) && (TMath::Abs(trackthetayz[k])>lower_angle))){
	      ang_cross++;
	      
	      for(int j=0; j<TMath::Min(ntrkhits[k][plane_number],3000); ++j){
		h_trkhitxSCE->Fill(trkhitxSCE[k][plane_number][j]); 
		if((trkhitxSCE[k][plane_number][j]<260)&&(trkhitxSCE[k][plane_number][j]>0)){
		  
		  if((trkhitySCE[k][plane_number][j]<120)&&(trkhitySCE[k][plane_number][j]>-120)){
		    
		    if((trkhitzSCE[k][plane_number][j]<1040)&&(trkhitzSCE[k][plane_number][j]>0)){
		      int t_bin=int(trklocaltime[k][plane_number][j])/t_bin_size;
		      int hittime_bin=int(trk_hit_time[k][plane_number][j])/hittime_bin_size;
		      int z_bin=int(trkhitzSCE[k][plane_number][j])/z_bin_size; 
		      int x_bin=int(trkhitxSCE[k][plane_number][j])/x_bin_size;
		      int y_bin;
		      if(trkhitySCE[k][plane_number][j]<0) y_bin=int(trkhitySCE[k][plane_number][j])/y_bin_size+23;
		      if(trkhitySCE[k][plane_number][j]>=0) y_bin=int(trkhitySCE[k][plane_number][j])/y_bin_size+24;
		      float yz_correction_factor;
		      yz_correction_factor=month_hist->GetBinContent(z_bin+1,y_bin+1);
		      float dqdx = trkdqdx[k][plane_number][j];
		      if(corr==1) dqdx = trkdqdxSCE[k][plane_number][j];
		      if(corr==2) dqdx = trkdqdx_efieldcorr[k][plane_number][j];
		      float corrected_dq_dx=dqdx*yz_correction_factor;
                      yz_correction_factor=1.0;
		      if(corrected_dq_dx!=0) dq_dx_value[x_bin].push_back(corrected_dq_dx);
		      if(corrected_dq_dx!=0) dq_dx_x_hists[x_bin]->Fill(corrected_dq_dx);
		      if(corrected_dq_dx!=0) histo_dq_dx_x->Fill(corrected_dq_dx);
		      if(corrected_dq_dx!=0) dq_dx_t_value[t_bin].push_back(corrected_dq_dx);
		      if(corrected_dq_dx!=0) dq_dx_t_hists[t_bin]->Fill(corrected_dq_dx);
		      if(corrected_dq_dx!=0) theta_xz_x_hists[x_bin]->Fill(trackthetaxz[k]);
		      if(corrected_dq_dx!=0) theta_yz_x_hists[x_bin]->Fill(trackthetayz[k]);
		      res_efield_spatial_corr->Fill(trkdqdx[k][plane_number][j] - trkdqdx[k][plane_number][j] );
		      res_spatialcorr->Fill(trkdqdx[k][plane_number][j] - trkdqdx[k][plane_number][j] );
		      res_efieldcorr->Fill(trkdqdx[k][plane_number][j] - trkdqdx[k][plane_number][j] );
		      fracres_efield_spatial_corr->Fill((trkdqdx[k][plane_number][j] - trkdqdx[k][plane_number][j])/trkdqdx[k][plane_number][j]);
		      fracres_spatialcorr->Fill((trkdqdx[k][plane_number][j] - trkdqdx[k][plane_number][j])/trkdqdx[k][plane_number][j] );
		      fracres_efieldcorr->Fill((trkdqdx[k][plane_number][j] - trkdqdx[k][plane_number][j])/trkdqdx[k][plane_number][j] );
		      
		    } // z coordinate
		    
		  } // y coordinate
		  
		} // x coordinate
	
	      } // loop over hits
		  
	    } // yz angle cut
		
	  } // xz angle cut
	      
	} // loop over crossing  tracks
	  
      } // loop over jentries
	
      SetHistoContent( dq_dx_value, all_dq_dx_value, dq_dx_x_hist );
      SetHistoContent( dq_dx_t_value, all_dq_dx_t_value, dq_dx_t_hist );
      SetHistoContent( dq_dx_hittime_value, all_dq_dx_hittime_value, dq_dx_hittime_hist );
      
      float global_median_dq_dx=TMath::Median(all_dq_dx_value.size(),&all_dq_dx_value[0]); 
      float global_median_dq_dx_t=TMath::Median(all_dq_dx_t_value.size(),&all_dq_dx_t_value[0]); 
      float global_median_dq_dx_hittime=TMath::Median(all_dq_dx_hittime_value.size(),&all_dq_dx_hittime_value[0]); 
      
      SetCorrHistoContent( dq_dx_value, global_median_dq_dx, dq_dx_x_error_hist, corrected_dq_dx_x_hist );
      SetCorrHistoContent( dq_dx_t_value, global_median_dq_dx_t, dq_dx_t_error_hist, corrected_dq_dx_t_hist );
      SetCorrHistoContent( dq_dx_hittime_value, global_median_dq_dx_t, dq_dx_hittime_error_hist, corrected_dq_dx_hittime_hist );
      
      dq_dx_x_hist->Write();
      dq_dx_x_error_hist->Write();
      corrected_dq_dx_x_hist->Write();
      
      dq_dx_t_hist->Write();
      dq_dx_t_error_hist->Write();
      corrected_dq_dx_t_hist->Write();
      
      dq_dx_hittime_hist->Write();
      dq_dx_hittime_error_hist->Write();
      corrected_dq_dx_hittime_hist->Write();
     
      for( int i=0; i < 30; i++ ){
	dq_dx_t_hists[i]->Write();
	dq_dx_hittime_hists[i]->Write();
	dq_dx_x_hists[i]->Write();
      }
    
      x_file->Write();
      x_file->Close();
      
    } // loop over plane numbers
    
    std::cout << "//////////////////////////////////// Done with generating all the x correction histograms /////////////////////////////////////" << std::endl;
  }// loop over dates
  
}

float GetMedianError( float median, std::vector<float> dqdx ){
  
  float sum_sd=0.;
  
  for( std::vector<float>::iterator it = dqdx.begin(); it != dqdx.end(); ++it ){
    if( std::isinf(*it) ) continue; //prevent inf or nan values
    float sd = *it - median;
    float sd_sqr = sd*sd;
    sum_sd += sd_sqr;
  }
  
  float sum_sd_n=sum_sd/((float)dqdx.size()-1.0);
  float sum_sq_n_sqrt=sqrt( sum_sd_n );
  float error=sum_sq_n_sqrt/(sqrt((float)dqdx.size()));
  
  return error;
}

void SetHistoContent( std::vector<std::vector<float> > value, std::vector<float>& all_value, TH1F* hist ){
  for(int i=0; i<value.size(); i++){
    if(value[i].size()>5){
      for(int k=0; k<value[i].size(); k++){
	all_value.push_back(value[i][k]);
      }
      float local_median=TMath::Median(value[i].size(),&value[i][0]);
      float error_median=GetMedianError(local_median, value[i]);
      
      hist->SetBinContent(i+1,local_median);
      hist->SetBinError(i+1,error_median);
    }
  }
  
}

void SetCorrHistoContent( std::vector<std::vector<float> > value, float global_median, TH1F* histerr, TH1F* histcorr ){
  
  std::vector<std::vector<float> > frac_error;
  frac_error.resize(30);
  
  for(int i=0; i<value.size(); i++){
    if( value[i].size()>5 ){
      float local_median=TMath::Median(value[i].size(),&value[i][0]);
      float fractional_error=float(global_median)/local_median;
      histerr->SetBinContent(i+1,fractional_error);
      frac_error[i].push_back(fractional_error);
    }
  }
  
  for(int i=0; i<value.size(); i++){
    if(value[i].size()>5){
      float local_median=TMath::Median(value[i].size(),&value[i][0]);
      float corrected=local_median*frac_error[i][0];
      float errorcorr=GetMedianError(corrected, value[i]);
      histcorr->SetBinContent(i+1,corrected);
      histcorr->SetBinError(i+1,errorcorr);
    }
  }
}

TString getDir( const std::string& subdir )
{
  
  TString getdir( subdir );
  
  if( 0 != system( Form( "test -d %s", getdir.Data() ) ) )
    {
      std::cout << "histos directory does not exist, making one now.... " << std::endl;
      int madedir = system( Form( "mkdir -m 755 -p %s", getdir.Data() ) );
      if( 0 != madedir )
	std::cout << "HistoDir, Could not make plot directory, " << getdir << std::endl;
    }
  
  return getdir;
}
