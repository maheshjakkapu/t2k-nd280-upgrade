#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include <G4PVReplica.hh>

#include <G4Tubs.hh>

#include "ND280SciFiConstructor.hh"
#include "ND280FiberScintConstructor.hh"

//
// Davide Sgalaberna 28/3/17
//
// Constructor of the SciFi detector
//

using namespace conv;

class ND280SciFiMessenger: public ND280ConstructorMessenger {
private:
  ND280SciFiConstructor* fConstructor;
  G4UIcmdWithADoubleAndUnit* fWidthCMD;
  G4UIcmdWithADoubleAndUnit* fHeightCMD;
  G4UIcmdWithADoubleAndUnit* fLengthCMD;
  
public:
    ND280SciFiMessenger(ND280SciFiConstructor* c) 
        : ND280ConstructorMessenger(c,"Control the off axis geometry."),
          fConstructor(c) {
        fWidthCMD = new G4UIcmdWithADoubleAndUnit(CommandName("width"),this);
        fWidthCMD->SetGuidance("Set the width of the off axis detector.");
        fWidthCMD->SetParameterName("Width",false);
        fWidthCMD->SetUnitCategory("Length");

        fHeightCMD = new G4UIcmdWithADoubleAndUnit(CommandName("height"),this);
        fHeightCMD->SetGuidance("Set the height of the off axis detector.");
        fHeightCMD->SetParameterName("Height",false);
        fHeightCMD->SetUnitCategory("Length");

        fLengthCMD
            = new G4UIcmdWithADoubleAndUnit(CommandName("length"),this);
        fLengthCMD->SetGuidance("Set the length of the off axis detector.");
        fLengthCMD->SetParameterName("Length",false);
        fLengthCMD->SetUnitCategory("Length");
    }

    virtual ~ND280SciFiMessenger() {
        delete fWidthCMD;
        delete fHeightCMD;
        delete fLengthCMD;
    }

    void SetNewValue(G4UIcommand *cmd, G4String val) {
      if (cmd==fWidthCMD) {
	fConstructor->SetWidth(fWidthCMD->GetNewDoubleValue(val));
      }
      else if (cmd==fHeightCMD) {
	fConstructor->SetHeight(fHeightCMD->GetNewDoubleValue(val));
      }
      else if (cmd==fLengthCMD) {
	fConstructor->SetLength(fLengthCMD->GetNewDoubleValue(val));
      }
      else {
	ND280ConstructorMessenger::SetNewValue(cmd,val);
      }
    }
};

ND280SciFiConstructor::~ND280SciFiConstructor() {;}

void ND280SciFiConstructor::Init(void) {
  
  /// The edge of a fiber of the SciFi detector 
  fEdge = 2.*CLHEP::mm;

  // Number of fibers along each axis 
  fFiberHorizNum = 1;
  fFiberVertNum = 1;

  SetWidth( 10*CLHEP::cm);
  SetHeight(10*CLHEP::cm);  
  SetLength(1000*CLHEP::mm);

  // Position of the center of the SciFi detector
  fPosX = 0.;
  fPosY = 0.;
  fPosZ = 0.;

  //
  // Note:
  // GetName() is the SciFi name given in DetectorConstruction  
  // i.e. /t2k/OA/.../SciFi
  //

  G4String nameRepLayer = "Layer"; // replica of layers along Z
  G4String nameRepFiberVert = nameRepLayer +"/RepY"; // rep fiber vert along Y
  G4String nameRepFiberHoriz = nameRepLayer +"/RepX"; // rep fiber horiz along X
  G4String nameFiberHoriz = nameRepFiberHoriz+"/SciFiHoriz";
  G4String nameFiberVert = nameRepFiberVert+"/SciFiVert";
  
  SetNameRepLayer(nameRepLayer);
  SetNameRepFiberHoriz(nameRepFiberHoriz);
  SetNameRepFiberVert(nameRepFiberVert);
  SetNameFiberHoriz(nameFiberHoriz);
  SetNameFiberVert(nameFiberHoriz);

  AddConstructor(new ND280FiberScintConstructor(GetNameFiberHoriz()+"/FiberScintHoriz", this)); 
  AddConstructor(new ND280FiberScintConstructor(GetNameFiberVert()+"/FiberScintVert", this)); 

  SetMessenger(new ND280SciFiMessenger(this));
}

