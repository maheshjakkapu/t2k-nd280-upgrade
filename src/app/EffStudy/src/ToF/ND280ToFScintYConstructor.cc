#include <globals.hh>
#include <G4RotationMatrix.hh>
#include <G4VSolid.hh>
#include <G4ThreeVector.hh>
#include <G4Box.hh>
#include <G4PVPlacement.hh>

//#include <TND280Log.hxx>

//#include "ecal/ND280ECal.hh"
#include "ND280ToFScintYConstructor.hh"

void ND280ToFScintYConstructor::Init() {
  //SetMaximumHitLength( ND280ECal::HitLength );
  //SetMaximumHitSagitta( ND280ECal::HitSagitta );

  SetBarBase( 1.*cm );
  //SetBarBase( ND280ECal::BarWidth );
  SetBarHeight( 1.*cm );
  SetLength( 1.*cm );
  SetBarGap(0.0);
  SquareBar();
  MakeParallelBars();
  SetEdgeSpace(0.0);
}

void ND280ToFScintYConstructor::SetUp(int n_bars,
				      double bar_length,
				      double bar_base){

  // NEW ND280 UPGRADE
  SetBarBase( bar_base*mm );
  SetBarHeight( bar_base*mm );
  //

  SetBarNumber( n_bars );
  SetWidth( bar_length );
  //SetHeight(n_bars * ND280ECal::BarWidth + 0.001*mm);
  SetHeight( n_bars * bar_base); // + 0.001*mm ); // length of an x-bar
}
