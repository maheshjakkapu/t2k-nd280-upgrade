#ifndef nueCCSelection_h
#define nueCCSelection_h

#include "SelectionBase.hxx"
#include "SubDetId.hxx"
#include "ToyBoxNDUP.hxx"
#include "EventBoxId.hxx"
#include "SystId.hxx"
#include "SubDetId.hxx"
#include "Parameters.hxx"
#include "BinnedParams.hxx"

#include <TRandom3.h>
#include <TFile.h>
#include <TSpline.h>

class nueCCSelection: public SelectionBase{
public:
  nueCCSelection(bool forceBreak=false);
  virtual ~nueCCSelection(){
    _file_ECAL_PDF->Close();

    delete _file_ECAL_PDF;
  }

  //---- These are mandatory functions
  void DefineSteps();
  void DefineDetectorFV();
  inline ToyBoxB* MakeToyBox();
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  SampleId::SampleEnum GetSampleEnum(){return SampleId::kTarget1NuECC;}
  bool IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantRecObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaRecObjectC*, SystId_h systId, Int_t branch=0) const;
  bool IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaTrueObjectC*, SystId_h systId, Int_t branch=0) const;
  bool IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const;
  void InitializeEvent(AnaEventC& event);
  bool CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep);

private:
  TFile *_file_ECAL_PDF;  
};

class ToyBoxCC4pi: public ToyBoxNDUP{

public:

  ToyBoxCC4pi() {
    track_ECal_MipEM = -0xABCDEF;
    track_ECal_EneOnL = -0xABCDEF;
    MainTrack = NULL;
    PairTrack = NULL;
    TPCVetoTrack = NULL;
    ECalVetoTrack = NULL;
    TPCTracks.clear(); 
    ECalTracks.clear();

    TPC_det = SubDetId::kInvalid;
    ToF_mass = ToF_true_mass = InvariantMass = -999.0;
    NFarTracks = NNearTracks = 0;
    ToF_lkl_muon = ToF_lkl_pion = ToF_lkl_electron = ToF_lkl_proton = -999.0;
    ToF_pull_muon = ToF_pull_pion = ToF_pull_electron = ToF_pull_proton = -999.0;

    daughterPDG = -999;
    beta_ToF = beta_true_ToF = -999.;
    FirstToF = SecondToF = SubDetId::kInvalid;

    nIsoTargetTracks = 0;
  }

  virtual void Reset(){
    ResetBase();
    track_ECal_MipEM = -0xABCDEF;
    track_ECal_EneOnL = -0xABCDEF;
    MainTrack = NULL;
    PairTrack = NULL;
    TPCVetoTrack = NULL;
    ECalVetoTrack = NULL;
    TPCTracks.clear(); 
    ECalTracks.clear();
    
    TPC_det = SubDetId::kInvalid;
    ToF_mass = ToF_true_mass = InvariantMass = -999.;
    main_ToF = main_ToF_true = -999.;
    NFarTracks = NNearTracks = 0;
    ToF_lkl_muon = ToF_lkl_pion = ToF_lkl_electron = ToF_lkl_proton = -999.0;
    ToF_pull_muon = ToF_pull_pion = ToF_pull_electron = ToF_pull_proton = -999.0;

    daughterPDG = -999;
    beta_ToF = beta_true_ToF = -999.;
    FirstToF = SecondToF = SubDetId::kInvalid;

    EleTargetLength = PiTargetLength = MuTargetLength = PrTargetLength = -999.;

    nIsoTargetTracks = 0;
  }

  virtual ~ToyBoxCC4pi(){}

  /// For storing tracks information in the bunch
  float track_ECal_MipEM, track_ECal_EneOnL;
  AnaTrackB* MainTrack;
  std::vector<AnaTrackB*> TPCTracks, ECalTracks;
  SubDetId::SubDetEnum TPC_det;

  /// Energy loss in target
  Float_t EleTargetLength;
  Float_t PiTargetLength;
  Float_t MuTargetLength;
  Float_t PrTargetLength;

  /// ToF vars
  float main_ToF, main_ToF_true;
  float ToF_mass, ToF_true_mass;
  float ToF_lkl_muon, ToF_lkl_pion, ToF_lkl_electron, ToF_lkl_proton;
  float ToF_pull_muon, ToF_pull_pion, ToF_pull_electron, ToF_pull_proton;
  float beta_ToF, beta_true_ToF;

  /// detectors that were used for the ToF calculations
  SubDetId::SubDetEnum FirstToF, SecondToF;

  Int_t daughterPDG;  

  float InvariantMass;
  int NFarTracks, NNearTracks;

  /// The particle that isn't HM track selected that forms the e+e- pair with the lowest invariant mass
  AnaTrackB* PairTrack;

  /// TPC veto track
  AnaTrackB* TPCVetoTrack;

  /// Ecal veto track
  AnaECalParticleB* ECalVetoTrack;

