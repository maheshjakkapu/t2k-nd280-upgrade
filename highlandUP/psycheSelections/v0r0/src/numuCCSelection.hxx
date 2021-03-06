#ifndef numuCCSelection_h
#define numuCCSelection_h

#include "SelectionBase.hxx"
#include "Parameters.hxx"
#include "ToyBoxNDUP.hxx"
#include "EventBoxId.hxx"
#include "SystId.hxx"
#include "SubDetId.hxx"

class numuCCSelection: public SelectionBase{
 public:
  numuCCSelection(bool forceBreak=true);
  virtual ~numuCCSelection(){}

  //---- These are mandatory functions
  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox(){return new ToyBoxNDUP();}
  SampleId::SampleEnum GetSampleEnum(){return SampleId::kTarget1NuMuCC;}
  bool IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantRecObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaRecObjectC*, SystId_h systId, Int_t branch=0) const;
  bool IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaTrueObjectC*, SystId_h systId, Int_t branch=0) const;
  bool IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const;
  void InitializeEvent(AnaEventC& event);
  bool CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep);

protected:

  Int_t _MuonPIDCutIndex;
  Int_t _MuonPIDStepIndex;
  Int_t _FindLeadingTracksStepIndex;
  Int_t _TotalMultiplicityCutIndex;


};

class FillSummaryAction_duneExample: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;  
  StepBase* MakeClone(){return new FillSummaryAction_duneExample();}
};
namespace numuCCUtils{
class TrackQualityFiducialCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TrackQualityFiducialCut();}
};


class MuonPIDCut: public StepBase{
 public:
  MuonPIDCut(){
      _prod5Cut = 0;
  }
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new MuonPIDCut();}
 private:
  bool _prod5Cut;
};

class DeltaLYZTarget1Cut: public StepBase{
 public:
          DeltaLYZTarget1Cut(){
            targetLYZCut = ND::params().GetParameterD("ndupAnalysis.targetLYZCut");
        }

        double targetLYZCut;

  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new DeltaLYZTarget1Cut();}
};

class FiducialCut: public StepBase{
 public:
  FiducialCut(SubDetId::SubDetEnum det){_detector=det;}
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FiducialCut(_detector);}
protected:
  SubDetId::SubDetEnum _detector;

};

class TotalMultiplicityCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TotalMultiplicityCut();}
};


class TrackQualityCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TrackQualityCut();}
};

/// Leading tracks with good quality in Subdet1_1
class FindLeadingTracksAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindLeadingTracksAction();}
};  /// Find the Vertex. For the moment it's just the Star position of the HM track
class FindVertexAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindVertexAction();}
};


  /// Find the Vertex. For the moment it's just the Star position of the longest track
class VertexInTarget1Cut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new VertexInTarget1Cut();}
};
  /// Find the Vertex. For the moment it's just the Star position of the longest track
class VertexInTarget2Cut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new VertexInTarget2Cut();}
};

  /// Find the Vertex. For the moment it's just the Star position of the longest track
class TrueVertexInTarget1Cut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TrueVertexInTarget1Cut();}
};

  /// Find the True Vertex. For the moment it's just start position of the true particle associated to the longest track
class FindTrueVertexAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindTrueVertexAction();}
};

  /// Find the True Vertex. For the moment it's just start position of the true particle associated to the longest track
class TrueNoPionCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TrueNoPionCut();}
};

class TrueOnePionCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TrueOnePionCut();}
};
  
class TrueOtherCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TrueOtherCut();}
};
  /// Find the True Vertex. For the moment it's just start position of the true particle associated to the longest track
class PionCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new PionCut();}
};
class ProtonCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new ProtonCut();}
};
class MuonCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new MuonCut();}
};
class ElectronCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new ElectronCut();}
};
}
  /// Find the True Vertex. For the moment it's just start position of the true particle associated to the longest track
//class ChargCut: public StepBase{
// public:
 // using StepBase::Apply;
 // bool Apply(AnaEventC& event, ToyBoxB& box) const;
 // StepBase* MakeClone(){return new ChargCut();}
//};

#endif
