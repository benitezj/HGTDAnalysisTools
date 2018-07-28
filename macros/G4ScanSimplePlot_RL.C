
void G4ScanSimplePlot_RL() {
  TString infile="/data/det-hgtd/hits/G4Scan_s3072/G4Scan/histo_0.root";

  //TFile* inFile = TFile::Open("rad_intLength.histo.root");
  TFile* inFile = TFile::Open(infile.Data(),"read");
  gDirectory->cd("radLen/");
  TProfile* h_Total_X0_RL = NULL;
  inFile->GetObject("radLen/Total_X0_RL",h_Total_X0_RL);
  THStack *h_stack = new THStack("X0StackTitle;#eta;%X0","Title");
  TLegend *leg_stack = new TLegend(0.71, 0.62, 0.87, 0.92);
  leg_stack->SetFillColor(0);
  leg_stack->SetShadowColor(0);
  int n_colour = 1;
  string searchstring = "D_";
  //string searchstring = "M_";
  //string searchstring = "E_";
  //string searchstring = "DM_Pixel_";
  //string searchstring = "DE_Pixel_";
  TKey *key;
  TObject *obj;
  TIter next(gDirectory->GetListOfKeys());
  while( (key = (TKey*)next()) ){
    string name = key->GetName();
    if(name.find(searchstring) != 0) continue;
    if ((name.find("Phi_RL") == (name.size()-6)) || (name.find("R_RL") == (name.size()-4)) || (name.find("Z_RL") == (name.size()-4))) continue;
    //if(name.find("Phi_RL") != (name.size()-6)) continue;
    //if(name.find("R_RL") != (name.size()-4)) continue;
    //if(name.find("Z_RL") != (name.size()-4)) continue;
    cout<<name<<endl;
    obj = key->ReadObj();
    TProfile* plot = (TProfile*)obj;
    TH1D* h_plot = plot->ProjectionX();
    h_plot->SetFillColor(n_colour);
    h_plot->SetLineColor(n_colour);
    h_plot->SetMarkerColor(n_colour);
    n_colour++;
    leg_stack->AddEntry( h_plot, name.c_str() );
    h_stack->Add(h_plot);
  }
  TH1D* h_temp_last = (TH1D*)h_stack->GetStack()->Last();
  h_temp_last->GetYaxis()->SetRangeUser(0,150);
  TCanvas *c_stack = new TCanvas("Name","Title",800,600);
  c_stack->SetFillColor(0);
  h_stack->Draw("hist");
  h_Total_X0_RL->Draw("same");
  leg_stack->AddEntry( h_Total_X0_RL, "Total_X0_RL" );
  leg_stack->Draw();
  c_stack->Update();

}
