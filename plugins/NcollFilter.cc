// -*- C++ -*-
//
// Package:    appeltel/NcollFilter
// Class:      NcollFilter
// 
/**\class NcollFilter NcollFilter.cc appeltel/NcollFilter/plugins/NcollFilter.cc

 Description: Filters MC HI Events based on Ncoll

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Eric Appelt
//         Created:  Sun, 05 Oct 2014 18:00:07 GMT
//
//


// system include files
#include <memory>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "CLHEP/Random/RandomEngine.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "HepMC/HeavyIon.h"

//
// class declaration
//

class NcollFilter : public edm::EDFilter {
   public:
      explicit NcollFilter(const edm::ParameterSet&);
      ~NcollFilter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      
      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
      std::vector<std::string> hepmcSrc_;
      int ncollmax_;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
NcollFilter::NcollFilter(const edm::ParameterSet& iConfig):
hepmcSrc_(iConfig.getParameter<std::vector<std::string> >("generators")),
ncollmax_(iConfig.getParameter<int>("ncollmax"))
{
  edm::Service<edm::RandomNumberGenerator> rng;
  if(!rng.isAvailable()) {
    throw cms::Exception("Configuration")
      << "NcollFilter requires the RandomNumberGeneratorService,\n"
         "which is not present in the configuration file. You must add\n"
         "the service in the configuration file or remove the modules that\n"
         "require it.\n";
  }


}


NcollFilter::~NcollFilter()
{
 
}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
NcollFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   int ncoll = 0;

   edm::Service<edm::RandomNumberGenerator> rng;
   CLHEP::HepRandomEngine& engine = rng->getEngine(iEvent.streamID());
   double threshold = engine.flat() * (double)ncollmax_;

   for(size_t ihep = 0; ihep < hepmcSrc_.size(); ++ihep)
   {
     edm::Handle<edm::HepMCProduct> hepmc;
     iEvent.getByLabel(hepmcSrc_[ihep],hepmc);
     const HepMC::HeavyIon* hi = hepmc->GetEvent()->heavy_ion();
     ncoll += hi->Ncoll();
   } 

   //std::cout << "Ncoll = " << ncoll << " threshold = " << threshold << std::endl;

   return ncoll > threshold ? true : false ;
}

// ------------ method called once each job just before starting event loop  ------------
void 
NcollFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
NcollFilter::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
NcollFilter::beginRun(edm::Run const&, edm::EventSetup const&)
{ 
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
NcollFilter::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
NcollFilter::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
NcollFilter::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
NcollFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(NcollFilter);
