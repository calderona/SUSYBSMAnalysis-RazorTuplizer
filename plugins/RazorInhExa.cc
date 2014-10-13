/*
Example of inherited class from class RazorTuplizer
Defines just constructor and destructor
*/

#include "RazorInhExa.hh"

//Defines Constructor
RazorAna::RazorAna(const edm::ParameterSet& iConfig) : RazorTuplizer(iConfig){};
//Defines Destructor
RazorAna::~RazorAna(){};
//Define custom analyzer
void RazorAna::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  using namespace edm;
  
  //initialize
  resetBranches();
  loadEvent(iEvent); //loads objects and resets tree branches

  bool isGoodEvent =
    fillEventInfo(iEvent)
    && fillPileUp()
    && fillMuons()
    && fillElectrons()
    && fillTaus()
    && fillPhotons()
    && fillJets()
    && fillJetsAK8()
    && fillMet()
    && fillRazor()
    && fillMC()
    && fillGenParticles();

  //fill the tree if the event wasn't rejected
  if(isGoodEvent) RazorEvents->Fill();
};

void RazorAna::resetBranches(){
  RazorTuplizer::resetBranches();
  //Re-set newly defined variables;
  //PU
  nBunchXing = 0;
  for(int j = 0; j < 99; j++){
    //PU
    BunchXing[j] = -99;
    nPU[j] = -99;
    nPUmean[j] = -99.0;
    
    //Mu
    muonCharge[j] = -99;
    muonIsLoose[j] = false;
    muonIsTight[j] = false;
    muon_d0[j] = -99.0;
    muon_dZ[j] = -99.0;
    muon_ip3d[j] = -99.0;
    muon_ip3dSignificance[j] = -99.0;
    muonType[j] = 0;
    muon_sumChargedHadronPt[j] = -99.0;
    muon_sumChargedParticlePt[j] = -99.0;
    muon_sumNeutralHadronEt[j] = -99.0;
    muon_sumPhotonEt[j] = -99.0;
    
    //Ele
    eleE_SC[j] = -99.0;
    eleEta_SC[j] = -99.0;
    elePhi_SC[j] = -99.0;
    eleSigmaIetaIeta[j] = -99.0;
    eleFull5x5SigmaIetaIeta[j] = -99.0;
    eleR9[j] = -99;
    ele_dEta[j] = -99;
    ele_dPhi[j] = -99;
    ele_HoverE[j] = -99;
    ele_d0[j] = -99;
    ele_dZ[j] = -99;
    ele_sumChargedHadronPt[j] = -99.0;
    ele_sumNeutralHadronEt[j] = -99.0;
    ele_sumPhotonEt[j] = -99.0;
    ele_MissHits[j] = -99;
    ele_ConvRejec[j] = -99;
    ele_OneOverEminusOneOverP[j] = -99.0;
    ele_RegressionE[j] = -99.0;
    ele_CombineP4[j] = -99.0;

    //Taus
    
    //Photons
    phoSigmaIetaIeta[j] = -99.0;
    phoFull5x5SigmaIetaIeta[j] = -99.0;
    phoR9[j] = -99.0;
    pho_HoverE[j] = -99.0;
    pho_sumChargedHadronPt[j] = -99.0;
    pho_sumNeutralHadronEt[j] = -99.0;
    pho_sumPhotonEt[j] = -99.0;
    pho_isConversion[j] = -99;
    pho_RegressionE[j] = -99.0;
    pho_IDMVA[j] = -99.0;

    //Jets
    jetMass[j] =  -99.0;
    jetJetArea[j] = -99.0;
    jetPileupE[j] = -99.0;

    //Event Info
    pvX = -99.0;
    pvY = -99.0;
    pvZ = -99.0;

    //MET
    sumMET = -99.0;
    UncMETdpx = -99.0;
    UncMETdpy = -99.0;
    UncMETdSumEt = -99.0;
    
    //GenInfo
    nGenParticle = 0;
    motherIndex[j] = -99999;
    motherId[j] = -99999;
    gParticleId[j] = -99999;
    gParticleE[j] = -99999.0;
    gParticlePt[j] = -99999.0;
    gParticleEta[j] = -99999.0;
    gParticlePhi[j] = -99999.0;
    gParticleVx[j] = -99999.0;
    gParticleVy[j] = -99999.0;
    gParticleVz[j] = -99999.0;
  }
};

