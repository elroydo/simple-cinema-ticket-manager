#include <iostream>
#include <sstream>
#include <string>
#include <thread>

using namespace std;
using namespace std::this_thread;
using namespace std::chrono; 

int welcome(); //the introductory screen 
int movSelect(); //selecting the movie to screen
float ticket_selection(int &cycle_aTickets, int &cycle_scTickets, int &cycle_cTickets, float &cycle_aCost, float &cycle_scCost, float &cycle_cCost, float &cycle_totalCost, float &doctorSale, float &theSale, float &total_movieSales); //determines which and how many tickets are purchased as well as the seats booked
int bookedSeats(int adultTickets, int seniorTickets, int childTickets); //determine the seats remaining and booked
float transactions(int cycle_aTickets, int cycle_scTickets, int cycle_cTickets, float &aCost, float &scCost, float &cCost, float &totalCost, float &doctorSale, float &theSale, float &total_movieSales); //the cost and sales result
float prntReceipt(int cycle_aTickets, int cycle_scTickets, int cycle_cTickets, float aCost, float scCost, float cCost, float totalCost); //customer receipt
float prntSales(float doctorSale, float theSale, float total_movieSales); //movie sales
int help(); //user help screen

string empName, movOne = "Doctor Strange", movTwo = "The Prestige"; 
const float aPrice = 10.30, scPrice = 8.24, cPrice = 5.15; 
static unsigned short int  remSeats = 60, bkdSeats = 0; 
unsigned short int movChoice; 

struct date { 
	int dd, mm, yy;
};

date today;

int main() {
	int inChoice, alTickets = 0, sclTickets = 0, clTickets = 0;
	float atickCost = 0, sctickCost = 0, ctickCost = 0, totalCost = 0, movoneSale = 0, movtwoSale = 0, totalSales = 0;
	bool loopOne = false;

	welcome();

	while (!loopOne) {

		if (remSeats != 0) {
			system("cls");
			cout << "Initiate Greyic cinema's Ticket Manager?\n(1) Initiate\n(2) Conclude\n(3) Help\n";
			cin >> inChoice;

			switch (inChoice) {
			case 1:
				movSelect();
				ticket_selection(alTickets, sclTickets, clTickets, atickCost, sctickCost, ctickCost, totalCost, movoneSale, movtwoSale, totalSales);
				prntReceipt(alTickets, sclTickets, clTickets, atickCost, sctickCost, ctickCost, totalCost);
				alTickets = 0, sclTickets = 0, clTickets = 0;
				break; 
			case 2:
				system("cls"); 
				cout << "Printing end day sales...\n\n";
				sleep_for(milliseconds(3000)); 
				system("cls"); 
				prntSales(movoneSale, movtwoSale, totalSales); //executes the receipt function, while also passing the values of sales for Doctor Strange, The Prestige and their sum
				loopOne = true; 
				break; 
			case 3:
				system("cls"); 
				help();
				break; 
			default:
				cout << "Please enter a valid option\n";
				cin.clear(); //clears the error state
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); //remove the bad input from the buffer
				break; 
			}
		}
		else {
			cout << "No more seats remaining, ticket manager concluding.\n";
			sleep_for(milliseconds(3000)); //idle for 3 seconds
			system("cls"); 
			cout << "printing end day sales...\n\n";
			sleep_for(milliseconds(3000));
			system("cls"); 
			prntSales(movoneSale, movtwoSale, totalSales);
			loopOne = true;
		}
	}
	cout << endl << "Thank you for using Greyic cinema's ticket manager, goodbye.\n";
	return 0;
}

int welcome() {
	cout << "Welcome to Greyic cinema's ticket manager.\n"
		<< "Please enter your name\n";
	getline(cin, empName); //was changed from 'cin' as it wouldn't store the user's last name, with 'getline(cin, empName)' it stores the entire input, including data after spaces
	cout << "Please enter today's date\nDay(dd): ";
	while (!(cin >> today.dd) || (today.dd < 1) || (today.dd > 31)) { 
		cin.clear(); //clears the error state
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); //remove the bad input from the buffer
		cout << "Invalid input, enter a correct day\n"; 
	}
	cout << "Month(mm): ";
	while (!(cin >> today.mm) || (today.mm < 1) || (today.mm > 12)) { 
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
		cout << "Invalid input, enter a correct month\n"; 
	}
	cout << "Year(yy): ";
	while (!(cin >> today.yy) || !(today.yy == 17)) { 
		cin.clear(); 
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
		cout << "Invalid input, enter a correct year\n"; 
	}

	return 0;
}

