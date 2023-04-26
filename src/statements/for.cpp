#include "for.h"

#include "../function.h"
//This whole file needs to be fixed.

std::unique_ptr<VarType> ASTStatementFor::StatementReturnType(ASTFunction& func)
{
    return nullptr;
}

void ASTStatementFor::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction& func)
{

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