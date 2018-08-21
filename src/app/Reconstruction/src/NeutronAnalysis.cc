#include <TFile.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TH3F.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TTree.h>
#include <TVector3.h>
#include <TFitResult.h>
#include <TFitResultPtr.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TMath.h>
#include <TStopwatch.h>
#include <TPolyMarker3D.h>
#include <TMarker.h>

#include <iostream>
#include <fstream>

#include <TRandom3.h>

#include "ND280UpConst.hh"
#include <ND280UpApplyResponse.hh>
#include "ND280UpRecoTrack.hh"
#include "ND280UpPID.hh"

#include "TND280UpEvent.hh"
#include "TND280UpVertex.hh"
#include "TND280UpRecoEvent.hh"
#include "TND280UpRecoTrack.hh"

#include "LightYield.hh"

const int VERTEX_ACTIVITY = 5;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int NeutronAnalysis(int argc,char** argv) {
  if (argc!=14){   // batch mode                                        
    cout << "You need to provide the following arguments:" << endl;
    cout << " 1) input ROOT file name (from GEANT4 simulation) " << endl;
    cout << " 2) first event number to run" << endl;
    cout << " 3) total number of events to run" << endl;
    cout << " 4) the tag for the output file name" << endl;
    cout << " 5) detector ID: 0-->SuperFGD, 1-->FGD-like, 2-->WAGASCI" << endl;
    cout << " 6) set debug option" << endl;
    cout << " 7) set debug plot option" << endl;
    cout << " 8) use view XY (0 or 1)" << endl;
    cout << " 9) use view XZ (0 or 1)" << endl;
    cout << " 10) use view YZ (0 or 1)" << endl;
    cout << " 11) Minimum track distance (if <=0 use default)" << endl;
    cout << " 12) Look only at tracks inside the Truth FV (use MC hits)" << endl;
    cout << " 13) Output file name. In iteractive run it can be first event, but for parallel need separate param" << endl;
    exit(1);
  }

  std::string prefix = "_na";

  
  string rootfilename = argv[1];
  const int evtfirst = atoi(argv[2]);
  const int nevents = atoi(argv[3]);
  string tag = argv[4];
  tag += prefix;
  const int detectorID = atoi(argv[5]);
  const int DEBUG = atoi(argv[6]);
  const int UseViewXY = atoi(argv[8]);
  const int UseViewXZ = atoi(argv[9]);
  const int UseViewYZ = atoi(argv[10]);
  const int outname = atoi (argv[13]);


  // Set the inputs

  nd280upconv::TargetType_t DetType;
  if     (detectorID == 0) DetType = nd280upconv::kSuperFGD;
  else if(detectorID == 1) DetType = nd280upconv::kFGDlike;
  
  const int NEvtDisplTot = 5; //atoi(argv[5]); // max # of evt. displays

  // Declare canvases
  TCanvas *cMPPCHits_XY[NEvtDisplTot]; 
  TCanvas *cMPPCHits_XZ[NEvtDisplTot]; 
  TCanvas *cMPPCHits_YZ[NEvtDisplTot];

  //
  // Declare histograms
  //

  // number of hits
  TH1F* hits_number = new TH1F("hitsN", "Number of hits", 50, 0., 50.);
  // hits versus energy
  TH2F* hits_energy = new TH2F("hitsE", "Hits vs Energy", 250, 0., 500., 50, 0., 50.);

  // initial energy versus cos theta
  TH2F* init_e_cos  = new TH2F("ini_ET", "Initial", 10, -1., 1., 250, 0., 500);
  TH2F* eff_e_cos   = new TH2F("eff_ET", "Efficiency", 10, -1., 1., 250, 0., 500);
  // p.e. vs theta and energy
  TH2F* pe_e_cos    = new TH2F("pe_ET", "PE from 1st hit", 10, -1., 1., 250, 0., 500);
  // distance from born position towards 1st hit
  TH2F* e_dist      = new TH2F("e_dist", "Energy vs distance", 300, 0., 1500, 250, 0., 500.);

  // Event displays

  TH2F *hMPPCHits_XY[NEvtDisplTot]; 
  TH2F *hMPPCHits_XZ[NEvtDisplTot]; 
  TH2F *hMPPCHits_YZ[NEvtDisplTot];

  TGraph *gMCHits_XY[NEvtDisplTot]; 
  TGraph *gMCHits_XZ[NEvtDisplTot]; 
  TGraph *gMCHits_YZ[NEvtDisplTot];

  TH1F *hPEVsTime_x[NEvtDisplTot];
  TH1F *hPEVsTime_y[NEvtDisplTot];
  TH1F *hPEVsTime_z[NEvtDisplTot];

   // Set outputs
  
  TString outfilename = TString::Format("%s_Evt%d_NEvt%d.root",tag.c_str(),outname,nevents);
  TFile *fileout = new TFile(outfilename.Data(),"RECREATE"); 

  /////////////////////////////
  //                         //
  // Apply detector response //
  //                         //
  /////////////////////////////

  // Take inputs
  TFile *finput = new TFile(rootfilename.c_str(),"READ");
  cout << "Reading the input ROOT file: " << rootfilename << endl;

  TH2F* h2d_xy; TH2F* h2d_xz; TH2F* h2d_yz;
  h2d_xy = (TH2F*)finput->Get("OutMPPCProj2D_XY");
  h2d_xz = (TH2F*)finput->Get("OutMPPCProj2D_XZ");
  h2d_yz = (TH2F*)finput->Get("OutMPPCProj2D_YZ");

  Int_t binX = h2d_xy->GetXaxis()->GetNbins();
  Int_t binY = h2d_xy->GetYaxis()->GetNbins();
  Int_t binZ = h2d_xz->GetYaxis()->GetNbins();

  Float_t X_min = h2d_xy->GetXaxis()->GetBinLowEdge(0);
  Float_t X_max = h2d_xy->GetXaxis()->GetBinUpEdge(binX);
  Float_t Y_min = h2d_xy->GetYaxis()->GetBinLowEdge(0);
  Float_t Y_max = h2d_xy->GetYaxis()->GetBinUpEdge(binX);
  Float_t Z_min = h2d_xz->GetYaxis()->GetBinLowEdge(0);
  Float_t Z_max = h2d_xz->GetYaxis()->GetBinUpEdge(binX);

  TH2F* first_bin_XY = (TH2F*)h2d_xy->Clone("fb_XY");
  TH2F* first_bin_XZ = (TH2F*)h2d_xz->Clone("fb_XZ");
  TH2F* first_bin_YZ = (TH2F*)h2d_yz->Clone("fb_YZ");

  TH3F* h3d = new TH3F("3d", "", binX, X_min, X_max, binY, Y_min, Y_max, binZ, Z_min, Z_max);

  // Take the World origin position in the target (local) reference system
  TPolyMarker3D *WorldOrigInLocal = (TPolyMarker3D*)finput->Get("WorldOrigInLocal");
  double OrigInLocX=0.;
  double OrigInLocY=0.;
  double OrigInLocZ=0.;
  WorldOrigInLocal->GetPoint(0,OrigInLocX,OrigInLocY,OrigInLocZ);
  
  cout << "The World origin in local coordinates is:" << endl; 
  cout << OrigInLocX << ", " << OrigInLocY << ", " << OrigInLocZ << endl;
  cout << endl;

  TGraph *mygraph = new TGraph();
  mygraph->SetName("mygraph");
  mygraph->SetTitle("mygraph");
  
  TTree *tinput = (TTree*) finput->Get("ND280upEvents");

  //
  // Take the MPPC 2D histogram
  //

  ND280UpApplyResponse ApplyResponse;    
  ApplyResponse.SetMPPCProj2D_XY(h2d_xy);
  ApplyResponse.SetMPPCProj2D_XZ(h2d_xz);
  ApplyResponse.SetMPPCProj2D_YZ(h2d_yz);
  // FGDlike
  if(DetType == nd280upconv::kFGDlike){
    ApplyResponse.SetDetNameAlongX("/BarScintHoriz");
    ApplyResponse.SetDetNameAlongY("");
    ApplyResponse.SetDetNameAlongZ("/BarScintVert");
  }
  // SciFi
  //ApplyResponse.SetDetNameAlongX("/FiberScintHoriz");
  //ApplyResponse.SetDetNameAlongY("");
  //ApplyResponse.SetDetNameAlongZ("/FiberScintVert");

  
  // Declare the input true event object
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

  TStopwatch sw_event;

  for(int ievt=evtfirst;ievt<=EntryLast;ievt++){ // get last entry
    tinput->GetEntry(ievt);

    if(!(ievt%500)){
      cout << "Event " << ievt;
      sw_event.Stop();
      double CPUtime = sw_event.CpuTime();
      sw_event.Reset();
      sw_event.Start(0);
      cout << " --> CPU time = " << CPUtime << endl;
    }

    h3d->Reset();
    
   // Initialize histograms
    
    TString name;

    if(ievt<NEvtDisplTot){

      name = TString::Format("gMCHits_XY_%d",ievt);
      gMCHits_XY[ievt] = new TGraph(); 
      gMCHits_XY[ievt]->SetName(name);
      gMCHits_XY[ievt]->SetTitle(name);
      gMCHits_XY[ievt]->SetMarkerSize(1);
      gMCHits_XY[ievt]->SetMarkerStyle(20);
      name = TString::Format("gMCHits_XZ_%d",ievt);
      gMCHits_XZ[ievt] = new TGraph(); 
      gMCHits_XZ[ievt]->SetName(name);
      gMCHits_XZ[ievt]->SetTitle(name);
      gMCHits_XZ[ievt]->SetMarkerSize(1);
      gMCHits_XZ[ievt]->SetMarkerStyle(20);
      name = TString::Format("gMCHits_YZ_%d",ievt);
      gMCHits_YZ[ievt] = new TGraph();
      gMCHits_YZ[ievt]->SetName(name);
      gMCHits_YZ[ievt]->SetTitle(name);
      gMCHits_YZ[ievt]->SetMarkerSize(1);
      gMCHits_YZ[ievt]->SetMarkerStyle(20);

      name = TString::Format("hMPPCHits_XY_%d",ievt);
      hMPPCHits_XY[ievt] = (TH2F*)h2d_xy->Clone(name);
      name = TString::Format("hMPPCHits_XZ_%d",ievt);
      hMPPCHits_XZ[ievt] = (TH2F*)h2d_xz->Clone(name);
      name = TString::Format("hMPPCHits_YZ_%d",ievt);
      hMPPCHits_YZ[ievt] = (TH2F*)h2d_yz->Clone(name);
      
      name = TString::Format("hPEVsTime_x_%d",ievt);
      hPEVsTime_x[ievt] = new TH1F(name,name,100,0,10000);
      name = TString::Format("hPEVsTime_y_%d",ievt);
      hPEVsTime_y[ievt] = new TH1F(name,name,100,0,10000);
      name = TString::Format("hPEVsTime_z_%d",ievt);
      hPEVsTime_z[ievt] = new TH1F(name,name,100,0,10000);    
      
      // Initialize canvases
      name = TString::Format("cMPPCHits_XY_%d",ievt);
      cMPPCHits_XY[ievt] = new TCanvas(name,name);
      name = TString::Format("cMPPCHits_XZ_%d",ievt);
      cMPPCHits_XZ[ievt] = new TCanvas(name,name);
      name = TString::Format("cMPPCHits_YZ_%d",ievt);
      cMPPCHits_YZ[ievt] = new TCanvas(name,name);
    }

    // take the vertex point
    if (nd280UpEvent->GetNVertices() < 1) {
      std::cout << "WARNING! no vertices in the event! The event will be skipped" << std::endl;
      continue;
    }

    Int_t vertex_binX = h2d_xz->GetXaxis()->FindBin(nd280UpEvent->GetVertex(0)->GetPosition().X());
    Int_t vertex_binY = h2d_yz->GetXaxis()->FindBin(nd280UpEvent->GetVertex(0)->GetPosition().Y());
    Int_t vertex_binZ = h2d_yz->GetYaxis()->FindBin(nd280UpEvent->GetVertex(0)->GetPosition().Z());

    Double_t vertex_x = nd280UpEvent->GetVertex(0)->GetPosition().X();
    Double_t vertex_y = nd280UpEvent->GetVertex(0)->GetPosition().Y();
    Double_t vertex_z = nd280UpEvent->GetVertex(0)->GetPosition().Z();

    if (DEBUG)
      std::cout << "Vertex placed in cubes X Y Z " << vertex_binX << "\t" << vertex_binY << "\t" << vertex_binZ << std::endl;

    // take for the neutron vars
    // TODO loop over trajs looking for neutron
    if (nd280UpEvent->GetNTracks() < 1) {
      std::cout << "WARNING! no tracks in the event! The event will be skipped" << std::endl;
      continue;
    }
    TND280UpTrack* track = nd280UpEvent->GetTrack(0);
    if (track->GetPDG() != 2112) {
      std::cout << "WARNING! First track is not neutron! The event will be skipped" << std::endl;
      continue;
    }

    Float_t ekin      = track->GetInitKinEnergy();
    Float_t costheta  = track->GetInitCosTheta();
    
    // Loop over the hits
    
    int NHits = nd280UpEvent->GetNHits();

    if(DEBUG){
      cout << endl;
      cout << "//////////////////////" << endl;
      cout << "Event: " << ievt << endl;
      cout << "# of hits = " << nd280UpEvent->GetNHits() << endl;
    }

    double first_hit_time = 1.e9;
    double dist = -2.;

    Int_t first_binX, first_binY, first_binZ;

    // DEFINE HITS MAP
    TH2F* hits_map_XY = (TH2F*)h2d_xy->Clone("hits_map_XY");
    TH2F* hits_map_XZ = (TH2F*)h2d_xz->Clone("hits_map_XZ");
    TH2F* hits_map_YZ = (TH2F*)h2d_yz->Clone("hits_map_YZ");

    // artificially fill the vertex activity region considering the worst case
    for (Int_t i = -VERTEX_ACTIVITY/2; i < VERTEX_ACTIVITY/2; ++i) {
      for (Int_t j = -VERTEX_ACTIVITY/2; j < VERTEX_ACTIVITY/2; ++j) {
        hits_map_XY->Fill(vertex_binX + i, vertex_binY + j, 100.);
        hits_map_XZ->Fill(vertex_binX + i, vertex_binZ + j, 100.);
        hits_map_YZ->Fill(vertex_binY + i, vertex_binZ + j, 100.);
      }
    }

    for(int ihit=0;ihit<NHits;ihit++){ // get last entry
      TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(ihit);

      double charge = 1.; // Apply alway Birks! 
      double time = (nd280UpHit->GetStartT() + nd280UpHit->GetStopT())/2.; // middle step time
      double posX = (nd280UpHit->GetStartX() + nd280UpHit->GetStopX())/2.; // middle step X
      double posY = (nd280UpHit->GetStartY() + nd280UpHit->GetStopY())/2.; // middle step Y 
      double posZ = (nd280UpHit->GetStartZ() + nd280UpHit->GetStopZ())/2.; // middle step Z
      TVector3 lightPos(posX,posY,posZ); // already in local position

      double edep = nd280UpHit->GetEnergyDeposit(); 

      double steplength = nd280UpHit->GetTrackLength(); // check how it's calculated in geant4

      string detname = nd280UpHit->GetDetName();

      Int_t hit_binX = h2d_xz->GetXaxis()->FindBin(posX);
      Int_t hit_binY = h2d_yz->GetXaxis()->FindBin(posY);
      Int_t hit_binZ = h2d_yz->GetYaxis()->FindBin(posZ);

      // ommit the vertex activity in VERTEX_ACTIVITY
      if (abs(hit_binX - vertex_binX) < VERTEX_ACTIVITY / 2 + 1 &&
          abs(hit_binY - vertex_binY) < VERTEX_ACTIVITY / 2 + 1 &&
          abs(hit_binZ - vertex_binZ) < VERTEX_ACTIVITY / 2 + 1)
        continue;

      //
      // Compute the detector response for each hit
      //
      
      ApplyResponse.SetTargetID(DetType);
      ApplyResponse.CalcResponse(lightPos,1,0,charge,time,steplength,edep,detname);
      
      double pex = ApplyResponse.GetHitPE().x();
      double pey = ApplyResponse.GetHitPE().y();
      double pez = ApplyResponse.GetHitPE().z();
      
      if(!UseViewXY) pez = 0;
      if(!UseViewYZ) pex = 0;
      if(!UseViewXZ) pey = 0;

      double timepex = ApplyResponse.GetHitTime().x();
      double timepey = ApplyResponse.GetHitTime().y();
      double timepez = ApplyResponse.GetHitTime().z();
      
      double poshitX = ApplyResponse.GetHitPos().x();
      double poshitY = ApplyResponse.GetHitPos().y();
      double poshitZ = ApplyResponse.GetHitPos().z();

      hits_map_XY->Fill(poshitX,poshitY,pez);  // pe along Z
      hits_map_XZ->Fill(poshitX,poshitZ,pey); // pe along Y
      hits_map_YZ->Fill(poshitY,poshitZ,pex); // pe along X

      if (pex + pey + pez > 0 && time < first_hit_time) {
        first_binX = hits_map_XY->GetXaxis()->FindBin(poshitX);
        first_binY = hits_map_XY->GetYaxis()->FindBin(poshitY);
        first_binZ = hits_map_YZ->GetYaxis()->FindBin(poshitZ);
        first_hit_time = time;
        dist = sqrt(  (posX - vertex_x) * (posX - vertex_x) + 
                      (posY - vertex_y) * (posY - vertex_y) +
                      (posZ - vertex_z) * (posZ - vertex_z));
      }

      Int_t bin = h3d->FindBin(poshitX, poshitY, poshitZ);
      if (!h3d->GetBinContent(bin)) {
        h3d->Fill(poshitX, poshitY, poshitZ);
      }

      if(ievt<NEvtDisplTot){
        gMCHits_XY[ievt]->SetPoint(gMCHits_XY[ievt]->GetN(), poshitX, poshitY);
        gMCHits_XZ[ievt]->SetPoint(gMCHits_XZ[ievt]->GetN(), poshitX, poshitZ);
        gMCHits_YZ[ievt]->SetPoint(gMCHits_YZ[ievt]->GetN(), poshitY, poshitZ);

        hMPPCHits_XY[ievt]->Add(hits_map_XY);
        hMPPCHits_XZ[ievt]->Add(hits_map_XZ);
        hMPPCHits_YZ[ievt]->Add(hits_map_YZ);

        hPEVsTime_x[ievt]->Fill(timepex,pex);
        hPEVsTime_y[ievt]->Fill(timepey,pey);
        hPEVsTime_z[ievt]->Fill(timepez,pez);
      }
    } // end loop over the hits

    hits_number->Fill(h3d->Integral());

    hits_energy->Fill(ekin, h3d->Integral());
    init_e_cos->Fill(costheta, ekin);
    e_dist->Fill(dist, ekin);

    // END OF FILLING HITS MAP

    // Draw the Canvas
    if(ievt<NEvtDisplTot){
      cMPPCHits_XY[ievt]->cd();
      hMPPCHits_XY[ievt]->Draw("colz");
      gMCHits_XY[ievt]->Draw("p same");
      //TargVtx_XY->Draw();
    
      cMPPCHits_YZ[ievt]->cd();
      hMPPCHits_YZ[ievt]->Draw("colz");
      gMCHits_YZ[ievt]->Draw("p same");
      //TargVtx_YZ->Draw();
    
      cMPPCHits_XZ[ievt]->cd();
      hMPPCHits_XZ[ievt]->Draw("colz");
      gMCHits_XZ[ievt]->Draw("p same");
      //TargVtx_XZ->Draw();
    }

    // The analysis

    // at least one hit should exists
    if (first_hit_time == 1.e9)
      continue;

    // map the first hit
    Double_t x = hits_map_XY->GetXaxis()->GetBinCenter(first_binX);
    Double_t y = hits_map_XY->GetYaxis()->GetBinCenter(first_binY);
    Double_t z = hits_map_XZ->GetYaxis()->GetBinCenter(first_binZ);

    first_bin_XY->Fill(x, y);
    first_bin_YZ->Fill(y, z);
    first_bin_XZ->Fill(x, z);

    // check that we have exactly one hit
    // hit cube should be isolated in at least 2 projections
    Int_t PE_around1 =  hits_map_XY->GetBinContent(first_binX + 1,  first_binY) + 
                        hits_map_XY->GetBinContent(first_binX + 1,  first_binY + 1) +
                        hits_map_XY->GetBinContent(first_binX,      first_binY + 1) +
                        hits_map_XY->GetBinContent(first_binX - 1,  first_binY + 1) +
                        hits_map_XY->GetBinContent(first_binX - 1,  first_binY) +
                        hits_map_XY->GetBinContent(first_binX - 1,  first_binY - 1) +
                        hits_map_XY->GetBinContent(first_binX,      first_binY - 1) +
                        hits_map_XY->GetBinContent(first_binX + 1,  first_binY - 1);

    Int_t PE_around2 =  hits_map_XZ->GetBinContent(first_binX + 1,  first_binZ) + 
                        hits_map_XZ->GetBinContent(first_binX + 1,  first_binZ + 1) +
                        hits_map_XZ->GetBinContent(first_binX,      first_binZ + 1) +
                        hits_map_XZ->GetBinContent(first_binX - 1,  first_binZ + 1) +
                        hits_map_XZ->GetBinContent(first_binX - 1,  first_binZ) +
                        hits_map_XZ->GetBinContent(first_binX - 1,  first_binZ - 1) +
                        hits_map_XZ->GetBinContent(first_binX,      first_binZ - 1) +
                        hits_map_XZ->GetBinContent(first_binX + 1,  first_binZ - 1);

    Int_t PE_around3 =  hits_map_YZ->GetBinContent(first_binY + 1,  first_binZ) + 
                        hits_map_YZ->GetBinContent(first_binY + 1,  first_binZ + 1) +
                        hits_map_YZ->GetBinContent(first_binY,      first_binZ + 1) +
                        hits_map_YZ->GetBinContent(first_binY - 1,  first_binZ + 1) +
                        hits_map_YZ->GetBinContent(first_binY - 1,  first_binZ) +
                        hits_map_YZ->GetBinContent(first_binY - 1,  first_binZ - 1) +
                        hits_map_YZ->GetBinContent(first_binY,      first_binZ - 1) +
                        hits_map_YZ->GetBinContent(first_binY + 1,  first_binZ - 1);

    // isolated in at least 2 views
    if ((PE_around1 > 0) + (PE_around2 > 0) + (PE_around3 > 0) > 1)
      continue;

    // SELECTION IS DONE 
    // filling histoes

    eff_e_cos->Fill(costheta, ekin);
    pe_e_cos->Fill( costheta, ekin,
                    hits_map_XY->GetBinContent(first_binX, first_binY) +
                    hits_map_XZ->GetBinContent(first_binX, first_binZ) +
                    hits_map_YZ->GetBinContent(first_binY, first_binZ));
  } // loop over events

  fileout->cd();

  hits_number->Write();
  hits_energy->Write();
  e_dist->Write();

  first_bin_XY->Write();
  first_bin_YZ->Write();
  first_bin_XZ->Write();

  pe_e_cos->Write();

  init_e_cos->Write();
  eff_e_cos->Write();

  int last = evtfirst+Nentries-1;
  for(int ievtdispl=evtfirst;ievtdispl<=last ;ievtdispl++){ 
    // Response
    if(ievtdispl<NEvtDisplTot){
      //cout << "ievtdispl = " << ievtdispl << endl;
      if(hMPPCHits_XY[ievtdispl]->Integral()>0 ||
         hMPPCHits_XZ[ievtdispl]->Integral()>0 ||
         hMPPCHits_YZ[ievtdispl]->Integral()>0 ){
  
        hMPPCHits_XY[ievtdispl]->Write();  
        hMPPCHits_XZ[ievtdispl]->Write();  
        hMPPCHits_YZ[ievtdispl]->Write();
      
        gMCHits_XY[ievtdispl]->Write();
        gMCHits_XZ[ievtdispl]->Write();
        gMCHits_YZ[ievtdispl]->Write();
      
        cMPPCHits_XY[ievtdispl]->Write();  
        cMPPCHits_XZ[ievtdispl]->Write();  
        cMPPCHits_YZ[ievtdispl]->Write();
      }
    }
  }

  fileout->Close();
  
  return 0;
}