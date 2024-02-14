#include <iostream>

using namespace std;

template<typename T>
struct Node {
    T data;
    Node<T>* next; // pointer to the next node in the list

    Node(T value): data(value), next(nullptr) {}
};

template<typename T>
class LinkedList {
private:
    Node<T>* head;

public:
    LinkedList(T initialValue): head(new Node<T>(initialValue)) {}
    LinkedList(): head(nullptr) {}

    ~LinkedList() {
        while (head != nullptr) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // add element at the end
    void push_back(T value) {
        Node<T>* n = new Node<T>(value);
        if (head == nullptr) {
            head = n;
        } else { // list is not empty, traverse to the end and attach node to current last
           Node<T>* temp = head; 
           while (temp->next != nullptr) {
                temp = temp->next;  
           }
           temp->next = n;
        }
    }

    // delete last element
    void pop_back() {
        if (head == nullptr) {
            // list is empty, nothing to pop
            return;
        } else if (head->next == nullptr) {
            // only 1 element in the list
            delete head;
            head = nullptr;
        } else {
            // traverse to the second-to-last node
            Node<T>* n = head;
            while (n->next->next != nullptr) {
                n = n->next;
            }
            // delete the last node
            delete n->next;
            n->next = nullptr;
        }
    }

    // return the value of the last element in the linked list without removing it
    T back() {
        if (head == nullptr) {
            // list is empty, return default value
            return T();
        } else {
            // traverse to the last node and return it's value
            Node<T>* n = head;
            while (n->next != nullptr) {
                n = n->next;
            }
            return n->data;
        }
    }

    int size() const {
        int count = 0;
        Node<T>* n = head;
        while (n != nullptr) {
            count++;
            n = n->next;
        }
        return count; 
    }
};

template<typename T>
class Stack {
private:
    LinkedList<T> list;

public:
    void push(T value) {
        list.push_back(value);
    }

    void pop() {
        list.pop_back();
    }

    T top() {
        return list.back();
    }

    bool isEmpty() const {
        return list.size() == 0;
    }
};

class Expression {
private: 
    string infix;
    string postfix;
    // 1 -> infix. 2 -> postfix
    int inputDirection;

public:
    Expression(string input, int direction) {
        inputDirection = direction;
        if (direction == 1) {
            infix = input;
        } else if (direction == 2) {
            postfix = input;
        }
    }

    string inToPost() {
        Stack<char> stack;
        string result;

        for (char ch : infix) {
            // if the character is an operand, append it to the result
            if (isalnum(ch)) {
                result += ch;
            } else if (ch == '(') {
                // if the character is a opening parenthesis, push it onto the stack
                stack.push(ch);
            } else if (ch == ')') {
                // if the character is a closing parenthesis, pop and append operators from the stack
                // until an opening parenthesis is encountered
                while (!stack.isEmpty() && stack.top() != '(') {
                    result += stack.top();
                    stack.pop();
                }
                // pop the opening parenthesis from the stack
                stack.pop(); // pop '('
            } else {
                // if the character is an operator, pop and append operators from the stack
                // until the stack is empty or the top operator has lower precedence
                while (
                    !stack.isEmpty() && getPrecedence(ch) < getPrecedence(stack.top()) ||
                    !stack.isEmpty() && getPrecedence(ch) == getPrecedence(stack.top()) &&
                    associativity(ch) == 'L'
                ) 
                {
                    result += stack.top();
                    stack.pop();
                }
                stack.push(ch);
            }
        }

        // pop and append any remaining operators from the stack to the result
        while (!stack.isEmpty()) {
            result += stack.top();
            stack.pop();
        }

        postfix = result;
        
        return result;
    }

    string postToIn() {
        Stack<string> stack;
        string result;

        for (char ch : postfix) {
            if (isalnum(ch)) {
                // if the character is an operand, push it onto the stack
                stack.push(string(1, ch));
            } else {
                // if the character is an operator, pop two operands from the stack,
                // create a sub-expression, and push it back onto the stack
                string operand2 = stack.top();
                stack.pop();
                string operand1 = stack.top();
                stack.pop();
                // build the sub-expression and push it back onto the stack
                string subExpression = "(" + operand1 + ch + operand2 + ")";
                stack.push(subExpression);
            }
        }

        // the final result is at the top of the stack
        result = stack.top();

        infix = result;

        return result;
    }

