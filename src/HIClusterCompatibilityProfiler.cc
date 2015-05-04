//
// Original Author:  Eric Appelt
//         Created:  Tue Apr 28, 2015
//
//

#include <iostream>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDGetToken.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include <DataFormats/VertexReco/interface/Vertex.h>
#include <DataFormats/VertexReco/interface/VertexFwd.h>

#include <DataFormats/HeavyIonEvent/interface/ClusterCompatibility.h>

#include "Appeltel/HIRun2015Ana/interface/HIClusterCompatibilityFilter.h"

#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TString.h"

class HIClusterCompatibilityProfiler : public edm::EDAnalyzer {
  public:
    explicit HIClusterCompatibilityProfiler(const edm::ParameterSet&);
    ~HIClusterCompatibilityProfiler();

  private:
    virtual void beginJob() ;
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    edm::EDGetTokenT<reco::VertexCollection> vertexSrc_;
    edm::EDGetTokenT<reco::ClusterCompatibility> cluscomSrc_;

    int eventCount_;

    std::map<std::string,TGraph*> graphs_;
    std::map<std::string,TH2F*> histos2d_;
  
};

HIClusterCompatibilityProfiler::HIClusterCompatibilityProfiler(const edm::ParameterSet& iConfig):
vertexSrc_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertexSrc"))),
cluscomSrc_(consumes<reco::ClusterCompatibility>(iConfig.getParameter<edm::InputTag>("cluscomSrc"))),
eventCount_(0)
{
  edm::Service<TFileService> fs;

  //just look at first 20 events
  for( int i=0; i<20; i++)
  {
    std::string vtx(Form("vtxz%d",i));
    std::string pxl(Form("npxl%d",i));
    std::string ccs(Form("ccc%d",i));
    graphs_[ccs] = fs->make<TGraph>(1000);
    graphs_[ccs]->SetName(ccs.c_str());
    graphs_[vtx] = fs->make<TGraph>(1);
    graphs_[vtx]->SetName(vtx.c_str());
    graphs_[pxl] = fs->make<TGraph>(1);
    graphs_[pxl]->SetName(pxl.c_str());
  }
  histos2d_["qual_hits"] = fs->make<TH2F>(
      "qual_hits",";# of pixel hits;Cluster-Vertex Compatibility",
      50, 0., 80000., 50, 0, 10);
   histos2d_["qual_hits_close"] = fs->make<TH2F>(
      "qual_hits_close",";# of pixel hits;Cluster-Vertex Compatibility",
      50, 0., 2000., 50, 0, 10);

}

HIClusterCompatibilityProfiler::~HIClusterCompatibilityProfiler() {}

void
HIClusterCompatibilityProfiler::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{


   using namespace edm;

  // obtain primary vertices
  Handle<reco::VertexCollection> vertexCol;
  iEvent.getByToken(vertexSrc_, vertexCol);

  // obtain cluster compatibility scores
  Handle<reco::ClusterCompatibility> cc;
  iEvent.getByToken(cluscomSrc_, cc);

  double nPxlHits = cc->nValidPixelHits();
  double clusVtxQual = HIClusterCompatibilityFilter::determineQuality(*cc,-20.,20.5);

  histos2d_["qual_hits"]->Fill(nPxlHits,clusVtxQual);
  histos2d_["qual_hits_close"]->Fill(nPxlHits,clusVtxQual);


  // Show some plots scanning z-axis for first 20 events
  if ( eventCount_ >= 20 ) return;

  std::string vtx(Form("vtxz%d",eventCount_));
  std::string pxl(Form("npxl%d",eventCount_));
  std::string ccs(Form("ccc%d",eventCount_));

  int i=0;
  for( const auto & vertex : *vertexCol)
  {
    if (i < 1) graphs_[vtx]->SetPoint(0, vertex.z(), 1);
    //std::cout << "Primary Vertex at z = " << vertex.z() << std::endl;
    i++;
  }

  graphs_[pxl]->SetPoint(0,cc->nValidPixelHits(), 1);
  for( i=0; i<cc->size(); i++ )
  {
    graphs_[ccs]->SetPoint(i, cc->z0(i),  cc->nHit(i));
    //std::cout << "  cc z0 = " << cc.z0() << " nHit = " << cc.nHit() << " chi = " << cc.chi() << std::endl;
  }

  eventCount_++;

}

void
HIClusterCompatibilityProfiler::beginJob()
{
}

void
HIClusterCompatibilityProfiler::endJob()
{
}

DEFINE_FWK_MODULE(HIClusterCompatibilityProfiler);
