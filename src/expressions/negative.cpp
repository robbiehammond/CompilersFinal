#include "negative.h"

std::unique_ptr<VarType> ASTExpressionNegation::ReturnType(ASTFunction& func) {
    return nullptr;
}

bool ASTExpressionNegation::IsLValue(ASTFunction& func)
{
    return false; // If we are adding values together, they must be usable R-Values. Adding these together just results in an R-Value.
}

llvm::Value* ASTExpressionNegation::Compile(llvm::IRBuilder<>& builder, ASTFunction& func)
{
    return nullptr;
}

std::string ASTExpressionNegation::ToString(const std::string& prefix)
{
    std::string ret = "(+)\n";
    ret += prefix + "├──" + a1->ToString(prefix + "│  ");
    return ret;
}