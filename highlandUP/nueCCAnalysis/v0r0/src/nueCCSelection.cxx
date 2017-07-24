#include "baseSelection.hxx"
#include "CutUtils.hxx"
#include "EventBoxUtils.hxx"
#include "EventBox.hxx"
#include "VersioningUtils.hxx"
#include "SystematicUtils.hxx"
#include "SelectionUtils.hxx"
#include "AnaRecPackManager.hxx"
#include "TreeConverterUtils.hxx"

#include "nueCCSelection.hxx"
#include "nueCCUtils.hxx"

#include <TH2F.h>
#include <TAxis.h>

#include <utility>
#include <map>

//********************************************************************
nueCCSelection::nueCCSelection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxNDUP) {
  //********************************************************************

  char filename[256];
  sprintf(filename, "%s/data/ECalPID.root", getenv("NUECCANALYSISROOT"));
  _file_ECAL_PDF = TFile::Open(filename);

  // Get charge confusion splines
  sprintf(filename, "%s/data/FGD_1_ChargeConfusion_TrueMom.root", getenv("NUECCANALYSISROOT"));
  _file_charge_confusion_1 = TFile::Open(filename);
  
  sprintf(filename, "%s/data/FGD_2_ChargeConfusion_TrueMom.root", getenv("NUECCANALYSISROOT"));
  _file_charge_confusion_2 = TFile::Open(filename);

  // Ecal
  _ECal_reco_eff = new BinnedParams(std::string(getenv("NUECCANALYSISROOT")) + "/data",
				    "ECal_recoEff", BinnedParams::k1D_SYMMETRIC); 
  
  _ECal_FGDmatch_eff = new BinnedParams(std::string(getenv("NUECCANALYSISROOT")) + "/data",
					"ECal_FGDmatchEff", BinnedParams::k1D_SYMMETRIC); 

  _randomGen = new TRandom3();
  
}

//********************************************************************
void nueCCSelection::DefineSteps(){
  //********************************************************************


  //last true means that if that cut is not fulfill the sequence is stop
  AddStep(StepBase::kAction, "find true vertex",      new FindTrueVertexAction());

  AddStep(StepBase::kCut,    "> 0 tracks ",           new TotalMultiplicityCut(), true);
  
  // Find the main track
  AddStep(StepBase::kAction, "Sort TPC tracks",       new SortTracksAction());

  // FV and quality
  AddStep(StepBase::kCut,    "quality+fiducial",      new TrackGQandFVCut());

  AddStep(StepBase::kAction, "find vertex",           new FindVertexAction());
  AddStep(StepBase::kAction, "fill summary",          new FillSummaryAction_nueCC());
  //AddStep(StepBase::kAction, "find pions",            new FindPionsAction());

  // Track length cut
  AddStep(StepBase::kCut,    "TPC Quality Cut",       new TPC_Quality());

  // ToF info
  AddStep(StepBase::kAction, "ToF",                   new ToF_senseDetermination());

  // for tracks with cos theta < 0 check the ToF
  AddStep(StepBase::kCut,    "ToF BWD tracks",        new ToF_BWD_cut());

  // TPC+ECal PID
  AddStep(StepBase::kCut,    "TPC Electron PID Cut",  new TPCElectronPID());
  AddStep(StepBase::kCut,    "ECal PID Cut",          new ECal_PID(_file_ECAL_PDF));
  AddStep(StepBase::kCut,    "TPC Muon PID Cut",      new TPCMuonPID()); // For events not going in the Ecal

  // Pair cut
  AddStep(StepBase::kAction, "find best e+e- pair",   new FindPairsAction());
  AddStep(StepBase::kCut,    "Pair Veto",	            new PairCut());

  // TPC Veto
  AddStep(StepBase::kCut,    "TPC Veto Cut",          new TPCVeto());

  SetBranchAlias(0,  "CCNuE Inclusive");
  //SetPreSelectionAccumLevel(0);

}

