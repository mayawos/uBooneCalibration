#define X_calibration_DataDrivenMap_cxx
#include "X_calibration_DataDrivenMap.h"
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


//helper function
void SetHistoContent( std::vector<std::vector<float> > value, std::vector<float>& all_value, TH1F* hist );
void SetExtraHistoContent( std::vector<std::vector<float> > value, TH1F* hist );
void SetCorrHistoContent( std::vector<std::vector<float> > value, float global_median, TH1F* histerr, TH1F* histcorr );
void SetHistoThetaCutContent( std::vector<std::vector<std::vector<float> > > value, std::vector<std::vector<float> >& all_value, std::vector<TH1F*> hists );
void SetTrackMedianHistoContent( std::vector<std::vector<float> > value, TH1F* hist );
float GetMedianError( float median, std::vector<float> dqdx );
void GetThetaCuts(int plane_number, float &upper_angle_xz1, float &lower_angle_xz1, float &upper_angle_yz1, float &lower_angle_yz1, float &upper_angle_xz2, float &lower_angle_xz2, float &upper_angle_yz2, float &lower_angle_yz2 );

std::vector<int> GetGoodRun(int run, std::string& RunPeriod);
std::string GetRunPeriod(int run);
TString GetDir( const std::string& subdir );
TString GetHistDir( int corr, TString subdir );
void MakePretty1DHistos( TH1F *hist1D, const char* xaxis_title, const char* yaxis_title, int stat=-1);


//global variables
std::vector<std::string> corrections = {"nocorr","spacecorr","efieldcorr","noSCE","cali","caliSCE"};
bool useDefault = true;
bool run_is_good = true;

