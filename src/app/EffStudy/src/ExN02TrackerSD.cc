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
// $Id: ExN02TrackerSD.cc,v 1.9 2006-06-29 17:48:27 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "ExN02TrackerSD.hh"
#include "ExN02Constants.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "G4Navigator.hh"
#include "G4NavigationHistory.hh"

#include "ND280RootPersistencyManager.hh"

#include "ExN02TargReadOut.hh"

#include <TH2F.h>

#include "ExN02ApplyResponse.hh"

//#define DEBUG

using namespace conv;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02TrackerSD::ExN02TrackerSD(G4String name)
 :G4VSensitiveDetector(name),
  trackerCollection(NULL), HCID(-1),
  fMaximumHitSagitta(1*CLHEP::mm), fMaximumHitLength(10*CLHEP::mm),
  fLastHit(0) {
  
  G4String HCname;
  collectionName.insert(HCname="trackerCollection");

  trackerResponse = new ExN02TrackerResponse(); // B.Q used for WAGASCI only 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02TrackerSD::~ExN02TrackerSD(){ 
  if(trackerResponse!=NULL) delete trackerResponse; // B.Q
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02TrackerSD::Initialize(G4HCofThisEvent* HCE)
{
  trackerCollection = new ExN02TrackerHitsCollection
                          (SensitiveDetectorName,collectionName[0]); 
  //static G4int HCID = -1; // NEW HitSegment
  if(HCID<0)
  { HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); }
  HCE->AddHitsCollection( HCID, trackerCollection ); 
  
  NSteps_ = 0;

  SetMaximumHitSagitta(1*CLHEP::mm);
  SetMaximumHitLength(10*CLHEP::mm);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool ExN02TrackerSD::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{

  // Take inputs
  ND280RootPersistencyManager* InputPersistencyManager
    = ND280RootPersistencyManager::GetInstance();
  ND280XMLInput = InputPersistencyManager->GetXMLInput();
  //

  G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep==0.) return false;

  
  //G4cout << "ExN02TrackerSD::ProcessHits" << G4endl;

  //return false;

  // COMMENT this line to store all the hits
  if(aStep->GetTrack()->GetParentID()>0) return false; // Look only at Primary Tracks!!!
  
  //if(aStep->GetTrack()->GetParentID()==0){
  
      
  
  
  //NSteps_++;
  
  ///// NEW ND280 HitSegment

  ExN02TrackerHit* currentHit = NULL;

  // Find the hit corresponding to the current step
  if (0<=fLastHit && fLastHit < trackerCollection->entries()) {
    ExN02TrackerHit *tmpHit = (*trackerCollection)[fLastHit];
    if (tmpHit->SameHit(aStep)) {
      currentHit = tmpHit;
    }
  }


  /////

  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  G4StepPoint* postStepPoint = aStep->GetPostStepPoint();
  
  
  // Calculate step length in each direction
  double postposX = postStepPoint->GetPosition().x();
  double postposY = postStepPoint->GetPosition().y();
  double postposZ = postStepPoint->GetPosition().z();

  double preposX = preStepPoint->GetPosition().x();
  double preposY = preStepPoint->GetPosition().y();
  double preposZ = preStepPoint->GetPosition().z();
  
  double deltaX = (postposX - preposX)/2.;
  double deltaY = (postposY - preposY)/2.;
  double deltaZ = (postposZ - preposZ)/2.;


#ifdef DEBUG 
  G4cout << "pre: " << preposX << ", "<< preposY << ", " << preposZ<< G4endl;
  G4cout << "post: " << postposX << ", " << postposY << ", " << postposZ << G4endl;
  G4cout << "delta: " << deltaX << ", " << deltaY << ", " << deltaZ << G4endl;
#endif
  
  // Volume information must be extracted from Touchable of "PreStepPoint"

  G4TouchableHandle theTouchable = preStepPoint->GetTouchableHandle();
  G4String touch_namedet = theTouchable->GetVolume()->GetLogicalVolume()->GetName();
  G4VPhysicalVolume * worldVolumePointer = theTouchable->GetHistory()->GetVolume(0);
 
  
  // Take the position in the local coordinate system (defined in DetectorConstruction)

  ND280RootPersistencyManager* persistencyManager
    = ND280RootPersistencyManager::GetInstance();

  if( persistencyManager->doNavigDetExist() ){

#ifdef DEBUG
    G4cout << "Persistency manager exists " << G4endl ;
#endif
    
    if( !persistencyManager->GetNavigTarg1() ){  // initialize only once   
      persistencyManager->InitNavigator(worldVolumePointer,preStepPoint->GetPosition()); 
    }
    
    if( !persistencyManager->GetHistoMovedTarg1() ){
      G4Exception("ExN02TrackerSD::ProcessHits",
		  "MyCode0002",FatalException,
		  "Navigator is initialized but no movement through history has been done!");    
    }
    
    //G4cout << "The local coordinate system is: "  << persistencyManager->GetNavigHistoVolName() << G4endl;


    /*

    //This piece of code is not used

    const G4NavigationHistory *pmHistory = persistencyManager->GetNavigHistoTarg1();
    
    G4String pm_namedet_0 = pmHistory->GetVolume(0)->GetName();
    G4String pm_namedet_2 = pmHistory->GetVolume(2)->GetName(); 
    G4int pm_historyDepth = pmHistory->GetDepth();
    G4int pm_maxDepth     = pmHistory->GetMaxDepth();
    G4String pm_namedet   = pmHistory->GetVolume(pm_historyDepth)->GetName();
    
    //G4cout << " "
    //<< "pm_historyDepth = " << pm_historyDepth << " --> " << pmHistory->GetVolume(pm_historyDepth)->GetLogicalVolume()->GetName() << G4endl
    //<< " - touch_namedet = " << touch_namedet << G4endl
    //<< " - pm_namedet_0 = " << pm_namedet_0
    //<< " - pm_namedet_2 = " << pm_namedet_2
    //<< G4endl;
    
    G4ThreeVector PMworldPosition = preStepPoint->GetPosition();
    //G4ThreeVector PMlocalPosition = pmHistory->
    //GetTopTransform().TransformPoint(PMworldPosition);
    //G4cout << "PMlocalPosition: " << PMlocalPosition << G4endl;
    G4ThreeVector PMlocalPosition = persistencyManager->GetLocalPosition(PMworldPosition);
    //G4cout << "PMlocalPosition: " << PMlocalPosition << G4endl;
    
    //  G4cout << "PMWorldPosition = " 
    //	 << PMworldPosition.x() << ", "
    //	 << PMworldPosition.y() << ", "
    //	 << PMworldPosition.z() << G4endl;
    //
    // G4cout << "PMLocPosition = " 
    // 	 << PMlocalPosition.x() << ", "
    // 	 << PMlocalPosition.y() << ", "
    // 	 << PMlocalPosition.z() << G4endl;
    // G4cout << G4endl;
    
          
    // Get the light position in the local frame and 
    // calculate the position of the correspding MPPC (at it's bin center)     
    double lightX = PMlocalPosition.x() + deltaX;
    double lightY = PMlocalPosition.y() + deltaY;
    double lightZ = PMlocalPosition.z() + deltaZ;
    G4ThreeVector LightPos = G4ThreeVector(lightX,lightY,lightZ);

    double mppcX = -9999999999;
    double mppcY = -9999999999;
    double mppcZ = -9999999999;

    int detID = kBadNum;
    G4ThreeVector LightBarPos(kBadNum,kBadNum,kBadNum); // middle step pos in bar system (used for WAGASCI)

    G4Track* track       = aStep->GetTrack();  
    G4int trackid        = track->GetTrackID();
    G4int parentid       = track->GetParentID();
    G4double particle    = track->GetDefinition()->GetPDGEncoding();
    G4double prestepTime = preStepPoint->GetGlobalTime() * ns;
    G4double steplength = aStep->GetStepLength();
    G4double charge = track->GetDefinition()->GetPDGCharge();
    */
    
    



    ///// NEW ND280 HitSegment    
    
    // #define SEPARATE_STEPS
#ifndef SEPARATE_STEPS
    // Look through the list of available hits and see the new hit should be
    // added to an existing one.
    do {
      // We have already found a hit to add this one too, so short-circuit
      // the search.
      if (currentHit) break;
      //G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
      const G4VProcess* preProcess = preStepPoint->GetProcessDefinedStep();
      G4ProcessType preProcessType = fNotDefined;
      if (preProcess) preProcessType = preProcess->GetProcessType();
      //G4StepPoint* postStepPoint = aStep->GetPostStepPoint();
      const G4VProcess* postProcess = postStepPoint->GetProcessDefinedStep();
      G4ProcessType postProcessType = fNotDefined;
      if (postProcess) postProcessType = postProcess->GetProcessType();
      // If the prestep is of type fTransporation, then we need a new step
      // since we are crossing a geometry boundary.
      if (preProcessType == fTransportation) break;
      // Try and find the hit in list of existing hits.
      for (int hitNumber = trackerCollection->entries()-1; 
	   0 <= hitNumber;
	   --hitNumber) {
	ExN02TrackerHit *tmpHit = (*trackerCollection)[hitNumber];
	if (tmpHit->SameHit(aStep)) {
	  currentHit = tmpHit;
	  fLastHit = hitNumber;
	  //ND280NamedVerbose("hit","Add step to " << hitNumber);
	  break;
	}
      }
    } while (false);
#endif
    
    // If a hit wasn't found, create one.
    if (!currentHit) {
      currentHit = new ExN02TrackerHit(fMaximumHitSagitta,fMaximumHitLength);
      fLastHit = trackerCollection->entries();
      trackerCollection->insert(currentHit);
    }
  
    currentHit->AddStep(aStep);

  } // if( persistencyManager->GetNavigDetName_Targ1()!="" )






  
  
  
  



  /*

  //////////////// My Navigator ////////////////
  
  G4Navigator* myNavigator = new G4Navigator();
  myNavigator->SetWorldVolume(worldVolumePointer);
  myNavigator->LocateGlobalPointAndSetup(preStepPoint->GetPosition()); // Set it to lowest daughter that contains that position
  G4TouchableHistoryHandle myTouchable =
    myNavigator->CreateTouchableHistoryHandle();

  
  G4int PMdepth = myTouchable->GetHistory()->GetDepth();
  for (G4int i = 0; i<PMdepth-1; ++i) {
    G4String touch_detname = myTouchable->GetVolume()->GetLogicalVolume()->GetName();
    G4cout << "touch_detname = " << touch_detname << G4endl;
    if(touch_detname == "/t2k/OA/Magnet/Basket/Target1") break;    
    myTouchable->MoveUpHistory(1);
  }
  

  G4int my_detID_0 = myTouchable->GetCopyNumber(0); // "/t2k/.../SuperFGD1/CubeScint/Core"
  G4int my_detID_2 = myTouchable->GetCopyNumber(2); // "/t2k/.../SuperFGD1/cubeDirZ"
  G4String my_namedet_0 = myTouchable->GetVolume(0)->GetName();
  G4String my_namedet_2 = myTouchable->GetVolume(2)->GetName(); 
  G4int my_historyDepth = myTouchable->GetHistory()->GetDepth();
  G4int my_maxDepth     = myTouchable->GetHistory()->GetMaxDepth();

  //G4cout << my_namedet_0 << " " << my_namedet_2 << G4endl;
  
  //G4cout << G4endl;
  //G4cout << "my_historyDepth = " << my_historyDepth << " --> " << myTouchable->GetHistory()->GetVolume(my_historyDepth)->GetLogicalVolume()->GetName() << G4endl
  //<< G4endl;  

  G4ThreeVector myworldPosition = preStepPoint->GetPosition();
  G4ThreeVector mylocalPosition = myTouchable->GetHistory()->
    GetTopTransform().TransformPoint(myworldPosition);
  
  G4cout << "myWorldPosition = " 
	 << myworldPosition.x() << ", "
	 << myworldPosition.y() << ", "
	 << myworldPosition.z() << G4endl;

  G4cout << "myLocPosition = " 
	 << mylocalPosition.x() << ", "
	 << mylocalPosition.y() << ", "
	 << mylocalPosition.z() << G4endl;
  G4cout << G4endl;


  if(PMlocalPosition.x() != mylocalPosition.x() || 
     PMlocalPosition.y() != mylocalPosition.y() ||
     PMlocalPosition.z() != mylocalPosition.z()
     ){

    G4cout << "PMlocalPosition: " 
	   << PMlocalPosition.x() << " " 
	   << PMlocalPosition.y() << " " 
	   << PMlocalPosition.z() << " " 
	   << G4endl;

    G4cout << "mylocalPosition: " 
	   << mylocalPosition.x() << " " 
	   << mylocalPosition.y() << " " 
	   << mylocalPosition.z() << " " 
	   << G4endl;
    
    exit(1);
  }
*/


  
  /////////////////////////////////////////////////////
  //                                                 //
  // N.B. MyNavigator should be deleted by myself!!! //
  //                                                 //
  /////////////////////////////////////////////////////

  /////////////////////////////////// 



  /*

  G4int detID_0 = theTouchable->GetCopyNumber(0); // "/t2k/.../SuperFGD1/CubeScint/Core"
  G4int detID_2 = theTouchable->GetCopyNumber(2); // "/t2k/.../SuperFGD1/cubeDirZ"
  G4String namedet_0 = theTouchable->GetVolume(0)->GetName();
  G4String namedet_2 = theTouchable->GetVolume(2)->GetName(); 
  G4int historyDepth = theTouchable->GetHistory()->GetDepth();
  G4int maxDepth     = theTouchable->GetHistory()->GetMaxDepth();
  
  G4cout << "historyDepth = " << historyDepth << " --> " << theTouchable->GetHistory()->GetVolume(historyDepth)->GetLogicalVolume()->GetName() << G4endl
	 << G4endl;  
  
  // Look at http://geant4.cern.ch/support/faq.shtml
  G4ThreeVector worldPosition = preStepPoint->GetPosition();
  G4ThreeVector localPosition = theTouchable->GetHistory()->
    GetTopTransform().TransformPoint(worldPosition);
  
  G4cout << "WorldPosition = " 
	 << worldPosition.x() << ", "
	 << worldPosition.y() << ", "
	 << worldPosition.z() << G4endl;

  G4cout << "LocPosition = " 
	 << localPosition.x() << ", "
	 << localPosition.y() << ", "
	 << localPosition.z() << G4endl;
  G4cout << G4endl;

  */

  // G4int nphysdaught = Touchable->GetVolume(0)->GetNoDaughters();
  // for(int i=0;i<nphysdaught;i++){
  //   G4VPhysicalVolume *PhysDaughter = Touchable->GetVolume(0)->GetDaughter(i);    
  // }
  
  /*  
  G4cout << "detID_0 = " << detID_0 << " --> " << namedet_0 
	 << " / detID_2 = " << detID_2 << " --> " << namedet_2
	 << G4endl;
  */

  /*

  // The geometry is stored in the PreStepPoint()
  
  G4StepPoint* prestep  = aStep->GetPreStepPoint();
  G4StepPoint* poststep = aStep->GetPostStepPoint();
  G4Track* track        = aStep->GetTrack();
  
  G4double steplength   = aStep->GetStepLength(); // step length corrected w/ multiple scattering

  // Get geometrical step length on yz plane (transverse). Not corrected w/ multiple scattering

  G4ThreeVector deltapos = aStep->GetDeltaPosition(); // geometrical step length (not corrected)
  G4double deltalength = deltapos.mag();

  G4ThreeVector prestepPos = prestep->GetPosition();
  G4ThreeVector poststepPos = poststep->GetPosition();

  G4double prestep_x = prestepPos.x() / CLHEP::mm;
  G4double prestep_y = prestepPos.y() / CLHEP::mm;
  G4double prestep_z = prestepPos.z() / CLHEP::mm;

  G4double poststep_x = poststepPos.x() / CLHEP::mm;
  G4double poststep_y = poststepPos.y() / CLHEP::mm;
  G4double poststep_z = poststepPos.z() / CLHEP::mm;
  
  G4double stepdeltalyz = sqrt( (prestep_y - poststep_y)*(prestep_y - poststep_y) +
				(prestep_z - poststep_z)*(prestep_z - poststep_z) );

  // G4double steplength_oth = sqrt( (prestep_x - poststep_x)*(prestep_x - poststep_x) +
  //(prestep_y - poststep_y)*(prestep_y - poststep_y) +
  //(prestep_z - poststep_z)*(prestep_z - poststep_z) );

  //if( fabs(steplength_oth-steplength) > 0.0001){
  //G4cout << steplength << ", " 
  //<< deltalength << ", "
  //	   << steplength_oth << ": " 
  //	   << (prestep_x - poststep_x) << ", "
  //	   << (prestep_y - poststep_y) << ", "
  //	   << (prestep_z - poststep_z) << ", "
  //	   << G4endl;
  //}

  //if (prestep->GetStepStatus() == fGeomBoundary)  return false;
  //if (poststep->GetStepStatus() == fGeomBoundary) return false;  

  //
  // Use the preStep because the SDManager is referred to the preStep detector!!!
  //
  G4String namedet     = prestep->GetTouchableHandle()->GetVolume()->GetName();
  //G4String namedet     = poststep->GetTouchableHandle()->GetVolume()->GetName();  
  
  //G4ThreeVector mom    = track->GetMomentum(); // gives poststep momentum --> WRONG!!!
  G4ThreeVector mom    = prestep->GetMomentum(); // gives prestep momentum
  
  G4double charge      = track->GetDefinition()->GetPDGCharge();
  G4int trackid        = track->GetTrackID();
  G4int parentid       = track->GetParentID();
  G4int trackpdg       = track->GetDefinition()->GetPDGEncoding();
  G4String trackname   = track->GetDefinition()->GetParticleName();
  
  //G4double tracklength = track->GetTrackLength(); // use sum of step lengths!!!
  G4double tracklength = 0.;
  
  G4double trackcosth = prestep->GetMomentumDirection().z(); // wrt nu direction (0,0,1)

  // if(aStep->IsFirstStepInVolume()){
  //   G4cout << "It's the first step" << G4endl; 
  // }
  // else if(aStep->IsLastStepInVolume()){
  //   G4cout << "It's the last step" << G4endl;     
  // }  

  ExN02TrackerHit* newHit = new ExN02TrackerHit();
  //newHit->SetChamberNb(aStep->GetPreStepPoint()->GetTouchableHandle()
  //->GetCopyNumber());
 
  newHit->SetTrackID    (trackid);
  newHit->SetParentID   (parentid);
  newHit->SetEdep       (edep);
  newHit->SetMom        (mom); 
  newHit->SetPreStepPos (prestepPos);
  newHit->SetPostStepPos(poststepPos);
  newHit->SetNameDet    (namedet);
  newHit->SetCharge     (charge);
  newHit->SetTrackName  (trackname);
  newHit->SetTrackPDG   (trackpdg);
  newHit->SetTrackLength(tracklength);
  newHit->SetNSteps     (NSteps_);
  newHit->SetStepLength (steplength);
  newHit->SetStepDeltaLyz (stepdeltalyz);
  newHit->SetTrackCosTheta (trackcosth);

  trackerCollection->insert( newHit );
  
  //newHit->Print();
  //newHit->Draw();  
  //}

  */




  NSteps_++;

  //  }
  
  //G4cout << "End of ExN02TrackerSD::ProcessHits()" << G4endl;
    
  return true;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02TrackerSD::EndOfEvent(G4HCofThisEvent*)
{

  if (verboseLevel>0) { 
    G4int NbHits = trackerCollection->entries();
    G4cout << "\n-------->Hits Collection: in this event they are " << NbHits 
	   << " hits in the tracker chambers: " << G4endl;
    //for (G4int i=0;i<NbHits;i++) (*trackerCollection)[i]->Print();
  } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

