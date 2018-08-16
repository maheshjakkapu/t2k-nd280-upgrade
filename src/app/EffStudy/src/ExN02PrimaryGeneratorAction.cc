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
// $Id: B4PrimaryGeneratorAction.cc 75215 2013-10-29 16:07:06Z gcosmo $
// 
/// \file B4PrimaryGeneratorAction.cc
/// \brief Implementation of the B4PrimaryGeneratorAction class

#include "ExN02PrimaryGeneratorAction.hh"
#include "ExN02PrimaryGeneratorMessenger.hh"
#include "ExN02RooTrackerKinematicsGenerator.hh"

#include "ExN02VertexInfo.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "G4Run.hh"

//#include "G4SystemOfUnits.hh" // NEW GLOBAL
#include <CLHEP/Units/SystemOfUnits.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02PrimaryGeneratorAction::ExN02PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(0),
   RooTrackerNEUT(),
   fPrimaryGeneratorMessenger(0),
   fGeneratorType("")
{
  // Set the messenger  
  fPrimaryGeneratorMessenger = new ExN02PrimaryGeneratorMessenger(this); 
  
  //
  // default particle kinematic
  //

  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);
  
  G4String particlename = "mu-"; //"pi-" //"pi+" 
  G4double particleenergy = 600 * CLHEP::MeV; // GeV   
  
  G4ParticleDefinition* particleDefinition 
    = G4ParticleTable::GetParticleTable()->FindParticle(particlename);
  fParticleGun->SetParticleDefinition(particleDefinition);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,1.,0.)); 
  fParticleGun->SetParticleEnergy(particleenergy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02PrimaryGeneratorAction::~ExN02PrimaryGeneratorAction()
{
  delete fPrimaryGeneratorMessenger;
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // This function is called at the begining of event

  bool doGun = false;
  if(fGeneratorType=="ParticleGun") doGun = true;

  if(doGun){
  
    // In order to avoid dependence of PrimaryGeneratorAction
    // on DetectorConstruction class we get world volume
    // from G4LogicalVolumeStore
    //
    G4double worldZHalfLength = 0;
    G4double targetYHalfLength = 0;
    
    G4LogicalVolume* worlLV
      = G4LogicalVolumeStore::GetInstance()->GetVolume("/t2k");
    G4Box* worldBox = 0;
    if ( worlLV) worldBox = dynamic_cast< G4Box*>(worlLV->GetSolid()); 
    if ( worldBox ) {
      worldZHalfLength = worldBox->GetZHalfLength();  
    }
    else  {
      G4ExceptionDescription msg;
      msg << "World volume of box not found." << G4endl;
      msg << "Perhaps you have changed geometry." << G4endl;
      msg << "The gun will be place in the center.";
      G4Exception("ExN02PrimaryGeneratorAction::GeneratePrimaries()",
		  "MyCode0002",FatalException, msg);
    } 

    /*
    G4LogicalVolume* targetLV
      = G4LogicalVolumeStore::GetInstance()->GetVolume("/t2k/OA/Magnet/Basket/Target1");
    G4Box* targetBox = 0;
    if ( targetLV) targetBox = dynamic_cast< G4Box*>(targetLV->GetSolid()); 
    if ( targetBox ) {
      targetYHalfLength = targetBox->GetYHalfLength();  
    }
    else  {
      G4ExceptionDescription msg;
      msg << "Target volume of box not found." << G4endl;
      msg << "Perhaps you have changed geometry." << G4endl;
      msg << "The gun will be place in the center.";
      G4Exception("ExN02PrimaryGeneratorAction::GeneratePrimaries()",
		  "MyCode0002",FatalException, msg);
    } 
    */

    G4double aMomAmp = fParticleGun->GetParticleMomentum();
    G4double aEAmp = fParticleGun->GetParticleEnergy();  
    
    if (fTypeMomentum=="Wide")
      {
        G4cout << "Momentum before update = " << aMomAmp << " MeV/c" << endl;
        aMomAmp = (G4UniformRand()*(1000.) );
        //G4double Ekin = 500. * G4UniformRand();
        //aMomAmp = sqrt(Ekin * (Ekin + 2 * 939.564));
        fParticleGun->SetParticleMomentum(aMomAmp*CLHEP::MeV);
        cout << "Momentum after update = " << aMomAmp << " MeV/c" << endl;
        aEAmp = fParticleGun->GetParticleEnergy();

      }


    if(fTypeDirection=="Uniform"){      
      G4ThreeVector aDir = fParticleGun->GetParticleMomentumDirection();
      // Ko uses for angles [0,pi/2]
      G4double cosTheta = G4UniformRand(); //cosTheta in [0,1] --> theta in [0,pi/2]
      // full phase spase coverage [0,pi]
      //G4double cosTheta = G4UniformRand()*2. - 1; //cosTheta in [-1,1] --> theta in [0,pi]
      G4double phi = G4UniformRand()*360*CLHEP::deg; //flat in [0,2pi]
      G4double sinTheta = std::sqrt(1.-cosTheta*cosTheta);
      G4ThreeVector dir(sinTheta*std::cos(phi),sinTheta*std::sin(phi),cosTheta);
      //G4cout << "direction of the gun before update = " << aDir[0] << ", " << aDir[1] << ", " << aDir[2] <<endl;
      //G4cout << "After update (favour foward going particles)= " << dir[0] << ", " << dir[1] << ", " << dir[2] <<endl;
      G4cout << "Direction of the gun: " 
	     << dir[0] << ", " << dir[1] << ", " << dir[2] 
	     << G4endl;
      fParticleGun->SetParticleMomentumDirection(dir);
      //B.Q end
    }

    if(fTypeGun=="FCTest"){

    //Initial Momentum

    G4cout << "Momentum before update = " << aMomAmp << " MeV/c" << endl;
    aMomAmp = (G4UniformRand()*(500.) );
    fParticleGun->SetParticleMomentum(aMomAmp*CLHEP::MeV);
    cout << "Momentum after update = " << aMomAmp << " MeV/c" << endl;
    aEAmp = fParticleGun->GetParticleEnergy();

    //Initial Direction

    G4ThreeVector aDir = fParticleGun->GetParticleMomentumDirection();
    G4double cosTheta = G4UniformRand();
    G4double phi = 15*CLHEP::deg+G4UniformRand()*150*CLHEP::deg;
    G4double sinTheta = std::sqrt(1.-cosTheta*cosTheta);
    G4ThreeVector dir(0,sinTheta*std::sin(phi),sinTheta*std::cos(phi));
    G4cout << "Direction of the gun: " 
           << dir[0] << ", " << dir[1] << ", " << dir[2] 
	   << G4endl;
    fParticleGun->SetParticleMomentumDirection(dir);

    //Particle Type

    G4double particlerandom = 4*G4UniformRand();
    G4String particlename;
  
    if(particlerandom < 1) particlename = "e-"; 
    if(particlerandom >=1 && particlerandom <2) particlename = "mu-"; 
    if(particlerandom >=2 && particlerandom <3) particlename = "pi-";
    if(particlerandom >=3 && particlerandom <=4) particlename = "proton";

    G4ParticleDefinition* particleDefinition 
    = G4ParticleTable::GetParticleTable()->FindParticle(particlename);
    fParticleGun->SetParticleDefinition(particleDefinition);
    }
    
    fParticleGun->GeneratePrimaryVertex(anEvent);

    if (fTypePosition=="Gaus") {
      G4double x = anEvent->GetPrimaryVertex()->GetX0();
      G4double y = anEvent->GetPrimaryVertex()->GetY0();
      G4double z = anEvent->GetPrimaryVertex()->GetZ0();

      std::cout << "Old vertex position  " << x << "  " << y << "   " << z << std::endl;
      G4RandGauss* gaus_rand = new G4RandGauss(G4Random::getTheEngine(), 0, 40);
      x = x + gaus_rand->fire();
      y = y + gaus_rand->fire();

      std::cout << "New vertex position  " << x << "  " << y << "   " << z << std::endl;
      anEvent->GetPrimaryVertex()->SetPosition(x, y, z);
    }

    if (fTypeMomentum=="Gaus" && aEAmp != 0) {  
      G4double mass = fParticleGun->GetParticleDefinition()->GetPDGMass(); 
      G4double e_full = aEAmp + mass;
      aMomAmp = sqrt(e_full*e_full - mass*mass);
      std::cout << "Old momentum: " << aMomAmp << "  old energy " << aEAmp << "   mass " << mass << std::endl;
      
      G4RandGauss* gaus_rand = new G4RandGauss(G4Random::getTheEngine(), aMomAmp, 0.07 * aMomAmp);
      aMomAmp = gaus_rand->fire();
      aEAmp = sqrt(aMomAmp*aMomAmp + mass*mass) - mass;
      std::cout << "New momentum: " << aMomAmp << "    Energy: " << aEAmp << std::endl;
      if (anEvent->GetPrimaryVertex()->GetNumberOfParticle() != 1)
        std::cout << "CAUTION!!!! More then 1 particle" << std::endl;
      anEvent->GetPrimaryVertex()->GetPrimary()->SetKineticEnergy(aEAmp);
      //fParticleGun->SetParticleMomentum(aMomAmp*CLHEP::MeV);
    }
  }
  
  else if(fGeneratorType=="Generator"){ // Read NEUT or GENIE output (same format)

    //
    // Check that the # of G4 envents is <= # of NEUT events
    // It must be done here because in order to get the tot # of events
    // the G4Run object is needed, but it's initialized only at the beginning of the first event
    // (see http://hypernews.slac.stanford.edu/HyperNews/geant4/get/runmanage/175/1.html)
    //
    // Also use current event ID from NEUT tree object, since the event is not initialized yet
    //
    
    // G4int CurrNEUTEvtID = RooTrackerNEUT.GetCurrNEUTevent();
    // if(CurrNEUTEvtID==0){      
    //   G4int TotG4events = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed(); // Number passed to BeamOn in vis.mac
    //   G4int TotNEUTevents = RooTrackerNEUT.GetTotNEUTevents();      
    //   if(TotG4events > TotNEUTevents){	
    // 	G4cerr << G4endl;
    // 	G4cerr << "The total # of G4 events exceeds the # of available NEUT events" << G4endl;
    // 	G4cerr << " - # G4 events = " << TotG4events << G4endl; 
    // 	G4cerr << " - # NEUT events = " << TotNEUTevents << G4endl;
    // 	G4cerr << G4endl;	
    // 	const char *msg = "The # of G4 events is larger than # of available NEUT events!";
    // 	const char *origin = "ExN02PrimaryGeneratorAction::GeneratePrimaries";
    // 	const char *code = "if(CurrNEUTEvtID==0){";
    // 	G4Exception(origin,code,FatalException,msg);   
    //   }  
    // }

    
    // Generate the primary vertex
    RooTrackerNEUT.GeneratePrimaryVertex(anEvent);   
  }  

  else{
    G4ExceptionDescription msg;
    msg << "Generator type has not been set!!!" << G4endl;
    G4Exception("ExN02PrimaryGeneratorAction::GeneratePrimaries()",
		"MyCode0002",FatalException, msg);        
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


