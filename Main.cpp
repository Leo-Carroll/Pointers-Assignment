/****************************************************************
* Author: Leo Carroll
* Description:
*	A simple example program of pointers using books with
*	pointers to authors, and authors with an array of pointers
*	to books. It does not use a custom Vector implementation
*	to keep simplicity.
* Date Created: 2026-02-08
* Date Modified: 2026-02-20
****************************************************************/

#include <iostream>			// Included for std::ostream and std::cout.
#include <cstdint>			// Included for std::uint32_t.

// Maximum number of books written per author.
// It is marked as 'constexpr' as constexpr is better than defining.
constexpr std::size_t MAX_BOOKS_WRITTEN = 100;

struct Book;		// Forward declare the Book structure for use in the Person class.

// Create the person class which represents that author.
struct Person {
	Book* booksWritten[MAX_BOOKS_WRITTEN];		// A stack-allocated array of Book pointers using the MAX_BOOKS_WRITTEN variable.
	std::string name;		// The name of the author.

	// Default constructor
	Person();
	// Custom constructor
	Person(Book*, std::size_t, const std::string&);

	void AddBook(Book*);
};

struct Book {
	Person* author;					// Person pointer to the author of the book.
	std::string title;				// Title of the book.
	std::uint32_t numberOfPages;	// Number of pages in the book.

	// Custom constructor with default values. This allows you to basically default construct your book.
	Book(Person* = nullptr, const std::string& = "", std::uint32_t = 0);
};

// Default constructor
// Assigns default values to the members of the object.
Person::Person() {
	// Iterate over the booksWritten.
	for (std::size_t i = 0; i < MAX_BOOKS_WRITTEN; ++i) {
		this->booksWritten[i] = nullptr;		// Set every value in the array to nullptr.
	}
	this->name = "";		// Set the name of the person to an empty string.
}

// Custom constructor
// Takes a pointer to the first book in the array, the number of books in the array, and the name of the author.
Person::Person(Book* first, std::size_t numBooks, const std::string& name) {
	std::size_t idx = 0;		// Create the index variable outside of the for loop so that it can be saved for the next for loop.
	if (first != nullptr) {
		// Iterate over the array from the first pointer.
		for (idx = 0; idx < numBooks; ++idx) {
			// Set booksWritten at the index variable to the address of the element idx elements after the first pointer.
			this->booksWritten[idx] = &first[idx];
		}
	}
	// Iterate over the array from the idx to the maximum number of books.
	for (std::size_t i = idx; i < MAX_BOOKS_WRITTEN; ++i) {
		this->booksWritten[i] = {};		// Default construct the booksWritten array at index i
	}

	this->name = name;		// Copy the value in the name arg to this->name member.
}

// Book custom constructor
Book::Book(Person* author, const std::string& title, std::uint32_t pages) {
	this->author = author;			// Set this->author member to the author arg.
	this->title = title;			// Set this->title member to the title arg.
	this->numberOfPages = pages;	// Set this->numberOfPages to the pages arg.
}

void Person::AddBook(Book* book) {
	if (book) {
		// Loop while the index is less than the maximum books, and while the author's booksWritten[idx] is not nullptr.
		std::size_t idx = 0;		// Create an index variable to store the index of the first empty book.
		while (idx < MAX_BOOKS_WRITTEN && this->booksWritten[idx]) {
			++idx;		// Increment.
		}
		// Ensure that idx is less than the maximum books to prevent buffer overrun.
		if (idx < MAX_BOOKS_WRITTEN) {
			this->booksWritten[idx] = book;		// Add this book object to the author's booksWritten at the first available element.
		}
	}
	return;
}

// Book operator overload.
std::ostream& operator<<(std::ostream&, const Book&);
// Person operator overload.
std::ostream& operator<<(std::ostream&, const Person&);

int main() {
	Person king(nullptr, 0, "Stephen King");		// Create a Person to hold Stephen King's books.
	Person tolkien(nullptr, 0, "J.R.R. Tolkien");	// Create a Person to hold J.R.R. Tolkien's books.

	// Create the Book variables for both authors. Note that the books are automatically added to the author's books in the constructor.
	Book book1(&king, "It", 1024);
	Book book2(&king, "The Shining", 976);
	Book book3(&king, "Cujo", 450);
	Book book4(&tolkien, "The Lord of the Rings: Fellowship of the Ring", 512);

	king.AddBook(&book1);
	king.AddBook(&book2);
	king.AddBook(&book3);
	tolkien.AddBook(&book4);

	// Call the output operator overload for king and tolkien.
	std::cout << king << "\n" << tolkien;
}

// Book output operator overload
std::ostream& operator<<(std::ostream& os, const Book& book) {
	// Output the book's contents, check if the author is nullptr before outputting the author's name, and output the number of pages.
	// Note that this is a good candidate for std::print, but I have decided to use the standard way for the sake of portability.
	os << book.title << ", " << (book.author ? book.author->name : "Unknown") << ", " << book.numberOfPages << " pages";
	return os;		// Return the output stream.
}

// Person output operator overload
std::ostream& operator<<(std::ostream& os, const Person& person) {
	os << person.name;		// Output the person's name
	std::size_t idx = 0;		// Create an idx variable to hold the index.
	// Loop while the idx is less than the maximum books, and while the person's booksWritten is not nullptr, indicating that there are no more books.
	while (idx < MAX_BOOKS_WRITTEN && person.booksWritten[idx] != nullptr) {
		os << "\n - " << *person.booksWritten[idx++];		// Call the Book output operator overload on the dereferenced Book pointer at index idx.
	}
	return os;		// Return the output stream.
}
