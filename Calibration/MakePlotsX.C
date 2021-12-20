#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TAttFill.h>
#include <TAxis.h>
#include <TColor.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPad.h>
#include <TFile.h>
#include <TString.h>


//--- HELPERS FUNCTION ---
//-----------------------------------------
void Draw_Overlay_With_Legend(std::vector<TH1D*> histos,TString canname, std::string xtitle, int type = 1, TPad *pad = 0);
void AddToTmp(TObject * obj );
void AddPlotLabel(const char * label, const double x, const double y, const double size = 0.05, const int color = 1, const int font = 62, const int align = 22, const double angle = 0 );
bool is_file_exist(const char *fileName);	
void PaintOverflow(TH1 *h, int histo_n);
TString getDir( const std::string& subdir );
//-----------------------------------------

void MakePlotsX(std::string dirtag, int year_start, int month_start, int month_end,bool expo = true);


void MakePlotsX(std::string dirtag, int year_start, int month_start, int month_end, bool expo ){

	ofstream outfile;
	std::string fname1; 

	//std::vector< std::string > tags = { "_nocorr", "_spacecorr", "_efieldcorr" };
	//std::vector< std::string > taglabels = { "No E field Corr.", "Spatial Corr", "Spatial + E field Corr." };
	std::vector< std::string > tags = { "_efieldcorr" };
	std::vector< std::string > taglabels = { "Spatial + E field Corr." };

	std::vector< std::string > planestr = {"0","1","2"};

	for( int plane=2; plane < 3; plane++ ){
		std::cout << "plane: " << planestr[plane] << std::endl;
		if( expo ) fname1 = "mcc9_1_globalcali_run4_plane" + planestr[plane] + "_lifetime_all_planes_efieldcorr_expo_month0plusShutdown.txt";
		else fname1 = "mcc9_1_globalcali_run4_plane" + planestr[plane] + "_lifetime_all_planes_efieldcorr_pol1_month0plusShutdown.txt";
		if( expo ) fname1 = "mcc9_1_globalcali_run4_plane" + planestr[plane] + "_lifetime_all_planes_efieldcorr_expo_month0plusShutdown.txt";
		else fname1 = "mcc9_1_globalcali_run4_plane" + planestr[plane] + "_lifetime_all_planes_efieldcorr_pol1_month0plusShutdown.txt";

		outfile.open(fname1);

		for( int i_year=year_start; i_year <year_start+1; i_year++ ){ 
			for( int i_month=month_start; i_month < month_end; i_month++ ){ 
				for( int i_date=1; i_date <32; i_date++ ){ 

					//write the time information into a txt file
					//will be used later when making the lifetime over the year plot
					std::cout <<i_year<<"/"<<i_month<<"/"<<i_date<< std::endl;
					outfile<<i_year<<"/"<<i_month<<"/"<<i_date<<"  ";

					TString filename;
					std::vector<TH1D*> reco_dqdx, corr_dqdx, reco_dqdx_t, corr_dqdx_t, dqdx_bin;
					std::vector<TString> datename;

					for( int t=0; t < tags.size(); t++ ){
						filename = Form("/uboone/data/users/wospakrk/DataMap_%s_plane%d/final_calibration_root_files_datadrivenmap%s_month%d/X_correction_factors_%d_month_%d_%d_plane_%d.root",dirtag.c_str(),plane,tags[t].c_str(),i_month,i_year,i_month,i_date,plane);
						cout << filename << endl;

						if( ! (is_file_exist( filename.Data() ) ) ){ 
							outfile<<"0.0  0.0"<< std::endl;
							continue;
						}

						TFile *_file0 = TFile::Open(filename);
						TH1D* dq_dx_x_hist = (TH1D*)_file0->Get(Form("dq_dx_x_hist_plane%d",plane));
						TH1D* corrected_dq_dx_x_hist = (TH1D*)_file0->Get(Form("corrected_dq_dx_x_hist_plane%d",plane));
						TH1D* dq_dx_t_hist = (TH1D*)_file0->Get(Form("dq_dx_t_hist_plane%d",plane));
						TH1D* corrected_dq_dx_t_hist = (TH1D*)_file0->Get(Form("corrected_dq_dx_t_hist_plane%d",plane));
						reco_dqdx.push_back(dq_dx_x_hist);
						corr_dqdx.push_back(corrected_dq_dx_x_hist);
						reco_dqdx_t.push_back(dq_dx_t_hist);
						corr_dqdx_t.push_back(corrected_dq_dx_t_hist);
						datename.push_back(filename);

					}

					if( ! (is_file_exist( filename.Data() ) ) ) continue;
					TGaxis::SetMaxDigits(4);

					//========================================
					// drift distance
					//========================================
					TString plotdir = getDir(Form("/uboone/data/users/%s/PLOTS_DataMap2_%s",getenv("USER"),dirtag.c_str()));

					TString cName = Form("%s/dq_dx_x_hist_month_%d_%d_plane_%d", plotdir.Data(), i_month,i_date,plane);
					Draw_Overlay_With_Legend( reco_dqdx, cName, "Reconstructed dQ/dx"); 
					cName = Form("%s/corrected_dq_dx_x_hist_month%d_%d_plane_%d", plotdir.Data(), i_month,i_date,plane);
					Draw_Overlay_With_Legend( corr_dqdx, cName, "Corrected dQ/dx");

					//========================================
					// drift time
					//========================================
					cName = Form("%s/dq_dx_t_hist_month%d_%d_plane_%d",plotdir.Data(), i_month,i_date,plane);
					Draw_Overlay_With_Legend( reco_dqdx_t, cName, "Reconstructed dQ/dx"); 
					cName = Form("%s/corrected_dq_dx_t_hist_month%d_%d_plane_%d",plotdir.Data(), i_month,i_date,plane);
					Draw_Overlay_With_Legend( corr_dqdx_t, cName, "Corrected dQ/dx"); 
					//========================================

					//Fitting
					//loop over tags
					for( int t=0; t < tags.size(); t++ ){

						TCanvas *canvas;
						TString plotsubdir;
						if( expo ) plotsubdir = getDir(Form("%s/expo",plotdir.Data()));
						else plotsubdir = getDir(Form("%s/pol1",plotdir.Data()));

						TString canname = Form("%s/Drift_distance_vs_dQdx%s_month%d_%02d_plane%d",plotsubdir.Data(),tags[t].c_str(),i_month,i_date,plane);
						canvas = new TCanvas(canname,canname);
						reco_dqdx[t]->SetLineColor(1);
						reco_dqdx[t]->GetYaxis()->SetTitle("Median dQ/dx [ADC/cm]");
						reco_dqdx[t]->GetXaxis()->SetTitle("Drift distance [cm]");
						reco_dqdx[t]->SetMaximum(1.1*reco_dqdx[t]->GetMaximum());
						reco_dqdx[t]->SetMinimum(0.9*reco_dqdx[t]->GetMinimum());
						reco_dqdx[t]->GetXaxis()->SetRangeUser(0.,260.);
						reco_dqdx[t]->Draw("hist E");

						//slope difference

						//fit stuff
						double m;
						double b;
						double m_err;
						double b_err;
						TF1 *fit;
						if( expo ) fit = new TF1("fit","expo", 0., 210. );
						else fit = new TF1("fit","pol1", 0., 210. );
						reco_dqdx[t]->Fit(fit, "QRN");

						double pars[2];
						fit->GetParameters(pars);
						const double *errs = fit->GetParErrors();
						m = pars[1];
						m_err = errs[1];    
						b = pars[0];
						b_err = errs[0];

						//extract the lifetime
						//------------------------------
						double lifetime = 1./m;
						lifetime /= 0.1098E3;

						//sanity check
						//-----------------------------
						cout << "m, b, lifetime +- error " << m << ", " << b << ", " << lifetime << " +- " << lifetime*(m_err/m) << endl;
						//----------------------------

						string fitParam;
						if( expo ) fitParam = Form( "y = exp[(%.2e #pm %.2e)x]", m, m_err);
						else fitParam = Form( "y = %.2e #pm %.2e)x + %.2f #pm %.4f]", m, m_err,b, b_err);

						if( expo ){
							outfile << m <<"  "<< m_err<<"  "<<std::endl;
						}
						else{	  
							outfile << m <<"  "<< m_err<<"  "<<std::endl;
						}

						string chi2 = Form("#chi^{2} / ndf: %.2f / %d = %.3f",fit->GetChisquare(), fit->GetNDF(), (double)fit->GetChisquare()/fit->GetNDF() );

						fit->SetLineColor(kRed);
						fit->SetLineWidth(2);
						fit->Draw("same");

						AddPlotLabel( fitParam.c_str(), 0.32, 0.85, 0.035, 2);
						AddPlotLabel( chi2.c_str(), 0.33, 0.79, 0.035, 2);
						AddPlotLabel( taglabels[t].c_str(), 0.78, 0.85, 0.05, 6);

						canvas->Update();
						canvas->Print(Form("%s.pdf",canname.Data()),"pdf");

					}  

					//clear vectors
					reco_dqdx.clear();
					corr_dqdx.clear();
					reco_dqdx_t.clear();
					corr_dqdx_t.clear();
					datename.clear();

				}
			}
		}
	}	

	return;
}

