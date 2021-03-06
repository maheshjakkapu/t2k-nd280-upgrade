#include "ND280UPAnalysisUtils.hxx"
#include <TVector3.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <typeinfo>
#include "DataClasses.hxx"
#include "DetectorDefinition.hxx"
#include "Parameters.hxx"
#include "AnaRecPackManager.hxx"
#include "CutUtils.hxx"

//ND280BeamBunching bunching;


//**************************************************
Int_t anaUtils::GetRunPeriod(Int_t run){
//**************************************************

    if      (             run<6000)   return 0;//run1  water                
    else if (run>=6462  && run<7664)  return 1;//run2  water                
    else if (run>=7665  && run<7754)  return 2;//run2  air                  
    else if (run>=8000  && run<8550)  return 3;//run3b air => mc associated to be the one of run2                  
    else if (run>=8550  && run<8800)  return 4;//run3c air                  
    else if (run>=8983  && run<9414)  return 5;//run4  water                
    else if (run>=9426  && run<9800)  return 6;//run4  air                  
    else if ((run>=10252 && run<10334)              //                    + 10252_10 to 10333
	     || (run>=10519 && run<10522)) return 7;//run5  water         + 10518_28 to 10521_13             
    else if (run>=10334 && run<10519) return 8;//run5  water (ANTINU) up to 10334_11 to 10518_9    
    else if (run>=10954 && run<11688) return 9;//run6  air (ANTINU)       

    else if (run>=90110000 && run<=90119999) return 0;//run1 water          
    else if (run>=90210000 && run<=90219999) return 1;//run2 water          
    else if (run>=90200000 && run<=90209999) return 2;//run2 air            
    else if (run>=90300000 && run<=90300015) return 3;//run3b air separated based on the pot of data (run3b/(run3b+run3c)=0.13542             
    else if (run>=90300016 && run<=90300110) return 4;//run3c air separated based on the pot of data            
    else if (run>=90410000 && run<=90419999) return 5;//run4 water          
    else if (run>=90400000 && run<=90409999) return 6;//run4 air            
    else if (run>=80510000 && run<=80519999) return 8;//run5 antinu-water
    else if (run>=80600000 && run<=80600159) return 9;//run6b antinu-air
    else if (run>=80600160 && run<=80600219) return 10;//run6c antinu-air - have to split Run 6 due to different flux tunings for the different parts
    else if (run>=80600220 && run<=80600299) return 11;//run6d antinu-air
    else if (run>=80600300 && run<=80600399) return 12;//run6e antinu-air
    else if (run>=80600400 && run<=80609999) return 12;//run6e antinu-air - set this as default, to catch any files that are not currently at TRIUMF

    else if (run>=80307000 && run<=80307999) return 8;//sand muons antinu
    else if (run>=90307000 && run<=90307999) return 4;//sand muons neut
    else if (run>=92307000 && run<=92307999) return 4;//sand muons old neut

    // genie
    else if (run>=91110000 && run<=91119999) return 0;//run1 water   
    else if (run>=91210000 && run<=91219999) return 1;//run2 water          
    else if (run>=91200000 && run<=91209999) return 2;//run2 air            
    else if (run>=91300000 && run<=91300015) return 3;//run3 air separated based on the pot of data (run3b/(run3b+run3c)=0.13542             
    else if (run>=91300016 && run<=91300110) return 4;//run3 air separated based on the pot of data (run3b/(run3b+run3c)=0.13542            
    else if (run>=91410000 && run<=91419999) return 5;//run4 water          
    else if (run>=91400000 && run<=91409999) return 6;//run4 air            
    else if (run>=81510000 && run<=81519999) return 8;//run5 antinu-water
    else if (run>=81600000 && run<=81600159) return 9;//run6b antinu-air
    else if (run>=81600160 && run<=81600219) return 10;//run6c antinu-air - have to split Run 6 due to different flux tunings for the different parts
    else if (run>=81600220 && run<=81600299) return 11;//run6d antinu-air
    else if (run>=81600300 && run<=81600399) return 12;//run6e antinu-air
    else if (run>=81600400 && run<=81609999) return 12;//run6e antinu-air - set this as default, to catch any files that are not currently at TRIUMF
    else return -1;

}


//**************************************************
int anaUtils::GetSandMode(int run){
//**************************************************

  if      (run>=80307000 && run<=80307999) return 0;	// antineutrino flux
  else if (run>=90307000 && run<=90307999) return 1;	// neutrino flux
  else if (run>=92307000 && run<=92307999) return 2;	// neutrino flux, old NEUT

  else return -1;
}

//********************************************************************
void anaUtils::IncrementPOTBySpill(const AnaSpillB& spill, Header& header) {
//********************************************************************

  const AnaBeamB& beam = *spill.Beam;
  const AnaEventInfoB& info = *spill.EventInfo;

  if (beam.POTSincePreviousSavedSpill > 1e+16) {
    std::cout << "WARNING: POT is suspiciously large for run " << info.Run << ", subrun " << info.SubRun << ", event " << info.Event << ": " << beam.POTSincePreviousSavedSpill << ". POT will not be counted for this event." << std::endl;
    return;
  }
  
  if (beam.POTSincePreviousSavedSpill < 0) {
    std::cout << "WARNING: POT is negative for run " << info.Run << ", subrun " << info.SubRun << ", event " << info.Event << ": " << beam.POTSincePreviousSavedSpill << ". POT will not be counted for this event." << std::endl;
    return;
  }

  // For real data
  if (!spill.GetIsMC()) {

    header._POT_NoCut   += beam.POTSincePreviousSavedSpill;
    header._Spill_NoCut += beam.SpillsSincePreviousSavedSpill;

    if (beam.GoodSpill == 0) {
      header._POT_BadBeam   += beam.POTSincePreviousSavedSpill;
      header._Spill_BadBeam += beam.SpillsSincePreviousSavedSpill;
      return;
    }


    if (beam.GoodSpill == 100) {
      header._POT_0KA += beam.POTSincePreviousSavedSpill;
    } else if (beam.GoodSpill == 1) {
      header._POT_250KA += beam.POTSincePreviousSavedSpill;
    } else if (beam.GoodSpill == 2) {
      header._POT_200KA += beam.POTSincePreviousSavedSpill;
    } else if (beam.GoodSpill == -1) {
      header._POT_m250KA += beam.POTSincePreviousSavedSpill;
    } else {
      header._POT_OtherKA += beam.POTSincePreviousSavedSpill;
    }

    header._POT_GoodBeamGoodND280   += beam.POTSincePreviousSavedSpill;
    header._Spill_GoodBeamGoodND280 += beam.SpillsSincePreviousSavedSpill;
  }
  else{
    // For MC there is no information about magnet current, Spill and DQ are always good
    header._Spill_NoCut             += beam.SpillsSincePreviousSavedSpill;
    header._Spill_GoodBeamGoodND280 += beam.SpillsSincePreviousSavedSpill;

    header._POT_NoCut             += beam.POTSincePreviousSavedSpill;
    header._POT_GoodBeamGoodND280 += beam.POTSincePreviousSavedSpill;
  }

}

//********************************************************************
Float_t anaUtils::ComputeInversePT( AnaDetCrossingB &cross, bool entrance) {
//********************************************************************

  Float_t p = entrance ? GetEntranceMomentum(cross): GetExitMomentum(cross);

  if (p <= 0) return -999; 
 
  Float_t ut = entrance ? sqrt(1-pow(cross.EntranceMomentum[0]/p,2)) :
    sqrt(1-pow(cross.ExitMomentum[0]/p,2));
  
  Float_t pt = ut * p;
  
  if (pt!= 0)
    return 1/pt;
  else
    return -999;
}

//********************************************************************
Float_t anaUtils::ComputeInversePT(AnaTPCParticleB& track){
//********************************************************************

  Float_t u_t = sqrt(1-pow(track.DirectionEnd[0],2));
  Float_t pt = u_t*(track.Momentum);
  if (pt!=0)
    return 1/pt;
  else
    return -999;
}


