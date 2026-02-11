#include <iostream>

constexpr size_t NUM_MAX_BOOKS = 10;

struct Book;

struct Person {
	Book* booksWritten[NUM_MAX_BOOKS];
	std::string name;

	Person(Book*[NUM_MAX_BOOKS], size_t, const std::string&);
};

struct Book {
	Person* author;
	std::string title;
	uint32_t numberOfPages;

	Book(Person*, const std::string&, uint32_t);
};

Person::Person(Book* books[NUM_MAX_BOOKS], size_t numStartingBooks, const std::string& name) {
	for (size_t i = 0; i < numStartingBooks; ++i) {
		if (books[i]) {
			this->booksWritten[i] = books[i];
		}
	}
	for (size_t i = numStartingBooks; i < NUM_MAX_BOOKS; ++i) {
		this->booksWritten[i] = nullptr;
	}

	this->name = name;
}

Book::Book(Person* author, const std::string& title, uint32_t pages) {
	this->author = author;
	this->title = title;
	this->numberOfPages = pages;
	if (this->author) {
		size_t idx = 0;
		while (author->booksWritten[idx++] != nullptr);
		author->booksWritten[idx - 1] = this;
	}
}

std::ostream& operator<<(std::ostream& os, const Book& book) {
	os << book.title << ", " << (book.author != nullptr ? book.author->name : "Unknown") << ", " << book.numberOfPages;
	return os;
}

std::ostream& operator<<(std::ostream& os, const Person& author) {
	os << author.name << '\n';
	size_t idx = 0;
	while (author.booksWritten[idx] != nullptr) {
		os << " - " << *author.booksWritten[idx] << (author.booksWritten[idx + 1] != nullptr ? "\n" : "");
		++idx;
	}
	return os;
}

int main() {
	Person king({}, 0, "Stephen King");
	Person tolkien({}, 0, "J.R.R. Tolkien");

	Book book1(&king, "It", 1024);
	Book book2(&king, "The Shining", 976);
	Book book3(&king, "Cujo", 450);
	Book book4(&tolkien, "Lord of the Rings: Fellowship of the Ring", 512);

	std::cout << king << '\n' << tolkien << '\n';
}