void RazorAna::setBranches(){
  enableEventInfoBranches();
  enablePileUpBranches();
  enableMuonBranches();
  enableElectronBranches();
  enableTauBranches();
  enablePhotonBranches();
  enableJetBranches();
  enableJetAK8Branches();
  enableMetBranches();
  enableRazorBranches();
  enableMCBranches();
  enableGenParticles();
  
};


void RazorAna::enableEventInfoBranches(){
  RazorTuplizer::enableEventInfoBranches();
  RazorEvents->Branch("pvX", &pvX, "pvX/F");
  RazorEvents->Branch("pvY", &pvY, "pvY/F");
  RazorEvents->Branch("pvZ", &pvZ, "pvZ/F");
};

void RazorAna::enablePileUpBranches(){
  RazorEvents->Branch("nBunchXing", &nBunchXing, "nBunchXing/I");
  RazorEvents->Branch("BunchXing", BunchXing, "BunchXing[nBunchXing]/I");
  RazorEvents->Branch("nPU", nPU, "nPU[nBunchXing]/I");
  RazorEvents->Branch("nPUmean", nPUmean, "nPUmean[nBunchXing]/F");
};

void RazorAna::enableMuonBranches(){
  RazorTuplizer::enableMuonBranches();
  RazorEvents->Branch("muonCharge", muonCharge, "muonCharge[nMuons]/I");
  RazorEvents->Branch("muonIsLoose", muonIsLoose,"muonIsLoose[nMuons]/O");
  RazorEvents->Branch("muonIsTight", muonIsTight,"muonIsTight[nMuons]/O");
  RazorEvents->Branch("muon_d0", muon_d0, "muon_d0[nMuons]/F");
  RazorEvents->Branch("muon_dZ", muon_dZ, "muon_dZ[nMuons]/F");
  RazorEvents->Branch("muon_ip3d", muon_ip3d, "muon_ip3d[nMuons]/F");
  RazorEvents->Branch("muon_ip3dSignificance", muon_ip3dSignificance, "muon_ip3dSignificance[nMuons]/F");
  RazorEvents->Branch("muonType", muonType, "muonType[nMuons]/s");
  RazorEvents->Branch("muon_sumChargedHadronPt", muon_sumChargedHadronPt, "muon_sumChargedHadronPt[nMuons]/F");
  RazorEvents->Branch("muon_sumChargedParticlePt", muon_sumChargedParticlePt, "muon_sumChargedParticlePt[nMuons]/F");
  RazorEvents->Branch("muon_sumNeutralHadronEt", muon_sumNeutralHadronEt, "muon_sumNeutralHadronEt[nMuons]/F");
  RazorEvents->Branch("muon_sumPhotonEt", muon_sumPhotonEt, "muon_sumPhotonEt[nMuons]/F");
};

void RazorAna::enableElectronBranches(){
  RazorTuplizer::enableElectronBranches();
  RazorEvents->Branch("eleCharge", eleCharge, "eleCharge[nElectrons]/F");
  //RazorEvents->Branch("EleE_SC", eleE_SC,"eleE_SC[nElectrons]/F");
  //RazorEvents->Branch("eleEta_SC", eleEta_SC,"eleEta_SC[nElectrons]/F");
  //RazorEvents->Branch("elePhi_SC", elePhi_SC,"elePhi_SC[nElectrons]/F");
  RazorEvents->Branch("eleSigmaIetaIeta", eleSigmaIetaIeta, "eleSigmaIetaIeta[nElectrons]/F");
  RazorEvents->Branch("eleFull5x5SigmaIetaIeta", eleFull5x5SigmaIetaIeta, "eleFull5x5SigmaIetaIeta[nElectrons]/F");
  RazorEvents->Branch("eleR9", eleR9, "eleR9[nElectrons]/F");
  RazorEvents->Branch("ele_dEta", ele_dEta, "ele_dEta[nElectrons]/F");
  RazorEvents->Branch("ele_dPhi", ele_dPhi, "ele_dPhi[nElectrons]/F");
  RazorEvents->Branch("ele_HoverE", ele_HoverE, "ele_HoverE[nElectrons]/F");
  RazorEvents->Branch("ele_d0", ele_d0, "ele_d0[nElectrons]/F");
  RazorEvents->Branch("ele_dZ", ele_dZ, "ele_dZ[nElectrons]/F");
  RazorEvents->Branch("ele_sumChargedHadronPt", ele_sumChargedHadronPt, "ele_sumChargedHadronPt[nElectrons]/F");
  RazorEvents->Branch("ele_sumNeutralHadronEt", ele_sumNeutralHadronEt, "ele_sumNeutralHadronEt[nElectrons]/F");
  RazorEvents->Branch("ele_sumPhotonEt", ele_sumPhotonEt, "ele_sumPhotonEt[nElectrons]/F");
  RazorEvents->Branch("ele_MissHits", ele_MissHits, "ele_MissHits[nElectrons]/I");
  RazorEvents->Branch("ele_ConvRejec", ele_ConvRejec, "ele_ConvRejec[nElectrons]/I");
  RazorEvents->Branch("ele_OneOverEminusOneOverP", ele_OneOverEminusOneOverP, "ele_OneOverEminusOneOverP[nElectrons]/F");
  RazorEvents->Branch("ele_RegressionE", ele_RegressionE, "ele_RegressionE[nElectrons]/F");
  RazorEvents->Branch("ele_CombineP4", ele_CombineP4, "ele_CombineP4[nElectrons]/F");
};

