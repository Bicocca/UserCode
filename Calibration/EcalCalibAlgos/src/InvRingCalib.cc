#include <memory>
#include <math.h>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Calibration/EcalCalibAlgos/interface/InvRingCalib.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/EgammaReco/interface/ClusterShape.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "CondFormats/DataRecord/interface/EcalIntercalibConstantsRcd.h"
#include "Calibration/Tools/interface/calibXMLwriter.h"
#include "CalibCalorimetry/CaloMiscalibTools/interface/CaloMiscalibTools.h"
#include "TH2.h"
#include "TFile.h"

#include "CalibCalorimetry/CaloMiscalibTools/interface/MiscalibReaderFromXMLEcalBarrel.h"
#include "CalibCalorimetry/CaloMiscalibTools/interface/MiscalibReaderFromXMLEcalEndcap.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/ClusterShapeFwd.h"
#include "DataFormats/EgammaReco/interface/ClusterShapeFwd.h"
#include "DataFormats/EgammaReco/interface/ClusterShape.h"
//----------------------------------------------------------------
//ctor

InvRingCalib::InvRingCalib (const edm::ParameterSet& iConfig) :
  m_barrelAlCa (iConfig.getParameter<edm::InputTag>("barrelAlca")),
  m_endcapAlCa (iConfig.getParameter<edm::InputTag>("endcapAlca")) ,
  m_ElectronLabel (iConfig.getParameter<edm::InputTag>("ElectronLabel")),
  m_recoWindowSide (iConfig.getParameter<int>("recoWindowSide")),
  m_minEnergyPerCrystal (iConfig.getParameter<double>("minEnergyPerCrystal")) ,
  m_maxEnergyPerCrystal (iConfig.getParameter<double>("maxEnergyPerCrystal")) ,
  m_etaStart (iConfig.getParameter<int>("etaStart")),
  m_etaEnd (iConfig.getParameter<int>("etaEnd")),
  m_etaWidth (iConfig.getParameter<int>("etaWidth")),
  m_maxSelectedNumPerRing (iConfig.getParameter<int>("maxNumPerRing")),
  m_minCoeff (iConfig.getParameter<double>("minCoeff")),
  m_maxCoeff (iConfig.getParameter<double>("maxCoeff")),
  m_usingBlockSolver(iConfig.getParameter<int>("usingBlockSolver")),
  m_loops (iConfig.getParameter<unsigned int>("loops")),
  m_startRing (iConfig.getParameter<int>("startRing")),
  m_endRing (iConfig.getParameter<int>("endRing")),
  m_EBcoeffFile (iConfig.getParameter<std::string>("EBcoeffs")),
  m_EEcoeffFile (iConfig.getParameter<std::string>("EEcoeffs")),
  m_EEZone (iConfig.getParameter<int>("EEZone"))
{
 edm::LogInfo ("IML") << "[InvRingCalib][ctor] entering " ;
 //controls if the parameters inputed are correct
 std::cerr<<"eta start "<<m_etaStart<<std::endl<<std::endl;
 assert (m_etaStart >=-85 && m_etaStart <= 85);
 assert (m_etaEnd >= m_etaStart && m_etaEnd <= 86);
 assert (m_startRing>-1 && m_startRing<= 40);
 assert (m_endRing>=m_startRing && m_endRing<=40);
 assert(!((m_endRing - m_startRing)%m_etaWidth));
 assert ((m_etaEnd-m_etaStart)%m_etaWidth == 0);
 assert (( abs(m_EEZone)<=1));
 
 //LP CalibBlock vector instantiation
 for (int i =0; i< EBRegionNum(); ++i)
 	m_ecalCalibBlocks.push_back (EcalCalibBlock (m_etaWidth)); 
 int EEBlocks;
 if (m_EEZone ==0) EEBlocks = 2* EERegionNum();
 if (m_EEZone ==1 || m_EEZone ==-1) EEBlocks = EERegionNum();
 for (int i=0; i<EEBlocks;++i)
 	m_ecalCalibBlocks.push_back (EcalCalibBlock (m_etaWidth));
 edm::LogInfo ("IML") <<" [InvRingCalib][ctor] end of creator";
}

