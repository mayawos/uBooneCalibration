//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Dec 21 13:08:32 2018 by ROOT version 6.12/06
// from TTree Event/Event Tree from Reco
// found on file: /uboone/app/users/wospakrk/DataDrivenMap/XYZcorrectionTest_month2.root
//////////////////////////////////////////////////////////

#ifndef X_validation_DataDrivenMap_plane2_h
#define X_validation_DataDrivenMap_plane2_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class X_validation_DataDrivenMap_plane2 {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           event;
   Double_t        evttime;
   Int_t           run;
   Int_t           subrun;
   Int_t           year_month_date;
   Int_t           hour_min_sec;
   Int_t           cross_trks;
   Int_t           hasCalo[3][3];   //[cross_trks]
   Int_t           caloIsValid[3][3];   //[cross_trks]
   Int_t           planenum_exists[3][3];   //[cross_trks]
   Int_t           planenumber;
   Int_t           all_trks;
   Float_t         xprojectedlen[300];   //[all_trks]
   Float_t         trackthetaxz[2];   //[cross_trks]
   Float_t         trackthetayz[2];   //[cross_trks]
   Float_t         trackthetaxy[2];   //[cross_trks]
   Float_t         trackstartpos[3][3];   //[cross_trks]
   Float_t         trackendpos[3][3];   //[cross_trks]
   Int_t           TrkID[2];   //[cross_trks]
   Float_t         trkstartcosxyz[3][3];   //[cross_trks]
   Float_t         trkendcosxyz[3][3];   //[cross_trks]
   Int_t           ntrkhits[3][3];   //[cross_trks]
   Int_t           ntrajpoints[2];   //[cross_trks]
   Float_t         trkdqdx[3][3][3000];   //[cross_trks]
   Float_t         trkdedx[3][3][3000];   //[cross_trks]
   Float_t         trkresrange[3][3][3000];   //[cross_trks]
   Float_t         trkdqdxSCE[3][3][3000];   //[cross_trks]
   Float_t         trkdedxSCE[3][3][3000];   //[cross_trks]
   Float_t         trkresrangeSCE[3][3][3000];   //[cross_trks]
   Float_t         trkdqdxcaliSCE[3][3][3000];   //[cross_trks]
   Float_t         trkdedxcaliSCE[3][3][3000];   //[cross_trks]
   Float_t         trkresrangecaliSCE[3][3][3000];   //[cross_trks]
   Float_t         trkdqdxcali[3][3][3000];   //[cross_trks]
   Float_t         trkdedxcali[3][3][3000];   //[cross_trks]
   Float_t         trkresrangecali[3][3][3000];   //[cross_trks]
   Float_t         trkhitx[3][3][3000];   //[cross_trks]
   Float_t         trkhity[3][3][3000];   //[cross_trks]
   Float_t         trkhitz[3][3][3000];   //[cross_trks]
   Float_t         trkhitxSCE[3][3][3000];   //[cross_trks]
   Float_t         trkhitySCE[3][3][3000];   //[cross_trks]
   Float_t         trkhitzSCE[3][3][3000];   //[cross_trks]
   Float_t         trkhitxcaliSCE[3][3][3000];   //[cross_trks]
   Float_t         trkhitycaliSCE[3][3][3000];   //[cross_trks]
   Float_t         trkhitzcaliSCE[3][3][3000];   //[cross_trks]
   Float_t         trkhitxcali[3][3][3000];   //[cross_trks]
   Float_t         trkhitycali[3][3][3000];   //[cross_trks]
   Float_t         trkhitzcali[3][3][3000];   //[cross_trks]
   Float_t         trkhitEx[3][3][3000];   //[cross_trks]
   Float_t         trkhitEy[3][3][3000];   //[cross_trks]
   Float_t         trkhitEz[3][3][3000];   //[cross_trks]
   Float_t         E0[3][3][3000];   //[cross_trks]
   Float_t         EfieldCorr[3][3][3000];   //[cross_trks]
   Float_t         trajposx[3][3][3000];   //[cross_trks]
   Float_t         trajposy[3][3][3000];   //[cross_trks]
   Float_t         trajposz[3][3][3000];   //[cross_trks]
   Float_t         trkdqdx_efieldcorr[3][3][3000];   //[cross_trks]
   Float_t         trkdedx_efieldcorr[3][3][3000];   //[cross_trks]
   Float_t         trkresrange_efieldcorr[3][3][3000];   //[cross_trks]
   Float_t         trkRconstant[3][3][3000];   //[cross_trks]
   Float_t         trkRcorr[3][3][3000];   //[cross_trks]
   Float_t         trklocaltime[3][3][3000];   //[cross_trks]
   Float_t         trklocaltimeSCE[3][3][3000];   //[cross_trks]
   Float_t         trklocalDriftVel[3][3][3000];   //[cross_trks]
   Float_t         trklocalDriftVelSCE[3][3][3000];   //[cross_trks]
   Float_t         trk_hit_time[3][3][3000];   //[cross_trks]

   // List of branches
   TBranch        *b_event;   //!
   TBranch        *b_evttime;   //!
   TBranch        *b_run;   //!
   TBranch        *b_surbrun;   //!
   TBranch        *b_year_month_date;   //!
   TBranch        *b_hour_min_sec;   //!
   TBranch        *b_cross_trks;   //!
   TBranch        *b_hasCalo;   //!
   TBranch        *b_caloIsValid;   //!
   TBranch        *b_planenum_exists;   //!
   TBranch        *b_planenumber;   //!
   TBranch        *b_all_trks;   //!
   TBranch        *b_xprojectedlen;   //!
   TBranch        *b_trackthetaxz;   //!
   TBranch        *b_trackthetayz;   //!
   TBranch        *b_trackthetaxy;   //!
   TBranch        *b_trackstartpos;   //!
   TBranch        *b_trackendpos;   //!
   TBranch        *b_TrkID;   //!
   TBranch        *b_trkstartcosxyz;   //!
   TBranch        *b_trkendcosxyz;   //!
   TBranch        *b_ntrkhits;   //!
   TBranch        *b_ntrajpoints;   //!
   TBranch        *b_trkdqdx;   //!
   TBranch        *b_trkdedx;   //!
   TBranch        *b_trkresrange;   //!
   TBranch        *b_trkdqdxSCE;   //!
   TBranch        *b_trkdedxSCE;   //!
   TBranch        *b_trkresrangeSCE;   //!
   TBranch        *b_trkdqdxcaliSCE;   //!
   TBranch        *b_trkdedxcaliSCE;   //!
   TBranch        *b_trkresrangecaliSCE;   //!
   TBranch        *b_trkdqdxcali;   //!
   TBranch        *b_trkdedxcali;   //!
   TBranch        *b_trkresrangecali;   //!
   TBranch        *b_trkhitx;   //!
   TBranch        *b_trkhity;   //!
   TBranch        *b_trkhitz;   //!
   TBranch        *b_trkhitxSCE;   //!
   TBranch        *b_trkhitySCE;   //!
   TBranch        *b_trkhitzSCE;   //!
   TBranch        *b_trkhitxcaliSCE;   //!
   TBranch        *b_trkhitycaliSCE;   //!
   TBranch        *b_trkhitzcaliSCE;   //!
   TBranch        *b_trkhitxcali;   //!
   TBranch        *b_trkhitycali;   //!
   TBranch        *b_trkhitzcali;   //!
   TBranch        *b_trkhitEx;   //!
   TBranch        *b_trkhitEy;   //!
   TBranch        *b_trkhitEz;   //!
   TBranch        *b_E0;   //!
   TBranch        *b_EfieldCorr;   //!
   TBranch        *b_trajposx;   //!
   TBranch        *b_trajposy;   //!
   TBranch        *b_trajposz;   //!
   TBranch        *b_trkdqdx_efieldcorr;   //!
   TBranch        *b_trkdedx_efieldcorr;   //!
   TBranch        *b_trkresrange_efieldcorr;   //!
   TBranch        *b_trkRconstant;   //!
   TBranch        *b_trkRcorr;   //!
   TBranch        *b_trklocaltime;   //!
   TBranch        *b_trklocaltimeSCE;   //!
   TBranch        *b_trklocalDriftVel;   //!
   TBranch        *b_trklocalDriftVelSCE;   //!
   TBranch        *b_trk_hit_time;   //!

   X_validation_DataDrivenMap_plane2(TString filename, int corr, int i_year, int i_month, int i_date, int f_date, std::string sample);
   virtual ~X_validation_DataDrivenMap_plane2();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(int corr, int i_year, int i_month, int i_date, int f_date, std::string sample);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef X_validation_DataDrivenMap_plane2_cxx
