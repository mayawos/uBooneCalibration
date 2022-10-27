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


void MakeRatioPlot(TH1D *hist, TH1D *slope, vector<std::string> Dates, int plane, int nlines);
void MakeRatioPlotOverlay(std::vector<TH1D*> histos, std::vector<TH1D*> slopes, vector<std::string> Dates, int nlines);
void ratioplot( TCanvas *c, TH1D *h1, TH1D *h3, vector<std::string> Dates, int nlines);
void ratioplotoverlay( TCanvas *c, TH1D *h1p0, TH1D *h3p0, TH1D *h1p1, TH1D *h3p1, TH1D *h1p2, TH1D *h3p2, vector<std::string> Dates, int nlines);

int MakeLifetimePlotsFromTable(std::string lifetime_file, std::string slope_file, int pl, bool latex, bool verbose=true )
{

	FILE *fp = fopen(lifetime_file.c_str(),"r");
   	FILE *fs = fopen(slope_file.c_str(),"r");
   	ofstream lifetime_db;
	if(latex) lifetime_db.open("lifetime_table.tex");
	else lifetime_db.open("lifetime_table.db");
        
   
	int countline=0;
	ifstream in(lifetime_file);
	std::string unused;
	while ( std::getline(in, unused) ) countline++;

   	std::vector<TH1D*> hist_planes;
   	std::vector<TH1D*> slope_planes;
   	for(int p=0; p < 3; p++){
   		TH1D *hist_plane = new TH1D(Form("date_vs_plane%d",p),"", countline, 0, countline );
		hist_planes.push_back(hist_plane);
   		TH1D *slope_plane = new TH1D(Form("date_vs_slope%d",p),"", countline, 0, countline );
		slope_planes.push_back(slope_plane);
   	}

	std::string date;
	std::vector<float> planes(3,0.), errs(3,0.);
	std::vector<float> slopes(3,0.), errslps(3,0.);
	if(latex){ 
		lifetime_db << "\\begin{table}[!h] \n";
    		lifetime_db << "\\centering \n";
    		lifetime_db << "\\begin{tabular}{c|c|} \n";
		lifetime_db << "Date & lifetime $\\pm$ lifetime_err \\\\ \n";
	}
	else lifetime_db << "Date lifetime lifetime_err" << std::endl;  
	int bin=0;
	vector<std::string> Dates;
	char line[500];
	//char *date[100];
	//read the lifetime txtfiles
	while (fgets(line,500,fp)) {
  		sscanf(&line[0], "%s %f %f",&date[0],&planes[pl],&errs[pl]);
  		if(bin%1==0){ 
    			if(verbose) std::cout << "date =" << date.c_str() << std::endl;
    			Dates.push_back(date.c_str());
  		}
  		else Dates.push_back("");
  		if(verbose) std::cout << "plane 2 = " << planes[pl] << std::endl;
  		double lifetime, stat_err, lifetime_text, stat_err_text;
  		if( planes[pl] == 0. ){
    			lifetime = -99999999999999.;
    			stat_err = 0.;
  		}else{
    			lifetime=(-1.0/planes[pl]);
    			lifetime /= 109.8;
    			stat_err = (errs[pl]/planes[pl])*lifetime;
  		}

		if(planes[pl] == 0.){
			lifetime_text = 0.;
			stat_err_text = 0.;
		}else{
			lifetime_text = fabs(lifetime);
			stat_err_text = fabs(stat_err);
		}

  		if(latex) lifetime_db << date.c_str() << " & " << fabs(lifetime_text) << " $\\pm$ " << fabs(stat_err_text) << "\\\\ \n";
  		else lifetime_db << date.c_str() << " " << fabs(lifetime_text) << " " << fabs(stat_err_text) << " \n";
  		hist_planes[pl]->SetBinContent(bin,lifetime);
  		hist_planes[pl]->SetBinError(bin,stat_err);
  		bin++;
	}
	if(latex){
		lifetime_db << "\\hline \n";
		lifetime_db << "\\end{tabular} \n";
		lifetime_db << "\\end{table} \n";
	}

	bin=0;
	char line_s[500];
	//char *date[100];
	//read the linear slope txtfiles
	while (fgets(line_s,500,fs)) {
  		sscanf(&line_s[0], "%s %f %f",&date[0],&slopes[pl],&errslps[pl]);
  		double diff=slopes[pl]*100.;
  		if( diff == 0. ){
     			diff = -99999999999999.; //set the point outside the canvas
     			errslps[pl] = 0.;
  		}
  		slope_planes[pl]->SetBinContent(bin,diff);
  		slope_planes[pl]->SetBinError(bin,100*errslps[pl]);
  		bin++;
	}

	gStyle->SetOptStat(0);

	// only make lifetime plots for one of the plane -- ONLY PLANE 2 for calibration
	MakeRatioPlot(hist_planes[pl],slope_planes[pl],Dates,pl,countline);
	
	fclose(fp);
	fclose(fs);
	return 0;

}