//---------------------------------------------------------------------------end ctor
//!destructor

InvRingCalib::~InvRingCalib ()
{
}
//----------------

//---------------------------------------------------
//!BeginOfJob
void 
InvRingCalib::beginOfJob (const edm::EventSetup& iSetup) 
{
 edm::LogInfo ("IML") << "[InvRingCalib][beginOfJob]" ;
 //gets the geometry from the event setup
 edm::ESHandle<CaloGeometry> geoHandle;
 iSetup.get<IdealGeometryRecord>().get(geoHandle);
 const CaloGeometry& geometry = *geoHandle;
 edm::LogInfo ("IML") <<"[InvRingCalib] Event Setup read";
 //fills a vector with all the cells
 m_barrelCells = geometry.getValidDetIds(DetId::Ecal, EcalBarrel);
 m_endcapCells = geometry.getValidDetIds(DetId::Ecal, EcalEndcap);
 //Defines the EB regions
 edm::LogInfo ("IML") <<"[InvRingCalib] Defining Barrel Regions";
 EBRegionDef();
 //Defines what is a ring in the EE
 edm::LogInfo ("IML") <<"[InvRingCalib] Defining endcap Rings";
 EERingDef(iSetup);
 //Defines the regions in the EE
 edm::LogInfo ("IML") <<"[InvRingCalib] Defining endcap Regions";
 EERegionDef();
 edm::LogInfo ("IML") <<"[InvRingCalib] Initializing the coeffs";
 //Sets the initial coefficients to 1.
 for (std::map<int,int>::const_iterator ring= m_xtalRing.begin();
 	ring!=m_xtalRing.end();++ring)
           m_InterRings[ring->second]=1.;	
 //Graphs to check ring, regions and so on, not needed in the final version
 TH2F EBRegion ("EBRegion","EBRegion",171,-85,86,360,1,361);
 TH2F EBRing ("EBRing","EBRing",171,-85,86,360,1,361);
 for (std::vector<DetId>::const_iterator it= m_barrelCells.begin();
       it!= m_barrelCells.end(); ++it ){
       EBDetId eb (*it);
       EBRing.Fill(eb.ieta(),eb.iphi(),m_xtalRing[it->rawId()]);
       EBRegion.Fill(eb.ieta(),eb.iphi(),m_xtalRegionId[it->rawId()]);
      }
 TH2F EEPRegion ("EEPRegion", "EEPRegion",100,1,101,100,1,101);
 TH2F EEPRing ("EEPRing", "EEPRing",100,1,101,100,1,101);
 TH2F EEPRingReg ("EEPRingReg", "EEPRingReg",100,1,101,100,1,101);
 TH2F EEMRegion ("EEMRegion", "EEMRegion",100,1,101,100,1,101);
 TH2F EEMRing ("EEMRing", "EEMRing",100,1,101,100,1,101);
 TH2F EEMRingReg ("EEMRingReg", "EEMRingReg",100,1,101,100,1,101);
 for (std::vector<DetId>::const_iterator it = m_endcapCells.begin();
      it!= m_endcapCells.end();++it)
    {
     EEDetId ee (*it);
     if (ee.zside()>0){
       EEPRegion.Fill(ee.ix(),ee.iy(),m_xtalRegionId[ee.rawId()]);
       EEPRing.Fill(ee.ix(),ee.iy(),m_xtalRing[ee.rawId()]);
       EEPRingReg.Fill(ee.ix(),ee.iy(),m_RinginRegion[ee.rawId()]);
     }
     if (ee.zside()<0){
       EEMRegion.Fill(ee.ix(),ee.iy(),m_xtalRegionId[ee.rawId()]);
       EEMRing.Fill(ee.ix(),ee.iy(),m_xtalRing[ee.rawId()]);
       EEMRingReg.Fill(ee.ix(),ee.iy(),m_RinginRegion[ee.rawId()]);
   }    
  } 
 TFile out ("EBZone.root", "recreate");
 EBRegion.Write();
 EBRing.Write();
 EEPRegion.Write();
 EEPRing.Write();
 EEPRingReg.Write();
 EEMRegion.Write();
 EEMRing.Write();
 EEMRingReg.Write();
 out.Close();
 edm::LogInfo ("IML") <<"[InvRingCalib] Start to acquire the coeffs";
 CaloMiscalibMapEcal EBmap;
 EBmap.prefillMap ();
 MiscalibReaderFromXMLEcalBarrel barrelreader (EBmap);
 if (!m_EBcoeffFile.empty()) barrelreader.parseXMLMiscalibFile (m_EBcoeffFile);
 EcalIntercalibConstants costants (EBmap.get());
 m_barrelMap = costants.getMap();
 CaloMiscalibMapEcal EEmap ;   
 EEmap.prefillMap ();
 MiscalibReaderFromXMLEcalEndcap endcapreader (EEmap);
 if (!m_EEcoeffFile.empty()) endcapreader.parseXMLMiscalibFile (m_EEcoeffFile) ;
 EcalIntercalibConstants EEcostants (EEmap.get());
 m_endcapMap = EEcostants.getMap();
}

