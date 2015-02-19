#include "Appeltel/HIRun2015Ana/interface/HITrackCorrectionTreeHelper.h"

HITrackCorrectionTreeHelper::HITrackCorrectionTreeHelper()
{
  hiTrackLeafString = "ids/I:status/I:charges/I:pts/F:etas/F:phis/F:hits/I";
  hiTrackLeafString = ":nmatch/I";
  hiTrackLeafString += ":charger/I:ptr/F:etar/F:phir/F:pterr/F:etaerr/F:phierr/F";
  hiTrackLeafString += ":d0/F:d0err/F:dz/F:dzerr/F:hitr/I:chi2r/F:algo/I";
  hiTrackLeafString += ":cbin/I";
}

void
HITrackCorrectionTreeHelper::Set(const reco::Track & t, const TrackingParticle & p, const reco::Vertex & v, int matches)
{

  b.ids = p.pdgId();
  b.status = p.status();  
  b.charges = p.charge();
  b.pts = p.pt();
  b.etas = p.eta();
  b.phis = p.phi();
  b.hits = p.matchedHit();
  
  b.nmatch = matches;

  b.charger = t.charge();
  b.ptr = t.pt();
  b.etar = t.eta();
  b.phir = t.phi();
  b.pterr = t.ptError();
  b.etaerr = t.etaError();
  b.phierr = t.phiError();

  math::XYZPoint vtxPoint(0.0,0.0,0.0);
  double vzErr =0.0, vxErr=0.0, vyErr=0.0;
  vtxPoint=v.position();
  vzErr=v.zError();
  vxErr=v.xError();
  vyErr=v.yError();

  double dxy=0.0, dz=0.0, dxysigma=0.0, dzsigma=0.0;
  dxy = t.dxy(vtxPoint);
  dz = t.dz(vtxPoint);
  dxysigma = sqrt(t.d0Error()*t.d0Error()+vxErr*vyErr);
  dzsigma = sqrt(t.dzError()*t.dzError()+vzErr*vzErr); 

  b.d0 = dxy;
  b.d0err = dxy/dxysigma;
  b.dz = dz;
  b.dzerr = dz/dzsigma;

  b.hitr = t.numberOfValidHits();
  b.chi2r = t.normalizedChi2();
  b.algo = t.algo();

  // centrality defaults to zero 
  b.cbin = 0;
}

void
HITrackCorrectionTreeHelper::Set(const reco::Track & t, const TrackingParticle & p, const reco::Vertex & v, int matches, int cbin)
{
  this->Set(t,p,v,matches);
  b.cbin = cbin;
}
