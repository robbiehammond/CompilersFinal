#pragma once

#include "function.h"
#include "scopeTable.h"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

// Abstract Syntax Tree, is the main representation of our program.
class AST
{

    // Main LLVM context. Note: If we want support for multiple compilation units, this should be moved elsewhere.
    llvm::LLVMContext context;

    // Module containing all functions.
    llvm::Module module;

    // Builder to build IR code in functions.
    llvm::IRBuilder<> builder;

    // List of functions to be compiled in order.
    std::vector<std::string> functionList;

    // Map function names to values.
    std::map<std::string, std::unique_ptr<ASTFunction>> functions;

    // If the module has been compiled or not.
    bool compiled = false;

    void DeadCodeRemoval(std::unique_ptr<ASTFunction> f);

    void KillUselessBlocks(std::unique_ptr<ASTFunction> func);

    void KillPostReturn(std::unique_ptr<ASTFunction> func);

    void KillInaccessibleIf(std::unique_ptr<ASTFunction> func);

    void KillInaccessibleElse(std::unique_ptr<ASTFunction> func);

    void KillInaccessiblePostWhile(std::unique_ptr<ASTFunction> func);

    void KillINaccessibleWhile(std::unique_ptr<ASTFunction> func);

    void KillUselessVars(std::unique_ptr<ASTFunction> func);

public:

    // Function pass manager for function optimizations.
    llvm::legacy::FunctionPassManager fpm;

    // Scope table for variables and functions.
    ScopeTable scopeTable;

    // Create a new abstract syntax tree.
    // modName: Name of the module to create.
    AST(std::string modName);

    // Add a function to the AST.
    // name: Name of the function to create.
    // returnType: The type of variable the function will return.
    // parameters: Collection of variable types and names to pass to the function call.
    // variadic: If the function is a variadic function.
    // Returns: A pointer to the newly added function.
    ASTFunction* AddFunction(const std::string& name, std::unique_ptr<VarType> returnType, ASTFunctionParameters parameters, bool variadic = false);

    // Get a function from a name.
    // name: Name of the function to fetch.
    // Returns: A pointer to the function. Throws an exception if it does not exist.
    ASTFunction* GetFunction(const std::string& name);

    // Compile the AST. This must be done before exporting any object files.
    void Compile();

    // Get a string representation of the AST.
    std::string ToString();

    // Write LLVM assembly (.ll) to file. Must be done after compilation.
    // outFile: Where to write the .ll file.
    void WriteLLVMAssemblyToFile(const std::string& outFile);

    // Write LLVM bitcode (.bc) to file. Must be done after compilation.
    // outFile: Where to write the .bc file.
    void WriteLLVMBitcodeToFile(const std::string& outFile);

    void Optimize();
};