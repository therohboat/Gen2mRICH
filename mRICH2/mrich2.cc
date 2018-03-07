// This code is used for EIC detector R&D project

// Date created:  8/6/2014, hexc
// Date updated: 12/4/2014, hexc, Liang, Ping
//               10/22/2015, hexc and Ping
//                  Adding a variable length for the aerogel block
// Date updated: 2/2/2017, hexc, Ping
//    Updated the code for the 2nd mRICH prototype
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"

#include "mRichDetectorConstruction.hh"
// Commented out by HEXC, Nov 20, 2013
#include "mRichPhysicsList.hh"
//#include "FTFP_BERT.hh"
//#include "QGSP_BERT.hh"
#include "mRichPrimaryGeneratorAction.hh"
#include "mRichRunAction.hh"
//#include "mRichEventAction.hh"
#include "mRichSteppingAction.hh"
#include "mRichSteppingVerbose.hh"
#include "mRichStackingAction.hh"
#include "mRichAgelTrackingAction.hh"

//#include "mRichHistoManager.hh"

//#include "mRichOutput.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  // Choose the Random engine
  //
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  
  // User Verbose output class
  //
  G4VSteppingVerbose::SetInstance(new mRichSteppingVerbose);
     
  // Construct the default run manager
  //
  G4RunManager * runManager = new G4RunManager;

  // Set mandatory initialization classes
  //
  /*
  if (argc != 2)
    {
      G4cout << "Need to add extra thickness for the Aerogel block" << G4endl; 
      G4cout << "   For example:   ./agel_sim 1.0 " << G4endl;
      exit(1);
    }

  G4double agel_thickness_added = atol(argv[1]);
  G4cout << "Adding Aerogel thickness is: " << agel_thickness_added << " cm." << G4endl;
  mRichDetectorConstruction* detector = new mRichDetectorConstruction(agel_thickness_added);
  */
  mRichDetectorConstruction* detector = new mRichDetectorConstruction();
  
  runManager->SetUserInitialization(detector);

  // Use the standard physics list
  // 
  G4VUserPhysicsList* physicsList = new mRichPhysicsList;
  //G4VModularPhysicsList* physicsList = new FTFP_BERT;
  //G4VModularPhysicsList* physicsList = new QGSP_BERT;
  runManager->SetUserInitialization(physicsList);
    
  // Set user action classes
  //
  mRichRunAction* run_action = new mRichRunAction();  
  runManager->SetUserAction(run_action);

  //
  mRichPrimaryGeneratorAction* gen_action = new mRichPrimaryGeneratorAction(run_action);
  runManager->SetUserAction(gen_action);
  // 
  // Create ourput ntuple
  //mRichHistoManager* histo = new mRichHistoManager();

  //
  //mRichEventAction* event_action = new mRichEventAction(run_action);
  //runManager->SetUserAction(event_action);
  //
  //G4UserSteppingAction* stepping_action = new mRichSteppingAction(gen_action, run_action);  
  mRichSteppingAction* stepping_action = new mRichSteppingAction(gen_action, run_action);
  runManager->SetUserAction(stepping_action);
 
  runManager->SetUserAction(new mRichAgelTrackingAction(run_action, gen_action));
 
  // Initialize G4 kernel
  //
  runManager->Initialize();
  
#ifdef G4VIS_USE
  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif
  
#ifdef G4UI_USE
  G4UIExecutive * ui = new G4UIExecutive(argc,argv);      
  ui->SessionStart();
  delete ui;
#endif
  
#ifdef G4VIS_USE
  delete visManager;
#endif                
  
  delete runManager;


  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
