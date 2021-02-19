// Isak Angerstig
// CWID 50209016
// 02/13/2021 - 02/18/2021
// Json Validator using pointer-based stack
// CSCI 520

#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
using namespace std;

// Node struct will contain value of node and pointer to next node.
struct Node
{
	char data;      // data that will be added to node
	Node* next;     // pointer to next node
};

// Stack will use Node structs as items 
class Stack
{
public:
	Stack()
	{
		top = NULL;
	}

	void Push(char item);   // Insert item to stack
	void Pop();             // Remove item from stack
	char TopValue();        // Find top value of stack
	bool CheckRemaining();  // Check if there are any values left in stack
	void PrintAll();        // Prints everything in stack
private:
	Node* top;              // A pointer to show top node
};

// Add an item (node) to the stack
void Stack::Push(char item)
{
	Node* temp;             // Pointer to the node
	temp = new Node();

	temp->data = item;
	temp->next = top;
	top = temp;
}

// Remove item (node) from stack
void Stack::Pop()
{
	Node* temp;
	if (top == NULL)
	{
		cout << "No items in stack" << endl;
		exit(1);
	}
	else
	{
		// Set temp to top node then go to second item and set that to top
		temp = top;
		top = top->next;
		// Now remove top node from stack and free up the memory
		temp->next = NULL;
		free(temp);
	}
}

char Stack::TopValue()
{
	Node* temp;
	temp = top;
	if (top == NULL)
	{
		//cout << "No items in stack" << endl;
		return false;
	}
	else
	{
		return temp->data;
	}
}

// Check if any values are left in stack
bool Stack::CheckRemaining()
{
	Node* temp;
	temp = top;
	if (top == NULL) return true;
	else return false;
}

void Stack::PrintAll()
{
	Node* temp;
	temp = top;

	while (temp != NULL)
	{
		cout << temp->data << " ";
		temp = temp->next;
	}
	cout << endl;
}

bool ValidateJsonString(string str)
{
	cout << "Input string: " << str << endl;
	// Create bracket stack
	Stack balance_stack;

	// Create characters stack
	Stack char_stack;

	int counter = 0;
	int spaces = 1;

	if (str.length() > 99)
	{
		cout << "False" << endl;	// Too many characters
		return false;
	}

	// Loop through all items
	for (int i = 0; i < str.length(); i++)
	{
		// If str[i] is a letter then quotes have to wrap it
		if (isalpha(str[i]))
		{
			if (balance_stack.TopValue() == '"')
			{
				// Good, it is in the right spot
				counter++;
				continue;
			}
			else
			{
				// If quote not on top, then letter is in the wrong spot
				cout << "False" << endl;	// Variable in the incorrect spot
				return false;
			}
		}

		// Condition to check if there is a quote in the stack
		// If there is, then the next value has to be one or more characters followed by an end quote
		if (balance_stack.TopValue() == '"')
		{
			// Now if counter is greater than 0, then there were letters inside the quotes
			// So now we need to check that there is an end quote to the key or value
			if (counter > 0)
			{
				// Now you NEED the closing quotes, otherwise return false REASON unmatched
				if (str[i] == '"')
				{
					balance_stack.Pop();
					counter = 0;
					// This is to ignore whitespaces between operations
					while (str[i + spaces] == ' ')
					{
						spaces++;
					}
					// Now I need to check if next char is colon, comma, or end bracket
					if (str[i + spaces] == ':' || str[i + spaces] == ',' ||
						str[i + spaces] == '}' || str[i + spaces] == '[')
					{
						spaces = 1;
						continue;
					}
					else
					{
						cout << "False" << endl;	// Incorrect syntax
						return false;
					}
					spaces = 1;
					continue;
				}
				else
				{
					cout << "False" << endl;	// Unmatched quotation marks
					return false;
				}
			}
			// There were no letters inside of the quotes
			else
			{
				cout << "False" << endl;	// A variable cannot have 0 length
				return false;
			}
		}

		// If char is an open bracket  or a quote push it to the bracket_stack
		if (str[i] == '[' || str[i] == '{' || str[i] == '"')
		{
			balance_stack.Push(str[i]);
		}

		// Check for correct brackets
		switch (str[i])
		{
		case ']':
			if (balance_stack.TopValue() != '[')
			{
				cout << "False" << endl;	// Unmatched bracket
				return false;
			}
			balance_stack.Pop();
			break;

		case '}':
			if (balance_stack.TopValue() != '{')
			{
				cout << "False" << endl;	// Unmatched bracket
				return false;
			}
			balance_stack.Pop();
			break;
		}
	}
	// If CheckRemaining has nothing on stack, it returns true. So if not CheckRemaining, then return false
	if (!balance_stack.CheckRemaining())
	{
		cout << "False" << endl;	// Unmatched bracket
		return false;
	}
	cout << "True" << endl;
	return true;
}

int main()
{
	// Checking few examples
	string str = R"({ "a":"b"})";
	ValidateJsonString(str);
	str = R"({ "a":"b", "x":"y"})";
	ValidateJsonString(str);
	str = R"({ "a":"b", "x":"y")";
	ValidateJsonString(str);
	str = R"({ "a": "b" "x": "y" })";
	ValidateJsonString(str);
	str = R"({ "a":"b", "x": [ { "r":"a" }] })";
	ValidateJsonString(str);
	str = R"({ "a":"b", "x": [ { "r":"a", "a":"b"  }] })";
	ValidateJsonString(str);
	str = R"({"x":[{"r":"a","a":"b"}],"a":"b","t":[{"r":"a","a":"b"}] })";
	ValidateJsonString(str);

	// Let user input anything
	string newString;
	cout << "Enter a json string: ";
	getline(cin, newString);

	ValidateJsonString(newString);

	system("pause");
	return 0;
}