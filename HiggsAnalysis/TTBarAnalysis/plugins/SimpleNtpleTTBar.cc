// -*- C++ -*-
//
// Package:    SimpleNtpleTTBar
// Class:      SimpleNtpleTTBar
// 
/**\class SimpleNtpleTTBar SimpleNtpleTTBar.cc Analysis/SimpleNtpleTTBar/src/SimpleNtpleTTBar.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
 */
//
// Original Author:  Andrea Massironi
//         Created:  Fri Jan  5 17:34:31 CEST 2010
// $Id: SimpleNtpleTTBar.cc,v 1.2 2010/01/14 10:41:39 amassiro Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/RefToBase.h"

#include "HiggsAnalysis/TTBarAnalysis/plugins/SimpleNtpleTTBar.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"


//--- objects ----
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/BTauReco/interface/JetTag.h"

#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETFwd.h"

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"

#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"


//---- utilities ----
#include "HiggsAnalysis/TTBarAnalysis/interface/MCDumperTTBar.h"


SimpleNtpleTTBar::SimpleNtpleTTBar(const edm::ParameterSet& iConfig)
{
 edm::Service<TFileService> fs ;
 outTree_  = fs->make <TTree>("SimpleTree","SimpleTree"); 
 
 TracksTag_ = iConfig.getParameter<edm::InputTag>("TracksTag");
 EleTag_ = iConfig.getParameter<edm::InputTag>("EleTag");
 MuTag_ = iConfig.getParameter<edm::InputTag>("MuTag");
 MetTag_ = iConfig.getParameter<edm::InputTag>("MetTag");
 JetTag_ = iConfig.getParameter<edm::InputTag>("JetTag");
 flag_JetBTag_ = iConfig.getUntrackedParameter<bool>("flag_JetBTag","False");
 if (flag_JetBTag_) JetBTag_ = iConfig.getUntrackedParameter<edm::InputTag>("JetBTag");
 correctedJetTag_ = iConfig.getParameter<edm::InputTag>("correctedJetTag");
 MCtruthTag_ = iConfig.getParameter<edm::InputTag>("MCtruthTag");
 genJetTag_ = iConfig.getParameter<edm::InputTag>("genJetTag");
 genMetTag_ = iConfig.getParameter<edm::InputTag>("genMetTag");

 verbosity_ = iConfig.getUntrackedParameter<bool>("verbosity","False");
 eventType_ = iConfig.getUntrackedParameter<int>("eventType",1);
 
 NtupleFactory_ = new NtupleFactory(outTree_);
}


// --------------------------------------------------------------------


SimpleNtpleTTBar::~SimpleNtpleTTBar()
{
 delete NtupleFactory_;
}


//
// member functions
//

