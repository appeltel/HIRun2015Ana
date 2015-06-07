{

  int i = 4;

  TFile * f = new TFile("ccgallery.root");

  char dir[256] = "clusComAna";

  TGraph * ccc = (TGraph *) f->Get(Form("%s/ccc%d",dir,i));
  TGraph * vtx = (TGraph *) f->Get(Form("%s/vtxz%d",dir,i));
  TGraph * pxl = (TGraph *) f->Get(Form("%s/npxl%d",dir,i));

  TCanvas * c = new TCanvas("c","c",600,600);
  c->cd();

  Double_t max = 0;
  for (int j=0; j<ccc->GetN();j++)
  {
    Double_t x,y;
    ccc->GetPoint(j,x,y);
    if( y>max) max = y;
    if( j > 390) ccc->SetPoint(j,50,0);
  }

  TH1F * hdum = new TH1F("hdum",";z [cm];Compatible Clusters",10,-40,40);
  hdum->SetMinimum(0.0);
  hdum->SetMaximum(max > 2. ? max*1.2 : 5.0);
  hdum->Draw();

  ccc->SetLineColor(kBlue);
  ccc->Draw("l");

  Double_t zpos, npxl, foo;
  vtx->GetPoint(0,zpos,foo);
  pxl->GetPoint(0,npxl,foo);

  TLatex * tex = new TLatex();
  tex->SetTextSize(0.034);
  tex->SetNDC();
  tex->DrawLatex(0.15,0.89,Form("nPxl = %d",int(npxl)));   

  TLine *  vz = new TLine(zpos, 0.0, zpos, max > 2. ? max*1.2 : 5.0);
  vz->SetLineColor(kRed);
  vz->Draw();

  TLatex * tex2 = new TLatex();
  tex2->SetTextSize(0.034);
  tex2->SetTextColor(kRed);
  tex2->DrawLatex(zpos+3.0,max > 2. ? max*1.1 : 4.5,Form("vtx_{Z} = %4.2f",zpos));   

}