//--------------------------------------------------------
//!startingNewLoop
void InvRingCalib::startingNewLoop (unsigned int ciclo) 
{
    edm::LogInfo ("IML") << "[InvMatrixCalibLooper][Start] entering loop " << ciclo;
    for (std::vector<EcalCalibBlock>::iterator calibBlock = m_ecalCalibBlocks.begin () ;
         calibBlock != m_ecalCalibBlocks.end () ;
         ++calibBlock)
      {
	//LP empties the energies vector, to fill DuringLoop.
        calibBlock->reset () ;
      }
   for (std::map<int,int>::const_iterator ring=m_xtalRing.begin();
         ring!=m_xtalRing.end();++ring)
	 m_RingNumOfHits[ring->second]=0;
   return ;
}

//--------------------------------------------------------
//!duringLoop
edm::EDLooper::Status InvRingCalib::duringLoop (const edm::Event& iEvent,
                          const edm::EventSetup&) 
{
 //gets the barrel recHits
 double pSubtract = 0.;
 double pTk = 0.;
 const EBRecHitCollection* barrelHitsCollection = 0;
 edm::Handle<EBRecHitCollection> barrelRecHitsHandle ;
 iEvent.getByLabel (m_barrelAlCa, barrelRecHitsHandle) ;
 if (barrelRecHitsHandle.isValid())
      barrelHitsCollection = barrelRecHitsHandle.product () ;
 else {
    edm::LogError("reading") << "[InvMatrixLooper] caught std::exception "
                << " in retrieving " << m_barrelAlCa << "\t" ;
    return  kContinue ;
  }

 //gets the endcap recHits
 const EERecHitCollection* endcapHitsCollection = 0;
 edm::Handle<EERecHitCollection> endcapRecHitsHandle ;
 iEvent.getByLabel (m_endcapAlCa, endcapRecHitsHandle) ;
 if (endcapRecHitsHandle.isValid())
         endcapHitsCollection = endcapRecHitsHandle.product () ;
 else  {
    edm::LogError("reading") << "[InvRingLooper] caught std::exception " 
              << " in retrieving " << m_endcapAlCa << "\t";
    return kContinue;
  }

 //gets the electrons
 edm::Handle<reco::PixelMatchGsfElectronCollection> pElectrons;
 iEvent.getByLabel(m_ElectronLabel,pElectrons);
 const reco::PixelMatchGsfElectronCollection * electronCollection;
 if (pElectrons.isValid())
	  electronCollection = pElectrons.product();
 else  {
	edm::LogError("reading")<<m_ElectronLabel<<"NotFound"
			<<"\t";
       return kContinue;
    }
 //loops over the electrons in the event
 for (eleIterator eleIt = electronCollection->begin();
      eleIt != electronCollection->end();
      ++eleIt )
   {
    pSubtract =0;
    pTk=0;
    //find the most energetic xtal
    DetId Max = findMaxHit (eleIt->superCluster ()->getHitsByDetId (), 
                            barrelHitsCollection, 
			    endcapHitsCollection) ;
    
    //Skips if findMaxHit failed 
    if (Max.det()==0) continue; 
    //Skips if the Max is in a region we don't want to calibrate
    if (m_xtalRegionId[Max.rawId()]==-1) continue;
    if (m_maxSelectedNumPerRing > 0 &&  
        m_RingNumOfHits [m_xtalRing[Max.rawId ()]] > m_maxSelectedNumPerRing ) continue ;
    ++m_RingNumOfHits [m_xtalRing[Max.rawId()]];
    //declares a map to be filled with the hits of the xtals around the MOX
    std::map<int , double> xtlMap;
    for (int i=0;i<m_etaWidth;++i)
         xtlMap[i]=0;
    //Gets the momentum of the track
    pTk = eleIt->trackMomentumAtVtx().R();
    if  ( Max.subdetId() == EcalBarrel  )
    {
     EBDetId EBmax = Max;
     //fills the map of the hits 
    if (fillEBMap (EBmax, barrelHitsCollection, xtlMap,
                m_xtalRegionId[Max.rawId()], pSubtract )) return kContinue ;
    }
    else 
    {
     EEDetId EEmax = Max;
     if (fillEEMap (EEmax, endcapHitsCollection, xtlMap,
                    m_xtalRegionId[Max.rawId()],pSubtract )) return kContinue ;
     //subtracts the preshower Energy deposit
     pSubtract += eleIt->superCluster()->preshowerEnergy() ;
    }
    //fills the calibBlock 
    m_ecalCalibBlocks.at(m_xtalRegionId[Max.rawId()]).Fill (xtlMap.begin(), xtlMap.end(),pTk,pSubtract) ;
  }
 return  kContinue;
}
//-----------------------------------
//end of duringLoop