void RazorAna::enableTauBranches(){
  RazorTuplizer::enableTauBranches();
};

void RazorAna::enablePhotonBranches(){
  RazorTuplizer::enablePhotonBranches();
  RazorEvents->Branch("phoSigmaIetaIeta", phoSigmaIetaIeta, "phoSigmaIetaIeta[nPhotons]/F");
  RazorEvents->Branch("phoFull5x5SigmaIetaIeta", phoFull5x5SigmaIetaIeta, "phoFull5x5SigmaIetaIeta[nPhotons]/F");
  RazorEvents->Branch("phoR9", phoR9, "phoR9[nPhotons]/F");
  RazorEvents->Branch("pho_HoverE", pho_HoverE, "pho_HoverE[nPhotons]/F");
  RazorEvents->Branch("pho_sumChargedHadronPt", pho_sumChargedHadronPt, "pho_sumChargedHadronPt[nPhotons]/F");
  RazorEvents->Branch("pho_sumNeutralHadronEt", pho_sumNeutralHadronEt, "pho_sumNeutralHadronEt[nPhotons]/F");
  RazorEvents->Branch("pho_sumPhotonEt", pho_sumPhotonEt, "pho_sumPhotonEt[nPhotons]/F");
  RazorEvents->Branch("pho_isConversion", pho_isConversion, "pho_isConversion[nPhotons]/I");
  RazorEvents->Branch("pho_RegressionE", pho_RegressionE, "pho_RegressionE[nPhotons]/F");
  RazorEvents->Branch("pho_IDMVA", pho_IDMVA, "pho_IDMVA[nPhotons]/F");
};

void RazorAna::enableJetBranches(){
  RazorTuplizer::enableJetBranches();
  RazorEvents->Branch("jetMass", jetMass, "jetMass[nJets]/F");
  RazorEvents->Branch("jetJetArea", jetJetArea, "jetJetArea[nJets]/F");
  RazorEvents->Branch("jetPileupE", jetPileupE, "jetPileupE[nJets]/F");
};

void RazorAna::enableJetAK8Branches(){
  RazorTuplizer::enableJetAK8Branches();
};

void RazorAna::enableMetBranches(){
  RazorTuplizer::enableMetBranches();
  RazorEvents->Branch("sumMET", &sumMET, "sumMET/F");
}

