#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeContent.h"
#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeUtils.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <iostream>
#include <string>
#include <boost/foreach.hpp>

#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile2D.h"
#include "TFile.h"
#include "TVector3.h"

#define PHI_MIN -3.1416
#define PHI_MAX +3.1416
#define PHI_BIN 360

#define ETA_MIN -1.5
#define ETA_MAX +1.5
#define ETA_BIN 170

#define X_BIN 500
#define X_MIN -800
#define X_MAX +800

#define Y_BIN 500
#define Y_MIN -800
#define Y_MAX +800

#define P_BIN 500
#define P_MIN 0
#define P_MAX 1000

#define E_BIN 1000
#define E_MIN 0
#define E_MAX 250



int main (int argc, char** argv)
{
  std::string fileName (argv[1]) ;
  boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile (fileName) ;
  boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet () ;
  std::cout << parameterSet->dump () << std::endl ; //PG for testing
  
  // edm::ParameterSet subPSetSelections =  parameterSet->getParameter<edm::ParameterSet> ("Selections") ;
  edm::ParameterSet subPSetInput = parameterSet->getParameter<edm::ParameterSet> ("inputNtuples") ;
  std::vector<std::string> inputFiles = subPSetInput.getParameter<std::vector<std::string> > ("inputFiles") ;

  TChain *chain = new TChain ("EcalCosmicsAnalysis") ;
  EcalCosmicsTreeContent treeVars ; 
  setBranchAddresses (chain, treeVars) ;


  // input files
  for (std::vector<std::string>::const_iterator listIt = inputFiles.begin () ;
       listIt != inputFiles.end () ; ++listIt)
  {
    std::cout << *listIt << " " << std::endl ;
    chain->Add (listIt->c_str ()) ;
  }

  int nEntries = chain->GetEntries () ;
  std::cout << "FOUND " << nEntries << " ENTRIES\n" ;    
  // input files


  // output file
  std::string outputRootName = "CRAFT_SCdistr.root" ;
  // output file  



  // output distributions
  TH2F SCoccupancy ("SCoccupancy", "SCoccupancy", PHI_BIN, PHI_MIN, PHI_MAX, ETA_BIN, ETA_MIN, ETA_MAX) ;
  
  TH1F SCenergy ("SCenergy", "SCenergy", E_BIN, E_MIN, E_MAX) ;
  TProfile2D SCenergy_ETAvsPHI ("SCenergy_ETAvsPHI", "SCenergy_ETAvsPHI", PHI_BIN, PHI_MIN, PHI_MAX, ETA_BIN, ETA_MIN, ETA_MAX) ;
  // output distributions



  // loop over entries
  for (int entry = 0; entry < nEntries; ++entry)
  {
    chain->GetEntry (entry) ;

    for (int SCindex = 0; SCindex < treeVars.nSuperClusters; ++SCindex)
    {
      SCoccupancy.Fill (treeVars.superClusterPhi[SCindex], treeVars.superClusterEta[SCindex]) ;
      
      SCenergy.Fill (treeVars.superClusterRawEnergy[SCindex]) ;
      SCenergy_ETAvsPHI.Fill (treeVars.superClusterPhi[SCindex], treeVars.superClusterEta[SCindex], treeVars.superClusterRawEnergy[SCindex]) ;
    }
  
  
  } //PG loop over entries

  TFile saving (outputRootName.c_str (),"recreate") ;
  saving.cd () ;  
  
  SCoccupancy.Write () ;
  SCenergy.Write () ;
  SCenergy_ETAvsPHI.Write () ;
  
  saving.Close () ;
  
  return 0 ;
}


