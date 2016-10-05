# C++ Notes

## 6. Types and Declarations

* Initialize variables with **{}** (list initializer) instead of traditional **=** initializaer. List initializers check for precission lose and unsafe implicit type casts. They can also be customized for user defined objects.
* When no argument is supplied to a list initializer, an object is initialized with its default constructor or default value.

```c++
class Person {
public:
	Person(): ag(21) { } // Default age is 21
	unsigned short age() { return this->ag; }
private:
	unsigned short ag;
};


Person p {}; // Initialize with an empty list initializer
std::cout << "age is " << p.age() << std::endl; // age is 21

```

* * is prefix, **[]** and **()** are postfix; the postfix declarator operators bind tighter than the prefix ones.
* When it comes to arrays and pointers, read from right to left.

```c++
char *arr[]; // array of pointers to **char**
char( *arr)[]; // Pointer to an array of char
```

## 7. Pointers, Arrays, and References

### 7.7 References
* A reference is an alias for an object, and does not impose performance overhead compared to pointers.

``` c++
void increment(int& num) { ++num; }

int x = 5;
increment(x);
std::cout << "x is " << x << std::endl; // x is 6

```

* Differences with pointers:
	* Access with the same syntax as accessing the object.
	* Always refers to the object it was initialized with.
	* There's no "null reference".

#### 7.7.1 Lvalue References