//-------------------------------------
//EndOfLoop
edm::EDLooper::Status InvRingCalib::endOfLoop (const edm::EventSetup& dumb,unsigned int iCounter)
{
edm::LogInfo ("IML") << "[InvMatrixCalibLooper][endOfLoop] Start to invert the matrixes" ;
//call the autoexplaining "solve" method for every calibBlock
for (std::vector<EcalCalibBlock>::iterator calibBlock=m_ecalCalibBlocks.begin();
	calibBlock!=m_ecalCalibBlocks.end();
	++calibBlock)
		calibBlock->solve(m_usingBlockSolver,m_minCoeff,m_maxCoeff);
edm::LogInfo("IML") << "[InvRingLooper][endOfLoop] Starting to write the coeffs";
TH1F coeffDistr ("coeffdistr","coeffdistr",100 ,0.7,1.4);
TH1F coeffMap ("coeffRingMap","coeffRingMap",250,-85,165);
std::map<int,bool> flag;
for(std::map<int,int>::const_iterator it=m_xtalRing.begin();
     it!=m_xtalRing.end();++it){
        flag[it->second]=0;
	}
int ID;
for (std::vector<DetId>::const_iterator it=m_barrelCells.begin();
       it!=m_barrelCells.end();++it)
     { 
      ID= it->rawId();
      if (m_xtalRegionId[ID]==-1) continue;
      if (flag[m_xtalRing[ID]]) continue;
      flag[m_xtalRing[ID]] =1;
      m_InterRings[m_xtalRing[ID]]= m_ecalCalibBlocks.at(m_xtalRegionId[ID]).at(m_RinginRegion[ID]);
      coeffMap.Fill (m_xtalRing[ID],m_InterRings[m_xtalRing[ID]]);
      coeffDistr.Fill(m_InterRings[m_xtalRing[ID]]);
     }
for (std::vector<DetId>::const_iterator it=m_endcapCells.begin();
       it!=m_endcapCells.end();++it)
    { 
     ID= it->rawId();
     if (m_xtalRegionId[ID]==-1) continue;
     if (flag[m_xtalRing[ID]]) continue;
     flag[m_xtalRing[ID]]= 1;
     m_InterRings[m_xtalRing[ID]]= m_ecalCalibBlocks.at(m_xtalRegionId[ID]).at(m_RinginRegion[ID]);
     if (isnan(m_InterRings[m_xtalRing[ID]])){
       std::cerr<<"Il nan sta in regione "<<m_xtalRegionId[ID]<<" nell'anello "<<m_RinginRegion[ID]<<std::endl;
       }
     coeffMap.Fill (m_xtalRing[ID],m_InterRings[m_xtalRing[ID]]);
     coeffDistr.Fill(m_InterRings[m_xtalRing[ID]]);
    }
char filename[80];
sprintf(filename,"coeff%d.root",iCounter);
TFile out(filename,"recreate");    
coeffDistr.Write();
coeffMap.Write();
out.Close();
edm::LogInfo("IML")<< "[InvRingLooper][endofLoop] Finished";
if (iCounter < m_loops-1 ) return kContinue ;
else return kStop; 
}


