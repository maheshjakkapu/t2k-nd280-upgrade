#include "AnaTreeConverterEvent.hxx"
#include "InputManager.hxx"
#include "BasicUtils.hxx"
#include "Units.hxx"
#include "ND280UPAnalysisUtils.hxx"
#include "PIDUtils.hxx"
//********************************************************************
AnaTreeConverterEvent::AnaTreeConverterEvent():InputConverter("ND280upEvents"){
//********************************************************************

  _spill = NULL;

  _isMC = false;
  _softwareVersion = "";

  _previousFile = "";
  _previousRunID = -1;
  _previousSubrunID = -1;
  _previousRefEventID = -1;
  hefficiency_target=NULL;

}

//********************************************************************
bool AnaTreeConverterEvent::Initialize(){
//********************************************************************
    AddChain("ND280upEvents");

  ND280upEvents = GetChain("ND280upEvents");

  fChain = ND280upEvents;

  // Set branch addresses and branch pointers
  nd280UpEvent = new TND280UpEvent();
  if (!ND280upEvents) {
  std::cout<<"non fChain"<<std::endl;
  return false;}
  fCurrent = -1;
  //-------------- Header ---------------------------------
  if (ND280upEvents) {
    ND280upEvents->SetBranchAddress("Event", &nd280UpEvent);;
  }

  fefficiency_target = new TFile("$HIGHLANDIOROOT/data/target_recon_efficiency.root");
  hefficiency_target = (TH2D*)fefficiency_target->Get("theta_length_Eff");

  return true;
}

//********************************************************************
AnaTreeConverterEvent::~AnaTreeConverterEvent(){
//********************************************************************

  if (!fChain) return;
  delete nd280UpEvent;
  if (ND280upEvents         ) delete   ND280upEvents          ->GetCurrentFile();
  if(hefficiency_target) delete hefficiency_target;
 
  if(fefficiency_target) delete fefficiency_target;
  
}

//****************************************************************************
bool AnaTreeConverterEvent::AddFileToTChain(const std::string& inputString){
//****************************************************************************

  std::cout << "AnaTreeConverterEvent::AddFileToTChain(). Adding file: " << inputString << std::endl;


  // Chain only the directories we are interested in

  if (ND280upEvents            )          ND280upEvents->AddFile(inputString.c_str());

  // Read one entry from the tree tree such that Run and Subrun are available
  ND280upEvents->GetEntry(ND280upEvents->GetEntries() - 1);


  // The previous attributes
  _previousFile         = inputString;
  _previousRunID        = 0;
  _previousSubrunID     = 0;
  
  
 return 1;
  }


//*****************************************************************************
Int_t AnaTreeConverterEvent::ReadEntries(Long64_t& entry) {
//*****************************************************************************
  
  Int_t entry_temp = ND280upEvents->GetEntry(entry);

  //-------- Increase the cache size to speed up reading Branches ----------
  static bool first = false;
  if (first){
    if( ND280upEvents ) {
      ND280upEvents->SetCacheSize(200000000);
      ND280upEvents->AddBranchToCache("*",kTRUE);
    }
    first=false;
  }

  return entry_temp;
}

//*****************************************************************************
Int_t AnaTreeConverterEvent::GetSpill(Long64_t& entry, AnaSpillC*& spill){
//*****************************************************************************

  static std::string currentfilename("");

  // Read contents of entry (which correspond to one Spill)
  if (!fChain) return 0;

  std::string filename(ND280upEvents->GetFile()->GetName());

  if( filename != currentfilename ) {
    std::cout << " Running on file: " << filename << std::endl;
    currentfilename = filename;
    
    //load geometry 
    //    ND::hgman().LoadGeometry(filename);
  }

  Int_t entry_temp = ReadEntries(entry);

  if (entry_temp > 0) {
    
    // Create an instance of the Spill
    spill = MakeSpill();
    
    // Cast it to AnaSpillB
    _spill = dynamic_cast<AnaSpill*>(spill);
    
    FillInfo(_spill,entry);
  }
  else{
    std::cout << "Failed in reading entry " << entry << std::endl;
  }
  

  entry++;

  if (entry%1000==0 || entry == _nentries)
      std::cout << "entry: " << entry << " of " << _nentries << " (" << (100*entry/_nentries) << "%)" << std::endl;
  return entry_temp;
}

