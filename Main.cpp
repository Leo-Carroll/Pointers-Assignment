#include <iostream>

constexpr std::size_t MAX_BOOKS_WRITTEN = 10;

struct Book;

struct Person {
	Book* booksWritten[MAX_BOOKS_WRITTEN];
	std::string name;

	Person(Book*, std::size_t, const std::string&);
};

struct Book {
	Person* author;
	std::string title;
	std::uint32_t numberOfPages;

	Book(Person*, const std::string&, std::uint32_t);
};

Person::Person(Book* books, std::size_t numBooks, const std::string& name) {
	std::size_t idx = 0;
	if (books != nullptr) {
		for (idx = 0; idx < numBooks; ++idx) {
			this->booksWritten[idx] = &books[idx];
		}
	}
	for (std::size_t i = idx; i < MAX_BOOKS_WRITTEN; ++i) {
		this->booksWritten[i] = {};
	}

	this->name = name;
}

Book::Book(Person* author, const std::string& title, std::uint32_t pages) {
	this->author = author;
	this->title = title;
	this->numberOfPages = pages;

	if (this->author) {
		size_t idx = 0;
		while (this->author->booksWritten[idx]) ++idx;
		this->author->booksWritten[idx] = this;
	}
}

std::ostream& operator<<(std::ostream&, const Book&);
std::ostream& operator<<(std::ostream&, const Person&);

int main() {
	Person king(nullptr, 0, "Stephen King");
	Person tolkien(nullptr, 0, "J.R.R. Tolkien");

	Book book1(&king, "It", 1024);
	Book book2(&king, "The Shining", 976);
	Book book3(&king, "Cujo", 450);
	Book book4(&tolkien, "The Lord of the Rings: Fellowship of the Ring", 512);

	std::cout << king << "\n" << tolkien;
}

std::ostream& operator<<(std::ostream& os, const Book& book) {
	os << book.title << ", " << (book.author ? book.author->name : "Unknown") << ", " << book.numberOfPages << " pages";
	return os;
}

std::ostream& operator<<(std::ostream& os, const Person& person) {
	os << person.name;
	size_t idx = 0;
	while (person.booksWritten[idx] != nullptr) {
		os << "\n - " << *person.booksWritten[idx++];
	}
	return os;
}