//---------------------------------------
//LP endOfJob
void 
InvRingCalib::endOfJob ()
{

 edm::LogInfo ("IML") << "[InvMatrixCalibLooper][endOfJob] saving calib coeffs" ;
 calibXMLwriter barrelWriter(EcalBarrel);
 calibXMLwriter endcapWriter(EcalEndcap);
 for (std::vector<DetId>::const_iterator barrelIt =m_barrelCells.begin(); 
       barrelIt!=m_barrelCells.end(); 
       ++barrelIt) {
	    EBDetId eb (*barrelIt);
	    barrelWriter.writeLine(eb,m_InterRings[m_xtalRing[eb.rawId()]]*m_barrelMap[eb]);
          }
 for (std::vector<DetId>::const_iterator endcapIt = m_endcapCells.begin();
     endcapIt!=m_endcapCells.end();
     ++endcapIt) {
	  EEDetId ee (*endcapIt);
	  endcapWriter.writeLine(ee,m_InterRings[m_xtalRing[ee.rawId()]]*m_endcapMap[ee]);
	}
}


//------------------------------------//
//      definition of functions       //
//------------------------------------//
//-------------------------

bool InvRingCalib::fillEBMap (EBDetId EBmax,
	 const EcalRecHitCollection * barrelHitsCollection,
	 std::map<int, double> & EBRegionMap,
	 int EBNumberOfRegion, double & pSubtract)
{
 int curr_eta;
 int curr_phi;
 //reads the hits in a recoWindowSide^2 wide region around the MOX
 for (int ii = 0 ; ii< m_recoWindowSide ; ++ii)
  for (int ij =0 ; ij< m_recoWindowSide ; ++ij) 
  {
   curr_eta=EBmax.ieta() + ii - (m_recoWindowSide/2);
   curr_phi=EBmax.iphi() + ij - (m_recoWindowSide/2);
   //skips if the xtals matrix falls over the border
   if (abs(curr_eta)>85) continue;
   //Couples with the zero gap in the barrel eta index
   if (curr_eta * EBmax.ieta() <= 0) {if (EBmax.ieta() > 0) curr_eta--; else curr_eta++; }  // JUMP over 0
   //The following 2 couples with the ciclicity of the phiIndex
   if (curr_phi < 1) curr_phi += 360;
   if (curr_phi > 360) curr_phi -= 360;
   //checks if the detId is valid
   if(EBDetId::validDetId(curr_eta,curr_phi))
    {
     EBDetId det = EBDetId(curr_eta,curr_phi,EBDetId::ETAPHIMODE);
     int ID= det.rawId();
     //finds the hit corresponding to the cell
     EcalRecHitCollection::const_iterator curr_recHit = barrelHitsCollection->find(det) ;
     double dummy = 0;
     dummy = curr_recHit->energy () ;
     //checks if the reading of the xtal is in a sensible range
     if ( dummy < m_minEnergyPerCrystal) continue;
     if (dummy > m_maxEnergyPerCrystal) 
             {edm::LogWarning("IML")<<"[InvRingCalib] recHit bigger than maxEnergy per Crystal\n"
	            <<" EB xtal eta"<<det.ieta()<<" phi "<<det.iphi();
	      return 1;
	      }
   if (isnan(dummy)){
	edm::LogError("IML")<<"\n\n------\n"
		<<"found a nan RecHit in Region "<<m_xtalRegionId[ID]<<" ring = "<<m_RinginRegion[ID]
		<<"\n----------------------------------\n";
		return 1;
		}
    //corrects the energy with the calibration coeff of the ring
     dummy *= m_barrelMap[det];
     dummy *= m_InterRings[m_xtalRing[ID]]; 
     //sums the energy of the xtal to the appropiate ring
     if (m_xtalRegionId[ID]==EBNumberOfRegion)
       EBRegionMap[m_RinginRegion[ID]]+= dummy;
     //adds the reading to pSubtract when part of the matrix is outside the region
     else pSubtract +=dummy; 
    }
  }
return 0;
}
//----------------------------
//! Filss the EE map to be given to the calibBlock

