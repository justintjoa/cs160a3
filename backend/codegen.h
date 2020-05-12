#pragma once

#include "frontend/ast.h"
#include "frontend/ast_visitor.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <memory>
#include <unordered_map>
#include <optional>

using namespace cs160::frontend;

namespace cs160::backend {

struct Table {
  std::vector<std::string> tempvariables;
  std::vector<int> tempoffsets;
  std::vector<std::string> declvariables;
  std::vector<int> decloffsets;
  std::vector<std::string> args;
  std::vector<int> argoffsets;
};

// This is meant to be a generic error, you can use whatever error message you want
struct CodeGenError : public std::runtime_error {
  explicit CodeGenError(const std::string & message) : runtime_error(message) {}
};

// The code generator is implemented as an AST visitor that will generate the relevant pieces of code as it traverses a node
class CodeGen final : public AstVisitor {
 public:
  // Entry point of the code generator. This function should visit given program and return generated code as a list of instructions and labels.
  std::vector<std::string> generateCode(const Program & program);

  // Visitor functions
  void addentry(std::string name, int offset);
  void adddeclentry(std::string name, int offset);
  void addarg(std::string name, int offset);
  int finddeclentry(std::string name);
  int findentry(std::string name);
  int findarg(std::string name);
  void customdealloc(int removal);
  void allocate();
  void reset();
  void silentalloc();
  void deallocate();
  void argallocate();
  void resettemp();
  void VisitIntegerExpr(const IntegerExpr& exp) override;
  void VisitVariableExpr(const VariableExpr& exp) override;
  void VisitAddExpr(const AddExpr& exp) override;
  void VisitSubtractExpr(const SubtractExpr& exp) override;
  void VisitMultiplyExpr(const MultiplyExpr& exp) override;
  void VisitLessThanExpr(const LessThanExpr& exp) override;
  void VisitLessThanEqualToExpr(const LessThanEqualToExpr& exp) override;
  void VisitEqualToExpr(const EqualToExpr& exp) override;
  void VisitLogicalAndExpr(const LogicalAndExpr& exp) override;
  void VisitLogicalOrExpr(const LogicalOrExpr& exp) override;
  void VisitLogicalNotExpr(const LogicalNotExpr& exp) override;
  void VisitIntTypeExpr(const IntType& exp) override;
  void VisitBlockExpr(const BlockExpr& exp) override;
  void VisitDeclarationExpr(const Declaration& exp) override;
  void VisitAssignmentExpr(const Assignment& assignment) override;
  void VisitConditionalExpr(const Conditional& conditional) override;
  void VisitLoopExpr(const Loop& loop) override;
  void VisitFunctionCallExpr(const FunctionCall& call) override;
  void VisitFunctionDefExpr(const FunctionDef& def) override;
  void VisitProgramExpr(const Program& program) override;

  // You can add more public or private members here

  private:
    std::vector<std::string> output;
    Table symbols;
    int latestoffset = 0;
    int positiveoffset = 4;
    int whilecount = 0;
};

}