void RazorAna::enableRazorBranches(){
  RazorTuplizer::enableRazorBranches();
}
void RazorAna::enableGenParticles(){
  RazorEvents->Branch("nGenParticle", &nGenParticle, "nGenParticle/s");
  RazorEvents->Branch("motherIndex", motherIndex, "motherIndex[nGenParticle]/I");
  RazorEvents->Branch("motherId", motherId, "motherId[nGenParticle]/I");
  RazorEvents->Branch("gParticleId", gParticleId, "gParticleId[nGenParticle]/I");
  RazorEvents->Branch("gParticleE", gParticleE, "gParticleE[nGenParticle]/F");
  RazorEvents->Branch("gParticlePt", gParticlePt, "gParticlePt[nGenParticle]/F");
  RazorEvents->Branch("gParticleEta", gParticleEta, "gParticleEta[nGenParticle]/F");
  RazorEvents->Branch("gParticlePhi", gParticlePhi, "gParticlePhi[nGenParticle]/F");
  RazorEvents->Branch("gParticleVx", gParticleVx, "gParticleVx[nGenParticle]/F");
  RazorEvents->Branch("gParticleVy", gParticleVy, "gParticleVy[nGenParticle]/F");
  RazorEvents->Branch("gParticleVz", gParticleVz, "gParticleVz[nGenParticle]/F");
}

/*
Re-defining Fill methods (will not use mother class methods)
*/

bool RazorAna::fillEventInfo(const edm::Event& iEvent){
  //store basic event info
  runNum = iEvent.id().run();
  lumiNum = iEvent.luminosityBlock();
  eventNum = iEvent.id().event();

  //select the primary vertex, if any
  if (vertices->empty()) return false; // skip the event if no PV found
  const reco::Vertex &PV = vertices->front();
  pvX = PV.x();
  pvY = PV.y();
  pvZ= PV.z();
  
  nPV = 0;
  //Check for good vertices
  for(unsigned int i = 0; i < vertices->size(); i++){
    if(vertices->at(i).isValid() && !vertices->at(i).isFake())nPV++;
  }
  if(nPV == 0)return false;

  return true;  
};

bool RazorAna::fillPileUp(){
  for(const PileupSummaryInfo &pu : *puInfo){
    BunchXing[nBunchXing] = pu.getBunchCrossing();
    nPU[nBunchXing] = pu.getPU_NumInteractions();
    nPUmean[nBunchXing] = pu.getTrueNumInteractions();
    nBunchXing++;
    //std::cout << "BC: " << pu.getBunchCrossing() << std::endl;
  }
  return true;
};

bool RazorAna::fillMuons(){
  //PV required for Tight working point
  const reco::Vertex &PV = vertices->front();
  for(const pat::Muon &mu : *muons){
    if(mu.pt() < 5) continue;
    muonE[nMuons] = mu.energy();
    muonPt[nMuons] = mu.pt();
    muonEta[nMuons] = mu.eta();
    muonPhi[nMuons] = mu.phi();
    muonCharge[nMuons] = mu.charge();
    muonIsLoose[nMuons] = mu.isLooseMuon();
    muonIsTight[nMuons] = mu.isTightMuon(PV);
    muon_d0[nMuons] = -mu.muonBestTrack()->dxy(PV.position());
    muon_dZ[nMuons] = mu.muonBestTrack()->dz(PV.position());
    muon_ip3d[nMuons] = mu.dB(pat::Muon::PV3D);
    muon_ip3dSignificance[nMuons] = mu.dB(pat::Muon::PV3D)/mu.edB(pat::Muon::PV3D);
    muonType[nMuons] = mu.isMuon() + mu.isGlobalMuon() + mu.isTrackerMuon() + mu.isStandAloneMuon()
      + mu.isCaloMuon() + mu.isPFMuon() + mu.isRPCMuon();
    muon_sumChargedHadronPt[nMuons] = mu.pfIsolationR04().sumChargedHadronPt;
    muon_sumChargedParticlePt[nMuons] = mu.pfIsolationR04().sumChargedParticlePt;
    muon_sumNeutralHadronEt[nMuons] = mu.pfIsolationR04().sumNeutralHadronEt;
    muon_sumPhotonEt[nMuons] =  mu.pfIsolationR04().sumPhotonEt;
    nMuons++;
  }

  return true;
};

