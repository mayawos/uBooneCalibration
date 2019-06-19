#define Y_Z_calibration_DataDrivenMap_cxx
#include "Y_Z_calibration_DataDrivenMap.h"
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

//===================================================
//all the helper functions
//see implementation after the end of main function
//===================================================
std::vector<int> GetGoodRun(int run, std::string& RunPeriod);
std::string GetRunPeriod(int run);
TString GetDir( const std::string& subdir );
TString GetHistDir( int corr, TString subdir );
 
void MakePretty2DHistos( TH2F *hist2D, const char *xaxis_title, const char *yaxis_title, float min, float max );
void MakePretty1DHistos( TH1F *hist1D, const char *xaxis_title );
void GetThetaCuts(int plane_number, float &upper_angle_xz1, float &lower_angle_xz1, float &upper_angle_yz1, float &lower_angle_yz1, float &upper_angle_xz2, float &lower_angle_xz2, float &upper_angle_yz2, float &lower_angle_yz2 );
void SetHistoContent( std::vector< std::vector< std::vector<float> > > value, std::vector<float>& all_value, TH2F* hist );
void SetCorrHistoContent( std::vector< std::vector<std::vector<float> > > value, float global_median, TH2F* error_hist, TH2F* corrected_hist );
void SetDebugHistoContent( std::vector< std::vector< std::vector<float> > > value, TH2F* hist );
void SetTrackMedianHistoContent( std::vector<std::vector<float> > value, TH1F* hist );

//global variables
std::vector<std::string> corrections = {"nocorr","spacecorr","efieldcorr","noSCE","cali","caliSCE"};
bool useDefault = true;
bool run_is_good = true;