bool InvRingCalib::fillEEMap (EEDetId EEmax,
                const EcalRecHitCollection * endcapHitsCollection,
                std::map<int,double> & EExtlMap,
                int EENumberOfRegion, double & pSubtract )
{
 int curr_x;
 int curr_y;
 for (int ii = 0 ; ii< m_recoWindowSide ; ++ii)
  for (int ij =0 ; ij< m_recoWindowSide ; ++ij) 
 {
  //Works as fillEBMap
  curr_x = EEmax.ix() - m_recoWindowSide/2 +ii;
  curr_y = EEmax.iy() - m_recoWindowSide /2 +ij;
  if(EEDetId::validDetId(curr_x,curr_y,EEmax.zside()))
  {
   EEDetId det = EEDetId(curr_x,curr_y,EEmax.zside(),EEDetId::XYMODE);
   int ID=det.rawId();
   EcalRecHitCollection::const_iterator curr_recHit = endcapHitsCollection->find(det) ;
   double dummy = curr_recHit->energy () ;
   if ( dummy < m_minEnergyPerCrystal ) continue; 
   if ( dummy > m_maxEnergyPerCrystal ) {
        edm::LogWarning("IML")<<"[InvRingCalib] recHit bigger than maxEnergy per Crystal\n"
	   <<"EE xtal "<<det.ix()<<" "<<det.iy()<<"  Energy= "<< dummy;
        return 1; 
   }
   if (isnan(dummy)){
	edm::LogError("IML")<<"\n\n------\n"
		<<"found a nan RecHit in Region "<<m_xtalRegionId[ID]<<" ring = "<<m_RinginRegion[ID]
		<<"\n-----------------------------\n\n";
	return 1;	
   }
   dummy *= m_endcapMap[det];
   dummy *= m_InterRings[m_xtalRing[ID]];
   if (m_xtalRegionId[ID]==EENumberOfRegion)
      EExtlMap[m_RinginRegion[ID]] += dummy;
   else pSubtract +=dummy; 
  }
 }
return 0;
}