//***************************************************************
Float_t anaUtils::GetToF(const AnaTrackB* track, AnaParticleB*& seg1, AnaParticleB*& seg2, Float_t& sigma, bool UseSmearing){
//***************************************************************
  
  if (!track) return -999.;

  Float_t ToF_time_resolution = ND::params().GetParameterD("psycheNDUPUtils.ToF.TimeResolution");
  Int_t add_ToF_on_targets    = ND::params().GetParameterI("psycheNDUPUtils.Geometry.AddToFOnTargets");
  Int_t add_Fake_P0D          = ND::params().GetParameterI("psycheNDUPUtils.Geometry.AddFakeP0D");
  Int_t detector_config       = ND::params().GetParameterI("psycheNDUPUtils.Geometry.nd280upConfiguration");

  std::vector< std::pair<AnaParticleB*, std::vector<Float_t> > > detTiming;
  

  // find timing information in Targets
  for (int i=0; i<track->nTargetSegments; i++){
    if (track->TargetSegments[i]->IsReconstructed) {
      Float_t true_time = track->TargetSegments[i]->PositionStart[3];
      Float_t     sigma = sqrt(pow(1.5/sqrt(track->TargetSegments[i]->SegLength/15),2)+0.6*0.6); // 3ns/sqrt(NHits)
      Float_t      time = gRandom->Gaus(true_time, sigma);

      std::vector<Float_t> vec;
      vec.push_back(time); vec.push_back(sigma);
      detTiming.push_back(std::make_pair(track->TargetSegments[i], vec));
    }
  }

  
  if (detector_config==0 && add_Fake_P0D) {

    // find timing in the pseudo P0D by using a TPC 1 segment
    Float_t Z_min = 9999.;
    AnaTPCParticleB* mostFwdSeg = NULL;
    for (Int_t i = 0; i < track->nTPCSegments; i++) {
      AnaTPCParticleB* seg = track->TPCSegments[i];
      if (!seg)
	continue;
      if (SubDetId::GetSubdetectorEnum(seg->Detectors) != SubDetId::kForwTPC1) 
	continue;

      // minimum Z
      if (std::min(seg->PositionStart[2], seg->PositionEnd[2]) < Z_min) {
	Z_min = std::min(seg->PositionStart[2], seg->PositionEnd[2]);
	if (mostFwdSeg) delete mostFwdSeg;
	mostFwdSeg = (AnaTPCParticleB*)seg->Clone();
      }
    }
  
    // invert the segment as we use start position for ToF calculations
    if (mostFwdSeg && mostFwdSeg->PositionStart[2] > mostFwdSeg->PositionEnd[2]) 
      anaUtils::FlipTrack(mostFwdSeg);

    bool RecoP0DTrack = false;
    if (mostFwdSeg) {
 
      Float_t pos[3] = {mostFwdSeg->PositionStart[0], 
			mostFwdSeg->PositionStart[1], 
			mostFwdSeg->PositionStart[2]};
      Float_t mom = mostFwdSeg->Momentum;      
      Float_t eff;
      if (mom<150)	 eff = 0.021;
      else if (mom<200)	 eff = 0.311;
      else if (mom<220)	 eff = 0.900;
      else if (mom<260)	 eff = 0.944;
      else if (mom<300)	 eff = 0.991;
      else if (mom<500)	 eff = 0.989;
      else if (mom<700)	 eff = 0.942;
      else if (mom<1000) eff = 0.978;
      else if (mom<1400) eff = 0.991;
      else               eff = 0.997;

      Float_t r = gRandom->Rndm();
      if (r < eff && fabs(pos[0])<800 && fabs(pos[1]+10)<800 && pos[2]<-550)
	RecoP0DTrack = true;
    }

    if (RecoP0DTrack) {
      Float_t true_time = mostFwdSeg->PositionStart[3];
      Float_t     sigma = 1; // 1ns
      Float_t      time = gRandom->Gaus(true_time, sigma);

      std::vector<Float_t> vec;
      vec.push_back(time); vec.push_back(sigma);
      detTiming.push_back(std::make_pair(mostFwdSeg, vec));
    }  

  }
  

  // find timing information in FGDs
  for (int i=0; i<track->nFGDSegments; i++){
    if (track->FGDSegments[i]->IsReconstructed) {
      Float_t true_time = track->FGDSegments[i]->PositionStart[3];
      Float_t     sigma = sqrt(pow(3/sqrt(track->FGDSegments[i]->SegLength/25),2)+0.6*0.6); // 3ns/sqrt(NHits)
      Float_t      time = gRandom->Gaus(true_time, sigma);

      std::vector<Float_t> vec;
      vec.push_back(time); vec.push_back(sigma);
      detTiming.push_back(std::make_pair(track->FGDSegments[i], vec));
    }
  }

  // find timing information in ECal
  for (int i=0; i<track->nECalSegments; i++) {
    if (track->ECalSegments[i]->IsReconstructedForTiming) {      
      Float_t true_time = track->ECalSegments[i]->PositionStart[3];
      Float_t     sigma = 1; // 1ns
      Float_t      time = gRandom->Gaus(true_time, sigma);

      std::vector<Float_t> vec;
      vec.push_back(time); vec.push_back(sigma);
      detTiming.push_back(std::make_pair(track->ECalSegments[i], vec));
    }
  }

  // find timing information in ToF counters
  for (int i=0; i<track->nToFSegments; i++) {
    Float_t true_time = track->ToFSegments[i]->PositionStart[3];
    Float_t     sigma = ToF_time_resolution; // 0.6ns
    Float_t      time = gRandom->Gaus(true_time, sigma);

    std::vector<Float_t> vec;
    vec.push_back(time); vec.push_back(sigma);
    detTiming.push_back(std::make_pair(track->ToFSegments[i], vec));
  }
  
  // if we don't have 2, give up
  if (detTiming.size() < 2)
    return -999.;


  Float_t ToF = -999., max_t_over_s = 0.;
  for (unsigned int i=0; i<detTiming.size(); i++){
    for (unsigned int j=i+1; j<detTiming.size(); j++){

      Float_t  t1 = detTiming[i].second[0],  t2 = detTiming[j].second[0];
      Float_t  s1 = detTiming[i].second[1],  s2 = detTiming[j].second[1];
      Float_t t_over_s = fabs( (t2-t1)/sqrt(s1*s1+s2*s2) );

      if ( t_over_s > max_t_over_s ){
        max_t_over_s = t_over_s;
        ToF = t2-t1;
        seg1 = detTiming[i].first;
        seg2 = detTiming[j].first;
        sigma = sqrt(s1*s1+s2*s2);
      }
      
    } // end second loop on timing info
  } // end first loop on timing info

  
  if (!seg1 || !seg2)
    return -999.;

  // flip so that seg1 true time is always before seg2
  if (seg2->PositionStart[3]-seg1->PositionStart[3] < 0) {
    AnaParticleB* temp;
    temp = seg1;
    seg1 = seg2;
    seg2 = temp;
    ToF  = -ToF;
  }

  if (UseSmearing)
    return ToF;

  return seg2->PositionStart[3]-seg1->PositionStart[3];

}

