#include "RecoVertex/AdaptiveVertexFinder/interface/TrackVertexArbitratration.h"
#include "DataFormats/GeometryVector/interface/VectorUtil.h"
//#define VTXDEBUG 1
using namespace reco;

TrackVertexArbitration::TrackVertexArbitration(const edm::ParameterSet &params) :
	primaryVertexCollection   (params.getParameter<edm::InputTag>("primaryVertices")),
	secondaryVertexCollection (params.getParameter<edm::InputTag>("secondaryVertices")),
	trackCollection           (params.getParameter<edm::InputTag>("tracks")),
        beamSpotCollection        (params.getParameter<edm::InputTag>("beamSpot")),
	dRCut                     (params.getParameter<double>("dRCut")),
	distCut                   (params.getParameter<double>("distCut")),
	sigCut                    (params.getParameter<double>("sigCut")),
	dLenFraction              (params.getParameter<double>("dLenFraction")),
        maxPixelResidual(params.getParameter<double>("maxPixelResidual")),
        minDRCut(params.getParameter<double>("minDRCut"))

{
	
}

bool TrackVertexArbitration::trackFilterArbitrator(const reco::TrackRef &track) const
{
        if (track->hitPattern().trackerLayersWithMeasurement() < 4)
                return false;
        if (track->pt() < 0.4 )
                return false;
        if (track->hitPattern().numberOfValidPixelHits() < 1)
                return false;
	if (maxPixelResidual>0 && fabs(track->residualX(0)) >=maxPixelResidual) 
		return false;
	if (maxPixelResidual > 0 && fabs(track->residualY(0)) >=maxPixelResidual) 
		return false;
        return true;
}