void MakeRatioPlot(TH1D *hist, TH1D *slope, vector<std::string> Dates, int plane, int nlines)
{ 
	std::string  planestr="Induction Plane U";
	if(plane==1) planestr="Induction Plane V";
	if(plane==2) planestr="Collection Plane";
	TString cName = Form("lifetime_efieldcorr_plane%d",plane);
	TCanvas *c1 = new TCanvas(cName,cName, 2000., 1280.);
	//hist->SetTitle(Form("Run 4, %s",planestr.c_str()));
	hist->SetTitle("Run 1-5 e- Lifetime");
	hist->SetLineWidth(3);
	hist->SetLineColor(4);
	hist->SetFillColor(4);
	hist->SetFillStyle(3144);
	hist->SetMarkerStyle(kFullCircle);
	hist->GetYaxis()->SetTitle("Lifetime #tau [ms]");
	hist->GetXaxis()->SetTitle("Date");
	hist->GetXaxis()->SetLabelSize(0.03);
	hist->SetMinimum(-200.);
	hist->SetMaximum(200.);
	hist->Draw("P HIST");
	ratioplot(c1, hist, slope, Dates, nlines);
	c1->Print(Form("%s.pdf",cName.Data()),"pdf");
}

void MakeRatioPlotOverlay(std::vector<TH1D*> histos, std::vector<TH1D*> slopes, vector<std::string> Dates, int nlines)
{
	TString cName = Form("lifetime_efieldcorr_allplanes");
	TCanvas *c1 = new TCanvas(cName,cName, 2000., 1280.);
	for(int p=2; p >=0 ; p--){
		histos[p]->SetLineWidth(3);
		histos[p]->SetLineColor(4);
		histos[p]->SetMarkerStyle(kFullCircle);
	}
	histos[2]->SetTitle("Run 4, All Planes");
	histos[2]->GetYaxis()->SetTitle("Lifetime #tau [ms]");
	histos[2]->GetXaxis()->SetTitle("Date");
	histos[2]->GetXaxis()->SetLabelSize(0.03);
	histos[2]->SetMinimum(-200.);
	histos[2]->SetMaximum(200.);
	histos[2]->Draw("P HIST");
	ratioplotoverlay(c1, histos[0], slopes[0], histos[1], slopes[1], histos[2], slopes[2], Dates, nlines);
	c1->Print(Form("%s.pdf",cName.Data()),"pdf");
}