//********************************************************************
void AnaTreeConverterEvent::IncrementPOTBySpill() {
//********************************************************************
  
}

//*****************************************************************************
void AnaTreeConverterEvent::FillInfo(AnaSpill* spill, Int_t entry){
//*****************************************************************************
  spill->EventInfo = MakeEventInfo();
  AnaEventInfo& info = *dynamic_cast<AnaEventInfo*>(spill->EventInfo);

  info.Run    = 0;
  info.SubRun = 0;
  info.Event  = entry;
  info.IsMC   = 1;

  spill->Beam = MakeBeam();

  
  // beam related information
  FillBeamInfo(dynamic_cast<AnaBeam*>(spill->Beam));


  // True vertex information
  FillTrueInfo(spill);

  AnaBunch* bunch = MakeBunch();
  // All information about each bunch
  FillBunchInfo(spill->TrueVertices, bunch);

  spill->Bunches.push_back(bunch);
}
//*****************************************************************************
void AnaTreeConverterEvent::FillBeamInfo(AnaBeam* beam){
//*****************************************************************************

    beam->GoodSpill = true;
}

//*****************************************************************************
void AnaTreeConverterEvent::FillTrueInfo(AnaSpill* spill){
//*****************************************************************************

 // Fill the true vertices vector
  spill->TrueVertices.clear();
  spill->TrueParticles.clear();
  int NTracks = nd280UpEvent->GetNTracks();
  int nParts = std::min((int)NMAXTRUEPARTICLES, NTracks);
  int NVertices = nd280UpEvent->GetNVertices();
  if(NVertices>1) return;

  int nVertices = std::min((int)NMAXTRUEVERTICES, 1);
      TND280UpVertex *nd280UpVertex = dynamic_cast<TND280UpVertex*>(nd280UpEvent->GetVertex(0));

    AnaTrueVertex* trueVertex = MakeTrueVertex();
    FillTrueVertexInfo(nd280UpVertex, trueVertex);
    
    trueVertex->nTrueParticles = 0;

    // allocate a big array. Once it is filled we will resize it
    anaUtils::CreateArray(trueVertex->TrueParticles, NMAXTRUEPARTICLES);

    for (int i = 0; i < nd280UpEvent->GetNTracks(); i++) {
      TND280UpTrack *nd280UpTrack = dynamic_cast<TND280UpTrack*>(nd280UpEvent->GetTrack(i));
      AnaTrueParticleB* truePart = MakeTrueParticle();
      FillTrueParticleInfo(nd280UpVertex, nd280UpTrack, truePart);
      spill->TrueParticles.push_back(truePart);
      truePart->TrueVertex=trueVertex;
      trueVertex->TrueParticlesVect.push_back(truePart);

    }
       spill->TrueVertices.push_back(trueVertex);

}

//*****************************************************************************
void AnaTreeConverterEvent::FillTrueVertexInfo(TND280UpVertex* ndtrueVertex, AnaTrueVertex* trueVertex){
//*****************************************************************************

  

  trueVertex->ID       = ndtrueVertex->GetVertexID();
    trueVertex->NuPDG    = ndtrueVertex->GetInTrack(0)->GetPDG();
    trueVertex->NuEnergy = ndtrueVertex->GetInTrack(0)->GetInitKinEnergy();
    trueVertex->NuMom = ndtrueVertex->GetInTrack(0)->GetInitMom().Mag();

  trueVertex->Position[0] = ndtrueVertex->GetPosition().X(); //*units::mm;
  trueVertex->Position[1] =ndtrueVertex->GetPosition().Y();//*units::mm;
  trueVertex->Position[2] =ndtrueVertex->GetPosition().Z();//*units::mm;
  trueVertex->Position[3] =ndtrueVertex->GetTime();//*units::s;

    

  trueVertex->ReacCode  =ndtrueVertex->GetReacMode();


 }


