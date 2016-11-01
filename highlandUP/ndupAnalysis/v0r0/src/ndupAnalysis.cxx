#include "ndupAnalysis.hxx"
#include "FiducialVolumeDefinition.hxx"
#include "Parameters.hxx"
#include "CategoriesUtils.hxx"
#include "BasicUtils.hxx"
#include "baseSelection.hxx"
#include "ndupSelection.hxx"
#include "TreeConverterUtils.hxx"
#include "CutUtils.hxx"
#include "MomRangeCorrection.hxx"
#include "TruthUtils.hxx"
#include "PIDCorrection.hxx"
#include "PIDUtils.hxx"
//********************************************************************
ndupAnalysis::ndupAnalysis(AnalysisAlgorithm* ana) : baseAnalysis(ana) {
//********************************************************************

  // Add the package version
  ND::versioning().AddPackage("ndupAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("NDUPANALYSISROOT")));
}

//********************************************************************
bool ndupAnalysis::Initialize() {
//********************************************************************

  // Initialize the baseAnalysis
  if (!baseAnalysis::Initialize()) return false;

  // Minimum accum level to save event into the output tree
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("ndupAnalysis.MinAccumLevelToSave"));

  // Define categories
  anaUtils::AddStandardCategories();

  _detector = SubDetId::kTPCUp1;

  return true;
}

//********************************************************************
void ndupAnalysis::DefineSelections() {
//********************************************************************

  //sel().AddSelection("ndup",    "inclusive ndup selection",     new baseSelection());     // true/false for forcing break
  sel().AddSelection("ndup",    "inclusive ndup selection",     new ndupSelection());     // true/false for forcing break

}

//********************************************************************
void ndupAnalysis::DefineCorrections() {
//********************************************************************

  // Some corrections are defined in baseAnalysis
  baseAnalysis::DefineCorrections();
  corr().AddCorrection("mom_corr",          new MomRangeCorrection(SubDetId::kTPC));
  corr().AddCorrection("pid_corr",          new PIDCorrection(SubDetId::kTPC));

}

//********************************************************************
void ndupAnalysis::DefineSystematics() {
//********************************************************************

  // Some corrections are defined in baseAnalysis
  baseAnalysis::DefineSystematics();


//  eweight().AddEventWeight(SystId::kDetectorMass,           "DetectorMass",           new DetectorMassWeight());

//  evar().AddEventVariation(SystId::kdEdx,           "dEdx",           new dEdxVariation());
}

//********************************************************************
void ndupAnalysis::DefineConfigurations() {
//********************************************************************

  // Some corrections are defined in baseAnalysis
  baseAnalysis::DefineConfigurations();

}

