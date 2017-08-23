#include <globals.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VisAttributes.hh>
#include <G4PVPlacement.hh>

#include <G4RotationMatrix.hh>

#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Trap.hh>
#include <G4SubtractionSolid.hh>

#include "G4SDManager.hh"
#include "ExN02TrackerSD.hh"

#include "ND280FiberScintConstructor.hh"

// used to keep a list of SD logical volumes
#include "G4RegionStore.hh"
#include <G4Region.hh> 
//

class ND280FiberScintMessenger: public ND280ConstructorMessenger {

private:
    
  ND280FiberScintConstructor *fConstructor;
  
  G4UIcmdWithAString*        fShapeCMD;
  G4UIcmdWithADoubleAndUnit* fLengthCMD;
  G4UIcmdWithADoubleAndUnit* fHeightCMD;
  G4UIcmdWithADoubleAndUnit* fCoatingThicknessCMD;
  G4UIcmdWithADoubleAndUnit* fGapCMD;
  G4UIcmdWithABool*          fVisibleCMD;

public:

  ND280FiberScintMessenger(ND280FiberScintConstructor* c) 
    : ND280ConstructorMessenger(c,"Control the T2K world geometry."),
      fConstructor(c) {
    
    //fShapeCMD = new G4UIcmdWithAString(CommandName("shape"),this);
    //fShapeCMD->SetGuidance("Set the extruded cross section of the bar.");
    //fShapeCMD->SetParameterName("Shape",false);
    //fShapeCMD->SetCandidates("triangle rectangle");
    
    fLengthCMD = new G4UIcmdWithADoubleAndUnit(CommandName("length"),this);
    fLengthCMD->SetGuidance("Set the length of a bar.");
    fLengthCMD->SetParameterName("Length",false);
    fLengthCMD->SetUnitCategory("Length");
    
    fHeightCMD = new G4UIcmdWithADoubleAndUnit(CommandName("height"),this);
    fHeightCMD->SetGuidance("Set the bar height (default from base).");
    fHeightCMD->SetParameterName("Height",false);
    fHeightCMD->SetUnitCategory("Length");
    
    fCoatingThicknessCMD =
      new G4UIcmdWithADoubleAndUnit(CommandName("coating"),this);
    fCoatingThicknessCMD->
      SetGuidance("Set thickness of the coating on the bars.");
    fCoatingThicknessCMD->SetParameterName("TCoating",false);
    fCoatingThicknessCMD->SetUnitCategory("Length");
    
    fGapCMD = new G4UIcmdWithADoubleAndUnit(CommandName("gap"),this);
    fGapCMD->SetGuidance("Set the gap between the bars.");
    fGapCMD->SetParameterName("Gap",false);
    fGapCMD->SetUnitCategory("Length");
    
    fVisibleCMD = new G4UIcmdWithABool(CommandName("visible"),this);
    fVisibleCMD->SetGuidance("Should the bar be drawn in pictures.");
    fVisibleCMD->SetParameterName("Visible",false);
    
  }
  
  
  ~ND280FiberScintMessenger() {
    //delete fShapeCMD;
    delete fLengthCMD;
    delete fHeightCMD;
    delete fCoatingThicknessCMD;
    delete fGapCMD;
    delete fVisibleCMD;
  }
  
  void SetNewValue(G4UIcommand *cmd, G4String val) {
    
    if (cmd == fLengthCMD) {
      fConstructor->SetLength(fLengthCMD->GetNewDoubleValue(val));
    }
    else if (cmd == fHeightCMD) {
      fConstructor->SetHeight(fHeightCMD->GetNewDoubleValue(val));
    }
    else if (cmd == fCoatingThicknessCMD) {
      fConstructor->SetCoatingThickness(fCoatingThicknessCMD
					->GetNewDoubleValue(val));
    }
    else if (cmd == fGapCMD) {
      fConstructor->SetGap(fGapCMD->GetNewDoubleValue(val));
    }
    //else if (cmd == fShapeCMD) {
    //if (val == "triangle") {
    //	fConstructor->SetShape(
    //			       ND280FiberScintConstructor::eTriangle);
    //}
    //else if (val == "rectangle") {
    //fConstructor->SetShape(
    //			     ND280FiberScintConstructor::eRectangle);
    //}
    //}
    else {
      ND280ConstructorMessenger::SetNewValue(cmd,val);
    }
  }
};
  
ND280FiberScintConstructor::~ND280FiberScintConstructor() {;}

void ND280FiberScintConstructor::Init() {
  
  SetBase(10*CLHEP::mm);
  SetWidth(10*CLHEP::mm);
  SetHeight(10*CLHEP::mm);
  SetLength(10*CLHEP::mm);
  SetCoatingThickness(0.25*CLHEP::mm);
  SetGap(0.0*CLHEP::mm);
  
  //SetSensitiveDetector(NULL);
  SetVisibility(false);

  SetCoatingMaterial("ScintillatorCoating");
  SetFiberCoreMaterial("FiberCore");
  SetFiberCladMaterial("FiberCladding");
  
  SetMessenger(new ND280FiberScintMessenger(this));
}

