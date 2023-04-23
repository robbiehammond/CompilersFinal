#pragma once

#include "../expression.h"
#include "../statement.h"

class ASTStatementFor : public ASTStatement
{
    std::unique_ptr<ASTExpression> init;
    std::unique_ptr<ASTExpression> condition;
    std::unique_ptr<ASTExpression> inc;
    std::unique_ptr<ASTStatement> stmt;

public:
    ASTStatementFor(std::unique_ptr<ASTExpression> init, std::unique_ptr<ASTExpression> condition, std::unique_ptr<ASTExpression> inc, std::unique_ptr<ASTStatement> stmt)
    : init(std::move(init)), condition(std::move(condition)), inc(std::move(inc)), stmt(std::move(stmt)) {}

    static auto Create(std::unique_ptr<ASTExpression> init, std::unique_ptr<ASTExpression> condition, std::unique_ptr<ASTExpression> inc, std::unique_ptr<ASTExpression> stmt)
    {
        return std::make_unique<ASTStatementFor>(std::move(init), std::move(condition), std::move(inc), std::move(stmt));
    }

    virtual std::unique_ptr<VarType> StatementReturnType(ASTFunction& func) override;
    virtual void Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction& func) override;
    virtual std::string ToString(const std::string& prefix) override;
};  