G4LogicalVolume *ND280SciFiConstructor::GetPiece(void) {

  ND280RootPersistencyManager* InputPersistencyManager
    = ND280RootPersistencyManager::GetInstance();
  fND280XMLInput = InputPersistencyManager->GetXMLInput();

  // Set total size of the Super-FGD

  double TotWidth  = fFiberVertNum * fEdge; 
  SetFiberHorizLength(TotWidth);
  SetWidth(TotWidth);
  
  double TotHeight = fFiberHorizNum * fEdge;   
  SetFiberVertLength(TotHeight);
  SetHeight(TotHeight);

  // Build the plastic scintillator fiber Vertical

  //G4cout << "Vertical Fiber" << G4endl;

  ND280FiberScintConstructor& fiberVert
    = Get<ND280FiberScintConstructor>(GetNameFiberVert()+"/FiberScintVert");
  fiberVert.SetFiberDir(conv::kAlongY);
  fiberVert.SetBase( fEdge); // 
  fiberVert.SetAxisLength(TotHeight); // lengthVert based on # of fibers along X 
  fiberVert.SetCoatingThickness(0.0*CLHEP::mm);
  fiberVert.SetGap(0.0*CLHEP::mm);
  G4LogicalVolume* fiberVert_logical = fiberVert.GetPiece();


  // Build the plastic scintillator fiber Horizontal

  //G4cout << "Horizontal Fiber" << G4endl;

  ND280FiberScintConstructor& fiberHoriz
    = Get<ND280FiberScintConstructor>(GetNameFiberHoriz()+"/FiberScintHoriz");
  fiberHoriz.SetFiberDir(conv::kAlongX);
  fiberHoriz.SetBase( fEdge); // 
  fiberHoriz.SetAxisLength(TotWidth); // lengthVert based on # of fibers along Y 
  fiberHoriz.SetCoatingThickness(0.0*CLHEP::mm);
  fiberHoriz.SetGap(0.0*CLHEP::mm);
  G4LogicalVolume* fiberHoriz_logical = fiberHoriz.GetPiece();

  //
  
  double LayerLength = fiberHoriz.GetLength() + fiberVert.GetLength();
  double NLayers = GetLayerNum();
  double TotLength = NLayers * LayerLength;
  SetLength(TotLength); // along Z








  // Build box N XY layers

  G4VSolid *repLayer_solid 
    = new G4Box(GetName()+"/"+GetNameRepLayer(), 
		GetWidth()/2, 
		GetHeight()/2, 
		GetLength()/2); 
  G4LogicalVolume *repLayer_logical 
    = new G4LogicalVolume(repLayer_solid,
			  FindMaterial("Air"),
			  GetName()+"/"+GetNameRepLayer());
  repLayer_logical->SetVisAttributes(G4VisAttributes::Invisible);  
  //repLayer_logical->SetVisAttributes(G4Colour(0.0,1.0,1.0)); //green
    
  




  

  // Plane of vert fibers --> replica vert fibers along X
  
  G4VSolid *repVert_solid 
    = new G4Box(GetName()+"/"+GetNameRepFiberVert(), 
		GetWidth()/2, 
		GetHeight()/2, 
		fiberVert.GetBase()/2); 
  G4LogicalVolume *repVert_logical 
    = new G4LogicalVolume(repVert_solid,
			  FindMaterial("Air"),
			  GetName()+"/"+GetNameRepFiberVert());
  repVert_logical->SetVisAttributes(G4VisAttributes::Invisible);  
  
  new G4PVReplica(GetName()+"/"+GetNameRepFiberVert(),        // its name
		  fiberVert_logical,    // its logical volume
		  repVert_logical, // its mother volume
		  kXAxis,          // axis along replication applied
		  fFiberVertNum,       // number of replicated volumes
		  fiberVert.GetBase() // width of single replica along axis 
		  );

  // Plane of horiz fibers --> replica horiz fibers along Y
  
  G4VSolid *repHoriz_solid 
    = new G4Box(GetName()+"/"+GetNameRepFiberHoriz(), 
		GetWidth()/2, 
		//fFiberHorizNum*fiberHoriz.GetBase()/2.,
		GetHeight()/2, 
		fiberHoriz.GetBase()/2); 
  G4LogicalVolume *repHoriz_logical 
    = new G4LogicalVolume(repHoriz_solid,
			  FindMaterial("Air"),
			  GetName()+"/"+GetNameRepFiberHoriz());
  repHoriz_logical->SetVisAttributes(G4VisAttributes::Invisible);  
  
  new G4PVReplica(GetName()+"/"+GetNameRepFiberHoriz(),        // its name
		  fiberHoriz_logical,    // its logical volume
		  repHoriz_logical, // its mother volume
		  kYAxis,          // axis along replication applied
		  fFiberHorizNum,       // number of replicated volumes
		  fiberHoriz.GetBase() // width of single replica along axis 
		  );  

  // make a layer volume (X + Y plane) --> vert+horiz
  
  G4VSolid *layer_solid
    = new G4Box(GetName()+"/"+GetNameRepLayer(),
		GetWidth()/2,
		GetHeight()/2,
		LayerLength/2 
		);
  
  G4LogicalVolume *layer_logical 
    = new G4LogicalVolume(layer_solid,
			  FindMaterial("Air"),
			  GetName()+"/"+GetNameRepLayer());
  layer_logical->SetVisAttributes(G4VisAttributes::Invisible);
  //layer_logical->SetVisAttributes(G4Colour(1.0,1.0,0.0)); //yellow

  
  
  // place the horiz and vert planes inside the layer

  double posZ_horiz = -fiberHoriz.GetBase()/2.;
  double posZ_vert  = +fiberVert.GetBase()/2.;

  new G4PVPlacement(0,                   // no rotation
		    G4ThreeVector(0,0,posZ_vert),     // position
		    repVert_logical,         // its logical volume
		    GetName()+"/"+GetNameRepFiberVert(),   // its name
		    layer_logical,     // its mother  volume
		    false,               // no boolean operations
		    0);                  // copy number
 
  new G4PVPlacement(0,                   // no rotation
		    G4ThreeVector(0,0,posZ_horiz),     // position
		    repHoriz_logical,         // its logical volume
		    GetName()+"/"+GetNameRepFiberHoriz(),   // its name
		    layer_logical,     // its mother  volume
		    false,               // no boolean operations
		    0);                  // copy number
  

  //G4cout << "GetLength() = " << GetLength() << G4endl;
  //G4cout << "LayerLength = " << LayerLength << G4endl;
  //G4cout << "NLayers = " << NLayers << G4endl;

  
  // make a target volume (all layers) --> replica of layers
  
  new G4PVReplica(GetName()+"/"+GetNameRepLayer(),        // its name
		  layer_logical,    // its logical volume
		  repLayer_logical, // its mother volume
		  kZAxis,          // axis along replication applied
		  NLayers,       // number of replicated volumes
		  LayerLength // width of single replica along axis 
		  );
 
  
  return repLayer_logical;










  /*

  //
  // Make the logical volumes for the replicas
  //
 
  // Build row of fibers along Z (replica of single fiber along Z)

  G4VSolid *repZ_solid 
    = new G4Box(GetName()+"/"+GetNameRepZ(), 
		fiber.GetBase()/2, 
		fiber.GetHeight()/2, 
		GetLength()/2); 
  G4LogicalVolume *repZ_logical 
    = new G4LogicalVolume(repZ_solid,
			  FindMaterial("Air"),
			  GetName()+"/"+GetNameRepZ());
  repZ_logical->SetVisAttributes(G4VisAttributes::Invisible);  
     
  // Build layer of fibers XZ (replica of single row of fibers along X)  

  G4VSolid *repXZ_solid 
    = new G4Box(GetName()+"/"+GetNameRepXZ(), 
		GetWidth()/2, 
		fiber.GetHeight()/2, 
		GetLength()/2);   
  G4LogicalVolume *repXZ_logical 
    = new G4LogicalVolume(repXZ_solid,
			  FindMaterial("Air"),
			  GetName()+"/"+GetNameRepXZ());
  repXZ_logical->SetVisAttributes(G4VisAttributes::Invisible);  

  // Build box XYZ of fibers

  G4VSolid *repXYZ_solid 
    = new G4Box(GetName()+"/"+GetNameRepXYZ(), 
		GetWidth()/2, 
		GetHeight()/2, 
		GetLength()/2); 
  G4LogicalVolume *repXYZ_logical 
    = new G4LogicalVolume(repXYZ_solid,
			  FindMaterial("Air"),
			  GetName()+"/"+GetNameRepXYZ());
  repXYZ_logical->SetVisAttributes(G4VisAttributes::Invisible);  

  //
  // Place the fibers:
  // 1) replica of fibers along Z --> single row
  // 2) replica of rows along X --> single layer
  // 3) replica of layers along Y --> SciFi
  //
  
  new G4PVReplica(GetName()+"/"+GetNameRepZ(),        // its name
		  fiber_logical,    // its logical volume
		  repZ_logical, // its mother volume
		  kZAxis,          // axis along replication applied
		  fFiberNumZ,       // number of replicated volumes
		  fiber.GetLength() // width of single replica along axis 
		  );

  new G4PVReplica(GetName()+"/"+GetNameRepXZ(),        // its name
		  repZ_logical,  // its logical volume
		  repXZ_logical, // its mother volume
		  kXAxis,           // axis along replication applied
		  fFiberNumX,        // number of replicated volumes
		  fiber.GetBase()    // width of single replica along axis 
		  );

  new G4PVReplica(GetName()+"/"+GetNameRepXYZ(),        // its name
		  repXZ_logical,  // its logical volume
		  repXYZ_logical, // its mother volume
		  kYAxis,            // axis along replication applied
		  fFiberNumY,         // number of replicated volumes
		  fiber.GetHeight()   // width of single replica along axis 
		  );

  return repXYZ_logical;
  */

}