G4LogicalVolume* ND280FiberScintConstructor::GetPiece(void) {

  // Get the SD defined in ExN02DetectorConstrucion  
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  ExN02TrackerSD *aTrackerSD = (ExN02TrackerSD*)SDman->FindSensitiveDetector("ExN02/Tracker"); 
  //
  
  ND280RootPersistencyManager* InputPersistencyManager
    = ND280RootPersistencyManager::GetInstance();
  fND280XMLInput = InputPersistencyManager->GetXMLInput();

  G4VSolid* cladding = NULL;
  G4VSolid* scintillator = NULL;

  // The offset of the core volume from the bar volume.
  double coreOffset = 0.0;
  
  G4VisAttributes *visAtt_Scint = new G4VisAttributes();
  visAtt_Scint->SetColor(01.0,1.0,1.0,0.); // yellow
  visAtt_Scint->SetForceSolid(true);
  G4VisAttributes *visAtt_Coat = new G4VisAttributes();
  visAtt_Coat->SetColor(0.0,1.0,0.0); // green
  visAtt_Coat->SetForceSolid(true);

  double width=0;
  double height=0;
  double length=0;
 
  double cladThickFrac = 0.02 * 2.; // 2% on both sides
  double scintWidth=0;
  double scintHeight=0;
  double scintLength=0;
 
  if(GetFiberDir()==conv::kAlongX){
    width  = GetAxisLength();
    height = GetBase();
    length = GetBase();

    scintWidth  = width; // no cladding on at the fiber end
    scintHeight = height*(1-cladThickFrac);
    scintLength = length*(1-cladThickFrac);
  }
  else if(GetFiberDir()==conv::kAlongY){
    width  = GetBase();
    height = GetAxisLength();
    length = GetBase();

    scintWidth  = width *(1-cladThickFrac);
    scintHeight = height; // no cladding at the fiber end
    scintLength = length*(1-cladThickFrac);
  }
  else if(GetFiberDir()==conv::kAlongZ){
    width  = GetBase();
    height = GetBase();
    length = GetAxisLength();
    
    scintWidth  = width *(1-cladThickFrac);
    scintHeight = height*(1-cladThickFrac);
    scintLength = length; // no cladding at the fiber end
  }

  // Set the dimension in the XYZ frame
  SetWidth(width);
  SetHeight(height);
  SetLength(length);

  cladding = new G4Box(GetName()+"/Cladding",
		       width/2,
		       height/2,
		       length/2
		       );
    
  scintillator = new G4Box(GetName()+"/Cladding/Core",
   			   scintWidth/2,
			   scintHeight/2,
			   scintLength/2
			   );
  
  //G4cout << "fiber base = " << GetBase() << G4endl;
  //G4cout << "fiber axis length = " << GetAxisLength() << G4endl;
  //G4cout << "fiber width = " << GetWidth() << G4endl;
  //G4cout << "fiber height = " << GetHeight() << G4endl;
  //G4cout << "fiber length = " << GetLength() << G4endl;

  // logical volumes

  coreOffset = 0.0;
  
  G4LogicalVolume* claddingVolume
    = new G4LogicalVolume(cladding,
			  FindMaterial(GetFiberCladMaterial()),
			  GetName()+"/Cladding");

  G4LogicalVolume *scintVolume;
  scintVolume = new G4LogicalVolume(scintillator,
				    FindMaterial(GetFiberCoreMaterial()),
				    GetName()+"/Cladding/Core");

  // Define the volume of plastic scintillator as sensitive detector
  scintVolume->SetSensitiveDetector( aTrackerSD ); 

  if( GetND280XML()->GetXMLInvisSciFi() ){
    claddingVolume->SetVisAttributes(G4VisAttributes::Invisible);
    scintVolume    ->SetVisAttributes(G4VisAttributes::Invisible);
  }
  else{
    claddingVolume->SetVisAttributes(visAtt_Coat); 
    scintVolume    ->SetVisAttributes(visAtt_Scint);
  }
  
  // Place the scintillator inside the cladding volume
  new G4PVPlacement(0,                   // no rotation
		    G4ThreeVector(0,coreOffset,0),     // position
		    scintVolume,         // its logical volume
		    GetName()+"/Cladding/Core",   // its name
		    claddingVolume,     // its mother  volume
		    false,               // no boolean operations
		    0);                  // copy number
 
  return claddingVolume;
}



//void ND280FiberScintConstructor::SetSensitiveDetector(
//  G4VSensitiveDetector* s) {
//  fSensitiveDetector = s;
//}
