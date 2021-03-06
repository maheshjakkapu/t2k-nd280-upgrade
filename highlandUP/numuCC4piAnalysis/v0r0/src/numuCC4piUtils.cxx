#include <TVector3.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

#include "TRandom3.h"
#include "TH2F.h"

#include "AnalysisUtils.hxx"
#include "EventBoxId.hxx"
#include "EventBox.hxx"
#include "ND280UPAnalysisUtils.hxx"
#include "CategoriesUtils.hxx"
#include "CutUtils.hxx"
#include "AnaRecPackManager.hxx"

#include "numuCC4piSelection.hxx"
#include "numuCC4piUtils.hxx"

#include "Parameters.hxx"

//********************************************************************
int numuCC4pi_utils::CC4piCategory(AnaTrack* candidate){
  //********************************************************************
	
  if (!candidate) return -1;
  AnaTrueParticle* truetrack = static_cast<AnaTrueParticle*>( candidate->TrueObject );
  if (!truetrack) return -1;
  AnaTrueVertex* truevtx = static_cast<AnaTrueVertex*>( truetrack->TrueVertex );
  if (!truevtx) return -1;
	
  bool muon = false;
  bool FV = false;
  bool CC = false;
  muon =  truetrack->Momentum == truevtx->LeptonMom;
	
  bool useTarget1 = ND::params().GetParameterI("numuCC4piAnalysis.EnableTarget1");
  bool useTarget2 = ND::params().GetParameterI("numuCC4piAnalysis.EnableTarget2");
  bool useFGD1 = ND::params().GetParameterI("numuCC4piAnalysis.EnableFGD1");
  bool useFGD2 = ND::params().GetParameterI("numuCC4piAnalysis.EnableFGD2");
  FV = ((useTarget1 && anaUtils::InFiducialVolume( SubDetId::kTarget1, truevtx->Position )) ||
	(useTarget2 && anaUtils::InFiducialVolume( SubDetId::kTarget2, truevtx->Position )) ||
	(useFGD1 && anaUtils::InFiducialVolume( SubDetId::kFGD1, truevtx->Position )) ||
	(useFGD2 && anaUtils::InFiducialVolume( SubDetId::kFGD2, truevtx->Position )));
	
  int reac = anaUtils::GetReactionNoTargetFv( *static_cast<AnaTrueVertex*>(truevtx) );
  CC = ((reac==0) || (reac==1) || (reac==2) || (reac==3) || (reac==9)) && truevtx->LeptonMom>0 && truevtx->LeptonPDG==13;
	
  if (FV) {
    if(  muon &&  CC ) return 0;
    if( !muon &&  CC ) return 1;
    if( !CC )          return 2;
  }
  else return 3;
	
  return -1;
	
}



