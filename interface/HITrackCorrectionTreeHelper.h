#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"
#include <DataFormats/VertexReco/interface/Vertex.h>
#include <DataFormats/VertexReco/interface/VertexFwd.h>

#include <TString.h>


// Branches for track correction analysis ntuples

// Simulated Particle (one) -> Reco Track (many)
// only best match is stored

typedef struct
{
  // PDG id, and status (i.e. -99 is secondary)
  Int_t ids;
  Int_t status;

  // kinematics
  Int_t charges;
  Float_t etas;
  Float_t pts;
  Float_t phis;

  // G4SimHits of simulated particle
  Int_t hits;

  // number of matched tracks
  Int_t nrec;

  // kinematics of the matched track
  Int_t charger;
  Float_t ptr;
  Float_t etar;
  Float_t phir;

  Float_t pterr;
  Float_t etaerr;
  Float_t phierr;

  // vertex compatibility of matched track
  Float_t dz;
  Float_t d0;
  Float_t d0err;
  Float_t dzerr;

  // reconstructed hits of matched track and goodness of fit
  Int_t hitr;
  Float_t chi2r;

  // Iteration of matched track
  Int_t algo;

  // Event centrality bin
  Int_t cbin;

} HISimTrackBranches_t;

// Reco track (one) -> simulated particle (many)
// only best match is stored

typedef struct
{

  // reco track kinematics
  Int_t charger;
  Float_t etar;
  Float_t ptr;
  Float_t phir;
  Float_t pterr;
  Float_t etaerr;
  Float_t phierr;

  // vertex compatibility
  Float_t dz;
  Float_t d0;
  Float_t d0err;
  Float_t dzerr;

  // number of hits and chi2
  Int_t hitr; 
  Float_t chi2r;

  // iteration
  Int_t algo;

  // number of matched simulated particles
  Int_t nsim;

  // status and pdgid of matched particle
  Int_t status;
  Int_t ids;

  // kinematics of matched particle
  Float_t etas;
  Float_t pts;
  Float_t phis;

  // centrality bin of event
  Int_t cbin;

} HIRecTrackBranches_t;


// Helper class for producing track correction ntuples

class HITrackCorrectionTreeHelper 
{
  public:

    HIRecTrackBranches_t r;
    HISimTrackBranches_t s;    

    TString rLeafString;
    TString sLeafString;
  
    void SetRec(const reco::Track &, const reco::TrackingParticle &, const reco::Vertex &);
    void SetRec(const reco::Track &, const reco::TrackingParticle &, const reco::Vertex &, double );

    void SetSim(const reco::Track &, const reco::TrackingParticle &, const reco::Vertex &);
    void SetSim(const reco::Track &, const reco::TrackingParticle &, const reco::Vertex &, double );
}; 
