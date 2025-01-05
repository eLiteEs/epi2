#include "utils.cpp"

bool canBeOperation(String s) {
    for(long long unsigned int i = 0; i < s.length(); i++) {
        if(!isDigit(s.c_str()[i]) && s.c_str()[i] != '+' && s.c_str()[i] != '-' && s.c_str()[i] != '/' && s.c_str()[i] != '*') {
            return false;
        }
    }
    return true;
}
int calculate(const std::string& expression) {
    std::stack<int> numbers;
    std::stack<char> operators;

    std::istringstream iss(expression);

    char currentChar;
    while (iss >> currentChar) {
        if (isdigit(currentChar)) {
            iss.putback(currentChar);
            int number;
            iss >> number;
            numbers.push(number);
        } else if (currentChar == '(') {
            operators.push(currentChar);
        } else if (currentChar == ')') {
            while (!operators.empty() && operators.top() != '(') {
                int operand2 = numbers.top();
                numbers.pop();
                int operand1 = numbers.top();
                numbers.pop();
                char op = operators.top();
                operators.pop();

                if (op == '+') {
                    numbers.push(operand1 + operand2);
                } else if (op == '-') {
                    numbers.push(operand1 - operand2);
                } else if (op == '*') {
                    numbers.push(operand1 * operand2);
                } else if (op == '/') {
                    numbers.push(operand1 / operand2);
                }
            }
            operators.pop();  // Pop '('
        } else if (currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == '/') {
            while (!operators.empty() && (operators.top() == '*' || operators.top() == '/') && currentChar != '(') {
                int operand2 = numbers.top();
                numbers.pop();
                int operand1 = numbers.top();
                numbers.pop();
                char op = operators.top();
                operators.pop();

                if (op == '+') {
                    numbers.push(operand1 + operand2);
                } else if (op == '-') {
                    numbers.push(operand1 - operand2);
                } else if (op == '*') {
                    numbers.push(operand1 * operand2);
                } else if (op == '/') {
                    numbers.push(operand1 / operand2);
                }
            }
            operators.push(currentChar);
        }
    }

    while (!operators.empty()) {
        int operand2 = numbers.top();
        numbers.pop();
        int operand1 = numbers.top();
        numbers.pop();
        char op = operators.top();
        operators.pop();

        if (op == '+') {
            numbers.push(operand1 + operand2);
        } else if (op == '-') {
            numbers.push(operand1 - operand2);
        } else if (op == '*') {
            numbers.push(operand1 * operand2);
        } else if (op == '/') {
            numbers.push(operand1 / operand2);
        }
    }

    return numbers.top();
}
