#include "TCanvas.h"
#include "TROOT.h"
#include "TH3.h"
#include "TH2.h"
#include "TH1.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TString.h"
#include "TFile.h"


//--Utilities--
void MakePrettyYXPlot(TH3D* h1, TString suffix, int bin);
void MakePrettyXYPlot(TH3D* h1, TString suffix, int bin);
void MakePrettyXZPlot(TH3D* h1, TString suffix, int bin);
void AddPlotLabel(const char * label, const double x, const double y, const double size = 0.05, const int color = 1, const int font = 62, const int align = 22, const double angle = 0 );

//--SCE map correction--
std::vector<double> GetRawCoordinate(std::vector<double> point,TH3D* hX, TH3D* hY, TH3D* hZ);
std::vector<double> GetOffsetsVoxel(std::vector<double> point,TH3D* hX, TH3D* hY, TH3D* hZ);
double TransformX(double xVal);
double TransformY(double yVal);
double TransformZ(double zVal);
std::vector<double> Transform(std::vector<double> point);
bool IsInsideBoundaries(std::vector<double> point);

//void MakeSlices(TString name1, TString name2){
void MakeSlices(){

  TString name1 = "/uboone/app/users/wospakrk/uboonecode_v08_00_00_57_commit/dev/Emap-NTT-500-N3-S500_laserdata_v1098_cathode2548_vectorstart_notshiftcurve.root";
  TString name2 = "/cvmfs/uboone.opensciencegrid.org/products/uboonedata/v08_00_00_57/SpaceCharge/SCEoffsets_dataDriven_laser_bkwd_Jan18.root";
  
  //reset stats opts
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
 
  //open raw laser map
  TFile *file1 = TFile::Open(name1);
  TFile *file2 = TFile::Open(name2);
  
  //name of output histos
  std::vector<TString> historaw_names = {"Distorted_EField_X", "Distorted_EField_Y", "Distorted_EField_Z"};
  std::vector<TString> histovoxel_names = {"hEx", "hEy", "hEz"};
  std::vector<TH3D*> histosrawnew, histosvoxelnew, histosraw, histosvoxel; 
  for(int i=0; i < historaw_names.size(); i++){
    //make 3d histogram of 5x5x5 voxel
    TH3D* histo1 = new TH3D(Form("%s_new", historaw_names[i].Data()), Form("%s_new", historaw_names[i].Data()), 52, 0.0, 260.0, 48, -120, 120., 208, 0., 1040.);
    histosrawnew.push_back(histo1);
    TH3D* histo2 = new TH3D(Form("%s_new", histovoxel_names[i].Data()), Form("%s_new", histovoxel_names[i].Data()), 52, 0.0, 260.0, 48, -120, 120., 208, 0., 1040.);
    histosvoxelnew.push_back(histo2);
    //fetch 3d histograms
    TH3D *hraw = (TH3D*)file1->Get(historaw_names[i]);
    histosraw.push_back(hraw);
    TH3D *hvoxel = (TH3D*)file2->Get(histovoxel_names[i]);
    histosvoxel.push_back(hvoxel);
  }
 
  for(int i=1 ; i<histosrawnew[0]->GetNbinsX() ; ++i) {
    for(int j=1 ; j<histosrawnew[0]->GetNbinsY() ; ++j) {
      for(int k=1 ; k<histosrawnew[0]->GetNbinsZ() ; ++k) {
        //std::cout << "histosrawnew[0]->GetXaxis()->GetBinCenter(i), histosrawnew[0]->GetYaxis()->GetBinCenter(j), histosrawnew[0]->GetZaxis()->GetBinCenter(k) = " << histosrawnew[0]->GetXaxis()->GetBinCenter(i) << ", " << histosrawnew[0]->GetYaxis()->GetBinCenter(j) << ", " << histosrawnew[0]->GetZaxis()->GetBinCenter(k) << std::endl;
        std::vector<double> point = {histosrawnew[0]->GetXaxis()->GetBinCenter(i), histosrawnew[0]->GetYaxis()->GetBinCenter(j), histosrawnew[0]->GetZaxis()->GetBinCenter(k)};
        std::vector<double> theEfieldOffsetsRaw = GetRawCoordinate( point, histosraw[0],histosraw[1],histosraw[2]);
        std::vector<double> theEfieldOffsetsVoxel = GetOffsetsVoxel( point, histosvoxel[0],histosvoxel[1],histosvoxel[2]);
        if(theEfieldOffsetsRaw[0]<=0.02&&theEfieldOffsetsRaw[0]>=-0.05) histosrawnew[0]->SetBinContent(i,j,k,theEfieldOffsetsRaw[0]);
        if(theEfieldOffsetsVoxel[0]<=0.02&&theEfieldOffsetsVoxel[0]>=-0.05) histosvoxelnew[0]->SetBinContent(i,j,k,theEfieldOffsetsVoxel[0]);
        if(theEfieldOffsetsRaw[1]<=0.03&&theEfieldOffsetsRaw[1]>=-0.02) histosrawnew[1]->SetBinContent(i,j,k,theEfieldOffsetsRaw[1]);
        if(theEfieldOffsetsVoxel[1]<=0.03&&theEfieldOffsetsVoxel[1]>=-0.02) histosvoxelnew[1]->SetBinContent(i,j,k,theEfieldOffsetsVoxel[1]);	
	histosrawnew[2]->SetBinContent(i,j,k,theEfieldOffsetsRaw[2]);
	histosvoxelnew[2]->SetBinContent(i,j,k,theEfieldOffsetsVoxel[2]);
      }
    }
  }
  
  //set range
  histosrawnew[0]->GetZaxis()->SetRange(int((histosrawnew[0]->GetNbinsZ()+1)/2.0),int((histosrawnew[0]->GetNbinsZ()+1)/2.0)); 	
  histosrawnew[1]->GetZaxis()->SetRange(int((histosrawnew[0]->GetNbinsZ()+1)/2.0),int((histosrawnew[0]->GetNbinsZ()+1)/2.0)); 	
  histosrawnew[2]->GetYaxis()->SetRange(int((histosrawnew[0]->GetNbinsY()+1)/2.0),int((histosrawnew[0]->GetNbinsY()+1)/2.0)); 	
  histosvoxelnew[0]->GetZaxis()->SetRange(int((histosrawnew[0]->GetNbinsZ()+1)/2.0),int((histosrawnew[0]->GetNbinsZ()+1)/2.0)); 	
  histosvoxelnew[1]->GetZaxis()->SetRange(int((histosvoxelnew[0]->GetNbinsZ()+1)/2.0),int((histosvoxelnew[0]->GetNbinsZ()+1)/2.0)); 	
  histosvoxelnew[2]->GetYaxis()->SetRange(int((histosvoxelnew[0]->GetNbinsY()+1)/2.0),int((histosvoxelnew[0]->GetNbinsY()+1)/2.0)); 	
  
  //draw histograms
  MakePrettyYXPlot(histosvoxelnew[0], "SCE_Voxelized_Map_", int((histosvoxelnew[0]->GetNbinsZ()+1)/2.0));
  MakePrettyYXPlot(histosrawnew[0], "SCE_Raw_Map_", int((histosrawnew[0]->GetNbinsZ()+1)/2.0));
  MakePrettyXYPlot(histosvoxelnew[1], "SCE_Voxelized_Map_", int((histosvoxelnew[1]->GetNbinsZ()+1)/2.0));
  MakePrettyXYPlot(histosrawnew[1], "SCE_Raw_Map_", int((histosrawnew[1]->GetNbinsZ()+1)/2.0));
  MakePrettyXZPlot(histosvoxelnew[2],"SCE_Voxelized_Map_", int((histosvoxelnew[2]->GetNbinsY()+1)/2.0));
  MakePrettyXZPlot(histosvoxelnew[2],"SCE_Raw_Map_", int((histosrawnew[2]->GetNbinsY()+1)/2.0));
  
  return;
  
}

