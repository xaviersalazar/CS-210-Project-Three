#include <Python.h>
#include <iostream>
#include <Windows.h>
#include <cmath>
#include <string>
#include <fstream>

using namespace std;

/*
Description:
	To call this function, simply pass the function name in Python that you wish to call.
Example:
	callProcedure("printsomething");
Output:
	Python will print on the screen: Hello from python!
Return:
	None
*/
void CallProcedure(string pName)
{
	char* procname = new char[pName.length() + 1];
	std::strcpy(procname, pName.c_str());

	Py_Initialize();
	PyObject* my_module = PyImport_ImportModule("PythonCode");
	PyErr_Print();
	PyObject* my_function = PyObject_GetAttrString(my_module, procname);
	PyObject* my_result = PyObject_CallObject(my_function, NULL);
	Py_Finalize();

	delete[] procname;
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("PrintMe","Test");
Output:
	Python will print on the screen:
		You sent me: Test
Return:
	100 is returned to the C++
*/
int callIntFunc(string proc, string param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	char* paramval = new char[param.length() + 1];
	std::strcpy(paramval, param.c_str());


	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(z)", paramval);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;
	delete[] paramval;


	return _PyLong_AsInt(presult);
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("doublevalue",5);
Return:
	25 is returned to the C++
*/
int callIntFunc(string proc, int param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(i)", param);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;

	return _PyLong_AsInt(presult);
}


/*
	Function for displaying the menu selection to the user
*/
void displayMenu() {
	cout << "1. Display all items purchased" << endl;
	cout << "2. Display frequency of individual item" << endl;
	cout << "3. Display histogram of all items purchased" << endl;
	cout << "4. Exit" << endl;
	cout << "Enter your selection as a number 1, 2, 3, or 4: ";
}

/*
	Function that checks if user entered a valid selection
*/
int isUserInputValid(int userInput) {
	int VALID_SELECTIONS[] = { 1, 2, 3, 4 }; // List of valid user selections

	return find(begin(VALID_SELECTIONS), end(VALID_SELECTIONS), userInput) != end(VALID_SELECTIONS);
}

/*
	Function that checks for user selection error and continues to prompt until no error 
*/
int displayError(int userSelection, string msg, bool showMenu = false) {
	int userInput = userSelection;

	while (cin.fail() || !isUserInputValid(userInput)) {
		cout << msg;
		cin.clear();
		cin.ignore(256, '\n');

		if (showMenu) {
			displayMenu();
		}

		cin >> userInput;
	}

	return userInput;
}

/*
	Function to display the selected item
*/
void displaySelectedItem() {
	string itemToDisplay;
	int numOfItemPurchased;

	cout << "Enter an item to show how many times it has been purchased: ";
	cin >> itemToDisplay;

	numOfItemPurchased = callIntFunc("displayItem", itemToDisplay);

	// Keep prompting for input if it is not valid
	while (cin.fail() || numOfItemPurchased == 0) {
		cout << itemToDisplay << " doesn't exist. Please enter a valid item: ";
		cin.clear();
		cin.ignore(256, '\n');

		cin >> itemToDisplay;

		numOfItemPurchased = callIntFunc("displayItem", itemToDisplay);
	}

	cout << itemToDisplay << " was purchased " << numOfItemPurchased << " time(s)" << endl;
}

/*
	Function for displaying histogram of items purchased
*/
void displayHistogram() {
	CallProcedure("writeItemsToFile"); // Call python function to write file

	ifstream file("frequency.dat"); // Open frequency.dat

	// Display error if cant open frequency.dat
	if (!file.is_open()) {
		cout << "Error opening frequency.dat" << endl;
	}
	
	// Read and display items until end of file
	while (!file.eof()) {
		string item;
		int numOfItemPurchased;

		file >> item >> numOfItemPurchased;

		if (item != "" && numOfItemPurchased > 0) {
			cout << item << " " << string(numOfItemPurchased, '*');
		}

		cout << endl;
	}

	file.close();

	cout << endl;
}

int main()
{
	int userSelection;

	// Do-while loop to display the menu and call the appropriate function
	do {
		displayMenu();
		cin >> userSelection;

		cout << endl << endl;

		// Determines if the user entered a integer
		userSelection = displayError(userSelection, "That is not a valid selection. Please enter 1, 2, 3, or 4.\n \n", true);
		
		// Switch statement to determine correct function to call
		switch (userSelection) {
			case 1:
				CallProcedure("displayItem");
				cout << endl;

				break;
			case 2:
				displaySelectedItem();
				cout << endl;

				break;
			case 3:
				displayHistogram();
				break;

			case 4:
				cout << "Exiting..." << endl;
				break;
		}
		
	} while (userSelection != 4);

	return 0;
}