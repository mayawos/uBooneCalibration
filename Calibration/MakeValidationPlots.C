#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TAttFill.h>
#include <TAxis.h>
#include <TColor.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLegend.h>
#include <TFile.h>
#include <TString.h>
#include <fstream>
#include <vector>
#include <cmath>
#include <math.h> 
#include <string>
#include <TImage.h>
#include <iomanip>

void Draw_Overlay_With_Legend(std::vector< std::vector<TH1F*> > histos, int plane, TString canname, TString title, TPad *pad = 0);
double NormScale(std::vector< std::vector<TH1F*> > histos, int mode, int plane);
void PaintOverflow(TH1 *h, int histo_n);

void MakeValidationPlots(){

  //initialize histograms
  //first store histo per plane and then histo per deconvolution
  std::vector< std::vector<TH2F*> > ratio_reco_det_plane; 
  std::vector< std::vector<TH2F*> > ratio_corr_det_plane;
  std::vector< std::vector<TH1F*> > ratio_reco_det_plane_1d;
  std::vector< std::vector<TH1F*> > ratio_corr_det_plane_1d;
  std::vector<TH2F*> ratio_reco_plane; 
  std::vector<TH2F*> ratio_corr_plane;
  std::vector<TH1F*> ratio_reco_plane_1d;
  std::vector<TH1F*> ratio_corr_plane_1d;
  std::vector<TH1F*> dq_dx_hist_trk_vector;
  std::vector<TH1F*> de_dx_hist_trk_vector;
  std::vector<TH1F*> dq_dx_x_vector, dq_dx_phi_vector, dq_dx_theta_vector;
  std::vector<TH1F*> de_dx_x_vector, de_dx_phi_vector, de_dx_theta_vector;
  std::vector<TH1F*> ratio_dqde_x_vector, ratio_dqde_phi_vector, ratio_dqde_theta_vector;
  std::vector< std::vector<TH1F*> > dq_dx_hist_trk_plane;
  std::vector< std::vector<TH1F*> > de_dx_hist_trk_plane;
  std::vector< std::vector<TH1F*> > dq_dx_x_plane, dq_dx_phi_plane, dq_dx_theta_plane;
  std::vector< std::vector<TH1F*> > de_dx_x_plane, de_dx_phi_plane, de_dx_theta_plane;
  std::vector< std::vector<TH1F*> > ratio_dqde_x_plane, ratio_dqde_phi_plane, ratio_dqde_theta_plane;

  std::vector< std::string > tags = { "_nocorr", "_spacecorr", "_efieldcorr" };
  //std::vector< std::string > tags = { "_nocorr","_spacecorr","_efieldcorr","_cali","_caliSCE" };
  
  //loop over the 2 methods of deconvolutional
  for( int m=0; m<1; m++ ){
    //if( m==1 ) continue;
    for( int t=0; t< tags.size(); t++ ){
      for( int plane=0;plane < 3; plane++ ){ 
	//loop over the 3 planes
	
	TString filename = Form("/uboone/data/users/wospakrk/DataMap_OffBeamVal/final_calibration_root_files_datadrivenmap%s_month%d/YZ_correction_factors_2016_month_%d_plane_%d.root",tags[t].c_str(),m,m,plane);
	TFile *_file0 = TFile::Open(filename);
	
	TH2F* dq_dx_z_vs_y_hist = (TH2F*)_file0->Get(Form("dq_dx_z_vs_y_hist_plane%d",plane));
	TH2F* corrected_dq_dx_hist = (TH2F*)_file0->Get(Form("corrected_dq_dx_hist_plane%d",plane));
	TH2F* error_dq_dx_hist = (TH2F*)_file0->Get(Form("error_dq_dx_z_vs_y_hist_plane%d",plane));
	
	TH2F* dqdx_vs_thetaxz_hist = (TH2F*)_file0->Get(Form("dqdx_vs_thetaxz_plane%d",plane));
	TH2F* dqdx_vs_thetayz_hist = (TH2F*)_file0->Get(Form("dqdx_vs_thetayz_plane%d",plane));
	TH2F* dqdx_vs_thetaxy_hist = (TH2F*)_file0->Get(Form("dqdx_vs_thetaxy_plane%d",plane));
	TH2F* thetaxz_vs_thetayz_hist_num = (TH2F*)_file0->Get(Form("thetaxz_vs_thetayz_num_plane%d",plane));
	TH2F* thetaxz_vs_thetayz_hist_denom = (TH2F*)_file0->Get(Form("thetaxz_vs_thetayz_denom_plane%d",plane));
	
	TH2F* after_dqdx_vs_thetaxz_hist = (TH2F*)_file0->Get(Form("after_dqdx_vs_thetaxz_plane%d",plane));
	TH2F* after_dqdx_vs_thetayz_hist = (TH2F*)_file0->Get(Form("after_dqdx_vs_thetayz_plane%d",plane));
	TH2F* after_dqdx_vs_thetaxy_hist = (TH2F*)_file0->Get(Form("after_dqdx_vs_thetaxy_plane%d",plane));
	TH2F* after_thetaxz_vs_thetayz_hist_num = (TH2F*)_file0->Get(Form("after_thetaxz_vs_thetayz_num_plane%d",plane));
	TH2F* after_thetaxz_vs_thetayz_hist_denom = (TH2F*)_file0->Get(Form("after_thetaxz_vs_thetayz_denom_plane%d",plane));
	
	TH1F* fracres_dqdx_spatialcorr = (TH1F*)_file0->Get(Form("FracRes_dQdx_spatialcorr_plane%d",plane));
	TH1F* fracres_dqdx_efieldcorr = (TH1F*)_file0->Get(Form("FracRes_dQdx_efieldcorr_plane%d",plane));
	TH1F* fracres_dqdx_efield_spatial_corr = (TH1F*)_file0->Get(Form("FracRes_dQdx_efield_spatial_corr_plane%d",plane));
	
	TH1F* dq_dx_hist_trk = (TH1F*)_file0->Get(Form("dq_dx_hist_trk_plane%d",plane));
	dq_dx_hist_trk->GetXaxis()->SetTitle("Median dQ/dx of Crossing Muon Tracks");
	TH1F* de_dx_hist_trk = (TH1F*)_file0->Get(Form("de_dx_hist_trk_plane%d",plane));
	de_dx_hist_trk->GetXaxis()->SetTitle("Median dE/dx of Crossing Muon Tracks");
	de_dx_hist_trk->GetXaxis()->SetRangeUser(0.,5.);

	dq_dx_hist_trk_vector.push_back(dq_dx_hist_trk);
	de_dx_hist_trk_vector.push_back(de_dx_hist_trk);
	//store the histograms to create the ratio histograms
	ratio_reco_plane.push_back(dq_dx_z_vs_y_hist);
	ratio_corr_plane.push_back(corrected_dq_dx_hist);
      
	//new 1D histogram to hold the dQ/dx value
	TString title = Form("plane %d", plane);
	TH1F *hist_dqdx_reco = new TH1F("dqdx_reco",title,60,60.,340.);
	TH1F *hist_dqdx_corr = new TH1F("dqdx_corr",title,60,60.,340.);
	
	//loop over bin in x
	std::cout << "dq_dx_z_vs_y_hist, tags, plane = " << dq_dx_z_vs_y_hist << ", " << tags[t] << ", " << plane << std::endl;
	for( int binx = 0; binx < dq_dx_z_vs_y_hist->GetNbinsX(); binx++ ){ 
	  //loop over bin in y
	  for( int biny = 0; biny < dq_dx_z_vs_y_hist->GetNbinsY(); biny++ ){
	    double dQdx_reco = dq_dx_z_vs_y_hist->GetBinContent(binx,biny);
	    //std::cout << "dQdx reco = " << dQdx_reco << std::endl; 
	    double dQdx_corr = corrected_dq_dx_hist->GetBinContent(binx,biny);
	    if( dQdx_reco == 0. ) continue;
	    hist_dqdx_reco->Fill(dQdx_reco);
	    hist_dqdx_corr->Fill(dQdx_corr);
	  }
	}
	
	//insert the 1d histograms here:
	ratio_reco_plane_1d.push_back(hist_dqdx_reco);
	ratio_corr_plane_1d.push_back(hist_dqdx_corr);
	
	TString filenamex = Form("/uboone/data/users/wospakrk/DataMap_OffBeamVal/final_calibration_root_files_datadrivenmap%s_month%d/X_correction_factors_2016_month_%d_%d_plane_%d.root",tags[t].c_str(),m,m,m,plane);
	TFile *_filex = TFile::Open(filenamex);
        std::cout << "_filex = " << _filex->GetName() << std::endl;	
	TH1F* dq_dx_x_hist = (TH1F*)_filex->Get(Form("dq_dx_x_hist_plane%d",plane));
	TH1F* dq_dx_phi_hist = (TH1F*)_filex->Get(Form("dq_dx_phi_hist_plane%d",plane));
	TH1F* dq_dx_theta_hist = (TH1F*)_filex->Get(Form("dq_dx_theta_hist_plane%d",plane));
	TH1F* de_dx_x_hist = (TH1F*)_filex->Get(Form("de_dx_x_hist_plane%d",plane));
	TH1F* de_dx_phi_hist = (TH1F*)_filex->Get(Form("de_dx_phi_hist_plane%d",plane));
	TH1F* de_dx_theta_hist = (TH1F*)_filex->Get(Form("de_dx_theta_hist_plane%d",plane));
	TH1F* dqde_dx_x_hist = (TH1F*)dq_dx_x_hist->Clone(Form("dqde_dx_x_hist_plane%d",plane));
	TH1F* dqde_dx_theta_hist = (TH1F*)dq_dx_x_hist->Clone(Form("dqde_dx_theta_hist_plane%d",plane));
	TH1F* dqde_dx_phi_hist = (TH1F*)dq_dx_x_hist->Clone(Form("dqde_dx_phi_hist_plane%d",plane));
	
	//divide them
	dqde_dx_x_hist->Divide(de_dx_x_hist);
	dqde_dx_phi_hist->Divide(de_dx_phi_hist);
	dqde_dx_theta_hist->Divide(de_dx_theta_hist);
	
	dq_dx_x_vector.push_back(dq_dx_x_hist);
	dq_dx_phi_vector.push_back(dq_dx_phi_hist);
	dq_dx_theta_vector.push_back(dq_dx_theta_hist);
	
	de_dx_x_vector.push_back(de_dx_x_hist);
	de_dx_phi_vector.push_back(de_dx_phi_hist);
	de_dx_theta_vector.push_back(de_dx_theta_hist);
	
	ratio_dqde_x_vector.push_back(dqde_dx_x_hist);
	ratio_dqde_phi_vector.push_back(dqde_dx_phi_hist);
	ratio_dqde_theta_vector.push_back(dqde_dx_theta_hist);
	
	//Print the histograms per plane per deconvolution
	
	//YZ map before correction
	TString cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/dq_dx_z_vs_y_hist_plane_%d%s",m,plane,tags[t].c_str());
	TString titlehist = Form("plane %d, %s ", plane, tags[t].c_str());
	TCanvas *c1 = new TCanvas(cName,cName);
	dq_dx_z_vs_y_hist->SetMaximum(300.);
	dq_dx_z_vs_y_hist->SetTitle(titlehist.Data());
	dq_dx_z_vs_y_hist->Draw("colz");
	c1->Print(Form("%s.png",cName.Data()),"png");
	
	//YZ map after correction
	cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/corrected_dq_dx_z_vs_y_hist_plane_%d%s",m,plane,tags[t].c_str());
	titlehist = Form("plane %d, %s", plane, tags[t].c_str());
	TCanvas *c20 = new TCanvas(cName,cName);
	corrected_dq_dx_hist->SetMaximum(300.);
	corrected_dq_dx_hist->SetTitle(titlehist.Data());
	corrected_dq_dx_hist->Draw("colz");
	c20->SetLeftMargin(0.2);
	c20->Print(Form("%s.png",cName.Data()),"png");
	
	//YZ map after correction
	cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/error_dq_dx_z_vs_y_hist_plane_%d%s",m,plane,tags[t].c_str());
	titlehist = Form("plane %d, %s", plane, tags[t].c_str());
	TCanvas *c2 = new TCanvas(cName,cName);
	error_dq_dx_hist->SetMaximum(1.);
	error_dq_dx_hist->SetTitle(titlehist.Data());
	error_dq_dx_hist->Draw("colz");
	c2->SetLeftMargin(0.2);
	c2->Print(Form("%s.png",cName.Data()),"png");
	
	//angular dependence plots
	{
	  cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/dqdx_vs_thetaxz_hist_plane_%d%s",m,plane,tags[t].c_str());
	  titlehist = Form("plane %d, %s", plane, tags[t].c_str());
	  TCanvas *can = new TCanvas(cName,cName);
	  dqdx_vs_thetaxz_hist->SetMaximum(1.);
	  dqdx_vs_thetaxz_hist->SetTitle(titlehist.Data());
	  dqdx_vs_thetaxz_hist->Draw("colz");
	  can->SetLeftMargin(0.2);
	  can->Print(Form("%s.png",cName.Data()),"png");
	}
	//angular dependence plots
	{
	  cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/dqdx_vs_thetayz_hist_plane_%d%s",m,plane,tags[t].c_str());
	  titlehist = Form("plane %d, %s", plane, tags[t].c_str());
	  TCanvas *can = new TCanvas(cName,cName);
	  dqdx_vs_thetayz_hist->SetMaximum(1.);
	  dqdx_vs_thetayz_hist->SetTitle(titlehist.Data());
	  dqdx_vs_thetayz_hist->Draw("colz");
	  can->SetLeftMargin(0.2);
	  can->Print(Form("%s.png",cName.Data()),"png");
	}
	//angular dependence plots
	{
	  cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/dqdx_vs_thetaxy_hist_plane_%d%s",m,plane,tags[t].c_str());
	  titlehist = Form("plane %d, %s", plane, tags[t].c_str());
	  TCanvas *can = new TCanvas(cName,cName);
	  dqdx_vs_thetaxy_hist->SetMaximum(1.);
	  dqdx_vs_thetaxy_hist->SetTitle(titlehist.Data());
	  dqdx_vs_thetaxy_hist->Draw("colz");
	  can->SetLeftMargin(0.2);
	  can->Print(Form("%s.png",cName.Data()),"png");
	} 
	//angular dependence plots
	{
	  cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/thetaxz_vs_thetayz_hist_plane_%d%s",m,plane,tags[t].c_str());
	  titlehist = Form("plane %d, %s", plane, tags[t].c_str());
	  TCanvas *can = new TCanvas(cName,cName);
	  thetaxz_vs_thetayz_hist_num->SetMaximum(300.);
	  thetaxz_vs_thetayz_hist_num->SetTitle(titlehist.Data());
	  thetaxz_vs_thetayz_hist_num->Divide(thetaxz_vs_thetayz_hist_denom);
	  thetaxz_vs_thetayz_hist_num->GetZaxis()->SetTitle("Average dQ/dx [ADC/cm] ");
	  thetaxz_vs_thetayz_hist_num->Draw("colz");
	  can->SetLeftMargin(0.2);
	  can->Print(Form("%s.png",cName.Data()),"png");
	}
	//angular dependence plots
	{
	  cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/after_dqdx_vs_thetaxz_hist_plane_%d%s",m,plane,tags[t].c_str());
	  titlehist = Form("plane %d, %s", plane, tags[t].c_str());
	  TCanvas *can = new TCanvas(cName,cName);
	  after_dqdx_vs_thetaxz_hist->SetMaximum(1.);
	  after_dqdx_vs_thetaxz_hist->SetTitle(titlehist.Data());
	  after_dqdx_vs_thetaxz_hist->Draw("colz");
	  can->SetLeftMargin(0.2);
	  can->Print(Form("%s.png",cName.Data()),"png");
	}
	//angular dependence plots
	{
	  cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/after_dqdx_vs_thetayz_hist_plane_%d%s",m,plane,tags[t].c_str());
	  titlehist = Form("plane %d, %s", plane, tags[t].c_str());
	  TCanvas *can = new TCanvas(cName,cName);
	  after_dqdx_vs_thetayz_hist->SetMaximum(1.);
	  after_dqdx_vs_thetayz_hist->SetTitle(titlehist.Data());
	  after_dqdx_vs_thetayz_hist->Draw("colz");
	  can->SetLeftMargin(0.2);
	  can->Print(Form("%s.png",cName.Data()),"png");
	}
	//angular dependence plots
	{
	  cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/after_dqdx_vs_thetaxy_hist_plane_%d%s",m,plane,tags[t].c_str());
	  titlehist = Form("plane %d, %s", plane, tags[t].c_str());
	  TCanvas *can = new TCanvas(cName,cName);
	  after_dqdx_vs_thetaxy_hist->SetMaximum(1.);
	  after_dqdx_vs_thetaxy_hist->SetTitle(titlehist.Data());
	  after_dqdx_vs_thetaxy_hist->Draw("colz");
	  can->SetLeftMargin(0.2);
	  can->Print(Form("%s.png",cName.Data()),"png");
	} 
	//angular dependence plots
	{
	  cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/after_thetaxz_vs_thetayz_hist_plane_%d%s",m,plane,tags[t].c_str());
	  titlehist = Form("plane %d, %s", plane, tags[t].c_str());
	  TCanvas *can = new TCanvas(cName,cName);
	  after_thetaxz_vs_thetayz_hist_num->SetMaximum(300.);
	  after_thetaxz_vs_thetayz_hist_num->SetTitle(titlehist.Data());
	  after_thetaxz_vs_thetayz_hist_num->Divide(after_thetaxz_vs_thetayz_hist_denom);
	  after_thetaxz_vs_thetayz_hist_num->GetZaxis()->SetTitle("Average dQ/dx [ADC/cm] ");
	  after_thetaxz_vs_thetayz_hist_num->Draw("colz");
	  can->SetLeftMargin(0.2);
	  can->Print(Form("%s.png",cName.Data()),"png");
	}
	
	//size of correction
	{
	  cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/FracRes_dQdx_spatialcorr_plane_%d%s",m,plane,tags[t].c_str());
	  titlehist = Form("plane %d", plane);
	  TCanvas *can = new TCanvas(cName,cName);
	  fracres_dqdx_spatialcorr->SetLineWidth(3);
	  fracres_dqdx_spatialcorr->SetLineColor(4);
	  fracres_dqdx_spatialcorr->GetYaxis()->SetDecimals();
	  fracres_dqdx_spatialcorr->GetXaxis()->SetRangeUser(-0.15,0.15);
	  fracres_dqdx_spatialcorr->SetTitle(titlehist.Data());
	  fracres_dqdx_spatialcorr->Draw("vector");
	  can->Print(Form("%s.png",cName.Data()),"png");
	}
	//size of correction
	{
	  cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/FracRes_dQdx_efieldcorr_plane_%d%s",m,plane,tags[t].c_str());
	  titlehist = Form("plane %d", plane);
	  TCanvas *can = new TCanvas(cName,cName);
	  fracres_dqdx_efieldcorr->SetLineWidth(3);
	  fracres_dqdx_efieldcorr->SetLineColor(4);
	  fracres_dqdx_efieldcorr->GetYaxis()->SetDecimals();
	  fracres_dqdx_efieldcorr->GetXaxis()->SetRangeUser(-0.15,0.15);
	  fracres_dqdx_efieldcorr->SetTitle(titlehist.Data());
	  fracres_dqdx_efieldcorr->Draw("vector");
	  can->Print(Form("%s.png",cName.Data()),"png");
	}
	//size of correction
	{
	  cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/FracRes_dQdx_efield_spatial_corr_plane_%d%s",m,plane,tags[t].c_str());
	  titlehist = Form("plane %d", plane);
	  TCanvas *can = new TCanvas(cName,cName);
	  fracres_dqdx_efield_spatial_corr->SetLineWidth(3);
	  fracres_dqdx_efield_spatial_corr->SetLineColor(4);
	  fracres_dqdx_efield_spatial_corr->GetYaxis()->SetDecimals();
	  fracres_dqdx_efield_spatial_corr->GetXaxis()->SetRangeUser(-0.015,0.015);
	  fracres_dqdx_efield_spatial_corr->SetTitle(titlehist.Data());
	  fracres_dqdx_efield_spatial_corr->Draw("vector");
	  can->Print(Form("%s.png",cName.Data()),"png");
	}
	
	//1d dqdx before correction
	cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/1d_dqdx_yzmap_reco_plane_%d%s",m,plane,tags[t].c_str());
	titlehist = Form("plane %d, %s; dQ/dx (ADC/cm); Entries", plane, tags[t].c_str());
	TCanvas *c3 = new TCanvas(cName,cName);
	hist_dqdx_reco->SetLineWidth(3);
	hist_dqdx_reco->SetLineColor(4);
	hist_dqdx_reco->GetYaxis()->SetDecimals();
	hist_dqdx_reco->GetXaxis()->SetRangeUser(0.,400.);
	hist_dqdx_reco->SetTitle(titlehist.Data());
	hist_dqdx_reco->Draw("vector");
	c3->Print(Form("%s.png",cName.Data()),"png");
	
	//1d dqdx after correction
	cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/1d_dqdx_yzmap_corr_plane_%d%s",m,plane,tags[t].c_str());
	titlehist = Form("plane %d, %s; dQ/dx (ADC/cm); Entries", plane, tags[t].c_str());
	TCanvas *c4 = new TCanvas(cName,cName);
	hist_dqdx_corr->SetLineWidth(3);
	hist_dqdx_corr->SetLineColor(4);
	hist_dqdx_corr->GetYaxis()->SetDecimals();
	hist_dqdx_corr->GetXaxis()->SetRangeUser(0.,400.);
	hist_dqdx_corr->SetTitle(titlehist.Data());
	hist_dqdx_corr->Draw("vector");
	c4->Print(Form("%s.png",cName.Data()),"png");
	
	//1d dqdx median of track
	cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/dqdx_median_corr_plane_%d%s",m,plane,tags[t].c_str());
	titlehist = Form("plane %d, %s; dQ/dx (ADC/cm); Entries", plane, tags[t].c_str());
	TCanvas *c4a = new TCanvas(cName,cName);
	dq_dx_hist_trk->SetLineWidth(3);
	dq_dx_hist_trk->SetLineColor(4);
	dq_dx_hist_trk->GetYaxis()->SetDecimals();
	dq_dx_hist_trk->GetXaxis()->SetRangeUser(0.,400.);
	dq_dx_hist_trk->GetXaxis()->SetTitle("median dQ/dx");
	dq_dx_hist_trk->SetTitle(titlehist.Data());
	dq_dx_hist_trk->Draw("vector");
	c4a->Print(Form("%s.png",cName.Data()),"png");
	
	//1d dedx median of track
	cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/dedx_median_corr_plane_%d%s",m,plane,tags[t].c_str());
	titlehist = Form("plane %d, %s; dE/dx (ADC/cm); Entries", plane, tags[t].c_str());
	TCanvas *c4b = new TCanvas(cName,cName);
	dq_dx_hist_trk->SetLineWidth(3);
	dq_dx_hist_trk->SetLineColor(4);
	dq_dx_hist_trk->GetYaxis()->SetDecimals();
	dq_dx_hist_trk->GetXaxis()->SetRangeUser(0.,5.);
	dq_dx_hist_trk->SetTitle(titlehist.Data());
	dq_dx_hist_trk->GetXaxis()->SetTitle("median dE/dx");
	dq_dx_hist_trk->Draw("vector");
	c4b->Print(Form("%s.png",cName.Data()),"png");
      }//loop over planes
      
      //insert plane per plane to plane of deconvolution
      //clear plane per plane
      dq_dx_x_plane.push_back(dq_dx_x_vector);
      dq_dx_phi_plane.push_back(dq_dx_phi_vector);
      dq_dx_theta_plane.push_back(dq_dx_theta_vector);
      
      de_dx_x_plane.push_back(de_dx_x_vector);
      de_dx_phi_plane.push_back(de_dx_phi_vector);
      de_dx_theta_plane.push_back(de_dx_theta_vector);
      
      ratio_dqde_x_plane.push_back(ratio_dqde_x_vector);
      ratio_dqde_phi_plane.push_back(ratio_dqde_phi_vector);
      ratio_dqde_theta_plane.push_back(ratio_dqde_theta_vector);
      
      ratio_reco_det_plane.push_back(ratio_reco_plane);
      ratio_corr_det_plane.push_back(ratio_corr_plane);
      
      dq_dx_hist_trk_plane.push_back(dq_dx_hist_trk_vector);
      de_dx_hist_trk_plane.push_back(de_dx_hist_trk_vector);
      
      ratio_reco_det_plane_1d.push_back(ratio_reco_plane_1d);
      ratio_corr_det_plane_1d.push_back(ratio_corr_plane_1d);
      
      ratio_reco_plane.clear();
      ratio_corr_plane.clear();
      
      ratio_reco_plane_1d.clear();
      ratio_corr_plane_1d.clear();
      
      dq_dx_hist_trk_vector.clear();
      de_dx_hist_trk_vector.clear();
      
      dq_dx_x_vector.clear();
      dq_dx_phi_vector.clear();
      dq_dx_theta_vector.clear();
      de_dx_x_vector.clear();
      de_dx_phi_vector.clear();
      de_dx_theta_vector.clear();
      ratio_dqde_x_vector.clear();
      ratio_dqde_phi_vector.clear();
      ratio_dqde_theta_vector.clear();
	
    }//tags
    
    //loop over the planes
    for( int plane=0; plane<3; plane++ ){
      //make plots
      //1d dqdx before correction
      TString cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/1dcomparisons_dqdx_yzmap_reco_plane_%d",m,plane);
      TString title = Form("plane %d; Reconstructed dQ/dx (ADC/cm); Entries", plane);
      Draw_Overlay_With_Legend( ratio_reco_det_plane_1d, plane, cName, title); 
      
      //1d dqdx after correction
      cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/1dcomparisons_dqdx_yzmap_corr_plane_%d",m,plane);
      title = Form("plane %d; Corrected dQ/dx (ADC/cm); Entries", plane);
      Draw_Overlay_With_Legend(ratio_corr_det_plane_1d, plane, cName, title);
      
      //dqdx median of track 
      cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/comparisons_dqdx_trackmedian_plane_%d",m,plane);
      title = Form("plane %d; Median dQ/dx of Track; Entries", plane);
      Draw_Overlay_With_Legend(dq_dx_hist_trk_plane, plane, cName, title);
      
      //dedx median of track 
      cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/comparisons_dedx_trackmedian_plane_%d",m,plane);
      title = Form("plane %d; Median dE/dx of Track; Entries", plane);
      Draw_Overlay_With_Legend(de_dx_hist_trk_plane, plane, cName, title);
      
      //dqdx median vs x
      cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/comparisons_dqdx_vs_x_plane_%d",m,plane);
      title = Form("plane %d; x (cm); Entries", plane);
      Draw_Overlay_With_Legend(dq_dx_x_plane, plane, cName, title);
      
      cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/comparisons_dedx_vs_x_plane_%d",m,plane);
      title = Form("plane %d; x (cm); Entries", plane);
      Draw_Overlay_With_Legend(de_dx_x_plane, plane, cName, title);
      
      cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/comparisons_dqde_dx_vs_x_plane_%d",m,plane);
      title = Form("plane %d; x (cm); Entries", plane);
      Draw_Overlay_With_Legend(ratio_dqde_x_plane, plane, cName, title);

      //dqdx median vs theta
      cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/comparisons_dqdx_vs_theta_plane_%d",m,plane);
      title = Form("plane %d; #theta (deg.); Entries", plane);
      Draw_Overlay_With_Legend(dq_dx_theta_plane, plane, cName, title);
      
      cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/comparisons_dedx_vs_theta_plane_%d",m,plane);
      title = Form("plane %d; #theta (deg.); Entries", plane);
      Draw_Overlay_With_Legend(de_dx_theta_plane, plane, cName, title);
      
      cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/comparisons_dqde_dx_vs_theta_plane_%d",m,plane);
      title = Form("plane %d; #theta (deg.); Entries", plane);
      Draw_Overlay_With_Legend(ratio_dqde_theta_plane, plane, cName, title);
      
      //dqdx median vs phi
      cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/comparisons_dqdx_vs_phi_plane_%d",m,plane);
      title = Form("plane %d; #phi (deg.); Entries", plane);
      Draw_Overlay_With_Legend(dq_dx_phi_plane, plane, cName, title);
      
      cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/comparisons_dedx_vs_phi_plane_%d",m,plane);
      title = Form("plane %d; #phi (deg.); Entries", plane);
      Draw_Overlay_With_Legend(de_dx_phi_plane, plane, cName, title);

      cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap_month%d/CSUPlots/comparisons_dqde_dx_vs_phi_plane_%d",m,plane);
      title = Form("plane %d; #phi (deg.); Entries", plane);
      Draw_Overlay_With_Legend(ratio_dqde_phi_plane, plane, cName, title);
    
      
    }//loop over plane

    ratio_reco_det_plane_1d.clear();
    ratio_corr_det_plane_1d.clear();
  }//month
  
  return;
}

