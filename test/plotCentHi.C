void plotCentHi() {



  gStyle->SetOptStat(0);

  set_plot_style();

  TFile *f = new TFile("trackCorrections.root");

  // note that eta and occ cuts must match bin boundaries!

  Double_t minOcc = 0;   // 0-50% centrality  
  Double_t maxOcc = 100;

  Double_t etaMin = -0.8;
  Double_t etaMax = 0.8;

  Double_t ptMin = 0.2;

  char dir[256] = "HITrackCorrections";

  // sim-to-reco hists
  TH3F *hSim = (TH3F*) f->Get(Form("%s/hsim3D",dir)); hSim->GetYaxis()->SetRangeUser(0,5);
  TH3F *hEff = (TH3F*) f->Get(Form("%s/heff3D",dir)); hEff->GetYaxis()->SetRangeUser(0,5);
  TH3F *hMul = (TH3F*) f->Get(Form("%s/hmul3D",dir)); hMul->GetYaxis()->SetRangeUser(0,5);

  // reco-to-sim hists
  TH3F *hRec = (TH3F*) f->Get(Form("%s/hrec3D",dir)); hRec->GetYaxis()->SetRangeUser(0,5);
  TH3F *hFak = (TH3F*) f->Get(Form("%s/hfak3D",dir)); hFak->GetYaxis()->SetRangeUser(0,5);
  TH3F *hSec = (TH3F*) f->Get(Form("%s/hsec3D",dir)); hSec->GetYaxis()->SetRangeUser(0,5);

  // Projection for centrality

  hSim->GetZaxis()->SetRangeUser( minOcc, maxOcc);
  TH2F * chSim = hSim->Project3D("yxe"); 
  hEff->GetZaxis()->SetRangeUser( minOcc, maxOcc);
  TH2F * chEff = hEff->Project3D("yxe"); 
  hMul->GetZaxis()->SetRangeUser( minOcc, maxOcc);
  TH2F * chMul = hMul->Project3D("yxe"); 
  hRec->GetZaxis()->SetRangeUser( minOcc, maxOcc);
  TH2F * chRec = hRec->Project3D("yxe"); 
  hFak->GetZaxis()->SetRangeUser( minOcc, maxOcc);
  TH2F * chFak = hFak->Project3D("yxe"); 
  hSec->GetZaxis()->SetRangeUser( minOcc, maxOcc);
  TH2F * chSec = hSec->Project3D("yxe"); 

  // ratio histograms
  TH2F *rEff = (TH2F*) chEff->Clone("rEff");
  TH2F *rMul = (TH2F*) chMul->Clone("rMul");
  TH2F *rFak = (TH2F*) chFak->Clone("rFak");
  TH2F *rSec = (TH2F*) chSec->Clone("rSec");

   

  //---------------------------------------------


  // reco efficiency fraction
  TCanvas *c2 = new TCanvas("c2","Reco Efficiency Fraction",600,500);
  gPad->SetRightMargin(0.15);
  rEff->Divide(chEff,chSim,1,1,"B");
  rEff->SetStats(0);
  rEff->SetMaximum(1.0); rEff->SetMinimum(0.0);
  rEff->SetTitle("Absolute Efficiency");
  rEff->Draw("colz");

  // multiple reco fraction
//  TCanvas *c3 = new TCanvas("c3","Multiple Reco Fraction",600,500);
//  gPad->SetRightMargin(0.15);
//  rMul->Divide(chMul,chSim,1,1,"B");
//  rMul->SetStats(0);
//  rMul->SetMaximum(0.00049); rMul->SetMinimum(0.0);
//  rMul->SetTitle("Multiple Reconstruction Fraction");
//  rMul->Draw("colz");

  // fake reco fraction
  TCanvas *c4 = new TCanvas("c4","Fake Reco Fraction",600,500);
  gPad->SetRightMargin(0.15);
  rFak->Divide(chFak,chRec,1,1,"B");
  rFak->SetStats(0);
  rFak->SetMaximum(0.5); rFak->SetMinimum(0.0);
  rFak->SetTitle("Fake Reconstruction Fraction");
  rFak->Draw("colz");

  // secondary reco fraction
//  TCanvas *c5 = new TCanvas("c5","Secondary Fraction",600,500);  
//  gPad->SetRightMargin(0.15);
//  rSec->Divide(chSec,chRec,1,1,"B");
//  rSec->SetStats(0);
//  rSec->SetMaximum(0.05); rSec->SetMinimum(0.0);
//  rSec->SetTitle("Non-Primary Reconstruction Fraction");
//  rSec->Draw("colz");

  //---------------------------------------------

  // find bins corresponding to projections for below
  Int_t ptbin04=chSim->GetYaxis()->FindBin(ptMin+0.01);
  Int_t ptbins=chSim->GetYaxis()->GetNbins();
  Int_t etabin10m=chSim->GetXaxis()->FindBin(etaMin+0.01);
  Int_t etabin10p=chSim->GetXaxis()->FindBin(etaMax-0.01);

  cout << "etabin10m: " << etabin10m << " etabin10p: " << etabin10p << endl;

  // projected hists: pt 
  TH1D* hSimEta = (TH1D*) chSim->ProjectionX("hSimEta",ptbin04,ptbins,"e");
  TH1D* hEffEta = (TH1D*) chEff->ProjectionX("hEffEta",ptbin04,ptbins,"e");
  TH1D* hMulEta = (TH1D*) chMul->ProjectionX("hMulEta",ptbin04,ptbins,"e");
  TH1D* hRecEta = (TH1D*) chRec->ProjectionX("hRecEta",ptbin04,ptbins,"e");
  TH1D* hFakEta = (TH1D*) chFak->ProjectionX("hFakEta",ptbin04,ptbins,"e");
  TH1D* hSecEta = (TH1D*) chSec->ProjectionX("hSecEta",ptbin04,ptbins,"e");


  TH1D* hDumEta = new TH1D("hDumEta",";#eta",60,-2.4,2.4); hDumEta->SetMaximum(1.0);
  hDumEta->GetXaxis()->CenterTitle(); hDumEta->GetYaxis()->SetTitleOffset(1.8);
  TH1D* hDumEta2 = (TH1D*) hDumEta->Clone("hDumEta2"); hDumEta2->SetMaximum(0.5); 
  TH1D* hDumEta3 = (TH1D*) hDumEta->Clone("hDumEta3"); hDumEta3->SetMaximum(0.00049); 


  // projected hists: abs(eta) 
  TH1D* hSimPt  = (TH1D*) chSim->ProjectionY("hSimPt",etabin10m,etabin10p,"e");
  TH1D* hEffPt  = (TH1D*) chEff->ProjectionY("hEffPt",etabin10m,etabin10p,"e");
  TH1D* hMulPt  = (TH1D*) chMul->ProjectionY("hMulPt",etabin10m,etabin10p,"e");
  TH1D* hRecPt  = (TH1D*) chRec->ProjectionY("hRecPt",etabin10m,etabin10p,"e");
  TH1D* hFakPt  = (TH1D*) chFak->ProjectionY("hFakPt",etabin10m,etabin10p,"e");
  TH1D* hSecPt  = (TH1D*) chSec->ProjectionY("hSecPt",etabin10m,etabin10p,"e");

  
  TH1D* hDumPt = new TH1D("hDumPt",";p_{T} [GeV/c]",40,0.0,5.0); hDumPt->SetMaximum(1.0);
  hDumPt->GetXaxis()->CenterTitle(); hDumPt->GetYaxis()->SetTitleOffset(1.8);
  TH1D* hDumPt2 = (TH1D*) hDumPt->Clone("hDumPt2"); hDumPt2->SetMaximum(0.5); 
  TH1D* hDumPt3 = (TH1D*) hDumPt->Clone("hDumPt3"); hDumPt3->SetMaximum(0.00049); 


  // Efficiency
  TGraphAsymmErrors *gEffEta = new TGraphAsymmErrors(); gEffEta->SetName("gEffEta");
  gEffEta->BayesDivide(hEffEta,hSimEta);
  gEffEta->SetMarkerStyle(20);
  gEffEta->SetLineStyle(2);
  gEffEta->SetLineColor(kBlue);
  gEffEta->SetMarkerColor(kBlue);

  TGraphAsymmErrors *gEffPt = new TGraphAsymmErrors(); gEffPt->SetName("gEffPt");
  gEffPt->BayesDivide(hEffPt,hSimPt);
  gEffPt->SetMarkerStyle(20);
  gEffPt->SetLineColor(kBlue);
  gEffPt->SetMarkerColor(kBlue);


  TLegend *legEta = new TLegend(0.35,0.3,0.65,0.5);
  legEta->SetFillColor(0); legEta->SetBorderSize(0);
  legEta->AddEntry(gEffEta,Form("p_{T} > %4.2f GeV/c",ptMin),"lp");

  TLegend *legPt = new TLegend(0.4,0.3,0.6,0.5);
  legPt->SetFillColor(0); legPt->SetBorderSize(0);
  legPt->AddEntry(gEffPt,Form("%4.2f < #eta < %4.2f",etaMin, etaMax),"lp");

  TCanvas *c7 = new TCanvas("c7","Efficiency Fraction",900,500);
  c7->Divide(2,1);
  hDumEtaEff=(TH1F*) hDumEta->Clone("hDumEtaEff");
  hDumEtaEff->GetYaxis()->SetTitle("Absolute efficiency");
  hDumPtEff=(TH1F*) hDumPt->Clone("hDumPtEff");
  hDumPtEff->GetYaxis()->SetTitle("Absolute efficiency");
  hDumPtEff->SetTitle(Form("Absolute Efficiency %4.2f < #eta < %4.2f",etaMin,etaMax));
  hDumEtaEff->SetTitle(Form("Absolute Efficiency p_{T} > %4.2f GeV/c",ptMin));
  c7->cd(1); hDumEtaEff->Draw(); gEffEta->Draw("p");//  legEta->Draw();
  c7->cd(2); hDumPtEff->Draw(); gEffPt->Draw("p"); // legPt->Draw();



  // Fakes
  TGraphAsymmErrors *gFakEta = new TGraphAsymmErrors();  gFakEta->SetName("gFakEta");
  gFakEta->BayesDivide(hFakEta,hRecEta);
  gFakEta->SetMarkerStyle(20);
  gFakEta->SetLineStyle(2);
  gFakEta->SetLineColor(kBlue);
  gFakEta->SetMarkerColor(kBlue);

  TGraphAsymmErrors *gFakPt = new TGraphAsymmErrors(); gFakPt->SetName("gFakPt");
  gFakPt->BayesDivide(hFakPt,hRecPt);
  gFakPt->SetMarkerStyle(20);
  gFakPt->SetLineColor(kBlue);
  gFakPt->SetMarkerColor(kBlue);


  TCanvas *c9 = new TCanvas("c9","Fake Fraction",900,500);
  c9->Divide(2,1);
  hDumEtaFak=(TH1F*) hDumEta2->Clone("hDumEtaMul");
  hDumEtaFak->GetYaxis()->SetTitle("Fake Reconstruction Fraction");
  hDumPtFak=(TH1F*) hDumPt2->Clone("hDumPtMul");
  hDumPtFak->GetYaxis()->SetTitle("Fake Reconstruction Fraction");
  hDumPtFak->SetTitle(Form("Fake Rate %4.2f < #eta < %4.2f",etaMin,etaMax));
  hDumEtaFak->SetTitle(Form("Fake Rate p_{T} > %4.2f GeV/c",ptMin));
  c9->cd(1); hDumEtaFak->Draw(); gFakEta->Draw("p"); // legEta->Draw();
  c9->cd(2); hDumPtFak->Draw(); gFakPt->Draw("p");  //legPt->Draw();





  /*
  TFile *f = new TFile("trkEffPY8.root","RECREATE");
  gAccPt->Write(); gAccPt2->Write(); gAccEta->Write(); gAccEta2->Write();
  gEffPt->Write(); gEffPt2->Write(); gEffEta->Write(); gEffEta2->Write();
  gMulPt->Write(); gMulPt2->Write(); gMulEta->Write(); gMulEta2->Write();
  gFakPt->Write(); gFakPt2->Write(); gFakEta->Write(); gFakEta2->Write();
  gSecPt->Write(); gSecPt2->Write(); gSecEta->Write(); gSecEta2->Write();
  f->Close();
  */
}


void set_plot_style() {

  // nicer colz plots

  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);

}