//*********************************************************************
Float_t anaUtils::GetLength(AnaTrackB* track, AnaParticleB*& p1, AnaParticleB*& p2, Float_t& sigma_length, bool UseSmearing) {
//*********************************************************************
  Double_t length1 = -999.0, length2 = -999.0;
  Float_t length = -999.;
  RP::State state1, state2;
  TVector3 pos  = anaUtils::ArrayToTVector3(track->PositionStart);
  TVector3 pos1 = anaUtils::ArrayToTVector3(p1->PositionStart);
  TVector3 pos2 = anaUtils::ArrayToTVector3(p2->PositionStart);
  TVector3 dir1 = anaUtils::ArrayToTVector3(p1->DirectionStart);
  TVector3 dir2 = anaUtils::ArrayToTVector3(p2->DirectionStart);   
 
  if (!ND::tman().AnaTrueParticleB_to_RPState(*(track->GetTrueParticle()), state1) || !ND::tman().PropagateToSurface(pos1, dir1, state1, length1, false) ){
    if ((pos-pos1).Mag()<2) length1=0;
  }
  if (!ND::tman().AnaTrueParticleB_to_RPState(*(track->GetTrueParticle()), state2) || !ND::tman().PropagateToSurface(pos2, dir2, state2, length2, false) ){
    if ((pos-pos2).Mag()<2) length2=0;
  }

  if (length1>-1 && length2>-1) {
    Float_t sigma_length = 10;
    if (cutUtils::DeltaLYZTPCCut(*(track))) sigma_length = 1;

    Float_t true_length = abs(length2-length1);

    if (UseSmearing) length  = gRandom->Gaus(true_length, sigma_length); 
    else             length  = true_length;
  }    

  return length;
}

