#include "ExN02ApplyResponse.hh"
#include <CLHEP/Random/Randomize.h>
#include <algorithm>
#include <TRandom.h>
#include <globals.hh>
#include "ExN02TargReadOut.hh"
#include "ExN02Constants.hh"

#include "G4SystemOfUnits.hh"

using namespace std;
using namespace conv;

ExN02ApplyResponse::ExN02ApplyResponse()
{
  this->Init();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02ApplyResponse::~ExN02ApplyResponse()
{
  this->Delete();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02ApplyResponse::Init(){

  persistencyManager = ND280RootPersistencyManager::GetInstance();
  ND280XMLInput = persistencyManager->GetXMLInput();

  fHitPos = G4ThreeVector(kBadNum,kBadNum,kBadNum);
  fHitPE = G4ThreeVector(kBadNum,kBadNum,kBadNum);
  fHitTime = G4ThreeVector(kBadNum,kBadNum,kBadNum);
  fHitTrkID = kBadNum;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02ApplyResponse::Delete(){
  fHitPos = G4ThreeVector(kBadNum,kBadNum,kBadNum);
  fHitPE = G4ThreeVector(kBadNum,kBadNum,kBadNum);
  fHitTime = G4ThreeVector(kBadNum,kBadNum,kBadNum);
  fHitTrkID = kBadNum;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02ApplyResponse::CalcResponse(G4ThreeVector lightPos,int trkid,int parid,double charge,double prestepTime,double steplength, double edep,G4String touch_namedet){
    
  // Get the light position in the local frame and 
  // calculate the position of the correspding MPPC (at it's bin center) 
  
  double lightX = lightPos.x();
  double lightY = lightPos.y();
  double lightZ = lightPos.z();
  // G4cout << "lightX = " << lightX << ", lightY = " << lightY << ", lightZ = " << lightZ << G4endl;
  
  double mppcX = -9999999999;
  double mppcY = -9999999999;
  double mppcZ = -9999999999;
  
  if( ND280XMLInput->GetXMLUseSuperFGD1() ){ // SuperFGD
    persistencyManager->GetHitPosXY(lightX,lightY,mppcX,mppcY);
    //G4cout << "mppc pos: X = " << mppcX << ", Y = " << mppcY << G4endl;
    //persistencyManager->GetHitPosXZ(lightX,lightZ,mppcX,mppcZ);
    //G4cout << "mppc pos: X = " << mppcX << ", Z = " << mppcZ << G4endl;
    persistencyManager->GetHitPosYZ(lightY,lightZ,mppcY,mppcZ);
    //G4cout << "mppc pos: Y = " << mppcY << ", Z = " << mppcZ << G4endl;
  }
  else if( ND280XMLInput->GetXMLUseSciFi() ||
	   ND280XMLInput->GetXMLUseFGDlike() ) { // SciFi_XZ or FGDlike_XZ
    persistencyManager->GetHitPosXY(lightX,lightY,mppcX,mppcY);
    //G4cout << "mppc pos: Y = " << mppcY << G4endl; 
    persistencyManager->GetHitPosYZ(lightY,lightZ,mppcY,mppcZ);
    //G4cout << "mppc pos: Y = " << mppcY << G4endl;
  }
  G4ThreeVector MPPCLocalPosition = G4ThreeVector(mppcX,mppcY,mppcZ);  
  
  G4double edep_q = edep;
  
  // Calculate the # of p.e. that reach the MPPC (X,Y,Z)    
  
  double peX=0.; G4double distX=0.; double timepeX=0.;// pe along fiber X
  double peY=0.; G4double distY=0.; double timepeY=0.;// pe along fiber Y
  double peZ=0.; G4double distZ=0.; double timepeZ=0.;// pe along fiber Z
  
  if( ND280XMLInput->GetXMLUseSuperFGD1() ){ // SuperFGD
    distX = lightX - persistencyManager->GetMPPCPosX(); 
    distY = lightY - persistencyManager->GetMPPCPosY(); 
    distZ = lightZ - persistencyManager->GetMPPCPosZ(); 
  }
  else if( ND280XMLInput->GetXMLUseSciFi() ||
	   ND280XMLInput->GetXMLUseFGDlike() ) { // SciFi_XZ or FGDlike_XZ
    
    distX = lightX - persistencyManager->GetMPPCPosX(); 
    distY = lightY - persistencyManager->GetMPPCPosY(); 
    distZ = lightZ - persistencyManager->GetMPPCPosZ();     
  }
  
  //////////////////////////////////////////////
  //
  // TODO: Add more checks on the hit position
  //
  //////////////////////////////////////////////
  
  if( (distX<0 && distX!=conv::kUndefined) || 
      (distY<0 && distY!=conv::kUndefined) || 
      (distZ<0 && distZ!=conv::kUndefined) ){
    G4cout << "distX = " << distX 
	   << ", distY = " << distY 
	   << ", distZ = " << distZ 
	   << G4endl;
    G4Exception("ExN02TrackerSD::ProcessHits",
		"MyCode0002",FatalException,
		"The hit is outside the target!!!");    
  }
  
  // Set the target type
  
  ExN02TargReadOut TargetReadOut;
  if( ND280XMLInput->GetXMLUseSuperFGD1() ){ // SuperFGD
    TargetReadOut.SetTargType(conv::kSuperFGD); 
  }
  else if( ND280XMLInput->GetXMLUseFGDlike() ){ // FGDlike
    TargetReadOut.SetTargType(conv::kFGDlike);
  }
  else{ // SciFi --> need to add the new response functions
    TargetReadOut.SetTargType(conv::kSciFi);
  }
  
  // light attenuation (Birks' formula)
  double pe = TargetReadOut.ApplyScintiResponse(edep_q,steplength,charge); 
  
  if( ND280XMLInput->GetXMLUseSuperFGD1() ){ // SuperFGD
    peX = pe;
    peY = pe;
    peZ = pe;
    
    timepeX = prestepTime;
    timepeY = prestepTime;
    timepeZ = prestepTime;
    
    // light collection + attenuation
    TargetReadOut.ApplyFiberResponse(peX,timepeX,distX); // along X fiber
    TargetReadOut.ApplyFiberResponse(peY,timepeY,distY); // along Y fiber
    TargetReadOut.ApplyFiberResponse(peZ,timepeZ,distZ); // along Z fiber
    
    // MPPC efficiency
    TargetReadOut.ApplyMPPCResponse(peX);
    TargetReadOut.ApplyMPPCResponse(peY);
    TargetReadOut.ApplyMPPCResponse(peZ);
    
  }
  else if( ND280XMLInput->GetXMLUseSciFi() ||
	   ND280XMLInput->GetXMLUseFGDlike() ) { 
    // SciFi_XZ or FGDlike_XZ (i.e. built in XY and rotated by 90deg on X axis)

    // Take the detector name along X and Z
    G4String DetNameAlongX = persistencyManager->GetDetNameAlongX(); 
    G4String DetNameAlongZ = persistencyManager->GetDetNameAlongZ();
    
    // Propagation only along X
    if(touch_namedet.contains(DetNameAlongX)){ // "FiberScintHoriz"
      peX = pe; timepeX = prestepTime;
      peY = 0.; timepeY = 0.;
      peZ = 0.; timepeZ = 0.;	
      // light collection + attenuation
      TargetReadOut.ApplyFiberResponse(peX,timepeX,distX);	
      // MPPC efficiency
      TargetReadOut.ApplyMPPCResponse(peX);
    }
    // Propagation only along Z
    else if(touch_namedet.contains(DetNameAlongZ)){ //"FiberScintVert"
      peX = 0.; timepeX = 0.;
      peY = 0.; timepeY = 0.;
      peZ = pe; timepeZ = prestepTime;	
      // light collection + attenuation
      TargetReadOut.ApplyFiberResponse(peZ,timepeZ,distZ);	
      // MPPC efficiency
      TargetReadOut.ApplyMPPCResponse(peZ);
    }
    else{
      G4Exception("ExN02TrackerSD::ProcessHits",
		  "MyCode0002",FatalException,
		  "The hit is outside both direction fibers!!!");    
    }            
  } // if SuperFGD || SciFi ...
  
  G4int trackid = trkid; 
  
  fHitPos = G4ThreeVector(MPPCLocalPosition.x(),MPPCLocalPosition.y(),MPPCLocalPosition.z());
  fHitPE = G4ThreeVector(peX,peY,peZ);
  fHitTime = G4ThreeVector(timepeX,timepeY,timepeZ);
  fHitTrkID = trackid;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

















