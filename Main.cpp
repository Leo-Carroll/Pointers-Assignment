/******************************************************
* Author: Leo Carroll
* Program Description:
*	A simple demonstration of pointers, operator 
*	overloading, and forward declaration.
*	It uses a custom Vector implementation as use of
*	std::vector is prohibited.
******************************************************/

#include "Vector.h"		// Include Vector.h for the custom Vector implementation.

#include <iostream>		// Include iostream for the std::cout stream.

struct Book;		// Forward declare the Book struct so that it can be used in the Person structure.

struct Person {
	std::string name;				// The name member is used to store the name of the author.
	Vector<Book*> booksWritten;		// The booksWritten member is used to store a Vector of Book pointers to represent the books written by the author.

	// Default Constructor
	// Takes in a name and const reference to a Vector of Book pointers that is defaulted to an empty Vector.
	Person(const std::string& name, const Vector<Book*>& books = {})
		: name(name), booksWritten(books) {}		// Assign the members to their corresponding values given by the params.

	friend std::ostream& operator<<(std::ostream&, const Person&);		// Friend the output operator overload. This doesn't have to be done as there are no private members, but it is good for if any private members are added.
};

// InitPerson() Function
// The assignment requested an initialize function, so here it is.
static void InitPerson(Person* person, const std::string& name, const Vector<Book*>& booksWritten) {
	person->name = name;		// Set the person pointer's name to the given name.
	person->booksWritten = booksWritten;		// Set the person's booksWritten to the given booksWritten.
}

struct Book {
	Person* author;				// Hold a pointer to the author to represent the author of the book.
	std::string title;			// The title member is used to store the title of the book.
	uint32_t numberOfPages;		// The numberOfPages member is used to store the pages in the book. It is an unsigned int because it cannot be 0, and uint16_t is kind of small.

	// Default Constructor
	// Takes in a Person pointer for the author, a const reference to a string for the title, and a uint32_t for the pages.
	Book(Person* author, const std::string& title, uint32_t pages)
		: author(author), title(title), numberOfPages(pages) {		// Assign the members to their corresponding values given by the params.
		// Check if the author is nullptr.
		if (author) {
			author->booksWritten.PushBack(this);		// Automatically push the created book to the back of the author's booksWritten.
		}
	}

	friend std::ostream& operator<<(std::ostream&, const Book&);		// Friend the output operator overload. This doesn't have to be done as this there are no private members, but it is good for if any private members are added.
};

// InitBook() Function
// The assignment requested an initialize function, so I implemented one.
static void InitBook(Book* book, Person* author, const std::string& title, uint32_t numPages) {
	book->author = author;				// Set the book's author pointer to the given author pointer.
	book->title = title;				// Set the book's title member to the given title.
	book->numberOfPages = numPages;		// Set the book's numberOfPages member to the given numPages.
}

// Book Operator Overload
// Output the formatted contents of the book to the given output stream.
std::ostream& operator<<(std::ostream& os, const Book& book) {
	os << book.title << ", " << book.author->name << ", " << book.numberOfPages << " pages";		// Output the formatted contents of the book to the output stream.
	return os;		// Return the output stream.
}

// Person Operator Overload
// Outputs the Person's name, and the books that they have wrote.
std::ostream& operator<<(std::ostream& os, const Person& person) {
	os << person.name << "(Total Books: " << person.booksWritten.Size() << ")\n";		// Output the Person's name.
	// Iterate over the books written by the person.
	for (size_t i = 0; i < person.booksWritten.Size(); ++i) {
		os << " - " << *person.booksWritten.At(i) << '\n';		// Output the books written by the person with a '-' in front, and an endline after.
	}
	return os;		// Return the output stream.
}

// main() Function
// This is the program's starting point.
int main() {
	// Note that for the following heap-allocated variables, I would normally use std::unique_ptr, but for the sake of simplicity I am not.
	Person* king = new Person("Stephen King");			// Heap-allocate a new Person pointer with the name "Stephen King" and no initial booksWritten Vector.
	Person* tolkien = new Person("J.R.R. Tolkien");		// Heap-allocate a new Person pointer with the name "J.R.R. Tolkien" and no initial booksWritten Vector.

	Book* book1 = new Book(king, "It", 1024);		// Heap-allocate a new Book pointer with the author king, the name "It", and 1024 pages.
	Book* book2 = new Book(king, "The Shining", 976);		// Heap-allocate a new Book pointer with the author king, the name "The Shining", and 976 pages.
	Book* book3 = new Book(king, "Cujo", 450);		// Heap-allocate a new Book pointer with the author king, the name "Cujo", and 450 pages.
	Book* book4 = new Book(tolkien, "Lord of the Rings: Fellowship of the Ring", 512);		// Heap-allocate a new Book pointer with the author tolkien, the "Lord of the Rings: Fellowship of the Ring", and 512 pages.name 

	std::cout << *king << *tolkien;			// Call the output overloads of the two authors. They do not need to be seperated by newlines as newlines are already printed in the overload.

	// Free the heap-allocated memory to prevent memory leaks.
	// Delete the books first to prevent undefined behaviour for the authors.
	delete book1;		// Delete book1.
	delete book2;		// Delete book2.
	delete book3;		// Delete book3.
	delete book4;		// Delete book4.

	delete king;		// Delete king.
	delete tolkien;		// Delete tolkien.
}