void Y_Z_calibration_DataDrivenMap::Loop(int corr, std::string sample, int runnumber, std::string type)
{
 
  cout << "runnumber = " << runnumber << endl; 
  gStyle->SetOptStat(1101);
  std::string RunPeriod = "MC";
  std::vector<int> good_run_vec;
  if( type == "data" ) good_run_vec = GetGoodRun(runnumber,RunPeriod);

  //create new directory if it hasn't existed already
  TString subdir = GetDir( Form("/uboone/data/users/%s/%s_%s", getenv("USER"), RunPeriod.c_str(), sample.c_str()) );
  TString histdir = GetHistDir( corr, subdir );
    
  for( int plane_number = 0; plane_number < 3; plane_number++ ){ 
        
    // ================================== START CREATING HISTOS FOR CALIBRATION =============================================
    // some of these histos are here for debugging purposes...

    TFile *file = new TFile( Form("%s/YZ_correction_factors_plane_%d.root",histdir.Data(),plane_number),"recreate");
    
    TH2F *dq_dx_z_vs_y_hist = new TH2F(Form("dq_dx_z_vs_y_hist_plane%d",plane_number),Form("plane_%d",plane_number),208,0,1040,48,-120,120);
    MakePretty2DHistos( dq_dx_z_vs_y_hist, "Z Coordinate(cm)", "Y Coordinate(cm)", 0., 300. );
      
    TH2F *dq_dx_thetaxzyz_hist = new TH2F(Form("dq_dx_thetaxzyz_hist_plane%d",plane_number),Form("plane_%d",plane_number),80,-200.,200.,80,-200.,200.);
    MakePretty2DHistos( dq_dx_thetaxzyz_hist, "#theta_{XZ} Coordinate(cm)", "#theta_{YZ} Coordinate(cm)", 10., 300. );
    
    TH2F *dq_dx_z_vs_x_hist = new TH2F(Form("dq_dx_z_vs_x_hist_plane%d",plane_number),Form("plane_%d",plane_number),208,0,1040,52,0.,260.);
    MakePretty2DHistos( dq_dx_z_vs_x_hist, "Z Coordinate(cm)", "X Coordinate(cm)", 0., 300. );
      
    TH2F *dq_dx_y_vs_x_hist = new TH2F(Form("dq_dx_y_vs_x_hist_plane%d",plane_number),Form("plane_%d",plane_number),48,-120,120,52,0,260);
    MakePretty2DHistos( dq_dx_y_vs_x_hist, "Y Coordinate(cm)", "X Coordinate(cm)", 0., 300. );
    
    TH2F *error_dq_dx_z_vs_y_hist = new TH2F(Form("error_dq_dx_z_vs_y_hist_plane%d",plane_number),Form("plane_%d",plane_number),208,0,1040,48,-120,120);
    MakePretty2DHistos( error_dq_dx_z_vs_y_hist, "Z Coordinate(cm)", "Y Coordinate(cm)", 0., 3. );
    
    TH2F *corrected_dq_dx_hist = new TH2F(Form("corrected_dq_dx_hist_plane%d",plane_number),Form("plane_%d",plane_number),208,0,1040,48,-120,120);
    MakePretty2DHistos( corrected_dq_dx_hist, "Z Coordinate(cm)", "Y Coordinate(cm)", 0., 300. );
    
    TH2F *dqdx_vs_theta = new TH2F(Form("dqdx_vs_theta_plane%d",plane_number),Form("plane_%d",plane_number), 100, 0., 500., 40, -1.0, 1.0);
    MakePretty2DHistos( dqdx_vs_theta, "#Theta", "reconstructed dQ/dx [ADC/cm]", -1, -1 );

    TH2F *dqdx_vs_phi = new TH2F(Form("dqdx_vs_phi_plane%d",plane_number),Form("plane_%d",plane_number), 100, 0., 500., 40, -1.0, 1.0);
    MakePretty2DHistos( dqdx_vs_phi, "#Phi", "reconstructed dQ/dx [ADC/cm]", -1, -1 );

    TH2F *dqdx_vs_resrange = new TH2F(Form("dqdx_vs_resrange_plane%d",plane_number),Form("plane_%d",plane_number), 80, -200.,200., 80, -200.,200.);
    MakePretty2DHistos(dqdx_vs_resrange, "Residual range", "reconstructed dQ/dx [ADC/cm]", -1., -1.);
    
    TH1F *hist_trkx = new TH1F(Form("trkhitx_dqdx_gt_500_plane%d",plane_number),Form("plane_%d",plane_number), 260, 0., 260.);
    MakePretty1DHistos(hist_trkx,"X Coordinate (cm)");
    
    TH1F *hist_thetaxz = new TH1F(Form("trkthetaxz_dqdx_plane%d",plane_number),Form("plane_%d",plane_number), 64, -3.2, 3.2);
    MakePretty1DHistos(hist_thetaxz,"X Coordinate (cm)");
    
    TH1F *hist_thetayz = new TH1F(Form("trkthetayz_dqdx_plane%d",plane_number),Form("plane_%d",plane_number), 64, -3.2, 3.2);
    MakePretty1DHistos(hist_thetayz,"X Coordinate (cm)");
    
    TH1F *hist_dqdx = new TH1F(Form("trk_dqdx_plane%d",plane_number),Form("plane_%d",plane_number), 100, 0., 500.);
    MakePretty1DHistos(hist_dqdx,"reconstructed dQ/dx (cm)");
    
    TH1F *hist_dqdx_e = new TH1F(Form("trk_dqdx_e_plane%d",plane_number),Form("plane_%d",plane_number), 1000, 0., 60000.);
    MakePretty1DHistos(hist_dqdx_e,"reconstructed dQ/dx (cm)");
    
    TH1F *dq_dx_hist_trk =  new TH1F(Form("dq_dx_hist_trk_plane%d",plane_number),Form("plane %d",plane_number),200,0.,400);
    MakePretty1DHistos(dq_dx_hist_trk,"median dQ/dx of track");
    
    TH1F *de_dx_hist_trk =  new TH1F(Form("de_dx_hist_trk_plane%d",plane_number),Form("plane %d",plane_number),100,0.,10);
    MakePretty1DHistos(de_dx_hist_trk,"median dE/dx of track");
    
    TH1F *dq_dx_cali_hist_trk =  new TH1F(Form("dq_dx_cali_hist_trk_plane%d",plane_number),Form("plane %d",plane_number),200,0.,400);
    MakePretty1DHistos(dq_dx_cali_hist_trk,"median dQ/dx of track");
    
    TH1F *de_dx_cali_hist_trk =  new TH1F(Form("de_dx_cali_hist_trk_plane%d",plane_number),Form("plane %d",plane_number),100,0.,10);
    MakePretty1DHistos(de_dx_cali_hist_trk,"median dE/dx of track");
    
    TH1F *dq_dx_caliSCE_hist_trk =  new TH1F(Form("dq_dx_caliSCE_hist_trk_plane%d",plane_number),Form("plane %d",plane_number),200,0.,400);
    MakePretty1DHistos(dq_dx_caliSCE_hist_trk,"median dQ/dx of track");
    
    TH1F *de_dx_caliSCE_hist_trk =  new TH1F(Form("de_dx_caliSCE_hist_trk_plane%d",plane_number),Form("plane %d",plane_number),100,0.,10);
    MakePretty1DHistos(de_dx_caliSCE_hist_trk,"median dE/dx of track");
    
    TH1F *low_dQdx_z =  new TH1F(Form("low_dQdx_z_plane%d",plane_number),Form("plane %d",plane_number),208,0.,1040);
    MakePretty1DHistos(low_dQdx_z,"hit z position (cm)");
    
    
    ///////////////// Make any changes to the Y and Z bin sizes here ///////////////
    
    const double y_bin_size = 5.0; // 48 bins in y direction
    const double z_bin_size = 5.0; // 208 bins in z direction
    const double x_bin_size = 5.0; // 208 bins in z direction
    const double thetaxz_bin_size = 5.0; // 80 bins in theta xz direction
    const double thetayz_bin_size = 5.0; // 80 bins in theta yz direction
    
    ///////////////////////////////////////////////////////////////////////////////
    
    vector< vector< vector<float> > > dq_dx_value;
    vector< vector< vector<float> > > dq_dx_xy_value;
    vector< vector< vector<float> > > dq_dx_xz_value;
    vector< vector< vector<float> > > dq_dx_x_value;
    vector< vector< vector<float> > > dq_dx_thetaxz_value;
    vector< vector< vector<float> > > dq_dx_thetayz_value;
    vector< vector< vector<float> > > dq_dx_thetaxzyz_value;
    
    vector<float> all_dq_dx_value;
    vector<float> dq_dx_value_trkhit;
    vector<float> de_dx_value_trkhit;
    vector<float> dq_dx_cali_value_trkhit;
    vector<float> de_dx_cali_value_trkhit;
    vector<float> dq_dx_caliSCE_value_trkhit;
    vector<float> de_dx_caliSCE_value_trkhit;
    vector< vector<float> > dq_dx_value_per_track;
    vector< vector<float> > de_dx_value_per_track;
    vector< vector<float> > dq_dx_cali_value_per_track;
    vector< vector<float> > de_dx_cali_value_per_track;
    vector< vector<float> > dq_dx_caliSCE_value_per_track;
    vector< vector<float> > de_dx_caliSCE_value_per_track;
    vector< vector< vector<float> > > dq_dx_frac_error;
    
    dq_dx_value.resize(208);
    dq_dx_xz_value.resize(208);
    dq_dx_xy_value.resize(48);
    dq_dx_x_value.resize(208);
    dq_dx_thetaxz_value.resize(208);
    dq_dx_thetayz_value.resize(208);
    dq_dx_frac_error.resize(208);
    dq_dx_thetaxzyz_value.resize(80);
    for(int i = 0; i < dq_dx_value.size(); i++){
      dq_dx_value[i].resize(48);
      dq_dx_x_value[i].resize(48);
      dq_dx_xz_value[i].resize(52);
      dq_dx_thetaxz_value[i].resize(48);
      dq_dx_thetayz_value[i].resize(48);
      dq_dx_frac_error[i].resize(48);
    }
    
    for(int i = 0; i < dq_dx_thetaxzyz_value.size(); i++){
      dq_dx_thetaxzyz_value[i].resize(80);
    }
    
    for(int i = 0; i < dq_dx_xy_value.size(); i++){
      dq_dx_xy_value[i].resize(52);
    }
    
    // these variables are used in the counter
    int n_cross=0;
    int passthetacut=0;
    int trk_has_calo = 0; 
    int calo_is_valid = 0; 
    int total_dqdx = 0;
    int high_dqdx = 0;
    int low_dqdx = 0;
    int quite_low_dqdx = 0;
    
    if (fChain == 0) return;
    Long64_t nentries = fChain->GetEntriesFast();
    Long64_t nbytes = 0, nb = 0;
    for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
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
      
      for(int i=0; i<cross_trks; ++i){
	  if( xprojectedlen[i] < 250. ) continue; 
	  n_cross++;
	  
	  if( hasCalo[i][plane_number] != 0 ) trk_has_calo++;
	  if( caloIsValid[i][plane_number] != 0 ) calo_is_valid++;
	  
	   // these are hard-coded based on the MCC9.0 absolute gain calibration
	   // used for debugging overlay
	   // ------------------------------------------------------------------
	   float ADC_scale; 
	   if(plane_number==0) ADC_scale=0.00408; 
	   else if(plane_number==1) ADC_scale=0.00397; 
	   else if(plane_number==2) ADC_scale=0.00408; 
	   // ------------------------------------------------------------------
	  
	  float upper_angle_xz1, upper_angle_xz2, upper_angle_yz1, upper_angle_yz2;
	  float lower_angle_xz1, lower_angle_xz2, lower_angle_yz1, lower_angle_yz2;

          //theta cuts by default is the same as MCC8 and MCC9.0
          //additional cuts are added for MCC9.1 due to tracks covering wider thetaxz vs thetayz phase space in plane 0 and plane 2 to be badly reconstructed
          GetThetaCuts(plane_number, upper_angle_xz1, lower_angle_xz1, upper_angle_yz1, lower_angle_yz1, upper_angle_xz2, lower_angle_xz2, upper_angle_yz2, lower_angle_yz2);	  

	  if(!((lower_angle_xz1<TMath::Abs(trackthetaxz[i])) && (TMath::Abs(trackthetaxz[i])<=upper_angle_xz1)) && !((lower_angle_xz2<TMath::Abs(trackthetaxz[i])) && (TMath::Abs(trackthetaxz[i]) <= upper_angle_xz2 ))){
	     if(!(((lower_angle_yz1<TMath::Abs(trackthetayz[i]))) && (TMath::Abs(trackthetayz[i])<=upper_angle_yz1)) && !((lower_angle_yz2<TMath::Abs(trackthetayz[i])) && (TMath::Abs(trackthetayz[i])<=upper_angle_yz2))){
	       passthetacut++;
	       for(int j=0; j<TMath::Min(ntrkhits[i][plane_number],3000); ++j){

		 float hitx=-9999.;
		 float hity=-9999.;
                 float hitz=-9999.;

		 if( corr==0 || corr==3 || corr==4 ){
		   hitx = trkhitx[i][plane_number][j];
		   hity = trkhity[i][plane_number][j];
		   hitz = trkhitz[i][plane_number][j];
		 }
		 else if( corr==1 || corr==2 || corr==5 ){
		   hitx = trkhitxSCE[i][plane_number][j];
		   hity = trkhitySCE[i][plane_number][j];
		   hitz = trkhitzSCE[i][plane_number][j];
		 }

		 if((hitx<260)&&(hitx>0)){
		   if((hity<120)&&(hity>-120)){
		     if((hitz<1040)&&(hitz>0)){
		       total_dqdx++;
		       
		       int z_bin = int(hitz)/z_bin_size; 
		       int y_bin;
                       int x_bin = int(hitx)/x_bin_size;
                       int thetaxz_bin, thetayz_bin;

		       if(hity<0) y_bin=int(hity)/y_bin_size+23;
		       if(hity>=0) y_bin=int(hity)/y_bin_size+24;
		       if(trackthetaxz[i]<0) thetaxz_bin = int(TMath::RadToDeg()*trackthetaxz[i])/thetaxz_bin_size+39;
		       if(trackthetaxz[i]>=0) thetaxz_bin = int(TMath::RadToDeg()*trackthetaxz[i])/thetaxz_bin_size+40;
		       if(trackthetayz[i]<0) thetayz_bin = int(TMath::RadToDeg()*trackthetayz[i])/thetayz_bin_size+39;
		       if(trackthetayz[i]>=0) thetayz_bin = int(TMath::RadToDeg()*trackthetayz[i])/thetayz_bin_size+40;
		       
		       float dqdx;
		       float dedx;
                       float resrange;;
		       
		       if( corr==1 ) dqdx = trkdqdxSCE[i][plane_number][j];
		       else if( corr==2 ) dqdx = trkdqdx_efieldcorr[i][plane_number][j];
		       else if( corr==3 ) dqdx = trkdqdx[i][plane_number][j];
		       else if( corr==4 ) dqdx = trkdqdxcali[i][plane_number][j];
		       else if( corr==5 ) dqdx = trkdqdxcaliSCE[i][plane_number][j];
		       else dqdx = trkdqdx[i][plane_number][j];
		       
		       if( corr==1 ) dedx = trkdedxSCE[i][plane_number][j];
		       else if( corr==2 ) dedx = trkdedx_efieldcorr[i][plane_number][j];
		       else if( corr==3 ) dedx = trkdedx[i][plane_number][j];
		       else if( corr==4 ) dedx = trkdedxcali[i][plane_number][j];
		       else if( corr==5 ) dedx = trkdedxcaliSCE[i][plane_number][j];
		       else dedx = trkdedx[i][plane_number][j];
		       
		       if( corr==1 ) resrange = trkresrangeSCE[i][plane_number][j];
		       else if( corr==2 ) resrange = trkresrange_efieldcorr[i][plane_number][j];
		       else if( corr==3 ) resrange = trkresrange[i][plane_number][j];
		       else if( corr==4 ) resrange = trkresrangecali[i][plane_number][j];
		       else if( corr==5 ) resrange = trkresrangecaliSCE[i][plane_number][j];
		       else resrange = trkresrange[i][plane_number][j];
		       
		       dqdx_vs_theta->Fill(dqdx,TMath::Cos(trackthetayz[i]));
		       dqdx_vs_phi->Fill(dqdx,trackthetaxy[i]);
		       dqdx_vs_resrange->Fill(resrange,dqdx);
		       
		       if( dqdx > 500. ) high_dqdx++;
		       if( dqdx < 0. ) low_dqdx++;
		       
                       //fill the vector used to calculate the median dq/dx per bin/voxel
		       dq_dx_value[z_bin][y_bin].push_back(dqdx);
		       dq_dx_thetaxzyz_value[thetaxz_bin][thetayz_bin].push_back(dqdx);
		       dq_dx_xz_value[z_bin][x_bin].push_back(dqdx);
		       dq_dx_xy_value[y_bin][x_bin].push_back(dqdx);
		       dq_dx_x_value[z_bin][y_bin].push_back(hitx);
		       dq_dx_thetaxz_value[z_bin][y_bin].push_back(trackthetaxz[i]);
		       dq_dx_thetayz_value[z_bin][y_bin].push_back(trackthetayz[i]);
		       hist_dqdx->Fill(dqdx);
		       hist_dqdx_e->Fill(dqdx/ADC_scale);
		       
		       dq_dx_value_trkhit.push_back(dqdx);
		       de_dx_value_trkhit.push_back(dedx);
		       dq_dx_cali_value_trkhit.push_back(trkdqdxcali[i][plane_number][j]);
		       de_dx_cali_value_trkhit.push_back(trkdedxcali[i][plane_number][j]);
		       dq_dx_caliSCE_value_trkhit.push_back(trkdqdxcaliSCE[i][plane_number][j]);
		       de_dx_caliSCE_value_trkhit.push_back(trkdedxcaliSCE[i][plane_number][j]);
		       
		     } // Z containment
		   } // Y containment
		 } // X containmen
		 
	       } // loop over hits of the track in the given plane
               
               //fill the vectors to plot the track median based on different SCE corr.
               dq_dx_value_per_track.push_back(dq_dx_value_trkhit);
               de_dx_value_per_track.push_back(de_dx_value_trkhit);
               dq_dx_cali_value_per_track.push_back(dq_dx_cali_value_trkhit);
               de_dx_cali_value_per_track.push_back(de_dx_cali_value_trkhit);
               dq_dx_caliSCE_value_per_track.push_back(dq_dx_caliSCE_value_trkhit);
               de_dx_caliSCE_value_per_track.push_back(de_dx_caliSCE_value_trkhit);
               dq_dx_value_trkhit.clear();
               de_dx_value_trkhit.clear();
               dq_dx_cali_value_trkhit.clear();
               de_dx_cali_value_trkhit.clear();
               dq_dx_caliSCE_value_trkhit.clear();
               de_dx_caliSCE_value_trkhit.clear();
	       
	     } // theta yz angle cut
	  } // theta xz angle cut
	  
      } // loop over crossing tracks in the event
      
    } // loop over jentries
    
    
    //YZ map calibration
    SetHistoContent( dq_dx_value, all_dq_dx_value, dq_dx_z_vs_y_hist );
    float global_median_dq_dx=TMath::Median(all_dq_dx_value.size(),&all_dq_dx_value[0]);
    SetCorrHistoContent( dq_dx_value, global_median_dq_dx, error_dq_dx_z_vs_y_hist, corrected_dq_dx_hist );
    
    //debugging and validation histos
    SetDebugHistoContent( dq_dx_xz_value, dq_dx_z_vs_x_hist );
    SetDebugHistoContent( dq_dx_xy_value, dq_dx_y_vs_x_hist );
    SetDebugHistoContent( dq_dx_thetaxzyz_value, dq_dx_thetaxzyz_hist );
    
    SetTrackMedianHistoContent(dq_dx_value_per_track,dq_dx_hist_trk);
    SetTrackMedianHistoContent(de_dx_value_per_track,de_dx_hist_trk);
    SetTrackMedianHistoContent(dq_dx_cali_value_per_track,dq_dx_cali_hist_trk);
    SetTrackMedianHistoContent(de_dx_cali_value_per_track,de_dx_cali_hist_trk);
    SetTrackMedianHistoContent(dq_dx_caliSCE_value_per_track,dq_dx_caliSCE_hist_trk);
    SetTrackMedianHistoContent(de_dx_caliSCE_value_per_track,de_dx_caliSCE_hist_trk);
 
    // !need to save the counter into a text file!
    std::cout << "Number of crossing tracks : " << n_cross << std::endl;
    std::cout << "trk has calo, plane " << plane_number << " = " << trk_has_calo <<  std::endl;
    std::cout << "calo is valid, plane " << plane_number << " = " << calo_is_valid <<  std::endl;
    std::cout << "total dqdx, plane " << plane_number << " = " << total_dqdx <<  std::endl;
    std::cout << "low dqdx, plane " << plane_number << " = " << low_dqdx <<  std::endl;
    std::cout << "quite low dqdx, plane " << plane_number << " = " << quite_low_dqdx <<  std::endl;
    std::cout << "high dqdx, plane " << plane_number << " = " << high_dqdx <<  std::endl;
    std::cout << "pass theta cut = " << plane_number << " = " << passthetacut << std::endl;
    
    file->Write();
    file->Close();
    
    std::cout << "*************** ext_data_test.C macro has ended ******************" << std::endl;  
    
  }//loop over plane number
  
}

