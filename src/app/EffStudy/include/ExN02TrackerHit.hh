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
// $Id: ExN02TrackerHit.hh,v 1.8 2006-06-29 17:47:53 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef ExN02TrackerHit_h
#define ExN02TrackerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ExN02TrackerHit : public G4VHit
{
public:

  ExN02TrackerHit();
  ~ExN02TrackerHit();
  ExN02TrackerHit(const ExN02TrackerHit&);
  const ExN02TrackerHit& operator=(const ExN02TrackerHit&);
  G4int operator==(const ExN02TrackerHit&) const;

  inline void* operator new(size_t);
  inline void  operator delete(void*);

  void Draw();
  void Print();

public:
  
  void SetTrackID     (G4int track)      { trackID = track; };
  void SetParentID    (G4int parent)     { parentID = parent; };
  void SetChamberNb   (G4int chamb)      { chamberNb = chamb; };  
  void SetEdep        (G4double de)      { edep = de; };
  void SetPreStepPos  (G4ThreeVector xyz){ prestepPos = xyz; };  
  void SetPostStepPos (G4ThreeVector xyz){ poststepPos = xyz; };  
  void SetMom         (G4ThreeVector xyz){ mom = xyz; }; 
  void SetNameDet     (G4String name)    { namedet = name; };
  void SetCharge      (G4int mycharge)   { charge = mycharge; };
  void SetTrackPDG    (G4int pdg)        { trackpdg = pdg; };
  void SetTrackName   (G4String name)    { trackname = name; };
  void SetTrackLength (G4double length ) { tracklength = length; };
  void SetStepLength  (G4double length ) { steplength = length; };
  void SetStepDeltaLyz(G4double deltalyz ) { stepdeltalyz = deltalyz; };
  void SetNSteps      (G4int num)        { nsteps = num; };
  void SetTrackCosTheta(G4double costh)  { trackcostheta = costh; };

  G4int GetTrackID()       { return trackID; };
  G4int GetParentID()      { return parentID; };
  G4int GetChamberNb()     { return chamberNb; };
  G4double GetEdep()       { return edep; };      
  G4ThreeVector GetPreStepPos()   { return prestepPos; };
  G4ThreeVector GetPostStepPos() { return poststepPos; };
  G4ThreeVector GetMom()   { return mom; }; 
  G4String GetNameDet()    { return namedet; }; 
  G4int GetCharge()        { return charge; }; 
  G4int GetTrackPDG()      { return trackpdg; }; 
  G4String GetTrackName()  { return trackname; }; 
  G4double GetTrackLength(){ return tracklength; }; 
  G4double GetStepLength() { return steplength; }; 
  G4double GetStepDeltaLyz() { return stepdeltalyz; }; 
  G4int GetNSteps()        { return nsteps; }; 
  G4double GetTrackCosTheta(){ return trackcostheta; };
  
private:
  
  G4int         trackID;
  G4int         parentID;
  G4int         chamberNb;
  G4double      edep;
  G4ThreeVector prestepPos;
  G4ThreeVector poststepPos;
  G4String      namedet;
  G4int         charge;
  G4ThreeVector mom; 
  G4int         trackpdg;
  G4String      trackname;  
  G4double      tracklength;
  G4double      steplength;
  G4double      stepdeltalyz;
  G4int         nsteps;
  G4double      trackcostheta;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<ExN02TrackerHit> ExN02TrackerHitsCollection;

extern G4Allocator<ExN02TrackerHit> ExN02TrackerHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* ExN02TrackerHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) ExN02TrackerHitAllocator.MallocSingle();
  return aHit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void ExN02TrackerHit::operator delete(void *aHit)
{
  ExN02TrackerHitAllocator.FreeSingle((ExN02TrackerHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif