// TargetDigitizer.hh
// --------------------------------------------------------------
// History:
//
// Created by Emanuele Leonardi (emanuele.leonardi@roma1.infn.it) 2106-07-22
// --------------------------------------------------------------

#ifndef TargetDigitizer_h
#define TargetDigitizer_h 1

#include "G4VDigitizerModule.hh"
#include "TargetDigi.hh" 

#include "TargetMessenger.hh"
#include "TargetGeometry.hh"

//#include "TFile.h"
#include "TH1D.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TargetDigitizer : public G4VDigitizerModule
{
public:
  TargetDigitizer(G4String);
  ~TargetDigitizer();

  void Digitize();

private: 

  TargetMessenger* fTargetMessenger;

  G4int PosToChannel(G4double pos);

  std::vector<G4double> RCFilter (std::vector<G4double>& traceI);
  std::vector<G4double> AddNoise (std::vector<G4double>& traceI);
  
  G4double stepfunCCD(G4double& ct, G4int& neh, G4int& cn, G4double& cd, G4double& os, G4int& cb, G4double& nt);

  G4String digiCollectionName;
  G4int nChannels; 
  G4double fThreshold;
  G4double nTrackDiv; 

  // ROOT file target.root
  //  TFile * fTargetFile;  
  TH1D * dTraceHCCD[32]; // hardcoded nchannels to change
  TH1D * dTraceVHCCD[32]; // 
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
