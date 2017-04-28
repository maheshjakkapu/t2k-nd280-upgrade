
// 
// Class to define an ND280 upgrade hits (not GEANT4 based)
// 
// 26/4/17 Davide Sgalaberna
//

#ifndef TND280UpHit_h
#define TND280UpHit_h 1

#include <TTree.h>
#include <TFile.h>
#include <TBits.h>
#include <TObjString.h>
#include <TVector3.h>
#include <TObject.h>
#include <TList.h>
#include <TCollection.h>
#include <TIterator.h>

#include <iostream>
#include <vector>

using namespace std;

const double kUndefHit = -9999999;

class TND280UpHit : public TObject
{
public :
     
  TND280UpHit();
  ~TND280UpHit();
  
  void SetHitID(int id){fHitID=id;};
  void SetProcessName(string name){fProcessName=name;};
  void SaveIt(bool saveit){fSaveIt=saveit;};
  void SetDetID(int detid){fDetID=detid;};
  void SetPDG(int pdg){fPDG=pdg;};
  void SetTrackID(int trkid){fTrackID=trkid;};
  void SetParentID(int id){fParentID=id;};
  void SetEdep(double edep){fEdep=edep;};
  void SetEdep_Q(double edep_q){fEdep_Q=edep_q;};
  void SetLocPosX(double x){fLocPosX=x;};
  void SetLocPosY(double y){fLocPosY=y;};
  void SetLocPosZ(double z){fLocPosZ=z;};
  void SetTime(double time){fTime=time;};
  void SetDetName(string name){fDetName=name;};
  
  int GetHitID() {return fHitID;};
  string GetProcessName(){return fProcessName;};
  double GetSaveIt(){return fSaveIt;};
  int GetDetID(){return fDetID;};
  int GetPDG(){return fPDG;};
  int GetTrackID(){return fTrackID;};
  int GetParentID() {return fParentID;};
  double GetEdep(){return fEdep;};
  double GetEdep_Q(){return fEdep_Q;};
  double GetLocPosX(){return fLocPosX;};
  double GetLocPosY(){return fLocPosY;};
  double GetLocPosZ(){return fLocPosZ;};
  double GetTime(){return fTime;};
  string GetDetName(){return fDetName;};
 
  void PrintHit();

private :

  int fHitID;
  string fProcessName;
  bool fSaveIt;
  int fDetID;
  int fPDG;
  int fTrackID;
  int fParentID;
  double fEdep;
  double fEdep_Q;
  double fLocPosX;
  double fLocPosY;
  double fLocPosZ;
  double fTime;
  string fDetName;

  ClassDef(TND280UpHit,5)
};

#endif
