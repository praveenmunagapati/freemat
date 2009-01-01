//===-- LowerSubregs.cpp - Subregister Lowering instruction pass ----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines a MachineFunction pass which runs after register
// allocation that turns subreg insert/extract instructions into register
// copies, as needed. This ensures correct codegen even if the coalescer
// isn't able to remove all subreg instructions.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "lowersubregs"
#include "llvm/CodeGen/Passes.h"
#include "llvm/Function.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/Target/TargetRegisterInfo.h"
#include "llvm/Target/TargetInstrInfo.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/Compiler.h"
using namespace llvm;

namespace {
  struct VISIBILITY_HIDDEN LowerSubregsInstructionPass
   : public MachineFunctionPass {
    static char ID; // Pass identification, replacement for typeid
    LowerSubregsInstructionPass() : MachineFunctionPass(&ID) {}
    
    const char *getPassName() const {
      return "Subregister lowering instruction pass";
    }

    virtual void getAnalysisUsage(AnalysisUsage &AU) const {
      AU.addPreservedID(MachineLoopInfoID);
      AU.addPreservedID(MachineDominatorsID);
      MachineFunctionPass::getAnalysisUsage(AU);
    }

    /// runOnMachineFunction - pass entry point
    bool runOnMachineFunction(MachineFunction&);
    
    bool LowerExtract(MachineInstr *MI);
    bool LowerInsert(MachineInstr *MI);
    bool LowerSubregToReg(MachineInstr *MI);
  };

  char LowerSubregsInstructionPass::ID = 0;
}

FunctionPass *llvm::createLowerSubregsPass() { 
  return new LowerSubregsInstructionPass(); 
}

bool LowerSubregsInstructionPass::LowerExtract(MachineInstr *MI) {
   MachineBasicBlock *MBB = MI->getParent();
   MachineFunction &MF = *MBB->getParent();
   const TargetRegisterInfo &TRI = *MF.getTarget().getRegisterInfo();
   const TargetInstrInfo &TII = *MF.getTarget().getInstrInfo();
   
   assert(MI->getOperand(0).isReg() && MI->getOperand(0).isDef() &&
          MI->getOperand(1).isReg() && MI->getOperand(1).isUse() &&
          MI->getOperand(2).isImm() && "Malformed extract_subreg");

   unsigned DstReg   = MI->getOperand(0).getReg();
   unsigned SuperReg = MI->getOperand(1).getReg();
   unsigned SubIdx   = MI->getOperand(2).getImm();
   unsigned SrcReg   = TRI.getSubReg(SuperReg, SubIdx);

   assert(TargetRegisterInfo::isPhysicalRegister(SuperReg) &&
          "Extract supperg source must be a physical register");
   assert(TargetRegisterInfo::isPhysicalRegister(DstReg) &&
          "Insert destination must be in a physical register");
          
   DOUT << "subreg: CONVERTING: " << *MI;

   if (SrcReg != DstReg) {
     const TargetRegisterClass *TRC = TRI.getPhysicalRegisterRegClass(DstReg);
     assert(TRC == TRI.getPhysicalRegisterRegClass(SrcReg) &&
             "Extract subreg and Dst must be of same register class");
     TII.copyRegToReg(*MBB, MI, DstReg, SrcReg, TRC, TRC);
     
#ifndef NDEBUG
     MachineBasicBlock::iterator dMI = MI;
     DOUT << "subreg: " << *(--dMI);
#endif
   }

   DOUT << "\n";
   MBB->erase(MI);
   return true;
}