void Draw_Overlay_With_Legend(std::vector<TH1D*> histos,TString canname, std::string xtitle, int type = 1, TPad *pad = 0)
{// this function draws the histoname from the TObjArray superimposed
	// and normalised if required

	if( histos.size() == 0 ) return; 

	std::vector<int> colors = {1,2,4};
	std::vector<std::string> legends_str = {"Spatial+E-field Corr."};
	//std::vector<std::string> legends_str = {"No Corr.", "Spatial Corr.", "E-field Corr."};
	if(type==2){ 
		legends_str.clear();
		colors.clear();
		for( int bin=1; bin < 27; bin++ ){
			legends_str.push_back(Form("bin %d", bin) );
			colors.push_back(bin);
		}
	}

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

	int line=0;
	for (uint i=0;i<histos.size();i++)
	{
		if( type == 2 ){
			int col=(i%9)+1;
			if(col==1) line++;
			histos[i]->SetLineColor(col);
			histos[i]->SetLineStyle(line);
		} else {
			histos[i]->SetLineColor(colors[i]);
			histos[i]->SetLineStyle(1); 
		}
		legend->AddEntry(histos[i],legends_str[i].c_str(),"L");
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
		//histos[i]->Scale(1.0/histos[i]->Integral());
		if( i==0 ){ 
			histos[i]->SetLineColor(1);
			//histos[i]->SetTitle("dQ/dx per bin");
			if( ((TString)histos[i]->GetName()).Contains("dq_dx") ){
				histos[i]->SetMaximum((histos[i]->GetMaximum())*1.25);
				histos[i]->SetMinimum((histos[i]->GetMinimum())*0.75);
			}
			else if( ((TString)histos[i]->GetName()).Contains("pitch") ){ 
				histos[i]->SetMaximum( 0.56);
				histos[i]->SetMinimum( 0.42);
			}
			else if( ((TString)histos[i]->GetName()).Contains("charge") ){
				histos[i]->SetMaximum( 110.);
				histos[i]->SetMinimum( 90.);
			}
			histos[i]->GetYaxis()->SetTitle(xtitle.c_str());
			histos[i]->SetMaximum(0.42);
			if( type == 2 ) PaintOverflow(histos[i],i);
			else histos[i]->Draw("hist E");
		}
		else
		{
			//area normalize to plane 0
			histos[i]->SetMaximum(0.42);
			if( type ==2 ) PaintOverflow(histos[i],i);
			else histos[i]->Draw("hist E same");
		}
	}

	pad->Update();

	for (uint i=0;i<histos.size();i++)
	{
		if (histos[i] != NULL) {
			// lets modify the stat boxes
			deltay = i*heightboxes;
			st1 = (TPaveStats*)histos[i]->GetListOfFunctions()->FindObject("stats");
			if (st1 != NULL) {
				st1->SetOptStat(1101);
				st1->SetY1NDC(top_corner-deltay-heightboxes); 
				st1->SetY2NDC(top_corner-deltay);
				st1->SetX1NDC(0.80); 
				st1->SetX2NDC(.995);
				st1->SetTextColor(colors[i]);
			}
			else
				printf("NULL\n");
		}
	}

	legend->Draw("");
	pad->Update();
	pad->Modified(); // so it updates the pad with the new changes
	pad->Draw("");
	canvas->Print(Form("%s.pdf",canname.Data()),"pdf");

	return;
}