void Draw_Overlay_With_Legend(std::vector< std::vector<TH1F*> > histos, int plane, TString canname, TString title, TPad *pad = 0)
{// this function draws the histoname from the TObjArray superimposed
 // and normalised if required

  if( histos.size() == 0 ) return; 
  cout << "HISTO SIZE = " <<  histos.size() << endl;
  //get the normalization scale
 
  //std::vector<std::string> legends_str = {"No E-field Corr.", "E-field Corr."};
  //std::vector<std::string> legends_str = {"pandoraCalo", "pandoraCaloSCE (Space Corr.)", "pandoraCaloSCE (E-field Corr.)", "pandoraCali", "pandoraCaliSCE"};
  std::vector<std::string> legends_str = {"pandoraCalo", "pandoraCaloSCE (Space Corr.)", "pandoraCaloSCE (E-field Corr.)"};
  std::vector<int> colors = {kBlack,kRed,kBlue,kOrange,kGreen};
  
 // lets open and draw the canvas 
  TCanvas *canvas;
  if(pad == 0){
     canvas = new TCanvas(canname,canname,1920,1080);
     pad = (TPad*)canvas->cd();
  }
  pad->cd();
  pad->SetTicks(0,0);
  pad->SetRightMargin(0.20);
 
  TLegend  *legend = new TLegend(0.80,0.2,0.995,0.4); // we need different positions for the legend to not 
 
  for (uint i=0;i<histos.size();i++)
  {
    histos[i][plane]->SetLineWidth(2);
    histos[i][plane]->SetLineColor(colors[i]);
    //histos[i][plane]->SetName(legends_str[i].c_str());
    legend->AddEntry(histos[i][plane],legends_str[i].c_str(),"L");
  }
 
  float heightboxes;
  // the position of the top corner
  float top_corner,deltay;
  top_corner = 0.9;
 
  // if the array has more than 0 histograms lets specify the stat boxes and determine whether we should
  // draw errors 
  if (histos.size() > 0) 
    heightboxes = (float)0.5/(float)histos.size();
  else
    heightboxes = (float)0.5;
 
 
  TPaveStats *st1;

  std::cout << "******* histos size = " << histos.size() << std::endl;
 
  double maxy=-999.;
  double miny=999.;
  for (uint i=0;i<histos.size();i++){
    cout << histos[i][plane]->GetName() << " = " << histos[i][plane]->GetMaximum() << endl; 
    if( histos[i][plane]->GetMaximum() > maxy ) maxy = histos[i][plane]->GetMaximum();
    if( histos[i][plane]->GetMinimum() < miny ) miny = histos[i][plane]->GetMinimum();
  }

  for (uint i=0;i<histos.size();i++)
  {
  //getthescale
  //double scale = NormScale(histos, i, plane );
  //double scale = 1.0;
  //histos[i][plane]->Scale(scale);
  if( i==0 ){ 
    //histos[i][plane]->SetLineStyle(9);
    histos[i][plane]->SetMaximum(1.08*maxy);
    histos[i][plane]->SetMinimum(0.92*miny);
    histos[i][plane]->SetTitle(title);
    //PaintOverflow(histos[i][plane],i);
    histos[i][plane]->Draw("hist");
  }
  else  
    histos[i][plane]->Draw("hist same");
    //PaintOverflow(histos[i][plane],i);
  } 
  pad->Update();
 
  for (uint i=0;i<histos.size();i++)
  {
    if (histos[i][plane] != NULL) {
      // lets modify the stat boxes
      deltay = i*heightboxes;
      st1 = (TPaveStats*)histos[i][plane]->GetListOfFunctions()->FindObject("stats");
      if (st1 != NULL) {
	st1->SetOptStat(1100);
        gStyle->SetOptStat("nemrou");
        st1->SetY1NDC(top_corner-deltay-heightboxes); 
        st1->SetY2NDC(top_corner-deltay);
        st1->SetX1NDC(0.80); 
        st1->SetX2NDC(.995);
        st1->SetTextColor(colors[i]);
        st1->SetTextSize(0.03);
	cout << "opt stat = " << st1->GetOptStat() << endl;
        st1->Draw("");
      }
      else
        printf("NULL\n");
    }
 
  legend->Draw("same");
 
  pad->Update();
  pad->Modified(); // so it updates the pad with the new changes
  pad->Draw("same");
  }
  canvas->Print(Form("%s.png",canname.Data()),"png");

  return;
}

