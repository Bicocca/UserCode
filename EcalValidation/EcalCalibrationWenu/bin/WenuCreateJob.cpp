//==== parameter include ====
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


//---- std include ----
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <fstream>


//---- boost ----
#include <boost/lexical_cast.hpp>


//==== function to create selection ====
std::string selection(
 const std::vector<std::string>& ScanVariables,
 const std::vector<double>& minValueTemp,
 const std::vector<double>& maxValueTemp)
{
 std::string result;
 result += "(1";
 for (unsigned int iVar = 0; iVar<ScanVariables.size(); iVar++ ) {
  result += "&& (";
  result += ScanVariables.at(iVar);
  result += " > ";
  result +=  boost::lexical_cast<std::string>(minValueTemp.at(iVar));
  result += " && ";
  result += ScanVariables.at(iVar);
  result += " < ";
  result +=  boost::lexical_cast<std::string>(maxValueTemp.at(iVar));
  result += ")";
   }
 result += ")";
 return result;
}


//==== function to create name of output ====
std::string nameSelection(
 const std::vector<std::string>& ScanVariablesId,
 const std::vector<double>& minValueTemp,
 const std::vector<double>& maxValueTemp)
{
 std::cerr << " nameSelection " << std::endl;
 std::string result;
 for (unsigned int iVar = 0; iVar<ScanVariablesId.size(); iVar++ ) {
  result += "_";
  result += ScanVariablesId.at(iVar);
  result += "_";
  result +=  boost::lexical_cast<std::string>(minValueTemp.at(iVar));
  result += "_";
  result +=  boost::lexical_cast<std::string>(maxValueTemp.at(iVar));
  std::cout << " minValueTemp==)))))).at(" << iVar << ":" << ScanVariablesId.size() << ") = " << minValueTemp.at(iVar) ;
  std::cout << " maxValueTemp==)))))).at(" << iVar << ":" << ScanVariablesId.size() << ") = " << maxValueTemp.at(iVar) << std::endl;
 }
 return result;
}


///==== Recursive function ====

void runFor(
 int iVar,
 std::vector<std::string> & nameFileOut,
 const std::vector<std::string>& ScanVariablesId,
 std::vector<std::string> & cut,
 const std::vector<std::string>& ScanVariables, 
 const std::vector< std::vector<double> >& minValue,
 const std::vector< std::vector<double> >& maxValue,
 std::vector<double>& minValueTemp,
 std::vector<double>& maxValueTemp
 )
{
 int nVar = ScanVariables.size();
 if (iVar == nVar){
  cut.push_back(selection(ScanVariables,minValueTemp,maxValueTemp));
  nameFileOut.push_back(nameSelection(ScanVariablesId,minValueTemp,maxValueTemp));
  iVar--;
 }
 else {
  for (unsigned int iBin = 0; iBin < minValue.at(iVar).size(); iBin ++){
   std::cerr << " minValue.at(" << iVar<< ").at(" << iBin << ") = " << minValue.at(iVar).at(iBin) << std::endl;
   std::cerr << " maxValue.at(" << iVar<< ").at(" << iBin << ") = " << maxValue.at(iVar).at(iBin) << std::endl;
   minValueTemp.push_back(minValue.at(iVar).at(iBin)) ;
   maxValueTemp.push_back(maxValue.at(iVar).at(iBin)) ;
   runFor(iVar+1,
     nameFileOut,
     ScanVariablesId,
     cut,
     ScanVariables, 
     minValue,
     maxValue,
     minValueTemp,
     maxValueTemp  
   );
   minValueTemp.pop_back();
   maxValueTemp.pop_back();
  }
 }
}

///=======================================







