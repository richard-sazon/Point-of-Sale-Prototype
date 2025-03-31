
#include <iostream>
#include <fstream>//for reading and writing to text files
#include <string>//for string conversion and using getline
#include <iomanip>//to make use of fixed and setprecision
#include <cmath>//to make use of abs
#include <vector>//to use vectors to read text files line by line
#include <sstream>//the make output string streams
#include <cctype>//to use tolower
using namespace std;

void pressEnter() {

    cout << "\nPress ENTER key to continue...";
    cin.clear(); // Clears the input buffer
    cin.ignore(); // Discards any remaining characters
    cin.get(); // Waits for user to press Enter

}

void login() {

    string username, password;
    int attempts = 3;

    do {

        cout << "------- LOGIN -------\n";
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;

        if (username == "admin" && password == "password") { // Replace "admin" and "password" with actual login credentials

            return; // Successful login
        } else {

            attempts--;
            cout << "Invalid username or password. " << attempts << " attempts remaining.\n";
            pressEnter();
        }

    } while (attempts > 0);

    cout << "Login attempts exceeded. Exiting.\n";
    exit(0); // Exit the program
}

void editLine(const string& filename, int lineNumber, const string& newContent) {

    ifstream inputFile(filename);

    if (!inputFile.is_open()) {

        cerr << "Error opening file for reading." << endl;
        return;

    }

    vector<string> lines;
    string line;
    // Read the file line by line and store it in a vector
    while (getline(inputFile, line)) {

        lines.push_back(line);

    }

    inputFile.close();
    int amtOfLines = lines.size();
    // Check if the line number is valid
    if (lineNumber < 1 || lineNumber > amtOfLines) {

        cerr << "Invalid line number." << endl;
        return;

    }

    // Modify the specific line
    lines[lineNumber - 1] = newContent;
    // Open the file for writing (this will overwrite the file)
    ofstream outputFile(filename);

    if (!outputFile.is_open()) {

        cerr << "Error opening file for writing." << endl;
        return;

    }

    // Write the modified lines back to the file
    for (const auto& modifiedLine : lines) {

        outputFile << modifiedLine << endl;

    }
}

void deleteLine(const string& filename, int lineNumber) {

    ifstream inputFile(filename);
    vector<string> lines;

    // Check if the file opened successfully
    if (!inputFile) {

        cerr << "Error opening file for reading." << endl;
        return;
    }

    // Read all lines from the file
    string line;

    while (getline(inputFile, line)) {

        lines.push_back(line);
    }

    inputFile.close();
    int amtOfLines = lines.size();
    // Check if the line number is valid
    if (lineNumber < 1 || lineNumber > amtOfLines) {

        cerr << "Invalid line number." << endl;
        return;
    }

    // Remove the specific line (lineNumber is 1-based)
    lines.erase(lines.begin() + (lineNumber - 1));
    // Write the modified lines back to the file
    ofstream outputFile(filename);

    if (!outputFile) {

        cerr << "Error opening file for writing." << endl;
        return;
    }

    for (const auto& modifiedLine : lines) {

        outputFile << modifiedLine << endl;

    }

}