//********************************************************************
int numuCC4pi_utils::OOFVCategory(AnaTrack* candidate, int topo){
  //********************************************************************
	
  if(!candidate) return -1;
  AnaTrueParticle* truetrack = static_cast<AnaTrueParticle*>( candidate->TrueObject );
  if (!truetrack) return -1;
  AnaTrueVertex* truevtx = static_cast<AnaTrueVertex*>( truetrack->TrueVertex );
  if (!truevtx) return -1;
	
  Float_t* tvertex = truevtx->Position;
	
  bool useTarget1 = ND::params().GetParameterI("numuCC4piAnalysis.EnableTarget1");
  bool useTarget2 = ND::params().GetParameterI("numuCC4piAnalysis.EnableTarget2");
	
  if((useTarget1 && anaUtils::InFiducialVolume( SubDetId::kTarget1, truevtx->Position )) ||
     (useTarget2 && anaUtils::InFiducialVolume( SubDetId::kTarget2, truevtx->Position )))
    return 0;
	
  Float_t* tdir = truetrack->Direction;
  Float_t* pos  = truetrack->Position;
	
  Float_t* fgd_det_min;
  Float_t* fgd_det_max;
  double zup,zdown;
  bool lastmodule;
  fgd_det_min     = DetDef::Target1min;
  fgd_det_max     = DetDef::Target1max;
  zup             =-60;
  zdown           = 623;
  bool layer28=( pos[2] >429 && pos[2] <435 );
  bool layer29=( pos[2] >442 && pos[2] <445 );
  lastmodule=(layer28 || layer29);
	
  //inTarget1scint. Put z condition first to be faster
  if ( tvertex[2]>fgd_det_min[2] && tvertex[2]<fgd_det_max[2] && tvertex[0]>fgd_det_min[0] && tvertex[0]<fgd_det_max[0] && tvertex[1]>fgd_det_min[1] && tvertex[1]<fgd_det_max[1] )	{
    if (tvertex[2]<candidate->PositionStart[2]) return 1;
    else return 2;
  }
  //upstreamTarget1scint
  if ( tvertex[2]>zup && tvertex[2]<fgd_det_min[2] && tvertex[0]>-1335 && tvertex[0]<1335 && tvertex[1]>-1280.5 && tvertex[1]<1280.5 ) return 3;
  //downstreamTarget1scint
  if ( tvertex[2]>fgd_det_max[2] && tvertex[2]<zdown && tvertex[0]>-1335 && tvertex[0]<1335 && tvertex[1]>-1280.5 && tvertex[1]<1280.5 ) return 4;
  //neutralparent
  if ( truetrack->ParentPDG  == 2112 || truetrack->ParentPDG  == 22 ||  truetrack->GParentPDG == 2112 || truetrack->GParentPDG == 22) return 5;
  if (topo==0) {
    //backwards
    if (tdir[2]<0) return 6;
    //highangle
    if (tdir[2]>=0 && tdir[2]<0.5) return 7;
    //veryforward
    if(fabs(tdir[0]/tdir[2])<0.07 || fabs(tdir[1]/tdir[2])<0.07) return 8;
  }
  if (topo==1) {
    //forwards
    if (tdir[2]>0) return 6;
    //highangle
    if (tdir[2]<=0 && tdir[2]>-0.5) return 7;
    //verybackward
    if(fabs(tdir[0]/tdir[2])<0.07 || fabs(tdir[1]/tdir[2])<0.07) return 8;
  }
  //layer2829
  if ( lastmodule ) return 9;
  //other
  return 10;
	
}
//********************************************************************
bool numuCC4pi_utils::IsForward(const AnaTrackB& track){
  //********************************************************************
	
  // check if a track is forward (in Z) going
  // need something fancier?
	
  return track.PositionStart[2] < track.PositionEnd[2];  
}

//********************************************************************
//int numuCC4pi_utils::GetFgdLayer(Float_t* pos){
//********************************************************************
/*
  Float_t initZ=0;
  Float_t finalZ=0;
  Float_t barCenter[30];
	
  for (Int_t i=0; i<30; i++) {
  barCenter[i] = DetDef::Target1BarCenter[i];
  if (i==0) initZ = barCenter[0]-(DetDef::TargetXYBarWidth/2)-DetDef::TargetXYGlueWidth-DetDef::TargetXYSkinWidth-DetDef::TargetXYAirWidth;
  else initZ = finalZ;
  if      (i%2==0) finalZ = barCenter[i]+(DetDef::TargetXYBarWidth/2)+(DetDef::TargetXYMiddleGlueWidth/2);
  else if (i%2==1) finalZ = barCenter[i]+(DetDef::TargetXYBarWidth/2)+DetDef::TargetXYGlueWidth+DetDef::TargetXYSkinWidth+(DetDef::TargetXYAirWidth/2);
  if (i==29) finalZ += (DetDef::TargetXYAirWidth/2);
  if (pos[2]>initZ && pos[2]<finalZ) return i;
  }
	
  return -1;
	
  }
*/

// ********************************************************************
Float_t* numuCC4pi_utils::GetFittedPos(Float_t* start, Float_t* end, double z){
  // ********************************************************************
	
  double t = (z-start[2])/(end[2]-start[2]);
  Float_t* pos = new Float_t(3);
  pos[0] = start[0]+(end[0]-start[0])*t;
  pos[1] = start[1]+(end[1]-start[1])*t;
  pos[2] = z;
	
  return pos;
	
}