int main(int argc, char** argv){

 if(argc != 2)
 {
  std::cerr << ">>>>> analysis.cpp::usage: " << argv[0] << " configFileName" << std::endl ;
  return 1;
 }

 std::string fileName (argv[1]) ;
 boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile(fileName) ;
 boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet () ;

 edm::ParameterSet subPSetInput = parameterSet->getParameter<edm::ParameterSet> ("inputTree") ;
 std::string treeNameDATA  = subPSetInput.getParameter<std::string> ("treeNameDATA") ;
 std::string inputFileDATA = subPSetInput.getParameter<std::string> ("inputFileDATA") ;
 std::string treeNameMC = subPSetInput.getParameter<std::string> ("treeNameMC") ;
 std::string inputFileMC = subPSetInput.getParameter<std::string> ("inputFileMC") ;

 edm::ParameterSet subPSetOutput = parameterSet->getParameter<edm::ParameterSet> ("outputTree") ;
 std::string outputFile = subPSetOutput.getParameter<std::string> ("outputFile") ;

 edm::ParameterSet subPSetOptions = parameterSet->getParameter<edm::ParameterSet> ("options") ;
 double MinScan  = subPSetOptions.getParameter<double> ("MinScan") ;
 double MaxScan  = subPSetOptions.getParameter<double> ("MaxScan") ;
 int iNoSteps = subPSetOptions.getParameter<int>    ("iNoSteps") ;
  
 double MinScanRange  = subPSetOptions.getParameter<double> ("MinScanRange") ;
 double MaxScanRange  = subPSetOptions.getParameter<double> ("MaxScanRange") ;
 
 double minBINS = subPSetOptions.getParameter<double> ("minBINS") ;
 double maxBINS = subPSetOptions.getParameter<double> ("maxBINS") ;
 int numBINS = subPSetOptions.getParameter<int>    ("numBINS") ;


 double minET = subPSetOptions.getParameter<double> ("minET") ;
 
 std::string temp_cut = subPSetOptions.getParameter<std::string> ("cut") ;


 int EEEB = subPSetOptions.getParameter<int> ("EEorEB");
 
 int maxIter = subPSetOptions.getParameter<int> ("numToyMC") ;

 
 edm::ParameterSet subPSetScanOptions = parameterSet->getParameter<edm::ParameterSet> ("ScanOptions") ;
 std::vector<std::string> variablesName = subPSetScanOptions.getParameter<std::vector<std::string> > ("variablesName") ;
 std::vector<std::string> variablesNameId = subPSetScanOptions.getParameter<std::vector<std::string> > ("variablesNameId") ;
 
 std::vector<std::string> ScanVariables = subPSetScanOptions.getParameter<std::vector<std::string> >("ScanVariables") ;
 std::vector<std::string> ScanVariablesId = subPSetScanOptions.getParameter<std::vector<std::string> >("ScanVariablesId") ;
  
 std::vector< std::vector<double> > minValue;
 std::vector< std::vector<double> > maxValue;
 
 for (unsigned int iVar = 0; iVar<ScanVariablesId.size(); iVar++ ) {
  std::string nameMin = "minValue" + ScanVariablesId.at(iVar);
  std::string nameMax = "maxValue" + ScanVariablesId.at(iVar);
  minValue.push_back(subPSetScanOptions.getParameter<std::vector<double> > (nameMin));
  maxValue.push_back(subPSetScanOptions.getParameter<std::vector<double> > (nameMax));
  std::cout << " iVar = " << iVar << " minValue.at(" << iVar  << ").size() = " << minValue.at(iVar).size() << std::endl;
  std::cout <<              "          maxValue.at(" << iVar  << ").size() = " << maxValue.at(iVar).size() << std::endl;
  for (unsigned int iSel = 0; iSel<minValue.at(iVar).size() ; iSel++ ) {
   std::cout << "      " << "minValue.at(" << iVar  << ").at(" << iSel << ") = " << minValue.at(iVar).at(iSel) << std::endl;   
   std::cout << "      " << "maxValue.at(" << iVar  << ").at(" << iSel << ") = " << maxValue.at(iVar).at(iSel) << std::endl;   
  }
 }
 
 ///==== create job files ====

 system("mkdir cfg/");  
 system("cd cfg")

 for (unsigned int ivariablesName = 0; ivariablesName < variablesName.size(); ivariablesName++){
  std::cout << " variablesName[" << ivariablesName << "] = " << variablesName.at(ivariablesName) << std::endl;
  std::vector<std::string> cut;
  std::vector<std::string> nameFileOut;
  std::vector<double> minValueTemp;
  std::vector<double> maxValueTemp;
  runFor(0,
   nameFileOut,
   ScanVariablesId,
   cut,
   ScanVariables, 
   minValue,
   maxValue,
   minValueTemp,
   maxValueTemp
  );
  for (unsigned int iCut = 0; iCut < cut.size(); iCut++){
    std::cerr << "   nameFileOut[" << iCut << "] = " << nameFileOut.at(iCut) << std::endl;
    std::cerr << "   cut[" << iCut << "] = " << cut.at(iCut) << std::endl;



    std::string nameFileOutput = "outputTree";
    nameFileOutput += "_";
    nameFileOutput += variablesNameId.at(ivariablesName);
    nameFileOutput += nameFileOut.at(iCut);
    nameFileOutput += ".root";

  

    std::string nameFile = "WenuEnScale";
    nameFile += "_";
    nameFile += variablesNameId.at(ivariablesName);
    nameFile += nameFileOut.at(iCut);
    nameFile += ".cfg";
    
    std::ofstream Instruction;
    
    Instruction.open (nameFile.c_str());
    Instruction << "import FWCore.ParameterSet.Config as cms" << std::endl;
    Instruction << "process = cms.Process(\"TEST\")" << std::endl;

    Instruction << "process.inputTree = cms.PSet(" << std::endl;
    Instruction << "   treeNameDATA = cms.string(\"" << treeNameDATA << "\")," << std::endl;
    Instruction << "   inputFileDATA = cms.string(\"" << inputFileDATA << "\")," << std::endl;
    Instruction << "   treeNameMC = cms.string(\"" << treeNameMC << "\")," << std::endl;
    Instruction << "   inputFileMC = cms.string(\"" << inputFileMC << "\")" << std::endl;
    Instruction << ")" << std::endl;

    Instruction << "process.outputTree = cms.PSet(" << std::endl;
    Instruction << "   outputTree = cms.string(\"" << nameFileOutput << "\")" << std::endl;
    Instruction << ")" << std::endl;

    Instruction << "process.options = cms.PSet(" << std::endl;
    Instruction << "  EEorEB = cms.int32(" << EEEB << ")," << std::endl;
    Instruction << "  numToyMC = cms.int32(" << maxIter << ")," << std::endl;
  
    Instruction << "  MinScan = cms.double(" << MinScan << ")," << std::endl;
    Instruction << "  MaxScan = cms.double(" << MaxScan << ")," << std::endl;
    Instruction << "  iNoSteps = cms.int32(" << iNoSteps << ")," << std::endl;

    Instruction << "  MinScanRange = cms.double(" << MinScanRange << ")," << std::endl;
    Instruction << "  MaxScanRange = cms.double(" << MaxScanRange << ")," << std::endl;

    Instruction << "  cut = cms.string(\"(" << cut.at(iCut) <<") && " << temp_cut << "\")," << std::endl;

    Instruction << "  minET = cms.double(" << minET << ")," << std::endl;

    Instruction << "  variableName = cms.string(\"" << variablesName.at(ivariablesName) << "\")," << std::endl;
    Instruction << "  minBINS      = cms.double(" << minBINS << ")," << std::endl;
    Instruction << "  maxBINS      = cms.double(" << maxBINS << ")," << std::endl;
    Instruction << "  numBINS      = cms.int32(" << numBINS << ")" << std::endl; 
    Instruction << ")" << std::endl;

    Instruction.close();
   
    ///==== file .sh ====
    std::string nameJob = "nameJob_WenuEnScale";
    nameJob += "_";
    nameJob += variablesNameId.at(ivariablesName);
    nameJob += nameFileOut.at(iCut);
    nameJob += ".sh";

   
    std::cout << " nameJob = " << nameJob << std::endl;
    std::ofstream job;
    job.open (nameJob.c_str());
    job << "echo -e #!/bin/sh" << std::endl;
    job << "cd /afs/cern.ch/user/a/amassiro/scratch0/WeCalib/CMSSW_3_8_4_patch3/src/EcalValidation/EcalCalibrationWenu/" << std::endl;
    job << "eval `scramv1 runtime -sh`" << std::endl;
    job << "cd - " << std::endl;
    job << "rfcp /castor/cern.ch/user/a/amassiro/EcalCalibrationWenu/" << inputFileMC   << " ./ " << std::endl;
    job << "rfcp /castor/cern.ch/user/a/amassiro/EcalCalibrationWenu/" << inputFileDATA << " ./ " << std::endl;
    job << "WenuCreateJob " << nameFile.c_str() << std::endl;
    job << "rfcp " << nameFileOutput << " /castor/cern.ch/user/a/amassiro/EcalCalibrationWenu/Result/ " << std::endl;
        
    ///=============================================================
   
    std::string CommandToExec = "chmod 777 " + nameJob;
    system(CommandToExec.c_str());  
   
    ///=============================================================
   
    CommandToExec = "bsub -q \"1nh\" " + nameJob;     
    std::cout << " CommandToExec = " << CommandToExec << std::endl;
//    system(CommandToExec.c_str());  
       
    ///=============================================================
    
   }
   
  }
  
 system("cd -");
 
 return 1;  
}





