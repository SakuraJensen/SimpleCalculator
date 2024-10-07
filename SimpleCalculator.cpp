// SimpleCalculator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// Copyright Sakura Jensen 2024 - Portfolio Collection

// THIS WAS SUPPOSED TO BE SIMPLE! @_@

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <map>
#include <stack>


std::string sanitize_input(const std::string& input) {
    std::string sanitized;
    for (char c : input) {
        if (isdigit(c) || c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')') {
            sanitized += c;
        }
    }
    return sanitized;
}

std::vector<std::string> tokenize(const std::string& expr) {
    std::vector<std::string> tokens;
    std::string current;

    for (char c : expr) {
        if (isdigit(c)) {
            current += c;
        }
        else {
            if (!current.empty()) {
                tokens.push_back(current); // Push the number as token.
                current.clear();
            }
            tokens.push_back(std::string(1, c)); // Push the operator as token.
        }
    }
    if (!current.empty()) {
        tokens.push_back(current); // Push the last number as Token if not operator and ensure current has been emptied.
    }
    return tokens;
}

// Map to define operator precedence
std::map<char, int> precedence = {
    {'+', 1}, {'-', 1},  // '+' and '-' have the lowest precedence
    {'*', 2}, {'/', 2}   // '*' and '/' have higher precedence
};

std::vector<std::string> shunting_yard(const std::vector<std::string>& tokens) {
    std::vector<std::string> output;
    std::stack<char> operators;

    // Iterate through each token, do order of op magic.
    for (const std::string& token : tokens) {
        if (isdigit(token[0])) {
            output.push_back(token);
        }
        else if (token[0] == '(') {
            operators.push('(');
        }
        else if (token[0] == ')') {

            while (!operators.empty() && operators.top() != '(') {
                output.push_back(std::string(1, operators.top()));  // Add the operator to output
                operators.pop();  // Remove the operator from the stack
            }
            operators.pop();
        }
        // Token is an operator ('+', '-', '*', '/')
        else {
            // While there is an operator on the stack with greater or equal precedence, pop it to the output
            while (!operators.empty() && precedence[operators.top()] >= precedence[token[0]]) {
                output.push_back(std::string(1, operators.top()));  // Add operator to output
                operators.pop();  // Remove the operator from the stack
            }
            // Push the current operator onto the stack
            operators.push(token[0]);
        }
    }

    // Pop remaining to output.
    while (!operators.empty()) {
        output.push_back(std::string(1, operators.top()));
        operators.pop();
    }

    return output;
}

int evaluate_postfix(const std::vector<std::string>& postfix) {
    std::stack<int> stack;  // Stack to hold operands during evaluation

    // Iterate through each token in the postfix expression
    for (const std::string& token : postfix) {
        // If the token is a number, push it onto the stack
        if (isdigit(token[0])) {
            stack.push(std::stoi(token));  // Convert the string to an integer and push it
        }
        // If the token is an operator, pop two operands from the stack and apply the operator
        else {
            // Pop the top two numbers from the stack
            int b = stack.top(); stack.pop();
            int a = stack.top(); stack.pop();

            // Apply the operator and push the result back onto the stack
            if (token == "+") stack.push(a + b);  // Add
            if (token == "-") stack.push(a - b);  // Subtract
            if (token == "*") stack.push(a * b);  // Multiply
            if (token == "/") stack.push(a / b);  // Divide
        }
    }

    // This is the result
    return stack.top();
}

int main()
{
	std::string userInput = "";
    std::string cleanInput = "";

    // Expression Input
	std::cout << "Enter Equation: ";
	std::getline(std::cin, userInput);
    std::cout << " \n\n"; // White space formatting

    // Santitization
    std::cout << "Unsanitized User Input: " + userInput + " \n";
    cleanInput = sanitize_input(userInput);
    std::cout << "Sanitized User Input: " + cleanInput + " \n\n";

    // Tokenization
    std::vector<std::string> tokenizedInput = tokenize(cleanInput);

    for (int i = 0; i < tokenizedInput.size(); i++) {
        std::cout << "Tokenized Input: " +  tokenizedInput[i] << " \n";
        
        // Formatting: Will add new lines only on last token.
        if (i == tokenizedInput.size()) {
            std::cout << " \n\n";
        }
    }
    
    // Solving the equation
    std::vector<std::string> postfixEquation = shunting_yard(tokenizedInput);
    int result = evaluate_postfix(postfixEquation);
    std::string resultString = std::to_string(result);

    std::cout << "Your Result is: " + resultString;
    std::cout << " \n\n";


    // Boring Exit
    return 0;
}