void X_calibration_DataDrivenMap::Loop(int corr, std::string sample, int runnumber, std::string type)
{

  cout << "Run number = " << runnumber << endl; 
  gStyle->SetOptStat(1101);
  std::string RunPeriod = "MC";
  std::vector<int> good_run_vec;
  if( type == "data" ) good_run_vec = GetGoodRun(runnumber,RunPeriod);

  //create new directory if it hasn't existed already
  TString subdir = GetDir( Form("/uboone/data/users/%s/%s_%s", getenv("USER"), RunPeriod.c_str(), sample.c_str()) );
  TString histdir = GetHistDir( corr, subdir );
  std::cout << "histdir: " << histdir << std::endl;


  //specify the binning
  const double t_bin_size=100.0; 
  const double x_bin_size=10.0; 
  const double y_bin_size=5.0; 
  const double z_bin_size=5.0; 
  const double phi_bin_size=10.0; 
  const double theta_bin_size=10.0; 
  
  std::vector<float> thetayz_low; 
  std::vector<float> thetayz_high;
  std::vector<float> thetaxz_low; 
  std::vector<float> thetaxz_high;
 
  for( int a=0; a<18; ++a){
    thetayz_low.push_back(a*(TMath::Pi()/18)); 
    thetayz_high.push_back((a+1)*(TMath::Pi()/18));
    thetaxz_low.push_back(a*(TMath::Pi()/18));
    thetaxz_high.push_back((a+1)*(TMath::Pi()/18));
  }

  for(int plane_number=0; plane_number<3; plane_number++){
    std::vector<TH1F*> dq_dx_t_hists, dq_dx_t_error_hists, corrected_dq_dx_t_hists, dqdx_bin;
    
    // ================================== START CREATING HISTOS FOR CALIBRATION =============================================
   
    //Fetch the YZ correction map 
    TFile my_file(Form("%s/YZ_correction_factors_plane_%d.root",histdir.Data(),plane_number));
    cout << (Form("%s/YZ_correction_factors_plane_%d.root",histdir.Data(),plane_number)) << endl; 
    TH2F *yzcorr_hist  = (TH2F*)my_file.Get(Form("error_dq_dx_z_vs_y_hist_plane%d",plane_number));

    //Create new TFile to store all the histos related to drift direction/time
    TFile *x_file = new TFile(Form("%s/X_correction_factors_plane_%d.root",histdir.Data(),plane_number), "RECREATE");
    
    TH1F *dq_dx_x_error_hist = new TH1F(Form("dq_dx_x_error_hist_plane%d",plane_number),Form("plane_%d",plane_number),26,0,260);
    MakePretty1DHistos(dq_dx_x_error_hist,"X Coordinate (cm)", "median dQ/dx of track");
    
    TH1F *dq_dx_phi_hist = new TH1F(Form("dq_dx_phi_hist_plane%d",plane_number),Form("plane_%d",plane_number),18,0,180);
    MakePretty1DHistos(dq_dx_phi_hist, "#Phi (deg)", "median dE/dx of track");
    
    TH1F *dq_dx_theta_hist = new TH1F(Form("dq_dx_theta_hist_plane%d",plane_number),Form("plane_%d",plane_number),18,0,180);
    MakePretty1DHistos(dq_dx_theta_hist,"#theta (deg)","median dQ/dx of track");
    
    TH1F *de_dx_phi_hist = new TH1F(Form("de_dx_phi_hist_plane%d",plane_number),Form("plane_%d",plane_number),18,0,180);
    MakePretty1DHistos(de_dx_phi_hist,"#Phi (deg)","median dE/dx of track");
    
    TH1F *de_dx_theta_hist = new TH1F(Form("de_dx_theta_hist_plane%d",plane_number),Form("plane_%d",plane_number),18,0,180);
    MakePretty1DHistos(de_dx_theta_hist,"#theta (deg.)","median dE/dx of track");
    
    TH1F *dq_dx_x_hist = new TH1F(Form("dq_dx_x_hist_plane%d",plane_number),Form("plane_%d",plane_number),26,0,260);
    MakePretty1DHistos(dq_dx_x_hist,"X Coordinate (cm)","median dQ/dx of track");

    TH1F *de_dx_x_hist = new TH1F(Form("de_dx_x_hist_plane%d",plane_number),Form("plane_%d",plane_number),26,0,260);
    MakePretty1DHistos(de_dx_x_hist,"X Coordinate (cm)","median dE/dx of track");

    TH1F *corrected_dq_dx_x_hist = new TH1F(Form("corrected_dq_dx_x_hist_plane%d",plane_number),Form("plane_%d",plane_number),26,0,260);
    MakePretty1DHistos(corrected_dq_dx_x_hist,"X Coordinate (cm)","median dQ/dx of track");
    
    //overlay theta cuts plots 
    std::vector<TH1F*> histos_thetaxz, histos_thetayz;
    for( int ang=0; ang <thetaxz_low.size() ; ++ang ){
      TH1F *dq_dx_x_hist_xz = new TH1F(Form("dq_dx_x_hist_thetaxz_cut%d_plane%d",ang,plane_number),Form("plane_%d",plane_number),26,0,260);
      MakePretty1DHistos(dq_dx_x_hist_xz,"X Coordinate(cm)","median dQ/dx of track");
      histos_thetaxz.push_back(dq_dx_x_hist_xz);
      
      TH1F *dq_dx_x_hist_yz = new TH1F(Form("dq_dx_x_hist_thetayz_cut%d_plane%d",ang,plane_number),Form("plane_%d",plane_number),26,0,260);
      MakePretty1DHistos(dq_dx_x_hist_yz,"X Coordinate(cm)","median dQ/dx of track");
      histos_thetayz.push_back(dq_dx_x_hist_yz);
    }
    
    //drift time
    TH1F *dq_dx_t_hist = new TH1F(Form("dq_dx_t_hist_plane%d",plane_number),Form("plane %d",plane_number),24,0,2400);
    MakePretty1DHistos(dq_dx_t_hist,"Drift time (#mus)","median dQ/dx of track");
    
    TH1F *dq_dx_t_error_hist = new TH1F(Form("dq_dx_t_error_hist_plane%d",plane_number),Form("plane %d",plane_number),24,0,2400);
    MakePretty1DHistos(dq_dx_t_error_hist,"Drift time (#mus)","median dQ/dx of track");
    
    TH1F *corrected_dq_dx_t_hist = new TH1F(Form("corrected_dq_dx_t_hist_plane%d",plane_number),Form("plane %d",plane_number),24,0,2400);
    MakePretty1DHistos(corrected_dq_dx_t_hist,"Drift time (#mus)","median dQ/dx of track");
    
    //as a function of drift distance
    for( int i=0; i < 26; i++ ){
      dqdx_bin.push_back( new TH1F(Form("dq_dx_x_hist_bin%d_plane%d",i+1,plane_number),Form("plane %d, drift distance bin %d",plane_number,i+1),80,0.,400.));
      MakePretty1DHistos(dqdx_bin.back(),"drift distance [cm]","median dQ/dx of track");
    }
    
    //as a function of drift time
    for( int i=0; i < 24; i++ ){
      dq_dx_t_hists.push_back( new TH1F(Form("dq_dx_t_hist_bin%d_plane%d",i+1,plane_number),Form("plane %d, drift time bin %d",plane_number,i+1),100,0,1500.));
      MakePretty1DHistos(dq_dx_t_hists.back(),"drift time(#mus)","median dQ/dx of track");
    }
    
    TH1F *dq_dx_hist_trk =  new TH1F(Form("dq_dx_hist_trk_plane%d",plane_number),Form("plane %d",plane_number),200,0.,400);
    MakePretty1DHistos(dq_dx_hist_trk,"median dQ/dx of track","median dQ/dx of track");
    
    TH1F *de_dx_hist_trk =  new TH1F(Form("de_dx_hist_trk_plane%d",plane_number),Form("plane %d",plane_number),100,0.,10);
    MakePretty1DHistos(de_dx_hist_trk,"median dE/dx of track","median dE/dx of track");
    
    float upper_angle, upper_anglexz, upper_angleyz, upper_angle_xz;
    float lower_angle, lower_anglexz, lower_angleyz, lower_angle_xz;
    
    vector<vector<vector<float>>> dq_dx_thetaxz_value, dq_dx_thetayz_value;
    vector<vector<float>> dq_dx_value, dq_dx_t_value;
    vector<vector<float>> dq_dx_theta_value, dq_dx_phi_value;
    vector<vector<float>> de_dx_value, de_dx_t_value;
    vector<vector<float>> de_dx_theta_value, de_dx_phi_value;
    vector<vector<float>> all_dq_dx_value_thetaxz, all_dq_dx_value_thetayz;
    vector<float> all_dq_dx_value, all_dq_dx_t_value;
    vector<float> dq_dx_value_trkhit;
    vector<float> de_dx_value_trkhit;
    vector< vector<float> > dq_dx_value_per_track;
    vector< vector<float> > de_dx_value_per_track;
    
    dq_dx_value.resize(26);
    dq_dx_t_value.resize(24);
    dq_dx_theta_value.resize(18);
    dq_dx_phi_value.resize(18);
    de_dx_value.resize(26);
    de_dx_theta_value.resize(18);
    de_dx_phi_value.resize(18);

    dq_dx_thetaxz_value.resize(thetaxz_low.size()); 
    dq_dx_thetayz_value.resize(thetayz_low.size());
    all_dq_dx_value_thetaxz.resize(thetaxz_low.size()); 
    all_dq_dx_value_thetayz.resize(thetayz_low.size());
 
    for( int i=0; i < dq_dx_thetaxz_value.size(); ++i ){
      dq_dx_thetaxz_value[i].resize(26); 
      dq_dx_thetayz_value[i].resize(26); 
    }

    int ang_cross=0;
    if(fChain == 0) return;
    Long64_t nentries = fChain->GetEntriesFast();
    Long64_t nbytes = 0, nb = 0;
    for(Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb=fChain->GetEntry(jentry);   nbytes += nb;
      if(jentry%1000==0) cout<<jentry<<"/"<<nentries<<endl;
      if( type == "data" ){
	run_is_good=false;
	for(int i_1=0; i_1<good_run_vec.size(); i_1++){
	  if(run==good_run_vec[i_1]){
	    run_is_good=true;
	    break;
	  } 
	}
      }
      
      if( !run_is_good ) continue;
      
      for(int k=0; k<cross_trks; ++k){
	
	if( xprojectedlen[k] < 250. ) continue;
	
	float upper_angle_xz1, upper_angle_xz2, upper_angle_yz1, upper_angle_yz2;
	float lower_angle_xz1, lower_angle_xz2, lower_angle_yz1, lower_angle_yz2;
	
	//theta cuts by default is the same as MCC8 and MCC9.0
	//additional cuts are added for MCC9.1 due to tracks covering wider thetaxz vs thetayz phase space in plane 0 and plane 2 to be badly reconstructed
	GetThetaCuts(plane_number, upper_angle_xz1, lower_angle_xz1, upper_angle_yz1, lower_angle_yz1, upper_angle_xz2, lower_angle_xz2, upper_angle_yz2, lower_angle_yz2);	  
	
	if(!((lower_angle_xz1<TMath::Abs(trackthetaxz[k])) && (TMath::Abs(trackthetaxz[k])<=upper_angle_xz1)) && !((lower_angle_xz2<TMath::Abs(trackthetaxz[k])) && (TMath::Abs(trackthetaxz[k]) <= upper_angle_xz2 ))){
	  if(!(((lower_angle_yz1<TMath::Abs(trackthetayz[k]))) && (TMath::Abs(trackthetayz[k])<=upper_angle_yz1)) && !((lower_angle_yz2<TMath::Abs(trackthetayz[k])) && (TMath::Abs(trackthetayz[k])<=upper_angle_yz2))){
	    ang_cross++;
	    float hitx;
	    float hity; 
	    float hitz;	
	    for(int j=0; j<TMath::Min(ntrkhits[k][plane_number],3000); ++j){
	      if( corr==0 || corr==3 || corr==4 ){
		hitx = trkhitx[k][plane_number][j];
		hity = trkhity[k][plane_number][j];
		hitz = trkhitz[k][plane_number][j];
	      }
	      else if( corr==1 || corr==2 || corr==5 ){
		hitx = trkhitxSCE[k][plane_number][j];
		hity = trkhitySCE[k][plane_number][j];
		hitz = trkhitzSCE[k][plane_number][j];
	      }
	      if((hitx<260)&&(hitx>0)){
		if((hity<120)&&(hity>-120)){
		  if((hitz<1040)&&(hitz>0)){
		    
		    int t_bin=int(hitx/0.1098)/t_bin_size;
		    int z_bin=int(hitz)/z_bin_size; 
		    int x_bin=int(hitx)/x_bin_size;
		    int y_bin;
		    int theta_bin=int((TMath::Abs(trackthetayz[k])*TMath::RadToDeg())/theta_bin_size);
		    int phi_bin=int((TMath::Abs(trackthetaxz[k])*TMath::RadToDeg())/phi_bin_size);
		    
		    if(hity<0) y_bin=int(hity)/y_bin_size+23;
		    if(hity>=0) y_bin=int(hity)/y_bin_size+24;
		    
		    float yz_correction_factor;
		    yz_correction_factor=yzcorr_hist->GetBinContent(z_bin+1,y_bin+1);
		    float dqdx = trkdqdx[k][plane_number][j];
		    float dedx = trkdedx[k][plane_number][j];
		    float resrange = trkresrange[k][plane_number][j];
		    if( corr==1 ) dqdx = trkdqdxSCE[k][plane_number][j];
		    else if( corr==2 ) dqdx = trkdqdx_efieldcorr[k][plane_number][j];
		    else if( corr==3 ) dqdx = trkdqdx[k][plane_number][j];
		    else if( corr==4 ) dqdx = trkdqdxcali[k][plane_number][j];
		    else if( corr==5 ) dqdx = trkdqdxcaliSCE[k][plane_number][j];
		    else dqdx = trkdqdx[k][plane_number][j];
		    
		    if( corr==1 ) dedx = trkdedxSCE[k][plane_number][j];
		    else if( corr==2 ) dedx = trkdedx_efieldcorr[k][plane_number][j];
		    else if( corr==3 ) dedx = trkdedx[k][plane_number][j];
		    else if( corr==4 ) dedx = trkdedxcali[k][plane_number][j];
		    else if( corr==5 ) dedx = trkdedxcaliSCE[k][plane_number][j];
		    else dedx = trkdedx[k][plane_number][j];
		    
		    if( corr==1 ) resrange = trkresrangeSCE[k][plane_number][j];
		    else if( corr==2 ) resrange = trkresrange_efieldcorr[k][plane_number][j];
		    else if( corr==3 ) resrange = trkresrange[k][plane_number][j];
		    else if( corr==4 ) resrange = trkresrangecali[k][plane_number][j];
		    else if( corr==5 ) resrange = trkresrangecaliSCE[k][plane_number][j];
		    else resrange = trkresrange[k][plane_number][j];
		    
		    float corrected_dq_dx=dqdx*yz_correction_factor;
		    float corrected_de_dx=dedx*yz_correction_factor;
		    
		    if(corrected_dq_dx!=0) dq_dx_value[x_bin].push_back(corrected_dq_dx);
		    if(corrected_dq_dx!=0) dq_dx_t_value[t_bin].push_back(corrected_dq_dx);
		    if(corrected_dq_dx!=0) dqdx_bin[x_bin]->Fill(corrected_dq_dx);
		    if(corrected_dq_dx!=0) dq_dx_theta_value[theta_bin].push_back(corrected_dq_dx);
		    if(corrected_dq_dx!=0) dq_dx_phi_value[phi_bin].push_back(corrected_dq_dx);
		    if(corrected_de_dx!=0) de_dx_value[x_bin].push_back(corrected_de_dx);
		    if(corrected_de_dx!=0) de_dx_theta_value[theta_bin].push_back(corrected_de_dx);
		    if(corrected_de_dx!=0) de_dx_phi_value[phi_bin].push_back(corrected_de_dx);
		    
		    for( size_t ang=0; ang < thetaxz_low.size(); ++ang ){
		      if(corrected_dq_dx!=0 && thetaxz_low[ang] < TMath::Abs(trackthetaxz[k]) && TMath::Abs(trackthetaxz[k]) <= thetaxz_high[ang] ) dq_dx_thetaxz_value[ang][x_bin].push_back(corrected_dq_dx);
		      if(corrected_dq_dx!=0 && thetayz_low[ang] < TMath::Abs(trackthetayz[k]) && TMath::Abs(trackthetayz[k]) <= thetayz_high[ang] ) dq_dx_thetayz_value[ang][x_bin].push_back(corrected_dq_dx);
		}
		    
		    dq_dx_value_trkhit.push_back(dqdx);
		    de_dx_value_trkhit.push_back(dedx);
		    
		  } // z coordinate
		} // y coordinate
	      } // x coordinate
	      
	    } // loop over hits
	
	    dq_dx_value_per_track.push_back(dq_dx_value_trkhit);
	    de_dx_value_per_track.push_back(de_dx_value_trkhit);
	    dq_dx_value_trkhit.clear();
	    de_dx_value_trkhit.clear();
	  } // yz angle cut
	  
	} // xz angle cut
	//	}
      } // loop over crossing  tracks
      
      //} // getting year/month/date
      
    } // loop over jentries
    
    SetHistoContent( dq_dx_value, all_dq_dx_value, dq_dx_x_hist );
    SetHistoContent( dq_dx_t_value, all_dq_dx_t_value, dq_dx_t_hist );
    SetExtraHistoContent( dq_dx_theta_value, dq_dx_theta_hist );
    SetExtraHistoContent( dq_dx_phi_value, dq_dx_phi_hist );
    SetExtraHistoContent( de_dx_value, de_dx_x_hist );
    SetExtraHistoContent( de_dx_theta_value, de_dx_theta_hist );
    SetExtraHistoContent( de_dx_phi_value, de_dx_phi_hist );
    
    SetHistoThetaCutContent( dq_dx_thetaxz_value, all_dq_dx_value_thetaxz, histos_thetaxz);
    SetHistoThetaCutContent( dq_dx_thetayz_value, all_dq_dx_value_thetayz, histos_thetayz);
    
    SetTrackMedianHistoContent(dq_dx_value_per_track,dq_dx_hist_trk);
    SetTrackMedianHistoContent(de_dx_value_per_track,de_dx_hist_trk);
    
    float global_median_dq_dx=TMath::Median(all_dq_dx_value.size(),&all_dq_dx_value[0]); 
    float global_median_dq_dx_t=TMath::Median(all_dq_dx_t_value.size(),&all_dq_dx_t_value[0]); 
    
    SetCorrHistoContent( dq_dx_value, global_median_dq_dx, dq_dx_x_error_hist, corrected_dq_dx_x_hist );
    SetCorrHistoContent( dq_dx_t_value, global_median_dq_dx_t, dq_dx_t_error_hist, corrected_dq_dx_t_hist );
    
    dq_dx_x_hist->Write();
    dq_dx_x_error_hist->Write();
    corrected_dq_dx_x_hist->Write();
    
    dq_dx_t_hist->Write();
    dq_dx_t_error_hist->Write();
    corrected_dq_dx_t_hist->Write();
    
    for( int i=0; i < 26; i++ ){
      dqdx_bin[i]->Write();
    }
    
    x_file->Write();
    x_file->Close();
    
  } // loop over plane numbers
  
  std::cout << "//////////////////////////////////// Done with generating all the x correction histograms /////////////////////////////////////" << std::endl;
}

