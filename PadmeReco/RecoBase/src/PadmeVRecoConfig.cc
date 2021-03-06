#include "PadmeVRecoConfig.hh"
#include <iostream>

PadmeVRecoConfig::PadmeVRecoConfig(utl::ConfigParser *cfgParser,TString Name) 
  :PadmeVNamedModule(Name)
{

  fConfigParser = cfgParser;

  if(cfgParser->HasConfig("DETECTOR","READOUT")){
     fReadOutType = cfgParser->GetSingleArg("DETECTOR","READOUT");
  } else {
    fReadOutType = "NO_READOUT";
  }
  
  std::cout << "Readout type: " << fReadOutType << std::endl;
  




  if(fReadOutType == "ADC") 
    ConfigADCReadout();

  //    InitChannelID(this);

}



void PadmeVRecoConfig::ConfigADCReadout(){
  utl::ConfigParser *cfgParser = fConfigParser;
  
  if(cfgParser->HasConfig("ADC","NADC")){
    fNBoards = std::stoi(cfgParser->GetSingleArg("ADC","NADC"));
  } else {
    return;
  }
  
  

  if(cfgParser->HasConfig("ADC","ID")){
    std::vector<std::string> bIDs = cfgParser->GetConfig("ADC","ID");
    for(auto it = bIDs.begin(); it != bIDs.end(); ++it) {
      fBoards.push_back(std::stoi(*it));
    }
  }

  
  std::cout << fBoards.size() << "  boards belonging to detector " << this->GetName() << ", IDs   ";
  for(auto it = fBoards.begin(); it != fBoards.end(); ++it) {
    std::cout << (*it) << "   ";
  }
  std::cout << std::endl;
  
  
  

}




PadmeVRecoConfig::~PadmeVRecoConfig() {;}

Bool_t PadmeVRecoConfig::BoardIsMine(Int_t iBoard){
  for(auto it = fBoards.begin(); it != fBoards.end(); ++it) {
    if(*it == iBoard) return true;
  }
  return false;
}

int PadmeVRecoConfig::GetParOrDefault(std::string group,std::string name,int val){
  return fConfigParser->HasConfig(group,name)?std::stoi(fConfigParser->GetSingleArg(group,name)):val;
}

double PadmeVRecoConfig::GetParOrDefault(std::string group,std::string name,double val){
  return fConfigParser->HasConfig(group,name)?std::stof(fConfigParser->GetSingleArg(group,name)):val;
}

std::string PadmeVRecoConfig::GetParOrDefault(std::string group,std::string name,std::string val){
  return fConfigParser->HasConfig(group,name)?fConfigParser->GetSingleArg(group,name):val;
}
