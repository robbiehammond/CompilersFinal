#include "while.h"

#include "../function.h"

std::unique_ptr<VarType> ASTStatementWhile::StatementReturnType(ASTFunction& func)
{

    return voidReturnType != nullptr ? voidReturnType->Copy() : nullptr;
    // It is completely possible for a while's condition to never be true, so even if does return something it's not confirmed.
    //return nullptr;

}

void ASTStatementWhile::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction& func)
{
    /*
    bool optimize = true;
    if (optimize) {
        //if always true, don't both with anything afterwards.
         * TODO: check if the function is void first and only optimize void ones so we don't need to fuck with types.
         * If we have time, we can make it so the return type is just something that matches the return of the function.
         * So if it's int, just return 0 or something. If it's bool, just return false, etc. It'll never actually be
         * returned, so it doesn't matter.
        if (condition->CompileRValue(builder, func) ==
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(builder.getContext()), 1)) {
            voidReturnType = VarTypeSimple::VoidType.Copy(); //so that the function ends with this block
            auto* funcVal = (llvm::Function*)func.GetVariableValue(func.name);
            auto whileLoop = llvm::BasicBlock::Create(builder.getContext(), "whileLoop", funcVal);
            auto whileLoopEnd = llvm::BasicBlock::Create(builder.getContext(), "whileLoopEnd", funcVal);
            builder.CreateBr(whileLoop);
            builder.SetInsertPoint(whileLoop);
            thenStatement->Compile(mod, builder, func);
            builder.CreateBr(whileLoop);
            builder.SetInsertPoint(whileLoopEnd);
            return;
        }

        //if always false, don't generate the code inside or the loop at all.
        else if (condition->CompileRValue(builder, func) ==
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(builder.getContext()), 0)) {
            return;
        }
    }
    */

    /*

        A while loop can be desugared to basic blocks. Take the following example:

            while (condition)
            {
                doThing();
            }

        This is really just another way of saying:

            whileLoop:
                if (condition) goto whileLoopBody else goto whileLoopEnd;

            whileLoopBody:
                doThing();
                goto whileLoop;

            whileLoopEnd:
                ...

    */

    // Create the basic blocks.
    auto* funcVal = (llvm::Function*)func.GetVariableValue(func.name);
    auto whileLoop = llvm::BasicBlock::Create(builder.getContext(), "whileLoop", funcVal);
    auto whileLoopBody = llvm::BasicBlock::Create(builder.getContext(), "whileLoopBody", funcVal);
    auto whileLoopEnd = llvm::BasicBlock::Create(builder.getContext(), "whileLoopEnd", funcVal);

    // Jump to the while loop.
    builder.CreateBr(whileLoop);

    // Compile condition and jump to the right block.
    builder.SetInsertPoint(whileLoop);
    auto conditionVal = condition->CompileRValue(builder, func);
    builder.CreateCondBr(conditionVal, whileLoopBody, whileLoopEnd);

    // Compile the body. Note that we need to not create a jump if there is a return.
    builder.SetInsertPoint(whileLoopBody);
    thenStatement->Compile(mod, builder, func);
    if (!thenStatement->StatementReturnType(func)) builder.CreateBr(whileLoop);

    // Continue from the end of the created while loop.
    builder.SetInsertPoint(whileLoopEnd);

}

std::string ASTStatementWhile::ToString(const std::string& prefix)
{
    std::string output = "while\n";
    output += prefix + "├──" + condition->ToString(prefix + "│  ");
    output += prefix + "└──" + thenStatement->ToString(prefix + "   ");
    return output;
}

bool ASTStatementWhile::CanOptimize(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction& func) {
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

Optimization ASTStatementWhile::HowToOptimize(llvm::Module &mod, llvm::IRBuilder<> &builder, ASTFunction &func) {
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