    double evaluate() {
        Stack<double> stack;

        for (char ch : postfix) {
            if (isdigit(ch)) {
                // if the character is a digit, push its numeric value onto the operand stack
                stack.push(ch - '0');
            } else if (isOperator(ch)) {
                // If the character is an operator, pop two operands from the stack,
                // perform the operation, and push the result back onto the stack
                double operand2 = stack.top();
                stack.pop();

                double operand1 = stack.top();
                stack.pop();

                double result = performOperation(operand1, operand2, ch);
                stack.push(result);
            }
        }

        return stack.top();
    }

    // helper metho to get the precedence of an operator
    int getPrecedence(char op) {
        if (op == '^') {
            return 3;
        } else if (op == '*' || op == '/') {
            return 2;
        } else if (op == '+' || op == '-') {
            return 1;
        } else {
            return -1;
        }
    }

    // function to return associativity of operators
    char associativity(char c) {
        if (c == '^') {
            return 'R';
        }
        return 'L'; // Default to left-associative
    }

    string getInfix() {
        return infix;
    }

    string getPostfix() {
        return postfix;
    }

    int getInputDirection() {
        return inputDirection;
    }

    // helper method to check if a character is an operator
    bool isOperator(char ch) {
        return ch == '+' || ch == '-' || ch == '*' || ch == '/';
    }

    // helper method to perform the arithmetic operation
    double performOperation(double operand1, double operand2, char op) {
        switch (op) {
            case '+':
                return operand1 + operand2;
            case '-':
                return operand1 - operand2;
            case '*':
                return operand1 * operand2;
            case '/':
                if (operand2 != 0) {
                    return operand1 / operand2;
                } else {
                    cerr << "Error: Division by zero." << endl;
                    exit(1);
                }
            default:
                cerr << "Error: Invalid operator." << endl;
                exit(1);
        }
    }

    void displayMenu() {
        cout << "Expression Conversion Menu:\n";
    
        if (getInputDirection() == 1) {
            // If the input is infix, show options for converting to postfix
            cout << "1. Convert Infix to Postfix\n";
        } else if (getInputDirection() == 2) {
            // If the input is postfix, show options for converting to infix
            cout << "1. Convert Postfix to Infix\n";
        }

        // Common options for both infix and postfix
        cout << "2. Evaluate Expression\n";
        cout << "3. Quit\n";
    }

    void processUserInput() {
        int choice;
        do {
            displayMenu();
            cout << "Enter your choice (1-3): ";
            cin >> choice;

            switch (choice) {
            case 1:
                if (getInputDirection() == 1) {
                    inToPost();
                    cout << "Postfix expression: " << getPostfix() << endl;
                } else if (getInputDirection() == 2) {
                    postToIn();
                    cout << "Infix expression: " << getInfix() << endl;
                } else {
                    cout << "Invalid input direction.\n";
                }
                break;
            case 2:
                evaluate();
                cout << "Result: " << evaluate() << endl;
                break;
            case 3:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 4.\n";            
                break;
            }
        } while (choice != 3);
    }
};

int getExpressionDirection() {
    int direction;
    cout << "Enter the direction (1 for infix, 2 for postfix): ";
    cin >> direction;
    return direction;
}

string getInputExpression(int direction) {
    string input;
    if (direction == 1) {
        cout << "Enter an infix expression: ";
    } else if (direction == 2) {
        cout << "Enter a postfix expression: ";
    } else {
        // TODO: Handle the case when the direction is neither 1 nor 2
        cout << "Enter an infix or postfix expression: ";
    }
    cin >> input;
    return input;
}

int main() {
    cout << "Welcome to the Expression Converter and Evaluator!\n";
    int direction = getExpressionDirection();
    string input = getInputExpression(direction);
    Expression exp(input, direction);
    exp.processUserInput();
    return 0;        
}