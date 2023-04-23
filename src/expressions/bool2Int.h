#pragma once

#include "../expression.h"

class ASTExpressionBool2Int : public ASTExpression 
{
    std::unique_ptr<ASTExpression> operand; 
public: 
    explicit ASTExpressionBool2Int(std::unique_ptr<ASTExpression> operand) : operand(std::move(operand)) {};

    static auto Create(std::unique_ptr<ASTExpression> operand)
    {
        return std::make_unique<ASTExpressionBool2Int>(std::move(operand));
    }

    std::unique_ptr<VarType> ReturnType(ASTFunction& func) override;
    bool IsLValue(ASTFunction& func) override;
    llvm::Value* Compile(llvm::IRBuilder<>& builder, ASTFunction& func) override;
    std::string ToString(const std::string& prefix) override;
};