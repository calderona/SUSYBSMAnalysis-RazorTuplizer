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
  resetAnaBranches();
  loadEvent(iEvent); //loads objects and resets tree branches

  bool isGoodEvent =
    fillEventInfo(iEvent)
    && fillMuons()
    && fillElectrons()
    && fillTaus()
    && fillPhotons()
    && fillJets()
    && fillJetsAK8()
    && fillMet()
    && fillRazor();

  //fill the tree if the event wasn't rejected
  if(isGoodEvent) outputTree->Fill();
};

void RazorAna::resetAnaBranches(){
  resetBranches();
  //Re-set newly defined variables;
  for(int j = 0; j < 99; j++){
    muonIsLoose[j] = 0.0;
    muonIsTight[j] = 0.0;
  }
};

void RazorAna::setAnaBranches(){
  enableEventInfoBranches();
  enableMuonBranchesAna();
  enableEleBranchesAna();
  enableTauBranchesAna();
  enablePhotonBranchesAna();
  enableJetBranchesAna();
  enableJetAK8BranchesAna();
  enableMetBranchesAna();
  enableRazorBranchesAna();
};

void RazorAna::enableMuonBranchesAna(){
  enableMuonBranches();
  std::cout << "======= enable new Branches =====" << std::endl;
  outputTree->Branch("muonIsLoose", muonIsLoose,"muonIsLoose[nMuons]/F");
  outputTree->Branch("muonIsTight", muonIsTight,"muonIsTight[nMuons]/F");
};

void RazorAna::enableEleBranchesAna(){
  enableElectronBranches();
}

void RazorAna::enableTauBranchesAna(){
  enableTauBranches();
};

void RazorAna::enablePhotonBranchesAna(){
  enablePhotonBranches();
};

void RazorAna::enableJetBranchesAna(){
  enableJetBranches();
};

void RazorAna::enableJetAK8BranchesAna(){
  enableJetAK8Branches();
};

void RazorAna::enableMetBranchesAna(){
  enableMetBranches();
}

void RazorAna::enableRazorBranchesAna(){
  enableRazorBranches();
}

/*
Re-defining Fill methods (will not use mother class methods)
*/
bool RazorAna::fillMuons(){
  //PV required for Tight working point
  const reco::Vertex &PV = vertices->front();
  for(const pat::Muon &mu : *muons){
    if(mu.pt() < 5) continue;
    muonE[nMuons] = mu.energy();
    muonPt[nMuons] = mu.pt();
    muonEta[nMuons] = mu.eta();
    muonPhi[nMuons] = mu.phi();
    muonIsLoose[nMuons] = mu.isLooseMuon();
    muonIsTight[nMuons] = mu.isTightMuon(PV);
    nMuons++;
  }

  return true;
};

bool RazorAna::fillElectrons(){
  for(const pat::Electron &ele : *electrons){
    if(ele.pt() < 5) continue;
    eleE[nElectrons] = ele.energy();
    elePt[nElectrons] = ele.pt();
    eleEta[nElectrons] = ele.eta();
    elePhi[nElectrons] = ele.phi();
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

  return true;
};

bool RazorAna::fillRazor(){
  //get good jets for razor calculation                                  
  vector<TLorentzVector> goodJets;
  for (const pat::Jet &j : *jets) {
    if (j.pt() < 20) continue;
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

//------ Method called once each job just before starting event loop ------//                                                               
void RazorAna::beginJob(){
  setAnaBranches();
};