//==============================================================================
//                 Implemetation of the helper functions
//===============================================================================

float GetMedianError( float median, std::vector<float> dqdx ){
  
  float sum_sd=0.;
  
  for( std::vector<float>::iterator it = dqdx.begin(); it != dqdx.end(); ++it ){
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


void SetHistoThetaCutContent( std::vector<std::vector<std::vector<float> > > value, std::vector<std::vector<float> >& all_value, std::vector<TH1F*> hists ){

  for(int ang=0; ang<value.size(); ang++){
    for(int i=0; i<value[ang].size(); i++){
      if(value[ang][i].size()>5){
	for(int k=0; k<value[ang][i].size(); k++){
	  all_value[ang].push_back(value[ang][i][k]);
	}
	float local_median=TMath::Median(value[ang][i].size(),&value[ang][i][0]);
	float error_median=GetMedianError(local_median, value[ang][i]);
	
	hists[ang]->SetBinContent(i+1,local_median);
	hists[ang]->SetBinError(i+1,error_median);
      }
    }
  }
  
}

void SetExtraHistoContent( std::vector<std::vector<float> > value, TH1F* hist ){
  for(int i=0; i<value.size(); i++){
    if(value[i].size()>5){
      float local_median=TMath::Median(value[i].size(),&value[i][0]);
      hist->SetBinContent(i+1,local_median);
    }
  }
  
}

void SetCorrHistoContent( std::vector<std::vector<float> > value, float global_median, TH1F* histerr, TH1F* histcorr ){
  
  std::vector<std::vector<float> > frac_error;
  frac_error.resize(26);
  
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

void SetTrackMedianHistoContent( std::vector<std::vector<float> > value, TH1F* hist ){
 for(int i=0; i<value.size(); i++){    
   if(value[i].size()<6) continue;
   float local_median=TMath::Median(value[i].size(),&value[i][0]);
   hist->Fill(local_median);
  } 
}

std::vector<int> GetGoodRun(int run, std::string& RunPeriod){
  //fetch the good run lists
  std::vector<int> good_run_vec;
  RunPeriod = GetRunPeriod(run);
  ifstream infile2;
  cout << "fetching file... " << Form("goodruns_intersec_all_runs_mcc9_%s.txt",RunPeriod.c_str()) << endl;
  infile2.open(Form("goodruns_intersec_all_runs_mcc9_%s.txt",RunPeriod.c_str()));
  int good_runs;
  infile2>>good_runs;
  while(good_runs!=-1){
    good_run_vec.push_back(good_runs);
    infile2>>good_runs;
  }
  infile2.close();
  return good_run_vec;
}

std::string GetRunPeriod(int run){

  std::string Run = "";
  if( 4951 < run && run < 7771 ) return "Run1"; 
  else if( 8405 < run && run < 11049 ) return "Run2";
  else if( 14116 < run && run < 17567 ) return "Run3";
  else{
    fprintf(stderr, "Cannot find Run Period! Exiting...\n");
    exit(-1);
  }
  return Run;

}

TString GetHistDir( int corr, TString subdir ){ 

 //create different directories for different SCE correction/data products
 if( corr < 5 ) return GetDir( Form("%s/final_calibration_root_files_datadrivenmap_%s", subdir.Data(), corrections[corr].c_str()) );
 else{
   cout << "YOU MUST PROVIDE CORRECTION!" << endl;
   exit(-1);
 }

 return "";

}

TString GetDir( const std::string& subdir )
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

void GetThetaCuts(int plane_number, float &upper_angle_xz1, float &lower_angle_xz1, float &upper_angle_yz1, float &lower_angle_yz1, float &upper_angle_xz2, float &lower_angle_xz2, float &upper_angle_yz2, float &lower_angle_yz2 ){

  upper_angle_yz2=1.e-34; upper_angle_xz2=1.e-34; 
  lower_angle_yz2=0.0; lower_angle_xz2=0.0;
  
  if( useDefault ){

    upper_angle_xz1= 10.5*(TMath::Pi()/18); lower_angle_xz1 =7.5*(TMath::Pi()/18);
    
    if(plane_number==2){
      upper_angle_yz1=10*(TMath::Pi()/18);lower_angle_yz1=8*(TMath::Pi()/18);
    }
    else if(plane_number==1){
      upper_angle_yz1=5*(TMath::Pi()/18);lower_angle_yz1=1*(TMath::Pi()/18);
    }
    else if(plane_number==0){
      upper_angle_yz1=16*(TMath::Pi()/18);lower_angle_yz1=11.5*(TMath::Pi()/18);
    }

  }else{

    if(plane_number==2){
      upper_angle_yz1=10*(TMath::Pi()/18); lower_angle_yz1=8*(TMath::Pi()/18);
      upper_angle_xz1= 11*(TMath::Pi()/18); lower_angle_xz1=7*(TMath::Pi()/18);
    }
    else if(plane_number==1){
      upper_angle_yz1=9*(TMath::Pi()/18);lower_angle_yz1=2*(TMath::Pi()/18); lower_angle_yz2 = (TMath::Pi()/18); upper_angle_yz2=2*(TMath::Pi()/18);
      upper_angle_xz1=13*(TMath::Pi()/18);lower_angle_xz1=4*(TMath::Pi()/18); upper_angle_xz2=2*(TMath::Pi()/18); lower_angle_xz2=0.;
    }
    else if(plane_number==0){
      upper_angle_yz1=17*(TMath::Pi()/18);lower_angle_yz1=12*(TMath::Pi()/18);
      upper_angle_xz1=14*(TMath::Pi()/18);lower_angle_xz1=6*(TMath::Pi()/18); upper_angle_xz2=(TMath::Pi()/18); lower_angle_xz2=0.;
    }

  }

}

void MakePretty1DHistos( TH1F *hist1D, const char* xaxis_title, const char* yaxis_title, int stat){

 hist1D->GetXaxis()->SetTitle(xaxis_title);
 hist1D->GetYaxis()->SetTitle(xaxis_title);
 if( stat != -1 )hist1D->SetStats(0);
 hist1D->Draw("HIST");

}
