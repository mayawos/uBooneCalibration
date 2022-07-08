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

//dir1="/uboone/data/users/wospakrk/DataMap_FinalMCC9Calib/final_calibration_root_files_datadrivenmap_efieldcorr_month0/"
//dir2="/uboone/data/users/wospakrk/DataMap_Run4Calib/final_calibration_root_files_datadrivenmap_efieldcorr_month0"

void MakeRatioPlot(TString dir1, TString dir2);

void MakeRatioPlot(TString dir1, TString dir2){
 
  std::vector<TH2F*> ratios_dqdx, ratios_errors;
  std::vector<TH2F*> deltas_dqdx, deltas_errors;
  
  //loop over the 3 planes
  for( int plane=0;plane < 3; plane++ ){ 
    TString filename0 = Form("%s/YZ_correction_factors_2016_month_0_plane_%d.root",dir1,plane);
    TString filename1 = Form("%s/YZ_correction_factors_2016_month_0_plane_%d.root",dir2,plane);
      
    TFile *_file0 = TFile::Open(filename0,"READ");
    TFile *_file1 = TFile::Open(filename1,"READ");
    cout << "filename1 = " << filename1 << endl; 
   
    TH2F* dq_dx_z_vs_y_hist0 = (TH2F*)_file0->Get(Form("dq_dx_z_vs_y_hist_plane%d",plane));
    TH2F* dq_dx_z_vs_y_hist1 = (TH2F*)_file1->Get(Form("dq_dx_z_vs_y_hist_plane%d",plane));
    
    TH2F *ratio_dqdx = (TH2F*)dq_dx_z_vs_y_hist0->Clone(Form("ratio_dq_dx_z_vs_y_hist"));
    TH2F *delta_dqdx = (TH2F*)dq_dx_z_vs_y_hist0->Clone(Form("delta_dq_dx_z_vs_y_hist"));
    
    TH2F *error_dq_dx_hist0 = (TH2F*)_file0->Get(Form("error_dq_dx_z_vs_y_hist_plane%d",plane));
    TH2F *error_dq_dx_hist1 = (TH2F*)_file1->Get(Form("error_dq_dx_z_vs_y_hist_plane%d",plane));
    
    TH2F *ratio_err = (TH2F*)error_dq_dx_hist0->Clone(Form("ratio_error_dq_dx_z_vs_y_hist"));
    TH2F *delta_err = (TH2F*)error_dq_dx_hist0->Clone(Form("delta_error_dq_dx_z_vs_y_hist"));
    
    ratio_dqdx->SetTitle(Form("dQ/dx ratio (Run 1 / Run2) plane%d",plane));
    ratio_dqdx->Divide(dq_dx_z_vs_y_hist1);
    
    ratio_err->SetTitle(Form("YZ correction factor ratio (Run 1 / Run2) plane%d",plane));
    ratio_err->Divide(error_dq_dx_hist1);
    
    delta_dqdx->SetTitle(Form("dQ/dx delta (non-T0 corr - T0 corr) plane%d",plane));
    dq_dx_z_vs_y_hist1->Scale(-1.0);
    delta_dqdx->Add(dq_dx_z_vs_y_hist1);
    
    delta_err->SetTitle(Form("YZ correction factor delta (non-T0 corr - T0 corr) plane%d",plane));
    error_dq_dx_hist1->Scale(-1.0);
    delta_err->Add(error_dq_dx_hist1);
    
    ratios_dqdx.push_back(ratio_dqdx);
    ratios_errors.push_back(ratio_err);
    deltas_dqdx.push_back(delta_dqdx);
    deltas_errors.push_back(delta_err);
    
    //_file0->Close();
    
  }//loop over planes
  
  TFile *ratiofile =  new TFile("/uboone/data/users/wospakrk/DataDataRatio_debug/ratios_and_deltas_dqdx.root", "recreate");
  for( int plane=0;plane < 3; plane++ ){ 
    TH2F* y_z_ratio = (TH2F*)ratios_dqdx[plane]->Clone(Form("2d_ratio_dqdx_plane%d",plane));
    y_z_ratio->SetMaximum(3.0);
    y_z_ratio->SetMinimum(0.5);
    TH2F* y_z_delta = (TH2F*)deltas_dqdx[plane]->Clone(Form("2d_delta_dqdx_plane%d",plane));
    y_z_delta->SetMaximum(300.);
    y_z_delta->SetMinimum(-300.);
    TH2F* y_z_ratio_err = (TH2F*)ratios_errors[plane]->Clone(Form("2d_ratio_err_plane%d",plane));
    y_z_ratio_err->SetMaximum(3.0);
    y_z_ratio_err->SetMinimum(0.5);
    TH2F* y_z_delta_err = (TH2F*)deltas_errors[plane]->Clone(Form("2d_delta_err_plane%d",plane));
    y_z_delta_err->SetMaximum(3.0);
    y_z_delta_err->SetMinimum(0.5);
    
    TString title = Form("plane %d", plane);
    TH1F *hist_dqdx_ratio = new TH1F(Form("ratio_reco_plane%d",plane),title,300,0.,3.);
    TH1F *hist_dqdx_delta = new TH1F(Form("delta_reco_plane%d",plane),title,300,-3.,3.);
    TH1F *hist_err_ratio = new TH1F(Form("ratio_err_plane%d",plane),title,300,0.,3.);
    TH1F *hist_err_delta = new TH1F(Form("delta_err_plane%d",plane),title,300,-3.,3.);
    
    //loop over bin in x
    for( int binx = 0; binx < y_z_ratio->GetNbinsX(); binx++ ){ 
      //loop over bin in y
      for( int biny = 0; biny < y_z_ratio->GetNbinsY(); biny++ ){
	double dQdx1 = y_z_ratio->GetBinContent(binx,biny);
	double dQdx2 = y_z_delta->GetBinContent(binx,biny);
      	double err1 = y_z_ratio_err->GetBinContent(binx,biny);
	double err2 = y_z_delta_err->GetBinContent(binx,biny);
        //cout << "dQdx1, dQdx2, err1, err2 = " << dQdx1 << ", " << dQdx2 << ", " << err1 << ", " << err2 << ", " << endl;
	if( dQdx1 != 0. ) hist_dqdx_ratio->Fill(dQdx1);
        hist_dqdx_delta->Fill(dQdx2);
	if( err1 != 0. ) hist_err_ratio->Fill(err1);
	hist_err_delta->Fill(err2);
      }
    } 
    
    //2d dqdx ratio
    {
      TString cName = Form("/uboone/data/users/wospakrk/DataDataRatio_debug/2d_dqdx_yzmap_ratio_plane_%d",plane);
      TString titlehist = Form("Ratio dQ/dx plane %d; Z coordinate [cm]; Y coordinate [cm]", plane);
      TCanvas *c1 = new TCanvas(cName,cName);
      y_z_ratio->GetYaxis()->SetDecimals();
      y_z_ratio->SetMaximum(3.);
      y_z_ratio->SetMinimum(0.);
      y_z_ratio->SetTitle(titlehist.Data());
      y_z_ratio->Draw("colz");
      c1->Print(Form("%s.pdf",cName.Data()),"pdf");
    }
    

    //2d dqdx delta
    {
      TString cName = Form("/uboone/data/users/wospakrk/DataDataRatio_debug/2d_dqdx_yzmap_delta_plane_%d",plane);
      TString titlehist = Form("Delta dQ/dx plane %d; Z coordinate [cm]; Y coordinate [cm]", plane);
      TCanvas *c1 = new TCanvas(cName,cName);
      y_z_delta->GetYaxis()->SetDecimals();
      y_z_delta->SetMaximum(3.);
      y_z_delta->SetMinimum(-3.);
      y_z_delta->SetTitle(titlehist.Data());
      y_z_delta->Draw("colz");
      c1->Print(Form("%s.pdf",cName.Data()),"pdf");
    }
    
    //2d correction ratio
    {
      TString cName = Form("/uboone/data/users/wospakrk/DataDataRatio_debug/2d_dqdx_yzmap_ratio_err_plane_%d",plane);
      TString titlehist = Form("Ratio YZ corr factor plane %d; Z coordinate [cm]; Y coordinate [cm]", plane);
      TCanvas *c1 = new TCanvas(cName,cName);
      y_z_ratio_err->GetYaxis()->SetDecimals();
      y_z_ratio_err->SetMaximum(3.);
      y_z_ratio_err->SetMinimum(0.);
      y_z_ratio_err->SetTitle(titlehist.Data());
      y_z_ratio_err->Draw("colz");
      c1->Print(Form("%s.pdf",cName.Data()),"pdf");
    }
    
    //2d correction delta
    {
      TString cName = Form("/uboone/data/users/wospakrk/DataDataRatio_debug/2d_dqdx_yzmap_delta_err_plane_%d",plane);
      TString titlehist = Form("Delta YZ corr factor plane %d; Z coordinate [cm]; Y coordinate [cm]", plane);
      TCanvas *c1 = new TCanvas(cName,cName);
      y_z_delta_err->GetYaxis()->SetDecimals();
      y_z_delta_err->SetMaximum(3.);
      y_z_delta_err->SetMinimum(-3.);
      y_z_delta_err->SetTitle(titlehist.Data());
      y_z_delta_err->Draw("colz");
      c1->Print(Form("%s.pdf",cName.Data()),"pdf");
    }
    
    //1d corr factor ratio
    TString cName = Form("/uboone/data/users/wospakrk/DataDataRatio_debug/1d_dqdx_yzmap_ratio_plane_%d",plane);
    TString titlehist = Form("plane %d; Ratio dQ/dx; Entries", plane);
    {
      TCanvas *c2 = new TCanvas(cName,cName);
      hist_dqdx_ratio->SetLineWidth(2);
      hist_dqdx_ratio->SetLineStyle(1);
      hist_dqdx_ratio->SetLineColor(kAzure+7);
      hist_dqdx_ratio->GetYaxis()->SetDecimals();
      hist_dqdx_ratio->GetXaxis()->SetRangeUser(0.,3.);
      hist_dqdx_ratio->SetTitle(titlehist.Data());
      hist_dqdx_ratio->Draw("hist E");
      c2->Print(Form("%s.pdf",cName.Data()),"pdf");
    }
    
    //1d corr factor delta
    cName = Form("/uboone/data/users/wospakrk/DataDataRatio_debug/1d_dqdx_yzmap_delta_plane_%d",plane);
    titlehist = Form("plane %d; Delta dQ/dx; Entries", plane);
    {
      TCanvas *c2 = new TCanvas(cName,cName);
      hist_dqdx_delta->SetLineWidth(2);
      hist_dqdx_delta->SetLineStyle(1);
      hist_dqdx_delta->SetLineColor(kAzure+7);
      hist_dqdx_delta->GetYaxis()->SetDecimals();
      hist_dqdx_delta->GetXaxis()->SetRangeUser(-300.,300.);
      hist_dqdx_delta->SetTitle(titlehist.Data());
      hist_dqdx_delta->Draw("hist E");
      c2->Print(Form("%s.pdf",cName.Data()),"pdf");
    }
    
    //1d corr factor ratio
    cName = Form("/uboone/data/users/wospakrk/DataDataRatio_debug/1d_dqdx_yzmap_ratio_err_plane_%d",plane);
    titlehist = Form("plane %d; Ratio Correction Factor; Entries", plane);
    {
      TCanvas *c2 = new TCanvas(cName,cName);
      hist_err_ratio->SetLineWidth(2);
      hist_err_ratio->SetLineStyle(1);
      hist_err_ratio->SetLineColor(kAzure+7);
      hist_err_ratio->GetYaxis()->SetDecimals();
      hist_err_ratio->GetXaxis()->SetRangeUser(0.,3.);
      hist_err_ratio->SetTitle(titlehist.Data());
      hist_err_ratio->Draw("hist E");
      c2->Print(Form("%s.pdf",cName.Data()),"pdf");
    }
    
    //1d corr factor delta
    cName = Form("/uboone/data/users/wospakrk/DataDataRatio_debug/1d_dqdx_yzmap_delta_err_plane_%d",plane);
    titlehist = Form("plane %d; Delta Correction Factor; Entries", plane);
    {
      TCanvas *c2 = new TCanvas(cName,cName);
      hist_err_delta->SetLineWidth(2);
      hist_err_delta->SetLineStyle(1);
      hist_err_delta->SetLineColor(kAzure+7);
      hist_err_delta->GetYaxis()->SetDecimals();
      hist_err_delta->GetXaxis()->SetRangeUser(-3.,3.);
      hist_err_delta->SetTitle(titlehist.Data());
      hist_err_delta->Draw("hist E");
      c2->Print(Form("%s.pdf",cName.Data()),"pdf");
    }
    
  }
  ratiofile->Write(); 
  ratiofile->Close();  
  
  return;
}