// ********************************************************************
Float_t numuCC4pi_utils::GetDeltaPos(Float_t* hit, Float_t* fit){
  // ********************************************************************
	
  Float_t delta = 0;
  if ( hit[0]>-4.6 && hit[0]<0.2 )  delta = hit[1] - fit[1];
  if ( hit[1]>50.4 && hit[1]<55.2 ) delta = hit[0] - fit[0];
	
  return delta;
	
}

// ********************************************************************
Float_t numuCC4pi_utils::GetDeltaPos2(Float_t* hit, Float_t* fit){
  // ********************************************************************
	
  Float_t delta = 0;
  if ( hit[0]>-4.6 && hit[0]<0.2 )  delta = hit[1]*hit[1] - fit[1]*fit[1];
  if ( hit[1]>50.4 && hit[1]<55.2 ) delta = hit[0]*hit[0] - fit[0]*fit[0];
	
  return delta;
	
}

//********************************************************************
bool numuCC4pi_utils::HGlobalMomFirst(AnaTrackB* a, AnaTrackB* b){
  //********************************************************************
	
  return a->SmearedMomentum > b->SmearedMomentum;
	
}

//********************************************************************
bool numuCC4pi_utils::isHMNT(const AnaTrackB* candidate, std::vector<AnaTrackB*> GroupTracks, bool lowangle) {
  //********************************************************************
	
  bool useTarget1 = ND::params().GetParameterI("numuCC4piAnalysis.EnableTarget1");
  bool useTarget2 = ND::params().GetParameterI("numuCC4piAnalysis.EnableTarget2");
	
  for (UInt_t i=0;i<GroupTracks.size(); ++i){
    if (lowangle) {
      if ( useTarget1 && anaUtils::InFiducialVolume(SubDetId::kTarget1, GroupTracks[i]->PositionStart, LAFVmin, LAFVmax) ) return GroupTracks[i]->SmearedMomentum == candidate->SmearedMomentum;
      if ( useTarget2 && anaUtils::InFiducialVolume(SubDetId::kTarget2, GroupTracks[i]->PositionStart, LAFVmin, LAFVmax) ) return GroupTracks[i]->SmearedMomentum == candidate->SmearedMomentum;
    }
    else {
      if ( useTarget1 && anaUtils::InFiducialVolume(SubDetId::kTarget1, GroupTracks[i]->PositionStart, HAFVmin, HAFVmax) ) return GroupTracks[i]->SmearedMomentum == candidate->SmearedMomentum;
      if ( useTarget2 && anaUtils::InFiducialVolume(SubDetId::kTarget2, GroupTracks[i]->PositionStart, HAFVmin, HAFVmax) ) return GroupTracks[i]->SmearedMomentum == candidate->SmearedMomentum;
    }
  }
	
  return false;
	
}

//********************************************************************
//bool numuCC4pi_utils::VetoCut(int topo, const AnaEventB& event, AnaTrackB& candidate){
//********************************************************************
/*
  if (topo==0) {
  AnaTrackB* VetoTrack = cutUtils::FindVetoTrack(event, candidate);
  if (VetoTrack) {
  if (VetoTrack->PositionStart[2]-candidate.PositionStart[2]<-100. && VetoTrack->Momentum/candidate.Momentum>0.8) return false;
  }
  return true;
  }
  if (topo==2) {
  AnaTrackB* VetoTrack = cutUtils::FindVetoTrack(event, candidate);
  if (VetoTrack) {
  if (VetoTrack->PositionStart[2]-candidate.PositionStart[2]<-150. && VetoTrack->Momentum/candidate.RangeMomentumMuon>0.9) return false;
  }
  return true;
  }
  if (topo==3) {
  AnaTrackB* VetoTrack = cutUtils::FindVetoTrack(event, candidate);
  if (VetoTrack) {
  if (VetoTrack->PositionStart[2]-candidate.PositionStart[2]<-400. && VetoTrack->Momentum/candidate.RangeMomentumMuon>0.9) return false;
  }
  return true;
  }
  return true;
	
  }
*/

