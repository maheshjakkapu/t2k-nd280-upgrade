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
// $Id: ExN02DetectorMessenger.hh,v 1.9 2008-09-22 16:41:20 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef ExN02DetectorMessenger_h
#define ExN02DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class ExN02DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ExN02DetectorMessenger: public G4UImessenger
{
  public:
    ExN02DetectorMessenger(ExN02DetectorConstruction*);
   ~ExN02DetectorMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    ExN02DetectorConstruction* myDetector;
    
  G4UIdirectory*             N02Dir;
  G4UIdirectory*             detDir;
  G4UIcmdWithAString*        Targ1MatCmd;
  G4UIcmdWithAString*        Targ2MatCmd;
  G4UIcmdWithAString*        FGD1MatCmd;
  G4UIcmdWithAString*        FGD2MatCmd;
  G4UIcmdWithAString*        SideTPCMatCmd;    
  //G4UIcmdWithADoubleAndUnit* FieldCmd;
  G4UIcmdWithADoubleAndUnit* StepMaxCmd;    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

