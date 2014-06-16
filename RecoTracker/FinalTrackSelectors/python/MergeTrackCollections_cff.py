import FWCore.ParameterSet.Config as cms

import RecoTracker.FinalTrackSelectors.trackListMerger_cfi
generalTracks = RecoTracker.FinalTrackSelectors.trackListMerger_cfi.trackListMerger.clone(
    TrackProducers = (cms.InputTag('initialStepTracks'),
                      cms.InputTag('jetCoreRegionalStepTracks'),	
                      cms.InputTag('lowPtTripletStepTracks'),
                      cms.InputTag('pixelPairStepTracks'),
                      cms.InputTag('detachedTripletStepTracks'),
                      cms.InputTag('mixedTripletStepTracks'),
                      cms.InputTag('pixelLessStepTracks'),
                      cms.InputTag('tobTecStepTracks')),
    hasSelector=cms.vint32(1,1,1,1,1,1,1,1),
    selectedTrackQuals = cms.VInputTag(cms.InputTag("initialStepSelector","initialStep"),
                                       cms.InputTag("jetCoreRegionalStepSelector","jetCoreRegionalStep"),
                                       cms.InputTag("lowPtTripletStepSelector","lowPtTripletStep"),
                                       cms.InputTag("pixelPairStepSelector","pixelPairStep"),
                                       cms.InputTag("detachedTripletStep"),
                                       cms.InputTag("mixedTripletStep"),
                                       cms.InputTag("pixelLessStepSelector","pixelLessStep"),
                                       cms.InputTag("tobTecStepSelector","tobTecStep")
                                       ),
    setsToMerge = cms.VPSet( cms.PSet( tLists=cms.vint32(0,1,2,3,4,5,6,7), pQual=cms.bool(True) )
                             ),
    copyExtras = True,
    makeReKeyedSeeds = cms.untracked.bool(False)
    )

conversionStepTracks = RecoTracker.FinalTrackSelectors.trackListMerger_cfi.trackListMerger.clone(
    TrackProducers = cms.VInputTag(cms.InputTag('convStepTracks')),
    hasSelector=cms.vint32(1),
    selectedTrackQuals = cms.VInputTag(cms.InputTag("convStepSelector","convStep")
                                       ),
    setsToMerge = cms.VPSet( cms.PSet( tLists=cms.vint32(1), pQual=cms.bool(True) )
                             ),
    copyExtras = True,
    makeReKeyedSeeds = cms.untracked.bool(False)
    )