//********************************************************************
//bool numuCC4pi_utils::ExternalCut(int topo, const AnaEventC& event, AnaTrackB& candidate){
//********************************************************************
/*
  if ( topo==0 ) {
  if ( GetTargetLayer(candidate.PositionStart) > 27 ) {
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];
  int nFgdTracks=EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithTarget1AndNoTPC];
  for (Int_t i=0;i<nTargetTracks;i++){
  AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithTarget1AndNoTPC][i]);
  AnaTargetParticleB *TargetTrack = track->TargetSegments[0];
  if( !TargetTrack ) continue;
  if( SubDetId::GetDetectorUsed(TargetTrack->Detector,SubDetId::kTarget1) ) return false;
  }
  }
  return true;
  }
  return true;
	
  }
*/

//**************************************************
bool numuCC4pi_utils::MuonPIDCut(const AnaTrackB& candidate, bool ApplyMIP) {
  //**************************************************
	
  Float_t cut1 = ND::params().GetParameterD("numuCC4piAnalysis.MuonPID.Cut.Lmip");
  Float_t cut2 = ND::params().GetParameterD("numuCC4piAnalysis.MuonPID.Cut.Lmu");
  Float_t pcut = ND::params().GetParameterD("numuCC4piAnalysis.MuonPID.Cut.p_mip");
	
  if (ApplyMIP) {
    if ( (anaUtils::GetPIDLikelihoodMIP(candidate) > cut1 || candidate.Momentum > pcut) &&
	 (anaUtils::GetPIDLikelihood(candidate, 0) > cut2) ) {
      return true;
    }
  }
  else
    if ( anaUtils::GetPIDLikelihood(candidate, 0) > cut2 ) {
      return true;
    }
	
  return false;
	
}

//**************************************************
bool numuCC4pi_utils::MuonECalPIDCut(const AnaTrackB& candidate, ToyBoxB& box, TFile* _file_ECAL_PDF) {
  //**************************************************

    // initializing
    ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  
    if (!_file_ECAL_PDF) {
      std::cerr << "In Utils::MuonECALPIDCut : No file found to define the PDFs for ECal variables" << std::endl;
      return false;
    }

    TH2F *h_binning = (TH2F*)_file_ECAL_PDF->Get("hBinning");
    AnaTrueParticleB* trueParticle = candidate.GetTrueParticle();
    if (!trueParticle) return false;
  
    // find the first segment in an ECal
    AnaDetCrossingB* firstECal_cross=NULL;
    for (unsigned int i=0; i<trueParticle->DetCrossingsVect.size(); i++) {
      AnaDetCrossingB* cross = trueParticle->DetCrossingsVect[i];
      if (!cross->Detector_name.Contains("ECal") || !cross->Detector_name.Contains("Bar")) continue;
      firstECal_cross = cross;
    }

    // fill the information at ECal entry
    if (!firstECal_cross)
      return false;
  
    TVector3 P   = anaUtils::ArrayToTVector3(firstECal_cross->EntranceMomentum);
    TVector3 pos = anaUtils::ArrayToTVector3(firstECal_cross->EntrancePosition);
    TVector3 entryNormal_vect(0,0,0);
    
    if (firstECal_cross->Detector_name.Contains("RightClam") &&
	firstECal_cross->Detector_name.Contains("BotLeftTopRight"))
      entryNormal_vect.SetY(1);  // (+Y)
    else if (firstECal_cross->Detector_name.Contains("RightClam") &&
	     firstECal_cross->Detector_name.Contains("TopLeftBotRight"))
      entryNormal_vect.SetY(-1); // (-Y)
    else if (firstECal_cross->Detector_name.Contains("LeftClam") &&
	     firstECal_cross->Detector_name.Contains("BotLeftTopRight"))
      entryNormal_vect.SetY(-1); // (-Y)
    else if (firstECal_cross->Detector_name.Contains("LeftClam") &&
	     firstECal_cross->Detector_name.Contains("TopLeftBotRight"))
      entryNormal_vect.SetY(1);  // (+Y)
    else if (firstECal_cross->Detector_name.Contains("LeftSide"))
      entryNormal_vect.SetX(1);  // (+X)
    else if (firstECal_cross->Detector_name.Contains("RightSide"))
      entryNormal_vect.SetX(-1); // (-X)
    else if(firstECal_cross->Detector_name.Contains("POD/USECal"))
      entryNormal_vect.SetZ(-1); // (-Z)
    else
      entryNormal_vect.SetZ(1);  // (+Z)

    float mom = P.Mag();
    float cos = P.Dot(entryNormal_vect)/mom;
    int   bin = h_binning->GetBinContent(h_binning->FindBin(cos,mom));

  
    // throw random the ECal variables using the pdfs
  
    TH2F *h_bin  = (TH2F*)_file_ECAL_PDF->Get(TString::Format("mipem_Vs_EneOnL_%i", bin));
    double MipEM, EneOnL;
    h_bin->GetRandom2(MipEM, EneOnL);

    // fill the ECal variables in the box
    cc4pibox->track_ECal_MipEM  = MipEM;
    cc4pibox->track_ECal_EneOnL = EneOnL;
  
    // apply cuts on ECal variables
  
    if (MipEM < 0 && EneOnL < 1.25)
      return true;
    return false;
	
}