reco::VertexCollection  TrackVertexArbitration::trackVertexArbitrator(
         edm::Handle<BeamSpot> &beamSpot, 
	 const reco::Vertex &pv,
	 edm::ESHandle<TransientTrackBuilder> &trackBuilder,
	 const edm::RefVector< TrackCollection > & selectedTracks,
	 reco::VertexCollection & secondaryVertices)
{
	using namespace reco;

	     //std::cout << "PV: " << pv.position() << std::endl;
        VertexDistance3D dist;

  double sigmacut = 3.0;
  double Tini     = 256.;
  double ratio    = 0.25;

  AdaptiveVertexFitter theAdaptiveFitter(
                                            GeometricAnnealing(sigmacut, Tini, ratio),
                                            DefaultLinearizationPointFinder(),
                                            KalmanVertexUpdator<5>(),
                                            KalmanVertexTrackCompatibilityEstimator<5>(),
                                            KalmanVertexSmoother() );



	reco::VertexCollection recoVertices;

       VertexDistance3D vdist;


for(std::vector<reco::Vertex>::const_iterator sv = secondaryVertices.begin();
	    sv != secondaryVertices.end(); ++sv) {
	    float gamma=sv->p4().Gamma();	
	    float dRCut1 = 3./gamma;
	    if(dRCut1 > dRCut) dRCut1=dRCut;
	    if(dRCut1 < minDRCut) dRCut1=minDRCut;
/*          recoVertices->push_back(*sv);

       for(std::vector<reco::Vertex>::iterator sv = recoVertices->begin();
	    sv != recoVertices->end(); ++sv) {
*/
	    GlobalPoint ppv(pv.position().x(),pv.position().y(),pv.position().z());
	    GlobalPoint ssv(sv->position().x(),sv->position().y(),sv->position().z());
            GlobalVector flightDir = ssv-ppv;
//            std::cout << "Vertex : " << sv-secondaryVertices->begin() << " " << sv->position() << std::endl;
            Measurement1D dlen= vdist.distance(pv,*sv);
            std::vector<reco::TransientTrack>  selTracks;
       for(unsigned int itrack = 0; itrack < selectedTracks.size(); itrack++){
	        TrackRef ref = (selectedTracks)[itrack];

        //for(TrackCollection::const_iterator track = tracks->begin();
          //  track != tracks->end(); ++track) {

            //    TrackRef ref(tracks, track - tracks->begin());
	        if(!trackFilterArbitrator(ref))                         continue;
                TransientTrack tt = trackBuilder->build(ref);
                tt.setBeamSpot(*beamSpot);
	        float w = sv->trackWeight(ref);
                std::pair<bool,Measurement1D> ipv = IPTools::absoluteImpactParameter3D(tt,pv);
                std::pair<bool,Measurement1D> itpv = IPTools::absoluteTransverseImpactParameter(tt,pv);
                std::pair<bool,Measurement1D> isv = IPTools::absoluteImpactParameter3D(tt,*sv);
		float dR = Geom::deltaR(flightDir,tt.track()); //.eta(), flightDir.phi(), tt.track().eta(), tt.track().phi());
		float scalePVsig=1.;	 //0.5
                if( w > 0 || ( isv.second.significance() < sigCut && isv.second.value() < distCut && isv.second.value() < dlen.value()*dLenFraction ) )
                {

                  if(( isv.second.value() < ipv.second.value()*scalePVsig  ) && isv.second.value() < distCut && isv.second.value() < dlen.value()*dLenFraction 
                  && dR < dRCut1  ) 
                  {
#ifdef VTXDEBUG
                     if(w > 0.5) std::cout << " = ";
                    else std::cout << " + ";
#endif 
                     selTracks.push_back(tt);
                  } else
                  {
#ifdef VTXDEBUG
                     if(w > 0.5 && isv.second.value() > ipv.second.value()*scalePVsig ) std::cout << " - ";
                  else std::cout << "   ";
#endif
                     //add also the tracks used in previous fitting that are still closer to Sv than Pv 
                     if(w > 0.5 && isv.second.value() <= ipv.second.value()*scalePVsig && dR < dRCut1) {  
                       selTracks.push_back(tt);
#ifdef VTXDEBUG
                       std::cout << " = ";
#endif
                     }
                     if(w > 0.5 && isv.second.value() <= ipv.second.value()*scalePVsig && dR >= dRCut1) {
#ifdef VTXDEBUG
                       std::cout << " - ";
#endif

                     }

                    
                  }
#ifdef VTXDEBUG

                  std::cout << "t : " << itrack << " ref " << ref.key() << " w: " << w 
                  << " svip: " << isv.second.significance() << " " << isv.second.value()  
                  << " pvip: " << ipv.second.significance() << " " << ipv.second.value()  << " res " << tt.track().residualX(0)   << "," << tt.track().residualY(0) 
                  << " tpvip: " << itpv.second.significance() << " " << itpv.second.value()  << " dr: "   << dR << std::endl;
#endif
 
                }
               else
                 {
#ifdef VTXDEBUG

                  std::cout << " . t : " << itrack << " ref " << ref.key()<<  " w: " << w 
                  << " svip: " << isv.second.significance() << " " << isv.second.value() << " res " << tt.track().residualX(0)   << "," << tt.track().residualY(0) 
                  << " pvip: " << ipv.second.significance() << " " << ipv.second.value()  << " dr: "   << dR << std::endl;
#endif

                 }
           }      

           if(selTracks.size() >= 2)
              { 
             	 TransientVertex singleFitVertex;
             	 singleFitVertex = theAdaptiveFitter.vertex(selTracks,ssv);
              	if(singleFitVertex.isValid())  { recoVertices.push_back(reco::Vertex(singleFitVertex));

#ifdef VTXDEBUG
                const reco::Vertex & extVertex = recoVertices.back();
                      GlobalVector vtxDir = GlobalVector(extVertex.p4().X(),extVertex.p4().Y(),extVertex.p4().Z());


		std::cout << " pointing : " << Geom::deltaR(extVertex.position() - pv.position(), vtxDir) << std::endl;
#endif
		}
              } 
	}
	return recoVertices;
}









