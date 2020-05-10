#include "backend/codegen.h"

#include <algorithm>
#include <sstream>
#include <vector>

namespace cs160::backend {

// Code generator methods


void CodeGen::addentry(std::string name, int offset) {
  symbols.variables.push_back(name);
  symbols.offsets.push_back(offset);
}

int CodeGen::findentry(std::string name) {
  for (int i = 0; i < symbols.variables.size(); i++) {
    if (symbols.variables.at(i).compare(name) == 0) {
      return symbols.offsets.at(i);
    }
  }
}


std::vector<std::string> CodeGen::generateCode(const Program & program) {
  // The main entry point of the code generator. This method should
  // clean up the data structures, visit given AST, and return the
  // generated list of instructions.

  // Right now it returns a dummy assembly program that returns 0 as
  // the output
  printf("generating Code");
  VisitProgramExpr(program);
  printf("leaving generating code");
}

void CodeGen::VisitIntegerExpr(const IntegerExpr& exp) {
  printf("Entering VisitIntegerExpr");

  printf("Exiting VisitIntegerExpr");
}

void CodeGen::VisitVariableExpr(const VariableExpr& exp) {
  printf("Entering VisitVariableExpr");
  printf("Exiting VisitVariableExpr");
}

void CodeGen::VisitAddExpr(const AddExpr& exp) {
  printf("Entering VisitAddExpr");
  printf("Exiting VisitAddExpr");
}

void CodeGen::VisitSubtractExpr(const SubtractExpr& exp) {
  printf("Entering VisitSubtractExpr");
  printf("Exiting VisitSubtractExpr");
}

void CodeGen::VisitMultiplyExpr(const MultiplyExpr& exp) {
  printf("Entering VisitMultiplyExpr");
  printf("Exiting VisitMultiplyExpr");
}

void CodeGen::VisitLessThanExpr(const LessThanExpr& exp) {
  printf("Entering VisitLessThanExpr");
  printf("Exiting VisitLessThanExpr");
}

void CodeGen::VisitLessThanEqualToExpr(const LessThanEqualToExpr& exp) {
  printf("Entering VisitLessThanToEqualExpr");
  printf("Exiting VisitLessThanEqualToExpr");
}

void CodeGen::VisitEqualToExpr(const EqualToExpr& exp) {
  printf("Entering VisitEqualToEqualExpr");
  printf("Exiting VisitEqualToExpr");
}

void CodeGen::VisitLogicalAndExpr(const LogicalAndExpr& exp) {
  printf("Entering VisitLogicalAndExpr");
  printf("Exiting VisitLogicalAndExpr");
}

void CodeGen::VisitLogicalOrExpr(const LogicalOrExpr& exp) {
  printf("Entering VisitLogicalOrExpr");
  printf("Exiting VisitLogicalOrExpr");
}

void CodeGen::VisitLogicalNotExpr(const LogicalNotExpr& exp) {
  printf("Entering VisitLogicalNotExpr");
  printf("Exiting VisitLogicalNotExpr");
}

void CodeGen::VisitIntTypeExpr(const IntType& exp) {
  printf("Entering VisitIntTypeExpr");
  printf("Exiting VisitIntTypeExpr");
}

void CodeGen::VisitBlockExpr(const BlockExpr& exp) {
  printf("Entering VisitBlockTypeExpr");
  printf("Exiting VisitBlockeExpr");
}

void CodeGen::VisitDeclarationExpr(const Declaration& exp) {
  printf("Entering VisitDeclarationExpr");
  printf("Exiting VisitDeclarationExpr");
}

void CodeGen::VisitAssignmentExpr(const Assignment& assignment) {
  printf("Entering VisitAssignmentExpr");
  printf("Exiting VisitAssignmentExpr");
}

void CodeGen::VisitConditionalExpr(const Conditional& conditional) {
  printf("Entering VisitConditionalExpr");
  printf("Exiting VisitConditionalExpr");
}

void CodeGen::VisitLoopExpr(const Loop& loop) {
  printf("Entering VisitLoopExpr");
  printf("Exiting VisitLoopExpr");
}

void CodeGen::VisitFunctionCallExpr(const FunctionCall& call) {
  printf("Entering VisitFunctionCallExpr");
  printf("Exiting VisitFunctionCallExpr");
}

void CodeGen::VisitFunctionDefExpr(const FunctionDef& def) {
  printf("Entering VisitFunctionDefExpr");
  printf("Exiting VisitFunctionDefExpr");
}

void CodeGen::VisitProgramExpr(const Program& program) {
  printf("Entering VisitProgramExpr");
  printf("Exiting VisitProgramExpr");
}

}  // namespace cs160::backend