X_validation_DataDrivenMap_plane2::X_validation_DataDrivenMap_plane2(TString filename, int corr, int i_year, int i_month, int i_date, int f_date, std::string sample) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   TTree *tree=0;
   TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(Form("%s.root",filename.Data()));
   if (!f || !f->IsOpen()) {
      f = new TFile(Form("%s.root",filename.Data()));
   }
   TDirectory * dir = (TDirectory*)f->Get(Form("%s.root:/XYZcorrection",filename.Data()));
   cout << Form("%s.root:XYZcorrection",filename.Data()) << endl;
   dir->GetObject("Event",tree);
   Init(tree);
   tree->GetEntry(0);

   Loop(corr, i_year, i_month, i_date, f_date, sample );
}

X_validation_DataDrivenMap_plane2::~X_validation_DataDrivenMap_plane2()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t X_validation_DataDrivenMap_plane2::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t X_validation_DataDrivenMap_plane2::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void X_validation_DataDrivenMap_plane2::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("evttime", &evttime, &b_evttime);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("subrun", &subrun, &b_surbrun);
   fChain->SetBranchAddress("year_month_date", &year_month_date, &b_year_month_date);
   fChain->SetBranchAddress("hour_min_sec", &hour_min_sec, &b_hour_min_sec);
   fChain->SetBranchAddress("cross_trks", &cross_trks, &b_cross_trks);
   fChain->SetBranchAddress("hasCalo", hasCalo, &b_hasCalo);
   fChain->SetBranchAddress("caloIsValid", caloIsValid, &b_caloIsValid);
   fChain->SetBranchAddress("planenum_exists", planenum_exists, &b_planenum_exists);
   fChain->SetBranchAddress("planenumber", &planenumber, &b_planenumber);
   fChain->SetBranchAddress("all_trks", &all_trks, &b_all_trks);
   fChain->SetBranchAddress("xprojectedlen", xprojectedlen, &b_xprojectedlen);
   fChain->SetBranchAddress("trackthetaxz", trackthetaxz, &b_trackthetaxz);
   fChain->SetBranchAddress("trackthetayz", trackthetayz, &b_trackthetayz);
   fChain->SetBranchAddress("trackthetaxy", trackthetaxy, &b_trackthetaxy);
   fChain->SetBranchAddress("trackstartpos", trackstartpos, &b_trackstartpos);
   fChain->SetBranchAddress("trackendpos", trackendpos, &b_trackendpos);
   fChain->SetBranchAddress("TrkID", TrkID, &b_TrkID);
   fChain->SetBranchAddress("trkstartcosxyz", trkstartcosxyz, &b_trkstartcosxyz);
   fChain->SetBranchAddress("trkendcosxyz", trkendcosxyz, &b_trkendcosxyz);
   fChain->SetBranchAddress("ntrkhits", ntrkhits, &b_ntrkhits);
   fChain->SetBranchAddress("ntrajpoints", ntrajpoints, &b_ntrajpoints);
   fChain->SetBranchAddress("trkdqdx", trkdqdx, &b_trkdqdx);
   fChain->SetBranchAddress("trkdedx", trkdedx, &b_trkdedx);
   fChain->SetBranchAddress("trkresrange", trkresrange, &b_trkresrange);
   fChain->SetBranchAddress("trkdqdxSCE", trkdqdxSCE, &b_trkdqdxSCE);
   fChain->SetBranchAddress("trkdedxSCE", trkdedxSCE, &b_trkdedxSCE);
   fChain->SetBranchAddress("trkhitx", trkhitx, &b_trkhitx);
   fChain->SetBranchAddress("trkhity", trkhity, &b_trkhity);
   fChain->SetBranchAddress("trkhitz", trkhitz, &b_trkhitz);
   fChain->SetBranchAddress("trkhitxSCE", trkhitxSCE, &b_trkhitxSCE);
   fChain->SetBranchAddress("trkhitySCE", trkhitySCE, &b_trkhitySCE);
   fChain->SetBranchAddress("trkhitzSCE", trkhitzSCE, &b_trkhitzSCE);
   fChain->SetBranchAddress("trkhitxSCE", trkhitxSCE, &b_trkhitxSCE);
   fChain->SetBranchAddress("trkhitySCE", trkhitySCE, &b_trkhitySCE);
   fChain->SetBranchAddress("trkhitzSCE", trkhitzSCE, &b_trkhitzSCE);
   fChain->SetBranchAddress("trkhitEx", trkhitEx, &b_trkhitEx);
   fChain->SetBranchAddress("trkhitEy", trkhitEy, &b_trkhitEy);
   fChain->SetBranchAddress("trkhitEz", trkhitEz, &b_trkhitEz);
   fChain->SetBranchAddress("E0", E0, &b_E0);
   fChain->SetBranchAddress("EfieldCorr", EfieldCorr, &b_EfieldCorr);
   fChain->SetBranchAddress("trajposx", trajposx, &b_trajposx);
   fChain->SetBranchAddress("trajposy", trajposy, &b_trajposy);
   fChain->SetBranchAddress("trajposz", trajposz, &b_trajposz);
   fChain->SetBranchAddress("trkdqdx_efieldcorr", trkdqdx_efieldcorr, &b_trkdqdx_efieldcorr);
   fChain->SetBranchAddress("trkdedx_efieldcorr", trkdedx_efieldcorr, &b_trkdedx_efieldcorr);
   fChain->SetBranchAddress("trkresrange_efieldcorr", trkresrange_efieldcorr, &b_trkresrange_efieldcorr);
   fChain->SetBranchAddress("trkRconstant", trkRconstant, &b_trkRconstant);
   fChain->SetBranchAddress("trkRcorr", trkRcorr, &b_trkRcorr);
   fChain->SetBranchAddress("trklocaltime", trklocaltime, &b_trklocaltime);
   fChain->SetBranchAddress("trklocaltimeSCE", trklocaltimeSCE, &b_trklocaltimeSCE);
   fChain->SetBranchAddress("trklocalDriftVel", trklocalDriftVel, &b_trklocalDriftVel);
   fChain->SetBranchAddress("trklocalDriftVelSCE", trklocalDriftVelSCE, &b_trklocalDriftVelSCE);
   fChain->SetBranchAddress("trk_hit_time", trk_hit_time, &b_trk_hit_time);
  
   Notify();
}

Bool_t X_validation_DataDrivenMap_plane2::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void X_validation_DataDrivenMap_plane2::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t X_validation_DataDrivenMap_plane2::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef X_validation_DataDrivenMap_plane2_cxx