bool RazorAna::fillElectrons(){
  const reco::Vertex &PV = vertices->front();
  for(const pat::Electron &ele : *electrons){
    if(ele.pt() < 5) continue;
    eleE[nElectrons] = ele.energy();
    elePt[nElectrons] = ele.pt();
    eleEta[nElectrons] = ele.eta();
    elePhi[nElectrons] = ele.phi();
    eleCharge[nElectrons] = ele.charge();
    eleE_SC[nElectrons] = ele.superCluster()->energy();
    eleEta_SC[nElectrons] = ele.superCluster()->eta();
    elePhi_SC[nElectrons] = ele.superCluster()->phi();
    eleSigmaIetaIeta[nElectrons] = ele.sigmaIetaIeta();
    eleFull5x5SigmaIetaIeta[nElectrons] = ele.full5x5_sigmaIetaIeta();
    eleR9[nElectrons] = ele.r9();
    ele_dEta[nElectrons] = ele.deltaEtaSuperClusterTrackAtVtx();
    ele_dPhi[nElectrons] = ele.deltaPhiSuperClusterTrackAtVtx();
    ele_HoverE[nElectrons] = ele.hcalOverEcalBc();
    ele_d0[nElectrons] = -ele.gsfTrack().get()->dxy(PV.position());
    ele_dZ[nElectrons] = ele.gsfTrack().get()->dz(PV.position());
    ele_sumChargedHadronPt[nElectrons] = ele.pfIsolationVariables().sumChargedHadronPt;
    ele_sumNeutralHadronEt[nElectrons] = ele.pfIsolationVariables().sumNeutralHadronEt;
    ele_sumPhotonEt[nElectrons] = ele.pfIsolationVariables().sumPhotonEt;
    ele_MissHits[nElectrons] = ele.gsfTrack()->trackerExpectedHitsInner().numberOfLostHits();
    ele_ConvRejec[nElectrons] = ele.convFlags();
    ele_OneOverEminusOneOverP[nElectrons] = 1./ele.correctedEcalEnergy()  -  1./ele.trackMomentumAtVtx().R();
    ele_RegressionE[nElectrons] = ele.ecalRegressionEnergy();
    ele_CombineP4[nElectrons] = ele.ecalTrackRegressionEnergy();
    nElectrons++;
  }
  
  return true;
};

bool RazorAna::fillTaus(){
  for (const pat::Tau &tau : *taus) {
    if (tau.pt() < 20) continue;
    tauE[nTaus] = tau.energy();
    tauPt[nTaus] = tau.pt();
    tauEta[nTaus] = tau.eta();
    tauPhi[nTaus] = tau.phi();
    nTaus++;
  }

  return true;
};

bool RazorAna::fillPhotons(){
  for (const pat::Photon &pho : *photons) {
    if (pho.pt() < 20 or pho.chargedHadronIso()/pho.pt() > 0.3) continue;
    phoE[nPhotons] = pho.energy();
    phoPt[nPhotons] = pho.pt();
    phoEta[nPhotons] = pho.eta();
    phoPhi[nPhotons] = pho.phi();
    phoSigmaIetaIeta[nPhotons] = pho.sigmaIetaIeta();
    phoFull5x5SigmaIetaIeta[nPhotons] = pho.full5x5_sigmaIetaIeta();
    phoR9[nPhotons] = pho.r9();
    pho_sumChargedHadronPt[nPhotons] = pho.chargedHadronIso();
    pho_sumNeutralHadronEt[nPhotons] = pho.neutralHadronIso();
    pho_sumPhotonEt[nPhotons] = pho.photonIso();
    pho_isConversion[nPhotons] = pho.hasConversionTracks();
    pho_RegressionE[nPhotons] = pho.getCorrectedEnergy(reco::Photon::P4type::regression1);
    pho_IDMVA[nPhotons] = pho.pfMVA();
    /*
    const reco::Candidate* genPhoton = pho.genPhoton();
    if(genPhoton != NULL)std::cout << "======>gen PT: " << genPhoton->pt() <<
      " recoPT: " << pho.pt() << std::endl;
    */
    nPhotons++;
  }

  return true;
};

bool RazorAna::fillJets(){
  for (const pat::Jet &j : *jets) {
    if (j.pt() < 20) continue;
    jetE[nJets] = j.energy();
    jetPt[nJets] = j.pt();
    jetEta[nJets] = j.eta();
    jetPhi[nJets] = j.phi();
    jetCSV[nJets] = j.bDiscriminator("combinedSecondaryVertexBJetTags");
    jetCISV[nJets] = j.bDiscriminator("combinedInclusiveSecondaryVertexBJetTags");
    jetMass[nJets] = j.mass();
    jetJetArea[nJets] = j.jetArea();
    jetPileupE[nJets] = j.pileup();
    nJets++;
  }

  return true;
};

