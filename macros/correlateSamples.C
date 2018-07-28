#include "HGTDAnalysisTools/macros/globals.h"

void correlateSamples(){

  TString sample = "mc15_13TeV.301399.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_ZZ4nu.evgen.EVNT.e4397";

  TString SAMPLE1="LArHitDec13IDR/LArHit_July26_mu200";
  loadChain(INDIR+"/"+SAMPLE1+"/"+sample);
  TTree* T1=TREE;
  if(T1->GetEntries()<=0) return;

  int truth1_VBFJ1;
  int truth1_VBFJ2;
  T1->SetBranchAddress("truth_VBFJ1",&truth1_VBFJ1);
  T1->SetBranchAddress("truth_VBFJ2",&truth1_VBFJ2);

  int njet1;
  float jet1_pt[100];
  float jet1_eta[100];
  float jet1_phi[100];

  T1->SetBranchAddress("njet",&njet1);
  T1->SetBranchAddress("jet_pt",jet1_pt);
  T1->SetBranchAddress("jet_eta",jet1_eta);
  T1->SetBranchAddress("jet_phi",jet1_phi);

  int jet1_truthJ[100];
  float jet1_truth_pt[100];
  float jet1_truth_eta[100];
  float jet1_truth_phi[100];

  T1->SetBranchAddress("jet_truthJ",jet1_truthJ);
  T1->SetBranchAddress("jet_truth_pt",jet1_truth_pt);
  T1->SetBranchAddress("jet_truth_eta",jet1_truth_eta);
  T1->SetBranchAddress("jet_truth_phi",jet1_truth_phi);

  int nclus1;
  int clus1_jet[200];
  float clus1_E[200];
  float clus1_eta[200];
  float clus1_phi[200];

  T1->SetBranchAddress("nclus",&nclus1);
  T1->SetBranchAddress("clus_E",clus1_E);
  T1->SetBranchAddress("clus_eta",clus1_eta);
  T1->SetBranchAddress("clus_phi",clus1_phi);
  T1->SetBranchAddress("clus_jet",clus1_jet);


  TString SAMPLE2="LArHitDec13IDR/LArHit_July26_mu0";
  loadChain(INDIR+"/"+SAMPLE2+"/"+sample);
  TTree* T2=TREE;
  if(T2->GetEntries()<=0) return;

  int truth2_VBFJ1;
  int truth2_VBFJ2;
  T2->SetBranchAddress("truth_VBFJ1",&truth2_VBFJ1);
  T2->SetBranchAddress("truth_VBFJ2",&truth2_VBFJ2);

  int njet2;
  float jet2_pt[100];
  float jet2_eta[100];
  float jet2_phi[100];

  T2->SetBranchAddress("njet",&njet2);
  T2->SetBranchAddress("jet_pt",jet2_pt);
  T2->SetBranchAddress("jet_eta",jet2_eta);
  T2->SetBranchAddress("jet_phi",jet2_phi);

  int jet2_truthJ[100];
  float jet2_truth_pt[100];
  float jet2_truth_eta[100];
  float jet2_truth_phi[100];

  T2->SetBranchAddress("jet_truthJ",jet2_truthJ);
  T2->SetBranchAddress("jet_truth_pt",jet2_truth_pt);
  T2->SetBranchAddress("jet_truth_eta",jet2_truth_eta);
  T2->SetBranchAddress("jet_truth_phi",jet2_truth_phi);

  int nclus2;
  int clus2_jet[200];
  float clus2_E[200];
  float clus2_eta[200];
  float clus2_phi[200];

  T2->SetBranchAddress("nclus",&nclus2);
  T2->SetBranchAddress("clus_E",clus2_E);
  T2->SetBranchAddress("clus_eta",clus2_eta);
  T2->SetBranchAddress("clus_phi",clus2_phi);
  T2->SetBranchAddress("clus_jet",clus2_jet);


  ///create output tree
  float pt;
  float eta;
  float drj1;
  float drj2;
  float drj12;
  float drcl;
  float ptcl;
  float drcsl;
  float ptcsl;
  float drcsln;
  float ptcsln;

  TTree T("tuple","tuple");
  T.Branch("pt",&pt,"pt/F");
  T.Branch("eta",&eta,"eta/F");
  T.Branch("drj1",&drj1,"drj1/F");
  T.Branch("drj2",&drj2,"drj2/F");
  T.Branch("drj12",&drj12,"drj12/F");
  T.Branch("drcl",&drcl,"drcl/F");
  T.Branch("ptcl",&ptcl,"ptcl/F");
  T.Branch("drcsl",&drcsl,"drcsl/F");
  T.Branch("ptcsl",&ptcsl,"ptcsl/F");
  T.Branch("drcsln",&drcsln,"drcsln/F");
  T.Branch("ptcsln",&ptcsln,"ptcsln/F");


  for(int e1=1;e1<T1->GetEntries();e1++){
    if(e1%10==0) cout<<"Event "<<e1<<endl;
    ///if(e1>10) break;
    T1->GetEntry(e1);
    
    ////find the truth jet
    int trueJ1=-1;
    for(int j1=0;j1<njet1;j1++){
      if(jet1_truthJ[j1]==truth1_VBFJ1 || jet1_truthJ[j1]==truth1_VBFJ2){
	trueJ1=j1; 
	break;
      }
    }
    if(trueJ1==-1) continue;


    //find leading cluster
    if(nclus1<2) continue;
    int lClus1=-1;
    for(int c1=0;c1<nclus1;c1++){
      if(clus1_jet[c1]!=trueJ1)continue;
      if(lClus1==-1)lClus1=c1;
      else if(clus1_E[c1]>clus1_E[lClus1]) lClus1=c1;
    }

    //sub leading cluster
    int slClus1=-1;
    for(int c1=0;c1<nclus1;c1++){
      if(clus1_jet[c1]!=trueJ1 || clus1_E[c1]>=clus1_E[lClus1])continue;
      if(slClus1==-1)slClus1=c1;
      else if(clus1_E[c1]>clus1_E[slClus1]) slClus1=c1;
    }



    ///find the corresponding jet in mu=0
    for(int e2=1;e2<T2->GetEntries();e2++){
      T2->GetEntry(e2);
      
      for(int j2=0;j2<njet2;j2++){
	if((jet2_truthJ[j2]==truth1_VBFJ1 || jet2_truthJ[j2]==truth2_VBFJ2)
	   && jet1_truth_pt[trueJ1]==jet2_truth_pt[j2]
	   && nclus2>=2){


	  ////find leading cluster
	  int lClus2=-1;
	  for(int c2=0;c2<nclus2;c2++){
	    if(clus2_jet[c2]!=j2)continue;
	    if(lClus2==-1)lClus2=c2;
	    else if(clus2_E[c2]>clus2_E[lClus2]) lClus2=c2;
	  }

	  //sub leading cluster
	  int slClus2=-1;
	  for(int c2=0;c2<nclus2;c2++){
	    if(clus2_jet[c2]!=j2 || clus2_E[c2]>=clus2_E[lClus2])continue;
	    if(slClus2==-1)slClus2=c2;
	    else if(clus2_E[c2]>clus2_E[slClus2]) slClus2=c2;
	  }

	  //nearest mu=200 cluster to sub-leading mu=0 cluster
	  int slClus2nearest=-1;
	  for(int c1=0;c1<nclus1;c1++){
	    if(clus1_jet[c1]!=trueJ1) continue;
	    if(slClus2nearest==-1) slClus2nearest=c1;
	    else if(deltaR(clus2_eta[slClus2],clus2_phi[slClus2],clus1_eta[c1],clus1_phi[c1])<
		    deltaR(clus2_eta[slClus2],clus2_phi[slClus2],clus1_eta[slClus2nearest],clus1_phi[slClus2nearest])) 
	      slClus2nearest=c1;
	  }
	  

	  
	  // cout<<" ("<<jet1_truth_pt[trueJ1]<<","<<jet1_truth_eta[trueJ1]<<","<<jet1_truth_phi[trueJ1]<<")"
	  //     <<" ("<<jet1_pt[trueJ1]<<","<<jet1_eta[trueJ1]<<","<<jet1_phi[trueJ1]<<")"
	  //     <<" ("<<jet2_pt[j2]<<","<<jet2_eta[j2]<<","<<jet2_phi[j2]<<")"
	  //     <<" ("<<clus1_E[lClus1]<<","<<clus1_eta[lClus1]<<","<<clus1_phi[lClus1]<<")"
	  //     <<" ("<<clus2_E[lClus2]<<","<<clus2_eta[lClus2]<<","<<clus2_phi[lClus2]<<")"
	  //     <<" ("<<clus1_E[slClus1]<<","<<clus1_eta[slClus1]<<","<<clus1_phi[slClus1]<<")"
	  //     <<" ("<<clus2_E[slClus2]<<","<<clus2_eta[slClus2]<<","<<clus2_phi[slClus2]<<")"
	  //     <<endl;


	  ////fill the output tree
	  pt=jet1_truth_pt[trueJ1];
	  eta=jet1_truth_eta[trueJ1];
	  drj1=deltaR(jet1_truth_eta[trueJ1],jet1_truth_phi[trueJ1],jet1_eta[trueJ1],jet1_phi[trueJ1]);	  
	  drj2=deltaR(jet1_truth_eta[trueJ1],jet1_truth_phi[trueJ1],jet2_eta[j2],jet2_phi[j2]);
	  drj12=deltaR(jet1_eta[trueJ1],jet1_phi[trueJ1],jet2_eta[j2],jet2_phi[j2]);	  	  
	  drcl=deltaR(clus1_eta[lClus1],clus1_phi[lClus1],clus2_eta[lClus2],clus2_phi[lClus2]);
	  ptcl=clus1_E[lClus1]/clus2_E[lClus2];
	  drcsl=deltaR(clus1_eta[slClus1],clus1_phi[slClus1],clus2_eta[slClus2],clus2_phi[slClus2]);
	  ptcsl=clus1_E[slClus1]/clus2_E[slClus2];
	  drcsln=deltaR(clus1_eta[slClus2nearest],clus1_phi[slClus2nearest],clus2_eta[slClus2],clus2_phi[slClus2]);
	  ptcsln=clus1_E[slClus2nearest]/clus2_E[slClus2];
	  T.Fill();
	}
      }
      

      
    }//loop over second sample
    
 
    
  }//loop over first sample
  
  

  TFile F("tuple.root","recreate");
  T.Write();
  F.ls();

}