//*********************************************************************
void numuCC4pi_utils::FindTPCPions(AnaEventC& event, ToyBoxB& boxB, SubDetId::SubDetEnum det){
  //*********************************************************************
	
  ToyBoxCC4pi* box = static_cast<ToyBoxCC4pi*>(&boxB);
	
  box->nPositivePionTPCtracks = 0;
  box->nPosPi0TPCtracks = 0;
  box->nNegativePionTPCtracks = 0;
  box->nElPi0TPCtracks = 0;
  AnaEventB& eventB = *static_cast<AnaEventB*>(&event); 
	
  EventBox::RecObjectGroupEnum groupID;
  if      (det==SubDetId::kTarget1) groupID = EventBox::kTracksWithGoodQualityTPCInTarget1FV;
  else if (det==SubDetId::kTarget2) groupID = EventBox::kTracksWithGoodQualityTPCInTarget2FV;
  else if (det==SubDetId::kFGD1)    groupID = EventBox::kTracksWithGoodQualityTPCInFGD1FV;
  else if (det==SubDetId::kFGD2)    groupID = EventBox::kTracksWithGoodQualityTPCInFGD2FV;
  else return;
	
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];
	
  // Look for pions in positive tracks 
  for(int i = 0; i < EventBox->nRecObjectsInGroup[groupID]; i++ ) {
	
    AnaTrackB *ptrack = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[groupID][i]);
    if( box->MainTrack == ptrack ) continue; // Same as the leading track.
    if (ptrack->Momentum < 20) continue;
		
    if (ptrack->Charge>0){
			
      Float_t PIDLikelihood[4];
      anaUtils::GetPIDLikelihood(*ptrack, PIDLikelihood);
			
      // For Positive tracks we distinguish pions, electrons and protons.
      double ElLklh = PIDLikelihood[1];  
      double ProtonLklh = PIDLikelihood[2];  
      double PionLklh = PIDLikelihood[3];
      double norm = ElLklh+ProtonLklh+PionLklh;
      ProtonLklh /= norm; 
      ElLklh /= norm; 
      PionLklh /= norm; 
			
      if( ProtonLklh > ElLklh && ProtonLklh > PionLklh )
	continue;
			
      // Id associated to the largest of the two probabilities.
      if (PionLklh > ElLklh) {
	box->nPositivePionTPCtracks++;
	box->PositivePionTPCtracks.push_back(ptrack);
      }
      else {
	if (ptrack->Momentum > 900.) // This is mainly protons.
	  continue;
	box->nPosPi0TPCtracks++;
	box->PosPi0TPCtracks.push_back(ptrack);
      }
    }
		
    else if (ptrack->Charge<0){
			
      // For Negative tracks we distinguish pions, electrons and protons.
      Float_t PIDLikelihood[4];
      anaUtils::GetPIDLikelihood(*ptrack, PIDLikelihood);
			
      // For Positive tracks we distinguish pions, electrons and protons.
      double ElLklh = PIDLikelihood[1];  
      double PionLklh = PIDLikelihood[3];  
      double norm = ElLklh+PionLklh;
      ElLklh /= norm; 
      PionLklh /= norm;
			
      if( PionLklh > 0.8 ) {
	box->nNegativePionTPCtracks++;
	box->NegativePionTPCtracks.push_back(ptrack);
      }
      else {
	box->nElPi0TPCtracks++;
	box->ElPi0TPCtracks.push_back(ptrack);
      }
			
    }
		
  } // end loop on tracks
	
}

