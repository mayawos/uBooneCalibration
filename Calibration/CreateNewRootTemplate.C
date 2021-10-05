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

void CreateNewRootTemplate(int m);

void CreateNewRootTemplate(int m){
  
  //std::vector<std::string> tags = {"_nocorr","_spacecorr","_efieldcorr"};
  std::vector<std::string> tags = {"_efieldcorr"};

  std::vector<TH2F*> errors;
  for( int t=0; t< tags.size(); t++ ){
    //loop over the 3 planes
    for( int plane=0;plane < 3; plane++ ){
      TString filename = Form("/uboone/data/users/wospakrk/DataMap_Run2Calib/final_calibration_root_files_datadrivenmap%s_month0/YZ_correction_factors_2016_month_0_plane_%d.root",tags[t].c_str(),plane);
      cout << "filename = " << filename << endl;
      TFile *_file0 = TFile::Open(filename,"READ");
      TH2F *error_dq_dx_hist = (TH2F*)_file0->Get(Form("error_dq_dx_z_vs_y_hist_plane%d",plane));
      errors.push_back(error_dq_dx_hist); 
      //_file0->Close();
    }//loop over planes

    TFile *newfile = new TFile(Form("/uboone/data/users/wospakrk/DataMap_Run2Calib/calibration_mcc9_v1.root"),"recreate");
    
    for( int plane=0;plane < 3; plane++ ){ 
      TH2F* y_z_corr = (TH2F*)errors[plane]->Clone(Form("correction_yz_plane%d",plane));
      y_z_corr->SetTitle(Form("Plane %d",plane));
      TH1F* x_corr = new TH1F(Form("correction_x_plane%d",plane), Form("Plane %d",plane), 26, 0., 260.);
      for( int binx = 1; binx < 27; binx++ ){
	x_corr->SetBinContent(binx,1.0);
      }
    }
    newfile->Write();
    newfile->Close();
    
  }//tags
  
  return;
}
