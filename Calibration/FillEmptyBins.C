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

void FillEmptyBins(int m){
  
  //std::vector<std::string> tags = {"_nocorr","_spacecorr","_efieldcorr"};
  std::vector<std::string> tags = {"_efieldcorr"};
  
  std::vector<TH2F*> errors;
  std::vector<TH2F*> newmaps;
  std::vector<TH1F*> avg_corrs;
  
  for( int t=0; t< tags.size(); t++ ){

    //loop over the 3 planes
    for( int plane=0;plane < 3; plane++ ){ 
      TString filename = Form("/uboone/data/users/wospakrk/DataMap_MCC9_1_Run4/calibration_mcc9_1_v1.root"); //change this for new input file
      //TString filename = Form("/uboone/data/users/wospakrk/DataMap_Run2Calib/calibration_mcc9_v1.root"); //change this for new input file
      cout << "filename = " << filename << endl;
      TFile *_file0 = TFile::Open(filename,"READ");
      TH2F *error_dq_dx_hist = (TH2F*)_file0->Get(Form("correction_yz_plane%d",plane));
      TH2F *newmap = (TH2F*)error_dq_dx_hist->Clone(Form("newcorrection_yz_plane%d",plane));
      TH1F *filled_val = new TH1F(Form("/uboone/data/users/wospakrk/FilledYZMap_Run4/AvgValue_plane%d",plane),Form("AvgValue_plane%d",plane),200.,0.,20.); //change this for new input file

      for( int it=0; it<15; it++ ){
	std::cout << "do the loop for " << it << std::endl;
	for( int binx = 1; binx < error_dq_dx_hist->GetNbinsX()+1; binx++ ){
	  for( int biny = 1; biny < error_dq_dx_hist->GetNbinsY()+1; biny++ ){
          if( it==0 && binx > 1  ) continue; 
          if( it==1 && biny > 1 ) continue; 
          if( it==2 && binx < error_dq_dx_hist->GetNbinsX()+1 ) continue; 
          if( it==3 && biny > error_dq_dx_hist->GetNbinsY()+1 ) continue; 
	  double corr_dqdx;
          cout << "newmap = " << newmap->Integral() << endl; 
          if( it==0 ) corr_dqdx = error_dq_dx_hist->GetBinContent(binx,biny);
          else corr_dqdx = newmap->GetBinContent(binx,biny);
          
          if( corr_dqdx>3. ) std::cout << "** I HAVE WEIRD NUMBER!! ** " << binx << ", " << biny << " = " << corr_dqdx<< std::endl;
          std::vector<float> interpolations;
	  if( !corr_dqdx ){
	    //get the non-zero bin and average!
	    for( int BinX = binx-1; BinX < binx+2; BinX++ ){
	      for( int BinY = biny-1; BinY < biny+2; BinY++ ){
                float bin;
		if( it == 0 ) bin = error_dq_dx_hist->GetBinContent(BinX,BinY);
		else bin = newmap->GetBinContent(BinX,BinY);
		if( bin != 0 ) interpolations.push_back(bin); 
	      }
	    }
            if( interpolations.size() == 9  && it == 4  ){ 
              float avg_dqdx = std::accumulate( interpolations.begin(), interpolations.end(), 0.0 )/(float)interpolations.size();
              std::cout << "num, denom, avg = " << std::accumulate( interpolations.begin(), interpolations.end(), 0.0 ) << ", " << interpolations.size() << " = " << avg_dqdx << std::endl; 
              newmap->SetBinContent(binx,biny,avg_dqdx);
              filled_val->Fill(avg_dqdx); 
            }
            else if( interpolations.size() > 7  && it == 5  ){ 
              float avg_dqdx = std::accumulate( interpolations.begin(), interpolations.end(), 0.0 )/(float)interpolations.size();
              std::cout << "num, denom, avg = " << std::accumulate( interpolations.begin(), interpolations.end(), 0.0 ) << ", " << interpolations.size() << " = " << avg_dqdx << std::endl; 
              newmap->SetBinContent(binx,biny,avg_dqdx);
              filled_val->Fill(avg_dqdx); 
            }
            else if( interpolations.size() > 6  && it == 6  ){ 
              float avg_dqdx = std::accumulate( interpolations.begin(), interpolations.end(), 0.0 )/(float)interpolations.size();
              std::cout << "num, denom, avg = " << std::accumulate( interpolations.begin(), interpolations.end(), 0.0 ) << ", " << interpolations.size() << " = " << avg_dqdx << std::endl; 
              newmap->SetBinContent(binx,biny,avg_dqdx);
              filled_val->Fill(avg_dqdx); 
            }
            else if( interpolations.size() > 5  && it == 7  ){ 
              float avg_dqdx = std::accumulate( interpolations.begin(), interpolations.end(), 0.0 )/(float)interpolations.size();
              std::cout << "num, denom, avg = " << std::accumulate( interpolations.begin(), interpolations.end(), 0.0 ) << ", " << interpolations.size() << " = " << avg_dqdx << std::endl; 
              newmap->SetBinContent(binx,biny,avg_dqdx);
              filled_val->Fill(avg_dqdx); 
            }
            else if( interpolations.size() > 4 && it == 8  ){ 
              float avg_dqdx = std::accumulate( interpolations.begin(), interpolations.end(), 0.0 )/(float)interpolations.size();
              std::cout << "num, denom, avg = " << std::accumulate( interpolations.begin(), interpolations.end(), 0.0 ) << ", " << interpolations.size() << " = " << avg_dqdx << std::endl; 
              newmap->SetBinContent(binx,biny,avg_dqdx);
              filled_val->Fill(avg_dqdx); 
            }
            else if( interpolations.size() > 3 && it == 9  ){ 
              float avg_dqdx = std::accumulate( interpolations.begin(), interpolations.end(), 0.0 )/(float)interpolations.size();
              std::cout << "num, denom, avg = " << std::accumulate( interpolations.begin(), interpolations.end(), 0.0 ) << ", " << interpolations.size() << " = " << avg_dqdx << std::endl; 
              newmap->SetBinContent(binx,biny,avg_dqdx);
              filled_val->Fill(avg_dqdx); 
            }
            else if( interpolations.size() > 3 && it == 10  ){ 
              float avg_dqdx = std::accumulate( interpolations.begin(), interpolations.end(), 0.0 )/(float)interpolations.size();
              std::cout << "num, denom, avg = " << std::accumulate( interpolations.begin(), interpolations.end(), 0.0 ) << ", " << interpolations.size() << " = " << avg_dqdx << std::endl; 
              newmap->SetBinContent(binx,biny,avg_dqdx);
              filled_val->Fill(avg_dqdx); 
            }
            else if( it > 12  ){ 
              float avg_dqdx = std::accumulate( interpolations.begin(), interpolations.end(), 0.0 )/(float)interpolations.size();
              std::cout << "num, denom, avg = " << std::accumulate( interpolations.begin(), interpolations.end(), 0.0 ) << ", " << interpolations.size() << " = " << avg_dqdx << std::endl; 
              newmap->SetBinContent(binx,biny,avg_dqdx);
              filled_val->Fill(avg_dqdx); 
            }
            interpolations.clear(); 
          }
	  }
	}
      }

      errors.push_back(error_dq_dx_hist);
      avg_corrs.push_back(filled_val); 
      newmaps.push_back(newmap); 
      //_file0->Close();
    }//loop over planes
    TFile *newfile = new TFile(Form("/uboone/data/users/wospakrk/FilledYZMap_Run4/Data_calibration_mcc9_v1%s_take2.root", tags[t].c_str()),"recreate"); //change this for new input file
    
    for( int plane=0;plane < 3; plane++ ){ 
      TH2F* y_z_corr = (TH2F*)newmaps[plane]->Clone(Form("correction_yz_plane%d",plane));
      y_z_corr->SetTitle(Form("Plane %d",plane));
      TH1F* x_corr = new TH1F( Form("correction_x_plane%d",plane),Form("correction_x_plane%d",plane),26,0.,260. );
      for(int bin=1; bin < 27; bin++ ) x_corr->SetBinContent(bin,1.0);
      x_corr->SetTitle(Form("Plane %d",plane));
      TH1F* avg_corr = (TH1F*)avg_corrs[plane]->Clone(Form("hAvgCorr%d",plane));
      avg_corr->SetTitle(Form("Plane %d",plane));
    }
    newfile->Write();
    newfile->Close();
    
  }//tags 
  return;
}