//********************************************************************
void nueCCSelection::DefineDetectorFV(){
  //********************************************************************

  // Change FV definition to take all thickness
  // The detector in which the selection is applied
  bool useTarget1 = ND::params().GetParameterI("nueCCAnalysis.EnableTarget1");
  bool useTarget2 = ND::params().GetParameterI("nueCCAnalysis.EnableTarget2");
  bool useFGD1 = ND::params().GetParameterI("nueCCAnalysis.EnableFGD1");
  bool useFGD2 = ND::params().GetParameterI("nueCCAnalysis.EnableFGD2");

  // WARNING in case of the combine analysis for target and FGDs the FV will be set to FGD
  if ( useTarget1 && !useTarget2) SetDetectorFV(SubDetId::kTarget1);
  if (!useTarget1 &&  useTarget2) SetDetectorFV(SubDetId::kTarget2);
  if ( useTarget1 &&  useTarget2) SetDetectorFV(SubDetId::kTarget);

  if ( useFGD1 && !useFGD2) SetDetectorFV(SubDetId::kFGD1);
  if (!useFGD1 &&  useFGD2) SetDetectorFV(SubDetId::kFGD2);
  if ( useFGD1 &&  useFGD2) SetDetectorFV(SubDetId::kFGD);
}

//**************************************************
void nueCCSelection::InitializeEvent(AnaEventC& eventBB){
  //**************************************************

  AnaEventB& event = *static_cast<AnaEventB*>(&eventBB);

  // Create the appropriate EventBox if it does not exist yet
  if (!event.EventBoxes[EventBoxId::kEventBoxNDUP]) event.EventBoxes[EventBoxId::kEventBoxNDUP] = new EventBox();

  // fill the tracks in a proper way
  bool useTarget1 = ND::params().GetParameterI("nueCCAnalysis.EnableTarget1");
  bool useTarget2 = ND::params().GetParameterI("nueCCAnalysis.EnableTarget2");
  bool useFGD1 = ND::params().GetParameterI("nueCCAnalysis.EnableFGD1");
  bool useFGD2 = ND::params().GetParameterI("nueCCAnalysis.EnableFGD2");

  // for the referenc configuration it is excessive if you specify the certain target
  if (useTarget1 || useTarget2) {
    boxUtils::FillTracksWithTPC(event,    static_cast<SubDetId::SubDetEnum>(SubDetId::kTarget));
    boxUtils::FillTracksWithTarget(event, static_cast<SubDetId::SubDetEnum>(SubDetId::kTarget));
  }

  if (useFGD1 || useFGD2) {
    if (GetDetectorFV() == SubDetId::kFGD) {
      boxUtils::FillTracksWithTPC(event,    static_cast<SubDetId::SubDetEnum>(SubDetId::kFGD));
      boxUtils::FillTracksWithFGD(event,    static_cast<SubDetId::SubDetEnum>(SubDetId::kFGD));
    } else 
      boxUtils::FillTracksWithFGD(event,    static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  }
 
  boxUtils::FillTracksWithECal(event);  
}

//********************************************************************
bool nueCCSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
  //********************************************************************

  if(allCutsPassed[0]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuECC;

  return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kTarget1NuECC);

}

