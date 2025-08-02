//============================================================================
// Name        : Project Two.cpp
// Author      : Leon Machado-Wilcox
// Version     : 1.1
// Description : Project 2 for CS-300
//============================================================================

#include <time.h>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <string> // atoi
#include <string.h>
#include <vector>
#include <sstream>
#include "CSVparser.hpp"

using namespace std;

const std::string DEFAULT_CSV_PATH = "ABCU_Advising_Program_Input.csv";
const std::string DEFAULT_COURSE_ID = "CSCI300";
//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold course information
struct Course
{
	string courseId; // unique identifier
	string title;
	string prereqs;

	Course()
	{
		courseId = "0";
	}
};

//============================================================================
// Linked-List class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a linked-list.
 */
class LinkedList
{

private:
	// Internal structure for list entries, housekeeping variables
	struct Node
	{
		Course course;
		Node *next;

		// default constructor
		Node()
		{
			next = nullptr;
		}

		// initialize with a course
		Node(Course aCourse)
		{
			course = aCourse;
			next = nullptr;
		}
	};

	Node *head;
	Node *tail;
	int size = 0;

public:
	LinkedList();
	virtual ~LinkedList();
	void Append(Course course);
	void PrintCourse(string courseId);
	void SortList();
	void PrintList();
	Course Search(string courseId);
	int Size();
};

/**
 * Default constructor
 */
LinkedList::LinkedList()
{
	// Initialize housekeeping variables
	head = nullptr;
	tail = nullptr;
}

/**
 * Destructor
 */
LinkedList::~LinkedList()
{
	// start at the head
	Node *current = head;
	Node *temp;

	// loop over each node, detach from list then delete
	while (current != nullptr)
	{
		temp = current;			 // hang on to current node
		current = current->next; // make current the next node
		delete temp;			 // delete the orphan node
	}
}

/**
 * Append a new course to the end of the list
 */
void LinkedList::Append(Course course)
{
	Node *newNode = new Node(course); // create the node for the course
	if (head == nullptr)			  // if we're starting at the beginning
	{
		head = newNode; // Make everything point to the new class
		tail = newNode;
	}
	else
	{
		tail->next = newNode; // Make the tail and tail->next point to the new class
		tail = newNode;
	}
	++size; // Increment the size of the LinkedList
}

/**
 * Simple printout of a course
 */
void LinkedList::PrintCourse(string courseId)
{
	Course course = this->Search(courseId);																												  // Find the course entered
	cout << "Course ID: " << course.courseId << " | " << "Course Title: " << course.title << " | " << "Course Prerequisites: " << course.prereqs << endl; // print the information
}

/**
 *  Sort the list according to courseId
 */
void LinkedList::SortList()
{
	Node *current = this->head; // Set the current node to the head
	LinkedList sortedList;		// Created a Linkedlist to work with

	Node *sortedNode = nullptr; // Set the linkedlist to null
	while (current != nullptr)
	{								// While the current node is not null
		Node *next = current->next; // Set a next node to the current->next node

		// If the sortednode is null or our comparrison passes
		if (sortedNode == nullptr || strcmp(current->course.courseId.c_str(), sortedNode->course.courseId.c_str()) < 0)
		{
			current->next = sortedNode; // Set the current->next to sortedNode
			sortedNode = current;		// Set the sorted node to the current
		}
		else
		{
			Node *tmp = sortedNode; // Create a tmp node from the sortednode

			// while the tmp->next is not null and our comparrison is valid
			while (tmp->next != nullptr && strcmp(tmp->next->course.courseId.c_str(), current->course.courseId.c_str()) < 0)
			{
				tmp = tmp->next; // Increment the tmp to the next node
			}
			current->next = tmp->next; // Set the current->next to the tmp->next
			tmp->next = current;	   // Set the tmp->next to the current
		}
		current = next; //  Set the current node to the next node
	}
	this->head = sortedNode; // Set the head to the sortedNode
	Node *tmp = this->head;	 // Set the tmp to the head
	while (tmp->next != nullptr)
	{					 // while tmp-> is not null
		tmp = tmp->next; // set tmp to tmp->next
	}
	this->tail = tmp;			  // this this list to have a tail value from tmp
	this->tail->next = tmp->next; // Set the tail-> next to also have the tail value of tmp
}

/**
 * Simple output of all courses in the list
 */
void LinkedList::PrintList()
{
	this->SortList();		   // Start by sorting the list
	Node *current = head;	   // Set the current location to head
	while (current != nullptr) // While the current location is not null
	{
		this->PrintCourse(current->course.courseId); // Print this course
		current = current->next;					 // Increment the current to the next node
	}
}

