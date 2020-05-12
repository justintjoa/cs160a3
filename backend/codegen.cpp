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
  symbols.tempvariables.push_back(name);
  symbols.tempoffsets.push_back(offset);
}

int CodeGen::findentry(std::string name) {
  for (int i = 0; i < symbols.tempvariables.size(); i++) {
    if (symbols.tempvariables.at(i).compare(name) == 0) {
      return symbols.tempoffsets.at(i);
    }
  }
}

void CodeGen::resettemp() {
  if (symbols.tempvariables.size() > 0) {
    symbols.tempvariables.clear();
    symbols.tempoffsets.clear();
  }
}

void CodeGen::adddeclentry(std::string name, int offset) {
  for (int i = 0; i < symbols.declvariables.size(); i++) {
    if (symbols.declvariables.at(i).compare(name) == 0) {
      symbols.decloffsets.at(i) = offset;
      return;
    }
  }
  symbols.declvariables.push_back(name);
  symbols.decloffsets.push_back(offset);
}
  
int CodeGen::finddeclentry(std::string name) {
  for (int i = 0; i < symbols.declvariables.size(); i++) {
    if (symbols.declvariables.at(i).compare(name) == 0) {
      return symbols.decloffsets.at(i);
    }
  }
}

void CodeGen::addarg(std::string name, int offset) {
  for (int i = 0; i < symbols.args.size(); i++) {
    if (symbols.args.at(i).compare(name) == 0) {
      symbols.argoffsets.at(i) = offset;
      return;
    }
  }
  symbols.args.push_back(name);
  symbols.argoffsets.push_back(offset);
}

int CodeGen::findarg(std::string name) {
  for (int i = 0; i < symbols.args.size(); i++) {
    if (symbols.args.at(i).compare(name) == 0) {
      return symbols.argoffsets.at(i);
    }
  }
  return -1;
}

void CodeGen::argallocate() {
  positiveoffset = positiveoffset + 4;
}

void CodeGen::allocate() {
  output.push_back("  sub $4, %esp");
  latestoffset = latestoffset - 4;
}

void CodeGen::silentalloc() {
  latestoffset = latestoffset - 4;
}

void CodeGen::deallocate() {
  output.push_back("  add $4, %esp");
  latestoffset = latestoffset + 4;
}

