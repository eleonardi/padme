//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: LAVHit.hh,v 1.2 2014/06/23 13:44:14 veni Exp $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef LAVHit_h
#define LAVHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class LAVHit : public G4VHit
{
  public:

      LAVHit();
     ~LAVHit();
      LAVHit(const LAVHit&);
      const LAVHit& operator=(const LAVHit&);
      G4int operator==(const LAVHit&) const;

      inline void* operator new(size_t);
      inline void  operator delete(void*);

      void Draw();
      void Print();

  public:
  
      void SetTrackID  (G4int track) { VtrackID = track; };
      void SetLAVNb(G4int cry)  { LAVNb = cry; };
      void SetPType(G4int typ)       { PType = typ; };
      void SetEdep(G4double de)      { edep = de; };
      void SetPos(G4ThreeVector xyz) { pos = xyz; };
      void SetTime(G4double time)    { VetoT = time; };
      
      G4int GetTrackID()    { return VtrackID; };
      G4int GetLAVNb() { return LAVNb; };
      G4double GetEdep()    { return edep; };      
      G4double GetTime()    { return VetoT; };
      G4int GetPType()      { return PType; };
      G4ThreeVector GetPos(){ return pos; };
      G4double GetX(){ return pos.x(); };
      G4double GetY(){ return pos.y(); };
      G4double GetZ(){ return pos.z(); };

  private:
  
      G4int         VtrackID;
      G4int         PType;
      G4int         LAVNb;
      G4double      VetoT;
      G4double      edep;
      G4ThreeVector pos;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<LAVHit> LAVHitsCollection;

extern G4Allocator<LAVHit> LAVHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* LAVHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) LAVHitAllocator.MallocSingle();
  return aHit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void LAVHit::operator delete(void *aHit)
{
  LAVHitAllocator.FreeSingle((LAVHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
