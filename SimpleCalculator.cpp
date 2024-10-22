#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <map>
#include <stack>

std::string sanitize_input(const std::string& input) {
    std::string sanitized;
    for (char c : input) {
        if (isdigit(c) || c == '.' || c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')') {
            sanitized += c;
        }
    }
    return sanitized;
}

std::vector<std::string> tokenize(const std::string& expr) {
    std::vector<std::string> tokens;
    std::string current;

    for (size_t i = 0; i < expr.size(); ++i) {
        char c = expr[i];

        if (isdigit(c) || c == '.') {
            current += c;  // Handle multi-digit numbers and decimals
        }
        else {
            if (!current.empty()) {
                tokens.push_back(current);  // Push the number token
                current.clear();
            }

            // Check for implicit multiplication: number followed by '(' or ')' followed by a number
            if (c == '(' && !tokens.empty() && (isdigit(tokens.back().back()) || tokens.back() == ")")) {
                tokens.push_back("*");
            }
            else if (c == ')' && i + 1 < expr.size() && (isdigit(expr[i + 1]) || expr[i + 1] == '(')) {
                tokens.push_back(std::string(1, c));  // Push closing parenthesis
                tokens.push_back("*");  // Insert multiplication
                continue;  // Move to the next character
            }

            tokens.push_back(std::string(1, c));  // Push the operator or parenthesis
        }
    }

    if (!current.empty()) {
        tokens.push_back(current);  // Push the last number token
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

    for (const std::string& token : tokens) {
        if (isdigit(token[0]) || token[0] == '.') {
            output.push_back(token);  // Numbers, including floats, go directly to output
        }
        else if (token[0] == '(') {
            operators.push('(');
        }
        else if (token[0] == ')') {
            while (!operators.empty() && operators.top() != '(') {
                output.push_back(std::string(1, operators.top()));  // Add operator to output
                operators.pop();
            }
            operators.pop();  // Pop the '(' from the stack
        }
        else {
            while (!operators.empty() && precedence[operators.top()] >= precedence[token[0]]) {
                output.push_back(std::string(1, operators.top()));  // Add operator to output
                operators.pop();
            }
            operators.push(token[0]);  // Push the current operator onto the stack
        }
    }

    // Pop remaining operators to output
    while (!operators.empty()) {
        output.push_back(std::string(1, operators.top()));
        operators.pop();
    }

    return output;
}

double evaluate_postfix(const std::vector<std::string>& postfix) {
    std::stack<double> stack;  // Stack to hold operands during evaluation

    for (const std::string& token : postfix) {
        if (isdigit(token[0]) || token[0] == '.') {
            stack.push(std::stod(token));  // Convert to double for float support
        }
        else {
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();

            // Apply the operator and push the result
            if (token == "+") stack.push(a + b);
            if (token == "-") stack.push(a - b);
            if (token == "*") stack.push(a * b);
            if (token == "/") stack.push(a / b);
        }
    }

    return stack.top();  // The final result is the only value left on the stack
}

int main() {
    std::string userInput = "";
    std::string cleanInput = "";

    // Expression Input
    std::cout << "Enter Equation: ";
    std::getline(std::cin, userInput);
    std::cout << " \n\n"; // White space formatting

    // Sanitization
    std::cout << "Unsanitized User Input: " + userInput + " \n";
    cleanInput = sanitize_input(userInput);
    std::cout << "Sanitized User Input: " + cleanInput + " \n\n";

    // Tokenization
    std::vector<std::string> tokenizedInput = tokenize(cleanInput);

    for (int i = 0; i < tokenizedInput.size(); i++) {
        std::cout << "Tokenized Input: " + tokenizedInput[i] << " \n";

        // Formatting: Ensure new lines only after the last token
        if (i == tokenizedInput.size() - 1) {
            std::cout << " \n\n";
        }
    }

    // Solving the equation
    std::vector<std::string> postfixEquation = shunting_yard(tokenizedInput);
    double result = evaluate_postfix(postfixEquation);
    std::string resultString = std::to_string(result);

    std::cout << "Your Result is: " + resultString;
    std::cout << " \n\n";

    return 0;
}