//**************************************************
bool FindTrueVertexAction::Apply(AnaEventC & event, ToyBoxB & boxB) const {
  //**************************************************
  
  (void)event;

  ToyBoxCC4pi& box  = static_cast<ToyBoxCC4pi&>(boxB);
  AnaEventB& eventB = static_cast<AnaEventB&>(event);
  
  // Cast the ToyBox to the appropriate type
  for (Int_t i = 0; i < eventB.nVertices; i++) {
    AnaVertexB* vertex = static_cast<AnaVertexB*>(eventB.Vertices[i]);
    box.Vertex = vertex->Clone();
  }
  
  if (!box.Vertex) return false;
  box.TrueVertex = box.Vertex->TrueVertex;
  //std::cout << "vertex" << std::endl;

  AnaTrueParticleB* trueTrack = NULL;
  // TODO understand why there are no tracks with ParentID == EleID
  //std::cout << box.TrueVertex->TrueParticlesVect.size() << std::endl;
  /*for (int i = 0; i < box.TrueVertex->TrueParticlesVect.size(); i++) {
    if (abs(box.TrueVertex->TrueParticlesVect[i]->PDG) == 11 && box.TrueVertex->TrueParticlesVect[i]->ParentID == 0) {
      trueTrack = static_cast<AnaTrueParticleB*>(box.TrueVertex->TrueParticlesVect[i]);
      if (trueTrack) 
        break;
    }
  }

  if (!trueTrack)
    return true;
  //std::cout << "true track" << std::endl;

  // save the ID of the electron track
  Int_t EleID = trueTrack->ID;

  // loop over all true track in the event, searching for the electron daughter
  //std::cout << eventB.nTrueParticles << std::endl;
  for (Int_t i = 0; i < eventB.nTrueParticles; ++i) {
    if (!eventB.TrueParticles[i])
      continue;
    //std::cout << eventB.TrueParticles[i]->ParentID << "   " << EleID << std::endl;
    if (eventB.TrueParticles[i]->PDG == 22) {
      std::cout << "ID   "<< eventB.TrueParticles[i]->ParentID << "   " << eventB.TrueParticles[i]->GParentID << "    " << trueTrack->ID << std::endl;
      std::cout << "PDG  "<< eventB.TrueParticles[i]->ParentPDG <<"   " << eventB.TrueParticles[i]->GParentPDG <<"    " << eventB.TrueParticles[i]->PDG << std::endl;
      std::cout << "Position " << eventB.TrueParticles[i]->Position[2] << "     Vertex " << box.TrueVertex->Position[2] << std::endl;
    }
    for (Int_t j = 0; j < eventB.nTrueParticles; ++j) {
      if (!eventB.TrueParticles[j])
        continue;
      if (eventB.TrueParticles[i]->ParentID ==  eventB.TrueParticles[j]->ID && eventB.TrueParticles[i]->PDG == 22)
        std::cout << "FOUND" << eventB.TrueParticles[j]->PDG << std::endl;
    }
    if (eventB.TrueParticles[i]->ParentID == EleID || eventB.TrueParticles[i]->GParentID == EleID) {
      box.daughterPDG = eventB.TrueParticles[i]->PDG;
      std::cout << "OK" << std::endl;
      break;
    }
  }*/
  return true;
}

//**************************************************
bool TotalMultiplicityCut::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  
  (void)box;
  
  // Check we have at least one reconstructed track in the FGD
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];

  /*std::cout << "call TotalMultiplicityCut" << std::endl;
  std::cout << "Detector = " << useTarget1 << useTarget2 << useFGD1 << useFGD2 << std::endl;
  
  std::cout << EventBox->nRecObjectsInGroup[EventBox::kTracksWithTarget1] << "  " << 
  EventBox->nRecObjectsInGroup[EventBox::kTracksWithTarget2] << "  " << 
  EventBox->nRecObjectsInGroup[EventBox::kTracksWithFGD1] << "  " << 
  EventBox->nRecObjectsInGroup[EventBox::kTracksWithFGD2] << "  " << std::endl;

  //std::cout << "FV " << sel().GetDetectorFV() << std::endl;

  std::cout << "return  " << ((useTarget1 && EventBox->nRecObjectsInGroup[EventBox::kTracksWithTarget1]>0) ||
    (useTarget2 && EventBox->nRecObjectsInGroup[EventBox::kTracksWithTarget2]>0) ||
    (useFGD1    && EventBox->nRecObjectsInGroup[EventBox::kTracksWithFGD1]>0   ) ||
    (useFGD2    && EventBox->nRecObjectsInGroup[EventBox::kTracksWithFGD2]>0   )) << std::endl;*/
  
  return ((useTarget1 && EventBox->nRecObjectsInGroup[EventBox::kTracksWithTarget1]>0) ||
	  (useTarget2 && EventBox->nRecObjectsInGroup[EventBox::kTracksWithTarget2]>0) ||
	  (useFGD1    && EventBox->nRecObjectsInGroup[EventBox::kTracksWithFGD1]>0   ) ||
	  (useFGD2    && EventBox->nRecObjectsInGroup[EventBox::kTracksWithFGD2]>0   ));
  
}

