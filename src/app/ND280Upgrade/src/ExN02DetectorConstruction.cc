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
// $Id: ExN02DetectorConstruction.cc,v 1.22 2010-01-22 11:57:03 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
#include "ExN02DetectorConstruction.hh"
#include "ExN02DetectorMessenger.hh"
#include "ExN02ChamberParameterisation.hh"
#include "ExN02MagneticField.hh"
#include "ExN02TrackerSD.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4SDManager.hh"
#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4PVReplica.hh"

#include "G4NistManager.hh"

#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
ExN02DetectorConstruction::ExN02DetectorConstruction()
:solidWorld(0),  logicWorld(0),  physiWorld(0),
 solidTarget(0), logicTarget(0), physiTarget(0), 
 solidTracker(0),logicTracker(0),physiTracker(0), 
 solidChamber1(0),logicChamber1(0),physiChamber1(0), 
 solidChamber2(0),logicChamber2(0),physiChamber2(0), 
 TargetMater(0), ChamberMater(0), //chamberParam(0),
 stepLimit(0), fpMagField(0),
 fWorldLength(0.), fWorldWidth(0.), fWorldHeight(0.),
 fTargetLength(0.),fTargetWidth(0.),fTargetHeight(0.),
 fTrackerLength(0.),
 NbOfChambers(0) ,  ChamberWidth(0.),  ChamberSpacing(0.)
{
  fpMagField = new ExN02MagneticField();
  detectorMessenger = new ExN02DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
ExN02DetectorConstruction::~ExN02DetectorConstruction()
{
  delete fpMagField;
  delete stepLimit;
  //delete chamberParam;
  delete detectorMessenger;             
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4VPhysicalVolume* ExN02DetectorConstruction::Construct()
{
  //--------- Material definition ---------

  G4String name, symbol;
  G4int nel, ncomponents, natoms;
  G4double fractionmass;
  G4double a, z;
  G4double density, temperature, pressure;

  G4NistManager* nistMan = G4NistManager::Instance();
  
  G4Element* elH = nistMan->FindOrBuildElement(1);
  G4Element* elC = nistMan->FindOrBuildElement(6);
  G4Element* elO = nistMan->FindOrBuildElement(8);
  G4Element* elAr = nistMan->FindOrBuildElement(18);

  //Air
  G4Element* N = new G4Element("Nitrogen", "N", z=7., a= 14.01*g/mole);
  G4Element* O = new G4Element("Oxygen"  , "O", z=8., a= 16.00*g/mole);
   
  G4Material* Air = new G4Material("Air", density= 1.29*mg/cm3, nel=2);
  Air->AddElement(N, 70*perCent);
  Air->AddElement(O, 30*perCent);

  //Lead
  G4Material* Pb = new G4Material("Lead", z=82., a= 207.19*g/mole, density= 11.35*g/cm3);
  
  //Xenon gas
  G4Material* Xenon = new G4Material("XenonGas", z=54., a=131.29*g/mole, density= 5.458*mg/cm3,
				     kStateGas, temperature= 293.15*kelvin, pressure= 1*atmosphere);
  
  //Carbon
  G4Material* C = new G4Material("Carbon", z=12., a=131.29*g/mole, density= 5.458*mg/cm3,
				 kStateGas, temperature= 293.15*kelvin, pressure= 1*atmosphere);
    
  //Water                           
  density = 1.0*g/cm3;
  G4Material* water = new G4Material(name="Water", density, nel=2);
  water->AddElement(elH, natoms=2);
  water->AddElement(elO, natoms=1);
 
  //Argon                                     
  density     = 1.66*mg/cm3;
  pressure    = 1*atmosphere;
  temperature = 293.15*kelvin;
  G4Material* argon =  new G4Material(name="Ar", density,
				      nel=1,kStateGas,temperature,
				      pressure);
  argon->AddElement(elAr, natoms=1);
 
  //Methane      
  density     = 0.667*mg/cm3;
  pressure    = 1*atmosphere;
  temperature = 293.15*kelvin;
  G4Material* methane = new G4Material(name="CH4", density,
				       nel=2,kStateGas,temperature,
				       pressure);
  methane->AddElement(elC, natoms=1);
  methane->AddElement(elH, natoms=4);
    
  //Argon + 10% Methane                            
  density     = 2.33*mg/cm3;
  G4Material* gasMixture
    = new G4Material(name="GasMixture", density, ncomponents=2);
  gasMixture->AddMaterial(argon, fractionmass = 90*perCent);
  gasMixture->AddMaterial(methane, fractionmass = 10*perCent);
  
  // Print all the materials defined.
  //
  G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;






  
  //--------- Sizes of the principal geometrical components (solids)  ---------
  
  fTargetLength  = 240.0 * cm;      // Full length of Target 
  fTargetWidth   = 230.0 * cm;      // Full width of Target 
  fTargetHeight  = 30.3 * cm;       // Full length of Target 
  
  NbOfChambers = 2;

  //ChamberWidth = 200*cm;
  ChamberLength = fTargetLength;
  ChamberWidth  = fTargetWidth;
  ChamberHeight = 100*cm;

  fTrackerLength = fTargetLength;                 // Full length of Tracker
  fTrackerWidth  = fTargetWidth;                  // Full width of Tracker
  fTrackerHeight = fTargetHeight+ChamberHeight*2; // Full height of Tracker
  
  TargetMater  = water; 
  ChamberMater = gasMixture; // gas mixture of ND280 TPCs
  
  fWorldLength= 1.2 *(fTrackerLength);
  fWorldWidth = 1.2 *(fTrackerWidth);
  fWorldHeight= 1.2 *(fTrackerHeight);
   
  G4double targetSizeLength  = 0.5*fTargetLength;    // Half length of the Target 
  G4double targetSizeWidth   = 0.5*fTargetWidth;     // Half width of the Target 
  G4double targetSizeHeight  = 0.5*fTargetHeight;    // Half height of the Target 
 
  G4double trackerSizeLenght = 0.5*fTrackerLength;  // Half length of the Tracker
  G4double trackerSizeWidth  = 0.5*fTrackerWidth;   // Half width of the Tracker
  G4double trackerSizeHeight = 0.5*fTrackerHeight;  // Half height of the Tracker
      
  //--------- Definitions of Solids, Logical Volumes, Physical Volumes ---------
  
  //------------------------------ 
  // World
  //------------------------------ 

  G4double HalfWorldLength = 0.5*fWorldLength;
  G4double HalfWorldWidth  = 0.5*fWorldWidth;
  G4double HalfWorldHeight = 0.5*fWorldHeight;
 
  G4GeometryManager::GetInstance()->SetWorldMaximumExtent(fWorldLength);
  G4cout << "Computed tolerance = "
         << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
         << " mm" << G4endl;

  solidWorld= new G4Box("world",HalfWorldWidth,HalfWorldHeight,HalfWorldLength);

  logicWorld= new G4LogicalVolume( solidWorld, Air, "World", 0, 0, 0);
  
  //  Must place the World Physical volume unrotated at (0,0,0).
  // 
  physiWorld = new G4PVPlacement(0,               // no rotation
                                 G4ThreeVector(), // at (0,0,0)
                                 logicWorld,      // its logical volume
				 "World",         // its name
                                 0,               // its mother  volume
                                 false,           // no boolean operations
                                 0);              // copy number

  G4cout << "World is " 
  	 << fWorldHeight/cm << " x " 
  	 << fWorldWidth/cm  << " x " 
  	 << fWorldLength/cm << " cm^3 of " 
  	 << G4endl;
				 
  //------------------------------ 
  // Target
  //------------------------------

  G4double HalfTargetHeight  = 0.5 * fTargetHeight;
  
  G4ThreeVector positionTarget = G4ThreeVector(0,0,0);
  

  solidTarget = new G4Box("target",targetSizeWidth,targetSizeHeight,targetSizeLength);
  
  logicTarget = new G4LogicalVolume(solidTarget,TargetMater,"Target",0,0,0);
  physiTarget = new G4PVPlacement(0,               // no rotation
  				  positionTarget,  // at (x,y,z)
  				  logicTarget,     // its logical volume  	  
  				  "Target",        // its name
  				  logicWorld,      // its mother  volume
  				  false,           // no boolean operations
  				  0);              // copy number 
  
  G4cout << "Target is " 
  	 << fTargetHeight/cm << " x " 
  	 << fTargetWidth/cm  << " x " 
  	 << fTargetLength/cm << " cm^3" 
         << TargetMater->GetName() 
  	 << G4endl;


  //------------------------------ 
  // Tracker
  //------------------------------
  
  G4double HalfTrackerLength = 0.5*(fTrackerLength); 
  G4double HalfTrackerWidth  = 0.5*(fTrackerWidth); 
  G4double HalfTrackerHeight = 0.5*(fTrackerHeight); 

  //G4ThreeVector positionTracker = G4ThreeVector(0,HalfTrackerHeight,0);
  G4ThreeVector positionTracker = G4ThreeVector(0,0,0);

  solidTracker = new G4Box("tracker",HalfTrackerWidth,HalfTrackerHeight,HalfTrackerLength);
  logicTracker = new G4LogicalVolume(solidTracker , Air, "Tracker",0,0,0);  
  physiTracker = new G4PVPlacement(0,              // no rotation
				   positionTracker, // at (x,y,z)
				   logicTracker,    // its logical volume	 
				   "Tracker",       // its name
				   logicWorld,      // its mother  volume
				   false,           // no boolean operations
				   0);              // copy number 
  
  G4cout << "Tracker is " 
  	 << fTrackerHeight/cm << " x " 
  	 << fTrackerWidth/cm  << " x " 
  	 << fTrackerLength/cm << " cm^3 of " 
  	 << G4endl;
 
  
  //------------------------------ 
  // Tracker segments
  //------------------------------
  // 
  // An example of Parameterised volumes
  // dummy values for G4Box -- modified by parameterised volume
  
  G4double HalfChamberLength = 0.5 * ChamberLength;
  G4double HalfChamberWidth  = 0.5 * ChamberWidth;
  G4double HalfChamberHeight = 0.5 * ChamberHeight;
  
  solidChamber1 = new G4Box("chamber1", HalfChamberWidth, HalfChamberHeight, HalfChamberLength); 
  logicChamber1 = new G4LogicalVolume(solidChamber1,ChamberMater,"Chamber1",0,0,0);

  ChamberSpacing = fTargetHeight;
  
  G4ThreeVector positionChamber1 = G4ThreeVector(0,
						(HalfChamberHeight+HalfTargetHeight),
						0);

  physiChamber1 = new G4PVPlacement(0,              // no rotation
				   positionChamber1, // at (x,y,z)
				   logicChamber1,    // its logical volume	 
				   "TPC Up",       // its name
				   logicTracker,      // its mother  volume
				   false,           // no boolean operations
				   0);              // copy number 


  
  solidChamber2 = new G4Box("chamber2", HalfChamberWidth, HalfChamberHeight, HalfChamberLength); 
  logicChamber2 = new G4LogicalVolume(solidChamber2,ChamberMater,"Chamber2",0,0,0);
  
  G4ThreeVector positionChamber2 = G4ThreeVector(0,
						-(HalfChamberHeight+HalfTargetHeight),
						0);

  physiChamber2 = new G4PVPlacement(0,              // no rotation
				   positionChamber2, // at (x,y,z)
				   logicChamber2,    // its logical volume	 
				   "TPC Down",       // its name
				   logicTracker,      // its mother  volume
				   false,           // no boolean operations
				   0);              // copy number 

  
  G4cout << "Chamber is " 
  	 << ChamberHeight/cm << " x " 
  	 << ChamberWidth/cm  << " x " 
  	 << ChamberLength/cm << " cm^3 of " 
         << ChamberMater->GetName() 
  	 << G4endl;

  // physiChamber = new G4PVReplica(
  // 				 "Chamber",       // their name
  // 				 logicChamber,    // their logical volume
  // 				 logicTracker,    // Mother logical volume
  // 				 kYAxis,          // Are placed along this axis 
  // 				 NbOfChambers,    // Number of chambers
  // 				 ChamberWidth,    // Width of a chamber
  // 				 0.0); // Spacing between chambers 
  //ChamberSpacing); // Spacing between chambers 
 
  // G4double firstPosition = -trackerSizeLenght + 0.5*ChamberWidth;  
  // G4double firstLength = fTrackerLength/10;
  // G4double lastLength  = fTrackerLength;
  
  //chamberParam = new ExN02ChamberParameterisation(  
  // 			   NbOfChambers,          // NoChambers 
  // 			   firstPosition,         // Z of center of first 
  // 			   ChamberSpacing,        // Z spacing of centers
  // 			   ChamberWidth,          // Width Chamber 
  // 			   firstLength,           // lengthInitial 
  // 			   lastLength);           // lengthFinal
			   
  // // dummy value : kZAxis -- modified by parameterised volume
  // //
  // physiChamber = new G4PVParameterised(
  //                           "Chamber",       // their name
  //                           logicChamber,    // their logical volume
  //                           logicTracker,    // Mother logical volume
  // 			    kZAxis,          // Are placed along this axis 
  //                           NbOfChambers,    // Number of chambers
  //                           chamberParam);   // The parametrisation

  // G4cout << "There are " << NbOfChambers << " chambers in the tracker region. "
  //        << "The chambers are " << ChamberWidth/mm << " mm of " 
  //        << ChamberMater->GetName() << "\n The distance between chamber is "
  // 	 << ChamberSpacing/cm << " cm" << G4endl;
	 



  //------------------------------------------------ 
  // Sensitive detectors
  //------------------------------------------------ 
  
  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  G4String trackerChamberSDname = "ExN02/TrackerChamberSD";
  ExN02TrackerSD* aTrackerSD = new ExN02TrackerSD( trackerChamberSDname );
  SDman->AddNewDetector( aTrackerSD );
  logicChamber1->SetSensitiveDetector( aTrackerSD );
  logicChamber2->SetSensitiveDetector( aTrackerSD );

  //--------- Visualization attributes -------------------------------

  G4VisAttributes* BoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  logicWorld  ->SetVisAttributes(BoxVisAtt);  
  logicTracker->SetVisAttributes(BoxVisAtt);

  G4VisAttributes* TargetVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  logicTarget ->SetVisAttributes(TargetVisAtt);
  
  G4VisAttributes* ChamberVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  logicChamber1->SetVisAttributes(ChamberVisAtt);
  logicChamber2->SetVisAttributes(ChamberVisAtt);
  
  //--------- example of User Limits -------------------------------

  // below is an example of how to set tracking constraints in a given
  // logical volume(see also in N02PhysicsList how to setup the processes
  // G4StepLimiter or G4UserSpecialCuts).
  
  // Sets a max Step length in the tracker region, with G4StepLimiter
  //
  //G4double maxStep = 0.5*ChamberWidth;
  //stepLimit = new G4UserLimits(maxStep);
  //logicTracker->SetUserLimits(stepLimit);
  
  // Set additional contraints on the track, with G4UserSpecialCuts
  //
  // G4double maxLength = 2*fTrackerLength, maxTime = 0.1*ns, minEkin = 10*MeV;
  // logicTracker->SetUserLimits(new G4UserLimits(maxStep,maxLength,maxTime,
  //                                               minEkin));
  
  return physiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void ExN02DetectorConstruction::setTargetMaterial(G4String materialName)
{
  // search the material by its name 
  G4Material* pttoMaterial = G4Material::GetMaterial(materialName);  
  if (pttoMaterial)
     {TargetMater = pttoMaterial;
      logicTarget->SetMaterial(pttoMaterial); 
      G4cout << "\n----> The target is " << fTargetLength/cm << " cm of "
             << materialName << G4endl;
     }             
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02DetectorConstruction::setChamberMaterial(G4String materialName)
{
  // search the material by its name 
  G4Material* pttoMaterial = G4Material::GetMaterial(materialName);  
  if (pttoMaterial)
     {ChamberMater = pttoMaterial;
      logicChamber1->SetMaterial(pttoMaterial); 
      logicChamber2->SetMaterial(pttoMaterial); 
      G4cout << "\n----> The chambers are " << ChamberWidth/cm << " cm of "
             << materialName << G4endl;
     }             
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void ExN02DetectorConstruction::SetMagField(G4double fieldValue)
{
  fpMagField->SetMagFieldValue(fieldValue);  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02DetectorConstruction::SetMaxStep(G4double maxStep)
{
  if ((stepLimit)&&(maxStep>0.)) stepLimit->SetMaxAllowedStep(maxStep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
