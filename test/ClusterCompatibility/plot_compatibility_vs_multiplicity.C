{

  gStyle->SetPadBottomMargin(0.13);                                                               
  gStyle->SetPadTopMargin(0.05);                                                                  
  gStyle->SetPadRightMargin(0.13);                                                                
  gStyle->SetPadLeftMargin(0.11);

  TFile * f = new TFile("ccgallery.root");

  char dir[256] = "clusComAna";

  TH2F * qc = (TH2F*) f->Get(Form("%s/qual_hits",dir));

  TCanvas * c = new TCanvas("c","c",600,600);
  c->cd();
  c->SetLogz();

  qc->GetXaxis()->SetNoExponent();
  qc->GetXaxis()->SetRangeUser(0,60000.);
  qc->GetYaxis()->SetTitleOffset(0.9);
  qc->Draw("colz");

  TLine * l1 = new TLine(150,0,150,0.675);
  TLine * l2 = new TLine(150,0.675,444.45,2);
  TLine * l3 = new TLine(444.45,2,60000,2);
  l1->SetLineColor(kRed);
  l2->SetLineColor(kRed);
  l3->SetLineColor(kRed);
  l1->Draw();
  l2->Draw();
  l3->Draw();

} 