//**************************************************
bool SortTracksAction::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  
  // Retrieve the EventBoxNDUP
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];
  
  //========================
  //Find TPCGoodQuality tracks in Fwd and Bwd
  //========================
  if (useTarget1) {
    
    int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget1FV];
    for (Int_t i=0;i<nTPC; ++i){
      AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget1FV][i]);

      // is selection charge is 1 then select positrons, if -1 then electrons. If 0 then don't care about the charge, select the highest momentum
      if(selection_charge > -0.5){
	if(track->Charge <= 0.5) continue;
      }
      else if (selection_charge < 0.5) {
	if ( track->Charge>=-0.5 ) continue;
      }
      
      cc4pibox->TPCTracks.push_back(track);
    }
    
  }
  if (useTarget2) {
    
    int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget2FV];
    for (Int_t i=0;i<nTPC; ++i){
      AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget2FV][i]);

      if(selection_charge > -0.5){
	if(track->Charge <= 0.5) continue;
      }
      else if (selection_charge < 0.5) {
	if ( track->Charge>=-0.5 ) continue;
      }
      
      cc4pibox->TPCTracks.push_back(track);
    }
    
  }
  if (useFGD1) {
    
    int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInFGD1FV];
    for (Int_t i=0;i<nTPC; ++i){
      AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInFGD1FV][i]);

      if(selection_charge > -0.5){
	if(track->Charge <= 0.5) continue;
      }
      else if (selection_charge < 0.5) {
	if ( track->Charge>=-0.5 ) continue;
      }
      
      cc4pibox->TPCTracks.push_back(track);
    }
    
  }
  if (useFGD2) {
    
    int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInFGD2FV];
    for (Int_t i=0;i<nTPC; ++i){
      AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInFGD2FV][i]);

      if(selection_charge > -0.5){
	if(track->Charge <= 0.5) continue;
      }
      else if (selection_charge < 0.5) {
	if ( track->Charge>=-0.5 ) continue;
      }
      
      cc4pibox->TPCTracks.push_back(track);
    }
    
  }
 
  // Sort TPCGoodQuality using Momentum
  std::sort(cc4pibox->TPCTracks.begin(), cc4pibox->TPCTracks.end(), nueCCUtils::HGlobalMomFirst);
  
  //========================
  // Find ECal only tracks
  //========================  
  int nECALTracks=EventBox->nRecObjectsInGroup[EventBox::kTracksWithECal];
  for(int i=0;i<nECALTracks;i++){
    AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithECal][i]);
    if ( (useTarget1 && anaUtils::InFiducialVolume(SubDetId::kTarget1, track->PositionStart)) ||
	 (useTarget2 && anaUtils::InFiducialVolume(SubDetId::kTarget2, track->PositionStart)) ||
	 (useFGD1    && anaUtils::InFiducialVolume(SubDetId::kFGD1,    track->PositionStart)) ||
	 (useFGD2    && anaUtils::InFiducialVolume(SubDetId::kFGD2,    track->PositionStart))) {
      if ( SubDetId::GetDetectorUsed(track->Detector,SubDetId::kDsECal)) continue;
      if ( cutUtils::DeltaLYZTPCCut(*track) )                            continue;
      if ( fabs(track->Charge) < 1e-3 ) continue;
      cc4pibox->ECalTracks.push_back(track);
    }
  }

  // Sort ECalTracks using RangeMomentum
  std::sort(cc4pibox->ECalTracks.begin(), cc4pibox->ECalTracks.end(), nueCCUtils::HGlobalMomFirst);
  
  if      (cc4pibox->TPCTracks.size()>0)
    cc4pibox->MainTrack = cc4pibox->TPCTracks[0];
  //else if (cc4pibox->ECalTracks.size()>0)
  //cc4pibox->MainTrack = cc4pibox->ECalTracks[0];
  else return false;  

  // Protection
  if(cc4pibox->MainTrack->SmearedMomentum <= 0.0) return false;
  
  cc4pibox->TPC_det = anaUtils::GetClosestTPC(*cc4pibox->MainTrack);

  //========================
  // Find TPC veto track
  //========================
  int nTPCs=EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPC];
  for (Int_t i=0;i<nTPCs; ++i){
    AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithTPC][i]);
    if(track == cc4pibox->MainTrack) continue;
    if(!cc4pibox->TPCVetoTrack)
      cc4pibox->TPCVetoTrack = track;
    else{
      if(track->SmearedMomentum > cc4pibox->TPCVetoTrack->SmearedMomentum)
	cc4pibox->TPCVetoTrack = track;
    }
  }

  //========================
  // Find ECal veto track
  //========================
  AnaTrackB* ecalonlytracks[500];
  Int_t nEcalOnly = anaUtils::GetAllTracksUsingOnlyDet(static_cast<const AnaEventB&> (event), SubDetId::kBrlECal, ecalonlytracks);
  for(Int_t i=0; i<nEcalOnly; i++){
    AnaTrackB* ecaltrack = ecalonlytracks[i];
    if(ecaltrack->PositionStart[2] == -999) continue;
    if(!cc4pibox->ECalVetoTrack)
      cc4pibox->ECalVetoTrack = ecaltrack;
    else{
      if(ecaltrack->PositionStart[2] < cc4pibox->ECalVetoTrack->PositionStart[2])
	cc4pibox->ECalVetoTrack = ecaltrack;
    }
  }

  //========================
  // Find Gamma Iso track
  //========================
  const Float_t vertex_activity = 50.0;
  if (useTarget1) {
    int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget1FV];
    for (Int_t i=0;i<nTPC; ++i){
      AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget1FV][i]);
      if(track == cc4pibox->MainTrack) continue;

      Float_t diff = (anaUtils::ArrayToTVector3(track->PositionStart) -
		      anaUtils::ArrayToTVector3(cc4pibox->MainTrack->PositionStart)).Mag();
      
      if(diff > vertex_activity)
	cc4pibox->NFarTracks++;
      else
	cc4pibox->NNearTracks++;
    }
  }

  if (useTarget2) {
    int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget2FV];
    for (Int_t i=0;i<nTPC; ++i){
      AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget2FV][i]);
      if(track == cc4pibox->MainTrack) continue;

      Float_t diff = (anaUtils::ArrayToTVector3(track->PositionStart) -
		      anaUtils::ArrayToTVector3(cc4pibox->MainTrack->PositionStart)).Mag();
      
      if(diff > vertex_activity)
	cc4pibox->NFarTracks++;
      else
	cc4pibox->NNearTracks++;
    }
  }

  return true;
  
}

