#include "RecoJets/JetProducers/interface/ECFAdder.h"
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "FWCore/Framework/interface/MakerMacros.h"

ECFAdder::ECFAdder(const edm::ParameterSet& iConfig) :
  src_(iConfig.getParameter<edm::InputTag>("src")),
  src_token_(consumes<edm::View<reco::Jet>>(src_)),
  Njets_(iConfig.getParameter<std::vector<unsigned> >("Njets")),
  beta_(iConfig.getParameter<double>("beta"))
{
  for ( std::vector<unsigned>::const_iterator n = Njets_.begin(); n != Njets_.end(); ++n )
    {
      std::ostringstream ecfN_str;
      ecfN_str << "ecf" << *n;
      variables_.push_back(ecfN_str.str());
      produces<edm::ValueMap<float> >(ecfN_str.str().c_str());
      routine_.push_back(std::auto_ptr<fastjet::contrib::EnergyCorrelator> ( new fastjet::contrib::EnergyCorrelator( *n, beta_, fastjet::contrib::EnergyCorrelator::pt_R ) ));
    }  
}

void ECFAdder::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {
  // read input collection
  edm::Handle<edm::View<reco::Jet> > jets;
  iEvent.getByToken(src_token_, jets);
  
  unsigned i=0;
  for ( std::vector<unsigned>::const_iterator n = Njets_.begin(); n != Njets_.end(); ++n )
    {
      // prepare room for output
      std::vector<float> ecfN;
      ecfN.reserve(jets->size());

      for ( typename edm::View<reco::Jet>::const_iterator jetIt = jets->begin() ; jetIt != jets->end() ; ++jetIt ) {

	edm::Ptr<reco::Jet> jetPtr = jets->ptrAt(jetIt - jets->begin());

	float t=getECF( i, jetPtr );

	ecfN.push_back(t);
      }

      std::auto_ptr<edm::ValueMap<float> > outT(new edm::ValueMap<float>());
      edm::ValueMap<float>::Filler fillerT(*outT);
      fillerT.insert(jets, ecfN.begin(), ecfN.end());
      fillerT.fill();

      iEvent.put(outT,variables_[i].c_str());
      ++i;
    }
}

float ECFAdder::getECF(unsigned index, const edm::Ptr<reco::Jet> & object) const
{
  std::vector<fastjet::PseudoJet> FJparticles;
  for (unsigned k = 0; k < object->numberOfDaughters(); ++k)
    {
      const reco::CandidatePtr & dp = object->daughterPtr(k);
      if ( dp.isNonnull() && dp.isAvailable() ){

	// Here, the daughters are the "end" node, so this is a PFJet
	if ( dp->numberOfDaughters() == 0 ) {
	  FJparticles.push_back( fastjet::PseudoJet( dp->px(), dp->py(), dp->pz(), dp->energy() ) );
	} else { // Otherwise, this is a BasicJet, so you need to descend further.
	  auto subjet = dynamic_cast<reco::Jet const * >( dp.get() );
	  for ( unsigned l = 0; l < subjet->numberOfDaughters(); ++l ) {
	    if ( subjet != 0 ) {
	      const reco::CandidatePtr & ddp = subjet->daughterPtr(l);
	      FJparticles.push_back( fastjet::PseudoJet( ddp->px(), ddp->py(), ddp->pz(), ddp->energy() ) );	      
	    } else {
	      edm::LogWarning("MissingJetConstituent") << "BasicJet constituent required for ECF computation is missing!";
	    }
	  }
	} // end if basic jet
      } // end if daughter pointer is nonnull and available
      else
	edm::LogWarning("MissingJetConstituent") << "Jet constituent required for ECF computation is missing!";
    }
    return routine_[index]->result(join(FJparticles)); 
}



DEFINE_FWK_MODULE(ECFAdder);