bool RazorAna::fillJetsAK8(){
  for (const pat::Jet &j : *jetsAK8) {
    fatJetE[nFatJets] = j.energy();
    fatJetPt[nFatJets] = j.pt();
    fatJetEta[nFatJets] = j.eta();
    fatJetPhi[nFatJets] = j.phi();
    nFatJets++;
  }

  return true;
};

bool RazorAna::fillMet(){
  const pat::MET &Met = mets->front();
  metPt = Met.pt();
  metPhi = Met.phi();
  sumMET = Met.sumEt();
  genMetPt = Met.genMET()->pt();
  genMetPhi = Met.genMET()->phi();
  return true;
};

bool RazorAna::fillRazor(){
  //get good jets for razor calculation                                  
  vector<TLorentzVector> goodJets;
  for (const pat::Jet &j : *jets) {
    if (j.pt() < 40) continue;
    if (fabs(j.eta()) > 3.0) continue;
    //add to goodJets vector                                          
    TLorentzVector newJet(j.px(), j.py(), j.pz(), j.energy());
    goodJets.push_back(newJet);
  }
  //MET                                                                                                        
  const pat::MET &Met = mets->front();
  
  //compute the razor variables using the selected jets and MET
  if(goodJets.size() > 1){
    vector<TLorentzVector> hemispheres = getHemispheres(goodJets);
    TLorentzVector pfMet(Met.px(), Met.py(), 0.0, 0.0);
    MR = computeMR(hemispheres[0], hemispheres[1]);
    RSQ = computeR2(hemispheres[0], hemispheres[1], pfMet);
  }

  return true;
};

bool RazorAna::fillGenParticles(){
  std::vector<const reco::Candidate*> prunnedV;//Allows easier comparison for mother finding
  //Fills selected gen particles
  for(size_t i=0; i<prunedGenParticles->size();i++){
    if((abs((*prunedGenParticles)[i].pdgId()) >= 1 && abs((*prunedGenParticles)[i].pdgId()) <= 6)
       || (abs((*prunedGenParticles)[i].pdgId()) >= 11 && abs((*prunedGenParticles)[i].pdgId()) <= 16)
       || (abs((*prunedGenParticles)[i].pdgId()) >= 21 && abs((*prunedGenParticles)[i].pdgId()) <= 25)
       || (abs((*prunedGenParticles)[i].pdgId()) >= 32 && abs((*prunedGenParticles)[i].pdgId()) <= 42)
       || (abs((*prunedGenParticles)[i].pdgId()) >= 1000001 && abs((*prunedGenParticles)[i].pdgId()) <= 1000039)
       ){
      prunnedV.push_back(&(*prunedGenParticles)[i]);
    }
  }
  //Total number of gen particles
  nGenParticle = prunnedV.size();
  //Look for mother particle and Fill gen variables
  for(unsigned int i = 0; i < prunnedV.size(); i++){
    gParticleId[i] = prunnedV[i]->pdgId();
    gParticleE[i] = prunnedV[i]->energy();
    gParticlePt[i] = prunnedV[i]->pt();
    gParticleEta[i] = prunnedV[i]->eta();
    gParticlePhi[i] = prunnedV[i]->phi();
    gParticleVx[i] = prunnedV[i]->vx();
    gParticleVy[i] = prunnedV[i]->vy();
    gParticleVz[i] = prunnedV[i]->vz();
    if(prunnedV[i]->numberOfMothers() == 0){
      motherIndex[i] = -1;
      motherId[i] = -9999; 
      continue;
    }
    for(unsigned int j = 0; j < prunnedV.size(); j++){
      if(prunnedV[j] == prunnedV[i]->mother()){
	motherIndex[i] = j;
	motherId[i] = prunnedV[j]->pdgId();
      }
    }
  }
  return true;
};

bool RazorAna::isAncestor(const reco::Candidate* ancestor, const reco::Candidate* particle){
  //particle is already the ancestor
  if(ancestor == particle ) return true;
  //otherwise loop on mothers, if any and return true if the ancestor is found
  for(size_t i=0;i< particle->numberOfMothers();i++){
    if(isAncestor(ancestor,particle->mother(i))) return true;
  }
  //if we did not return yet, then particle and ancestor are not relatives
  return false;
};

//------ Method called once each job just before starting event loop ------//
void RazorAna::beginJob(){
  setBranches();
};