//**************************************************
bool TrackGQandFVCut::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  
  if (!cc4pibox->MainTrack)
    return false;
  
  if ( (useTarget1 && anaUtils::InFiducialVolume(SubDetId::kTarget1, cc4pibox->MainTrack->PositionStart,
						 LAFVmin, LAFVmax)) ||
       (useTarget2 && anaUtils::InFiducialVolume(SubDetId::kTarget2, cc4pibox->MainTrack->PositionStart,
						 LAFVmin, LAFVmax)) ||
       (useFGD1 && anaUtils::InFiducialVolume(SubDetId::kFGD1, cc4pibox->MainTrack->PositionStart,
					      LAFVmin, LAFVmax)) ||
       (useFGD2 && anaUtils::InFiducialVolume(SubDetId::kFGD2, cc4pibox->MainTrack->PositionStart,
					      LAFVmin, LAFVmax)))
    return true;
  
  return false;
  
}

//**************************************************
bool FindVertexAction::Apply(AnaEventC& event, ToyBoxB& boxB) const{
  //**************************************************
  
  (void)event;
  
  ToyBoxNDUP& box = *static_cast<ToyBoxNDUP*>(&boxB);
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&boxB);
  
  // reset the vertex
  box.Vertex = NULL;
  box.TrueVertex = NULL;
  
  if ( !cc4pibox->MainTrack ) return false;
  box.MainTrack = cc4pibox->MainTrack;
  
  box.Vertex = new AnaVertexB();
  anaUtils::CreateArray(box.Vertex->Particles, 1);
  
  box.Vertex->nParticles = 0;
  box.Vertex->Particles[box.Vertex->nParticles++] = box.MainTrack;
  
  for(int i = 0; i < 4; ++i) box.Vertex->Position[i] = box.MainTrack->PositionStart[i];
  if ( box.MainTrack->GetTrueParticle() ) box.TrueVertex = box.Vertex->TrueVertex = cc4pibox->MainTrack->GetTrueParticle()->TrueVertex;
  
  return true;
  
}