//*****************************************************************************
void AnaTreeConverterEvent::FillBunchInfo(std::vector<AnaTrueVertexB*>& TrueVertices, AnaBunch* bunch){
//*****************************************************************************

  bunch->Bunch  = 1;
  bunch->Weight = 1;
 
  for (std::vector<AnaTrueVertexB*>::const_iterator it = TrueVertices.begin(); it != TrueVertices.end(); ++it) {

    std::vector<AnaTrueParticleB*> trueParticles = (*it)->TrueParticlesVect;
  
  for (std::vector<AnaTrueParticleB*>::const_iterator itp = trueParticles.begin(); itp != trueParticles.end(); ++itp) {
      AnaTrackB* part = NULL;
      if (*itp) {
        AnaTrack* part = dynamic_cast<AnaTrack*> (MakeTrack());

        Fill_Tracks_Recon_From_True((*itp), part);
        bunch->Particles.push_back(part);

      }
    }



    AnaVertexB* vertex = MakeVertex();
    FillVertexInfo(*it, vertex, 1);
    vertex->TrueVertex = *it;

    vertex->ParticlesVect = bunch->Particles;
    bunch->Vertices.push_back(vertex);
  }
}



//*****************************************************************************
void AnaTreeConverterEvent::FillVertexInfo(AnaTrueVertexB * truevertex, AnaVertexB* vertex, int bunch){
//*****************************************************************************

  anaUtils::CopyArray(truevertex->Position,vertex->Position,4);
  vertex->Bunch         = bunch;
  truevertex->Bunch=bunch;
}
//*****************************************************************************
AnaTrueObjectC* AnaTreeConverterEvent::FindTrueParticle(Int_t g4id, std::vector<AnaTrueParticleB*>& trueParticles){
//*****************************************************************************


  for (UInt_t i=0;i<trueParticles.size();i++){
    if (trueParticles[i]->ID == g4id) return trueParticles[i];
  }

  return NULL;
}

