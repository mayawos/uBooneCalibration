#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>


#ifdef WIN32
  #define unsigned_long_long unsigned __int256
  #define long_long __int256
#else // gcc. Might not work on other compilers!
  #define unsigned_long_long unsigned long long int
  #define long_long long long int
#endif

void CloneAndFilterEvents(std::string indir, int i_month, int f_month){

  //============= original tuples ======================================= 
  TString input = Form("/uboone/data/users/${USER}/%s",indir.c_str());
  TFile *oldfile = new TFile(input,"READ");
  TTree *oldtree = (TTree*)oldfile->Get("XYZcorrection/Event");

  int year_month_date;
  TBranch *b_year_month_date;
  int run;
  TBranch *b_run;
  int subrun;
  TBranch *b_subrun;
  int event;
  TBranch *b_event;

  oldtree->SetBranchAddress("run", &run, &b_run);
  oldtree->SetBranchAddress("subrun", &subrun, &b_subrun);
  oldtree->SetBranchAddress("event", &event, &b_event);
  oldtree->SetBranchAddress("year_month_date", &year_month_date, &b_year_month_date);

  //============= XYZcorrection tuples =======================================
  std::vector<int> months;
  for(int i = i_month; i <= f_month; i++ ) months.push_back(i);
  std::vector<int>::iterator it;
  std::vector<TFile*> newfiles;
  std::vector<TTree*> newtrees;
  for(int m = 0; m < months.size(); m++ ){
    TFile *newfile = new TFile(Form("/uboone/data/users/${USER}/uBooNECalibration/XYZmapsRootfiles/Run4CalibrationT0Corr_April_K/slimmedXYZcorrection_month%d/XYZcorrection.root",months[m]),"RECREATE");
    TDirectory *dir = gDirectory->mkdir("XYZcorrection");
    dir->cd();
    TTree *newtree = (TTree*)oldtree->CloneTree(0);
    newfiles.push_back(newfile);
    newtrees.push_back(newtree);
  }
  
  std::vector<long_long> evts;
  // loop over entry
  for(int i=0; i<oldtree->GetEntries(); i++){
    oldtree->GetEntry(i);
    if(i%10000==0) cout << i << "/" << oldtree->GetEntries() << endl;
    long_long evt = (run*1e9)+(subrun*1e5)+event;
    //bool evt_good = (std::find( good_evt_vec.begin(), good_evt_vec.end(), evt) != good_evt_vec.end())? true : false;
    //bool evt_good = true;
    bool duplicated_event = (std::find( evts.begin(), evts.end(), evt) != evts.end())? true : false;
    if(!duplicated_event) evts.push_back(evt);
    if(duplicated_event) std::cout << "found duplicated events " << evt << std::endl;
    int month = (year_month_date%10000)/100;
    it=find(months.begin(),months.end(),month);
    int pos = distance(months.begin(), it);
    

    if(i%10000==0) cout << "month: " << month << endl;
    if(it!=months.end()&&!duplicated_event)
      newtrees[pos]->Fill();
  }  //end loop

  for(int m = 0; m < months.size(); m++){
    newfiles[m]->Write();
    delete newfiles[m];
  }
  delete oldfile;
}