//********************************************************************
bool FillSummaryAction_nueCC::Apply(AnaEventC& event, ToyBoxB& boxB) const{
  //********************************************************************
  
  // Cast the ToyBox to the appropriate type
  ToyBoxNDUP& box = *static_cast<ToyBoxNDUP*>(&boxB);
  
  if(!box.MainTrack) return 1;
  
  static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kTarget1NuECC] = box.MainTrack;
  
  for(int i = 0; i < 4; ++i) static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kTarget1NuECC][i] = box.MainTrack->PositionStart[i];
  if(box.MainTrack->GetTrueParticle()) static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kTarget1NuECC] = box.MainTrack->GetTrueParticle()->TrueVertex;
  
  static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kTarget2NuECC] = box.MainTrack;
  
  for(int i = 0; i < 4; ++i) static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kTarget2NuECC][i] = box.MainTrack->PositionStart[i];
  if(box.MainTrack->GetTrueParticle()) static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kTarget2NuECC] = box.MainTrack->GetTrueParticle()->TrueVertex;
  
  static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD1NuECC] = box.MainTrack;
  
  for(int i = 0; i < 4; ++i) static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD1NuECC][i] = box.MainTrack->PositionStart[i];
  if(box.MainTrack->GetTrueParticle()) static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD1NuECC] = box.MainTrack->GetTrueParticle()->TrueVertex;
  
  static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD2NuECC] = box.MainTrack;
  
  for(int i = 0; i < 4; ++i) static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD2NuECC][i] = box.MainTrack->PositionStart[i];
  if(box.MainTrack->GetTrueParticle()) static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD2NuECC] = box.MainTrack->GetTrueParticle()->TrueVertex;
  
  return true;
  
}

//**************************************************
bool TPC_Quality::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if(!cutUtils::DeltaLYZTPCCut(*cc4pibox->MainTrack)) return false;
  
  return true;

}

//**************************************************
bool ToF_BWD_cut::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);

  if (!cc4pibox->MainTrack)
    return false;

  return (!conf || cc4pibox->MainTrack->DirectionStart[2] > 0 || cc4pibox->main_ToF > 0);
}

//**************************************************
bool TPCElectronPID::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);

  return ( nueCCUtils::TPCElectronPIDCut(*cc4pibox->MainTrack, lik_elec) );

}

//**************************************************
bool TPCMuonPID::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);

  if(cc4pibox->MainTrack->SmearedMomentum > lik_mom && cc4pibox->track_ECal_MipEM  != -999.0) return true;

  return ( nueCCUtils::TPCMuonPIDCut(*cc4pibox->MainTrack, lik_muon) );

}

