#ifndef ND280_CUBESCINT_CONSTRUCTOR_HH_SEEN
#define ND280_CUBESCINT_CONSTRUCTOR_HH_SEEN
// $Id: ND280CubeScintConstructor.hh, 2017/3/28  $
//

#include <G4ThreeVector.hh>

class G4LogicalVolume;

#include "ND280Constructor.hh"

// NEW ND280UPGRADE                     
#include "ND280RootPersistencyManager.hh" // XML input file  
#include "ExN02ND280XML.hh" // XML input file
//   


///
/// This class is taken and adapted from /nd280mc/ND280ExtrudedScintConstructor
///
/// Construct a cubic extruded scintillator cube.  The cube
/// are constructed as rectangular bars with edges of same size
/// with the Z axis along the length of the bar, the base is
/// along the X axis, and the height along the Y axis. 
/// 
/// By default, the bars are made with polystyrene, and have "coextruded"
/// TiO2+Polystyrene outer coating.  If the coating thickness can be set to
/// zero with SetCoatingThickness() but will still be constructed.  The bars
/// have a hole for the fiber (default: 0.9 mm), and a fiber (default: 0.5 mm).
/// If the hole radius is less than the fiber radius then neither the fiber
/// nor the hole are constructed.
///

class ND280CubeScintConstructor : public ND280Constructor {

public:

  //typedef enum {eTriangle, eRectangle} ExtrudedShape;

  //ND280CubeScintConstructor(G4String n, ND280UserDetectorConstruction* c)
    ND280CubeScintConstructor(G4String n, ExN02DetectorConstruction* c)
        : ND280Constructor(n,c) {Init();};
    ND280CubeScintConstructor(G4String n, ND280Constructor* p)
        : ND280Constructor(n,p) {Init();};
    virtual ~ND280CubeScintConstructor();

    /// Get a bar of scintillator.
    virtual G4LogicalVolume *GetPiece(void);

    /// Get the length of the bar to be constructed.
    virtual double GetLength(void) {return fLength;};

    /// Set the length of the bar to be constructed.
    virtual void SetLength(double len) {fLength = len;};

    /// Get the base width of the extruded bar.
    virtual double GetBase(void) {return fBase;};

    /// Set the base width of the extruded bar.
    virtual void SetBase(double base);

    /// Get the width of the top of the scintillator.  The top is along the X
    /// axis, and will be equal to zero for triangular bars.  For rectangular
    /// bars, top is equal to the base.  The size of the top is calculated
    /// internally so that it matches the current base and shape definitions.
    //
    //virtual double GetTop(void);
    //
  
  // Set the radius of the hole.
  virtual void SetHoleRadius(double radius) {fHoleRadius = radius;};
  
  // Get the radius of the hole.
  virtual double GetHoleRadius(void) {return fHoleRadius;};

  // Set the Fiber Hole position along X axis
  virtual void SetHolePosition_X(G4ThreeVector pos) {fHolePositionX = pos;};
  // Get the Fiber Hole position along X axis
  virtual G4ThreeVector GetHolePosition_X(void) {return fHolePositionX;};

  // Set the Fiber Hole position along Y axis
  virtual void SetHolePosition_Y(G4ThreeVector pos) {fHolePositionY = pos;};
  // Get the Fiber Hole position along Y axis
  virtual G4ThreeVector GetHolePosition_Y(void) {return fHolePositionY;};

  // Set the Fiber Hole position along Z axis
  virtual void SetHolePosition_Z(G4ThreeVector pos) {fHolePositionZ = pos;};
  // Get the Fiber Hole position along Z axis
  virtual G4ThreeVector GetHolePosition_Z(void) {return fHolePositionZ;};


  /// Set the WLS fiber radius
  virtual void SetFiberRadius(double rad){fFiberRadius = rad;};
  /// Get the WLS fiber radius
  double GetFiberRadius(){return fFiberRadius;};

  /// Set the WLS fiber material
  virtual void SetFiberMaterial(std::string mat){fFiberMaterial = mat;};
  /// Get the WLS fiber material
  std::string GetFiberMaterial(){return fFiberMaterial;};



  // Set rotation for the Fiber Hole along X axis
  virtual void SetHoleRot_X(G4RotationMatrix rot) {fHoleRotX = rot;};
  // Get rotation for the Fiber Hole along X axis
  virtual G4RotationMatrix GetHoleRot_X(void) {return fHoleRotX;};

  // Set rotation for the Fiber Hole along Y axis
  virtual void SetHoleRot_Y(G4RotationMatrix rot) {fHoleRotY = rot;};
  // Get rotation for the Fiber Hole along Y axis
  virtual G4RotationMatrix GetHoleRot_Y(void) {return fHoleRotY;};