//********************************************************************
void ndupAnalysis::DefineMicroTrees(bool addBase) {
//********************************************************************

  std::cout << "-------- Add variables to the analysis tree ----------------------" << std::endl;

  // Variables from baseAnalysis (run, event, ...)
  if (addBase) baseAnalysis::DefineMicroTrees(addBase);
  AddVarI(output(), Nu_pdg,         "nu pdg");
  AddVarF(output(), Nu_mom,         "nu momentum");
  AddVarI(output(), reaction_code,         "reaction_code");
  AddVarI(output(), current_code,         "current_code");

  AddVar4VF(output(), vertex_position,         "vertex position");

  AddVarI(output(), muon_pdg,         "muon pdg");
  AddVarF(output(), muon_SDLength,         "muon SD Length");
  AddVarF(output(), muon_mom,         "muon momentum");
  AddVarF(output(), muon_costheta,         "muon costheta");
  AddVarF(output(), muon_cosphi,         "muon cosphi");
  AddVarF(output(), muon_theta,         "muon theta");
  AddVarF(output(), muon_phi,         "muon phi");
  AddVarF(output(), muon_pidlikelihood1,         "muon_pidlikelihood1");
  AddVarF(output(), muon_pidlikelihood2,         "muon_pidlikelihood2");
  AddVarF(output(), muon_pidlikelihood3,         "muon_pidlikelihood3");
  AddVarF(output(), muon_pidlikelihood4,         "muon_pidlikelihood4");
 AddVarF(output(), mip_pidlikelihood,         "mip_pidlikelihood");

  AddVarF(output(), muon_charge,         "muon charge");
  AddVarF(output(), muon_ekin,         "muon kinetic energy");
  AddVarI(output(), muon_istpccut,         "muon is length cut in tpc");

  AddVarI(output(), muon_istarget1,         "muon is length cut in target1");
  AddVarI(output(), muon_istarget2,         "muon is length cut in target2");

  AddVarMF(output(), muon_tpc_entrancepos,    "muon entrance position in tpc", tpc_muon, -300, 4);

  AddVarMF(output(), muon_tpc_exitpos,    "muon exit position in tpc", tpc_muon, -300, 4);
  AddVarVI(output(), muon_tpc_det,         "muon tpc det id", tpc_muon);
  AddVarVF(output(), muon_LYZTPC,         "muon LYTPC", tpc_muon);
 
  AddVarVF(output(), muon_tpc_length,         "muon tpc segmentLength", tpc_muon);
  AddVarVF(output(), muon_tpc_EDep,         "muon tpc EDeposit", tpc_muon);
  AddVarVF(output(), muon_tpc_smeared_momentum,         "muon tpc smeared momentum", tpc_muon);
  AddVarVF(output(), muon_tpc_momentum_error,         "muon tpc smeared momentum error", tpc_muon);
  AddVarVF(output(), tpc_pull_muon,         "muon tpc pull ", tpc_muon);
  AddVarVF(output(), muon_tpc_momentum,         "muon tpc true momentum", tpc_muon);

  AddVarF(output(), muon_EDep_Target1,         "muon EDep in Target1");
  AddVarF(output(), muon_EDep_Target2,         "muon EDep in Target2");
  AddVarF(output(), muon_LTarget1,         "muon LTarget1");
  AddVarF(output(), muon_LTarget2,         "muon LTarget2");
  AddVarI(output(), muon_isLastinTarget1,         " is the muon track end postion in target1");
  AddVarI(output(), muon_isLastinTarget2,         "is the muon track end postion in target2");

  AddVar4VF(output(), muon_target1_entrancepos,         "muon target1 entrance position");
  AddVar4VF(output(), muon_target1_exitpos,         "muon target1 exit position");
  AddVar4VF(output(), muon_target2_entrancepos,         "muon target2 entrance position");
  AddVar4VF(output(), muon_target2_exitpos,         "muon target2 exit position");




  AddVarI(output(), ppion_pdg,         "pi+ pdg");
  AddVarF(output(), ppion_SDLength,         "pi+ SD Length");
  AddVarF(output(), ppion_mom,         "pi+ mom");
  AddVarF(output(), ppion_costheta,         "pi+ costheta");
  AddVarF(output(), ppion_cosphi,         "pi+ cosphi");
  AddVarF(output(), ppion_theta,         "pi+ theta");
  AddVarF(output(), ppion_phi,         "pi+ phi");
  AddVarI(output(), ppion_istpccut,         "pi+ is length cut in tpc");

  AddVarI(output(), ppion_istarget1,         "pi+ is length cut in target1");
  AddVarI(output(), ppion_istarget2,         "pi+ is length cut in target1");

  AddVarF(output(), ppion_charge,         "pi+ charge");
  AddVarF(output(), ppion_ekin,         "pi+ kinetic energy");

  AddVarMF(output(), ppion_tpc_entrancepos,    "pi+ tpc entrance position", tpc_ppion, -300, 4);
  AddVarVF(output(), tpc_pull_pion,         "pi+ tpc pull ", tpc_ppion);

  AddVarMF(output(), ppion_tpc_exitpos,    "pi+ tpc exit position", tpc_ppion, -300, 4);

  AddVarVI(output(), ppion_tpc_det,         "pi+ tpc det id", tpc_ppion);
  AddVarVF(output(), ppion_LYZTPC,         "pi+ LYZYPC ", tpc_ppion);
  AddVarVF(output(), ppion_tpc_smeared_momentum,         "pi+ tpc smeared momentum", tpc_ppion);
  AddVarVF(output(), ppion_tpc_momentum_error,         "pi+ tpc smeared momentum error", tpc_ppion);
  AddVarVF(output(), ppion_tpc_momentum,         "pi+ tpc true momentum", tpc_ppion);

  AddVarVF(output(), ppion_tpc_length,         "pi+ Length in TPC", tpc_ppion);
  AddVarVF(output(), ppion_tpc_EDep,         " pi+ TPC EDeposit", tpc_ppion);

  AddVarF(output(), ppion_EDep_Target1,         "pi+ EDep in Target1");
  AddVarF(output(), ppion_EDep_Target2,         "pi+ EDep in Target2");
  AddVarF(output(), ppion_LTarget1,         "pi+ Length in Target1");
  AddVarF(output(), ppion_LTarget2,         "pi+ Length in Target2");
  AddVarI(output(), ppion_isLastinTarget1,         "is the pi+ track end postion in target1");
  AddVarI(output(), ppion_isLastinTarget2,         "is the pi+ track end postion in target2");

  AddVar4VF(output(), ppion_target1_entrancepos,         "pi+ target1 entrance position");
  AddVar4VF(output(), ppion_target1_exitpos,         "pi+ target1 exit position");
  AddVar4VF(output(), ppion_target2_entrancepos,         "pi+ target2 entrance position");
  AddVar4VF(output(), ppion_target2_exitpos,         "pi+ target2 exit position");




  AddVarI(output(), mpion_pdg,         "pi- pdg");
  AddVarF(output(), mpion_SDLength,         "pi- SD Length");
  AddVarF(output(), mpion_mom,         "pi- mom");
  AddVarF(output(), mpion_costheta,         "pi- costheta");
  AddVarF(output(), mpion_cosphi,         "pi- cosphi");
  AddVarF(output(), mpion_theta,         "pi- theta");
  AddVarF(output(), mpion_phi,         "pi- phi");
  AddVarI(output(), mpion_istpccut,         "pi- is length cut in tpc");

  AddVarI(output(), mpion_istarget1,         "pi- is length cut in target1");
  AddVarI(output(), mpion_istarget2,         "pi- is length cut in target2");

  AddVarF(output(), mpion_charge,         "pi- charge");
  AddVarF(output(), mpion_ekin,         "pi- kinetic energy");

  AddVarMF(output(), mpion_tpc_entrancepos,    "pi- tpc entrance position", tpc_mpion, -300, 4);

  AddVarMF(output(), mpion_tpc_exitpos,    "pi- tpc exit position", tpc_mpion, -300, 4);
  AddVarVI(output(), mpion_tpc_det,         "pi- tpc det id", tpc_mpion);
  AddVarVF(output(), mpion_LYZTPC,         "pi- LYTPC", tpc_mpion);
  AddVarVF(output(), mpion_tpc_smeared_momentum,         "pi- tpc smeared momentum", tpc_mpion);
  AddVarVF(output(), mpion_tpc_momentum_error,         "pi- tpc smeared momentum error", tpc_mpion);
  AddVarVF(output(), mpion_tpc_momentum,         "pi-  tpc true momentum", tpc_mpion);

  AddVarVF(output(), mpion_tpc_length,         "pi-  Length in TPC", tpc_mpion);
  AddVarVF(output(), mpion_tpc_EDep,         "pi- TPC EDeposit", tpc_mpion);

  AddVarF(output(), mpion_EDep_Target1,         "pi- EDep in Target1");
  AddVarF(output(), mpion_EDep_Target2,         "pi- EDep in Target2");
  AddVarF(output(), mpion_LTarget1,         "pi- Length in Target1");
  AddVarF(output(), mpion_LTarget2,         "pi- Length in Target2");
  AddVarI(output(), mpion_isLastinTarget1,         "is the pi- track end postion in target1");
  AddVarI(output(), mpion_isLastinTarget2,         "is the pi- track end postion in target2");

  AddVar4VF(output(), mpion_target1_entrancepos,         "pi- target1 entrance position");
  AddVar4VF(output(), mpion_target1_exitpos,         "pi- target1 exit position");
  AddVar4VF(output(), mpion_target2_entrancepos,         "pi- target2 entrance position");
  AddVar4VF(output(), mpion_target2_exitpos,         "pi- target2 entrance position");



  AddVarI(output(), proton_pdg,         "proton pdg");
  AddVarF(output(), proton_SDLength,         "proton SD Length");
  AddVarF(output(), proton_mom,         "proton mom");
  AddVarF(output(), proton_costheta,         "proton costheta");
  AddVarF(output(), proton_cosphi,         "proton cosphi");
  AddVarF(output(), proton_theta,         "proton theta");
  AddVarF(output(), proton_phi,         "proton phi");
  AddVarI(output(), proton_istpccut,         "proton is length cut in tpc");

  AddVarI(output(), proton_istarget1,         "proton is length cut in target1");
  AddVarI(output(), proton_istarget2,         "proton is length cut in target2");

  AddVarF(output(), proton_charge,         "proton charge");
  AddVarF(output(), proton_ekin,         "proton kinetic energy");

  AddVarMF(output(), proton_tpc_entrancepos,    "proton tpc entrance position", tpc_proton, -300, 4);

  AddVarMF(output(), proton_tpc_exitpos,    "proton tpc exit position", tpc_proton, -300, 4);
  AddVarVI(output(), proton_tpc_det,         "proton tpc det id", tpc_proton);
  AddVarVF(output(), proton_LYZTPC,         "proton LYTPC", tpc_proton);
  AddVarVF(output(), tpc_pull_proton,         "proton tpc pull", tpc_proton);
  AddVarVF(output(), proton_tpc_momentum,         "proton tpc smeared momentum", tpc_proton);

  AddVarVF(output(), proton_tpc_smeared_momentum,         "proton tpc smeared momentum error", tpc_proton);
  AddVarVF(output(), proton_tpc_momentum_error,         "proton tpc true momentum", tpc_proton);

  AddVarVF(output(), proton_tpc_length,         "proton Length in TPC", tpc_proton);
  AddVarVF(output(), proton_tpc_EDep,         "proton TPC EDeposit", tpc_proton);

  AddVarF(output(), proton_EDep_Target1,         "proton EDep in Target1");
  AddVarF(output(), proton_EDep_Target2,         "proton EDep in Target2");
  AddVarF(output(), proton_LTarget1,         "proton Length in Target1");
  AddVarF(output(), proton_LTarget2,         "proton Length in Target2");
  AddVarI(output(), proton_isLastinTarget1,         "is the proton track end postion in target1");
  AddVarI(output(), proton_isLastinTarget2,         "is the proton track end postion in target2");

  AddVar4VF(output(), proton_target1_entrancepos,         "proton target1 entrance position");
  AddVar4VF(output(), proton_target1_exitpos,         "proton target1 exit position");
  AddVar4VF(output(), proton_target2_entrancepos,         "proton target2 entrance position");
  AddVar4VF(output(), proton_target2_exitpos,         "proton target2 exit position");


}

