// Derived from HLTrigger/special/src/HLTPixelClusterShapeFilter.cc
// at version 7_5_0_pre3
//
//
// Author of Derived Filter:  Eric Appelt
//         Created:  Wed Apr 29, 2015
//
//


// Don't compile this by default. Comment below to build this 
// filter
#define DONT_COMPILE_THIS
#ifndef DONT_COMPILE_THIS

#include <iostream>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDGetToken.h"

#include <DataFormats/HeavyIonEvent/interface/ClusterCompatibility.h>
#include <DataFormats/HeavyIonEvent/interface/Centrality.h>


class HIClusterCompatibilityFilter : public edm::EDFilter {
  public:
    explicit HIClusterCompatibilityFilter(const edm::ParameterSet&);
    ~HIClusterCompatibilityFilter();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);



  private:
    virtual void beginJob() override;
    virtual bool filter(edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;

    edm::EDGetTokenT<reco::Centrality> centSrc_;
    edm::EDGetTokenT<reco::ClusterCompatibilityCollection> cluscomSrc_;
  
    double              minZ_;          // beginning z-vertex position
    double              maxZ_;          // end z-vertex position

    std::vector<double> clusterPars_;   //pixel cluster polynomial pars for vertex compatibility cut
    int                 nhitsTrunc_;    //maximum pixel clusters to apply compatibility check
    double              clusterTrunc_;  //maximum vertex compatibility value for event rejection
};

HIClusterCompatibilityFilter::HIClusterCompatibilityFilter(const edm::ParameterSet& iConfig):
centSrc_(consumes<reco::Centrality>(iConfig.getParameter<edm::InputTag>("centSrc"))),
cluscomSrc_(consumes<reco::ClusterCompatibilityCollection>(iConfig.getParameter<edm::InputTag>("cluscomSrc"))),
minZ_(iConfig.getParameter<double>("minZ")),
maxZ_(iConfig.getParameter<double>("maxZ")),
clusterPars_(iConfig.getParameter< std::vector<double> >("clusterPars")),
nhitsTrunc_(iConfig.getParameter<int>("nhitsTrunc")),
clusterTrunc_(iConfig.getParameter<double>("clusterTrunc"))
{}

HIClusterCompatibilityFilter::~HIClusterCompatibilityFilter() {}

bool
HIClusterCompatibilityFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  bool accept = true;

  // obtain centrality object
  Handle<reco::Centrality> cent;
  iEvent.getByToken(centSrc_, cent);
  double nPxlHits = cent->NValidPixelClusters();

  // obtain cluster compatibility scores
  Handle<reco::ClusterCompatibilityCollection> ccc;
  iEvent.getByToken(cluscomSrc_, ccc);


  // will compare cluster compatibility at a determined best 
  // z position to + and - 10 cm from the best position
  reco::ClusterCompatibility best,low,high;


  // look for best vertex z position within zMin to zMax range
  // best position is determined by maximum nHit with 
  // chi used for breaking a tie
  int nhits_max = 0;
  double chi_max = 1e+9;
  for( const auto & cc : *ccc )
  {
    if( cc.z0() > maxZ_ || cc.z0() < minZ_ ) continue;
    if(cc.nHit() == 0) continue;
    if(cc.nHit() > nhits_max) {
      chi_max = 1e+9;
      nhits_max = cc.nHit();
    }
    if(cc.nHit() >= nhits_max && cc.chi() < chi_max) {
      chi_max = cc.chi();
      best = cc;
    }
  }

  // find ClusterCompatibility objects at + or - 10 cm of the best, 
  // or get as close as possible.
  double low_target = best.z0() - 10.0;
  double high_target = best.z0() + 10.0;
  double low_match = 1000., high_match = 1000.;
  for( const auto & cc : *ccc )
  {  
    if( fabs(cc.z0()-low_target) < low_match )
    {
       low = cc;
       low_match = fabs(cc.z0()-low_target);
    }
    if( fabs(cc.z0()-high_target) < high_match )
    {
       low = cc;
       high_match = fabs(cc.z0()-high_target);
    }
  }

  // determine vertex compatibility quality score
  double clusVtxQual=0.0;
  if ((low.nHit()+high.nHit())> 0)
    clusVtxQual = (2.0*best.nHit())/(low.nHit()+high.nHit());  // A/B
  else if (best.nHit() > 0)
    clusVtxQual = 1000.0;                      // A/0 (set to arbitrarily large number)
  else
    clusVtxQual = 0;   

  // construct polynomial cut on cluster vertex quality vs. npixelhits
  double polyCut=0;
  for(unsigned int i=0; i < clusterPars_.size(); i++) {
    polyCut += clusterPars_[i]*std::pow((double)nPxlHits,(int)i);
  }
  if(nPxlHits < nhitsTrunc_)
    polyCut=0;             // don't use cut below nhitsTrunc_ pixel hits
  if(polyCut > clusterTrunc_ && clusterTrunc_ > 0)
    polyCut=clusterTrunc_; // no cut above clusterTrunc_

  if (clusVtxQual < polyCut) accept = false;

  // return with final filter decision
  return accept;

}

void
HIClusterCompatibilityFilter::beginJob()
{
}

void
HIClusterCompatibilityFilter::endJob()
{
}

void
HIClusterCompatibilityFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  // The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

DEFINE_FWK_MODULE(HIClusterCompatibilityFilter);

#endif