// ------------ method called to for each event  ------------
void
  SimpleNtpleTTBar::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 ///---- fill muons ----
 edm::Handle<edm::View<reco::Muon> > MuHandle ;
 iEvent.getByLabel (MuTag_,MuHandle);
 int nMu;
 if(MuHandle->size() < 30 ){ nMu = MuHandle->size(); }
 else {nMu = 30;}
 for(int i=0; i< nMu; i++){
  NtupleFactory_->Fill4V("muons",(*MuHandle)[i].p4());
  NtupleFactory_->FillFloat("muons_charge",((*MuHandle)[i].charge()));
  NtupleFactory_->FillFloat("muons_tkIsoR03",((*MuHandle)[i].isolationR03()).sumPt);
  NtupleFactory_->FillFloat("muons_nTkIsoR03",((*MuHandle)[i].isolationR03()).nTracks);    
  NtupleFactory_->FillFloat("muons_emIsoR03",((*MuHandle)[i].isolationR03()).emEt);
  NtupleFactory_->FillFloat("muons_hadIsoR03",((*MuHandle)[i].isolationR03()).hadEt);
    
  NtupleFactory_->FillFloat("muons_tkIsoR05",((*MuHandle)[i].isolationR05()).sumPt);
  NtupleFactory_->FillFloat("muons_nTkIsoR05",((*MuHandle)[i].isolationR05()).nTracks);    
  NtupleFactory_->FillFloat("muons_emIsoR05",((*MuHandle)[i].isolationR05()).emEt);
  NtupleFactory_->FillFloat("muons_hadIsoR05",((*MuHandle)[i].isolationR05()).hadEt);
 }

 ///---- fill electrons ----
 edm::Handle<edm::View<reco::GsfElectron> > EleHandle ;
 iEvent.getByLabel (EleTag_,EleHandle);
 int nEle;
 if(EleHandle->size() < 30 ){ nEle = EleHandle->size(); }
 else {nEle = 30;}
 for(int i=0; i< nEle; i++){
  NtupleFactory_->Fill4V("electrons",(*EleHandle)[i].p4());
  NtupleFactory_->FillFloat("electrons_charge",((*EleHandle)[i].charge()));
  NtupleFactory_->FillFloat("electrons_tkIso",((*EleHandle)[i].dr03TkSumPt()));
  NtupleFactory_->FillFloat("electrons_emIso",((*EleHandle)[i].dr03EcalRecHitSumEt()));
  NtupleFactory_->FillFloat("electrons_hadIso_1",((*EleHandle)[i].dr03HcalDepth1TowerSumEt()));
  NtupleFactory_->FillFloat("electrons_hadIso_2",((*EleHandle)[i].dr03HcalDepth2TowerSumEt()));
  //   if ((*EleHandle)[i].classification()== GsfElectron::GOLDEN
  NtupleFactory_->FillFloat("electrons_IdLoose",0);
  NtupleFactory_->FillFloat("electrons_IdTight",0);
  NtupleFactory_->FillFloat("electrons_IdRobustLoose",0);
  NtupleFactory_->FillFloat("electrons_IdRobustTight",0);  
 }

 ///---- fill tracks ----
 edm::Handle<edm::View<reco::Track> > TracksHandle ;
 iEvent.getByLabel (TracksTag_, TracksHandle) ;
 for (edm::View<reco::Track>::const_iterator tkIt = TracksHandle->begin (); tkIt != TracksHandle->end (); ++tkIt ) 
 { 
  NtupleFactory_->Fill3V("tracks_in",(*tkIt).innerMomentum ());
  NtupleFactory_->Fill3V("tracks_out",(*tkIt).outerMomentum ());
 }
 
 ///---- fill jets ---- 
 edm::Handle<edm::View<reco::CaloJet> > JetHandle ;
 iEvent.getByLabel (JetTag_,JetHandle);
 edm::Handle<reco::JetTagCollection> bTagHandle;
 if (flag_JetBTag_) iEvent.getByLabel(JetBTag_, bTagHandle);
 reco::JetTagCollection bTags;
 if (flag_JetBTag_) bTags = *(bTagHandle.product());
 int counter_jet = 0;
 for (edm::View<reco::CaloJet>::const_iterator jetIt = JetHandle->begin (); jetIt != JetHandle->end (); ++jetIt ) 
 { 
  NtupleFactory_->Fill4V("jets",jetIt->p4());
  if (flag_JetBTag_) NtupleFactory_->FillFloat("jets_btagging",bTags[counter_jet].second);
  counter_jet++;
 }

 ///---- fill met ---- 
 edm::Handle<reco::CaloMETCollection> MetHandle ;
 iEvent.getByLabel (MetTag_,MetHandle);
 const reco::CaloMET *calomet = &(MetHandle->front());
 NtupleFactory_->Fill4V("met",calomet->p4());

 
 ///---- fill genJets ---- 
 edm::Handle<edm::View<reco::GenJet> > genJetHandle ;
 iEvent.getByLabel (genJetTag_,genJetHandle);
 for (edm::View<reco::GenJet>::const_iterator genJetIt = genJetHandle->begin (); genJetIt != genJetHandle->end (); ++genJetIt ) 
 { 
  NtupleFactory_->Fill4V("genJets",genJetIt->p4());
 }

 
  ///---- fill genMet ---- 
 edm::Handle< reco::GenMETCollection > genMetHandle ;
 iEvent.getByLabel( genMetTag_, genMetHandle ) ;

 for (reco::GenMETCollection::const_iterator gMIt = genMetHandle->begin (); gMIt != genMetHandle->end (); ++gMIt ) 
 {
  math::XYZTLorentzVector myvect_XYZT(gMIt->px(),gMIt->py(),gMIt->pz(),gMIt->energy());
  NtupleFactory_->Fill4V("genMet",myvect_XYZT);
 }
 
  
 ///---- fill MCParticle ---- 
 edm::Handle<reco::GenParticleCollection> genParticles;
 iEvent.getByLabel(MCtruthTag_, genParticles);

 std::vector<int> whichOn;
 whichOn.push_back(11);
 whichOn.push_back(12);
 MCDumper myMCDumper(genParticles, true, whichOn);

 
 std::cerr << "num ele = " << (myMCDumper.Get(11)).size() << std::endl;
 
 
 
 
 MCDumperTTBar mcAnalysis(genParticles, eventType_, verbosity_);
 bool isValid = mcAnalysis.isValid();
  
 if( (eventType_ == 0) && (isValid == true) )
 {
  NtupleFactory_->Fill4V("mcT1",mcAnalysis.mcT1()->p4());
  NtupleFactory_->FillFloat("mcT1_charge",mcAnalysis.mcT1()->charge());
  
  NtupleFactory_->Fill4V("mcT2",mcAnalysis.mcT2()->p4());
  NtupleFactory_->FillFloat("mcT2_charge",mcAnalysis.mcT2()->charge());
  
  NtupleFactory_->Fill4V("mcB1",mcAnalysis.mcB1()->p4());
  NtupleFactory_->FillFloat("mcB1_charge",mcAnalysis.mcB1()->charge());
  
  NtupleFactory_->Fill4V("mcB2",mcAnalysis.mcB2()->p4());
  NtupleFactory_->FillFloat("mcB2_charge",mcAnalysis.mcB2()->charge());
  
  NtupleFactory_->Fill4V("mcV1",mcAnalysis.mcV1()->p4());
  NtupleFactory_->FillFloat("mcV1_charge",mcAnalysis.mcV1()->charge());
  NtupleFactory_->FillFloat("mcV1_pdgId",mcAnalysis.mcV1()->pdgId());
  
  NtupleFactory_->Fill4V("mcV2",mcAnalysis.mcV2()->p4());
  NtupleFactory_->FillFloat("mcV2_charge",mcAnalysis.mcV2()->charge());
  NtupleFactory_->FillFloat("mcV2_pdgId",mcAnalysis.mcV2()->pdgId());
     
  NtupleFactory_->Fill4V("mcF1_fromV1",mcAnalysis.mcF1_fromV1()->p4());
  NtupleFactory_->FillFloat("mcF1_fromV1_charge",mcAnalysis.mcF1_fromV1()->charge());
  NtupleFactory_->FillFloat("mcF1_fromV1_pdgId",mcAnalysis.mcF1_fromV1()->pdgId());

  NtupleFactory_->Fill4V("mcF2_fromV1",mcAnalysis.mcF2_fromV1()->p4());
  NtupleFactory_->FillFloat("mcF2_fromV1_charge",mcAnalysis.mcF2_fromV1()->charge());
  NtupleFactory_->FillFloat("mcF2_fromV1_pdgId",mcAnalysis.mcF2_fromV1()->pdgId());

  NtupleFactory_->Fill4V("mcF1_fromV2",mcAnalysis.mcF1_fromV2()->p4());
  NtupleFactory_->FillFloat("mcF1_fromV2_charge",mcAnalysis.mcF1_fromV2()->charge());
  NtupleFactory_->FillFloat("mcF1_fromV2_pdgId",mcAnalysis.mcF1_fromV2()->pdgId());

  NtupleFactory_->Fill4V("mcF2_fromV2",mcAnalysis.mcF2_fromV2()->p4());
  NtupleFactory_->FillFloat("mcF2_fromV2_charge",mcAnalysis.mcF2_fromV2()->charge());
  NtupleFactory_->FillFloat("mcF2_fromV2_pdgId",mcAnalysis.mcF2_fromV2()->pdgId());
        
  }
  
 
  ///---- save the entry of the tree ----
 NtupleFactory_->FillNtuple();

}

    
// ------------ method called once each job just before starting event loop  ------------
  
  void 
    SimpleNtpleTTBar::beginJob(const edm::EventSetup& iSetup)
{
   
 NtupleFactory_->Add4V("muons");
 NtupleFactory_->AddFloat("muons_charge"); 
 NtupleFactory_->AddFloat("muons_tkIsoR03"); 
 NtupleFactory_->AddFloat("muons_nTkIsoR03"); 
 NtupleFactory_->AddFloat("muons_emIsoR03"); 
 NtupleFactory_->AddFloat("muons_hadIsoR03"); 
 NtupleFactory_->AddFloat("muons_tkIsoR05"); 
 NtupleFactory_->AddFloat("muons_nTkIsoR05"); 
 NtupleFactory_->AddFloat("muons_emIsoR05"); 
 NtupleFactory_->AddFloat("muons_hadIsoR05"); 
   
 NtupleFactory_->Add4V("electrons");
 NtupleFactory_->AddFloat("electrons_charge"); 
 NtupleFactory_->AddFloat("electrons_tkIso"); 
 NtupleFactory_->AddFloat("electrons_emIso"); 
 NtupleFactory_->AddFloat("electrons_hadIso_1"); 
 NtupleFactory_->AddFloat("electrons_hadIso_2"); 
 NtupleFactory_->AddFloat("electrons_IdLoose"); 
 NtupleFactory_->AddFloat("electrons_IdTight"); 
 NtupleFactory_->AddFloat("electrons_IdRobustLoose"); 
 NtupleFactory_->AddFloat("electrons_IdRobustTight"); 
   
 NtupleFactory_->Add3V("tracks_in");
 NtupleFactory_->Add3V("tracks_out");   
 NtupleFactory_->Add4V("jets");      
 NtupleFactory_->AddFloat("jets_btagging");   
   
 NtupleFactory_->Add4V("met");         
 NtupleFactory_->Add4V("genJets");         
 NtupleFactory_->Add4V("genMet");         

 NtupleFactory_->Add4V("mcT1");    
 NtupleFactory_->AddFloat("mcT1_charge");    
 NtupleFactory_->Add4V("mcT2");    
 NtupleFactory_->AddFloat("mcT2_charge");    

 NtupleFactory_->Add4V("mcB1");    
 NtupleFactory_->AddFloat("mcB1_charge");    
 NtupleFactory_->Add4V("mcB2");    
 NtupleFactory_->AddFloat("mcB2_charge");   
       
 NtupleFactory_->Add4V("mcV1");         
 NtupleFactory_->AddFloat("mcV1_charge");    
 NtupleFactory_->AddFloat("mcV1_pdgId");    
 
 NtupleFactory_->Add4V("mcV2");         
 NtupleFactory_->AddFloat("mcV2_charge");    
 NtupleFactory_->AddFloat("mcV2_pdgId");  
  
 NtupleFactory_->Add4V("mcF1_fromV1");   
 NtupleFactory_->AddFloat("mcF1_fromV1_charge");    
 NtupleFactory_->AddFloat("mcF1_fromV1_pdgId");  
       
 NtupleFactory_->Add4V("mcF2_fromV1");         
 NtupleFactory_->AddFloat("mcF2_fromV1_charge");    
 NtupleFactory_->AddFloat("mcF2_fromV1_pdgId");  
 
 NtupleFactory_->Add4V("mcF1_fromV2");         
 NtupleFactory_->AddFloat("mcF1_fromV2_charge");    
 NtupleFactory_->AddFloat("mcF1_fromV2_pdgId");  
 
 NtupleFactory_->Add4V("mcF2_fromV2");         
 NtupleFactory_->AddFloat("mcF2_fromV2_charge");    
 NtupleFactory_->AddFloat("mcF2_fromV2_pdgId");  
 
}
   
   
// ------------ method called once each job just after ending the event loop  ------------
     
  void 
    SimpleNtpleTTBar::endJob() {
   
   NtupleFactory_->WriteNtuple();

    }