//********************************************************************
void ndupAnalysis::DefineTruthTree() {
//********************************************************************

  // Variables from baseAnalysis (run, event, ...)
  baseAnalysis::DefineTruthTree();
  AddVarI(output(), true_Nu_pdg,         "nu pdg");
  AddVarF(output(), true_Nu_mom,         "nu momentum");
  AddVarI(output(), true_reaction_code,         "reaction_code");
  AddVar4VF(output(), true_vertex_position,         "vertex position");

  AddVarI(output(), true_parentID,         "  true_parentID");                  

  AddVarI(output(), true_pdg,         "  pdg");
  AddVarF(output(), true_SDlength,  " SD length  ");
  AddVarF(output(), true_Edep,  " Edeposite  ");
  AddVarF(output(), true_mom,         " mom");
  AddVarF(output(), true_costheta,         " costheta");
  AddVarF(output(), true_phi,         " phi");
  AddVarF(output(), true_charge,         " charge");
  AddVarF(output(), true_ekin,         " kinetic energy");

}

//********************************************************************
void ndupAnalysis::FillMicroTrees(bool addBase) {
//********************************************************************

  // WARNING: IF YOU RUN FGD1 AND FGD2 AT ONCE ( "whichFGD" parameter = 3), only for events with accum_level > 5
  // the vars in the output microtree will surely refer to the muon candidate in that FGD

  // Variables from baseAnalysis (run, event, ...)
  if (addBase) baseAnalysis::FillMicroTreesBase(addBase);
  if (box().Vertex) {
    output().FillVar(Nu_pdg, (box().Vertex->TrueVertex)->NuPDG);
    AnaTrueVertex *vtx = dynamic_cast<AnaTrueVertex*>(box().Vertex->TrueVertex);
    output().FillVar(Nu_mom, (box().Vertex->TrueVertex)->NuMom);

    output().FillVar(reaction_code, vtx->ReacCode);
    output().FillVar(current_code, anaUtils::GetReacAll(vtx->ReacCode));
    output().FillVectorVarFromArray(vertex_position, (box().Vertex)->Position, 4);
  }
// Muon information
  if (box().MainTrack) {
    AnaTrueParticle *trueP = dynamic_cast<AnaTrueParticle*>(box().MainTrack->GetTrueParticle());
    output().FillVar(muon_pdg, trueP->PDG);
    output().FillVar(muon_charge, trueP->Charge);
    output().FillVar(muon_mom, box().MainTrack->SmearedMomentum);
    output().FillVar(muon_costheta, (box().MainTrack)->DirectionStart[2]);
    output().FillVar(muon_theta, (float)acos((box().MainTrack)->DirectionStart[2]));
    output().FillVar(muon_istpccut, cutUtils::DeltaLYZTPCCut(*box().MainTrack));
    output().FillVar(muon_istarget1, (int)cutUtils::DeltaLYZTargetCut(*box().MainTrack, SubDetId::kTarget1));
    output().FillVar(muon_istarget2, (int)cutUtils::DeltaLYZTargetCut(*box().MainTrack, SubDetId::kTarget2));


    float phi = atan2((box().MainTrack)->DirectionStart[1], (box().MainTrack)->DirectionStart[0]);
    float cosphi = cos(phi);

    output().FillVar(muon_cosphi, cosphi);
    output().FillVar(muon_phi, phi);

    AnaTrack *track = dynamic_cast<AnaTrack*>(box().MainTrack);
    Float_t PIDLikelihood[4];
    anaUtils::GetPIDLikelihood(*box().MainTrack, PIDLikelihood, 0);
    output().FillVar (mip_pidlikelihood, (PIDLikelihood[0] + PIDLikelihood[3]) / (1 - PIDLikelihood[2]));
    output().FillVar(muon_pidlikelihood1, PIDLikelihood[0]);
    output().FillVar(muon_pidlikelihood2, PIDLikelihood[1]);
    output().FillVar(muon_pidlikelihood3, PIDLikelihood[2]);
    output().FillVar(muon_pidlikelihood4, PIDLikelihood[3]);

    output().FillVar(muon_ekin, (box().MainTrack)->EKin);
    output().FillVar(muon_SDLength, track->Length);
    Float_t pulls[4];
    for (int subdet = 0; subdet < 7; subdet++) {
      AnaTPCParticleB* TPCSegment = dynamic_cast<AnaTPCParticleB*>(anaUtils::GetSegmentInDet( *box().MainTrack, static_cast<SubDetId::SubDetEnum >(subdet)));
      if (TPCSegment) {
        output().FillVectorVar(muon_tpc_det, subdet);
        output().FillVectorVar(muon_LYZTPC, TPCSegment->DeltaLYZ);
        output().FillVectorVar(muon_tpc_EDep, TPCSegment->EDeposit);
        output().FillVectorVar(muon_tpc_length, TPCSegment->SegLength);
        output().FillVectorVar(muon_tpc_smeared_momentum, TPCSegment->SmearedMomentum);
        output().FillVectorVar(muon_tpc_momentum, TPCSegment->Momentum);

        output().FillVectorVar(muon_tpc_momentum_error, TPCSegment->MomentumError);
        //  anaUtils::ComputeTPCPull(*TPCSegment, *box().HMT);

        anaUtils::ComputeTPCPull(*TPCSegment, pulls);
        output().FillVectorVar(tpc_pull_muon, pulls[0]);

        output().FillMatrixVarFromArray(muon_tpc_entrancepos,    TPCSegment->PositionStart, 4);
        output().FillMatrixVarFromArray(muon_tpc_exitpos,    TPCSegment->PositionEnd, 4);
        output().IncrementCounterForVar(muon_tpc_det);

      }

    }

    AnaTargetParticleB* TargetSegment1 = dynamic_cast<AnaTargetParticleB*>(anaUtils::GetSegmentInDet( *box().MainTrack, SubDetId::kTarget1));
    if (TargetSegment1) {
      output().FillVar(muon_LTarget1, TargetSegment1->DeltaLYZ);
      output().FillVar(muon_EDep_Target1, TargetSegment1->EDeposit);
      output().FillVectorVarFromArray(muon_target1_entrancepos,    TargetSegment1->PositionStart, 4);
      output().FillVectorVarFromArray(muon_target1_exitpos,    TargetSegment1->PositionEnd, 4);
      output().FillVar(muon_isLastinTarget1, (int)cutUtils::FiducialCut((box().MainTrack)->PositionEnd, SubDetId::kTarget1) );


    }
    AnaTargetParticleB* TargetSegment2 = dynamic_cast<AnaTargetParticleB*>(anaUtils::GetSegmentInDet( *box().MainTrack, SubDetId::kTarget2));
    if (TargetSegment2) {
      output().FillVar(muon_LTarget2, TargetSegment2->DeltaLYZ);
      output().FillVar(muon_EDep_Target2, TargetSegment2->EDeposit);
      output().FillVectorVarFromArray(muon_target2_entrancepos,    TargetSegment2->PositionStart, 4);
      output().FillVectorVarFromArray(muon_target2_exitpos,    TargetSegment2->PositionEnd, 4);
      output().FillVar(muon_isLastinTarget2, (int)cutUtils::FiducialCut((box().MainTrack)->PositionEnd, SubDetId::kTarget2) );

    }

  }


}

