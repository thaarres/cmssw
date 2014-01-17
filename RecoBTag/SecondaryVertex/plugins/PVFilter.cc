// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "RecoBTag/SecondaryVertex/interface/SecondaryVertex.h"
//
// class declaration
//
#include "RecoBTag/SecondaryVertex/interface/VertexFilter.h"
class PVFilter : public edm::EDFilter {
   public:
      explicit PVFilter(const edm::ParameterSet&);
      ~PVFilter();

   private:
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      edm::InputTag                           primaryVertexCollection;
};

PVFilter::PVFilter(const edm::ParameterSet& params):
      primaryVertexCollection(params.getParameter<edm::InputTag>("primaryVertices"))
{
        produces<reco::VertexCollection>();

}

PVFilter::~PVFilter()
{
}

bool
PVFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 edm::Handle<reco::VertexCollection> pvHandle; 
 iEvent.getByLabel(primaryVertexCollection,pvHandle);

 // skips the event in case of an empty collection of PV 
 if(pvHandle->size()==0) {
   return false;
 }
 else return true;
}


//define this as a plug-in
DEFINE_FWK_MODULE(PVFilter);
