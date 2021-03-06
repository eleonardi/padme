// PVetoMessenger.cc
// --------------------------------------------------------------
// History:
//
// 2015-12-14 Created by Emanuele Leonardi (emanuele.leonardi@roma1.infn.it)
//
// --------------------------------------------------------------

#include "PVetoMessenger.hh"

#include "PVetoDetector.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"

#include "PVetoGeometry.hh"

PVetoMessenger::PVetoMessenger(PVetoDetector* det)
:fPVetoDetector(det)
{

  fPVetoGeometry = PVetoGeometry::GetInstance();

  fPVetoDetectorDir = new G4UIdirectory("/Detector/PVeto/");
  fPVetoDetectorDir->SetGuidance("UI commands to control PVeto detector geometry");

  fSetPVetoNFingersCmd = new G4UIcommand("/Detector/PVeto/NFingers",this);
  fSetPVetoNFingersCmd->SetGuidance("Set number of PVeto fingers.");
  G4UIparameter* nfNFinParameter = new G4UIparameter("NFin",'i',false);
  nfNFinParameter->SetParameterRange("NFin >= 1 && NFin <= 1000");
  fSetPVetoNFingersCmd->SetParameter(nfNFinParameter);
  fSetPVetoNFingersCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fSetFingerSizeCmd = new G4UIcommand("/Detector/PVeto/FingerSize",this);
  fSetFingerSizeCmd->SetGuidance("Set size (side of square) of finger front face in cm.");
  G4UIparameter* fsSizeParameter = new G4UIparameter("Size",'d',false);
  fsSizeParameter->SetParameterRange("Size > 0. && Size <= 10.");
  fSetFingerSizeCmd->SetParameter(fsSizeParameter);
  fSetFingerSizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fSetFingerLengthCmd = new G4UIcommand("/Detector/PVeto/FingerLength",this);
  fSetFingerLengthCmd->SetGuidance("Set vertical length of finger in cm.");
  G4UIparameter* fsLengthParameter = new G4UIparameter("Length",'d',false);
  fsLengthParameter->SetParameterRange("Length > 0. && Length <= 23.");
  fSetFingerLengthCmd->SetParameter(fsLengthParameter);
  fSetFingerLengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fSetPVetoInnerFaceXCmd = new G4UIcommand("/Detector/PVeto/InnerFaceX",this);
  fSetPVetoInnerFaceXCmd->SetGuidance("Set position along X of PVeto inner face in cm.");
  G4UIparameter* pifPosXParameter = new G4UIparameter("PosX",'d',false);
  pifPosXParameter->SetParameterRange("PosX < 0. && PosX >= -55.");
  fSetPVetoInnerFaceXCmd->SetParameter(pifPosXParameter);
  fSetPVetoInnerFaceXCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fSetPVetoFrontFaceZCmd = new G4UIcommand("/Detector/PVeto/FrontFaceZ",this);
  fSetPVetoFrontFaceZCmd->SetGuidance("Set position along Z of PVeto front face in cm.");
  G4UIparameter* pifPosZParameter = new G4UIparameter("PosZ",'d',false);
  pifPosZParameter->SetParameterRange("PosZ >=-50. && PosZ <= 50.");
  fSetPVetoFrontFaceZCmd->SetParameter(pifPosZParameter);
  fSetPVetoFrontFaceZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fSetVerboseLevelCmd = new G4UIcmdWithAnInteger("/Detector/PVeto/VerboseLevel",this);
  fSetVerboseLevelCmd->SetGuidance("Set verbose level for PVeto code.");
  fSetVerboseLevelCmd->SetParameterName("VL",false);
  fSetVerboseLevelCmd->SetRange("VL >= 0");
  fSetVerboseLevelCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

PVetoMessenger::~PVetoMessenger()
{

  delete fSetPVetoNFingersCmd;
  delete fSetFingerSizeCmd;
  delete fSetFingerLengthCmd;

  delete fSetPVetoInnerFaceXCmd;
  delete fSetPVetoFrontFaceZCmd;

  delete fSetVerboseLevelCmd;

  delete fPVetoDetectorDir;

}

void PVetoMessenger::SetNewValue(G4UIcommand* cmd, G4String par)
{

  if ( cmd == fSetPVetoNFingersCmd ) {
    G4int n; std::istringstream is(par); is >> n;
    fPVetoGeometry->SetPVetoNFingers(n);
  }

  if ( cmd == fSetFingerSizeCmd ) {
    G4double s; std::istringstream is(par); is >> s;
    fPVetoGeometry->SetFingerSizeX(s*cm);
    fPVetoGeometry->SetFingerSizeZ(s*cm);
  }

  if ( cmd == fSetFingerLengthCmd ) {
    G4double s; std::istringstream is(par); is >> s;
    fPVetoGeometry->SetFingerSizeY(s*cm);
  }

  if ( cmd == fSetPVetoInnerFaceXCmd ) {
    G4double x; std::istringstream is(par); is >> x;
    fPVetoGeometry->SetPVetoInnerFacePosX(x*cm);
  }

  if ( cmd == fSetPVetoFrontFaceZCmd ) {
    G4double z; std::istringstream is(par); is >> z;
    fPVetoGeometry->SetPVetoFrontFacePosZ(z*cm);
  }
 
  if ( cmd == fSetVerboseLevelCmd )
    fPVetoGeometry->SetVerboseLevel(fSetVerboseLevelCmd->GetNewIntValue(par));

}
