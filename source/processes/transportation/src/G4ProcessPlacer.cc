#include "G4ProcessPlacer.hh"
#include "G4ProcessManager.hh"
#include "G4VProcess.hh"
#include "G4ParticleTable.hh"

G4ProcessPlacer::G4ProcessPlacer(const G4String &particlename):
  G4VProcessPlacer(particlename),
  fParticleName(particlename){
  G4cout << "+++ G4ProcessPlacer::G4ProcessPlacer: for: " <<  particlename 
	 << G4endl;
}

void G4ProcessPlacer::AddProcessAs(G4VProcess *process, SecondOrLast sol) {

  G4cout << "  ProcessName: " << process->GetProcessName() << G4endl;
  
  G4ProcessVector* processGPILVec = 
    GetProcessManager().GetPostStepProcessVector(typeGPIL);
  G4cout << "The initial GPILVec: " << G4endl;
  PrintProcVec(processGPILVec);
  G4int  lenGPIL = processGPILVec->length();

  G4ProcessVector* processDoItVec = 
    GetProcessManager().GetPostStepProcessVector(typeDoIt); 
  G4cout << "The initial DoItVec: " << G4endl;
  PrintProcVec(processDoItVec);
  
  // the logic
  if (sol == eSecond) {
    processGPILVec->insertAt(lenGPIL-1, process);
    processDoItVec->insertAt(1, process);
  }
  else if (sol == eLast) {
    processGPILVec->insertAt(0, process);
    processDoItVec->insert(process);
  } 
  else {
    G4cout << "G4ProcessPlacer::AddProcessAs Error" << G4endl;
  }
  
  // for verification inly
  G4cout << "The final GPIL Vec: " << G4endl;
  PrintProcVec(processGPILVec);
  G4cout << "The final DoIt Vec: " << G4endl;
  PrintProcVec(processDoItVec);
  G4cout << "================================================" << G4endl;
  
}
void G4ProcessPlacer::AddProcessAsSecondDoIt(G4VProcess *process) {
  G4cout << "=== G4ProcessPlacer::AddProcessAsSecondDoIt ===" << G4endl;
  AddProcessAs(process, eSecond);
}

void G4ProcessPlacer::AddProcessAsLastDoIt(G4VProcess *process) {
  G4cout << "=== G4ProcessPlacer::AddProcessAsLastDoIt ===" << G4endl;
  AddProcessAs(process, eLast);
}
G4ProcessManager &G4ProcessPlacer::GetProcessManager() { 
  // get particle iterator to add processes ---------------------
  G4ParticleTable* theParticleTable;
  G4ParticleTable::G4PTblDicIterator* theParticleIterator;
  theParticleTable = G4ParticleTable::GetParticleTable();
  theParticleIterator = theParticleTable->GetIterator();
  // -------------------------------------------------------
  G4ProcessManager *processmanager = 0;
  // add parallel transport processe ---------------------------
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    if (particle->GetParticleName() == fParticleName) {
      processmanager =  particle->GetProcessManager();
      break;
    }
  }
  // ---------------------------------------------------------
  if (!processmanager) G4Exception(" G4ProcessPlacer::GetProcessManager: no ProcessManager");
  return *processmanager;
}


void G4ProcessPlacer::PrintProcVec(G4ProcessVector* processVec) {
  if (!processVec) G4Exception("G4ProcessPlacer::G4ProcessPlacer: no processVec");
  G4int len = processVec->length();
  if (len==0) G4Exception("G4ProcessPlacer::G4ProcessPlacer:processVec len = 0");
  for (int pi=0; pi<len; pi++) {
    G4cout << "   " << (*processVec)[pi]->GetProcessName() << G4endl;
  }
}
