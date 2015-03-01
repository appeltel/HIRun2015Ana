#ifndef HIRun2015Ana_HITrackCorrectionTreeHelper
#define HIRun2015Ana_HITrackCorrectionTreeHelper

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"
#include <DataFormats/VertexReco/interface/Vertex.h>
#include <DataFormats/VertexReco/interface/VertexFwd.h>

#include <TString.h>


// Branches for track correction analysis ntuples

// Simulated Particle (one) -> Reco Track (many)
// or
// Reco Track (one) -> Simulated Particle (many)
//
// only best match is stored

typedef struct
{
  // sim particle PDG id, and status (i.e. -99 is secondary)
  Int_t ids;
  Int_t status;

  // simulated particle kinematics
  Int_t charges;
  Float_t pts;
  Float_t etas;
  Float_t phis;

  // G4SimHits of simulated particle
  Int_t hits;

  // number of matched tracks or particles
  Int_t nmatch;

  // kinematics of the reco track
  Int_t charger;
  Float_t ptr;
  Float_t etar;
  Float_t phir;

  Float_t pterr;
  Float_t etaerr;
  Float_t phierr;

  // vertex compatibility of reco track
  Float_t d0;
  Float_t d0err;
  Float_t dz;
  Float_t dzerr;

  // reconstructed hits of reco track and goodness of fit
  // and highPurity bit
  Int_t hitr;
  Float_t chi2r;
  Bool_t highPurityr;

  // Iteration of reco track
  Int_t algo;

  // Event centrality bin
  Int_t cbin;

} HITrackBranches_t;


// Helper class for producing track correction ntuples

class HITrackCorrectionTreeHelper 
{
  public:

    HITrackCorrectionTreeHelper();

    void Set(const TrackingParticle &, const reco::Track &, const reco::Vertex &, int, int );

    // need polymorphic Set functions for unmatched case where there is no Track or TP
    void Set(const reco::Track &, const reco::Vertex &, int);
    void Set(const TrackingParticle &, int);

    HITrackBranches_t b;
    TString hiTrackLeafString;
 
  private:
    // these are just to factorize setting the Track or TP information
    void SetTrack(const reco::Track &, const reco::Vertex &);
    void SetTP(const TrackingParticle &);  

};

#endif 