//*********************************************************************
Float_t anaUtils::ComputeToFMass(float mom, float ToF, float length){
//*********************************************************************

  Float_t c = 299.792458; // mm/ns

  // return 0 in case \beta > 1
  if (pow(c*ToF/length, 2) -1 < 0)
    return 0.;

  return mom*sqrt(pow(c*ToF/length, 2) -1);
}

//*********************************************************************
Float_t anaUtils::ComputeToFTime(Float_t mom, Float_t mass, Float_t length){
//*********************************************************************

  Float_t c = 299.792458; // mm/ns
  return length/c*sqrt(pow(mass/mom, 2) +1);
}

//*********************************************************************
bool anaUtils::GetToFdetectors(AnaParticleB*& seg1, AnaParticleB*& seg2, SubDetId::SubDetEnum& det1, SubDetId::SubDetEnum& det2) {
//*********************************************************************
  det1 = det2 = SubDetId::kInvalid;

  if (!seg1 || !seg2)
    return false;

  std::vector<std::pair<AnaParticleB*, SubDetId::SubDetEnum> > DetSegments;
  DetSegments.push_back(std::make_pair(seg1, det1));
  DetSegments.push_back(std::make_pair(seg2, det2));  

  for (UInt_t i = 0; i < DetSegments.size(); ++i) {
    if (SubDetId::GetDetectorUsed(DetSegments[i].first->Detectors, SubDetId::kTarget1))
      DetSegments[i].second = SubDetId::kTarget1;

    if (SubDetId::GetDetectorUsed(DetSegments[i].first->Detectors, SubDetId::kTarget2))
      DetSegments[i].second = SubDetId::kTarget2;

    if (SubDetId::GetDetectorUsed(DetSegments[i].first->Detectors, SubDetId::kFGD1))
      DetSegments[i].second = SubDetId::kFGD1;

    if (SubDetId::GetDetectorUsed(DetSegments[i].first->Detectors, SubDetId::kFGD2))
      DetSegments[i].second = SubDetId::kFGD2;

    if (SubDetId::GetDetectorUsed(DetSegments[i].first->Detectors, SubDetId::kBrlECal))
      DetSegments[i].second = SubDetId::kBrlECal;

    if (SubDetId::GetDetectorUsed(DetSegments[i].first->Detectors, SubDetId::kDsECal))
      DetSegments[i].second = SubDetId::kDsECal;

    if (SubDetId::GetDetectorUsed(DetSegments[i].first->Detectors, SubDetId::kToF))
      DetSegments[i].second = SubDetId::kToF;

    // if we use fake ToF over targets we will have segments from TPC
    if (SubDetId::GetDetectorUsed(DetSegments[i].first->Detectors, SubDetId::kTPC))
      DetSegments[i].second = SubDetId::kTPC;
  }

  det1 = DetSegments[0].second;
  det2 = DetSegments[1].second;

  return true;
}

//*********************************************************************
bool anaUtils::GetToFdetectors(AnaTrackB* track, SubDetId::SubDetEnum& det1, SubDetId::SubDetEnum& det2, bool UseSmearing) {
//*********************************************************************
  if (!track)
    return false;

  AnaParticleB *seg1=0, *seg2=0;
  Float_t sigma=0;
  Float_t ToF = GetToF(track, seg1, seg2, sigma, UseSmearing);
  if (ToF == -999.)
    return false;

  return (GetToFdetectors(seg1, seg2, det1, det2));
}