int movSelect() {
	bool loopTwo = false; 

	system("cls"); 

	while (!loopTwo) {
		cout << "Please enter the movie option that will be selected for viewing\n(1) Doctor Strange\n(2) The Prestige\n";
		cin >> movChoice; 

		switch (movChoice) { 
		case 1:
			loopTwo = true; 
			break; 
		case 2:
			loopTwo = true; 
			break; 
		default:
			cout << "Please enter a valid option.\n";
			cin.clear(); 
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break; 
		}
	}
	return 0;
}

//the byref arguments (signified with an & sign) have been utilised to return values back to main, to be used by the receipt and sales functions
float ticket_selection(int &cycle_aTickets, int &cycle_scTickets, int &cycle_cTickets, float &cycle_aCost, float &cycle_scCost, float &cycle_cCost, float &cycle_totalCost, float &doctorSale, float &theSale, float &total_movieSales) {
	int tickChoice, adultTicket = 0, seniorTicket = 0, childTicket = 0;
	bool loopThree = false;

	while (!loopThree) {
		system("cls"); 
		cout << "Selected Movie: ";
		if (movChoice == 1) {
			cout << movOne << endl; //outputs Doctor Strange if movChoice is equal to 1
		}
		else {
			cout << movTwo << endl; //Prestige if movChoice is equal to 2
		}
		adultTicket = 0; seniorTicket = 0; childTicket = 0; //ticket variables are assigned to 0 to reset the value after user input in the loop
		if (remSeats != 0) { //if remaining seats is not equal to 0, execute ticket selection, else print receipt and assign the bool value to "true"
			cout << "Seats remaining currently: " << remSeats << endl //remaining seats (updated after every loop cycle)
				<< "Select the category of ticket purchase\n"
				<< "(1) Adult ticket/s\n" 
				<< "(2) Senior Citizen ticket/s\n" 
				<< "(3) Child ticket/s\n" 
				<< "(4) End ticket selection\n"; 
			cin >> tickChoice; 

			switch (tickChoice) { 
			case 1: //adult ticket section
				cout << "Enter the number of adult tickets\n";
				while (!(cin >> adultTicket) || (adultTicket < 1) || (adultTicket > remSeats)) { //validates ticket purchase input for an integer and whether the input is greater than 1 and less than the seats remaining
					cin.clear(); //clears the error state
					cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
					cout << "Invalid input, enter the valid number of adult tickets.\n"; 
				}
				cycle_aTickets += adultTicket;
				break; 
			case 2: //senior citizen ticket section 
				cout << "Enter the number of senior citizen tickets\n";
				while (!(cin >> seniorTicket) || (seniorTicket < 1) || (seniorTicket > remSeats)) { 
					cin.clear(); //clears the error state
					cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
					cout << "Invalid input, enter a valid number of senior citizen tickets.\n"; 
				}
				cycle_scTickets += seniorTicket;
				break; 
			case 3: //child ticket section
				cout << "Enter the number of child tickets\n";
				while (!(cin >> childTicket) || (childTicket < 1) || (childTicket > remSeats)) { 
					cin.clear(); //clears the error state
					cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
					cout << "Invalid input, enter a valid number of child tickets.\n"; 
				}
				cycle_cTickets += childTicket; 
				break; 
			case 4:
				system("cls"); 
				cout << "Ticket selection ending, printing receipt...\n";
				sleep_for(milliseconds(3000));
				loopThree = true; 
				break; 
			default:
				cout << "Please select a valid option\n";
				cin.clear(); //clears the error state
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
				break; 
			}
		}
		else {
			system("cls"); 
			cout << "No more seats left, ticket selection ending, printing receipt...\n";
			sleep_for(milliseconds(3000)); //idle 3 seconds
			loopThree = true; 
		}
		bookedSeats(adultTicket, seniorTicket, childTicket); //validation added in to prevent input over remaining seats (e.g. 60), executes the booked seats function to determine the remaining seats, the adult, senior citizen and child ticket values are passed on
	}
	transactions(cycle_aTickets, cycle_scTickets, cycle_cTickets, cycle_aCost, cycle_scCost, cycle_cCost, cycle_totalCost, doctorSale, theSale, total_movieSales); 

	return cycle_aTickets, cycle_scTickets, cycle_cTickets, cycle_aCost, cycle_scCost, cycle_cCost, cycle_totalCost, doctorSale, theSale, total_movieSales;
}

int bookedSeats(int adultTickets, int seniorTickets, int childTickets) {
	int totalTickets = 0; 

	totalTickets = adultTickets + seniorTickets + childTickets; //the tickets from the ticket selection function are summed up and assigned to the total tickets variable
	bkdSeats = totalTickets; //the global variable of booked seats has the value of total tickets assigned to it
	remSeats -= bkdSeats; //the remaining seats variable is assigned the value of remaining seats subtracted by the booked seats value

	return remSeats, bkdSeats;
}

