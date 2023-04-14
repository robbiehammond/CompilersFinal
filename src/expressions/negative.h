#pragma once

#include "../expression.h"

class ASTExpressionNegation : public ASTExpression
{
    // Operands to work with.
    std::unique_ptr<ASTExpression> a1;

    // Return type to cache.
    VarTypeSimple* returnType = nullptr;
public:
    ASTExpressionNegation(std::unique_ptr<ASTExpression> a1) : a1(std::move(a1)) {}

    static auto Create(std::unique_ptr<ASTExpression> a1)
    {
        return std::make_unique<ASTExpressionNegation>(std::move(a1));
    }

    // Virtual functions. See base class for details.
    std::unique_ptr<VarType> ReturnType(ASTFunction& func) override;
    bool IsLValue(ASTFunction& func) override;
    llvm::Value* Compile(llvm::IRBuilder<>& builder, ASTFunction& func) override;
    std::string ToString(const std::string& prefix) override;
};