double NormScale(std::vector< std::vector<TH1F*> > histos, int mode, int plane){

  //normalize to unity
  double total_integral = 0.0;
  double scale = histos[mode][plane]->Integral();
  for( int i = 0; i < 2; ++i ){
    total_integral += histos[i][plane]->Integral();
  }
  
  //return (1.0/total_integral);
  return (1.0/scale);
}

void PaintOverflow(TH1 *h, int histo_n)
{
   // This function paint the histogram h with an extra bin for overflows

   string hname  = (std::string)h->GetName();
   TString name  = Form("%s_tmp", hname.c_str());
   TString title = h->GetTitle();
   Int_t nx    = h->GetNbinsX()+1;
   cout << "name, nx = " << name << ", " << nx << endl;
   //if( name.Contains("de_dx_hist_trk") ) nx = std::round(nx/2)+1;
   cout << "name, nx = " << name << ", " << nx << endl;
   Double_t x1 = h->GetBinLowEdge(1);
   Double_t bw = h->GetBinWidth(nx);
   Double_t x2 = h->GetBinLowEdge(nx)+bw;
   cout << h->Integral() << endl;
   // Book a temporary histogram having ab extra bin for overflows
   TH1F *htmp = new TH1F(name, title, nx, x1, x2);

   // Fill the new hitogram including the extra bin for overflows
   for (Int_t i=1; i<=nx; i++) {
      htmp->Fill(htmp->GetBinCenter(i), h->GetBinContent(i));
   }

   // Fill the underflows
   htmp->Fill(x1-1, h->GetBinContent(0));
   // Restore the number of entries
   htmp->SetEntries(h->GetEntries());
   //Get color and Style
   htmp->GetXaxis()->SetTitle(h->GetXaxis()->GetTitle());
   htmp->SetLineWidth(h->GetLineWidth());
   htmp->SetMarkerColor(h->GetLineColor());
   htmp->SetLineColor(h->GetLineColor());
   htmp->SetMarkerStyle(11);
   htmp->SetMarkerSize(0.5);
   htmp->SetStats(0);
   //cout << "maxy = " << h->GetMaximum() << endl;
   //htmp->SetMaximum(230.);
   //htmp->SetMinimum(215.);
   //htmp->SetMinimum(180.);
   // Draw the temporary histogram
   if( histo_n==0 ) htmp->Draw("hist");
   else htmp->Draw("hist same");
}
