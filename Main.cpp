/******************************************************
* Author: Leo Carroll
* Program Description:
*	A simple demonstration of pointers, operator 
*	overloading, and forward declaration.
*	It uses a custom Vector implementation as use of
*	std::vector is prohibited.
******************************************************/

#include "Vector.h"		// Include Vector.h for the custom Vector implementation.

#include <iostream>		// For std::cout.

struct Book;		// Forward declare the Book struct so that it can be used in the Person structure.

struct Person {
	std::string name;				// Author's name.
	Vector<Book*> booksWritten;		// Books authored by this person.

	// Default Constructor: Initialize name and optional books vector.
	Person(const std::string& name, const Vector<Book*>& books = {})
		: name(name), booksWritten(books) {}

	// Note that this does not specifically need to be marked as a friend as there are no private members. It is still good to mark it for if private members are added.
	friend std::ostream& operator<<(std::ostream&, const Person&);
};

// Initialize a Person (Assigment requires this, so I added it just in case).
static void InitPerson(Person* person, const std::string& name, const Vector<Book*>& booksWritten) {
	person->name = name;		// Set the author's name.
	person->booksWritten = booksWritten;		// Set author's books.
}

struct Book {
	Person* author;				// Hold a pointer to the author to represent the author of the book.
	std::string title;			// The title member is used to store the title of the book.
	uint32_t numberOfPages;		// The numberOfPages member is used to store the pages in the book. It is an unsigned int because it cannot be 0, and uint16_t is kind of small.

	// Default Constructor: initialize author, title, pages, and automatically add the book to the author's book Vector.
	Book(Person* author, const std::string& title, uint32_t pages)
		: author(author), title(title), numberOfPages(pages) {
		// Check if the author is nullptr.
		if (author) {
			author->booksWritten.PushBack(this);		// Automatically push the created book to the back of the author's booksWritten.
		}
	}

	friend std::ostream& operator<<(std::ostream&, const Book&);		// Friend the output operator overload. This doesn't have to be done as this there are no private members, but it is good for if any private members are added.
};

// Function to initialize a Book. The assignment required a function like this, so here it is.
static void InitBook(Book* book, Person* author, const std::string& title, uint32_t numPages) {
	book->author = author;				// Set the book's author pointer.
	book->title = title;				// Set the book's title member.
	book->numberOfPages = numPages;		// Set the book's numberOfPages member.
	// Check if the author is nullptr.
	if (author) {
		author->booksWritten.PushBack(book);		// Push the book to the back of the author's booksWritten Vector.
	}
}

// Book Operator Overload: Output the formatted contents of the book to the given output stream.
std::ostream& operator<<(std::ostream& os, const Book& book) {
	os << book.title << ", " << ((book.author) ? book.author->name : "Unknown") << ", " << book.numberOfPages << " pages";
	return os;
}

// Person Operator Overload: Outputs the Person's name, and the books that they have written.
std::ostream& operator<<(std::ostream& os, const Person& person) {
	os << person.name << "(Total Books: " << person.booksWritten.Size() << ")\n";
	// Iterate over the books written by the person.
	for (size_t i = 0; i < person.booksWritten.Size(); ++i) {
		os << " - " << *person.booksWritten.At(i) << '\n';
	}
	return os;
}

int main() {
	// Create authors on the heap. Note that this is a good use-case for std::unique_ptr.
	Person* king = new Person("Stephen King");
	Person* tolkien = new Person("J.R.R. Tolkien");

	// Create books on the heap and automatically link them to the authors in the constructor.
	Book* book1 = new Book(king, "It", 1024);
	Book* book2 = new Book(king, "The Shining", 976);
	Book* book3 = new Book(king, "Cujo", 450);
	Book* book4 = new Book(tolkien, "Lord of the Rings: Fellowship of the Ring", 512);

	// Display authors and their books.
	std::cout << *king << '\n' << *tolkien;

	// Free the heap-allocated memory to prevent memory leaks (Delete the books first to prevent undefined behaviour for the authors).
	delete book1;
	delete book2;
	delete book3;
	delete book4;
	delete king;
	delete tolkien;
}