//----------------------------------------------------------------------------
/// Make YX Slice Plot at Z center
void MakePrettyYXPlot(TH3D* h1, TString suffix, int bin) 
{
  TCanvas *canvas;
  TString canname = Form("%sYX_Slice_Zbin_%d", suffix.Data(), bin);
  canvas = new TCanvas(canname,canname);
  h1->SetMaximum(0.02);
  h1->SetMinimum(-0.05);
  h1->GetYaxis()->SetTitle("Y [cm]");
  h1->GetXaxis()->SetTitle("X [cm]");
  h1->Project3D("yx")->Draw("colz");
  AddPlotLabel(Form("YX Slice, Z = %.1f cm", h1->GetZaxis()->GetBinCenter(bin)),0.5,0.95);
  canvas->Update();
  canvas->Print(canname+".pdf","pdf");
}

//----------------------------------------------------------------------------
/// Make XY Slice Plot at Z center
void MakePrettyXYPlot(TH3D* h1, TString suffix, int bin) 
{
  TCanvas *canvas;
  TString canname = Form("%sXY_Slice_Zbin_%d", suffix.Data(), bin);
  canvas = new TCanvas(canname,canname);
  h1->SetMaximum(0.03);
  h1->SetMinimum(-0.02);
  h1->GetYaxis()->SetTitle("Y [cm]");
  h1->GetXaxis()->SetTitle("X [cm]");
  h1->Project3D("xy")->Draw("colz");
  AddPlotLabel(Form("XY Slice, Z = %.1f cm", h1->GetZaxis()->GetBinCenter(bin)),0.5,0.95);
  canvas->Update();
  canvas->Print(canname+".pdf","pdf");
}