void ratioplot( TCanvas *c, TH1D *h1, TH1D *h3, vector<std::string> Dates, int nlines){

	//gStyle->SetOptTitle(0);
	// Upper plot will be in pad1
	TPad *pad1 = new TPad("pad1", "pad1", 0., 0.5, 1.0, 1.0);
	pad1->SetBottomMargin(0.05); // Upper and lower plot are joined
	pad1->SetTopMargin(0.2); // Upper and lower plot are joined
	pad1->SetGridx(0);        // Vertical grid
	pad1->Draw();             // Draw the upper pad: pad1
	pad1->cd();               // pad1 becomes the current pad
	//gPad->SetLogy();
	h1->SetStats(0);
	h1->GetXaxis()->SetLabelSize(0);
	h1->GetXaxis()->SetTitleSize(0);
	h1->GetYaxis()->SetLabelSize(0.05);
	h1->GetYaxis()->SetTitleSize(60);
	h1->SetMaximum(37.5);
	h1->SetMinimum(-1.);
	h1->Draw("hist P");       // Draw h1
	
	TLine *line = new TLine(0,0.,float(nlines),0.);
	line->SetLineWidth(1);
	line->SetLineColor(kRed);
	line->SetLineStyle(3);
	line->Draw();
	TLatex pt;
	pt.SetTextSize(0.06);
	pt.SetTextColor(kRed);
	//pt.DrawLatex(-5.0,0.1,"#tau = 0");
	
	//Clear Histogram Title
	//And add plot title using TLatex
	TLatex *latex = new TLatex( 0.5, 0.9, h1->GetTitle() );
	h1->SetTitle("");
	latex->SetNDC();
	latex->SetTextSize(60);
	latex->SetTextColor(kBlack);
	latex->SetTextFont(23);
	latex->SetTextAlign(22);
	latex->SetTextAngle(0);
	latex->Draw();
	
	// Do not draw the Y axis label on the upper plot and redraw a small
	// axis instead, in order to avoid the first label (0) to be clipped.
	//h1->GetYaxis()->SetLabelSize(0.);
	
	//TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
	//axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
	//axis->SetLabelSize(15);
	//axis->Draw();
	
	// lower plot will be in pad
	c->cd();          // Go back to the main canvas before defining pad2
	TPad *pad2 = new TPad("pad2", "pad2", 0, 0.0, 1, 0.5);
	//pad2->SetTopMargin(0.1);
	pad2->SetBottomMargin(0.25);
	pad2->SetGridx(1); // vertical grid
	pad2->Draw();
	pad2->cd();       // pad2 becomes the current pad
	
	h3->SetLineColor(kRed);
	h3->SetMarkerColor(kRed);
	h3->SetMinimum(-29);  // Define Y ..
	//h3->SetMinimum(-10);  // Define Y ..
	h3->SetMaximum(20); // .. range
	//h3->Sumw2();
	h3->SetStats(0);      // No statistics on lower plot
	h3->SetMarkerStyle(8);
	h3->SetMarkerSize(0.5);
	h3->Draw("ep");       // Draw the ratio plot
	
	TLine *l1 = new TLine(0.,5.,float(nlines),5.);
	l1->SetLineWidth(2);
	l1->SetLineColor(kRed);
	l1->SetLineStyle(3);
	l1->Draw();
	pt.SetTextSize(0.055);
	pt.SetTextColor(kRed);
	pt.DrawLatex(1500.,5.,"#tau = 37 ms");
	
	TLine *l2 = new TLine(0.,-5.,float(nlines),-5.);
	l2->SetLineWidth(2);
	l2->SetLineColor(kRed);
	l2->SetLineStyle(3);
	l2->Draw();
	pt.SetTextSize(0.055);
	pt.SetTextColor(kRed);
	pt.DrawLatex(1500,-5,"#tau = 37 ms");
	
	TLine *l3 = new TLine(0.,-8.5,float(nlines),-8.5);
	l3->SetLineWidth(2);
	l3->SetLineColor(kBlue);
	l3->SetLineStyle(3);
	l3->Draw();
	pt.SetTextSize(0.055);
	pt.SetTextColor(kBlue);
	pt.DrawLatex(1500.,-9,"#tau = 21.6 ms");

        //redraw gridlines
	gPad->RedrawAxis("g");

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
	  if(Dates[i]=="2016/2/10"){ 
		TLine *l = new TLine(i,-30.,i,20.);
		l->SetLineWidth(1);
		l->SetLineColor(kPink+2);
		l->SetLineStyle(1);
		l->Draw();
		pt.SetTextSize(0.065);
		pt.SetTextColor(kPink+2);
		pt.SetTextFont(22);
		pt.DrawLatex(i+45,22,"Run 1");
	  	h3->GetXaxis()->SetBinLabel(h3->GetXaxis()->FindBin(i+1), Dates[i].c_str());
	  }
	  if(Dates[i]=="2016/10/16"){
		TLine *l = new TLine(i,-30.,i,20.);
		l->SetLineWidth(1);
		l->SetLineColor(kPink+2);
		l->SetLineStyle(1);
		l->Draw();
		pt.SetTextSize(0.065);
		pt.SetTextColor(kPink+2);
		pt.SetTextFont(22);
		pt.DrawLatex(i+150,22,"Run 2");
	  	h3->GetXaxis()->SetBinLabel(h3->GetXaxis()->FindBin(i+1), Dates[i].c_str());
	  }
	  if(Dates[i]=="2017/10/27"){
		TLine *l = new TLine(i,-30.,i,20.);
		l->SetLineWidth(1);
		l->SetLineColor(kPink+2);
		l->SetLineStyle(1);
		l->Draw();
		pt.SetTextSize(0.065);
		pt.SetTextColor(kPink+2);
		pt.SetTextFont(22);
		pt.DrawLatex(i+150,22,"Run 3");
	  	h3->GetXaxis()->SetBinLabel(h3->GetXaxis()->FindBin(i+1), Dates[i].c_str());
	  }
	  if(Dates[i]=="2018/9/17"){ 
		TLine *l = new TLine(i,-30.,i,20.);
		l->SetLineWidth(1);
		l->SetLineColor(kPink+2);
		l->SetLineStyle(1);
		l->Draw();
		pt.SetTextSize(0.065);
		pt.SetTextColor(kPink+2);
		pt.SetTextFont(22);
		pt.DrawLatex(i+150,22,"Run 4");
	  	h3->GetXaxis()->SetBinLabel(h3->GetXaxis()->FindBin(i+1), Dates[i].c_str());
	  }
	  if(Dates[i]=="2019/10/30"){ 
		TLine *l = new TLine(i,-30.,i,20.);
		l->SetLineWidth(1);
		l->SetLineColor(kPink+2);
		l->SetLineStyle(1);
		l->Draw();
		pt.SetTextSize(0.065);
		pt.SetTextColor(kPink+2);
		pt.SetTextFont(22);
		pt.DrawLatex(i+30,22,"Run 5");
	  	h3->GetXaxis()->SetBinLabel(h3->GetXaxis()->FindBin(i+1), Dates[i].c_str());
	  }
	}
	h3->GetXaxis()->SetTitle("Date");
	h3->GetXaxis()->SetTickLength(0.);
	h3->GetXaxis()->SetTitleSize(45);
	h3->GetXaxis()->SetTitleFont(43);
	h3->GetXaxis()->SetTitleOffset(2);
	h3->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
	h3->GetXaxis()->SetLabelSize(30);
	//h3->GetXaxis()->SetLabelOffset(0.2);
	h3->GetXaxis()->LabelsOption("h");

	TLatex *latex2 = new TLatex( 0.763, 0.95, "MicroBooNE In-Progress" );
	latex2->SetNDC();
	latex2->SetTextSize(35);
	latex2->SetTextColor(kBlack);
	latex2->SetTextFont(43);
	latex2->SetTextAlign(22);
	latex2->SetTextAngle(0);
	//latex2->Draw();
}

