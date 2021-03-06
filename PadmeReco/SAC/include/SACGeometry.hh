// --------------------------------------------------------------
// History:
//
// Created by Stefania Spagnolo (stefania.spagnolo@le.infn.it) 2019-03-14
//
// --------------------------------------------------------------
#ifndef SACGeometry_H
#define SACGeometry_H

#include "PadmeVGeometry.hh"



class SACGeometry : public PadmeVGeometry
{

public:
  
  SACGeometry();
  //~SACGeometry();
  void Init(PadmeVRecoConfig *cfg, RecoVChannelID *chIdMgr );
  TVector3  LocalPosition(Int_t chId);
  

private:

  double fCrystalSizeX;
  double fCrystalSizeY;
  double fCrystalSizeZ;
  double fCrystalGap;
  double fCrystalCoating;
  double fTedlarThickness;
  double fSACFrontFacePosZ;
  
 
};
#endif

