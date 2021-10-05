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

void MakePlotsYZ(){

  //initialize histograms
  //first store histo per plane and then histo per detolution
  std::vector< std::vector<TH2F*> > ratio_reco_det_plane; 
  std::vector< std::vector<TH2F*> > ratio_corr_det_plane;
  std::vector< std::vector<TH1F*> > ratio_reco_det_plane_1d;
  std::vector< std::vector<TH1F*> > ratio_corr_det_plane_1d;
  std::vector<TH2F*> ratio_reco_plane; 
  std::vector<TH2F*> ratio_corr_plane;
  std::vector<TH1F*> ratio_reco_plane_1d;
  std::vector<TH1F*> ratio_corr_plane_1d;

  std::vector< std::string > tags = { "noSCE", "SCEnoCorr", "SCEwithCorr" };

  //loop over the 2 methods of detolutional
  for( int t=0; t< tags.size(); t++ ){
    //loop over the 3 planes
    for( int plane=0;plane < 3; plane++ ){ 
      
      TString filename = Form("/uboone/data/users/wospakrk/v2/latest_y_z_calibration_%s_plane_%d.root",tags[t].c_str(),plane);
      TFile *_file0 = TFile::Open(filename);
      
      TH2F* dq_dx_z_vs_y_hist = (TH2F*)_file0->Get("dq_dx_z_vs_y_hist");
      TH2F* corrected_dq_dx_hist = (TH2F*)_file0->Get("corrected_dq_dx_hist");
      
      //store the histograms to create the ratio histograms
      ratio_reco_plane.push_back(dq_dx_z_vs_y_hist);
      ratio_corr_plane.push_back(corrected_dq_dx_hist);
      
      //new 1D histogram to hold the dQ/dx value
      TString title = Form("plane %d", plane);
      TH1F *hist_dqdx_reco = new TH1F("dqdx_reco",title,600,0.,600.);
      TH1F *hist_dqdx_corr = new TH1F("dqdx_corr",title,600,0.,600.);
      
      //loop over bin in x
      std::cout << "dq_dx_z_vs_y_hist, tags, plane = " << dq_dx_z_vs_y_hist << ", " << tags[t] << ", " << plane << std::endl;
      for( int binx = 0; binx < dq_dx_z_vs_y_hist->GetNbinsX(); binx++ ){ 
	//loop over bin in y
	for( int biny = 0; biny < dq_dx_z_vs_y_hist->GetNbinsY(); biny++ ){
	  double dQdx_reco = dq_dx_z_vs_y_hist->GetBinContent(binx,biny); 
	  double dQdx_corr = corrected_dq_dx_hist->GetBinContent(binx,biny);
          if( dQdx_reco == 0. ) continue;
	  hist_dqdx_reco->Fill(dQdx_reco);
	  hist_dqdx_corr->Fill(dQdx_corr);
	}
      }
      
      //insert the 1d histograms here:
      ratio_reco_plane_1d.push_back(hist_dqdx_reco);
      ratio_corr_plane_1d.push_back(hist_dqdx_corr);
      
      //Print the histograms per plane per detolution
      
      //YZ map before correction
      TString cName = Form("/uboone/data/users/wospakrk/PLOTS_SCEstudy_v2/dq_dx_z_vs_y_hist_plane_%d_%s",plane,tags[t].c_str());
      TString titlehist = Form("plane %d, %s ", plane, tags[t].c_str());
      TCanvas *c1 = new TCanvas(cName,cName);
      dq_dx_z_vs_y_hist->SetMaximum(600.);
      dq_dx_z_vs_y_hist->SetTitle(titlehist.Data());
      dq_dx_z_vs_y_hist->Draw("colz");
      c1->Print(Form("%s.png",cName.Data()),"png");
      
      //YZ map after correction
      cName = Form("/uboone/data/users/wospakrk/PLOTS_SCEstudy_v2/corrected_dq_dx_z_vs_y_hist_plane_%d_%s",plane,tags[t].c_str());
      titlehist = Form("plane %d, %s", plane, tags[t].c_str());
      TCanvas *c2 = new TCanvas(cName,cName);
      corrected_dq_dx_hist->SetMaximum(600.);
      corrected_dq_dx_hist->SetTitle(titlehist.Data());
      corrected_dq_dx_hist->Draw("colz");
      c2->SetLeftMargin(0.2);
      c2->Print(Form("%s.png",cName.Data()),"png");
      
      //1d dqdx before correction
      cName = Form("/uboone/data/users/wospakrk/PLOTS_SCEstudy_v2/1d_dqdx_yzmap_reco_plane_%d_%s",plane,tags[t].c_str());
      titlehist = Form("plane %d, %s; dQ/dx (ADC/cm); Entries", plane, tags[t].c_str());
      TCanvas *c3 = new TCanvas(cName,cName);
      hist_dqdx_reco->SetLineWidth(3);
      hist_dqdx_reco->SetLineColor(4);
      //hist_dqdx_reco->SetFillColor(4);
      //hist_dqdx_reco->SetFillStyle(3144);
      hist_dqdx_reco->GetYaxis()->SetDecimals();
      hist_dqdx_reco->GetXaxis()->SetRangeUser(0.,400.);
      hist_dqdx_reco->SetTitle(titlehist.Data());
      hist_dqdx_reco->Draw("hist");
      c3->Print(Form("%s.png",cName.Data()),"png");
      
      //1d dqdx after correction
      cName = Form("/uboone/data/users/wospakrk/PLOTS_SCEstudy_v2/1d_dqdx_yzmap_corr_plane_%d_%s",plane,tags[t].c_str());
      titlehist = Form("plane %d, %s; dQ/dx (ADC/cm); Entries", plane,tags[t].c_str());
      TCanvas *c4 = new TCanvas(cName,cName);
      hist_dqdx_corr->SetLineWidth(3);
      hist_dqdx_corr->SetLineColor(4);
      //hist_dqdx_corr->SetFillColor(4);
      //hist_dqdx_corr->SetFillStyle(3144);
      hist_dqdx_corr->GetYaxis()->SetDecimals();
      hist_dqdx_corr->GetXaxis()->SetRangeUser(0.,400.);
      hist_dqdx_corr->SetTitle(titlehist.Data());
      hist_dqdx_corr->Draw("hist");
      c4->Print(Form("%s.png",cName.Data()),"png");
      
    }//loop over planes
    
    //insert vector per plane to vector of detolution
    //clear vector per plane
    ratio_reco_det_plane.push_back(ratio_reco_plane);
    ratio_corr_det_plane.push_back(ratio_corr_plane);
    ratio_reco_det_plane_1d.push_back(ratio_reco_plane_1d);
    ratio_corr_det_plane_1d.push_back(ratio_corr_plane_1d);
    ratio_reco_plane.clear();
    ratio_corr_plane.clear();
    ratio_reco_plane_1d.clear();
    ratio_corr_plane_1d.clear();
    
  }//tags

  //loop over the planes
  for( int plane=0; plane<3; plane++ ){
    //make plots
    //1d dqdx before correction
    TString cName = Form("/uboone/data/users/wospakrk/PLOTS_SCEstudy_v2/1dcomparisons_dqdx_yzmap_reco_plane_%d",plane);
    TString title = Form("plane %d; Reconstructed dQ/dx (ADC/cm); Entries", plane);
    Draw_Overlay_With_Legend( ratio_reco_det_plane_1d, plane, cName, title); 
    /*ratio_reco_det_plane_1d[0][plane]->SetTitle(title.Data());
    TCanvas *c3 = new TCanvas(cName,cName);
    ratio_reco_det_plane_1d[0][plane]->SetLineWidth(3);
    ratio_reco_det_plane_1d[0][plane]->SetLineColor(1);
    ratio_reco_det_plane_1d[2][plane]->SetLineStyle(9);
    ratio_reco_det_plane_1d[0][plane]->Draw("hist");
    for( int t=1; t<3; t++ ){
      ratio_reco_det_plane_1d[t][plane]->SetLineWidth(3);
      ratio_reco_det_plane_1d[t][plane]->SetLineColor(t*2);
      ratio_reco_det_plane_1d[t][plane]->Draw("hist same");
      ratio_reco_det_plane_1d[t][plane]->Draw("hist same");
    }
    c3->Print(Form("%s.png",cName.Data()),"png");
    */

    //1d dqdx after correction
    cName = Form("/uboone/data/users/wospakrk/PLOTS_SCEstudy_v2/1dcomparisons_dqdx_yzmap_corr_plane_%d",plane);
    title = Form("plane %d; Corrected dQ/dx (ADC/cm); Entries", plane);
    Draw_Overlay_With_Legend(ratio_corr_det_plane_1d, plane, cName, title); 
/*    TCanvas *c4 = new TCanvas(cName,cName);
    ratio_corr_det_plane_1d[0][plane]->SetLineWidth(3);
    ratio_corr_det_plane_1d[0][plane]->SetLineColor(1);
    //ratio_corr_det_plane_1d[0][plane]->SetFillColor(4);
    //ratio_corr_det_plane_1d[0][plane]->SetFillStyle(3144);
    ratio_corr_det_plane_1d[0][plane]->GetXaxis()->SetRangeUser(130.,230.);
    ratio_corr_det_plane_1d[2][plane]->SetLineStyle(9);
    ratio_corr_det_plane_1d[0][plane]->Draw("hist");
    for( int t=1; t<3; t++ ){
      ratio_corr_det_plane_1d[t][plane]->SetLineWidth(3);
      ratio_corr_det_plane_1d[t][plane]->SetLineColor(t*2);
      ratio_corr_det_plane_1d[t][plane]->Draw("hist same");
      ratio_corr_det_plane_1d[t][plane]->Draw("hist same");
    }
    c4->Print(Form("%s.png",cName.Data()),"png");    
  */
  }//loop over plane


 /* 
  //loop over the planes
  for( int plane=0; plane<3; plane++ ){
    
    //now we can take the ratio and plot the ratio of histos per plane
    TH2F* ratio_reco = (TH2F*)ratio_reco_det_plane[1][plane]->Clone();
    TH2F* ratio_corr = (TH2F*)ratio_corr_det_plane[1][plane]->Clone();
    TH1F* ratio_reco_1d = (TH1F*)ratio_reco_det_plane_1d[1][plane]->Clone();
    TH1F* ratio_corr_1d = (TH1F*)ratio_corr_det_plane_1d[1][plane]->Clone();
    ratio_reco->Divide(ratio_reco_det_plane[1][plane],ratio_reco_det_plane[0][plane],1.0,1.0);
    ratio_corr->Divide(ratio_corr_det_plane[1][plane],ratio_corr_det_plane[0][plane],1.0,1.0);


    //loop over bin in x
    for( int binx = 0; binx < ratio_reco->GetNbinsX(); binx++ ){ 
	//loop over bin in y
	for( int biny = 0; biny < ratio_reco->GetNbinsY(); biny++ ){
	  double dQdx_reco = ratio_reco->GetBinContent(binx,biny); 
	  double dQdx_corr = ratio_corr->GetBinContent(binx,biny);
          if( dQdx_reco == 0. ) continue;
	  ratio_reco_1d->Fill(dQdx_reco);
	  ratio_corr_1d->Fill(dQdx_corr);
	}
    }
    
    //NOW LET'S MAKE PRETTY PLOTS
    //Print the histograms per plane per detolution 
    //YZ map before correction
    TString cName = Form("/uboone/data/users/wospakrk/PLOTS_SCEstudy_v2dq_dx_z_vs_y_hist_plane_%d",plane);
    TString title = Form("plane %d, 2D/1D detolution", plane);
    TCanvas *c1 = new TCanvas(cName,cName);
    ratio_reco->GetZaxis()->SetRangeUser(0,3.0);
    ratio_reco->SetMaximum(3.0);
    ratio_reco->SetTitle(title.Data());
    ratio_reco->Draw("colz");
    c1->Print(Form("%s.png",cName.Data()),"png");
    
    //YZ map after correction
    cName = Form("/uboone/data/users/wospakrk/PLOTS_SCEstudy_v2corrected_dq_dx_z_vs_y_hist_plane_%d",plane);
    title = Form("plane %d, 2D/1D detolution", plane);
    TCanvas *c2 = new TCanvas(cName,cName);
    ratio_corr->SetMaximum(3.0);
    ratio_corr->SetTitle(title.Data());
    ratio_corr->Draw("colz");
    c2->SetLeftMargin(0.2);
    c2->Print(Form("%s.png",cName.Data()),"png");
    
    //1d dqdx before correction
    cName = Form("/uboone/data/users/wospakrk/PLOTS_SCEstudy_v2/1d_dqdx_yzmap_reco_plane_%d",plane);
    title = Form("plane %d, 2D/1D detolution; Reconstructed dQ/dx (ADC/cm); Entries", plane);
    TCanvas *c3 = new TCanvas(cName,cName);
    ratio_reco_1d->SetLineWidth(3);
    ratio_reco_1d->SetLineColor(4);
    ratio_reco_1d->SetFillColor(4);
    ratio_reco_1d->SetFillStyle(3144);
    ratio_reco_1d->GetXaxis()->SetRangeUser(0.,3.0);
    ratio_reco_1d->SetTitle(title.Data());
    ratio_reco_1d->Draw("hist");
    c3->Print(Form("%s.png",cName.Data()),"png");
    
    //1d dqdx after correction
    cName = Form("/uboone/data/users/wospakrk/PLOTS_SCEstudy_v2/1d_dqdx_yzmap_corr_plane_%d",plane);
    title = Form("plane %d, 2D/1D detolution; Corrected dQ/dx (ADC/cm); Entries", plane);
    TCanvas *c4 = new TCanvas(cName,cName);
    ratio_corr_1d->SetLineWidth(3);
    ratio_corr_1d->SetLineColor(4);
    ratio_corr_1d->SetFillColor(4);
    ratio_corr_1d->SetFillStyle(3144);
    ratio_corr_1d->GetXaxis()->SetRangeUser(0.,3.0);
    ratio_corr_1d->SetTitle(title.Data());
    ratio_corr_1d->Draw("hist");
    c4->Print(Form("%s.png",cName.Data()),"png");    
    
  }//loop over planes
 */ 
  return;
}