//*********************************************************************
void numuCC4pi_utils::FindIsoTargetPions(AnaEventC& event, ToyBoxB& boxB, SubDetId::SubDetEnum det){
  //*********************************************************************
	
  ToyBoxCC4pi* box = static_cast<ToyBoxCC4pi*>(&boxB);
  box->nIsoTargetPiontracks = 0;
	
  EventBox::RecObjectGroupEnum groupID;
  if      (det==SubDetId::kTarget1) groupID = EventBox::kTracksWithTarget1;
  else if (det==SubDetId::kTarget2) groupID = EventBox::kTracksWithTarget2;
  else if (det==SubDetId::kFGD1)    groupID = EventBox::kTracksWithFGD1;
  else if (det==SubDetId::kFGD2)    groupID = EventBox::kTracksWithFGD2;
  else return;
    
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];
    
  //loop over tracks
  for (int i=0;i<EventBox->nRecObjectsInGroup[groupID]; i++ ){
    AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[groupID][i]);
    if (track == box->MainTrack) continue;
		
    if (!anaUtils::InFiducialVolume(det, track->PositionStart) ||
	!anaUtils::InFiducialVolume(det, track->PositionEnd) )
      continue;
				
    if (det==SubDetId::kTarget1 || det==SubDetId::kTarget2) {
      if (track->nTargetSegments < 1) continue;
			
      AnaTargetParticleB* TargetSegment = 
	dynamic_cast<AnaTargetParticleB*>(anaUtils::GetSegmentInDet(*track, det));
      if (!TargetSegment) continue;
      if (!TargetSegment->IsReconstructed) continue;
      if (TargetSegment->IdAsProton) continue;

    }
    if (det==SubDetId::kFGD1 || det==SubDetId::kFGD2) {
      if (track->nFGDSegments < 1) continue;
		
      AnaFGDParticleB* FGDSegment = 
	dynamic_cast<AnaFGDParticleB*>(anaUtils::GetSegmentInDet(*track, det));
      if (!FGDSegment) continue;
      if (!FGDSegment->IsReconstructed) continue;
			
      AnaTrueParticleB* true_part = track->GetTrueParticle();
      if (!true_part) continue;

      float r=gRandom->Rndm();
      float cut=0;
      if (abs(true_part->PDG) == 211 || abs(true_part->PDG) == 13)
	cut = ND::params().GetParameterD("numuCC4piAnalysis.IsoTargetPi.PID_MuonToMuon_FGD");
      else
	cut = 1-ND::params().GetParameterD("numuCC4piAnalysis.IsoTargetPi.PID_ProtonToProton_FGD");
      if (r<cut) {
	box->nIsoTargetPiontracks++;
	box->IsoTargetPiontracks.push_back(track);
      }

    }
		
  }
}


