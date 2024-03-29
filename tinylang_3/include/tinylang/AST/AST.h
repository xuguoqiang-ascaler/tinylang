#ifndef TINYLANG_AST_AST_H
#define TINYLANG_AST_AST_H

#include "tinylang/Basic/LLVM.h"
#include "tinylang/Basic/TokenKinds.h"
#include "llvm/ADT/APSInt.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/SMLoc.h"

#include <string>
#include <vector>

namespace tinylang {

class Decl;
class FormalParamaterDeclaration;
class Expr;
class Stmt;

using DeclList = std::vector<Decl *>;
using FormalParamList = std::vector<FormalParamaterDeclaration *>;
using ExprList = std::vector<Expr *>;
using StmtList = std::vector<Stmt *>;

class Ident {

    SMLoc Loc;
    StringRef Name;

public:
    Ident(SMLoc Loc, const StringRef &Name) : Loc(Loc), Name(Name) {}
    SMLoc getLocation() { return Loc; }
    const StringRef &getName() { return Name; }
};

using IdentList = std::vector<std::pair<SMLoc, StringRef>>;

class Decl {

public:
    enum DeclKind {
        DK_Module,
        DK_Const,
        DK_Type,
        DK_Var,
        DK_Param,
        DK_Proc
    };

private:
    const DeclKind Kind;

protected:
    Decl *EnclosingDecL;
    SMLoc Loc;
    StringRef Name;

public:
    Decl(DeclKind Kind, Decl *EnclosingDecL, SMLoc Loc, StringRef Name) :
    Kind(Kind), EnclosingDecL(EnclosingDecL), Loc(Loc), Name(Name) {}
    
    DeclKind getKind() const { return Kind; } 
    SMLoc getLocation() { return Loc; }
    StringRef getName() { return Name; }
    Decl *getEnclosingDecl() { return EnclosingDecL; }
};

class ModuleDeclaration : public Decl {
    DeclList Decls;
    StmtList Stmts;

public:
    ModuleDeclaration(Decl *EnclosingDecL, SMLoc Loc, StringRef Name) :
    Decl(DK_Module, EnclosingDecL, Loc, Name) {}

    ModuleDeclaration(Decl *EnclosingDecL, SMLoc Loc, StringRef Name, 
                      DeclList &Decls, StmtList &Stmts) :
    Decl(DK_Module, EnclosingDecL, Loc, Name),
    Decls(Decls), Stmts(Stmts) {} 

    const DeclList &getDecls() { return Decls; }
    void setDecls(DeclList &D) { Decls = D; }
    const StmtList &getStmts() { return Stmts; }
    void setStmts(StmtList &L) { Stmts = L; }

    static bool classof(const Decl *D) {
        return D->getKind() == DK_Module;
    }
};

class ConstantDeclaration : public Decl {

    Expr *E;

public:
    ConstantDeclaration(Decl *EnclosingDecL, SMLoc Loc, StringRef Name, Expr *E) :
    Decl(DK_Type, EnclosingDecL, Loc, Name), E(E) {}

    Expr *getExpr() { return E; }

    static bool classof(const Decl *D) {
        return D->getKind() == DK_Const;
    }
};

class TypeDeclaration : public Decl {
public:
    TypeDeclaration(Decl *EnclosingDecL, SMLoc Loc, StringRef Name) :
    Decl(DK_Type, EnclosingDecL, Loc, Name) {}

    static bool classof(const Decl *D) {
        return D->getKind() == DK_Type;
    }
};

class VariableDeclaration : public Decl {

    TypeDeclaration *Ty;

public:
    VariableDeclaration(Decl *EnclosingDecL, SMLoc Loc, StringRef Name, TypeDecalaration *Ty) :
    Decl(DK_Var, EnclosingDecL, Loc, Name), Ty(Ty) {}

    TypeDeclaration *getType() { return Ty; }
    static bool classof(const Decl *D) {
        return D->getKind() == DK_Var;
    }
};

class FormalParamaterDeclaration : public Decl {
    TypeDecalaration *Ty;
    bool IsVar;

public:
    FormalParamaterDeclaration(Decl *EnclosingDecL, SMLoc Loc, StringRef Name,
                               TypeDeclaration *Ty, bool IsVar) : 
    Decl(DK_Param, EnclosingDecL, Loc, Name), Ty(Ty), IsVar(IsVar) {}

