#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TString.h>
#include <TChain.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <string.h>
#include <cmath>

int MakeLifetimePlotsFromTable( )
{

   FILE *fp1 = fopen("mcc9_1_globalcali_run4_EpochH_oldplane2_2018_lifetime_all_planes_efieldcorr_expo_month0plusShutdown.txt","r");
   FILE *fp2 = fopen("mcc9_1_globalcali_run4_EpochH_newplane2_2018_lifetime_all_planes_efieldcorr_expo_month0plusShutdown.txt","r");
   FILE *fp3 = fopen("mcc9_1_globalcali_run4_EpochH_newT0corrplane2_2018_lifetime_all_planes_efieldcorr_expo_month0plusShutdown.txt","r");
   
   TH1D *hist1 = new TH1D("date_vs_f1","plane 2", 47, 0, 47 ); 
   TH1D *hist2 = new TH1D("date_vs_f2","plane 2", 47, 0, 47 ); 
   TH1D *hist3 = new TH1D("date_vs_f3","plane 2", 47, 0, 47 ); 

   std::string date;
   //float plane0, plane1, plane2, err0, err1, err2;
   float p1, e1, p2, e2, p3, e3;
  
   int bin=0;
   vector<std::string> Dates;
   char line1[500];
   while (fgets(line1,500,fp1)) {
     sscanf(&line1[0], "%s %f %f",&date[0],&p1,&e1);
     double lifetime = -1/p1;
     lifetime /= 0.1098E3;
     double error = (e1/p1)*lifetime;
     
     if( std::isinf(lifetime) ){
        lifetime = -99999999999999.;
        error = 0.;
     }
     hist1->SetBinContent(bin,lifetime);
     hist1->SetBinError(bin,error);
     //if(bin%5==0){ 
       std::cout << "date =" << date.c_str() << ", lifetime = " << lifetime << std::endl;
       Dates.push_back(date.c_str());
     //}
     //else Dates.push_back("");
     bin++;
   }

   bin=0;
   char line2[500];
   while (fgets(line2,500,fp2)) {
     sscanf(&line2[0], "%s %f %f",&date[0],&p2,&e2);
     double lifetime = -1/p2;
     lifetime /= 0.1098E3;
     double error = (e2/p2)*lifetime;
     if( std::isinf(lifetime) ){
        lifetime = -99999999999999.;
        error = 0.;
     }
     hist2->SetBinContent(bin,lifetime);
     hist2->SetBinError(bin,error);
     bin++;
   }

   bin=0;
   char line3[500];
   while (fgets(line3,500,fp3)) {
     sscanf(&line3[0], "%s %f %f",&date[0],&p3,&e3);
     double lifetime=-1/p3;
     lifetime /= 0.1098E3;
     double error = (e3/p3)*lifetime;
     if( std::isinf(lifetime) ){
        lifetime = -99999999999999.;
        error = 0.;
     }
     std::cout << "lifetime, error = " << lifetime << ", " << error << std::endl;
     hist3->SetBinContent(bin,lifetime);
     hist3->SetBinError(bin,error);
     bin++;
   }

   TH1D *hist_diff = (TH1D*)hist1->Clone("histo_lifetime_diff");
   hist_diff->Add(hist3,-1.0);

   TH1D *h_delta = new TH1D("delta_lifetime","delta_lifetime",40,-20.,20.);
   for(int bin=1; bin<hist_diff->GetNbinsX();bin++){
     std::cout << "fill histo: " << hist_diff->GetBinContent(bin) << std::endl;
     if( hist_diff->GetBinContent(bin) != 0. ) h_delta->Fill(hist_diff->GetBinContent(bin));
   }

   gStyle->SetOptStat(0);
   std::cout << "segfault here??" << std::endl;
   {
     TString cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap/Run4_EpochH_comparisons");
     TCanvas *c1 = new TCanvas(cName,cName, 2048, 1200);
     hist1->SetLineWidth(3);
     hist1->SetLineColor(kBlack);
     hist1->SetMarkerColor(kBlack);
     hist1->SetFillStyle(3144);
     hist1->SetMarkerStyle(kFullCircle);
     hist2->SetLineWidth(3);
     hist2->SetLineColor(kBlue);
     hist2->SetMarkerColor(kBlue);
     hist2->SetFillStyle(3144);
     hist2->SetMarkerStyle(kFullCircle);
     hist3->SetLineWidth(3);
     hist3->SetLineColor(kRed);
     hist3->SetMarkerColor(kRed);
     hist3->SetFillStyle(3144);
     hist3->SetMarkerStyle(kFullCircle);
     hist1->SetTitle("");
     hist1->GetYaxis()->SetTitle("Lifetime #tau [ms]");
     hist1->GetXaxis()->SetTitle("Date");
     hist1->GetXaxis()->SetLabelSize(0.03);
     std::cout << "dates size: " << Dates.size() << std::endl;
     for (Int_t i = 0; i < Dates.size()-1; i++){
       if(i%5==0){ std::cout << "bin, dates, lifetime = " << i+1 << ", " << Dates[i+1] << ", " << hist1->GetBinContent(i+1) << std::endl; hist1->GetXaxis()->SetBinLabel(hist1->GetXaxis()->FindBin(i + 1.), Dates[i+2].c_str());}
     }
     std::cout << "segfault here 2??" << std::endl;
     hist1->SetMinimum(-10.);
     hist1->SetMaximum(100.);
     hist1->Draw("PE1");
     //hist2->Draw("PE1 same");
     hist3->Draw("PE1 same");


    TLegend  *legend = new TLegend(0.75,0.95,0.95,0.8); // we need different positions for the legend to not 
    legend->AddEntry(hist1,"No Pitch Correction");
    legend->AddEntry(hist3,"T0-Pitch Corrected");
    legend->Draw("same");

     c1->Print(Form("%s.png",cName.Data()),"png");
     //c1->SaveAs("1.png","png");
   }

   std::cout << "segfault here 2??" << std::endl;
   {
     std::cout << "segfault here 3??" << std::endl;
     TString cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap/Run4_EpochH_comparisons_diff");
     TCanvas *c1 = new TCanvas(cName,cName, 2048, 1200);
     std::cout << "segfault here 4??" << std::endl;
     hist_diff->SetLineWidth(3);
     std::cout << "segfault here 5??" << std::endl;
     hist_diff->SetLineColor(kBlack);
     hist_diff->SetMarkerColor(kBlack);
      hist_diff->SetFillStyle(3144);
     hist_diff->SetMarkerStyle(kFullCircle);
     hist_diff->SetTitle("");
     hist_diff->GetYaxis()->SetTitle("Lifetime #tau [ms]");
     hist_diff->GetXaxis()->SetTitle("Date");
     hist_diff->GetXaxis()->SetLabelSize(0.03);
     std::cout << "dates size: " << Dates.size() << std::endl;
     for (Int_t i = 0; i < Dates.size()-1; i++){
       if(i%5==0){ std::cout << "bin, dates, lifetime = " << i+1 << ", " << Dates[i+1] << ", " << hist_diff->GetBinContent(i+1) << std::endl; hist_diff->GetXaxis()->SetBinLabel(hist_diff->GetXaxis()->FindBin(i + 1.), Dates[i+2].c_str());}
     }
     hist_diff->SetMinimum(-10);
     hist_diff->SetMaximum(20);
     hist_diff->Draw("PE1");

    //TLegend  *legend = new TLegend(0.75,0.95,0.95,0.8); // we need different positions for the legend to not 
    //legend->AddEntry(hist_diff,"No Pitch Correction");
    //legend->AddEntry(hist3,"T0-Pitch Corrected");
    //legend->Draw("same");

     c1->Print(Form("%s.png",cName.Data()),"png");
     //c1->SaveAs("1.png","png");
   }
   {
   std::cout << "segfault here 6??" << std::endl;
     TString cName = Form("/uboone/data/users/wospakrk/PLOTS_DataDrivenMap/Run4_EpochH_comparisons_delta1D");
     TCanvas *c1 = new TCanvas(cName,cName, 2048, 1200);
     std::cout << "segfault here 7??" << std::endl;
     gStyle->SetOptStat(1111111);
     h_delta->SetLineWidth(3);
     std::cout << "segfault here 8??" << std::endl;
     h_delta->SetLineColor(kBlack);
     h_delta->SetMarkerColor(kBlack);
     h_delta->SetFillStyle(3144);
     h_delta->SetMarkerStyle(kFullCircle);
     h_delta->SetTitle("");
     h_delta->GetXaxis()->SetTitle("#Delta #tau [ms]");
     h_delta->GetYaxis()->SetTitle("Entries");
     h_delta->GetXaxis()->SetLabelSize(0.03);
     //h_delta->SetMinimum(-10);
     //h_delta->SetMaximum(20);
     h_delta->Draw("PE1");

    //TLegend  *legend = new TLegend(0.75,0.95,0.95,0.8); // we need different positions for the legend to not 
    //legend->AddEntry(h_delta,"No Pitch Correction");
    //legend->AddEntry(hist3,"T0-Pitch Corrected");
    //legend->Draw("same");

     c1->Print(Form("%s.png",cName.Data()),"png");
     //c1->SaveAs("1.png","png");
   }


   fclose(fp1);
   fclose(fp2);
   fclose(fp3);
   return 0;
}

