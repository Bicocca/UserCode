#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <stdexcept>
#include <fstream>

#include "TROOT.h"
#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TClonesArray.h"
#include "TString.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3F.h"
#include "TText.h"
#include "TLegend.h"
#include "TF1.h"
#include "TApplication.h"
#include "TStyle.h"
#include "CRUZET/Calibration/interface/CRUtils.h"


int main (int argc, char** argv)
{
	gROOT->SetStyle("Plain"); 
	
	// Tree construction
	TChain * chain = new TChain ("EcalCosmicsAnalysis") ;
// 	chain->Add ("~/public/MuonTreeLungTestMatrix_43439_*.root") ;
	chain->Add ("/tmp/snidero/EcalCosmicsTree50908.tree.root") ;
//       chain->Add ("/afs/cern.ch/user/s/snidero/CMSSW_2_0_9/src/CRUZET/Calibration/bin/EcalCosmicsTree-50908.tree.root") ;
	 
	EcalCosmicsAnalysisVariables treeVars ;
	
	chain->SetBranchAddress ("runId", &treeVars.runId) ;                           
	chain->SetBranchAddress ("eventId", &treeVars.eventId) ;                       
	chain->SetBranchAddress ("timeStampLow", &treeVars.timeStampLow) ;             
	chain->SetBranchAddress ("timeStampHigh", &treeVars.timeStampHigh) ;           
	chain->SetBranchAddress ("isECALL1", &treeVars.isECALL1) ;                     
	chain->SetBranchAddress ("isHCALL1", &treeVars.isHCALL1) ;                     
	chain->SetBranchAddress ("isDTL1", &treeVars.isDTL1) ;                         
	chain->SetBranchAddress ("isRPCL1", &treeVars.isRPCL1) ;                       
	chain->SetBranchAddress ("isCSCL1", &treeVars.isCSCL1) ;                       
	chain->SetBranchAddress ("nClusters", &treeVars.nCosmicClusters) ;       
	chain->SetBranchAddress ("clusterEnergy", treeVars.cosmicClusterEnergy) ;
	chain->SetBranchAddress ("clusterE1", treeVars.cosmicClusterE1) ;        
	chain->SetBranchAddress ("clusterE2", treeVars.cosmicClusterE2) ;        
	chain->SetBranchAddress ("clusterE9", treeVars.cosmicClusterE9) ;        
	chain->SetBranchAddress ("clusterE25", treeVars.cosmicClusterE25) ;      
	chain->SetBranchAddress ("clusterTime", treeVars.cosmicClusterTime) ;    
	chain->SetBranchAddress ("clusterEta", treeVars.cosmicClusterEta) ;      
	chain->SetBranchAddress ("clusterPhi", treeVars.cosmicClusterPhi) ;      
	chain->SetBranchAddress ("clusterXtals", treeVars.cosmicClusterXtals) ;  
	chain->SetBranchAddress ("clusterXtalsAbove3Sigma", treeVars.cosmicClusterXtalsAbove3Sigma) ;    
	chain->SetBranchAddress ("clusterMaxId", treeVars.cosmicClusterMaxId) ;      
	chain->SetBranchAddress ("cluster2ndId", treeVars.cosmicCluster2ndId) ;      
	chain->SetBranchAddress ("nRecoMuons", &treeVars.nRecoMuons) ;                     
	chain->SetBranchAddress ("muonPt", treeVars.muonPt) ;                              
	chain->SetBranchAddress ("muonEta", treeVars.muonEta) ;                            
	chain->SetBranchAddress ("muonPhi", treeVars.muonPhi) ;                            
	chain->SetBranchAddress ("muonNChi2", treeVars.muonNChi2) ;                        
	chain->SetBranchAddress ("muonNDof", treeVars.muonNDof) ;                          
	chain->SetBranchAddress ("muonNHits", treeVars.muonNHits) ;                        
	chain->SetBranchAddress ("muonCharge", treeVars.muonCharge) ;                      
	chain->SetBranchAddress ("muonQOverP", treeVars.muonQOverP) ;                      
	chain->SetBranchAddress ("muond0", treeVars.muond0) ;                              
	chain->SetBranchAddress ("muondz", treeVars.muondz) ;                              
	chain->SetBranchAddress ("muonTkAtEcalEta", treeVars.muonTkAtEcalEta) ;            
	chain->SetBranchAddress ("muonTkAtEcalPhi", treeVars.muonTkAtEcalPhi) ;            
	chain->SetBranchAddress ("muonTkAtHcalEta", treeVars.muonTkAtHcalEta) ;            
	chain->SetBranchAddress ("muonTkAtHcalPhi", treeVars.muonTkAtHcalPhi) ;            
	chain->SetBranchAddress ("muonEcalEnergy3x3", treeVars.muonEcalEnergy3x3) ;        
	chain->SetBranchAddress ("muonEcalEnergy5x5", treeVars.muonEcalEnergy5x5) ;        
	chain->SetBranchAddress ("muonEcalEnergyCrossed", treeVars.muonEcalEnergyCrossed) ;
	chain->SetBranchAddress ("muonHcalEnergy3x3", treeVars.muonHcalEnergy3x3) ;        
	chain->SetBranchAddress ("muonHcalEnergyCrossed", treeVars.muonHcalEnergyCrossed) ;
	chain->SetBranchAddress ("muonNCrossedEcalDetId", treeVars.muonNCrossedEcalDetId) ;
	chain->SetBranchAddress ("muonMaxEneEcalDetIdCrossed", treeVars.muonMaxEneEcalDetIdCrossed) ; 
	
	chain->SetBranchAddress ("muonTkLengthInEcalDetail", treeVars.muonTkLengthInEcalDetail);
	chain->SetBranchAddress ("muonMomentumX", treeVars.muonMomentumX) ;
	chain->SetBranchAddress ("muonMomentumY", treeVars.muonMomentumY) ; 
	chain->SetBranchAddress ("muonMomentumZ", treeVars.muonMomentumZ) ;
	
	// declare variables 
	int nEvents = (int) chain->GetEntries();
	int nClusters = 0;
	std::cout << "events " << nEvents << std::endl;
	std::cout << " " << std::endl;
	
	#define PI 3.14159265
	const double deg = PI/180;  // 1 deg in radians

	TH1F dEondX("dEondX", "dEondX", 90, 0., 0.07);		 
	TH1F Angle("Angle", "Angle", 180, 0., PI);
	TH1F Length("Length","Length",100,-2,40); 	
	 //
	TH2F Occupancy("Occupancy","Occupancy",360,-3.14,3.14,170,-1.47,1.47); 

	// define angle intervals
	int AngleInterval = 5; 
	double step = 1; //superposition 1=no sup, 0.5=one half of superpos
	int nIntervals = (int)(90./AngleInterval/step);
	std::cout << "n. of intervals: "<< nIntervals << std::endl; 
	std::cout << " " << std::endl;	

	// creating array of angles = medium point of each interval
	double IntervalMeanAngle[50];
	 
	// creating array of peaks dEondX = mean of each gaussian fit
	double IntervaldEondXPeakBottom[50];
	double IntervaldEondXPeakErrorBottom[50];
	double IntervaldEondXPeakTop[50];
	double IntervaldEondXPeakErrorTop[50];
	double IntervaldEondXPeakTBDiff[50];
	double IntervaldEondXPeakErrorTBDiff[50];
	double IntervaldEondXPeakTBRatio[50];
	double IntervaldEondXPeakErrorTBRatio[50];
	
	// create vector of histos
	std::vector <TH1F*> HistodEondXTop;		
	std::vector <TH1F*> HistodEondXBottom;
	std::vector <TH2F*> HistoOccupancyTop;	
	std::vector <TH2F*> HistoOccupancyBottom;		
	 
	// create histos with different names and put them into vector && fill angle array               
	for(int iInterval = 0 ; iInterval < nIntervals ; ++iInterval)
	{  	
		char number[80];
		
		sprintf (number, "dEondXTop_%d", iInterval );
		TH1F* tempdEondXTop = new TH1F(number, number, 70, 0., 0.07);
		HistodEondXTop.push_back(tempdEondXTop);
		
		sprintf (number, "dEondXBottom_%d", iInterval );
		TH1F* tempdEondXBottom = new TH1F(number, number, 70, 0., 0.07);
		HistodEondXBottom.push_back(tempdEondXBottom);		
		
		sprintf (number, "OccupancyTop_%d", iInterval );
		TH2F* tempOccupancyTop = new TH2F(number, number, 360, -3.14, 3.14, 170, -1.47, 1.47);
		HistoOccupancyTop.push_back(tempOccupancyTop);
		
		sprintf (number, "OccupancyBottom_%d", iInterval );
		TH2F* tempOccupancyBottom = new TH2F(number, number, 360, -3.14, 3.14, 170, -1.47, 1.47);
		HistoOccupancyBottom.push_back(tempOccupancyBottom);
		
		IntervalMeanAngle[iInterval] = (iInterval*step + iInterval*step +1 )*AngleInterval/2 ;
		std::cout << iInterval*AngleInterval*step << " - " << (iInterval*step+1)*AngleInterval << " ;  interv mean:" << (iInterval*step + iInterval*step +1 )*AngleInterval/2 << std::endl ;
   }
	
	//PG loop over entries
	for (int iEvent = 0 ; iEvent < nEvents ; ++iEvent)   
	{
		if(iEvent%10000 == 0) std::cout << "event n. " << iEvent << std::endl;
			
		chain->GetEntry (iEvent);
		
		nClusters =  treeVars.nCosmicClusters; 
		
		// loop over entry cluster
		for (int iCluster = 0 ; iCluster < nClusters ; ++iCluster)   
		{
			// SELECIONS: length>0
// 			if (treeVars.muonTkLengthInEcalDetail[iCluster] < 20 || treeVars.muonTkLengthInEcalDetail[iCluster] > 30) continue; 
			if (treeVars.muonTkLengthInEcalDetail[iCluster] < 1) continue; 
				
			// get directions
			TVector3 SC0_pos (0., 0., 0.) ;
			setVectorOnECAL (SC0_pos, treeVars.cosmicClusterEta[iCluster], treeVars.cosmicClusterPhi[iCluster]);
			
			TVector3 MuonDir (treeVars.muonMomentumX[iCluster], treeVars.muonMomentumY[iCluster], treeVars.muonMomentumZ[iCluster]);
			
			double angle;
			angle = fabs( MuonDir.Angle( SC0_pos ) );
			
			// fill histos 
			dEondX.Fill( treeVars.cosmicClusterEnergy[iCluster] / treeVars.muonTkLengthInEcalDetail[iCluster] ); 
			Angle.Fill(angle);
			Length.Fill(treeVars.muonTkLengthInEcalDetail[iCluster]);
			Occupancy.Fill(treeVars.cosmicClusterPhi[iCluster], treeVars.cosmicClusterEta[iCluster]);   
			
			for(int iInterval = 0 ; iInterval < nIntervals ; ++iInterval)
			{
				if( !( angle > iInterval*step*AngleInterval*deg  &&  angle < (iInterval*step+ 1)*AngleInterval*deg )  &&  
					!( angle > (180-(iInterval*step+1)*AngleInterval)*deg  &&  angle < (180-iInterval*step*AngleInterval)*deg ) ) continue;
				
				if( treeVars.cosmicClusterPhi[iCluster] > 0 )
				{	
					HistodEondXTop.at(iInterval)->Fill(treeVars.cosmicClusterEnergy[iCluster] / treeVars.muonTkLengthInEcalDetail[iCluster]);
					HistoOccupancyTop.at(iInterval)->Fill(treeVars.cosmicClusterPhi[iCluster], treeVars.cosmicClusterEta[iCluster]);
				}
				else
				{
					HistodEondXBottom.at(iInterval)->Fill(treeVars.cosmicClusterEnergy[iCluster] / treeVars.muonTkLengthInEcalDetail[iCluster]);  
					HistoOccupancyBottom.at(iInterval)->Fill(treeVars.cosmicClusterPhi[iCluster], treeVars.cosmicClusterEta[iCluster]);
				}		
			}
		}// loop over entry cluster
	}//PG loop over entries 
	
	double entries=0;
	
	// get the peak of dEondX
	TF1 * gaussianast = new TF1("gaussianast","gaus", 0., 0.05);
	gaussianast->SetLineColor(kBlue);
	TF1 * gaussianand = new TF1("gaussianand","gaus", 0., 0.05);
	gaussianand->SetLineColor(kRed);	
	
	for(int iInterval = 0 ; iInterval < nIntervals ; ++iInterval) 
	{  
		// fit histos
		gaussianast->SetRange(HistodEondXTop.at(iInterval)->GetMean() - 1.1*HistodEondXTop.at(iInterval)->GetRMS(), HistodEondXTop.at(iInterval)->GetMean() - 0.2*HistodEondXTop.at(iInterval)->GetRMS() );			
		gaussianast->SetParameters( 100, HistodEondXTop.at(iInterval)->GetMean(), HistodEondXTop.at(iInterval)->GetRMS() );
 		HistodEondXTop.at(iInterval)->Fit("gaussianast","R");

		gaussianand->SetRange(gaussianast->GetParameter(1) - 1.5*gaussianast->GetParameter(2), gaussianast->GetParameter(1) + 0.7*gaussianast->GetParameter(2) );	
		gaussianand->SetParameters( gaussianast->GetParameter(0), gaussianast->GetParameter(1), gaussianast->GetParameter(2) );
		HistodEondXTop.at(iInterval)->Fit("gaussianand","R+");
		
		IntervaldEondXPeakTop[iInterval] = gaussianand->GetParameter(1);
		IntervaldEondXPeakErrorTop[iInterval] = gaussianand->GetParError(1);
		
		gaussianast->SetRange(HistodEondXBottom.at(iInterval)->GetMean() - 1.1*HistodEondXBottom.at(iInterval)->GetRMS(), HistodEondXBottom.at(iInterval)->GetMean() - 0.2*HistodEondXBottom.at(iInterval)->GetRMS() );	
		gaussianast->SetParameters( 100, HistodEondXBottom.at(iInterval)->GetMean(), HistodEondXBottom.at(iInterval)->GetRMS() );
		HistodEondXBottom.at(iInterval)->Fit("gaussianast","R");
		
		gaussianand->SetRange(gaussianast->GetParameter(1) - 1.5*gaussianast->GetParameter(2), gaussianast->GetParameter(1) + 0.7*gaussianast->GetParameter(2) );	
		gaussianand->SetParameters( gaussianast->GetParameter(0), gaussianast->GetParameter(1), gaussianast->GetParameter(2) );		
		HistodEondXBottom.at(iInterval)->Fit("gaussianand","R+");
		
		IntervaldEondXPeakBottom[iInterval] = gaussianand->GetParameter(1);
		IntervaldEondXPeakErrorBottom[iInterval] = gaussianand->GetParError(1);
		
		IntervaldEondXPeakTBDiff[iInterval]=IntervaldEondXPeakTop[iInterval]  - IntervaldEondXPeakBottom[iInterval] ;
		IntervaldEondXPeakErrorTBDiff[iInterval]=sqrt( pow(IntervaldEondXPeakErrorBottom[iInterval],2) + pow(IntervaldEondXPeakErrorTop[iInterval],2) );
		
		IntervaldEondXPeakTBRatio[iInterval]=IntervaldEondXPeakTop[iInterval]/IntervaldEondXPeakBottom[iInterval] ;
		IntervaldEondXPeakErrorTBRatio[iInterval]=1/IntervaldEondXPeakBottom[iInterval] * sqrt( pow(IntervaldEondXPeakErrorBottom[iInterval],2) + pow(IntervaldEondXPeakTop[iInterval]/IntervaldEondXPeakBottom[iInterval]*IntervaldEondXPeakErrorBottom[iInterval], 2) );

		// histos entries control 
		std::cout << entries << std::endl ;	
		entries += HistodEondXTop.at(iInterval)->GetEntries();
		std::cout << entries << std::endl ;	
		entries += HistodEondXBottom.at(iInterval)->GetEntries();
		std::cout << entries << std::endl ;
	}

	std::cout << " " << std::endl ;		
	std::cout << " " << std::endl ;		
	std::cout << entries << std::endl ;	
	std::cout << " " << std::endl ;		
	std::cout << " " << std::endl ;		
	
	// canvas for graphs
	TCanvas* c1 = new TCanvas("c1", "c1", 0, 0, 400, 400);
	TGraphErrors * gTop = new TGraphErrors(nIntervals, IntervalMeanAngle, IntervaldEondXPeakTop, 0, IntervaldEondXPeakErrorTop);
	TGraphErrors * gBottom = new TGraphErrors(nIntervals, IntervalMeanAngle, IntervaldEondXPeakBottom, 0, IntervaldEondXPeakErrorBottom);
	
	TCanvas* c2 = new TCanvas("c2", "c2", 0, 0, 400, 400);	
	TGraphErrors * gTBdiff = new TGraphErrors(nIntervals, IntervalMeanAngle, IntervaldEondXPeakTBDiff, 0, IntervaldEondXPeakErrorTBDiff);
	
	TCanvas* c3 = new TCanvas("c3", "c3", 0, 0, 400, 400);	
	TGraphErrors * gTBratio = new TGraphErrors(nIntervals, IntervalMeanAngle, IntervaldEondXPeakTBRatio, 0, IntervaldEondXPeakErrorTBRatio);	
// 	TGraphErrors * gdiffTB = new TGraphErrors(nIntervals, IntervalMeanAngle, (IntervaldEondXPeakTop - IntervaldEondXPeakBottom), 0, IntervaldEondXPeakErrorBottom);	
	
// 	TGraphErrors * gdiffTB = new TGraphErrors(nIntervals, IntervalMeanAngle, IntervaldEondXPeakTop-IntervaldEondXPeakBottom, 0, IntervaldEondXPeakErrorTop+IntervaldEondXPeakErrorBottom);
// 	TF1 * g1 = new TF1("g1", "pol1", 0., 80.);
	 
// 	g->SetTitle("diff_dEondX Bot-Top");
// 	cherenkov->GetXaxis()->SetTitle("angle (deg)");
// 	cherenkov->GetYaxis()->SetTitle("diff_dEondX (GeV/cm)");
// 	cherenkov->GetYaxis()->SetRangeUser(-0.01,0.01);
// 	cherenkov->GetYaxis()->SetTitleOffset(1.1);

	gTop->SetMarkerColor(kBlue);
	gTop->SetMarkerStyle(7);
	gBottom->SetMarkerColor(kRed);
	gBottom->SetMarkerStyle(7);
	
	gTBdiff->SetMarkerColor(kMagenta);
	gTBdiff->SetMarkerStyle(7);
	
	gTBratio->SetMarkerColor(kGreen);
	gTBratio->SetMarkerStyle(7);	

	c1->cd();
   gTop->Draw("AP");
	gBottom->Draw("P");
	
	c2->cd();
	gTBdiff->Draw("AP");
	
	c3->cd();
	gTBratio->Draw("AP");
	
	// write on file 
	TFile out ("angoli_histos.root","recreate");
	
 	TDirectory * Intervals = gDirectory->mkdir("Intervals");
	
	dEondX.Write(); 
	Angle.Write(); 
   Length.Write();	
	Occupancy.Write();
	
	Intervals->cd();
	for(int iInterval = 0 ; iInterval < nIntervals ; ++iInterval) 
	{
		HistodEondXTop.at(iInterval)->Write();
		HistodEondXBottom.at(iInterval)->Write();
		HistoOccupancyTop.at(iInterval)->Write();	
		HistoOccupancyBottom.at(iInterval)->Write();					
	}
	
	out.cd();

	c1->Write("TB");
	c2->Write("TBdiff");	
	c3->Write("TBratio");		
	 
	out.Close();
	
	// delete
	for(int iInterval = 0 ; iInterval < nIntervals ; ++iInterval)
	{ 
		delete HistodEondXTop.at(iInterval);
		delete HistodEondXBottom.at(iInterval);
		delete HistoOccupancyTop.at(iInterval);
		delete HistoOccupancyBottom.at(iInterval);				
	}
		
	delete gaussianast;	
	delete gaussianand;			
	
/*	Occupancy.SetDrawOption("COLZ");
	AngleVsdiff_dEondX.Write(); 	
	 //
	TCanvas * c2 = new TCanvas("c2", "c2", 0, 0, 400, 400);
	c2->cd();
	Occupancy.GetXaxis()->SetTitle("#phi");
	Occupancy.GetYaxis()->SetTitle("#eta");
	Occupancy.Draw("COLZ");
	c2->Write("Occupancy");
	 //
	Intervals->cd();
	for(int iInterval = 0 ; iInterval < nIntervals ; ++iInterval) 
		diff_dEondX_histo_interv.at(iInterval)-> Write();
	out.cd();


*/	
	 
	return(0);
}