std::vector<std::string> CodeGen::generateCode(const Program & program) {
  // The main entry point of the code generator. This method should
  // clean up the data structures, visit given AST, and return the
  // generated list of instructions.

  // Right now it returns a dummy assembly program that returns 0 as
  // the output
  cout << "Making Code" << endl;
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
  for (int i = 0; i < symbols.args.size(); i++) {
    if (symbols.args.at(i).compare(input) == 0) {
      int index = symbols.argoffsets.at(i);
      output.push_back("  movl " + std::to_string(index) + "(%ebp), %eax");
      cout << "outputted variable" << endl;
      return;
    }
  }
  for (int i = 0; i < symbols.declvariables.size(); i++) {
    if (symbols.declvariables.at(i).compare(input) == 0) {
      int index = symbols.decloffsets.at(i);
      output.push_back("  movl " + std::to_string(index) + "(%ebp), %eax");
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
  output.push_back("  movl %eax, " + std::to_string(latestoffset) + "(%ebp)");
  exp.rhs().Visit(this);
  output.push_back("  movl " + std::to_string(latestoffset) + "(%ebp), %edx");
  output.push_back("  add %edx, %eax");
  deallocate();
  cout << "Exiting VisitAddExpr" << endl;
}

void CodeGen::VisitSubtractExpr(const SubtractExpr& exp) {
  cout << "Entering VisitSubtractExpr" << endl;
  allocate();
  exp.lhs().Visit(this);
  output.push_back("  movl %eax, " + std::to_string(latestoffset) + "(%ebp)");
  exp.rhs().Visit(this);
  output.push_back("  movl " + std::to_string(latestoffset) + "(%ebp), %edx");
  output.push_back("  sub %eax, %edx");
  output.push_back("  movl %edx, %eax");
  deallocate();
  cout << "Exiting VisitSubtractExpr" << endl;
}

void CodeGen::VisitMultiplyExpr(const MultiplyExpr& exp) {
  cout << "Entering VisitMultiplyExpr" << endl;
  allocate();
  exp.lhs().Visit(this);
  output.push_back("  movl %eax, " + std::to_string(latestoffset) + "(%ebp)");
  exp.rhs().Visit(this);
  output.push_back("  movl " + std::to_string(latestoffset) + "(%ebp), %edx");
  output.push_back("  imul %edx, %eax");
  deallocate();
  cout << "Exiting VisitMultiplyExpr" << endl;
}

void CodeGen::VisitLessThanExpr(const LessThanExpr& exp) {
  cout << "Entering VisitLessThanExpr" << endl;
  allocate();
  exp.lhs().Visit(this);
  output.push_back("  movl %eax, " + std::to_string(latestoffset) + "(%ebp)");
  exp.rhs().Visit(this);
  output.push_back("  movl " + std::to_string(latestoffset) + "(%ebp), %edx");
  output.push_back("  cmp %eax, %edx");
  output.push_back("  setl %al");
  output.push_back("  movzbl %al, %eax");
  deallocate();
  cout << "Exiting VisitLessThanExpr" << endl;
}

void CodeGen::VisitLessThanEqualToExpr(const LessThanEqualToExpr& exp) {
  cout << "Entering VisitLessThanToEqualExpr" << endl;
  allocate();
  exp.lhs().Visit(this);
  output.push_back("  movl %eax, " + std::to_string(latestoffset) + "(%ebp)");
  exp.rhs().Visit(this);
  output.push_back("  movl " + std::to_string(latestoffset) + "(%ebp), %edx");
  output.push_back("  cmp %eax, %edx");
  output.push_back("  setle %al");
  output.push_back("  movzbl %al, %eax");
  deallocate();
  cout << "Exiting VisitLessThanEqualToExpr" << endl;
}


void CodeGen::VisitEqualToExpr(const EqualToExpr& exp) {
  cout << "Entering VisitEqualToEqualExpr" << endl;
  allocate();
  exp.lhs().Visit(this);
  output.push_back("  movl %eax, " + std::to_string(latestoffset) + "(%ebp)");
  exp.rhs().Visit(this);
  output.push_back("  movl " + std::to_string(latestoffset) + "(%ebp), %edx");
  output.push_back("  cmp %eax, %edx");
  output.push_back("  sete %al");
  output.push_back("  movzbl %al, %eax");
  deallocate();
  cout << "Exiting VisitEqualToExpr" << endl;
}

void CodeGen::VisitLogicalAndExpr(const LogicalAndExpr& exp) {
  cout << "Entering VisitLogicalAndExpr" << endl;
  allocate();
  exp.lhs().Visit(this);
  output.push_back("  movl %eax, " + std::to_string(latestoffset) + "(%ebp)");
  exp.rhs().Visit(this);
  output.push_back("  movl " + std::to_string(latestoffset) + "(%ebp), %edx");
  output.push_back("  andl %edx, %eax");
  deallocate();
  cout << "Exiting VisitLogicalAndExpr" << endl;
}

void CodeGen::VisitLogicalOrExpr(const LogicalOrExpr& exp) {
  cout << "Entering VisitLogicalOrExpr" << endl;
  allocate();
  exp.lhs().Visit(this);
  output.push_back("  movl %eax, " + std::to_string(latestoffset) + "(%ebp)");
  exp.rhs().Visit(this);
  output.push_back("  movl " + std::to_string(latestoffset) + "(%ebp), %edx");
  output.push_back("  orl %edx, %eax");
  deallocate();
  cout << "Exiting VisitLogicalOrExpr" << endl;
}

void CodeGen::VisitLogicalNotExpr(const LogicalNotExpr& exp) {
  cout << "Entering VisitLogicalNotExpr" << endl;
  exp.operand().Visit(this);
  output.push_back("  cmp $0, %eax");
  output.push_back("  sete %al");
  output.push_back("  movzbl %al, %eax");
  cout << "Exiting VisitLogicalNotExpr" << endl;
}

void CodeGen::VisitIntTypeExpr(const IntType& exp) {
  cout << "Entering VisitIntTypeExpr" << endl;
  exp.value();
  cout << "Exiting VisitIntTypeExpr" << endl;
}

void CodeGen::VisitBlockExpr(const BlockExpr& exp) {
  cout << "Entering VisitBlockTypeExpr" << endl;
  int relevdecl = exp.decls().size();
  output.push_back("  sub $" + std::to_string(relevdecl*4) + ", %esp");
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
  silentalloc();
  adddeclentry(exp.id().toString(), latestoffset);
  output.push_back("  movl $0, " + std::to_string(latestoffset) + "(%ebp)");
  cout << "Exiting VisitDeclarationExpr" << endl;
}

void CodeGen::VisitAssignmentExpr(const Assignment& assignment) {
  cout << "Entering VisitAssignmentExpr" << endl;
  string identifier = assignment.lhs().toString();
  assignment.rhs().Visit(this);
  /*
  output.push_back("culprit");
  for (int i = 0; i < symbols.declvariables.size(); i++) {
    output.push_back(symbols.declvariables.at(i));
    output.push_back(std::to_string(symbols.decloffsets.at(i)));
  }
  */
  for (int i = 0; i < symbols.declvariables.size(); i++) {
    if (symbols.declvariables.at(i).compare(identifier) == 0) {
      int index = symbols.decloffsets.at(i);
      output.push_back("  movl %eax, " + std::to_string(index) + "(%ebp)");
      cout << "Successfully changed variable" << endl;
      return;
    }
  }
  cout << "Exiting VisitAssignmentExp. Could not change variable" << endl;
}

void CodeGen::VisitConditionalExpr(const Conditional& conditional) {
  cout << "Entering VisitConditionalExpr" << endl;
  int current = scopecount;
  scopecount++;
  conditional.guard().Visit(this);
  output.push_back("  cmp $0, %eax");
  output.push_back("  je IF_FALSE_" + std::to_string(current));
  conditional.true_branch().Visit(this);
  if (conditional.true_branch().decls().size() == 0) {
    output.push_back("  add $0, %esp");
  }
  else {
    int space = conditional.true_branch().decls().size();
    customdealloc(space);
  }
  output.push_back("  jmp IF_END_" + std::to_string(current));
  output.push_back("IF_FALSE_" + std::to_string(current) + ":");
  conditional.false_branch().Visit(this);
  if (conditional.false_branch().decls().size() == 0) {
    output.push_back("  add $0, %esp");
  }
  else {
    int space = conditional.false_branch().decls().size();
    customdealloc(space);
  }
  output.push_back("IF_END_" + std::to_string(current) + ":");
  cout << "Exiting VisitConditionalExpr" << endl;
}

void CodeGen::VisitLoopExpr(const Loop& loop) {
  cout << "Entering VisitLoopExpr" << endl;
  int current = scopecount;
  scopecount++;
  output.push_back("WHILE_START_" + std::to_string(current) + ":");
  loop.guard().Visit(this);
  output.push_back("  cmp $0, %eax");
  output.push_back("  je WHILE_END_" + std::to_string(current));
  loop.body().Visit(this);
  if (loop.body().decls().size() == 0) {
    output.push_back("  add $0, %esp");
  }
  else {
    int alpha = loop.body().decls().size();
    customdealloc(alpha);
  }
  output.push_back("  jmp WHILE_START_" + std::to_string(current));
  output.push_back("WHILE_END_" + std::to_string(current) + ":");
  cout << "Exiting VisitLoopExpr" << endl;
}

void CodeGen::customdealloc(int removal) {
  output.push_back("  add $" + std::to_string(removal*4) + ", %esp");
  latestoffset = latestoffset + 4*removal;
}


void CodeGen::VisitFunctionCallExpr(const FunctionCall& call) {
  cout << "Entering VisitFunctionCallExpr" << endl;
  for (auto it = call.arguments().rbegin(); it != call.arguments().rend(); ++it) {
      (*it)->Visit(this);
      output.push_back("  push %eax");
      silentalloc();
  }
  output.push_back("  call " + call.callee_name());
  customdealloc(call.arguments().size());
  cout << "Exiting VisitFunctionCallExpr" << endl;
}


void CodeGen::reset() {
  if (symbols.tempvariables.size() > 0) {
    symbols.tempvariables.clear();
    symbols.tempoffsets.clear();
  }
  if (symbols.declvariables.size() > 0) {
    symbols.declvariables.clear();
    symbols.decloffsets.clear();
  }
  if (symbols.args.size() > 0) {
    symbols.args.clear();
    symbols.argoffsets.clear();
  }
  latestoffset = 0;
  positiveoffset = 4;
}



void CodeGen::VisitFunctionDefExpr(const FunctionDef& def) {
  cout << "Entering VisitFunctionDefExpr" << endl;
  output.push_back(def.function_name() + ":");
  output.push_back("  push %ebp");
  output.push_back("  movl %esp, %ebp");
  for (auto it = def.parameters().begin(); it != def.parameters().end(); ++it) {
      (*it).first->Visit(this);
      (*it).second->Visit(this);
      argallocate();
      addarg((*it).second->name(),positiveoffset);
  }
  def.type().Visit(this);
  def.function_body().Visit(this);
  def.retval().Visit(this);
  if (latestoffset >= 0) {
    output.push_back("  add $0, %esp");
  }
  else {
    output.push_back("  add $" + std::to_string(latestoffset*-1) + ", %esp");
  }
  reset();
  output.push_back("  movl %ebp, %esp");
  output.push_back("  pop %ebp");
  output.push_back("  ret");
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
    output.push_back("  .globl Entry");
    output.push_back("  .type Entry, @function");
  output.push_back("Entry:");
  output.push_back("  push %ebp");
  output.push_back("  movl %esp, %ebp");
  program.statements().Visit(this);
  program.arithmetic_exp().Visit(this);
  if (latestoffset < 0) {
    output.push_back("  add $" + std::to_string(latestoffset*-1) + ", %esp");
  }
  latestoffset = 0;
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