float transactions(int cycle_aTickets, int cycle_scTickets, int cycle_cTickets, float &aCost, float &scCost, float &cCost, float &totalCost, float &doctorSale, float &theSale, float &total_movieSales) {
	totalCost = 0;
	//the tickets purchased from the ticket selection function are multiplied by the ticket price and the result is assigned	
	aCost = cycle_aTickets * aPrice; 
	scCost = cycle_scTickets * scPrice;
	cCost = cycle_cTickets * cPrice; 
	totalCost = aCost + scCost + cCost;

	if (movChoice == 1) { //if the movie choice was 1, the total ticket cost is added onto Doctor Stranger sales, else total ticket cost is added to The Prestige sales
		doctorSale += totalCost; //the sum value of Doctor Strange sales and the total ticket cost
	}
	else {
		theSale += totalCost; //the sum value of The Prestige sales and the total ticket cost
	}
	total_movieSales = doctorSale + theSale; //the sum value of Doctor Strange and The Prestige sales

	return aCost, scCost, cCost, totalCost, doctorSale, theSale, total_movieSales;
}

float prntReceipt(int cycle_aTickets, int cycle_scTickets, int cycle_cTickets, float aCost, float scCost, float cCost, float totalCost) {
	system("cls"); 
	cout << "///////////////////////RECEIPT///////////////////////\n";
	cout << "You've been served by " << empName << " on " << today.dd << '/' << today.mm << '/' << today.yy << endl; //operator's name and the present date
	if (movChoice == 1) {
		cout << "Movie screening: " << movOne << endl;
	}
	else {
		cout << "Movie screening: " << movTwo << endl;
	}
	cout << "No. of adult tickets purchased: " << cycle_aTickets << endl //totals
		<< "No. of senior citizen tickets purchased: " << cycle_scTickets << endl
		<< "No. of child tickets purchased: " << cycle_cTickets << endl 
		<< "Adult ticket purchase cost = \x9C" << aCost << endl 
		<< "Senior Citizen ticket purchase cost = \x9C" << scCost << endl 
		<< "Child ticket purchase cost = \x9C" << cCost << endl 
		<< "Total = \x9C" << totalCost << endl 
		<< "///////////////////////RECEIPT///////////////////////\n\n";
	system("pause"); //outputs "Press any key to continue" and waits for user to press a key to continue executing the code

	return 0;
}

float prntSales(float doctorSale, float theSale, float total_movieSales) {
	cout << "Final Greyic cinema's Ticket Manager operator: " << empName << ". " << today.dd << '/' << today.mm << '/' << today.yy << " finalised.\n" //ouputs the operator's name and the present date
		<< "///////////////////////SALES///////////////////////\n"
		<< movOne << " total sales = \x9C" << doctorSale << endl 
		<< movTwo << " total sales = \x9C" << theSale << endl 
		<< "Total movie sales = \x9C" << total_movieSales << endl 
		<< "///////////////////////SALES///////////////////////\n\n";

	return 0;
}

int help() {
	system("cls"); 
	cout << "///////////////////////HELP///////////////////////\n"
		<< "-Introduction-\n"
		<< "When presented with option/input prompts, please enter the valid input requested for an error-free execute, e.g. 1 for the first option, 2 for the second option, etc.\n"
		<< "-Welcome screen-\n"
		<< "Enter your full name (First-name Middle-name Second-name). The date in relation to the day (dd), month (mm) and year (yy) should be input using double digits.\n"
		<< "-Movie Selection screen-\n"
		<< "A movie can be selected for viewing using 1 for Doctor Strange or 2 for The Prestige input options.\n"
		<< "-Ticket Selection screen-\n"
		<< "Pick the relevant ticket category and input the quantity of tickets to be purchased, e.g. 1 for the adult ticket category, then 4 for the amount; the input quantity of tickets being purchased needs to be more than 1, but less than the remaining seats.\n"
		<< "-Receipt screen-\n"
		<< "name of the employee, date, the amount and cost of tickets bought from each category, followed by the total cost.\n"
		<< "-Sales screen-\n"
		<< "name of the employee, date, the sales for Doctor Strange and The Prestige, along with their sum total.\n"
		<< "-Conclusion-\n"
		<< "For any bug or error discoveries, please report them immediately to a technician, whereby, a patch will be issued as soon as possible.\n"
		<< "///////////////////////HELP///////////////////////\n\n";
	system("pause"); //outputs "Press any key to continue" and waits for user to press a key to continue executing the code

	return 0;
}