void AddPlotLabel  ( const char * 	label,
		const double 	x,
		const double 	y,
		const double 	size = 0.05,
		const int 	        color = 1,
		const int 	        font = 62,
		const int 	        align = 22,
		const double 	angle = 0	 
		)			
{
	TLatex *latex = new TLatex( x, y, label );
	AddToTmp( latex );

	latex->SetNDC();
	latex->SetTextSize(size);
	latex->SetTextColor(color);
	latex->SetTextFont(font);
	latex->SetTextAlign(align);
	latex->SetTextAngle(angle);
	latex->Draw();
}


void AddToTmp(TObject * obj )
{
	//note: build this everytime to avoid obj unused compiler warning
	if( 0 == obj )
	{
		Warning( "MnvPlotter::AddToTmp", "Attempting to add NULL object to garbage.");
		return;
	}
}

bool is_file_exist(const char *fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

void PaintOverflow(TH1 *h, int histo_n)
{
	// This function paint the histogram h with an extra bin for overflows

	string hname  = (std::string)h->GetName();
	TString name  = Form("%s_tmp", hname.c_str());
	TString title = h->GetTitle();
	Int_t nx    = h->GetNbinsX()+1;
	Double_t x1 = h->GetBinLowEdge(1);
	Double_t bw = h->GetBinWidth(nx);
	Double_t x2 = h->GetBinLowEdge(nx)+bw;

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
	htmp->SetLineWidth(h->GetLineWidth());
	htmp->SetLineStyle(h->GetLineStyle());
	htmp->SetMarkerColor(h->GetLineColor());
	htmp->SetLineColor(h->GetLineColor());
	htmp->SetMarkerStyle(11);
	htmp->SetMarkerSize(0.5);
	htmp->SetStats(0);
	// Draw the temporary histogram
	if( histo_n==0 ) htmp->Draw("hist");
	else htmp->Draw("hist same");
}

TString getDir( const std::string& subdir )
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
