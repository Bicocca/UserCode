// $Id: VBFUtils.cc,v 1.9 2008/02/26 10:27:17 tancini Exp $

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFUtils.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include <iostream>

void setMomentum (TLorentzVector & myvector, const reco::Candidate & gen)
{
    myvector.SetPx (gen.px ()) ;
    myvector.SetPy (gen.py ()) ;
    myvector.SetPz (gen.pz ()) ;
    myvector.SetE (gen.energy ()) ;
}


// --------------------------------------------------------------------


std::pair<VBFjetIt,VBFjetIt>	
findTagJets (VBFjetIt begin, VBFjetIt end,
             double jetPtMin, double jetEtaMax) 
{

  std::pair<VBFjetIt,VBFjetIt> tagJets (begin,begin) ;
  double maxInvMass = 0. ;

  //PG find the tagging jets

  //PG first loop over jets
  for (VBFjetIt firstJet = begin ; 
       firstJet != end ; 
       ++firstJet ) 
    {
      if (firstJet->pt () < jetPtMin || 
          fabs (firstJet->eta ()) > jetEtaMax) continue ;
      math::XYZTLorentzVector firstLV = firstJet->p4 () ;
      //PG second loop over jets
      for (VBFjetIt secondJet = firstJet + 1 ; 
           secondJet != end ; 
           ++secondJet ) 
        {
          if (secondJet->pt () < jetPtMin || 
              fabs (secondJet->eta ()) > jetEtaMax) continue ;
          math::XYZTLorentzVector sumLV = secondJet->p4 () + firstLV ;
          if (sumLV.M () > maxInvMass)
            {
              maxInvMass = sumLV.M () ;
              tagJets.first = firstJet ;
              tagJets.second = secondJet ;
            }
        } //PG second loop over jets
    } //PG first loop over jets

  return tagJets ;
}

// --------------------------------------------------------------------


std::pair<VBFjetIt,VBFjetIt>
findMaxPtJetsPair (VBFjetIt begin, VBFjetIt end,
             double jetPtMin, double jetEtaMax)
{
  std::pair<VBFjetIt,VBFjetIt> tagJets (begin, begin) ; 

  double ptMax1 = 0;
  double ptMax2 = 0;

  VBFjetIt myJet1;
  VBFjetIt myJet2;
  for (VBFjetIt Jet = begin ;
       Jet != end ;
       ++Jet)
    {
      if (Jet->p4().Pt() > ptMax1)
	{
	  myJet1 = Jet;
	  myJet2 = myJet1;
	  ptMax2 = ptMax1;
	  ptMax1 = Jet->p4().Pt() ;

        } 
      else if (Jet->p4().Pt() > ptMax2)
	{
         myJet2 = Jet;
         ptMax2 = Jet->p4().Pt() ;
	}
    }

  tagJets.first = myJet1;
  tagJets.second = myJet2 ;

  return tagJets ;

    }

// --------------------------------------------------------------------------------

double deltaPhi (double phi1, double phi2)
{

  double deltaphi=fabs(phi1-phi2);
  if (deltaphi > 6.283185308) deltaphi -= 6.283185308;
  if (deltaphi > 3.141592654) deltaphi = 6.283185308-deltaphi;
  return deltaphi;
}
