#include "backend/codegen.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

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

void CodeGen::allocate() {
  output.push_back("  sub $4, %esp");
  latestoffset = latestoffset + 4;
}

void CodeGen::deallocate() {
  output.push_back("  add $4, %esp");
  latestoffset = latestoffset - 4;
}

std::vector<std::string> CodeGen::generateCode(const Program & program) {
  // The main entry point of the code generator. This method should
  // clean up the data structures, visit given AST, and return the
  // generated list of instructions.

  // Right now it returns a dummy assembly program that returns 0 as
  // the output
  cout << "Making Code" << endl;
  if (program.function_defs().size() == 0) {
    output.push_back("  .globl Entry");
    output.push_back("  .type Entry, @function");
  }
  VisitProgramExpr(program);
  cout << "leaving making code" << endl;
  return output;
}

void CodeGen::VisitIntegerExpr(const IntegerExpr& exp) {
  cout << "Entering VisitIntegerExpr" << endl;
  output.push_back("  movl $" + std::to_string(exp.value()) + ", %eax");
  cout << "Exiting VisitIntegerExpr" << endl;
}


void CodeGen::VisitVariableExpr(const VariableExpr& exp) {
  cout << "Entering VisitVariableExpr" << endl;
  string input = exp.name();
  for (int i = 0; i < symbols.variables.size(); i++) {
    if (symbols.variables.at(i).compare(input) == 0) {
      int index = symbols.offsets.at(i);
      output.push_back("  movl -" + std::to_string(index) + "(%ebp), %eax");
      output.push_back("  add $" + std::to_string(index) + ", %esp");
      latestoffset = 0;
      cout << "outputted variable" << endl;
      return;
    }
  }
  cout << "Exiting VisitVariableExpr, could not output variable" << endl;
}



void CodeGen::VisitAddExpr(const AddExpr& exp) {
  cout << "Entering VisitAddExpr" << endl;
  allocate();
  exp.lhs().Visit(this);
  output.push_back("  movl %eax, -" + std::to_string(latestoffset) + "(%ebp)");
  exp.rhs().Visit(this);
  output.push_back("  movl -" + std::to_string(latestoffset) + "(%ebp), %edx");
  output.push_back("  add %edx, %eax");
  deallocate();
  cout << "Exiting VisitAddExpr" << endl;
}

void CodeGen::VisitSubtractExpr(const SubtractExpr& exp) {
  cout << "Entering VisitSubtractExpr" << endl;
  allocate();
  exp.lhs().Visit(this);
  output.push_back("  movl %eax, -" + std::to_string(latestoffset) + "(%ebp)");
  exp.rhs().Visit(this);
  output.push_back("  movl -" + std::to_string(latestoffset) + "(%ebp), %edx");
  output.push_back("  sub %eax, %edx");
  output.push_back("  movl %edx, %eax");
  deallocate();
  cout << "Exiting VisitSubtractExpr" << endl;
}

void CodeGen::VisitMultiplyExpr(const MultiplyExpr& exp) {
  cout << "Entering VisitMultiplyExpr" << endl;
  allocate();
  exp.lhs().Visit(this);
  output.push_back("  movl %eax, -" + std::to_string(latestoffset) + "(%ebp)");
  exp.rhs().Visit(this);
  output.push_back("  movl -" + std::to_string(latestoffset) + "(%ebp), %edx");
  output.push_back("  imul %edx, %eax");
  deallocate();
  cout << "Exiting VisitMultiplyExpr" << endl;
}

void CodeGen::VisitLessThanExpr(const LessThanExpr& exp) {
  cout << "Entering VisitLessThanExpr" << endl;
  exp.lhs().Visit(this);
  exp.rhs().Visit(this);
  cout << "Exiting VisitLessThanExpr" << endl;
}

void CodeGen::VisitLessThanEqualToExpr(const LessThanEqualToExpr& exp) {
  cout << "Entering VisitLessThanToEqualExpr" << endl;
  exp.lhs().Visit(this);
  exp.rhs().Visit(this);
  cout << "Exiting VisitLessThanEqualToExpr" << endl;
}

void CodeGen::VisitEqualToExpr(const EqualToExpr& exp) {
  cout << "Entering VisitEqualToEqualExpr" << endl;
  exp.lhs().Visit(this);
  exp.rhs().Visit(this);
  cout << "Exiting VisitEqualToExpr" << endl;
}

void CodeGen::VisitLogicalAndExpr(const LogicalAndExpr& exp) {
  cout << "Entering VisitLogicalAndExpr" << endl;
  exp.lhs().Visit(this);
  exp.rhs().Visit(this);
  cout << "Exiting VisitLogicalAndExpr" << endl;
}

void CodeGen::VisitLogicalOrExpr(const LogicalOrExpr& exp) {
  cout << "Entering VisitLogicalOrExpr" << endl;
  exp.lhs().Visit(this);
  exp.rhs().Visit(this);
  cout << "Exiting VisitLogicalOrExpr" << endl;
}

