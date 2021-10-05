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
#include <numeric>

void FillEmptyBins(int m);

void FillEmptyBinsWithOne(int m){
  
  //std::vector<std::string> tags = {"_nocorr","_spacecorr","_efieldcorr"};
  std::vector<std::string> tags = {"_efieldcorr"};
  
  std::vector<TH2F*> errors;
  std::vector<TH2F*> newmaps;
  std::vector<TH1F*> avg_corrs;
  
  for( int t=0; t< tags.size(); t++ ){

    //loop over the 3 planes
    for( int plane=0;plane < 3; plane++ ){ 
      //TString filename = Form("/uboone/data/users/wospakrk/FinalYZMap/MC_calibration_mcc9_v1%s_8ms.root", tags[t].c_str());
      TString filename = "/uboone/app/users/wospakrk/dev_areas/dQdxStudy/MCCalibration/Y_Z_calibration_mcc9_run4calib.root";
      cout << "filename = " << filename << endl;
      TFile *_file0 = TFile::Open(filename,"READ");
      TH2F *error_dq_dx_hist = (TH2F*)_file0->Get(Form("correction_yz_plane%d",plane));
      //TH2F *error_dq_dx_hist = (TH2F*)_file0->Get(Form("hCorr%d",plane));
      TH2F *newmap = (TH2F*)error_dq_dx_hist->Clone(Form("newcorrection_yz_plane%d",plane));

	for( int binx = 1; binx < error_dq_dx_hist->GetNbinsX()+1; binx++ ){
	  for( int biny = 1; biny < error_dq_dx_hist->GetNbinsY()+1; biny++ ){
	  double corr_dqdx = error_dq_dx_hist->GetBinContent(binx,biny);
          if( !corr_dqdx ) newmap->SetBinContent(binx,biny,1.0);
	  }
	}

      errors.push_back(error_dq_dx_hist);
      newmaps.push_back(newmap); 
      //_file0->Close();
    }//loop over planes
    
    TFile *newfile = new TFile("/uboone/data/users/wospakrk/FilledYZMapWithOne/Y_Z_calibration_mcc9_run4calib.root","recreate");
    
    for( int plane=0;plane < 3; plane++ ){ 
      //TH2F* y_z_corr = (TH2F*)newmaps[plane]->Clone(Form("hCorr%d",plane));
      TH2F* y_z_corr = (TH2F*)newmaps[plane]->Clone(Form("correction_yz_plane%d",plane));
      y_z_corr->SetTitle(Form("Plane %d",plane));
      TH1F* x_corr = new TH1F( Form("correction_x_plane%d",plane),Form("correction_x_plane%d",plane),26,0.,260. );
      for(int bin=1; bin < 27; bin++ ) x_corr->SetBinContent(bin,1.0);
      x_corr->SetTitle(Form("Plane %d",plane));
    }
    newfile->Write();
    newfile->Close();
    
  }//tags 
  return;
}
