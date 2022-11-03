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

void MakePlotsX(std::string dirtag, int year_start, int month_start, int month_end, bool expo = true){

	ofstream outfile;
	std::string fname1; 

	//std::vector< std::string > tags = { "_nocorr", "_spacecorr", "_efieldcorr" };
	//std::vector< std::string > taglabels = { "No E field Corr.", "Spatial Corr", "Spatial + E field Corr." };
	std::vector< std::string > tags = { "_efieldcorr" };
	std::vector< std::string > taglabels = { "Spatial + E field Corr." };

	std::vector< std::string > planestr = {"0","1","2"};

	for( int plane=2; plane < 3; plane++ ){
		std::cout << "plane: " << planestr[plane] << std::endl;
		if( expo ) fname1 = "mcc9_1_lifetime_plane" + planestr[plane] + "_expo.txt";
		else fname1 = "mcc9_1_lifetime_plane" + planestr[plane] + "_pol1.txt";

		outfile.open(Form("/uboone/data/users/%s/uBooneCalibration/%s/Output/%s",getenv("USER"),dirtag.c_str(),fname1.c_str()));

		for( int i_year=year_start; i_year <year_start+1; i_year++ ){ 
			for( int i_month=month_start; i_month <= month_end; i_month++ ){ 
				for( int i_date=1; i_date <32; i_date++ ){ 

					//write the time information into a txt file
					//will be used later when making the lifetime over the year plot
					std::cout <<i_year<<"/"<<i_month<<"/"<<i_date<< std::endl;
					outfile<<i_year<<"/"<<i_month<<"/"<<i_date<<"  ";

					TString filename;
					std::vector<TH1D*> reco_dqdx, corr_dqdx, reco_dqdx_t, corr_dqdx_t, dqdx_bin;
					std::vector<TString> datename;

					for( int t=0; t < tags.size(); t++ ){
					  filename = Form("/uboone/data/users/%s/uBooneCalibration/%s/Histograms/%s_month%d/X_correction_factors_%d_month_%d_%d_plane_%d.root",
							  getenv("USER"),dirtag.c_str(),tags[t].c_str(),i_month,i_year,i_month,i_date,plane);
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
						reco_dqdx[t]->SetTitle(Form("%i-%i-%i, Plane 2",i_year,i_month,i_date));
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
						AddPlotLabel( taglabels[t].c_str(), 0.70, 0.85, 0.05, 6);

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
