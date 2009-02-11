// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDAssociation.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFEleTrackerIsolationAlgo.h"

#include "TTree.h"
#include <TLorentzVector.h>
#include <TClonesArray.h>

//
// class decleration
//

class SimpleNtple : public edm::EDAnalyzer {
 public:
  explicit SimpleNtple(const edm::ParameterSet&);
  ~SimpleNtple();

  typedef edm::View<reco::Track> trackCollection ;
  typedef math::XYZTLorentzVector LorentzVector ;
  
 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  void Init();
  void FillEle (const edm::Event&, const edm::EventSetup&);
  void FillMu (const edm::Event&, const edm::EventSetup&);
  void FillMet (const edm::Event&, const edm::EventSetup&);
  void FillTagJet (const edm::Event&, const edm::EventSetup&);
  void FillJet (const edm::Event&, const edm::EventSetup&);
  void FillTracks(const edm::Event&, const edm::EventSetup&);

  void setMomentum (TLorentzVector &myvector, const LorentzVector & mom) ;
  
  // ----------member data ---------------------------
  TTree *mytree_;
  //electrons;
  int nEle;
  float IsolEleSumPt[30],IsolEleNTracks[30];
  int EleId[30];
  //muons
  int nMu;
  float IsolMuSumPt[30],IsolMuNTracks[30];
  float MinvTags;
  //other jets

  TClonesArray * m_tagJets ;
  TClonesArray * m_otherJets ;
  TClonesArray * m_electrons ;
  TClonesArray * m_muons ;
  TClonesArray * m_MET ;
  TClonesArray * m_tracks ;

  TLorentzVector myvector ;

  //! isolation algorithm
  VBFEleTrackerIsolationAlgo m_tkIsolationAlgo ;
  

  edm::InputTag m_eleIDPTDRLooseInputTag ;
  edm::InputTag m_eleIDPTDRMediumInputTag ;
  edm::InputTag m_eleIDPTDRTightInputTag ;
  edm::InputTag TracksTag_;
  edm::InputTag EleTag_;
  edm::InputTag IsolEleTag_;
  edm::InputTag MuTag_;
  edm::InputTag IsolMuTag_;
  edm::InputTag MetTag_;
  edm::InputTag TagJetTag_;
  edm::InputTag JetTag_;
  
};