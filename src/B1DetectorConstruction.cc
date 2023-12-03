//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Envelope parameters
  //
  G4double env_sizeXY = 200*cm, env_sizeZ = 200*cm;
//  G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4double world_sizeXY = 3*env_sizeXY;
  G4double world_sizeZ  = 3*env_sizeZ;

  G4double density;
  G4double z,a;
  density = universe_mean_density;    //from PhysicalConstants.h
  G4Material* vacuum=new G4Material("Galactic", z=1., a=1.008*g/mole, density,
                             kStateGas,2.73*kelvin,3.e-18*pascal);

  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* world_mat1 = nist->FindOrBuildMaterial("G4_WATER");
  G4Box* solidWorld =
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size

  G4LogicalVolume* logicWorld =
    new G4LogicalVolume(solidWorld,          //its solid
                        vacuum,           //its material
                        "World");            //its name

  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking



  //
  // Shape 1
  //
  G4Material* shape1_mat = nist->FindOrBuildMaterial("G4_W");
  G4ThreeVector pos1 = G4ThreeVector(0, 0, 20*mm);

  // Conical section shape
  G4double shape1_rmina =  0.*mm, shape1_rmaxa = 0.5*mm;
  G4double shape1_rminb =  0.*mm, shape1_rmaxb = 3.*mm;
  G4double shape1_hz = 10.*mm;
  G4double shape1_phimin = 0.*deg, shape1_phimax = 360.*deg;
  G4Cons* solidShape1 =
    new G4Cons("Shape1",
    shape1_rmina, shape1_rmaxa, shape1_rminb, shape1_rmaxb, shape1_hz,
    shape1_phimin, shape1_phimax);

  G4LogicalVolume* logicShape1 =
    new G4LogicalVolume(solidShape1,         //its solid
                        shape1_mat,          //its material
                        "Shape1");           //its name

  G4VPhysicalVolume* physShape1 =
  new G4PVPlacement(0,                       //no rotation
                    pos1,                    //at position
                    logicShape1,             //its logical volume
                    "Shape1",                //its name
                  logicWorld,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

//shape2
  G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_W");
  G4ThreeVector pos2 = G4ThreeVector(0, 0, 90*mm);

  // Conical section shape
  G4double shape2_rmina =  0.*mm, shape2_rmaxa = 3*mm;
  G4double shape2_rminb =  0.*mm, shape2_rmaxb = 3*mm;
  G4double shape2_hz = 60.*mm;
  G4double shape2_phimin = 0.*deg, shape2_phimax = 360.*deg;
  G4Cons* solidShape2 =
    new G4Cons("Shape2",
    shape2_rmina, shape2_rmaxa, shape2_rminb, shape2_rmaxb, shape2_hz,
    shape2_phimin, shape2_phimax);

  G4LogicalVolume* logicShape2 =
    new G4LogicalVolume(solidShape2,         //its solid
                        shape2_mat,          //its material
                        "Shape2");           //its name

  G4VPhysicalVolume* physShape2 =
  new G4PVPlacement(0,                       //no rotation
                    pos2,                    //at position
                    logicShape2,             //its logical volume
                    "Shape2",                //its name
                  logicWorld,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking


//shape3
  G4Material* shape3_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4ThreeVector pos3 = G4ThreeVector(0, 0, -50*mm);

  // Conical section shape
  G4double shape3_rmina =  0.*mm, shape3_rmaxa = 100*mm;
  G4double shape3_rminb =  0.*mm, shape3_rmaxb = 100*mm;
  G4double shape3_hz = 0.01*mm;
  G4double shape3_phimin = 0.*deg, shape3_phimax = 360.*deg;
  G4Cons* solidShape3 =
    new G4Cons("Shape3",
    shape3_rmina, shape3_rmaxa, shape3_rminb, shape3_rmaxb, shape3_hz,
    shape3_phimin, shape3_phimax);

  G4LogicalVolume* logicShape3 =
    new G4LogicalVolume(solidShape3,         //its solid
                        shape3_mat,          //its material
                        "Shape3");           //its name

  G4VPhysicalVolume* physShape3 =
  new G4PVPlacement(0,                       //no rotation
                    pos3,                    //at position
                    logicShape3,             //its logical volume
                    "Shape3",                //its name
                  logicWorld,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking









  //
// Shape 3
//G4Material* shape3_mat = nist->FindOrBuildMaterial("G4_AIR");
////  //
//    G4Box* shape3 =
//      new G4Box("shape3",                    //its name
//          50*mm, 50*mm, 0.01*mm); //its size

//    G4LogicalVolume* logicshape3=
//      new G4LogicalVolume(shape3,            //its solid
//                         shape3_mat ,             //its material
//                          "shape3");         //its name

//  G4VPhysicalVolume* physShape3 =
//    new G4PVPlacement(0,                       //no rotation
//                      G4ThreeVector(0,-50*mm,0),         //at (0,0,0)
//                      logicshape3,                //its logical volume
//                      "shape3",              //its name
//                      logicWorld,              //its mother  volume
//                      false,                   //no boolean operation
//                      0,                       //copy number
//                      checkOverlaps);          //overlaps checking



//  G4Material* filter_mat = nist->FindOrBuildMaterial("G4_Be");
//  G4Box*solidfilter=new G4Box("filter",
//                         50*cm,2.5*cm,50*cm
//                         );
//  G4LogicalVolume*logicfilter=new G4LogicalVolume(solidfilter,
//                                                  filter_mat,
//                                                  "filter");
//  new G4PVPlacement(0,
//                    G4ThreeVector(0,-30*cm,0),
//                    logicfilter,
//                    "filter",
//                    logicWorld,
//                    false,
//                    0,
//                    checkOverlaps);
// Set Shape33 as scoring volume



    //
  fScoringVolume = logicShape3;


  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