//*****************************************************************************
void AnaTreeConverterEvent::FillTrueParticleInfo(TND280UpVertex* trueVertex, TND280UpTrack* upTrack, AnaTrueParticleB* truePart){
//*****************************************************************************
  if (upTrack == NULL ) {
    truePart->ID = 0;
    truePart->PDG = 0;
    truePart->ParentID = 0;
    anaUtils::VectorToArray(TLorentzVector(0, 0, 0, 0), truePart->Position);
    truePart->Momentum = 0;
    anaUtils::VectorToArray(TVector3(0, 0, 0), truePart->Direction);
    truePart->Charge = 0;
    truePart->VertexID = -1;
    return;
  }
  truePart->ID = upTrack->GetTrackID();
  truePart->PDG = upTrack->GetPDG();
  truePart->ParentID = upTrack->GetParentID();
  truePart->CosTheta=upTrack->GetInitCosTheta();
double tmpp=-9999;
double tmpd=9999;
int ti=-1;
int td=-1;
  if (upTrack->GetNPoints() > 0) {
    for (int ipt = 0; ipt < upTrack->GetNPoints(); ipt++) {

      if (upTrack->GetPoint(ipt)->GetTime() > tmpp ){
        ti=ipt;
        tmpp=upTrack->GetPoint(ipt)->GetTime();
      }
      if (upTrack->GetPoint(ipt)->GetTime() < tmpd) {
        td=ipt;
        tmpd=upTrack->GetPoint(ipt)->GetTime();
      }
    }
  }
  if(ti>=0 && td>=0){
  anaUtils::VectorToArray(upTrack->GetPoint(td)->GetPostPosition(), truePart->Position);

    anaUtils::VectorToArray(upTrack->GetPoint(ti)->GetPostPosition(), truePart->PositionEnd);
    truePart->PositionEnd[3]=upTrack->GetPoint(ti)->GetTime();
    truePart->Position[3]=upTrack->GetPoint(td)->GetTime();
    if(upTrack->GetPoint(ti)->GetMomentum().Mag()>0){
      truePart->stopped=0;
    }else{
      truePart->stopped=1;
    }
  }
  //}
  truePart->Momentum = upTrack->GetInitMom().Mag();
  double mass = 0.;
  if     (abs(upTrack->GetPDG())==211)  mass = 139.570; // pi+-
  else if(abs(upTrack->GetPDG())==13)   mass = 105.658; // muon
  else if(abs(upTrack->GetPDG())==2212) mass = 938.272; // proton
  else if(abs(upTrack->GetPDG())==11)   mass = 0.511;   // electron

  anaUtils::VectorToArray((1 / truePart->Momentum)*upTrack->GetInitMom(), truePart->Direction);
  truePart->Charge = upTrack->GetCharge();
  truePart->VertexID = trueVertex->GetVertexID();
  truePart->Length=upTrack->GetSDLength();
  truePart->EDeposit=upTrack->GetSDTotalEnergyDeposit();
  truePart->EKin=mass+upTrack->GetInitKinEnergy();
  FindSegments(upTrack,truePart);

  
}
void AnaTreeConverterEvent::FindSegments(TND280UpTrack* upTrack,AnaTrueParticleB* truePart){
 double TPCUp1Length=0;
double TPCUp2Length=0;
double TPCDown1Length=0;
double TPCDown2Length=0;
double ForwTPC1Length=0;
double ForwTPC2Length=0;
double ForwTPC3Length=0;
double Target1Length=0;
double Target2Length=0;


double TPCUp1Z=9999;
double TPCUp2Z=9999;
double TPCDown1Z=9999;
double TPCDown2Z=9999;
double ForwTPC1Z=9999;
double ForwTPC2Z=9999;
double ForwTPC3Z=9999;
double Target1Z=9999;
double Target2Z=9999;

double TPCUp1fZ=-9999;
double TPCUp2fZ=-9999;
double TPCDown1fZ=-9999;
double TPCDown2fZ=-9999;
double ForwTPC1fZ=-9999;
double ForwTPC2fZ=-9999;
double ForwTPC3fZ=-9999;
double Target1fZ=-9999;
double Target2fZ=-9999;



double TPCUp1LengthYZ=0;
double TPCUp2LengthYZ=0;
double TPCDown1LengthYZ=0;
double TPCDown2LengthYZ=0;
double ForwTPC1LengthYZ=0;
double ForwTPC2LengthYZ=0;
double ForwTPC3LengthYZ=0;
double Target1LengthYZ=0;
double Target2LengthYZ=0;

double TPCUp1Edep=0;
double TPCUp2Edep=0;
double TPCDown1Edep=0;
double TPCDown2Edep=0;
double ForwTPC1Edep=0;
double ForwTPC2Edep=0;
double ForwTPC3Edep=0;
double Target1Edep=0;
double Target2Edep=0;


TND280UpTrackPoint* lastTPCUp1=NULL;
TND280UpTrackPoint* lastTPCUp2=NULL;
TND280UpTrackPoint* lastTPCDown1=NULL;
TND280UpTrackPoint* lastTPCDown2=NULL;
TND280UpTrackPoint* lastForwTPC1=NULL;
TND280UpTrackPoint* lastForwTPC2=NULL;
TND280UpTrackPoint* lastForwTPC3=NULL;
TND280UpTrackPoint* lastTarget1=NULL;
TND280UpTrackPoint* lastTarget2=NULL;

TND280UpTrackPoint* firstTPCUp1=NULL;
TND280UpTrackPoint* firstTPCUp2=NULL;
TND280UpTrackPoint* firstTPCDown1=NULL;
TND280UpTrackPoint* firstTPCDown2=NULL;
TND280UpTrackPoint* firstForwTPC1=NULL;
TND280UpTrackPoint* firstForwTPC2=NULL;
TND280UpTrackPoint* firstForwTPC3=NULL;
TND280UpTrackPoint* firstTarget1=NULL;
TND280UpTrackPoint* firstTarget2=NULL;

  for (int ip = 0; ip < upTrack->GetNPoints(); ip++) {
    TND280UpTrackPoint* Tpoint = upTrack->GetPoint(ip);
    if (Tpoint->GetPhysVolName().find("TPCUp1") != std::string::npos) {
      if (Tpoint->GetMomentum().Mag() < TPCUp1Z) {
        lastTPCUp1 = Tpoint;
        TPCUp1Z = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > TPCUp1fZ) {
        firstTPCUp1 = Tpoint;
        TPCUp1fZ = Tpoint->GetMomentum().Mag();
      }

    } 
    if (Tpoint->GetPhysVolName().find("TPCUp2") != std::string::npos) {
  
      if (Tpoint->GetMomentum().Mag() < TPCUp2Z) {
        lastTPCUp2 = Tpoint;
        TPCUp2Z = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > TPCUp2fZ) {
        firstTPCUp2 = Tpoint;
        TPCUp2fZ = Tpoint->GetMomentum().Mag();
      }

    }
     if (Tpoint->GetPhysVolName().find("TPCDown1") != std::string::npos) {

      if (Tpoint->GetMomentum().Mag() < TPCDown1Z) {
        lastTPCDown1 = Tpoint;
        TPCDown1Z = Tpoint->GetMomentum().Mag();
      }
     if (Tpoint->GetMomentum().Mag() > TPCDown1fZ) {
        firstTPCDown1 = Tpoint;
        TPCDown1fZ = Tpoint->GetMomentum().Mag();
      }


    } 
    if (Tpoint->GetPhysVolName().find("TPCDown2") != std::string::npos) {

      if (Tpoint->GetMomentum().Mag() < TPCDown2Z) {
        lastTPCDown2 = Tpoint;
        TPCDown2Z = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > TPCDown2fZ) {
        firstTPCDown2 = Tpoint;
        TPCDown2fZ = Tpoint->GetMomentum().Mag();
      }


    }
    if (Tpoint->GetPhysVolName().find("ForwTPC1") != std::string::npos) {

      if (Tpoint->GetMomentum().Mag() < ForwTPC1Z) {
        lastForwTPC1 = Tpoint;
        ForwTPC1Z = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > ForwTPC1fZ) {
        firstForwTPC1 = Tpoint;
        ForwTPC1fZ = Tpoint->GetMomentum().Mag();
      }


    } 
    if (Tpoint->GetPhysVolName().find("ForwTPC2") != std::string::npos) {
  
      if (Tpoint->GetMomentum().Mag() < ForwTPC2Z) {
        lastForwTPC2 = Tpoint;
        ForwTPC2Z = Tpoint->GetMomentum().Mag();
      }

      if (Tpoint->GetMomentum().Mag() > ForwTPC2fZ) {
        firstForwTPC2 = Tpoint;
        ForwTPC2fZ = Tpoint->GetMomentum().Mag();
      }

    } 
    if (Tpoint->GetPhysVolName().find("ForwTPC3") != std::string::npos) {
   
      if (Tpoint->GetMomentum().Mag() < ForwTPC3Z) {
        lastForwTPC3 = Tpoint;
        ForwTPC3Z = Tpoint->GetMomentum().Mag();
      }

      if (Tpoint->GetMomentum().Mag() > ForwTPC3fZ) {
        firstForwTPC3 = Tpoint;
        ForwTPC3fZ = Tpoint->GetMomentum().Mag();
      }

    } 
    if (Tpoint->GetPhysVolName().find("Target1") != std::string::npos) {
      if (Tpoint->GetMomentum().Mag() < Target1Z) {
        lastTarget1 = Tpoint;
        Target1Z = Tpoint->GetMomentum().Mag();
      }
    if (Tpoint->GetMomentum().Mag() > Target1fZ) {
        firstTarget1 = Tpoint;
        Target1fZ = Tpoint->GetMomentum().Mag();
      }

    } 
    if (Tpoint->GetPhysVolName().find("Target2") != std::string::npos) {
    
      if (Tpoint->GetMomentum().Mag() < Target2Z) {
        lastTarget2 = Tpoint;
        Target2Z = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > Target2fZ) {
        firstTarget2 = Tpoint;
        Target2fZ = Tpoint->GetMomentum().Mag();
      }

    }

}
  int nCrossers=0;
  if(firstTPCUp1){
        nCrossers++;
  }
  if(firstTPCUp2){
       nCrossers++;
  }
  if(firstTPCDown1){
       nCrossers++;
  }
   if(firstTPCDown2){
       nCrossers++;
  }
   if(firstForwTPC1){
       nCrossers++;
  }
   if(firstForwTPC2){
       nCrossers++;
  }
   if(firstForwTPC3){
       nCrossers++;
  }
  if(firstTarget1){
       nCrossers++;
  }
   if(firstTarget2){
       nCrossers++;
  }

  truePart->nDetCrossings=0;
//  anaUtils::CreateArray(truePart->DetCrossings, nCrossers);
  if(firstTPCUp1){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
   // firstTPCUp1->PrintTrackPoint();
    //lastTPCUp1->PrintTrackPoint();

    detCross->EntrancePosition[0] = firstTPCUp1->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstTPCUp1->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstTPCUp1->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstTPCUp1->GetTime();
    detCross->ExitPosition[0] = lastTPCUp1->GetPostPosition().X();
    detCross->ExitPosition[1] = lastTPCUp1->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastTPCUp1->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastTPCUp1->GetTime();
    detCross->EntranceMomentum[0] = firstTPCUp1->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstTPCUp1->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstTPCUp1->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastTPCUp1->GetMomentum().X();
    detCross->ExitMomentum[1] = lastTPCUp1->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastTPCUp1->GetMomentum().Z();
    detCross->DeltaLYZ=upTrack->GetLyzTPCUp1();
    detCross->EDeposit=upTrack->GetEdepTPCUp1();
    detCross->SegLength=upTrack->GetLengthTPCUp1();

    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kTPCUp1);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstTPCUp2){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
  
    detCross->EntrancePosition[0] = firstTPCUp2->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstTPCUp2->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstTPCUp2->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstTPCUp2->GetTime();
    detCross->ExitPosition[0] = lastTPCUp2->GetPostPosition().X();
    detCross->ExitPosition[1] = lastTPCUp2->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastTPCUp2->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastTPCUp2->GetTime();
    detCross->EntranceMomentum[0] = firstTPCUp2->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstTPCUp2->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstTPCUp2->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastTPCUp2->GetMomentum().X();
    detCross->ExitMomentum[1] = lastTPCUp2->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastTPCUp2->GetMomentum().Z();
    detCross->DeltaLYZ=upTrack->GetLyzTPCUp2();
    detCross->EDeposit=upTrack->GetEdepTPCUp2();
    detCross->SegLength=upTrack->GetLengthTPCUp2();
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kTPCUp2);
  
    truePart->DetCrossingsVect.push_back(detCross);
  }

  if (firstTPCDown1) {
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
  
    detCross->EntrancePosition[0] = firstTPCDown1->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstTPCDown1->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstTPCDown1->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstTPCDown1->GetTime();
    detCross->ExitPosition[0] = lastTPCDown1->GetPostPosition().X();
    detCross->ExitPosition[1] = lastTPCDown1->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastTPCDown1->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastTPCDown1->GetTime();
    detCross->EntranceMomentum[0] = firstTPCDown1->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstTPCDown1->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstTPCDown1->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastTPCDown1->GetMomentum().X();
    detCross->ExitMomentum[1] = lastTPCDown1->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastTPCDown1->GetMomentum().Z();
    detCross->DeltaLYZ=upTrack->GetLyzTPCDown1();
    detCross->EDeposit=upTrack->GetEdepTPCDown1();
    detCross->SegLength=upTrack->GetLengthTPCDown1();

    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kTPCDown1);
    truePart->DetCrossingsVect.push_back(detCross);

  }

  if(firstTPCDown2){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
  
    detCross->EntrancePosition[0] = firstTPCDown2->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstTPCDown2->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstTPCDown2->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstTPCDown2->GetTime();
    detCross->ExitPosition[0] = lastTPCDown2->GetPostPosition().X();
    detCross->ExitPosition[1] = lastTPCDown2->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastTPCDown2->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastTPCDown2->GetTime();
    detCross->EntranceMomentum[0] = firstTPCDown2->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstTPCDown2->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstTPCDown2->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastTPCDown2->GetMomentum().X();
    detCross->ExitMomentum[1] = lastTPCDown2->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastTPCDown2->GetMomentum().Z();
    detCross->DeltaLYZ=upTrack->GetLyzTPCDown2();
    detCross->EDeposit=upTrack->GetEdepTPCDown2();
    detCross->SegLength=upTrack->GetLengthTPCDown2();

    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kTPCDown2);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstForwTPC1){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();

    detCross->EntrancePosition[0] = firstForwTPC1->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstForwTPC1->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstForwTPC1->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstForwTPC1->GetTime();
    detCross->ExitPosition[0] = lastForwTPC1->GetPostPosition().X();
    detCross->ExitPosition[1] = lastForwTPC1->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastForwTPC1->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastForwTPC1->GetTime();
    detCross->EntranceMomentum[0] = firstForwTPC1->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstForwTPC1->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstForwTPC1->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastForwTPC1->GetMomentum().X();
    detCross->ExitMomentum[1] = lastForwTPC1->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastForwTPC1->GetMomentum().Z();
    detCross->DeltaLYZ=upTrack->GetLyzForwTPC1();
    detCross->EDeposit=upTrack->GetEdepForwTPC1();
    detCross->SegLength=upTrack->GetLengthForwTPC1();

    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kForwTPC1);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstForwTPC2){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();

    detCross->EntrancePosition[0] = firstForwTPC2->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstForwTPC2->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstForwTPC2->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstForwTPC2->GetTime();
    detCross->ExitPosition[0] = lastForwTPC2->GetPostPosition().X();
    detCross->ExitPosition[1] = lastForwTPC2->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastForwTPC2->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastForwTPC2->GetTime();
    detCross->EntranceMomentum[0] = firstForwTPC2->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstForwTPC2->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstForwTPC2->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastForwTPC2->GetMomentum().X();
    detCross->ExitMomentum[1] = lastForwTPC2->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastForwTPC2->GetMomentum().Z();
    detCross->DeltaLYZ=upTrack->GetLyzForwTPC2();
    detCross->EDeposit=upTrack->GetEdepForwTPC2();
    detCross->SegLength=upTrack->GetLengthForwTPC2();

    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kForwTPC2);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstForwTPC3){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();

    detCross->EntrancePosition[0] = firstForwTPC3->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstForwTPC3->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstForwTPC3->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstForwTPC3->GetTime();
    detCross->ExitPosition[0] = lastForwTPC3->GetPostPosition().X();
    detCross->ExitPosition[1] = lastForwTPC3->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastForwTPC3->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastForwTPC3->GetTime();
    detCross->EntranceMomentum[0] = firstForwTPC3->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstForwTPC3->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstForwTPC3->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastForwTPC3->GetMomentum().X();
    detCross->ExitMomentum[1] = lastForwTPC3->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastForwTPC3->GetMomentum().Z();
    detCross->DeltaLYZ=upTrack->GetLyzForwTPC3();
    detCross->EDeposit=upTrack->GetEdepForwTPC3();
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kForwTPC3);
    detCross->SegLength=upTrack->GetLengthForwTPC3();
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstTarget1){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();

    detCross->EntrancePosition[0] = firstTarget1->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstTarget1->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstTarget1->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstTarget1->GetTime();
    detCross->ExitPosition[0] = lastTarget1->GetPostPosition().X();
    detCross->ExitPosition[1] = lastTarget1->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastTarget1->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastTarget1->GetTime();
    detCross->EntranceMomentum[0] = firstTarget1->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstTarget1->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstTarget1->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastTarget1->GetMomentum().X();
    detCross->ExitMomentum[1] = lastTarget1->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastTarget1->GetMomentum().Z();
    detCross->DeltaLYZ=upTrack->GetLengthTarget1();
    detCross->EDeposit=upTrack->GetEdepTarget1();

    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kTarget1);

    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstTarget2){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();

    detCross->EntrancePosition[0] = firstTarget2->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstTarget2->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstTarget2->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstTarget2->GetTime();
    detCross->ExitPosition[0] = lastTarget2->GetPostPosition().X();
    detCross->ExitPosition[1] = lastTarget2->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastTarget2->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastTarget2->GetTime();
    detCross->EntranceMomentum[0] = firstTarget2->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstTarget2->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstTarget2->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastTarget2->GetMomentum().X();
    detCross->ExitMomentum[1] = lastTarget2->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastTarget2->GetMomentum().Z();
    detCross->DeltaLYZ=upTrack->GetLengthTarget2();
    detCross->EDeposit=upTrack->GetEdepTarget2();
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kTarget2);
    truePart->DetCrossingsVect.push_back(detCross);

  }

}
bool AnaTreeConverterEvent::GetEfficiency(double length,double theta){
    int bin =hefficiency_target->FindBin(theta,length);

    double prob=hefficiency_target->GetBinContent(bin);
 
    if(gRandom->Uniform()>prob){
      return 1;
    }else{
      return 0;
    }
}
void AnaTreeConverterEvent::Fill_Tracks_Recon_From_True(AnaTrueParticleB* trueParticle, AnaTrack* reconParticle) {

  reconParticle->UniqueID       = trueParticle->ID;
  reconParticle->Momentum       = trueParticle->Momentum;
  reconParticle->Charge         = trueParticle->Charge;
  reconParticle->Length         = trueParticle->Length;
  reconParticle->EDeposit         = trueParticle->EDeposit;
  reconParticle->EKin             = trueParticle->EKin;
  anaUtils::CopyArray(trueParticle->Direction, reconParticle->DirectionStart, 3);
  anaUtils::CopyArray(trueParticle->Position,  reconParticle->PositionStart, 4);
  anaUtils::CopyArray(trueParticle->PositionEnd,   reconParticle->PositionEnd, 4);
  reconParticle->MomentumEle = -10000;
  reconParticle->MomentumMuon = -10000;
  reconParticle->MomentumProton = -10000;
  reconParticle->MomentumErrorEle = -10000;
  reconParticle->MomentumErrorMuon = -10000;
  reconParticle->MomentumErrorProton = -10000;
  reconParticle->nTargetSegments = 0;
  reconParticle->nTPCSegments = 0;
  reconParticle->TrueObject=dynamic_cast<AnaTrueObjectC*>(trueParticle);
  for (int i = 0; i < trueParticle->DetCrossingsVect.size(); i++) {

    if (SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kTPCUp1) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kTPCUp2) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kTPCDown1) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kTPCDown2) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kForwTPC2) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kForwTPC2) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kForwTPC3)) {
      AnaTPCParticleB* seg = dynamic_cast<AnaTPCParticleB*>(MakeTPCTrack());
      Float_t mom = sqrt(trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] + trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] + trueParticle->DetCrossingsVect[i]->EntranceMomentum[2] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[2]);
      anaUtils::VectorToArray(TVector3(trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] / mom, trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] / mom, trueParticle->DetCrossingsVect[i]->EntranceMomentum[2] / mom), seg->DirectionStart);
      anaUtils::VectorToArray(TVector3(trueParticle->DetCrossingsVect[i]->ExitMomentum[0] / mom, trueParticle->DetCrossingsVect[i]->ExitMomentum[1] / mom, trueParticle->DetCrossingsVect[i]->ExitMomentum[2] / mom), seg->DirectionEnd);
      anaUtils::CopyArray(trueParticle->DetCrossingsVect[i]->EntrancePosition, seg->PositionStart, 4);
      anaUtils::CopyArray(trueParticle->DetCrossingsVect[i]->ExitPosition,  seg->PositionEnd, 4);
      SubDetId::SubDetEnum dsub = SubDetId::GetSubdetectorEnum(trueParticle->DetCrossingsVect[i]->Detector);
 
      SubDetId::SetDetectorUsed(reconParticle->Detectors, dsub);
 
      seg->Detectors=trueParticle->DetCrossingsVect[i]->Detector;
 
      seg->EDeposit = trueParticle->DetCrossingsVect[i]->EDeposit;
      seg->Momentum = mom;
      seg->DeltaLYZ = trueParticle->DetCrossingsVect[i]->DeltaLYZ;
      seg->SegLength = trueParticle->DetCrossingsVect[i]->SegLength;
      anaUtils::ComputeTPCPull(*seg,*reconParticle);
      reconParticle->TPCSegments[reconParticle->nTPCSegments++] = seg;
 

    }
    if (SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kTarget1) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kTarget2)) {
      AnaTargetParticleB* seg = dynamic_cast<AnaTargetParticleB*>(MakeTargetTrack() );
      Float_t mom = sqrt(trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] + trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] + trueParticle->DetCrossingsVect[i]->EntranceMomentum[2] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[2]);
      anaUtils::VectorToArray(TVector3(trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] / mom, trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] / mom, trueParticle->DetCrossingsVect[i]->EntranceMomentum[2] / mom), seg->DirectionStart);
      anaUtils::VectorToArray(TVector3(trueParticle->DetCrossingsVect[i]->ExitMomentum[0] / mom, trueParticle->DetCrossingsVect[i]->ExitMomentum[1] / mom, trueParticle->DetCrossingsVect[i]->ExitMomentum[2] / mom), seg->DirectionEnd);
      anaUtils::CopyArray(trueParticle->DetCrossingsVect[i]->EntrancePosition, seg->PositionStart, 4);
      anaUtils::CopyArray(trueParticle->DetCrossingsVect[i]->ExitPosition,  seg->PositionEnd, 4);
      seg->DeltaLYZ = trueParticle->DetCrossingsVect[i]->DeltaLYZ;

      seg->EDeposit = trueParticle->DetCrossingsVect[i]->EDeposit;
      SubDetId::SubDetEnum dsub = SubDetId::GetSubdetectorEnum(trueParticle->DetCrossingsVect[i]->Detector);
      SubDetId::SetDetectorUsed(reconParticle->Detectors, dsub);
 
      seg->Detectors = trueParticle->DetCrossingsVect[i]->Detector;
      seg->IsReconstructed = GetEfficiency(seg->DeltaLYZ, trueParticle->CosTheta);
      reconParticle->TargetSegments[reconParticle->nTargetSegments++] = seg;

    }
  }
    
}