//==============================================================================
//                 Implemetation of the helper functions
//===============================================================================
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

void MakePretty2DHistos( TH2F *hist2D, const char* xaxis_title, const char* yaxis_title, float min, float max){

 hist2D->GetXaxis()->SetTitle(xaxis_title);
 hist2D->GetYaxis()->SetTitle(yaxis_title);
 hist2D->SetStats(0);
 if( min != -1 ) hist2D->SetMinimum(min);
 if( max != -1 ) hist2D->SetMaximum(max);
 hist2D->Draw("COLZ");

}

void MakePretty1DHistos( TH1F *hist1D, const char* xaxis_title){

 hist1D->GetXaxis()->SetTitle(xaxis_title);
 hist1D->GetYaxis()->SetTitle("Entries");
 hist1D->SetStats(0);
 hist1D->Draw("HIST");

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

void SetHistoContent( std::vector< std::vector< std::vector<float> > > value, std::vector<float>& all_value, TH2F* hist ){
 for(int i=0; i<value.size(); i++){
   for(int j=0; j<value[i].size(); j++){
     if(value[i][j].size()>5){
       
       for(int k=0; k<value[i][j].size(); k++){
	 all_value.push_back(value[i][j][k]); 
       }
       
       float local_median=TMath::Median(value[i][j].size(),&value[i][j][0]);
       hist->SetBinContent(i+1,j+1,local_median);
     }
   }
 }
}

void SetCorrHistoContent( std::vector<std::vector<std::vector<float> > >value, float global_median, TH2F* error_hist, TH2F* corrected_hist ){

 std::vector<std::vector<float> > frac_error;
 frac_error.resize(value.size());

 for(int i=0; i<value.size(); i++){
   frac_error[i].resize(value[i].size());
   for(int j=0; j<value[i].size(); j++){
     if(value[i][j].size()>5){
       float local_median=TMath::Median(value[i][j].size(),&value[i][j][0]);
       float fractional=float(global_median)/local_median; 
       error_hist->SetBinContent(i+1,j+1,fractional);
       frac_error[i][j]=fractional;
     }
   }
 }
 
 for(int i=0; i<value.size(); i++){
   for(int j=0; j<value[i].size(); j++){
     if(value[i][j].size()>5){
       float local_median=TMath::Median(value[i][j].size(),&value[i][j][0]);
       float corrected=local_median*frac_error[i][j];
       corrected_hist->SetBinContent(i+1,j+1,corrected);
     } 
   }
 }

}

void SetDebugHistoContent( std::vector< std::vector< std::vector<float> > > value, TH2F* hist ){
 for(int i=0; i<value.size(); i++){
   for(int j=0; j<value[i].size(); j++){
     if(value[i][j].size()>5){
       
       float local_median=TMath::Median(value[i][j].size(),&value[i][j][0]);
       hist->SetBinContent(i+1,j+1,local_median);
     }
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