//*********************************************************************
void numuCC4pi_utils::FindMEPions(AnaEventC& event, ToyBoxB& boxB, SubDetId::SubDetEnum det){
  //*********************************************************************
	
  ToyBoxCC4pi* box = static_cast<ToyBoxCC4pi*>(&boxB);
  box->nMichelElectrons = 0;
	
  Float_t ME_eff_Target = ND::params().GetParameterD("numuCC4piAnalysis.MEPions.ME.EfficiencyTarget");
  Float_t ME_eff_FGD1   = ND::params().GetParameterD("numuCC4piAnalysis.MEPions.ME.EfficiencyFGD1");
  Float_t ME_eff_FGD2   = ND::params().GetParameterD("numuCC4piAnalysis.MEPions.ME.EfficiencyFGD2");
  Float_t ME_mom        = ND::params().GetParameterD("numuCC4piAnalysis.MEPions.ME.MomentumCut");
	
  EventBox::RecObjectGroupEnum groupID;
  if      (det==SubDetId::kTarget1) groupID = EventBox::kTracksWithTarget1;
  else if (det==SubDetId::kTarget2) groupID = EventBox::kTracksWithTarget2;
  else if (det==SubDetId::kFGD1)    groupID = EventBox::kTracksWithFGD1;
  else if (det==SubDetId::kFGD2)    groupID = EventBox::kTracksWithFGD2;
  else return;
    
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];
	
  // true muon candidate
  if (!box->MainTrack) return;
  if (!box->MainTrack->TrueObject) return;
  AnaTrueParticleB* true_mu = box->MainTrack->GetTrueParticle();
	
  //loop over tracks
  for (int i=0;i<EventBox->nRecObjectsInGroup[groupID]; i++ ){
    AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[groupID][i]);
    if (track == box->MainTrack) continue;
    if (!anaUtils::InFiducialVolume(det, track->PositionStart) ||
	!anaUtils::InFiducialVolume(det, track->PositionEnd)) continue;
		
    if (det==SubDetId::kTarget1 || det==SubDetId::kTarget2) {
      if (track->nTargetSegments < 1) continue;
			
      AnaTargetParticleB* TargetSegment = 
	dynamic_cast<AnaTargetParticleB*>(anaUtils::GetSegmentInDet(*track, det));
      if (!TargetSegment) continue;
			
      AnaTrueParticleB* true_part = track->GetTrueParticle();
      if (!true_part) continue;
	  
      if (abs(true_part->PDG) == 11)
	if (true_part->Position[3]-true_mu->Position[3] > 100 && true_part->Momentum > ME_mom) {
	  double r = gRandom->Rndm();
	  if (r < ME_eff_Target) {
	    box->nMichelElectrons++;
	    box->MichelElectrons.push_back(track);
	  }
	}
    }
    if (det==SubDetId::kFGD1 || det==SubDetId::kFGD2) {
      if (track->nFGDSegments < 1) continue;
			
      AnaFGDParticleB* FGDSegment = 
	dynamic_cast<AnaFGDParticleB*>(anaUtils::GetSegmentInDet(*track, det));
      if (!FGDSegment) continue;
			
      AnaTrueParticleB* true_part = track->GetTrueParticle();
      if (!true_part) continue;
      if (abs(true_part->PDG) == 11)
	if (true_part->Position[3]-true_mu->Position[3] > 100 && true_part->Momentum > ME_mom) {
	  double r = gRandom->Rndm();
	  if ((det==SubDetId::kFGD1 && r < ME_eff_FGD1) ||
	      (det==SubDetId::kFGD2 && r < ME_eff_FGD2)) {
	    box->nMichelElectrons++;
	    box->MichelElectrons.push_back(track);
	  }
	}
    }
		
  }
	
}