//*********************************************************************
bool anaUtils::GetToFLikelihood(AnaTrackB* track, Float_t* ToF_lkl, bool UseSmearing) {
//*********************************************************************
  if (!track)
    return false;

  // Get ToF pulls
  Float_t ToF_pull[4];
  ComputeToFpulls(track, ToF_pull, UseSmearing);

  return CalculateToFLikelihood(ToF_pull, ToF_lkl);
}

//*********************************************************************
bool anaUtils::GetToFLikelihood(Float_t mom, Float_t mom_err,  Float_t ToF, Float_t sigma, Float_t length, Float_t sigma_length, Float_t* ToF_lkl, bool UseSmearing) {
//*********************************************************************
  Float_t ToF_pull[4];
  // Get ToF pulls
  ComputeToFpulls(mom, mom_err, ToF, sigma, length, sigma_length, ToF_pull, UseSmearing);

  return CalculateToFLikelihood(ToF_pull, ToF_lkl);

}

//*********************************************************************
bool anaUtils::CalculateToFLikelihood(Float_t* ToF_pull, Float_t* ToF_lkl) {
//*********************************************************************
  ToF_lkl[0] = ToF_lkl[1] = ToF_lkl[2] = ToF_lkl[3] = -999.;

  if (ToF_pull[0] == -999. || ToF_pull[1] == -999. || ToF_pull[2] == -999. || ToF_pull[3] == -999.)
    return false;
  // compute the likelihood for each particle hypothesis
  // 0 - muon, 1 - electron, 2 - proton, 3 - pion
  Float_t P_mu = exp(-pow(ToF_pull[0],2)/2);
  Float_t P_pi = exp(-pow(ToF_pull[3],2)/2);
  Float_t P_e  = exp(-pow(ToF_pull[1],2)/2);
  Float_t P_p  = exp(-pow(ToF_pull[2],2)/2);

  Float_t sum_prob = P_mu+P_pi+P_e+P_p;

  ToF_lkl[0] = P_mu/sum_prob;
  ToF_lkl[3] = P_pi/sum_prob;
  ToF_lkl[1] = P_e/sum_prob;
  ToF_lkl[2] = P_p/sum_prob;

  return true;
}

//*********************************************************************
Float_t anaUtils::GetToFLikelihood(AnaTrackB* track, Int_t hypo, bool UseSmearing) {
//*********************************************************************
  if (!track)
    return -999.;

  Float_t ToF_lkl[4];
  GetToFLikelihood(track, ToF_lkl, UseSmearing);

  return ToF_lkl[hypo];
}

//*********************************************************************
Float_t anaUtils::GetToFLikelihood(Float_t mom, Float_t mom_err, Float_t ToF, Float_t sigma, Float_t length, Float_t sigma_length, Int_t hypo, bool UseSmearing) {
//*********************************************************************

  Float_t ToF_lkl[4];
  GetToFLikelihood(mom, mom_err, ToF, sigma, length, sigma_length, ToF_lkl, UseSmearing);

  return ToF_lkl[hypo];
}

//*********************************************************************
bool anaUtils::ComputeToFpulls(AnaTrackB* track, Float_t* ToF_pull, bool UseSmearing) {
//*********************************************************************
  if (!track)
    return false;

  AnaParticleB *p1=0, *p2=0;
  Float_t sigma=0;

  Float_t ToF     = GetToF(track, p1, p2, sigma, UseSmearing);
  if (!p1 || !p2)    return false;
  if (ToF == -999.0) return false;
  Float_t sigma_length;
  Float_t length  = GetLength(track, p1, p2, sigma_length, UseSmearing);
  if (length == -999.) return false;

  Float_t true_mom = track->GetTrueParticle()->Momentum;
  Float_t mom = track->SmearedMomentum;
  if (fabs(true_mom-mom)<1e-3)
    mom = gRandom->Gaus(true_mom, 0.10*true_mom);

  Float_t mom_err = track->MomentumError;

  return ComputeToFpulls(mom, mom_err, ToF, sigma, length, sigma_length, ToF_pull, UseSmearing);
}