  AnaTrackB** IsoTargetTracks;
  Int_t       nIsoTargetTracks;
  
};

inline ToyBoxB* nueCCSelection::MakeToyBox() {return new ToyBoxCC4pi();}

class TotalMultiplicityCut: public StepBase{
public:
  TotalMultiplicityCut() {
    useTarget1 = ND::params().GetParameterI("nueCCAnalysis.EnableTarget1");
    useTarget2 = ND::params().GetParameterI("nueCCAnalysis.EnableTarget2");
    useFGD1 = ND::params().GetParameterI("nueCCAnalysis.EnableFGD1");
    useFGD2 = ND::params().GetParameterI("nueCCAnalysis.EnableFGD2");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TotalMultiplicityCut();}
private:
  bool useTarget1, useTarget2;
  bool useFGD1, useFGD2;
};

class SortTracksAction: public StepBase{
public:
  SortTracksAction() {
    useTarget1 = ND::params().GetParameterI("nueCCAnalysis.EnableTarget1");
    useTarget2 = ND::params().GetParameterI("nueCCAnalysis.EnableTarget2");
    useFGD1 = ND::params().GetParameterI("nueCCAnalysis.EnableFGD1");
    useFGD2 = ND::params().GetParameterI("nueCCAnalysis.EnableFGD2");
    selection_charge = (Int_t)ND::params().GetParameterI("nueCCAnalysis.SelectCharge");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new SortTracksAction();}
private:
  bool useTarget1, useTarget2;
  bool useFGD1, useFGD2;
  Int_t selection_charge;
};

class TrackGQandFVCut: public StepBase{
public:
  TrackGQandFVCut() {
    useTarget1 = ND::params().GetParameterI("nueCCAnalysis.EnableTarget1");
    useTarget2 = ND::params().GetParameterI("nueCCAnalysis.EnableTarget2");
    useFGD1 = ND::params().GetParameterI("nueCCAnalysis.EnableFGD1");
    useFGD2 = ND::params().GetParameterI("nueCCAnalysis.EnableFGD2");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TrackGQandFVCut();}
  
private:
  bool useTarget1, useTarget2;
  bool useFGD1, useFGD2;
};

/// Find the True Vertex. For the moment it's just start position of the true particle associated to the longest track
class FindTrueVertexAction: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindTrueVertexAction();}
};

class FindVertexAction: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindVertexAction();}
};

class FillSummaryAction_nueCC: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;  
  StepBase* MakeClone(){return new FillSummaryAction_nueCC();}
};

class TPC_Quality: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TPC_Quality();}
};

class TPCElectronPID: public StepBase{
public:
  TPCElectronPID() {
    lik_elec = (Float_t)ND::params().GetParameterD("nueCCAnalysis.TPCPID.Cut.Lelec");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TPCElectronPID();}
  
private:
  Float_t lik_elec;
};

class TPCMuonPID: public StepBase{
public:
  TPCMuonPID() {
    lik_muon = (Float_t)ND::params().GetParameterD("nueCCAnalysis.TPCPID.Cut.Lmu");
    lik_mom  = (Float_t)ND::params().GetParameterD("nueCCAnalysis.TPCPID.Cut.Momentum");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TPCMuonPID();}
  
private:
  Float_t lik_muon;
  Float_t lik_mom;
};

class TPCVeto: public StepBase{
public:
  TPCVeto() {
    DeltaZ = (Float_t)ND::params().GetParameterD("nueCCAnalysis.TPCVetoDeltaZ");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TPCVeto();}
  
private:
  Float_t DeltaZ;
};

class ECalVeto: public StepBase{
public:
  ECalVeto() {
    DeltaZ = (Float_t)ND::params().GetParameterD("nueCCAnalysis.ECalVetoDeltaZ");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new ECalVeto();}
  
private:
  Float_t DeltaZ;
};

class ECal_PID: public StepBase{
public:
  ECal_PID(TFile *f){
    _file_ECAL_PDF=f;
    _useECal = (bool)ND::params().GetParameterD("nueCCAnalysis.ECal.UseP0DECal");
  }
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new ECal_PID(_file_ECAL_PDF);}
private:
  TFile* _file_ECAL_PDF; 
  bool _useECal;
};

class ToF_senseDetermination: public StepBase{
public:
  ToF_senseDetermination(){
    _randomGen = new TRandom3(0);
  }
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new ToF_senseDetermination();}
private:
  TRandom3* _randomGen;
};

class FindPairsAction: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new FindPairsAction();}
};

class PairCut: public StepBase{
public:
  PairCut(){
    _inv_mass_min = (Float_t)ND::params().GetParameterD("nueCCAnalysis.InvMassMin");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new PairCut();}
protected:
  Float_t _inv_mass_min;
};

class FindIsoTargetElectrons: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new FindIsoTargetElectrons();}
};

class LengthPIDcut: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new LengthPIDcut();}
};

#endif
