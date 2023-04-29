#include "block.h"

#include "../function.h"
#include "../types/simple.h"

#include <iostream>


//PROBLEM IDENTIFIED 
//AN EMPTY STATEMENT IS BEING PUSHED TO THE END FOR SOME REASON. SEGFAULT WHEN ACCESSED.
std::unique_ptr<VarType> ASTStatementBlock::StatementReturnType(ASTFunction& func)
{

    // This one is more interesting.
    // If we come across a statement that returns something, then we simply just return it.
    // Otherwise, we return nothing since we made it to the end.

    // Go through each statement.

    for (int i = 0; i < statements.size(); i++)
    {
        auto& statement = statements[i];
        if (statement) {
            auto ret = statement->StatementReturnType(func);
            if (ret) return std::move(ret);
        }
    }

    // Made it through the end, return nothing.
    return nullptr;

}

void ASTStatementBlock::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction& func)
{

    // Pretty much just keep compiling until we find a return value. We don't want to keep compiling past any return statements.
    for (auto& statement : statements)
    {
        statement->Compile(mod, builder, func);
        if (statement->StatementReturnType(func)) return;
    }
}

bool ASTStatementBlock::CanOptimize(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction& func) {
    for (int i = 0; i < statements.size(); i++) {
        auto& statement = statements[i];
        if (statement->CanOptimize(mod, builder, func)) {

            Optimization opt = statement->howToOptimize(mod, builder, func);
            if (opt == REMOVE_LOOP) {
                statements.erase(statements.begin() + i);
            }
            else if (opt == REMOVE_POST_LOOP) {

            }
            else if (opt == REMOVE_THEN) {

            }
            else if (opt == REMOVE_ELSE) {

            }
            /*
             * TODO: Change return of CanOptimize to be an enum containing a number of things to indicate what to do
             * (i.e. erase inner, erase after, etc).
             */
            // continue to insert code to replace FOR block
            //statements.erase(statements.begin() + i);
            //statements.push_back(std::unique_ptr<ASTStatement>(new ASTStatementBlock()));
            //std::cout << "remove me"  << std::endl;
        }
    }
}

std::string ASTStatementBlock::ToString(const std::string& prefix)
{
    std::string output = "block\n";
    if(statements.size() > 0) {
      for (int i = 0; i < statements.size() - 1; i++)
        output += prefix + "├──" + (statements.at(i) == nullptr ? "nullptr\n" : statements.at(i)->ToString(prefix + "│  "));
      output += prefix + "└──" + (statements.back() == nullptr ? "nullptr\n" : statements.back()->ToString(prefix + "   "));
    }
    return output;
}