bool LowerSubregsInstructionPass::LowerSubregToReg(MachineInstr *MI) {
  MachineBasicBlock *MBB = MI->getParent();
  MachineFunction &MF = *MBB->getParent();
  const TargetRegisterInfo &TRI = *MF.getTarget().getRegisterInfo(); 
  const TargetInstrInfo &TII = *MF.getTarget().getInstrInfo();
  assert((MI->getOperand(0).isReg() && MI->getOperand(0).isDef()) &&
         MI->getOperand(1).isImm() &&
         (MI->getOperand(2).isReg() && MI->getOperand(2).isUse()) &&
          MI->getOperand(3).isImm() && "Invalid subreg_to_reg");
          
  unsigned DstReg  = MI->getOperand(0).getReg();
  unsigned InsReg  = MI->getOperand(2).getReg();
  unsigned SubIdx  = MI->getOperand(3).getImm();     

  assert(SubIdx != 0 && "Invalid index for insert_subreg");
  unsigned DstSubReg = TRI.getSubReg(DstReg, SubIdx);
  
  assert(TargetRegisterInfo::isPhysicalRegister(DstReg) &&
         "Insert destination must be in a physical register");
  assert(TargetRegisterInfo::isPhysicalRegister(InsReg) &&
         "Inserted value must be in a physical register");

  DOUT << "subreg: CONVERTING: " << *MI;

  if (DstSubReg == InsReg) {
    // No need to insert an identify copy instruction.
    DOUT << "subreg: eliminated!";
  } else {
    // Insert sub-register copy
    const TargetRegisterClass *TRC0= TRI.getPhysicalRegisterRegClass(DstSubReg);
    const TargetRegisterClass *TRC1= TRI.getPhysicalRegisterRegClass(InsReg);
    TII.copyRegToReg(*MBB, MI, DstSubReg, InsReg, TRC0, TRC1);

#ifndef NDEBUG
    MachineBasicBlock::iterator dMI = MI;
    DOUT << "subreg: " << *(--dMI);
#endif
  }

  DOUT << "\n";
  MBB->erase(MI);
  return true;                    
}

bool LowerSubregsInstructionPass::LowerInsert(MachineInstr *MI) {
  MachineBasicBlock *MBB = MI->getParent();
  MachineFunction &MF = *MBB->getParent();
  const TargetRegisterInfo &TRI = *MF.getTarget().getRegisterInfo(); 
  const TargetInstrInfo &TII = *MF.getTarget().getInstrInfo();
  assert((MI->getOperand(0).isReg() && MI->getOperand(0).isDef()) &&
         (MI->getOperand(1).isReg() && MI->getOperand(1).isUse()) &&
         (MI->getOperand(2).isReg() && MI->getOperand(2).isUse()) &&
          MI->getOperand(3).isImm() && "Invalid insert_subreg");
          
  unsigned DstReg = MI->getOperand(0).getReg();
  unsigned SrcReg = MI->getOperand(1).getReg();
  unsigned InsReg = MI->getOperand(2).getReg();
  unsigned SubIdx = MI->getOperand(3).getImm();     

  assert(DstReg == SrcReg && "insert_subreg not a two-address instruction?");
  assert(SubIdx != 0 && "Invalid index for insert_subreg");
  unsigned DstSubReg = TRI.getSubReg(DstReg, SubIdx);
  
  assert(TargetRegisterInfo::isPhysicalRegister(SrcReg) &&
         "Insert superreg source must be in a physical register");
  assert(TargetRegisterInfo::isPhysicalRegister(InsReg) &&
         "Inserted value must be in a physical register");

  DOUT << "subreg: CONVERTING: " << *MI;

  if (DstSubReg == InsReg) {
    // No need to insert an identify copy instruction.
    DOUT << "subreg: eliminated!";
  } else {
    // Insert sub-register copy
    const TargetRegisterClass *TRC0= TRI.getPhysicalRegisterRegClass(DstSubReg);
    const TargetRegisterClass *TRC1= TRI.getPhysicalRegisterRegClass(InsReg);
    TII.copyRegToReg(*MBB, MI, DstSubReg, InsReg, TRC0, TRC1);
#ifndef NDEBUG
    MachineBasicBlock::iterator dMI = MI;
    DOUT << "subreg: " << *(--dMI);
#endif
  }

  DOUT << "\n";
  MBB->erase(MI);
  return true;                    
}

/// runOnMachineFunction - Reduce subregister inserts and extracts to register
/// copies.
///
bool LowerSubregsInstructionPass::runOnMachineFunction(MachineFunction &MF) {
  DOUT << "Machine Function\n";
  
  bool MadeChange = false;

  DOUT << "********** LOWERING SUBREG INSTRS **********\n";
  DOUT << "********** Function: " << MF.getFunction()->getName() << '\n';

  for (MachineFunction::iterator mbbi = MF.begin(), mbbe = MF.end();
       mbbi != mbbe; ++mbbi) {
    for (MachineBasicBlock::iterator mi = mbbi->begin(), me = mbbi->end();
         mi != me;) {
      MachineInstr *MI = mi++;
           
      if (MI->getOpcode() == TargetInstrInfo::EXTRACT_SUBREG) {
        MadeChange |= LowerExtract(MI);
      } else if (MI->getOpcode() == TargetInstrInfo::INSERT_SUBREG) {
        MadeChange |= LowerInsert(MI);
      } else if (MI->getOpcode() == TargetInstrInfo::SUBREG_TO_REG) {
        MadeChange |= LowerSubregToReg(MI);
      }
    }
  }

  return MadeChange;
}