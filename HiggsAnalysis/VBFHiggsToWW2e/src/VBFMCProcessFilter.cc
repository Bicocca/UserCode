// $Id: VBFMCProcessFilter.cc,v 1.4 2007/12/07 14:09:16 govoni Exp $

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFMCProcessFilter.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include <iostream>


//! ctor
VBFMCProcessFilter::VBFMCProcessFilter(const edm::ParameterSet& iConfig) :
  label_ (iConfig.getUntrackedParameter<std::string> ("moduleLabel",std::string ("source")))
{}


// ------------------------------------------------------------------------------------


//! dtor
VBFMCProcessFilter::~VBFMCProcessFilter()
{}


// ------------------------------------------------------------------------------------


//! filtering method
bool 
VBFMCProcessFilter::filter (edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Handle<edm::HepMCProduct> evtMC;
  iEvent.getByLabel(label_,evtMC);

  const HepMC::GenEvent * Evt = evtMC->GetEvent();

  int processID = Evt->signal_process_id() ;
  if (processID == 123 || processID == 124) return true ;
  //if (processID == 102) return true ;
  return false ;

}
	