//!EE ring definition
void InvRingCalib::EERingDef(const edm::EventSetup& iSetup)  
{
 //Gets the Handle for the geometry from the eventSetup
 edm::ESHandle<CaloGeometry> geoHandle;
 iSetup.get<IdealGeometryRecord>().get(geoHandle);
 //Gets the geometry of the endcap
 const CaloGeometry& geometry = *geoHandle;
 const CaloSubdetectorGeometry *endcapGeometry = geometry.getSubdetectorGeometry(DetId::Ecal, EcalEndcap);
 //for every xtal gets the position Vector and the phi position
 for (std::vector<DetId>::const_iterator endcapIt = m_endcapCells.begin();
    endcapIt!=m_endcapCells.end();
    ++endcapIt) {
     const CaloCellGeometry *cellGeometry = endcapGeometry->getGeometry(*endcapIt);
     m_cellPos[endcapIt->rawId()] = cellGeometry->getPosition();
     m_cellPhi[endcapIt->rawId()] = cellGeometry->getPosition().phi();
  }
 //takes the first 39 xtals at a fixed y varying the x coordinate and saves their eta coordinate 
 float eta_ring[39];
 for (int ring=0; ring<39; ring++) 
	if (EEDetId::validDetId(ring+1,50,1)){  
	  EEDetId det = EEDetId (ring+1,50,1,EEDetId::XYMODE);
          eta_ring[ring]=m_cellPos[det.rawId()].eta();
	  }
 //defines the bonduary of the rings as the average eta of a xtal
 double etaBonduary[40];
 etaBonduary[0]=1.485;
 etaBonduary[39]=4.0;
 for (int ring=1; ring<39; ++ring)
       etaBonduary[ring]=(eta_ring[ring]+eta_ring[ring-1])/2.;
 //assign to each xtal a ring number
 int CRing;
 for (int ring=0; ring<39; ring++) 
   for (std::vector<DetId>::const_iterator endcapIt=m_endcapCells.begin();
        endcapIt!=m_endcapCells.end();++endcapIt){
     if (fabs(m_cellPos[endcapIt->rawId()].eta())>etaBonduary[ring] &&
         fabs(m_cellPos[endcapIt->rawId()].eta())<etaBonduary[ring+1])
	  {
	      EEDetId ee(*endcapIt);
	      if (ee.zside()>0) CRing=ring + 86; 
	      else CRing = ring + 125;
              m_xtalRing[endcapIt->rawId()]=CRing;
	  }    
      }
 return;
}

//
//!Gives the Id of the region given the id of the xtal
int InvRingCalib::EERegId( int id) 
{
   int reg;
   int ring;
   EEDetId ee (id);
  //sets the reg to -1 if the ring doesn't exist or is outside the region of interest 
   if (m_xtalRing[id] == -1) return -1;
  //avoid the calibration in the wrong zside
   if (m_EEZone == 1 ){
   if (ee.zside()<0) return -1;
   ring = m_xtalRing[id]-86;
   if(ring >=m_endRing) return -1;
   if (ring<m_startRing) return -1;
   reg = (ring -m_startRing) / m_etaWidth;
   return reg;
   }
   if (m_EEZone == -1){
   if (ee.zside()>0) return -1;
   ring = m_xtalRing[id] -125;
   if(ring >=m_endRing) return -1;
   if (ring<m_startRing) return -1;
   reg = (ring -m_startRing) / m_etaWidth;
   return reg;
   }
   if (ee.zside()>0) ring=m_xtalRing[id]-86;
   else ring = m_xtalRing[id]-125;
   if(ring >=m_endRing) return -1;
   if (ring<m_startRing) return -1;
   reg = (ring -m_startRing) / m_etaWidth;
   return reg;
}
//----------------------------------------
//!Loops over all the endcap xtals and sets for each xtal the value of the region
//!the xtal is in, and the ringNumber inside the region 
void InvRingCalib::EERegionDef ()
{ 
int reg;
int start;
for (std::vector<DetId>::const_iterator endcapIt=m_endcapCells.begin();
     endcapIt!=m_endcapCells.end();++endcapIt){
      EEDetId ee(*endcapIt);
      reg = EERegId(endcapIt->rawId());
      //If the ring is not of interest saves only the region Id(-1)
      if(reg==-1) 
         m_xtalRegionId[endcapIt->rawId()]=reg;
      //sums the number of region in EB or EB+EE to have different regionsId in different regions 
      else {
      if (ee.zside()>0){
           reg += EBRegionNum();
	   start=m_startRing+86;
	   }
      else {
          reg += EBRegionNum()+EERegionNum();
	  start=m_startRing+125;
	  }
      m_xtalRegionId[endcapIt->rawId()]=reg;
      m_RinginRegion[endcapIt->rawId()]=(m_xtalRing[endcapIt->rawId()]-start)%m_etaWidth;
   }
  }
}