void addInventory(ofstream& outFile) {

    string input, description;
    int code, codeLen = 0, descLen = 0;
    bool isOpen = true;
    double price = 0;

//    outFile.open("inventory.txt");

    while (isOpen == true) {

        codeLen = 0; // Reset codeLen for each new input attempt

        while (codeLen != 6) {

            cout << "Insert Code: ";
            cin >> input; // Read input as a string

            try {

                code = stoi(input); // Try to convert the input to an integer

                if (code == 000) { //tests if input is 000

                    cout << "Command Cancelled" << endl;
                    codeLen = 6; // Exit the loop

                } else {

                	codeLen = input.length();

                    if (codeLen != 6) { //checks if the length of the code is 6

                        cout << "INVALID SIX-DIGIT INVENTORY CODE" << endl;//outputs if code length is not 6

                    } else {

                    	ifstream inFile;
                    	int tempCode = 0;
                    	string tempDesc = "";
                    	double tempPrice = 0;

                    	inFile.open("inventory.txt");

                    	if (inFile.is_open()){

                    		while (!inFile.eof()){//reads the file line by line
                    			inFile >> tempCode;
                    			inFile.ignore();
                    			getline(inFile, tempDesc, ',');
                    			inFile >> tempPrice;
                    			inFile.ignore();

                    			if (tempCode == code){//checks if the code already exists in the file

                    				cout << "INVALID: CODE ALREADY EXISTS" << endl;
                    				codeLen = 0;
                    				inFile.close();
                    				break;
                    			}
                    		}
                    	} else {

                    		cout << "Could not open input file" << endl;

                    	}
                    	inFile.close();
                    }

                }

            } catch (const invalid_argument&) {

                // If conversion fails, output error message once
                cout << "INVALID INPUT! Please enter a valid integer." << endl;

            }

        }


        // After valid input

        if (code == 000) {//brings user back to main menu

            cout << "Command Cancelled" << endl;
            isOpen = false;

        } else {//continues to prompt user of description

            while (true){

            	cout << "Enter description [10 characters max] >>> ";
            	cin >> description;
            	descLen = description.length();

            	if (descLen > 10){//tests if description length is 10

            		cout << "DESCRIPTION IS 10 CHARACTERS MAX" << endl;//outputs if description length is not 10

            	} else {
            		break;
            	}

            }

            if (description == "000"){//checks if input is 000 and brings user back to main menu if it is

            	cout << "Command Cancelled" << endl;
				isOpen = false;

            } else {//continues to prompts user for price

            	while (true){

            		cout << "Enter price >>> $";
            		cin >> price;
            		double rounded = round(price * 100.0)/100.0;//rounds price to 2 decimal places

            		if (price == 000){//checks if input is 000

            			break;
            		}

            		if (price > 0 && abs(price - rounded) < 0.001){//verifies the input is positive

            			break;

            		} else {

            			cout << "INVALID PRICE" << endl;//outputs if input is an invalid value and loops back to prompt user for price

            		}
            	}

            	if (price == 000){//brings user back to main menu and cancels command

            		cout << "Command Cancelled" << endl;
            		isOpen = false;

            	} else {//adds entered data into inventory.txt

            		cout << "Inventory added" << endl;
					outFile << code << "," << description << "," << fixed << setprecision(2) << price << endl; // Make sure to write a newline after the code
					pressEnter();
					isOpen = false;

            	}

            }
        }
    }
}

