# Compilers design final

## Things To Do / Things Done
<ul>
    <li>Remove code after outer-block return (done automatically ig) ✅</li>
    <li>Remove code in if statement if always evaluates to false ✅</li>
    <li>Remove code in else statement if always evaluates to true ✅</li>
    <li>Remove code after infinite while loops that will never be reached ✅</li>
    <li>Remove code in while loop where the condition always evaluates to false ✅</li>
    <li>Remove code after infinite for loops that will never be reached ✅</li>
    <li>Remove code in for loop where the condition always evaluates to false ✅</li>
    <li>Remove variables that are declared but never used/given a value ❌</li>
    <li>Delete empty functions entirely, remove any corresponding calls ❌</li>
    <li>Clean up code, make optimizations easy to turn on and off❌</li>
</ul>