#ifndef EleCalibSelector_h
#define EleCalibSelector_h

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventPrincipal.h" 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"

#include <functional>
#include <vector>
#include <map>


class EleCalibSelector{

 public:
  
 typedef reco::PixelMatchGsfElectronCollection collection ;
 typedef reco::PixelMatchGsfElectronRef electron ;
 typedef std::vector<const reco::PixelMatchGsfElectron *> container;
 typedef container::const_iterator const_iterator;
 
 //! ctor
 EleCalibSelector (const edm::ParameterSet& iConfig) ;
 //!dtor
 ~EleCalibSelector () ;
 
 const_iterator begin() const { return selected_.begin(); }
 const_iterator end() const { return selected_.end(); }
  
 void select (edm::Handle<collection>, const edm::Event&, const edm::EventSetup&) ;
 
 private:

 container selected_ ; //! the selected collection

 edm::ESHandle<CaloTopology> theCaloTopology;   

 DetId findMaxHit (const std::vector<DetId> & v1,
		   const EBRecHitCollection* EBhits,
		   const EERecHitCollection* EEhits);
 
 double EnergyNxN(const std::vector<DetId> & vNxN,
		  const EBRecHitCollection* EBhits,
		  const EERecHitCollection* EEhits);
 
 double ESCOPinMin_, ESeedOPoutMin_, PinMPoutOPinMin_, E5x5OPoutMin_, E3x3OPinMin_ ;
 double ESCOPinMax_, ESeedOPoutMax_, PinMPoutOPinMax_, E5x5OPoutMax_, E3x3OPinMax_ ;
 edm::InputTag EBrecHitLabel_;
 edm::InputTag EErecHitLabel_;
};  

#endif


