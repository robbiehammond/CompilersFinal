#include "int2Bool.h"

std::unique_ptr<VarType> ASTExpressionInt2Bool::ReturnType(ASTFunction& func)
{
    return VarTypeSimple::BoolType.Copy(); // Of course Int2Bool returns a bool what else would it.
}

bool ASTExpressionInt2Bool::IsLValue(ASTFunction& func)
{
    return false; 
}

llvm::Value* ASTExpressionInt2Bool::Compile(llvm::IRBuilder<>& builder, ASTFunction& func)
{
    // Make sure operand is valid int type.
    if (!operand->ReturnType(func)->Equals(&VarTypeSimple::IntType))
        throw std::runtime_error("ERROR: Expected integer operand in int2bool but got another type instead!");

   return builder.CreateICmpNE(operand->CompileRValue(builder, func), llvm::ConstantInt::get(llvm::Type::getInt32Ty(builder.getContext()), 0, true));
}

std::string ASTExpressionInt2Bool::ToString(const std::string& prefix)
{
    return "int2bool\n" + prefix + "└──" + operand->ToString(prefix + "   ");
}