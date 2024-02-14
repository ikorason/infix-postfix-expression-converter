#include <iostream>

using namespace std;

struct ListNode {
    char data;
    ListNode* next; // pointer to the next node in the list

    ListNode(char value): data(value), next(nullptr) {}
};

// linked list
class LinkedList {
private:
    ListNode* head;

public:
    LinkedList(char initialValue): head(new ListNode(initialValue)) {}
    LinkedList(): head(nullptr) {}

    ~LinkedList() {
        while (head != nullptr) {
            ListNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // allocate new list node with initial value `value` and append to front
    void push_front(int value) {
        ListNode* n = new ListNode(value);
        n->next = head;
        head = n;
    }

    // add element at the end
    void push_back(int value) {
        ListNode* n = new ListNode(value);
        if (head == nullptr) {
            head = n;
        } else { // list is not empty, traverse to the end and attach node to current last
           ListNode* temp = head; 
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
            ListNode* n = head;
            while (n->next->next != nullptr) {
                n = n->next;
            }
            // delete the last node
            delete n->next;
            n->next = nullptr;
        }
    }

    char peek() {
        if (head == nullptr) {
            // list is empty, return default value
            return '\0';
        } else {
            // traverse to the last node and return it's value
            ListNode* n = head;
            while (n->next != nullptr) {
                n = n->next;
            }
            return n->data;
        }
    }

    int size() const {
        int count = 0;
        ListNode* n = head;
        while (n != nullptr) {
            count++;
            n = n->next;
        }
        return count; 
    }
};

class Stack {
private:
    LinkedList list;

public:
    void push(char value) {
        if (isdigit(value)) {
            list.push_back(value - '0'); // Convert digit character to integer value
        } else {
            list.push_back(value);
        }
    }

    void pop() {
        list.pop_back();
    }

    char peek() {
        return list.peek();
    }

    bool isEmpty() const {
        return list.size() == 0;
    }
};

class Expression {
private: 
    string infix;
    string postfix;
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
        Stack stack;
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
                while (!stack.isEmpty() && stack.peek() != '(') {
                    result += stack.peek();
                    stack.pop();
                }
                // pop the opening parenthesis from the stack
                stack.pop();
            } else {
                // if the character is an operator, pop and append operators from the stack
                // until the stack is empty or the top operator has lower precedence
                while (!stack.isEmpty() && stack.peek() != '(' && getPrecedence(ch) <= getPrecedence(stack.peek())) {
                    result += stack.peek();
                    stack.pop();
                }
                stack.push(ch);
            }
        }

        // pop and append any remaining operators from the stack to the result
        while (!stack.isEmpty()) {
            result += stack.peek();
            stack.pop();
        }

        postfix = result;
        
        return result;
    }

    // helper metho to get the precedence of an operator
    int getPrecedence(char op) {
        if (op == '+' || op == '-') {
            return 1;
        } else if (op == '*' || op == '/') {
            return 2;
        }
        return 0; //default precedence for other characters;
    }

    string postToIn() {
        // TODO: implement this
        return "";
    }

    double evaluate() {
        // TODO: implement this
        return 0.0;
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
                // TODO: Handle expression evaluation
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