//**************************************************
bool ECal_PID::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************

  (void)event;

  if (!_file_ECAL_PDF) {
    std::cerr << "ERROR::ECALPIDCut:: No file found to define the PDFs for ECal variables. Will die." << std::endl;
    exit(0);
  }
  
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  
  AnaTrueParticleB* trueParticle = cc4pibox->MainTrack->GetTrueParticle();
  if (!trueParticle) return false;
  
  bool IsEcalReco = false;
  //std::cout << "test" << std::endl;
  for(int i=0; i<cc4pibox->MainTrack->nECalSegments; i++){
    //std::cout << "test" << std::endl;
    if(cc4pibox->MainTrack->ECalSegments[i]->IsReconstructed){
      IsEcalReco = true;
      //std::cout << "WOW" << std::endl;
      break;
    }
  }
  
  // Check if track is actually reconstructed in ecal
  if(!IsEcalReco){
    cc4pibox->track_ECal_MipEM  = -999.0;
    cc4pibox->track_ECal_EneOnL = -999.0;
    
    return true;
  }
  
  // find the first segment in an ECal
  AnaDetCrossingB* firstECal_cross = NULL;
  for (unsigned int i=0; i<trueParticle->DetCrossingsVect.size(); i++) {
    AnaDetCrossingB* cross = trueParticle->DetCrossingsVect[i];
    if (!cross->Detector_name.Contains("ECal") || !cross->Detector_name.Contains("Bar")) continue;
    firstECal_cross = cross;
  }
  
  // fill the information at ECal entry
  if (!firstECal_cross){
    cc4pibox->track_ECal_MipEM  = -999.0;
    cc4pibox->track_ECal_EneOnL = -999.0;
    
    return true;
  }
 
  Int_t findecal = -1;
  if (firstECal_cross->Detector_name.Contains("RightClam") &&
      firstECal_cross->Detector_name.Contains("BotLeftTopRight"))
    findecal = 1;
  else if (firstECal_cross->Detector_name.Contains("RightClam") &&
	   firstECal_cross->Detector_name.Contains("TopLeftBotRight"))
    findecal = 1;
  else if (firstECal_cross->Detector_name.Contains("LeftClam") &&
	   firstECal_cross->Detector_name.Contains("BotLeftTopRight"))
     findecal = 1;
  else if (firstECal_cross->Detector_name.Contains("LeftClam") &&
	   firstECal_cross->Detector_name.Contains("TopLeftBotRight"))
    findecal = 1;
  else if (firstECal_cross->Detector_name.Contains("LeftSide"))
    findecal = 1;
  else if (firstECal_cross->Detector_name.Contains("RightSide"))
    findecal = 1;
  else if(firstECal_cross->Detector_name.Contains("POD/USECal"))
    findecal = -1;
  else
    findecal = 0;

  // No P0DEcal
  if(findecal < 0){
    cc4pibox->track_ECal_MipEM  = -999.0;
    cc4pibox->track_ECal_EneOnL = -999.0;
    
    return true;
  }


  TAxis *h_binning = (TAxis*)_file_ECAL_PDF->Get("MomentumBins");
  Int_t nbins = h_binning->GetNbins();
  Double_t mombins[nbins];
  for (int i=1; i <= nbins; i++) {
    mombins[i-1] = h_binning->GetBinLowEdge(i);
  }
  
  TVector3 P   = anaUtils::ArrayToTVector3(firstECal_cross->EntranceMomentum);
  float mom    = P.Mag();
  if(mom > mombins[nbins-1])
    mom = mombins[nbins-1];

  Int_t findmombin = -1;
  for(Int_t k = 0; k < nbins-1; k++){
    if( mom > mombins[k] && mom <= mombins[k+1] ){
      findmombin = k;
      break;
    }
  }

  if(findmombin < 0){
    cc4pibox->track_ECal_MipEM  = -999.0;
    cc4pibox->track_ECal_EneOnL = -999.0;
    
    return true;
  }

  TString histname;
  if( abs(trueParticle->PDG) == 11 ){
    histname.Form("ElecHisto_%i_%i-%i",findecal,(int)mombins[findmombin],(int)mombins[findmombin+1]);
  }
  else if( abs(trueParticle->PDG) == 13 )
    histname.Form("MuonHisto_%i_%i-%i",findecal,(int)mombins[findmombin],(int)mombins[findmombin+1]);
  else if( abs(trueParticle->PDG) == 211 )
    histname.Form("PionHisto_%i_%i-%i",findecal,(int)mombins[findmombin],(int)mombins[findmombin+1]);
  else
    histname.Form("ProtHisto_%i_%i-%i",findecal,(int)mombins[findmombin],(int)mombins[findmombin+1]);

  TH1D* histo = (TH1D*)_file_ECAL_PDF->Get(histname.Data());
  cc4pibox->track_ECal_MipEM  = histo->GetRandom();
  cc4pibox->track_ECal_EneOnL = -999.0;

  delete h_binning;
  delete histo;

  if(cc4pibox->track_ECal_MipEM < 0.0) return false;

  return true;

}

//**************************************************
bool TPCVeto::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);

  if(!cc4pibox->TPCVetoTrack) return true;

  Float_t DZ = cc4pibox->TPCVetoTrack->PositionStart[2] - cc4pibox->MainTrack->PositionStart[2];

  return (DZ > DeltaZ);

}