//----------------------------------------------------------------------------
/// Make XZ Slice Plot at Y center
void MakePrettyXZPlot(TH3D* h1, TString suffix, int bin) 
{
  TCanvas *canvas;
  TString canname = Form("%sXZ_Slice_Ybin_%d", suffix.Data(), bin);
  canvas = new TCanvas(canname,canname);
  h1->GetYaxis()->SetTitle("Z [cm]");
  h1->GetXaxis()->SetTitle("X [cm]");
  h1->Project3D("xz")->Draw("colz");
  AddPlotLabel(Form("XZ Slice, Y = %.1f cm", h1->GetYaxis()->GetBinCenter(bin)),0.5,0.95);
  canvas->Update();
  canvas->Print(canname+".pdf","pdf");
}

void AddPlotLabel( const char * label,
		const double 	x,
		const double 	y,
		const double 	size = 0.05,
		const int 	color = 1,
		const int 	font = 62,
		const int 	align = 22,
		const double 	angle = 0	 
		)			
{
	TLatex *latex = new TLatex( x, y, label );
	latex->SetNDC();
	latex->SetTextSize(size);
	latex->SetTextColor(color);
	latex->SetTextFont(font);
	latex->SetTextAlign(align);
	latex->SetTextAngle(angle);
	latex->Draw();
}

// ---------------------------------------------------------------------------
// Following functions are adapted from: ubevt/SpaceCharge/SpaceChargeMicroBooNE.cxx 

//----------------------------------------------------------------------------
/// Provides position offsets using coordinate interpolation
std::vector<double> GetRawCoordinate
(std::vector<double> point,TH3D* hX, TH3D* hY, TH3D* hZ)
{
  return {
    0.2739-hX->Interpolate(point[0],point[1],point[2]),
    hY->Interpolate(point[0],point[1],point[2]),
    hZ->Interpolate(point[0],point[1],point[2])
  };
  
}

//----------------------------------------------------------------------------
/// Provides position offsets using voxelized interpolation
std::vector<double> GetOffsetsVoxel
(std::vector<double> point,TH3D* hX, TH3D* hY, TH3D* hZ)
{
  std::vector<double> transformedPoint = Transform(point);
  
  return {
    hX->Interpolate(transformedPoint[0],transformedPoint[1],transformedPoint[2]),
    hY->Interpolate(transformedPoint[0],transformedPoint[1],transformedPoint[2]),
    hZ->Interpolate(transformedPoint[0],transformedPoint[1],transformedPoint[2])
  };
  
}

//----------------------------------------------------------------------------
/// Transform X to SCE X coordinate:  [2.56,0.0] --> [0.0,2.50]
double TransformX(double xVal)
{
  double xValNew;
  xValNew = 2.50 - (2.50/2.56)*(xVal/100.0);
  //xValNew = 1.25;
  
  return xValNew;
}

//----------------------------------------------------------------------------
/// Transform Y to SCE Y coordinate:  [-1.165,1.165] --> [0.0,2.50]
double TransformY(double yVal)
{
  double yValNew;
  yValNew = (2.50/2.33)*((yVal/100.0)+1.165);
  //yValNew -= 1.25;
  
  return yValNew;
}

//----------------------------------------------------------------------------
/// Transform Z to SCE Z coordinate:  [0.0,10.37] --> [0.0,10.0]
double TransformZ(double zVal)
{
  double zValNew;
  zValNew = (10.0/10.37)*(zVal/100.0);
  
  return zValNew;
}

//----------------------------------------------------------------------------
std::vector<double> Transform (std::vector<double> point)
{
  return
    { TransformX(point[0]), TransformY(point[1]), TransformZ(point[2]) };
}

//----------------------------------------------------------------------------
/// Check to see if point is inside boundaries of map (allow to go slightly out of range)
bool IsInsideBoundaries(std::vector<double> point)
{
  return !(
           (point[0] <    0.001) || (point[0] >  255.999)
           || (point[1] < -116.499) || (point[1] >  116.499)
           || (point[2] <    0.001) || (point[2] > 1036.999)
           );
}