void ratioplot( TCanvas *c, TH1D *h1, TH1D *h3, vector<std::string> Dates){

   //gStyle->SetOptTitle(0);
   // Upper plot will be in pad1
   TPad *pad1 = new TPad("pad1", "pad1", 0., 0.5, 1.0, 1.0);
   pad1->SetBottomMargin(0.05); // Upper and lower plot are joined
   pad1->SetTopMargin(0.2); // Upper and lower plot are joined
   pad1->SetGridx(2);        // Vertical grid
   pad1->Draw();             // Draw the upper pad: pad1
   pad1->cd();               // pad1 becomes the current pad
   //gPad->SetLogy();
   h1->SetStats(0);
   h1->GetXaxis()->SetLabelSize(0);
   h1->GetXaxis()->SetTitleSize(0);
   h1->GetYaxis()->SetLabelSize(0.05);
   h1->GetYaxis()->SetTitleSize(60);
   h1->SetMaximum(300);
   h1->SetMinimum(-300);
   h1->Draw("hist P");       // Draw h1

   TLine *line = new TLine(0,0.,217.,0.);
   line->SetLineWidth(1);
   line->SetLineColor(kRed);
   line->SetLineStyle(3);
   line->Draw();
   TLatex pt;
   pt.SetTextSize(0.05);
   pt.SetTextColor(kRed);
   pt.DrawLatex(0.1,0.1,"#tau = 0");

   //Add Plot Title!
   TLatex *latex = new TLatex( 0.5, 0.9, "Run 4, Collection Plane" );
   latex->SetNDC();
   latex->SetTextSize(45);
   latex->SetTextColor(kBlack);
   latex->SetTextFont(43);
   latex->SetTextAlign(22);
   latex->SetTextAngle(0);
   latex->Draw();

   // Do not draw the Y axis label on the upper plot and redraw a small
   // axis instead, in order to avoid the first label (0) to be clipped.
   //h1->GetYaxis()->SetLabelSize(0.);
/*
   TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
   axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
   axis->SetLabelSize(15);
   axis->Draw();
*/

   // lower plot will be in pad
   c->cd();          // Go back to the main canvas before defining pad2
   TPad *pad2 = new TPad("pad2", "pad2", 0, 0.0, 1, 0.5);
   //pad2->SetTopMargin(0.1);
   pad2->SetBottomMargin(0.25);
   pad2->SetGridx(); // vertical grid
   pad2->Draw();
   pad2->cd();       // pad2 becomes the current pad

   h3->SetLineColor(kMagenta);
   h3->SetMinimum(-25);  // Define Y ..
   h3->SetMaximum(10); // .. range
   //h3->Sumw2();
   h3->SetStats(0);      // No statistics on lower plot
   h3->SetMarkerStyle(8);
   h3->SetMarkerSize(0.5);
   h3->Draw("ep");       // Draw the ratio plot

   TLine *l1 = new TLine(0.,5.,218.,5.);
   l1->SetLineWidth(1);
   l1->SetLineColor(kRed);
   l1->SetLineStyle(3);
   l1->Draw();
   pt.SetTextSize(0.05);
   pt.SetTextColor(kRed);
   pt.DrawLatex(218.,5.,"#tau = 37 ms");

   TLine *l2 = new TLine(0.,-5.,218.,-5.);
   l2->SetLineWidth(1);
   l2->SetLineColor(kRed);
   l2->SetLineStyle(3);
   l2->Draw();
   pt.SetTextSize(0.05);
   pt.SetTextColor(kRed);
   pt.DrawLatex(218,-5,"#tau = 37 ms");

   TLine *l3 = new TLine(0.,-8.5,218.,-8.5);
   l3->SetLineWidth(1);
   l3->SetLineColor(kBlue);
   l3->SetLineStyle(3);
   l3->Draw();
   pt.SetTextSize(0.05);
   pt.SetTextColor(kBlue);
   pt.DrawLatex(218,-9,"#tau = 21.6 ms");

   // h1 settings
   h1->SetMarkerColor(kBlue+1);
   h1->SetLineColor(kBlue+1);
   h1->SetLineWidth(2);

   // Y axis h1 plot settings
   h1->GetYaxis()->SetTitleSize(40);
   h1->GetYaxis()->SetTitleFont(43);
   h1->GetYaxis()->SetTitleOffset(1.0);

   // Ratio plot (h3) settings
   h3->SetTitle(""); // Remove the ratio title

   // Y axis ratio plot settings
   h3->GetYaxis()->SetTitle("slope of the linear fit [%]");
   h3->GetYaxis()->SetNdivisions(505);
   h3->GetYaxis()->SetTitleSize(40);
   h3->GetYaxis()->SetTitleFont(43);
   h3->GetYaxis()->SetTitleOffset(1.);
   h3->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
   h3->GetYaxis()->SetLabelSize(30);

   // X axis ratio plot settings
   for (Int_t i = 0; i < Dates.size()-1; i++){
     if(i%5==0)h3->GetXaxis()->SetBinLabel(h3->GetXaxis()->FindBin(i+1), Dates[i].c_str());
   }
   h3->GetXaxis()->SetTitle("Dates");
   h3->GetXaxis()->SetTitleSize(45);
   h3->GetXaxis()->SetTitleFont(43);
   h3->GetXaxis()->SetTitleOffset(3);
   h3->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
   h3->GetXaxis()->SetLabelSize(15);

}