//********************************************************************
void ndupAnalysis::FillToyVarsInMicroTrees(bool addBase) {
//********************************************************************

  // Fill all tree variables that vary for each toy experiment

  // Fill the common variables
  if (addBase) baseAnalysis::FillToyVarsInMicroTreesBase(addBase);

}

//********************************************************************
bool ndupAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx) {
//********************************************************************
  // GetReactionCC already takes into account the outFV
  bool numuCCInFV = true; //(anaUtils::GetReactionCC(vtx, fgdID) == 1);
  return (numuCCInFV);
}

//********************************************************************
void ndupAnalysis::FillTruthTree(const AnaTrueVertex& vtx) {
//********************************************************************

  // workaround to use the same code for the antuNumu package
  // calling ndupAnalysis::FillTruthTreeBase(vtx,true)
  bool IsAntinu = false;
  std::vector<AnaTrueParticleB*> TrueParticles = vtx.TrueParticlesVect;
  output().FillVar(true_Nu_pdg, vtx.NuPDG);
  output().FillVar(true_Nu_mom, vtx.NuMom);

  output().FillVar(true_reaction_code, vtx.ReacCode);
  output().FillVectorVarFromArray(true_vertex_position, vtx.Position, 4);
  AnaTrueParticleB* trueTrack=NULL;
  for (Int_t i = 0; i < TrueParticles.size(); i++) {
    if (TrueParticles[i]->PDG == 13) {
      trueTrack = dynamic_cast<AnaTrueParticleB*>(TrueParticles[i]);
    }
  }
  if (trueTrack) {
    output().FillVar(true_parentID, trueTrack->ParentID);
    output().FillVar(true_pdg, trueTrack->PDG);
    output().FillVar(true_mom, trueTrack->Momentum);
    output().FillVar(true_Edep, trueTrack->EDeposit);
    output().FillVar(true_SDlength, trueTrack->Length);
    output().FillVar(true_costheta, trueTrack->CosTheta);
    output().FillVar(true_ekin, trueTrack->EKin);
    output().FillVar(true_charge, trueTrack->Charge);

    float phi = atan2(trueTrack->Direction[1], trueTrack->Direction[0]);
    float cosphi = cos(phi);

    output().FillVar(true_phi, phi);


  }
  return FillTruthTreeBase(vtx, IsAntinu);
}


//********************************************************************
void ndupAnalysis::FillTruthTreeBase(const AnaTrueVertex& vtx, bool IsAntinu) {
//********************************************************************

  // this method is also called from the antiNumu package with IsAntinu = true

  // Fill the common variables
  baseAnalysis::FillTruthTreeBase(vtx, _detector, IsAntinu);
  // Muon true-rec association.
  //  if (box().MainTrack)
  //    if (box().MainTrack->TrueObject)
  //      output().FillVar("mu_true_rec", (box().MainTrack->GetTrueParticle()->PDG==13));   // Is the recon muon a true muon ?

  // Fill truth categories for FGD2, same categories with prefix "fgd2", i,e, "fgd2reaction" etc.
  // We could directly set which FGD in the usual categories, but this is not possible for the true vertices
  //  anaUtils::FillCategories(&vtx, "fgd2", _detector, IsAntinu,GetSpill().GetIsSandMC());

}

//********************************************************************
void ndupAnalysis::FillCategories() {
//********************************************************************

  // Fill the track categories for color drawing

  // For the muon candidate
}