void editInventory(ofstream& outFile){

	ifstream inFile;
	int lineToEdit = 0;
	int tempCode, oldCode, newCode;
	string tempDesc, oldDesc, newDesc = "", strNewCode, strNewPrice;
	double tempPrice, oldPrice, newPrice;
	bool isOpen = true, loopBack = false;;


	while (isOpen == true){

		cout << "Enter code you want to edit >>> ";//prompts user to enter code to edit
		cin >> tempCode;
		inFile.open("inventory.txt");

		while (!inFile.eof()){
			//reads inventory.txt line by line and increments lineToEdit counter by 1 after each new line
			lineToEdit++;
			inFile >> oldCode;
			inFile.ignore();
			getline(inFile, oldDesc, ',');
			inFile >> oldPrice;
			inFile.ignore();

			if (tempCode == oldCode){//checks if entered code actually exists

				cout << "Enter new code or press ENTER key to keep current value >>> ";//prompts user for if they want to change the code
				cin.ignore();
				cin.clear();
				getline (cin, strNewCode);

				if (strNewCode.empty()){//keeps current code number if user presses ENTER

					newCode = oldCode;
					loopBack = false;
					break;

				} else if (strNewCode.length() > 6) {//outputs if code does not equal 6 digits

					cout << "INVALID! CODE MUST BE 6 DIGITS!" << endl;

				} else {

					try {

						newCode = stoi(strNewCode);
						inFile.close();
						inFile.open("inventory.txt");

						while (!inFile.eof()){
							//reads file line by line to check if entered code already exists
							inFile >> tempCode;
							inFile.ignore();
							getline(inFile, tempDesc, ',');
							inFile >> tempPrice;
							inFile.ignore();

							if (newCode == tempCode) {//outputs if the code already exists

								cout << "INVALID! NEW CODE ALREADY EXISTS!" << endl;
								loopBack = true;
								inFile.close();
								break;

							} else if (inFile.eof()) {//breaks out of loop and keeps edited code

								loopBack = false;
								inFile.close();
								break;

							}

						}


					} catch (const invalid_argument&) {

		                // If conversion fails, output error message once
		                cout << "INVALID INPUT! Please enter a valid integer." << endl;
		                inFile.close();
		                break;

		            }
				}

			} else if (inFile.eof()){//outputs if loop reaches end of file and can't find code to edit

				cout << "INVALID: CODE DOES NOT EXIST" << endl;
				inFile.close();
				loopBack = true;


			} else if (tempCode == 000) {//breaks out of loop if input is 000

				break;
			}
		}

		if (tempCode == 000) {//brings user back to main menu and cancels command

			cout << "Command Cancelled" << endl;
			isOpen = false;

		} else if (loopBack == true){//loops user back to enter code they wish to edit if input is invalid

			isOpen = true;

		} else {

			inFile.close();
			while (true){//continues to prompt user for description

					cout << "Enter new description [10 characters max] or press ENTER to keep current value >>> ";
					cin.clear();
					getline(cin, newDesc);
					int descLen = newDesc.length();

					if (descLen > 10){//checks if description length is greater than 10

						cout << "DESCRIPTION IS 10 CHARACTERS MAX" << endl;

					} else if (newDesc.empty()){//keeps current value if user presses ENTER

						newDesc = oldDesc;
						break;

					} else {

						break;
					}

				}

			if (newDesc == "000") {//brings user back to main menu and cancels command

				cout << "Command Cancelled" << endl;
				isOpen = false;

			} else {//continues to prompts user for price

				while (true){

					try {

						cout << "Enter price or press ENTER to keep current value >>> $";
						cin.clear();
						getline(cin, strNewPrice);
						double rounded;

						if (strNewPrice.empty()) {//keeps current value if user presses ENTER

							newPrice = oldPrice;
							break;
						} else {

							newPrice = stod(strNewPrice);
							rounded = round(newPrice * 100.0)/100.0;
						}


						if (newPrice == 000){//checks if input is 000

							break;
						}

						if (newPrice > 0 && abs(newPrice - rounded) < 0.001){//checks if input is valid

							break;

						} else {//outputs message if price is invalid and loops back to prompts user for price

							cout << "INVALID PRICE" << endl;

						}

					} catch (const invalid_argument&) {
						//outputs when program catches invalid_argument exception
						cout << "INVALID PRICE!" << endl;
					}

				}
			}

				if (newPrice == 000){//brings user back to main menu and cancels command

					cout << "Command Cancelled" << endl;
					isOpen = false;

				} else {

					ostringstream oss; // Create an output string stream
					oss << fixed << setprecision(2) << newPrice; // Format the price
					string formattedPrice = oss.str();

					string filename = "inventory.txt";
					string editedLine = to_string(newCode) + "," + newDesc + "," + formattedPrice;
					editLine(filename, lineToEdit, editedLine);//edits the desired values entered by user
					cout << "Inventory Edited" << endl;
					pressEnter();
					isOpen = false;
				}

			}

		}

	}



