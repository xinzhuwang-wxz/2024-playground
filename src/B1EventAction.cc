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
/// \file B1EventAction.cc
/// \brief Implementation of the B1EventAction class

#include <G4SIunits.hh>
#include "B1EventAction.hh"
#include "B1RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include <fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::B1EventAction(B1RunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fEdep(0.)
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::~B1EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::BeginOfEventAction(const G4Event*)
{
    //hitInfoArray.clear();
  fEdep = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void B1EventAction::AddHitInfo(const HitInfo& hitInfo)
{
    // Add the hitInfo object to the hitInfoArray
    hitInfoArray.push_back(hitInfo);
}

void B1EventAction::EndOfEventAction(const G4Event*)
{   
  // accumulate statistics in run action
  fRunAction->AddEdep(fEdep);
    // Calculate momentum using hitInfoArray

    if (hitInfoArray.empty()) {
        G4cout << "hitInfoArray is ～～～～～～～～～～～～～～～～～～～～～empty" << G4endl;
        return;
    }


    auto firstHit = hitInfoArray[0];
    auto lastHit = hitInfoArray[hitInfoArray.size() - 1];
    auto distance = sqrt(pow(firstHit.x_hit - lastHit.x_hit, 2)
                         + pow(firstHit.y_hit - lastHit.y_hit, 2)
                         + pow(firstHit.z_hit - lastHit.z_hit, 2));
    auto dt = lastHit.time - firstHit.time;
    auto v = distance / dt;
    auto c = 299;
    auto gamma = 1 / sqrt(1 - pow(v, 2) / pow(c, 2));
    auto p = gamma * 938 * MeV / pow(c, 2) * v * c;

    G4cout<<"p is ------------------>"<<p<<G4endl;

    std::ofstream outputFile("momentum.txt", std::ios::app);

        // Write the position and momentum to the file
        outputFile << "Momentum: " << p << std::endl;
        outputFile.close();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
