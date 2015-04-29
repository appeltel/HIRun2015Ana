//
// Original Author:  Eric Appelt
//         Created:  Tue Apr 28, 2015
//
//


// Don't compile this by default. Comment below to build this 
// analyzer
#define DONT_COMPILE_THIS
#ifndef DONT_COMPILE_THIS

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

#include "TGraph.h"
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

    std::map<std::string,TGraph*> histos_;
  
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
    std::string ccs(Form("ccc%d",i));
    histos_[ccs] = fs->make<TGraph>(1000);
    histos_[ccs]->SetName(ccs.c_str());
    histos_[vtx] = fs->make<TGraph>(1);
    histos_[vtx]->SetName(vtx.c_str());
  }

}

HIClusterCompatibilityProfiler::~HIClusterCompatibilityProfiler() {}

void
HIClusterCompatibilityProfiler::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   if ( eventCount_ >= 20 ) return;

   using namespace edm;

  // obtain primary vertices
  Handle<reco::VertexCollection> vertexCol;
  iEvent.getByToken(vertexSrc_, vertexCol);

  // obtain cluster compatibility scores
  Handle<reco::ClusterCompatibility> cc;
  iEvent.getByToken(cluscomSrc_, cc);

  std::string vtx(Form("vtxz%d",eventCount_));
  std::string ccs(Form("ccc%d",eventCount_));


  int i=0;
  for( const auto & vertex : *vertexCol)
  {
    if (i < 1) histos_[vtx]->SetPoint(0, vertex.z(), 1);
    //std::cout << "Primary Vertex at z = " << vertex.z() << std::endl;
    i++;
  }

  i=0;
  for( int i=0; i<cc->size(); i++ )
  {
    histos_[ccs]->SetPoint(i, cc->z0(i),  cc->nHit(i));
    //std::cout << "  cc z0 = " << cc.z0() << " nHit = " << cc.nHit() << " chi = " << cc.chi() << std::endl;
    i++;
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

#endif