//*********************************************************************
bool anaUtils::ComputeToFpulls(Float_t mom, Float_t mom_err, Float_t ToF, Float_t sigma, Float_t length, Float_t sigma_length, Float_t* ToF_pull, bool UseSmearing) {
//*********************************************************************

  ToF_pull[0] = ToF_pull[1] = ToF_pull[2] = ToF_pull[3] = -999.;
  
  Float_t ToFmass = ComputeToFMass(mom, ToF, length);

  if (ToFmass == 0)
    return false;

  Float_t invp = 1/mom;
  if (mom_err==0) mom_err=0.10*invp;

  // expected mass
  Float_t m = 0;
  // factor which is defined here to clarify the code
  Float_t a = 1.;
  a = 1+pow(mom/ToFmass, 2);
  // resolution on reconstructed mass
  Float_t sigma_m = ToFmass*sqrt(pow(mom_err/invp,2)+pow(a*sigma/ToF,2)+pow(a*sigma_length/length,2));

  // compute the pull for each particle hypothesis
  m = units::pdgBase->GetParticle(13)->Mass()*1000; // muon
  ToF_pull[0] = (ToFmass-m)/sigma_m;
  m = units::pdgBase->GetParticle(211)->Mass()*1000; // pion
  ToF_pull[3] = (ToFmass-m)/sigma_m;
  m = units::pdgBase->GetParticle(11)->Mass()*1000; // electron
  ToF_pull[1] = (ToFmass-m)/sigma_m;
  m = units::pdgBase->GetParticle(2212)->Mass()*1000; // proton
  ToF_pull[2] = (ToFmass-m)/sigma_m;

}


void anaUtils::FlipTrack(AnaTrack* track){

  Float_t temp_pos[4];
  anaUtils::CopyArray(track->PositionStart, temp_pos,             4); //temp_pos = pos_start
  anaUtils::CopyArray(track->PositionEnd,   track->PositionStart, 4); //pos_start = pos_end
  anaUtils::CopyArray(temp_pos,             track->PositionEnd,   4); //pos_end = temp_pos

  Float_t temp_dir[3];
  anaUtils::CopyArray(track->DirectionStart, temp_dir,              3); //temp_dir = dir_start
  anaUtils::CopyArray(track->DirectionEnd,   track->DirectionStart, 3); //dir_start = dir_end
  anaUtils::CopyArray(temp_dir,              track->DirectionEnd,   3); //dir_end = temp_dir

  track->Charge = -track->Charge;

  for (int i=0; i<track->nTargetSegments; i++)
    FlipTrack(track->TargetSegments[i]);
  for (int i=0; i<track->nFGDSegments; i++)
    FlipTrack(track->FGDSegments[i]);
  for (int i=0; i<track->nToFSegments; i++)
    FlipTrack(track->ToFSegments[i]);
  for (int i=0; i<track->nTPCSegments; i++)
    FlipTrack(track->TPCSegments[i]);
  for (int i=0; i<track->nECalSegments; i++)
    FlipTrack(track->ECalSegments[i]);

  track->HasFlipped = true;

}


void anaUtils::FlipTrack(AnaParticleB* track){

  Float_t temp_pos[4];
  anaUtils::CopyArray(track->PositionStart, temp_pos,             4); //temp_pos = pos_start
  anaUtils::CopyArray(track->PositionEnd,   track->PositionStart, 4); //pos_start = pos_end
  anaUtils::CopyArray(temp_pos,             track->PositionEnd,   4); //pos_end = temp_pos

  Float_t temp_dir[3];
  anaUtils::CopyArray(track->DirectionStart, temp_dir,              3); //temp_dir = dir_start
  anaUtils::CopyArray(track->DirectionEnd,   track->DirectionStart, 3); //dir_start = dir_end
  anaUtils::CopyArray(temp_dir,              track->DirectionEnd,   3); //dir_end = temp_dir
  
}