    TypeDeclaration *getType() { return Ty; }
    bool isVar() { return IsVar; } 

    static bool classof(const Decl *D) {
        return D->getKind() == DK_Param;
    }
};


class ProcedureDeclaration : public Decl {
    FormalParamList Params;
    TypeDecalaration *RetType;
    DecList Decls;
    StmtList Stmts;

public:
    ProcedureDeclaration(Decl *EnclosingDecL, SMLoc Loc, StringRef Name) :
    Decl(DK_Proc, EnclosingDecL, Loc, Name) {}

    ProcedureDeclaration(Decl *EnclosingDecL, SMLoc Loc, StringRef Name,
                         FormalParamList &Params, TypeDecalaration *RetType,
                         DeclList &Decls, StmtList &Stmts) : 
    Decl(DK_Proc, EnclosingDecL, Loc, Name), Params(Params), RetType(RetType),
    Decls(Decls), Stmts(Stmts) {}

    const FormalParamList &getFormalParams() { return Params; }
    void setFormalParams(FormalParamList &FP) { Params = FP; }
    TypeDeclaration *getRetType() { return RetType; }
    void setRetType(TypeDeclaration *Ty) { RetType = Ty; }

    const DeclList &getDecls() { return Decls; }
    void setDecls(DeclList &D) { Decls = D; }
    const StmtsList &getStmts() { return Stmts; }
    void setStmts(StmtList &L) { Stmts = L; }

    static bool classof(const Decl *D) {
        return D->getKind() == DK_Proc;
    }
};

class OperationInfo {
    SMLoc Loc;
    uint32_t Kind : 16;
    uint32_t IsUnspecified: 1;

public:
    OperatorInfo() :
    Loc(), Kind(tok::unknown), IsUnspecified(true) {}
    OperatorInfo(SMLoc Loc, tok::TokenKind Kind, bool IsUnspecified = false) :
    Loc(Loc), Kind(Kind), IsUnspecified(IsUnspecified) {}

    SMLoc getLocation() const { return Loc; }
    tok::TokenKind getKind() const {
        return static_cast<tok::TokenKind>(Kind);
    }
    bool isUnspecified() const { return IsUnspecified; }
};

class Expr {

public:
    enum ExprKind {
        EK_Infix,
        EK_Prefix,
        EK_Int,
        EK_Bool,
        EK_Var,
        EK_Const,
        EK_Func
    }; 

private:
    const ExprKind Kind;
    TypeDeclaration *Ty;
    bool IsConstant;

protected:
    Expr(ExprKind Kind, TypeDecalaration *Ty, bool IsConst) :
    Kind(Kind), Ty(Ty), IsConstant(IsConst) {}

public:
    ExprKind getKind() const { return Kind; }
    TypeDeclaration *getType() { return Ty; } 
    void setType(TypeDeclaration *T) { Ty  = T; }
    bool isConst() { return IsConstant; }
};

class InfixExpression : public Expr {
    Expr *Left;
    Expr *Right;
    const OperationInfo Op;
public:
    InfixExpression(Expr *Left, Expr *Right, OperatorInfo Op,
                    TypeDeclaration *Ty, bool IsConst) :
    Expr(EK_Infix, Ty, IsConst), Left(Left), Right(Right), Op(Op) {}

    Expr *getLeft() { return Left; }
    Expr *getRight() { return Right; }
    const OperationInfo &getOperation() { return Op; }

    static bool classof(const Expr *E) {
        return E->getKind() == EK_Infix;
    }
};

class PrefixExpression : public Expr {
    Expr *E;
    const OperatorInfo Op;

public:
    PrefixExpression(Expr *E, OperatorInfo Op, TypeDecalaration *Ty, bool IsConst) :
    Expr(EK_Prefix, Ty, IsConst), E(E), Op(Op) {};

    Expr *getExpr() { return E; }
    const OperationInfo &getOperationInfo() { return op; }
    static bool classof(const Expr *E) {
        return E->getKind() == EK_Prefix;
    }
};

class IntegerLiteral : public Expr {
    SMLoc Loc;
    llvm::APSInt Value;

public:
    IntegerLiteral(SMLoc Loc, const llvm::APSInt &Value, TypeDeclaration *Ty) :
    Expr(EK_Int, Ty, true), Loc(Loc), Value(Value) {}

    llvm::APSInt &getValue() { return Value; }

