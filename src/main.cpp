#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "commands.h"


// Struct type for expressions
struct expression
{
    unsigned int 	data : 24;
    bool			declared;
};

bool and_func(int x, int y) {
	// Returns both of the first bit of X and Y are both true
	// Sorry for the fancy unreadable math, it's the solution I found to this on StackOverflow
	return ( ( x & ( 1 << 0 )) >> 0) &&  ( ( y & ( 1 << 0 )) >> 0);
}



int main(int argc, char *argv[]) {
	
	// Get user file name
	std::string file_name; 
	if (argc<2 || argc>3) {
		throw std::invalid_argument("Invalid number of arguments. Please specify a file name.");
	} else {
		file_name = argv[1];
	}

	// Import input code from the specified file and convert to string
	std::fstream file_input;
	file_input.open (file_name, std::ios::in | std::ios::app | std::ios::binary);

	std::string input((std::istreambuf_iterator<char>(file_input)),
                 std::istreambuf_iterator<char>());

	// Index of which character we're currently at in the code (THIS IS VERY IMPORTANT)
	long int read_index = 0;
	int command;
	bool declare_command_end = true;



	// Vector containg all the variable names (of exp type) that we have declared (these will be used when we wanna access variables)
	std::vector<std::string> exp_variable_names;

	// Vector containg all the variable values(of exp type) that we have declared 
	std::vector<expression> exp_variable_values;


	while (read_index < input.size()) {

		// TODO: Replace these if-else loops with a switch loop to improve readability
		if(input.substr(read_index, 8) == "declare ") {
			command = DECLARE;
		} else if (input.substr(read_index, 2) == "<-") {
			command = ASSIGN;
		} else {
			command = UNKNOWN;
		}

		// Declare code
		bool declare_command_end = true;
		if (command == DECLARE) {
			// Increase the read index by 8
			read_index += 8;
				while (declare_command_end) {

				int var_name_len = 0;
				int var_value_len = 0;
				bool declare_no_define = false;

				// Move the pointer forward until we find an equals sign, a semicolon, or a comma
				while (input[read_index + var_name_len] != '=' && input[read_index + var_name_len] != ';' && input[read_index + var_name_len] != ',') {
					var_name_len++;
				}

				// If we find a semicolon or a comma instead of the variable's definition, it probably doesn't have a definition. Skip the value recognition code.
				if (input[read_index + var_name_len + 1] == ';' || input[read_index + var_name_len + 1] == ',') {
					declare_no_define = true;
				}
				var_name_len++;

				/// TRIM OFF SPACES AT THE END OF THE VARIABLE NAME TO MAKE ACCESSING VIA INPUT CODE POSSIBLE

				// Create a string for the variable name
				std::string var_name = input.substr(read_index, var_name_len);

				// Find the last character in the variable name and save it to "found"
				int found = var_name.find_last_not_of(" ,=;");

				// Erase all characters between "found" and the string's end
				var_name.erase (found + 1,var_name.size()); 




				// Insert the variable name into the names table
				exp_variable_names.insert(exp_variable_names.begin(),var_name);
				std::cout << "Variable added: "<< exp_variable_names[0] << "." << "\n";

				// Increment the read index (plus 1, the equals sign isn't a number)
				read_index += var_name_len + 1;
				if (!declare_no_define) {

					while (input[read_index + var_value_len] != ';' && input[read_index + var_value_len] != ',') {
						var_value_len++;
					}

					var_value_len++;
					
					/// CONVERTING STRING TO INT24

					// Convert from string to int
					unsigned int var_value; 
					std::stringstream conv; 
					conv << (input.substr(read_index, var_value_len) );
					conv >> var_value;

					// Convert from int to int24 
					expression yes;
					yes.data = var_value;
					yes.declared = true;

					/// END
							
					std::cout << "Variable value: "<< yes.data << "\n";
					// Insert the int24 into the values table
					exp_variable_values.insert(exp_variable_values.begin(), yes);
					read_index += var_value_len;
				} else {
					expression null_exp;
					null_exp.data = 0;
					null_exp.declared = false;
					exp_variable_values.insert(exp_variable_values.begin(), null_exp);

				}
				std::cout << "\n";
				if (input[read_index] == ',' || input[read_index - 1] == ',') {
					// Increment the read index so it's not stuck on the comma
					read_index++;
					// Increment the read index until it's not pointing to a space
					while (input[read_index] == ' ') {
						read_index++;
					}
				} else {
					break;		
				}
			}
		}

		//TODO: Assign code
		if (command == ASSIGN) {
			int assign_leftside_end = read_index - 1;
			int assign_rightside_len = 0;
			std::string assign_leftside, assign_rightside;
			std::cout << "Assign command recognized." << "\n";
			std::cout << input.substr(0, read_index) << "\n";

			// Finding the end of the value before the arrow

			// Go backwards until we stop encountering spaces
			while (input[assign_leftside_end] == ' ') {
				assign_leftside_end--;
			}
			std::cout << assign_leftside_end << input[assign_leftside_end] << "\n";

			// Finding the beginning of the value before the arrow
			int assign_leftside_begin = assign_leftside_end;


			// Go backwards until we find a semicolon
			while (input[assign_leftside_begin] != ';') {
				assign_leftside_begin--;	
				std::cout << assign_leftside_begin << input[assign_leftside_begin] << "\n";
			}

			// Use our new pointers to create a string of the left value's name
			assign_leftside = input.substr(assign_leftside_begin, assign_leftside_end - assign_leftside_begin + 1);

			// Move the read index past the left arrow
			read_index += 2;

			// Increment the pointer until we reach a semicolon
			while (input[read_index + assign_rightside_len] != ';') {
				assign_rightside_len++;
			}

			// Use our new pointers to create a string of the right value's name
			assign_rightside = input.substr(read_index, assign_rightside_len);

			// Detected variable name
			// TODO: Remove semicolon and spaces from the beginning to allow searching for it in the vector
			std::cout << "." << assign_leftside << ".\n";
		}
		// Increase the read index
		read_index++;
	}
	std::cout << "\n" << "Finished execution." << "\n";

	return 0;
}