/**
 * Search for the specified courseId
 *
 * @param courseId The course id to search for
 */
Course LinkedList::Search(string courseId)
{
	Node *current = head;	   // Set a current node to the head
	Node *temp = new Node;	   // Create a temp node
	while (current != nullptr) // While the current node is not null
	{
		if (current->course.courseId == courseId) // If the current course matches
		{										  // the searched one
			return current->course;				  // Give it back to the caller
		}
		current = current->next; // Otherwise increment and continue looking
	}
	return temp->course; // If we don't find it, return a null course
}

/**
 * Returns the current size (number of elements) in the list
 */
int LinkedList::Size()
{
	return size; // Return the size of the linkedlist
}

/**
 * Load a CSV file containing courses into a linked list.
 *
 * @param csvPath The path to the CSV file to load.
 * @param linkedList Pointer to the linked list where courses will be stored.
 */
void loadCourses(string csvPath, LinkedList *linkedList)
{
	cout << "Loading CSV file " << csvPath << endl;

	// Initialize the CSV Parser using the given path
	csv::Parser file = csv::Parser(csvPath);

	// Read and display header row - optional
	vector<string> header = file.getHeader(); // Create a vector for the first row of the file
	for (auto const &c : header)
	{
		cout << c << " | ";
	}
	cout << "" << endl;

	try
	{
		// Loop to read rows of the CSV file
		for (unsigned int i = 0; i < file.rowCount(); i++)
		{
			const auto &row = file[i]; // Get the current row
			if (row.size() < 3)
			{ // Check if the row has at least 3 columns
				std::cerr << "Row " << i << " does not have enough columns. Skipping." << std::endl;
				continue; // Skip this row if it doesn't have enough
			}
			// Create a Course object and populate it with data from the current row
			Course course;
			course.courseId = row[0]; // Get the current row[0];   // Set the course ID
			course.title = row[1];	  // Get the current row[1];      // Set the course title
			course.prereqs = row[2];  // Get the current row[2];    // Set the course prerequisites

			// Append this course to the linked list
			linkedList->Append(course);
		}
	}
	catch (csv::Error &e)
	{
		// Handle any CSV parsing errors
		std::cerr << e.what() << std::endl;
	}
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch)
{
	str.erase(remove(str.begin(), str.end(), ch), str.end());
	return atof(str.c_str());
}

int main(int argc, char *argv[])
{
	// process command line arguments
	string csvPath, courseId;
	switch (argc)
	{
	case 2:
		csvPath = argv[1];
		courseId = "CSCI300";
		break;
	case 3:
		csvPath = argv[1];
		courseId = argv[2];
		break;
	default:
		csvPath = DEFAULT_CSV_PATH;
		courseId = DEFAULT_COURSE_ID;
	}

	// Define a timer variable
	clock_t ticks;

	// Define a LinkedList to hold all the courses
	LinkedList *courseTable = new LinkedList;

	bool running = true; // Flag to control the menu loop

	while (running)
	{ // Loop until the user chooses to exit
		// Display the menu
		cout << "Menu" << endl;
		cout << "-----------" << endl;
		cout << "1. Load Data Structure" << endl;
		cout << "2. Print Course List" << endl;
		cout << "3. Print Course" << endl;
		cout << "4. Exit" << endl;
		cout << "Enter your choice (1-4): ";

		// Get the user's choice
		std::string input;
		std::getline(std::cin, input); // Read the entire line
		std::stringstream ss(input);   // Create a stringstream from the input

		int choice;
		if (!(ss >> choice) || choice < 1 || choice > 4)
		{
			cout << "Invalid input. Please enter a number between 1 and 4." << endl;
			continue;
		}
		// Accept the user's choice and process it
		switch (choice)
		{

		case 1:
		{
			// Initialize a timer variable before loading courses
			ticks = clock();

			// Complete the method call to load the courses
			loadCourses(csvPath, courseTable);
			// Calculate elapsed time and display result
			ticks = clock() - ticks; // current clock ticks minus starting clock ticks
			cout << "time: " << ticks << " clock ticks" << endl;
			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
			break;
		}
		case 2:
		{
			courseTable->PrintList();
			break;
		}
		case 3:
		{
			cout << "\n"
				 << "Enter course number: " << endl;
			string courseLookup;
			cin >> courseLookup;
			cin.clear();
			courseTable->PrintCourse(courseLookup);
			break;
		}
		case 4:
		{
			running = false;
			break;
		}
		default:
		{
			cout << "Invalid choice, please re-enter" << endl;
			choice = 0;
			break;
		}
		}
	}
	cout << "Good bye" << endl;
	return 0;
}