void deleteInventory(ofstream& outFile){

	string strCode, description;
	int lineToDelete = 0, codeToDelete, tempCode;
	double price;
	char decision;
	ifstream inFile;
	bool loopBack = true;

	while (loopBack == true) {

		try {
			cout << "Enter the code you want to delete >>> ";//prompts user for code to delete
			cin >> strCode;

			if (strCode == "000") {//checks if code is 000

				loopBack = false;
			}

			if (strCode.length() != 6 && strCode != "000") {//outputs if code is not correct length

				cout << "INVALID! CODE MUST BE 6 DIGITS" << endl;
			} else if (strCode.length() == 6){

				codeToDelete = stoi(strCode);
				inFile.open("inventory.txt");

				while (!inFile.eof()) {
					//reads file line by line and increments lineToDelete counter by 1
					lineToDelete++;
					inFile >> tempCode;
					inFile.ignore();
					getline(inFile, description, ',');
					inFile >> price;
					inFile.ignore();

					if (inFile.eof() && codeToDelete != tempCode) {//outputs if code does not exist and loops back to prompt user for code

						cout << "INVALID! CODE DOES NOT EXIST" << endl;
						inFile.close();
						break;
					} else if (tempCode == codeToDelete) {

						loopBack = false;
						inFile.close();
						break;
					}
				}
			}

		} catch (const invalid_argument&) {
			//outputs if program if program catches invalid_argument exception
			cout << "INVALID! CODE MUST BE A 6 DIGIT INTEGER" << endl;
		}
	}

	if (strCode == "000") {//cancels command and goes back to main menu

		cout << "Command Cancelled" << endl;
	} else {

		cout << "Unit Code: " << codeToDelete << endl <<
				"Description: " << description << endl <<
				"Unit Price: " << price << endl;

		cout << "Are you sure you want to delete? [y/n] >>> ";//prompts user for decision
		cin >> decision;
		decision = tolower(decision);

		if (decision == 'y') {//if y, it deletes the data

			deleteLine("inventory.txt", lineToDelete);
			cout << "Inventory deleted" << endl;
			pressEnter();
		} else if (decision == 'n') {//if n, it keeps the data

			cout << "Inventory not deleted" << endl;
			pressEnter();
		} else {//keeps the data if input is invalid

			cout << "INVALID INPUT! inventory not deleted" << endl;
			pressEnter();
		}

	}

}

void listAll(ofstream& outFile){

	ifstream inFile;
	vector<string> lines;
	string line;
	int index = 0, amtOfItems;

	inFile.open("inventory.txt");
	while(getline(inFile, line)){//reads file line by line

		lines.push_back(line);
	}

	inFile.close();
	amtOfItems = lines.size();

	for (string item : lines){//loops through the inventory line by line

		index++;
		cout << lines[index-1] << endl;

		if (index == amtOfItems){//outputs when it reaches end of file

			cout << "--- End of List ---" << endl;
			pressEnter();
		}

		if (index % 15 == 0 && index != amtOfItems){//pauses output if it reaches a line perfectly divisible by 15

			pressEnter();
		}



	}

}

int main() {

	login();
	int choice;
	ofstream outFile;
	outFile.open("inventory.txt", ios::app);

	while (choice != 0){

		cout << "\tMain Menu" << endl <<
				"[1] Add Inventory" << endl <<
				"[2] Edit Inventory" << endl <<
				"[3] Delete Inventory" << endl <<
				"[4} List All Inventories" << endl <<
				"[0] Exit" << endl <<
				"Enter Your Choice >>> ";
		cin >> choice;

		if (choice == 1){
			addInventory(outFile);
		} else if (choice == 2) {
			editInventory(outFile);
		} else if (choice == 3) {
			deleteInventory(outFile);
		} else if (choice == 4) {
			listAll(outFile);
		} else if (choice == 0) {
			outFile.close();
			cout << "Exiting Program" << endl;
		} else {
			cout << "ERROR INVALID INPUT" << endl;
		}
	}

	return 0;
}