void CodeGen::VisitLogicalNotExpr(const LogicalNotExpr& exp) {
  cout << "Entering VisitLogicalNotExpr" << endl;
  exp.operand().Visit(this);
  cout << "Exiting VisitLogicalNotExpr" << endl;
}

void CodeGen::VisitIntTypeExpr(const IntType& exp) {
  cout << "Entering VisitIntTypeExpr" << endl;
  exp.value();
  cout << "Exiting VisitIntTypeExpr" << endl;
}

void CodeGen::VisitBlockExpr(const BlockExpr& exp) {
  cout << "Entering VisitBlockTypeExpr" << endl;
  int numvar = exp.decls().size();
  if (numvar == 0) {
    output.push_back("  sub $0, %esp");
  }
  for (auto it = exp.decls().begin(); it != exp.decls().end(); ++it) {
      (*it)->Visit(this);
  }
  for (auto it = exp.stmts().begin(); it != exp.stmts().end(); ++it) {
    (*it)->Visit(this);
  }
  cout << "Exiting VisitBlockeExpr" << endl;
}

void CodeGen::VisitDeclarationExpr(const Declaration& exp) {
  cout << "Entering VisitDeclarationExpr" << endl;
  exp.type().Visit(this);
  exp.id().Visit(this);
  allocate();
  addentry(exp.id().toString(), latestoffset);
  output.push_back("  movl $0, -" + std::to_string(latestoffset) + "(%ebp)");
  cout << "Exiting VisitDeclarationExpr" << endl;
}

void CodeGen::VisitAssignmentExpr(const Assignment& assignment) {
  cout << "Entering VisitAssignmentExpr" << endl;
  assignment.lhs().Visit(this);
  assignment.rhs().Visit(this);
  for (int i = 0; i < symbols.variables.size(); i++) {
    if (symbols.variables.at(i).compare(assignment.lhs().toString()) == 0) {
      int index = symbols.offsets.at(i);
      output.push_back("  movl $" + assignment.rhs().toString() + ", -" + std::to_string(index) + "(%ebp)");
      cout << "Successfully changed variable" << endl;
      return;
    }
  }
  cout << "Exiting VisitAssignmentExp. Could not change variable" << endl;
}

void CodeGen::VisitConditionalExpr(const Conditional& conditional) {
  cout << "Entering VisitConditionalExpr" << endl;
  conditional.guard().Visit(this);
  conditional.true_branch().Visit(this);
  conditional.false_branch().Visit(this);
  cout << "Exiting VisitConditionalExpr" << endl;
}

void CodeGen::VisitLoopExpr(const Loop& loop) {
  cout << "Entering VisitLoopExpr" << endl;
  loop.guard().Visit(this);
  loop.body().Visit(this);
  cout << "Exiting VisitLoopExpr" << endl;
}




void CodeGen::VisitFunctionCallExpr(const FunctionCall& call) {
  cout << "Entering VisitFunctionCallExpr" << endl;
  output.push_back("  push %eax");
  output.push_back("  call " + call.callee_name());
  for (auto it = call.arguments().begin(); it != call.arguments().end(); ++it) {
      (*it)->Visit(this);
  }
  cout << "Exiting VisitFunctionCallExpr" << endl;
}



void CodeGen::VisitFunctionDefExpr(const FunctionDef& def) {
  cout << "Entering VisitFunctionDefExpr" << endl;
  output.push_back(def.function_name() + ":");
  output.push_back("  push %ebp");
  output.push_back("  movl %esp, %ebp");
  for (auto it = def.parameters().begin(); it != def.parameters().end(); ++it) {
      (*it).first->Visit(this);
      (*it).second->Visit(this);
  }
  def.type().Visit(this);
  def.function_body().Visit(this);
  def.retval().Visit(this);
  cout << "Exiting VisitFunctionDefExpr" << endl;
}

//Program contians FunctionDef::Block function_defs_; std::unique_ptr<const BlockExpr> statements_;
// std::unique_ptr<const ArithmeticExpr> arithmetic_exp_;
//eax is result register
//edx is other register
//ebp is frame pointer
//esp is stack pointer


void CodeGen::VisitProgramExpr(const Program& program) {
  cout << "Entering VisitProgramExpr" << endl;
  for (auto it = program.function_defs().begin(); it != program.function_defs().end(); ++it) {
      (*it)->Visit(this);
    }
  output.push_back("Entry:");
  output.push_back("  push %ebp");
  output.push_back("  movl %esp, %ebp");
  program.statements().Visit(this);
  program.arithmetic_exp().Visit(this);
  int numvar = program.statements().decls().size();
  if (numvar == 0) {
    output.push_back("  add $0, %esp");
  }
  output.push_back("  movl %ebp, %esp");
  output.push_back("  pop %ebp");
  output.push_back("  ret");
  cout << "leaving VisitProgramExpr" << endl;
}

}  // namespace cs160::backend