//----------------------------------------
//!Number of Regions in EE 
inline int InvRingCalib::EERegionNum () const 
{
  return ((m_endRing - m_startRing)/m_etaWidth);
}


//! number of Ring in EB
inline int InvRingCalib::EBRegionNum () const 
{
  return ((m_etaEnd - m_etaStart )/m_etaWidth); 
}
//! gives the region Id given ieta
int InvRingCalib::EBRegId(const int ieta) const
{
 int reg;
 if (ieta<m_etaStart || ieta>=m_etaEnd) return -1;
 else reg = (ieta - m_etaStart)/m_etaWidth;
 return reg;
}

//----------------------------------------
//EB Region Definition
void InvRingCalib::EBRegionDef()
{
  for (std::vector<DetId>::const_iterator it=m_barrelCells.begin();
  	it!=m_barrelCells.end();++it)
  {
    EBDetId eb (*it);
    m_xtalRing[eb.rawId()] = eb.ieta() ;
    m_xtalRegionId[eb.rawId()] = EBRegId (eb.ieta()); 
    if (m_xtalRegionId[eb.rawId()]==-1) continue;
    m_RinginRegion[eb.rawId()] = (eb.ieta() - m_etaStart)% m_etaWidth; 
  }
}

//---------------------
//!Find the most energetic Xtal
DetId  InvRingCalib::findMaxHit (const std::vector<DetId> & v1,
				    const EBRecHitCollection* EBhits,
				    const EERecHitCollection* EEhits) 
{
 double currEnergy = 0.;
 //creates an empty DetId 
 DetId maxHit;
 //Loops over the vector of the recHits
 for (std::vector<DetId>::const_iterator idsIt = v1.begin () ; 
      idsIt != v1.end () ; ++idsIt)
   {
    if (idsIt->subdetId () == EcalBarrel) 
       {              
         EBRecHitCollection::const_iterator itrechit;
         itrechit = EBhits->find (*idsIt) ;
	 //not really understood why this should happen, but it happens
         if (itrechit == EBhits->end () )
           {
            edm::LogWarning("IML") <<"max hit not found";
            continue;
           }
	 //If the energy is greater than the currently stored energy sets maxHits to the current recHit
         if (itrechit->energy () > currEnergy)
           {
             currEnergy = itrechit->energy () ;
             maxHit= *idsIt ;
           }
       } //barrel part ends
    else 
       {     
	 //as the barrel part
         EERecHitCollection::const_iterator itrechit;
         itrechit = EEhits->find (*idsIt) ;
         if (itrechit == EEhits->end () )
           {
             edm::LogWarning("IML") <<"max hit not found";
             continue;
           }
              
         if (itrechit->energy () > currEnergy)
           {
            currEnergy=itrechit->energy ();
            maxHit= *idsIt;
           }
       } //ends the endcap part
    } //end of the loop over the detId
 return maxHit;
}
