#include <memory>
#include <set>


#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/IPTools/interface/IPTools.h"


#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "RecoVertex/VertexTools/interface/VertexDistance3D.h"


#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"





//#define VTXDEBUG

class NuclearInteractionIdentifier : public edm::EDProducer {
    public:
	NuclearInteractionIdentifier(const edm::ParameterSet &params);


	virtual void produce(edm::Event &event, const edm::EventSetup &es);

    private:
	bool trackFilter(const reco::TrackRef &track) const;

	edm::InputTag				primaryVertexCollection;
	edm::InputTag				secondaryVertexCollection;
        edm::InputTag                           beamSpotCollection;
};

NuclearInteractionIdentifier::NuclearInteractionIdentifier(const edm::ParameterSet &params) :
	primaryVertexCollection      (params.getParameter<edm::InputTag>("primaryVertices")),
	secondaryVertexCollection    (params.getParameter<edm::InputTag>("secondaryVertices")),
        beamSpotCollection           (params.getParameter<edm::InputTag>("beamSpot"))
{
	produces<reco::VertexCollection>();
}


void NuclearInteractionIdentifier::produce(edm::Event &event, const edm::EventSetup &es)
{
	using namespace reco;

	edm::Handle<VertexCollection> secondaryVertices;
	event.getByLabel(secondaryVertexCollection, secondaryVertices);
        VertexCollection theSecVertexColl = *(secondaryVertices.product());

        edm::Handle<VertexCollection> primaryVertices;
        event.getByLabel(primaryVertexCollection, primaryVertices);

	std::auto_ptr<VertexCollection> recoVertices(new VertexCollection);

        if(primaryVertices->size()!=0){ 
        const reco::Vertex &pv = (*primaryVertices)[0];
    
        edm::Handle<BeamSpot> beamSpot;
        event.getByLabel(beamSpotCollection, beamSpot);

        for(unsigned int ivtx=0; ivtx < theSecVertexColl.size(); ivtx++){
	       const reco::Vertex & sv = theSecVertexColl[ivtx];	
	       float mass=sv.p4().M();
	       float pt=sv.p4().Pt();
	       float transverseBoost=pt/mass;
               GlobalPoint ppv(pv.position().x(),pv.position().y(),pv.position().z());
               GlobalPoint ssv(sv.position().x(),sv.position().y(),sv.position().z());
               GlobalVector flightDir = ssv-ppv;
	       float flightDistance2D = flightDir.perp(); 
	       unsigned int ntracks = sv.nTracks();
	       float z=sv.position().z();
	       float nctau=flightDistance2D/transverseBoost/0.05;  //number of c*taus
 	
	       if( (fabs(z) < 29 &&  flightDistance2D > 2.5 && nctau > 4)			
                   or (fabs(z) < 29 &&  flightDistance2D > 2.0 && nctau > 5)
                   or (fabs(z) < 29 &&  flightDistance2D > 2.0 && nctau > 3 && ntracks > 6)
			)
		       recoVertices->push_back(theSecVertexColl[ivtx]);

        }

        }	
	event.put(recoVertices);
	
	
	
}

DEFINE_FWK_MODULE(NuclearInteractionIdentifier);