void ratioplotoverlay( TCanvas *c, TH1D *h1p0, TH1D *h3p0, TH1D *h1p1, TH1D *h3p1, TH1D *h1p2, TH1D *h3p2, vector<std::string> Dates, int nlines){

	//gStyle->SetOptTitle(0);
	// Upper plot will be in pad1
	TPad *pad1 = new TPad("pad1", "pad1", 0., 0.5, 1.0, 1.0);
	pad1->SetBottomMargin(0.05); // Upper and lower plot are joined
	pad1->SetTopMargin(0.2); // Upper and lower plot are joined
	pad1->SetGridx(2);        // Vertical grid
	pad1->Draw();             // Draw the upper pad: pad1
	pad1->cd();               // pad1 becomes the current pad
	//gPad->SetLogy();
	h1p0->SetStats(0);
	h1p0->GetXaxis()->SetLabelSize(0);
	h1p0->GetXaxis()->SetTitleSize(0);
	h1p0->GetYaxis()->SetLabelSize(0.05);
	h1p0->GetYaxis()->SetTitleSize(60);
	h1p0->SetMaximum(40);
	h1p0->SetMinimum(0);
	h1p0->SetMarkerColor(kBlue);
	h1p1->SetMarkerColor(kRed);
	h1p2->SetMarkerColor(kBlack);
	h1p0->SetLineColor(kBlue);
	h1p1->SetLineColor(kRed);
	h1p2->SetLineColor(kBlack);
	h1p0->Draw("hist P");       // Draw h1
	h1p1->Draw("hist P same");       // Draw h1
	h1p2->Draw("hist P same");       // Draw h1
	
	TLine *line = new TLine(0,0.,30.,0.);
	line->SetLineWidth(1);
	line->SetLineColor(kMagenta);
	line->SetLineStyle(3);
	line->Draw();
	TLatex pt;
	pt.SetTextSize(0.05);
	pt.SetTextColor(kMagenta);
	pt.DrawLatex(0.1,0.1,"#tau = 0");
	
	//Add Plot Title!
	TLatex *latex = new TLatex( 0.5, 0.9, "Run 4, All Planes" );
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
	
	//TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
	//axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
	//axis->SetLabelSize(15);
	//axis->Draw();
	
	
	// lower plot will be in pad
	c->cd();          // Go back to the main canvas before defining pad2
	TPad *pad2 = new TPad("pad2", "pad2", 0, 0.0, 1, 0.5);
	//pad2->SetTopMargin(0.1);
	pad2->SetBottomMargin(0.25);
	//pad2->SetGridx(); // vertical grid
	pad2->Draw();
	pad2->cd();       // pad2 becomes the current pad
	
	h3p0->SetLineColor(kMagenta);
	h3p0->SetMinimum(-22);  // Define Y ..
	h3p0->SetMaximum(0); // .. range
	//h3p0->Sumw2();
	h3p0->SetStats(0);      // No statistics on lower plot
	h3p0->SetMarkerStyle(8);
	h3p0->SetMarkerSize(0.5);
	h3p0->SetMarkerColor(kBlue);
	h3p1->SetMarkerColor(kRed);
	h3p2->SetMarkerColor(kBlack);
	h3p0->SetLineColor(kBlue);
	h3p1->SetLineColor(kRed);
	h3p2->SetLineColor(kBlack);
	h3p0->Draw("ep");       // Draw the ratio plot
	h3p1->Draw("ep same");       // Draw the ratio plot
	h3p2->Draw("ep same");       // Draw the ratio plot
	
	//TLine *l1 = new TLine(0.,5.,float(nlines),5.);
	//l1->SetLineWidth(1);
	//l1->SetLineColor(kMagenta);
	//l1->SetLineStyle(3);
	//l1->Draw();
	//pt.SetTextSize(0.05);
	//pt.SetTextColor(kRed);
	//pt.DrawLatex(31.,5.,"#tau = 37 ms");
	
	TLine *l2 = new TLine(0.,-5.,float(nlines),-5.);
	l2->SetLineWidth(1);
	l2->SetLineColor(kMagenta);
	l2->SetLineStyle(3);
	l2->Draw();
	pt.SetTextSize(0.05);
	pt.SetTextColor(kMagenta);
	pt.DrawLatex(30.5,-5,"#tau = 37 ms");

	TLine *l3 = new TLine(0.,5.,float(nlines),5.);
	l3->SetLineWidth(1);
	l3->SetLineColor(kMagenta);
	l3->SetLineStyle(3);
	l3->Draw();
	pt.SetTextSize(0.05);
	pt.SetTextColor(kMagenta);
	pt.DrawLatex(30.5,5,"#tau = 37 ms");
	/*	
	TLine *l3 = new TLine(0.,-19.5,float(nlines),-19.5);
	l3->SetLineWidth(1);
	l3->SetLineColor(kCyan+3);
	l3->SetLineStyle(3);
	l3->Draw();
	pt.SetTextSize(0.05);
	pt.SetTextColor(kCyan+3);
	pt.DrawLatex(30.5,-20,"#tau = 9.4 ms");
        */	
	// h1 settings
	//h1p0->SetMarkerColor(kBlue+1);
	//h1p0->SetLineColor(kBlue+1);
	h1p0->SetLineWidth(2);
	
	// Y axis h1 plot settings
	h1p0->GetYaxis()->SetTitleSize(40);
	h1p0->GetYaxis()->SetTitleFont(43);
	h1p0->GetYaxis()->SetTitleOffset(1.0);
	
	// Ratio plot (h3p0) settings
	h3p0->SetTitle(""); // Remove the ratio title
	
	// Y axis ratio plot settings
	h3p0->GetYaxis()->SetTitle("slope of the linear fit [%]");
	h3p0->GetYaxis()->SetNdivisions(505);
	h3p0->GetYaxis()->SetTitleSize(40);
	h3p0->GetYaxis()->SetTitleFont(43);
	h3p0->GetYaxis()->SetTitleOffset(1.);
	h3p0->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
	h3p0->GetYaxis()->SetLabelSize(30);
	
	// X axis ratio plot settings
	for (Int_t i = 0; i < (Dates.size()/3)-1; i++){
	  if(i%2==0)h3p0->GetXaxis()->SetBinLabel(h3p0->GetXaxis()->FindBin(i), Dates[i].c_str());
	}
	h3p0->GetXaxis()->SetTitle("Dates");
	h3p0->GetXaxis()->SetTitleSize(45);
	h3p0->GetXaxis()->SetTitleFont(43);
	h3p0->GetXaxis()->SetTitleOffset(3);
	h3p0->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
	h3p0->GetXaxis()->SetLabelSize(15);


}