//**************************************************
bool ToF_senseDetermination::Apply(AnaEventC& event, ToyBoxB& boxB) const{
  //**************************************************
  
  (void)event;

  ToyBoxCC4pi *box = static_cast<ToyBoxCC4pi*>(&boxB);
  if (!box->MainTrack) return false;

  AnaParticleB *p1=0, *p2=0;
  float sigma=0;
  // Get the ToF and sgment with maximum t/dt
  Float_t ToF = anaUtils::GetToF(box->MainTrack, p1, p2, sigma, _randomGen, true);

  if (ToF == -999 || !p1 || !p2)
    return false;

  // save ToF
  box->main_ToF      = ToF;

  // save true ToF
  Float_t true_ToF = p2->PositionStart[3]-p1->PositionStart[3];
  box->main_ToF_true = true_ToF;

  // calculate the track length between segment with t/dt
  Float_t sigma_length;
  Float_t length      = anaUtils::GetLength(box->MainTrack, p1, p2, sigma_length, _randomGen, true);
  Float_t true_length = anaUtils::GetLength(box->MainTrack, p1, p2, sigma_length, _randomGen, false);

  Float_t true_mom = box->MainTrack->GetTrueParticle()->Momentum;
  Float_t mom = box->MainTrack->SmearedMomentum;
  if (fabs(true_mom-mom)<1e-3)
    mom = _randomGen->Gaus(true_mom, 0.10*true_mom);

  // store detectors used
  SubDetId::SubDetEnum det1, det2;
  anaUtils::GetToFdetectors(p1, p2, det1, det2);
  box->FirstToF  = det1;
  box->SecondToF = det2;

  // store beta value
  double c = 299.792458; // mm/ns
  box->beta_ToF       = length / (c*ToF);
  box->beta_true_ToF  = true_length / (c*true_ToF);

  // store ToF mass
  box->ToF_mass      = anaUtils::ComputeToFMass(mom,      ToF,      length);
  box->ToF_true_mass = anaUtils::ComputeToFMass(true_mom, true_ToF, true_length);

  // get Llh
  Float_t ToF_llh[4];
  anaUtils::GetToFLikelihood(mom, box->MainTrack->MomentumError, ToF, sigma, length, sigma_length, ToF_llh, _randomGen, true);

  // store Llh
  box->ToF_lkl_muon     = ToF_llh[0];
  box->ToF_lkl_pion     = ToF_llh[3];
  box->ToF_lkl_electron = ToF_llh[1];
  box->ToF_lkl_proton   = ToF_llh[2];

  return true;
}

//**************************************************
bool FindPairsAction::Apply(AnaEventC& eventC, ToyBoxB& boxB) const {
  //**************************************************

  AnaEventB& event = *static_cast<AnaEventB*>(&eventC); 
  
  // Cast the ToyBox to the appropriate type
  ToyBoxCC4pi *box = static_cast<ToyBoxCC4pi*>(&boxB);

  AnaTrackB* track = box->MainTrack;
  if(!track) return false;
  
  // Find the best pair track candidate
  Float_t distance = 100.0;
  box->PairTrack = nueCCUtils::FindPairTrack(event, *track, distance);
 
  return true;
}

//**************************************************
bool PairCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
  //**************************************************
  
  (void) event;
  
  // Cast the ToyBox to the appropriate type
  ToyBoxCC4pi *box = static_cast<ToyBoxCC4pi*>(&boxB);

  AnaTrackB* track = box->MainTrack;
  if(!track) return false;

  AnaTrackB* pair = box->PairTrack;
  if(!pair) return true;

  Float_t mass = 0.511;

  // The minimum invariant mass required to pass the cut.
  TLorentzVector secondary_PE = nueCCUtils::GetMomLorentzVector(*pair, mass);
  TLorentzVector primary_PE   = nueCCUtils::GetMomLorentzVector(*track, mass);
  Float_t minv = 2. * mass * mass + 2. * (secondary_PE.Dot(primary_PE));

  if(minv < 0) return true; // backward track?

  minv = sqrt(minv);

  box->InvariantMass = minv;

  if(minv < _inv_mass_min) return false;

  return true;

}

//**************************************************
bool nueCCSelection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
  //**************************************************
	
  (void)event;
  (void)box;

  if(systId == SystId::kPileUp){
    if (branch==1) return false;
  }

  return true;

}

//**************************************************
bool nueCCSelection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
  //**************************************************

  (void)event;
  (void)branch;

  AnaTrackB* track = static_cast<AnaTrackB*>(recObj);

  if (!track) return false;
  if (!track->TrueObject) return false;

  return true;

}

//**************************************************
bool nueCCSelection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
  //**************************************************
	
  (void)event;
  (void)branch;

  AnaTrueParticleB* trueTrack = static_cast<AnaTrueParticleB*>(trueObj);

  if (!trueTrack) return false;

  return true;

}

//**************************************************
bool nueCCSelection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
  //**************************************************

  (void)event;
  (void)branch;
  
  return true;

}

//**************************************************
bool nueCCSelection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
  //**************************************************

  (void)event;
  (void)branch;

  return true;

}

//********************************************************************
bool nueCCSelection::CheckRedoSelection(const AnaEventC& eventBB, const ToyBoxB& PreviousToyBoxB, Int_t& redoFromStep){
  //********************************************************************

  (void)eventBB;
  (void)PreviousToyBox;
  (void)redoFromStep;

  return true;

}