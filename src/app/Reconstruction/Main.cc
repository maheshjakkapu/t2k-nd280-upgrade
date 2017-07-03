
#include <TFile.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TTree.h>
#include <TVector3.h>

#include <iostream>
#include <fstream>

#include "ND280UpConst.hh"
#include "ND280UpApplyResponse.hh"
//#include "ND280UpReco.hh"

#include "TND280UpEvent.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{   

  if (argc!=6){   // batch mode                                        
    cout << "You need to provide the following arguments:" << endl;
    cout << " 1) input ROOT file name (from GEANT4 simulation) " << endl;
    cout << " 2) first event number to run" << endl;
    cout << " 3) total number of events to run" << endl;
    cout << " 4) the tag for the output file name" << endl;
    cout << " 5) maximum number of event displays" << endl;
    exit(1);
  }
 
  string rootfilename = argv[1];
  const int evtfirst = atoi(argv[2]);
  const int nevents = atoi(argv[3]);
  string tag = argv[4];
  const int NEvtDisplTot = atoi(argv[5]); // max # of evt. displays 

  // Declare histograms

  TH2F *hMPPCHits_XY[NEvtDisplTot]; 
  TH2F *hMPPCHits_XZ[NEvtDisplTot]; 
  TH2F *hMPPCHits_YZ[NEvtDisplTot];
  TH2F *hMPPCHitsDelay100ns_XY[NEvtDisplTot]; 
  TH2F *hMPPCHitsDelay100ns_XZ[NEvtDisplTot]; 
  TH2F *hMPPCHitsDelay100ns_YZ[NEvtDisplTot];
  TH1F *hPEVsTime_x[NEvtDisplTot]; 
  TH1F* hPEVsTime_y[NEvtDisplTot]; 
  TH1F* hPEVsTime_z[NEvtDisplTot]; 
  
  // Take inputs

  TFile *finput = new TFile(rootfilename.c_str(),"READ");
  cout << "Reading the input ROOT file: " << rootfilename << endl;

  TH2F* h2d_xy; TH2F* h2d_xz; TH2F* h2d_yz;
  h2d_xy = (TH2F*)finput->Get("OutMPPCProj2D_XY");
  h2d_xz = (TH2F*)finput->Get("OutMPPCProj2D_XZ");
  h2d_yz = (TH2F*)finput->Get("OutMPPCProj2D_YZ");
    
  TTree *tinput = (TTree*) finput->Get("ND280upEvents");

  //
  // Take the MPPC 2D histogram
  //

  ND280UpApplyResponse ApplyResponse;    
  ApplyResponse.SetMPPCProj2D_XY(h2d_xy);
  ApplyResponse.SetMPPCProj2D_XZ(h2d_xz);
  ApplyResponse.SetMPPCProj2D_YZ(h2d_yz);

  // FGDlike
  ApplyResponse.SetDetNameAlongX("/BarScintHoriz");
  ApplyResponse.SetDetNameAlongY("");
  ApplyResponse.SetDetNameAlongZ("/BarScintVert");
  // SciFi
  //ApplyResponse.SetDetNameAlongX("/FiberScintHoriz");
  //ApplyResponse.SetDetNameAlongY("");
  //ApplyResponse.SetDetNameAlongZ("/FiberScintVert");

  TND280UpEvent *nd280UpEvent = new TND280UpEvent();  
  tinput->SetBranchAddress("Event",&nd280UpEvent);

  int NTreeEntries = tinput->GetEntries();  
  int evtlasttree = NTreeEntries-1;
  int Nentries = -999;

  cout << endl;
  cout << "# of entries in the Tree: " << NTreeEntries << endl;
  cout << "last tree event ID: " << evtlasttree << endl;
  cout << "first event ID: " << evtfirst << endl;
  cout << endl;
  
  if(evtfirst > evtlasttree){
    cout << endl;
    cout << "first selection evt ID > first evt ID of tree:" << endl;
    cout << " - # of tree events = " << NTreeEntries << endl;
    cout << " - last tree event = " << evtlasttree << endl;
    cout << " - first event = " << evtfirst << endl;
    cout << "Exit!!!" << endl;
    cout << endl;
    exit(1);
  }
  else if(NTreeEntries < (evtfirst+nevents)){
    
    Nentries = NTreeEntries - evtfirst;
    
    cout << endl;
    cout << "WARNING:" << endl;
    cout << "first evt ID + # of events > tot # of tree events:" << endl;
    cout << " - # of tree events = " << NTreeEntries << endl;
    cout << " - # of events to run = " << nevents << endl;
    cout << " - first event = " << evtfirst << endl;
    cout << "Set # of events to run to " << Nentries;
    cout << endl;
  }
  else{
    Nentries = nevents;    
  }
  
  int EntryLast = evtfirst+Nentries-1;
  
  cout << endl;
  cout << "First event = " << evtfirst << endl;
  cout << "Last event = " << EntryLast << endl;
  cout << "# of events = " << Nentries << endl;
  cout << endl;

  //
  // Loop over the events:
  // - apply response event by event
  // - apply reconstruction event by event
  //

  for(int ievt=evtfirst;ievt<=EntryLast;ievt++){ // get last entry
    tinput->GetEntry(ievt);
    
    // Initialize histograms
    
    TString name;
    name = TString::Format("hMPPCHits_XY_%d",ievt);
    hMPPCHits_XY[ievt] = (TH2F*)h2d_xy->Clone(name);
    name = TString::Format("hMPPCHits_XZ_%d",ievt);
    hMPPCHits_XZ[ievt] = (TH2F*)h2d_xz->Clone(name);
    name = TString::Format("hMPPCHits_YZ_%d",ievt);
    hMPPCHits_YZ[ievt] = (TH2F*)h2d_yz->Clone(name);

    name = TString::Format("hMPPCHitsDelay100ns_XY_%d",ievt);
    hMPPCHitsDelay100ns_XY[ievt] = (TH2F*)h2d_xy->Clone(name);
    name = TString::Format("hMPPCHitsDelay100ns_XZ_%d",ievt);
    hMPPCHitsDelay100ns_XZ[ievt] = (TH2F*)h2d_xz->Clone(name);
    name = TString::Format("hMPPCHitsDelay100ns_YZ_%d",ievt);
    hMPPCHitsDelay100ns_YZ[ievt] = (TH2F*)h2d_yz->Clone(name);

    name = TString::Format("hPEVsTime_x_%d",ievt);
    hPEVsTime_x[ievt] = new TH1F(name,name,100,0,10000);
    name = TString::Format("hPEVsTime_y_%d",ievt);
    hPEVsTime_y[ievt] = new TH1F(name,name,100,0,10000);
    name = TString::Format("hPEVsTime_z_%d",ievt);
    hPEVsTime_z[ievt] = new TH1F(name,name,100,0,10000);    


    /*
    // Loop over the tracks    
    int NTracks = nd280UpEvent->GetNTracks();    
    for(int itrk=0;itrk<NTracks;itrk++){
      TND280UpTrack *nd280UpTrack = nd280UpEvent->GetTrack(itrk);
      int trkID = nd280UpTrack->GetTrackID();
      int parentID = nd280UpTrack->GetParentID();
      int pdg = nd280UpTrack->GetPDG();
      double charge = nd280UpTrack->GetCharge();
      double ekin = nd280UpTrack->GetInitKinEnergy();
      double mom = nd280UpTrack->GetInitMom().Mag(); 
      TND280UpTrackPoint *nd280UpTrackPoint = nd280UpTrack->GetPoint(0); 
      string volname = nd280UpTrackPoint->GetLogVolName();
      }
    */


    // Loop over the hits
    
    int NHits = nd280UpEvent->GetNHits();

    cout << endl;
    cout << "Event: " << ievt << endl;
    cout << "# of hits = " << nd280UpEvent->GetNHits() << endl;

    int trkID_prev = 0.;

    for(int ihit=0;ihit<NHits;ihit++){ // get last entry
      TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(ihit);
      //nd280UpHit->PrintHit();
      
      int parentid = nd280UpHit->GetPrimaryId(); 
      //cout << parentid << endl;

      //int detid = (int)nd280upconv::kBadNum; 
      int pdg = nd280UpHit->GetPDG(); // only if MC hits of primary tracks are stored in TrackerSD
      //int pdg = (int)nd280upconv::kBadNum; 
      int trkid = nd280UpHit->fContributors.front(); 
      double edep = nd280UpHit->GetEnergyDeposit(); 
      double charge = 1.; // Apply alway Birks! 
      double time = (nd280UpHit->GetStartT() + nd280UpHit->GetStopT())/2.; // middle step time
      double posX = (nd280UpHit->GetStartX() + nd280UpHit->GetStopX())/2.; // middle step X
      double posY = (nd280UpHit->GetStartY() + nd280UpHit->GetStopY())/2.; // middle step Y 
      double posZ = (nd280UpHit->GetStartZ() + nd280UpHit->GetStopZ())/2.; // middle step Z
      TVector3 lightPos(posX,posY,posZ); // already in local position
      
      string detname = nd280UpHit->GetDetName();
      double steplength = nd280UpHit->GetTrackLength();

      ApplyResponse.SetTargetID(nd280upconv::kSuperFGD);
      //ApplyResponse.SetTargetID(nd280upconv::kFGDlike);
      ApplyResponse.CalcResponse(lightPos,trkid,parentid,charge,time,steplength,edep,detname);

      double pex = ApplyResponse.GetHitPE().x();
      double pey = ApplyResponse.GetHitPE().y();
      double pez = ApplyResponse.GetHitPE().z();
      
      double timepex = ApplyResponse.GetHitTime().x();
      double timepey = ApplyResponse.GetHitTime().y();
      double timepez = ApplyResponse.GetHitTime().z();
      
      double poshitX = ApplyResponse.GetHitPos().x();
      double poshitY = ApplyResponse.GetHitPos().y();
      double poshitZ = ApplyResponse.GetHitPos().z();
      
      trkid = ApplyResponse.GetHitTrkID();
            
      hMPPCHits_XY[ievt]->Fill(poshitX,poshitY,pez); // pe along Z
      hMPPCHits_XZ[ievt]->Fill(poshitX,poshitZ,pey); // pe along Y
      hMPPCHits_YZ[ievt]->Fill(poshitY,poshitZ,pex); // pe along X

      hPEVsTime_x[ievt]->Fill(timepex,pex);
      hPEVsTime_y[ievt]->Fill(timepey,pey);
      hPEVsTime_z[ievt]->Fill(timepez,pez);
      
      if(trkid!=trkID_prev){
	cout << "trkid: " << trkid << ", "
	     << "pdg: " << nd280UpHit->GetPDG() << " | "; 
      }
      //cout << nd280UpHit->GetPEX() << ", " << pex << endl;
      //cout << nd280UpHit->GetPEY() << ", " << pey << endl;
      //cout << nd280UpHit->GetPEZ() << ", " << pez << endl;
      //cout << endl;

      trkID_prev = trkid;
      
      delete nd280UpHit;
      nd280UpHit = 0;
    } // end loop over the hits

    cout << endl;

    //delete nd280UpEvent;    
    //nd280UpEvent = 0;
  } // end loop over the events

  
  // Write output file
  TString outfilename = TString::Format("%s_Evt%d_NEvt%d.root",tag.c_str(),evtfirst,nevents);
  TFile *out = new TFile(outfilename.Data(),"RECREATE");
  //
  for(int ievtdispl=evtfirst;ievtdispl<=EntryLast;ievtdispl++){ // get last entry
    if(hMPPCHits_XY[ievtdispl]->Integral()>0 ||
       hMPPCHits_XZ[ievtdispl]->Integral()>0 ||
       hMPPCHits_YZ[ievtdispl]->Integral()>0 ){
      
      hMPPCHits_XY[ievtdispl]->Write();  
      hMPPCHits_XZ[ievtdispl]->Write();  
      hMPPCHits_YZ[ievtdispl]->Write();
      
      hMPPCHitsDelay100ns_XY[ievtdispl]->Write();  
      hMPPCHitsDelay100ns_XZ[ievtdispl]->Write();  
      hMPPCHitsDelay100ns_YZ[ievtdispl]->Write();
      
      hPEVsTime_x[ievtdispl]->Write(); 
      hPEVsTime_y[ievtdispl]->Write(); 
      hPEVsTime_z[ievtdispl]->Write(); 
    }
  }
  //
  out->Close();
  
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

