#include "for.h"

#include "../function.h"
#include <iostream>
//This whole file needs to be fixed.

std::unique_ptr<VarType> ASTStatementFor::StatementReturnType(ASTFunction& func)
{
    return voidReturnType != nullptr ? voidReturnType->Copy() : nullptr;
}

void ASTStatementFor::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction& func)
{
    /*
    bool optimize = true;
    if (optimize) {
        if (condition->CompileRValue(builder, func) ==
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(builder.getContext()), 1)) {
            voidReturnType = VarTypeSimple::VoidType.Copy(); //so that the function ends with this block
            auto *funcVal = (llvm::Function *) func.GetVariableValue(func.name);
            auto forLoop = llvm::BasicBlock::Create(builder.getContext(), "forLoop", funcVal);
            auto forLoopEnd = llvm::BasicBlock::Create(builder.getContext(), "forLoopEnd", funcVal);

            init->Compile(builder, func);
            builder.CreateBr(forLoop);

            builder.SetInsertPoint(forLoop);
            stmt->Compile(mod, builder, func);
            builder.CreateBr(forLoop);
            builder.SetInsertPoint(forLoopEnd);
            return;
        }
        else if (condition->CompileRValue(builder, func) ==
                 llvm::ConstantInt::get(llvm::Type::getInt32Ty(builder.getContext()), 0)) {
            return;
        }

    }
    */
    // Create the basic blocks.
    auto* funcVal = (llvm::Function*)func.GetVariableValue(func.name);
    auto forLoop = llvm::BasicBlock::Create(builder.getContext(), "forLoop", funcVal);
    auto forLoopBody = llvm::BasicBlock::Create(builder.getContext(), "forLoopBody", funcVal);
    auto forLoopEnd = llvm::BasicBlock::Create(builder.getContext(), "forLoopEnd", funcVal);


    init->Compile(mod, builder, func);
    builder.CreateBr(forLoop);

    builder.SetInsertPoint(forLoop);
    auto conditionVal = condition->CompileRValue(builder, func);
    builder.CreateCondBr(conditionVal, forLoopBody, forLoopEnd);

    // Compile condition and jump to the right block.


    builder.SetInsertPoint(forLoopBody);

    // Compile the body. Note that we need to not create a jump if there is a return.
    stmt->Compile(mod, builder, func);
    inc->Compile(mod, builder, func);
    if (!stmt->StatementReturnType(func)) builder.CreateBr(forLoop);


    // Continue from the end of the created while loop.
    builder.SetInsertPoint(forLoopEnd);
}

std::string ASTStatementFor::ToString(const std::string& prefix)
{
    std::string output = "for\n";
    output += prefix + "├──" + init->ToString(prefix + "│  ");
    output += prefix + "├──" + condition->ToString(prefix + "│  ");
    output += prefix + "├──" + inc->ToString(prefix + "│  ");
    output += prefix + "└──" + stmt->ToString(prefix + "   ");
    return output;
}

bool ASTStatementFor::CanOptimize(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction& func) {
    try {
        if (condition->CompileRValue(builder, func) ==
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(builder.getContext()), 0)
            || condition->CompileRValue(builder, func) ==
               llvm::ConstantInt::get(llvm::Type::getInt32Ty(builder.getContext()), 1)) {
            return true;
        }
        else
            return false;
    }
    //if we can't resolve a variable or anything, just say we can't optimize.
    catch (std::runtime_error& e) {
        return false;
    }
}

Optimization ASTStatementFor::howToOptimize(llvm::Module &mod, llvm::IRBuilder<> &builder, ASTFunction &func) {
    if (condition->CompileRValue(builder, func) == llvm::ConstantInt::get(llvm::Type::getInt32Ty(builder.getContext()), 0)) {
        return REMOVE_LOOP;
    }
    else if (condition->CompileRValue(builder, func) == llvm::ConstantInt::get(llvm::Type::getInt32Ty(builder.getContext()), 1)) {
        return REMOVE_POST_LOOP;
    }
    else {
        return NO_OPTIM;
    }

}




