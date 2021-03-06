#ifndef BeamFlagHit_h
#define BeamFlagHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class BeamFlagHit : public G4VHit
{

public:

  BeamFlagHit();
  ~BeamFlagHit();
  BeamFlagHit(const BeamFlagHit&);
  const BeamFlagHit& operator=(const BeamFlagHit&);
  G4int operator==(const BeamFlagHit&) const;

  inline void* operator new(size_t);
  inline void  operator delete(void*);

  void Draw();
  void Print();

public:
  
  void SetChannelId(G4int c) { fChannelId = c; }
  G4int GetChannelId() { return fChannelId; }

  void SetTime(G4double t) { fTime = t; };
  G4double GetTime() { return fTime; };

  void SetEnergy(G4double e) { fEnergy = e; }
  G4double GetEnergy() { return fEnergy; }

  void SetPosition(G4ThreeVector p) { fPosition = p; }
  G4ThreeVector GetPosition() { return fPosition; }
  G4double GetPosX() { return fPosition.x(); };
  G4double GetPosY() { return fPosition.y(); };
  G4double GetPosZ() { return fPosition.z(); };

  //M. Raggi 2/04/2019 added track energy
  void SetTrackEnergy(G4double etr) { fTrackEnergy = etr; }
  G4double GetTrackEnergy() { return fTrackEnergy; }

  //M. Raggi 2/04/2019 added track energy
  G4double GetPX() { return fMomentumDirection.x(); };
  G4double GetPY() { return fMomentumDirection.y(); };
  G4double GetPZ() { return fMomentumDirection.z(); };
  void SetPDir(G4ThreeVector pdir) {fMomentumDirection=pdir;}

  void SetLocalPosition(G4ThreeVector p) { fLocalPosition = p; }
  G4ThreeVector GetLocalPosition() { return fLocalPosition; }
  G4double GetLocalPosX() { return fLocalPosition.x(); };
  G4double GetLocalPosY() { return fLocalPosition.y(); };
  G4double GetLocalPosZ() { return fLocalPosition.z(); };

  // Kept for backward compatibility: will be removed
  void SetEdep(G4double e) { fEnergy = e; }
  G4double GetEdep() { return fEnergy; }
  void SetPos(G4ThreeVector xyz) { fPosition = xyz; };
  G4ThreeVector GetPos() { return fPosition; };
  G4double GetX() { return fPosition.x(); };
  G4double GetY() { return fPosition.y(); };
  G4double GetZ() { return fPosition.z(); };

private:

  G4int         fChannelId;
  G4double      fTime;
  G4double      fEnergy;
  G4double      fTrackEnergy; //M. Raggi 2/04/2019 added track energy
  G4ThreeVector fPosition;
  G4ThreeVector fLocalPosition;
  G4ThreeVector fMomentumDirection; //M. Raggi 5/04/2019 added track energy

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<BeamFlagHit> BeamFlagHitsCollection;

extern G4Allocator<BeamFlagHit> BeamFlagHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* BeamFlagHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) BeamFlagHitAllocator.MallocSingle();
  return aHit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void BeamFlagHit::operator delete(void *aHit)
{
  BeamFlagHitAllocator.FreeSingle((BeamFlagHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