    static bool classof(const Expr *E) {
        return E->getKind() == EK_Int;
    }
};

class BooleanLiteral : public Expr {
    bool Value;
public:
    BooleanLiteral(bool Value, TypeDecalaration *Ty) :
    Expr(EK_Bool, Ty, true), Value(Value) {}

    bool getValue() { return Value; }
    static bool classof(const Expr *E) {
        return E->getKind() == EK_Bool;
    }
};

class VariableAccess : public Expr {
    Decl *Var;
public:
    VariableAccess(VariableDeclaration *Var) :
    Expr(EK_Var, Var->getType(), false), Var(Var) {}
    VariableAccess(FormalParamaterDeclaration *Param) :
    Expr(EK_Var, Param->getType(), false), Var(Param) {}

    Decl *getDecl() { return Var; }
    static bool classof(const Expr *E) {
        return E->getKind() == EK_Var;
    }
};

class ConstantAccess : public Expr {
    ContantDeclaration *Const;

public:
    ConstantAccess(ConstantDeclaration *Const) :
    Expr(EK_Const, Const->getExpr()->getType(), true), Const(Const) {}

    ConstantDeclaration *getDecl() { return Const; }

    static bool classof(const Expr *E) {
        return E->getKind() == EK_Const;
    }
};

class FunctionCallExpr : public Expr {
    ProcedureDeclaration *Proc;
    ExprList Params;

public:
    FunctionCallExpr(ProcedureDeclaration *Proc, ExprList Params) :
    Expr(EK_Func, Proc->getRetType(), false), Proc(Proc), Params(Params) {}

    ProcedureDeclaration *getDecl() { return Proc; }
    const ExprList &getParams() { return Params; }

    static bool classof(const Expr *E) {
        return E->getKind() == EK_Func;
    }
};

class Stmt {
public:
    enum StmtKind {
        SK_Assign,
        SK_ProcCall,
        SK_If,
        SK_While,
        SK_Return
    };

private:
    const StmtKind Kind;

protected:
    Stmt(StmtKind Kind) : Kind(Kind) {}

public:
    StmtKind getKind() const { return Kind; }
};

class AssignmentStatement : public Stmt {\

    VariableDeclaration *Var;
    Expr *E;

public:
    AssignmentStatement(VariableDeclaration *Var, Expr *E) :
    Stmt(SK_Assign), Var(Var), E(E) {} 

    VariableDeclaration *getVar() { return Var; }
    Expr *getExpr() { return E; }

    static bool classof(const Stmt *S) {
        return S->getKind() == SK_Assign;
    }
};

class ProcedureCallStatement : public Stmt {
    ProcedureDeclaration *Proc;
    ExprList Params;

public:
    ProcedureDeclaration *getProc() { return Proc; }
    const ExprList &getParams() { return Params; }

    static bool classof(const Stmt *S) {
        return S->getKind() == SK_ProcCall;
    }
};

class IfStatement : public Stmt {
    Expr *Cond;
    StmtList IfStmts;
    StmtList ElseStmts;
    
public:
    IfStatement(Expr *Cond, StmtList &IfStmts, StmtList &ElseStmts) :
    Stmt(SK_If), Cond(Cond), IfStmts(IfStmts), ElseStmts(ElseStmts) {}

    Expr *getCond() { return Cond; }
    const StmtList &getIfStmts() { return IfStmts; }
    const StmtList &getElseStmts() { return ElseStmts; } 

    static bool classof(const Stmt *S) {
        return S->getKind() == SK_If;
    }
};

class WhiteStatement : public Stmt {
    Expr *Cond;
    StmtList Stmts;

public:
    WhileStatement(Expr *Cond, StmtList &Stmts) :
    Stmt(SK_While), Cond(Cond), Stmts(Stmts) {}

    Expr *getCond() { return Cond; }
    const StmtList &getWhileStmts() { return Stmts; }

    static bool classof(const Stmt *S) {
        return S->getKind() == SK_While;
    }
};

class ReturnStatement : public Stmt {
    Expr *RetVal;

public:
    ReturnStatement(Expr *RetVal) :
    Stmt(SK_Return), RetVal(RetVal) {}

    Expr *getRetVal() { return RetVal; }

    static bool classof(const Stmt *S) {
        return S->getKind() == SK_Return;
    }
};

} // namespace tinylang
#endif
