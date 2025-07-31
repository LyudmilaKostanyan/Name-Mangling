#include <iostream>

class Demo {
public:
	void print(int x);
	void print(const int* x);
	void print(unsigned int& x);
	void print(const unsigned int& x);
	void print(long* x);
	void print(short x);
	void print(double x);
	void print(bool s);
	void print(char c);
	void print(std::string s);
	void print(const char* s);
};

void Demo::print(int x) {
		std::cout << "Int: " << x << std::endl;
	}

void Demo::print(const int* x) {
	std::cout << "Const Int* : " << *x << std::endl;
}

void Demo::print(unsigned int& x) {
	std::cout << "Unsigned Int& : " << x << std::endl;
}

void Demo::print(const unsigned int& x) {
	std::cout << "Const Unsigned Int& : " << x << std::endl;
}

void Demo::print(long* x) {
	std::cout << "Long* : " << *x << std::endl;
}

void Demo::print(short x) {
	std::cout << "Short: " << x << std::endl;
}
	
void Demo::print(double x) {
	std::cout << "Double: " << x << std::endl;
}

void Demo::print(bool s) {
	std::cout << "Bool: " << std::boolalpha << s << std::endl;
}

void Demo::print(char c) {
	std::cout << "Char: " << c << std::endl;
}

void Demo::print(std::string s) {
	std::cout << "String: " << s << std::endl;
}

void Demo::print(const char* s) {
	std::cout << "C-string: " << s << std::endl;
}

int main() {
	Demo d;
	const int x = 10;
	const int* const px = &x;
	unsigned int y = 20;
	const unsigned int& cy = y;
	long longValue = 30L;

	d.print(x);
	d.print(px);
	d.print(y);
	d.print(cy);
	d.print(&longValue);
	d.print(static_cast<short>(50));
	d.print(3.14);
	d.print(true);
	d.print('A');
	d.print("C-style string");
	d.print(std::string("C++ string"));
}