//*********************************************************************
void numuCC4pi_utils::FindTPCProtons(AnaEventC& event, ToyBoxB& boxB, SubDetId::SubDetEnum det){
  //*********************************************************************
	
  ToyBoxCC4pi* box = static_cast<ToyBoxCC4pi*>(&boxB);
  AnaEventB& eventB = *static_cast<AnaEventB*>(&event); 
	
  box->nProtonTPCtracks = 0;

  EventBox::RecObjectGroupEnum groupID;
  if      (det==SubDetId::kTarget1) groupID = EventBox::kTracksWithGoodQualityTPCInTarget1FV;
  else if (det==SubDetId::kTarget2) groupID = EventBox::kTracksWithGoodQualityTPCInTarget2FV;
  else if (det==SubDetId::kFGD1)    groupID = EventBox::kTracksWithGoodQualityTPCInFGD1FV;
  else if (det==SubDetId::kFGD2)    groupID = EventBox::kTracksWithGoodQualityTPCInFGD2FV;
  else return;
	
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];
	
  // Look for pions in positive tracks 
  for(int i = 0; i < EventBox->nRecObjectsInGroup[groupID]; i++ ) {
	
    AnaTrackB *ptrack = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[groupID][i]);
    if( box->MainTrack == ptrack ) continue; // Same as the leading track.
    if (ptrack->Momentum < 20) continue;
    if (ptrack->Charge<0) continue;
			
      Float_t PIDLikelihood[4];
      anaUtils::GetPIDLikelihood(*ptrack, PIDLikelihood);
			
      // For Positive tracks we distinguish pions, electrons and protons.
      double ElLklh = PIDLikelihood[1];  
      double ProtonLklh = PIDLikelihood[2];  
      double PionLklh = PIDLikelihood[3];
      double norm = ElLklh+ProtonLklh+PionLklh;
      ProtonLklh /= norm; 
      ElLklh /= norm; 
      PionLklh /= norm; 
			
      if( ProtonLklh > ElLklh && ProtonLklh > PionLklh ) {
	box->nProtonTPCtracks++;
	box->ProtonTPCtracks.push_back(ptrack);
      }
			
      // Id associated to the largest of the two probabilities.
      if (PionLklh < ElLklh && ptrack->Momentum > 900.) {
	box->nProtonTPCtracks++;
	box->ProtonTPCtracks.push_back(ptrack);
      }
	
  } // end loop on tracks
	
}

//*********************************************************************
void numuCC4pi_utils::FindIsoTargetProtons(AnaEventC& event, ToyBoxB& boxB, SubDetId::SubDetEnum det){
  //*********************************************************************
	
  ToyBoxCC4pi* box = static_cast<ToyBoxCC4pi*>(&boxB);
  box->nIsoTargetProtontracks = 0;
	
  EventBox::RecObjectGroupEnum groupID;
  if      (det==SubDetId::kTarget1) groupID = EventBox::kTracksWithTarget1;
  else if (det==SubDetId::kTarget2) groupID = EventBox::kTracksWithTarget2;
  else if (det==SubDetId::kFGD1)    groupID = EventBox::kTracksWithFGD1;
  else if (det==SubDetId::kFGD2)    groupID = EventBox::kTracksWithFGD2;
  else return;
    
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];
    
  //loop over tracks
  for (int i=0;i<EventBox->nRecObjectsInGroup[groupID]; i++ ){
    AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[groupID][i]);
    if (track == box->MainTrack) continue;
		
    if (!anaUtils::InFiducialVolume(det, track->PositionStart) ||
	!anaUtils::InFiducialVolume(det, track->PositionEnd) )
      continue;
				
    if (det==SubDetId::kTarget1 || det==SubDetId::kTarget2) {
      if (track->nTargetSegments < 1) continue;
			
      AnaTargetParticleB* TargetSegment = 
	dynamic_cast<AnaTargetParticleB*>(anaUtils::GetSegmentInDet(*track, det));
      if (!TargetSegment) continue;
      if (!TargetSegment->IsReconstructed) continue;
      if (TargetSegment->IdAsProton) {
	box->nIsoTargetProtontracks++;
	box->IsoTargetProtontracks.push_back(track);
      }

    }
    if (det==SubDetId::kFGD1 || det==SubDetId::kFGD2) {
      if (track->nFGDSegments < 1) continue;
		
      AnaFGDParticleB* FGDSegment = 
	dynamic_cast<AnaFGDParticleB*>(anaUtils::GetSegmentInDet(*track, det));
      if (!FGDSegment) continue;
      if (!FGDSegment->IsReconstructed) continue;
			
      AnaTrueParticleB* true_part = track->GetTrueParticle();
      if (!true_part) continue;

      float r=gRandom->Rndm();
      float cut=0;
      if (abs(true_part->PDG) == 211 || abs(true_part->PDG) == 13)
	cut = 1-ND::params().GetParameterD("numuCC4piAnalysis.IsoTargetPi.PID_MuonToMuon_FGD");
      else
	cut = ND::params().GetParameterD("numuCC4piAnalysis.IsoTargetPi.PID_ProtonToProton_FGD");
      if (r<cut) {
	box->nIsoTargetProtontracks++;
	box->IsoTargetProtontracks.push_back(track);
      }

    }
		
  }
}