  // Set rotation for the Fiber Hole along Z axis
  virtual void SetHoleRot_Z(G4RotationMatrix rot) {fHoleRotZ = rot;};
  // Get rotation for the Fiber Hole along Z axis
  virtual G4RotationMatrix GetHoleRot_Z(void) {return fHoleRotZ;};





  
  /// Get the thickness of the reflective coating.
  virtual double GetCoatingThickness(void) {return fCoatingThickness;}
  
  /// Set the thickness of the reflective coating.
    virtual void SetCoatingThickness(double t) {fCoatingThickness=t;}

  ///// Get the inner radius of the corner reflective coating for FGD scints.
  //virtual double GetCoatingRadius(void) {return fCoatingRadius;}

  ///// Set the inner radius of the corner reflective coating for FGD scints.
  //virtual void SetCoatingRadius(double r) {fCoatingRadius=r;} 

    /// Set the gap between the bars which corresponds to the space taken by
    /// whatever material is around the extrusion.  E.G. This is the space
    /// taken by the glue that is holding the bars together.
  virtual void SetGap(double g) {fGap = g;};

    /// Get the gap between the bars which corresponds to the space taken by
    /// whatever material is around the extrusion.  
    virtual double GetGap(void) {return fGap;};

  /// Set the shape of the extrusion.
  //virtual void SetShape(ExtrudedShape s) {fShape = s;};

  /// Get the shape of the extrusion.
  //virtual ExtrudedShape GetShape(void) {return fShape;};

    /// Get the height of the extruded bar.  The default value will be by a
    /// call to SetBase().
    virtual double GetHeight(void) {return fHeight;};

    /// Set the height of the extruded bar.
    virtual void SetHeight(double height);

    /// Get the center to center spacing for bars which depends on the shape
    /// of the bar.
    //
    //virtual double GetCenterToCenter(void);
    //

    /// @{ The name of the material to use for the scintillator coating.
    /// [Default: ScintillatorCoating]
    virtual void SetCoatingMaterial(const char* mat) {fCoatingMaterial = mat;}
    const char* GetCoatingMaterial() const {return fCoatingMaterial.c_str();}
    /// @}

    /// @{ The name of the material to use for the scintillator. 
    /// [Default: Scintillator]
    virtual void SetScintillatorMaterial(const char* mat) {
        fScintillatorMaterial = mat;}
    const char* GetScintillatorMaterial() const {
        return fScintillatorMaterial.c_str();}
    /// @}

    virtual void SetVisibility(bool vis) {fIsVisible = vis;}

    virtual bool GetVisibility(void) {return fIsVisible;}

  /// Set the sensitive detector for this component.
  //virtual void SetSensitiveDetector(G4VSensitiveDetector* s);
  
  /// Get the sensitive detector for this component.
  //virtual G4VSensitiveDetector* GetSensitiveDetector(void) const {
  //  return fSensitiveDetector;
  //}

  ExN02ND280XML * GetND280XML() const {return fND280XMLInput;};
  
private:

    void Init(void);

    /// The length of the scintillator bar.
    double fLength;
    
    /// The shape of the bar.
    //ExtrudedShape fShape;

    /// The width of the base of the scintillator.  The base is along the X
    /// axis.
    double fBase;
    
    /// The height of the scintillator.  If the bar is triangular this will be
    /// calculated when the base is set, but may be overridden.  
    double fHeight;

    /// The radius of the hole.
    double fHoleRadius;

  // Fiber Hole position along X, Y and Z axes
  G4ThreeVector fHolePositionX;
  G4ThreeVector fHolePositionY;
  G4ThreeVector fHolePositionZ;

  // Rotation for the Fiber Hole along X, Y and Z axes
  G4RotationMatrix fHoleRotX;
  G4RotationMatrix fHoleRotY;
  G4RotationMatrix fHoleRotZ;
  
    /// The thickness of the material that is wrapping the scintillator (this
    /// may be extruded).
    double fCoatingThickness;

    /// The inner corner radius of the extruded wrapping for square FGD
    /// scintillators
    //double fCoatingRadius;

    /// Space outside of the bar to allow for glueing and mounting.
    double fGap;

    /// Flag if the created bar should be set to visible for G4 visualization. 
    bool fIsVisible;

    /// The name of the material to use for the coating.
    std::string fCoatingMaterial;

    /// The name of the material to use for the scintillator.
    std::string fScintillatorMaterial;

  /// The WLS fiber material
  std::string fFiberMaterial;

  /// The WLS fiber radius
  double fFiberRadius;
  
  ExN02ND280XML *fND280XMLInput;
};
#endif