void Draw_Overlay_With_Legend(std::vector< std::vector<TH1F*> > histos, int plane, TString canname, TString title, TPad *pad = 0)
{// this function draws the histoname from the TObjArray superimposed
 // and normalised if required

   if( histos.size() == 0 ) return; 
   
  std::vector<std::string> legends_str = {"SCE off", "SCE on, no SCE corr.", "SCE on, with SCE corr."};
  std::vector<int> colors = {1,2,4};
  
 // lets open and draw the canvas 
  TCanvas *canvas;
  if(pad == 0){
     canvas = new TCanvas(canname,canname);
     pad = (TPad*)canvas->cd();
  }
  pad->cd();
  pad->SetTicks(0,0);
  pad->SetRightMargin(0.20);
 
  TLegend  *legend = new TLegend(0.80,0.3,0.995,0.4); // we need different positions for the legend to not 
 
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
 
  for (uint i=0;i<histos.size();i++)
  {
  if( i==0 ){ 
    histos[i][plane]->SetLineColor(1);
    //histos[i][plane]->SetLineStyle(9);
    histos[i][plane]->SetMaximum(1.5*histos[i][plane]->GetMaximum());
    histos[i][plane]->SetTitle(title);
    histos[i][plane]->Draw("hist");
  }
  else{
    histos[i][plane]->Scale(histos[0][plane]->Integral()/histos[i][plane]->Integral());
    histos[i][plane]->Draw("hist same");
  }
  } 
  pad->Update();
 
  for (uint i=0;i<3;i++)
  {
    if (histos[i][plane] != NULL) {
      // lets modify the stat boxes
      deltay = i*heightboxes;
      st1 = (TPaveStats*)histos[i][plane]->GetListOfFunctions()->FindObject("stats");
      if (st1 != NULL) {
        std::cout << "deltay, i, heightboxes = " << deltay << ", " << i << ", " << heightboxes << std::endl;
	st1->SetOptStat(1111);
        st1->SetY1NDC(top_corner-deltay-heightboxes); 
        st1->SetY2NDC(top_corner-deltay);
        st1->SetX1NDC(0.80); 
        st1->SetX2NDC(.995);
        st1->SetTextColor(colors[i]);
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
