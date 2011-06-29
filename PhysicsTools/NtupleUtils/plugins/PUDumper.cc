#include "PhysicsTools/NtupleUtils/plugins/PUDumper.h"






//! ctor
PUDumper::PUDumper(const edm::ParameterSet& iConfig)
{
  MCPileupTag_ = iConfig.getParameter<edm::InputTag>("MCPileupTag");
  mcFlag_ = iConfig.getUntrackedParameter<bool>("mcFlag",true);  
  
  // create histograms
  edm::Service<TFileService> fs;
  
  h_nPU    = fs -> make<TH1F>("nPU",   "nPU",   50,-0.5,49.5);
  h_nPUit  = fs -> make<TH1F>("nPUit", "nPUit", 50,-0.5,49.5);
  h_nPUoot = fs -> make<TH1F>("nPUoot","nPUoot",50,-0.5,49.5);
}

// ----------------------------------------------------------------






//! dtor
PUDumper::~PUDumper()
{}

// ----------------------------------------------------------------






//! loop over the reco particles and count leptons
void PUDumper::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // skip if running on data
  if( !mcFlag_ ) return;
  
  
  // get the PU collection
  edm::Handle<std::vector<PileupSummaryInfo> > PupInfo;
  iEvent.getByLabel(MCPileupTag_,PupInfo);
  
  
  // nPU
  int nPU = 0;
  int nPUit = 0;
  int nPUoot = 0;
  
  
  // loop on BX
  std::vector<PileupSummaryInfo>::const_iterator PVI;
  for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI)
  {
    //std::cout << "PUDumper::BX: " << PVI->getBunchCrossing() << "   nPU: " << PVI->getPU_NumInteractions() << std::endl;
    
    // in-time pileup
    if( PVI->getBunchCrossing() == 0 )
    {
      nPU    += PVI->getPU_NumInteractions();
      nPUit  += PVI->getPU_NumInteractions();
    }
    
    // out-of-time pileup
    else
    {
      nPU    += PVI->getPU_NumInteractions();
      nPUoot += PVI->getPU_NumInteractions();
    }
  } // loop on BX                      
  
  
  // fill histograms
  h_nPU    -> Fill(nPU);
  h_nPUit  -> Fill(nPUit);
  h_nPUoot -> Fill(nPUoot);
}

DEFINE_FWK_MODULE(PUDumper);