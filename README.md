This is a simple PEMDAS capable calculator for my portfolio.
It will take any basic expression(advanced mathematics such as cos, sin, sqrt, etc are not available) and solve it.

I've tried to illustrate how the data is being manipulated as the calculator is doing it's work.


How does it work?

1. The program takes the expression as input
2. Santitizes the expression 
3. Tokenizes the expression so that it can be worked with
4. Using a map of operator precedence and a shunting yard algoritm it will them reorder the tokenized expression so that it can be solved according to PEMDAS.
5. Now the program will evaluate(solve) the expression and print the result.
