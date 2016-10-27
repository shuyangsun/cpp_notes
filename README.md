# C++ Notes

___


## 6. Types and Declarations

#### 6.3.1 The Structure of Declarations

* * is prefix, **[]** and **()** are postfix; the postfix declarator operators bind tighter than the prefix ones.

```c++
char *arr[]; // Array of pointers to char
char( *arr)[]; // Pointer to an array of char
```

* When it comes to arrays and pointers, read from right to left.

#### 6.3.3 Names
* Nonlocal names starting with an underscore are reserved for special facilities in the implementation and the run-time environment, so such names should not be used in application programs. Similarly, names starting with a double underscore (__) or an underscore followed by an uppercase letter (e.g., _Foo) are reserved.
* Use all capitals for macros and never for non-macros (not even for non-macro constants). Use underscores to separate words in an identifier.
* Language and the standard library use lowercase for types; this can be seen as a standard.

#### 6.3.4 Scope
* A hidden global name can be referred to using the scope resolution operator **::**.

```c++
int x = 6;

void foo() {
	int x = 1;
	x = 2; // Local x := 2
	::x = 8; // Global x := 8
}
```

#### 6.3.5 Initialization

* Initialize variables with **{}** (list initializer) instead of traditional **=** initializer.
* List initializers does not allow narrowing (check for precision lose and unsafe implicit type casts). They can also be customized for user defined objects.
* When no argument is supplied to a list initializer, an object is initialized with its default constructor or default value.

```c++
class person {
public:
	person(): ag(21) { } // Default age is 21
	unsigned short age() { return this->ag; }
private:
	unsigned short ag;
};

// Initialize with empty list initializers
int x {};
person p {};

std::cout << "x is " << x << std::endl; // x is 0
std::cout << "age is " << p.age() << std::endl; // age is 21

```

* Local variables and objects created on the free store are not initialized by default unless they are of user-defined types with a default constructor.
* Prefer **=** rather than **{}** when using **auto**.

#### 6.3.6 Deducing a Type: auto and decltype()
* **auto** and **decltype()** simply report the type of an expression already known to the compiler.
* **decltype(expr)** is the type of **expr**, return type of **expr** must can be known by the compiler.

___
## 7. Pointers, Arrays, and References

#### 7.4.2 Multidimensional Arrays
* Sizes of all dimensions for variables of n-dimensional array has to be known by the compiler

```c++
int arr1[][]; // error: array has incomplete element type 'int []'
int arr2[][5]; // error: definition of variable with array type needs an explicit size or an initializer
int arr3[3][5]; // OK
```

* When n-dimensional array is used as function argument, only the last n - 1 dimensions' sizes has to be known by the compiler.

```c++
void foo1(int arr[][]); // error: array has incomplete element type 'int []'
void foo2(int arr[][5]); // OK
void foo3(int arr[5][]); // error: array has incomplete element type 'int []'
void foo4(int arr[][][5]); // error: array has incomplete element type 'int [][5]'
void foo5(int arr[][3][5]); // OK
```

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
* **T&** lval;
* The value of a reference cannot be changed after initialization.
* Cannot define an array of references.
* The initializer for a "plain" **T&** must be an lvalue of type **T**.

#### 7.7.2 Rvalue References
* **T&&** rval;
* Both a **const** lvalue reference and a rvalue reference can bind to an rvalue, but their purposes are fundamentally different:
	* We use rvalue references to implement a "destructive read" for optimization of what would otherwise have required a copy.
	* We use a **const** lvalue reference to prevent modification of an argument.
* Use **move()** instead of **static_cast\<T&&\>(x)** to get a rvalue reference.

#### 7.7.4 Pointers and References
* If you need to change which object to refer to, use a pointer.
* If you want to be sure that a name always refers to the same object, use a reference.
* If you want to use a user-defined (overloaded) operator on something that refers to an object, use a reference.

___
## 8. Structures, Unions, and Enumerations

### 8.2 Structures
* There's a terminating **;** (semicolon) at the end of structure definition, each member is also terminated by **;**.
* Structs can be initialized using **{}** notation.

```c++
struct person {
	const char *first_name;
	const char *last_name;
	unsigned short age;
};

person me {"Wolve", "Rine", 658};
```

#### 8.2.1 struct Layout
* Members are allocated in memory in declaration order.
* The size of an object of a **struct** is not necessarily the sum of the sizes of it's members, because they have to be properly *aligned* in the machine memory.
* Can minimize wasted space by simply ordering members by size (largest member first).
* Use of multiple access specifiers (i.e., **public**, **private**, or **protected**) can affect layout.

```c++
// Allocation 1 (on Intel based macOS, assuming word size is 8 bytes)
struct product {
	short year_released;
	const char *name;
	short month_released;
};
// Total: 3 words
```

| Member | Memory Space | Number of Bytes |
| --- | --- | --- |
| year_released | == | 2 |
| name | ======== | 8 |
| month_released | == | 2 |

```c++
// Allocation 2 (on Intel based macOS, assuming word size is 8 bytes)
struct product {
	const char *name;
	short year_released;
	short month_released;
};
// Total: 2 words
```

| Member | Memory Space | Number of Bytes |
| --- | --- | --- |
| name | ======== | 8 |
| (year\_released, month\_released) | ==== | 4 |


#### 8.2.2 struct Names
* The name of a type becomes available for use immediately after it has been encountered and not just after the complete declaration has been seen.
* It is not possible to declare new objects of a **struct** until its complete declaration has been seen, because the compiler is not able to determine the size of that **struct** object (but is able to for the pointer to that **struct** object).

```c++
struct node {
	node* next; // OK, it's a pointer to node.
};

struct node_bad {
	node_bad next; // NOT OK, it's an object of node_bad.
};
```

* To allow multiple **struct**s to refer to each other, we can declare a name to be the name of a **struct**.

```c++
struct lion; // Declaration, will be defined later.

struct deer {
	lion* eaten_by;
};

struct lion {
	deer* lunch;
};
```

* It's possible to declare a **struct** and a non-**struct** with the same name in the same scope.
* Use **struct** <*name*> to refer to the **struct** type, and plane <*name*> to refer to the other.
* Avoid overloading names.

```c++
struct stat { /* ... */ }
int stat(char *name, struct stat* buf);
```

#### 8.2.3 Structures and Classes
* A **struct** is simply a **class** where the members are public by default. So, a **struct** can have member functions. In particular, a **struct** can have constructors.
* Use list initializer to initialize the structure with default values, even if no constructor is defined

```c++
struct person {
	const char *first_name;
	const char *last_name;
	unsigned short age;
	person(const char *f_name, const char *l_name, const unsigned short ag);
};

person::person(const char *f_name, const char *l_name, const unsigned short ag):
	first_name{f_name},
	last_name{l_name}
{
	if (ag < 150) {
		age = ag;
	} else {
		std::cerr << "Trying to initialize an alien." << std::endl;
		exit(1);
	}
}

person me {"Shuyang", "Sun", 23};
person alien {"E", "T", 290};
```

#### 8.2.5 Type Equivalence
* Two structs are different even when they have the same members.

#### 8.2.6 Plain Old Data
* POD can be copied very efficiently by using block move machine instructions (e.g., **std::memcpy()**).
* To be a POD, the object must:
	* Not have a complicated layout (e.g., with a **vptr**).
	* Not have nonstandard (user-defined) copy semantics.
	* Have a trivial default constructor.
* **is_pod** in standard-library defined in **\<type_traits\>** allows us to ask if a type is POD.

```c++
struct S1 { int x; };
struct S2 { int x; S2(int num): x{num} {}; }; // Has customized constructor, no default constructor.
struct S3 { int x; S3(int num): x{num} {}; S3() {}; }; // Has default constructor.
	
std::cout << std::is_pod<int>::value << std::endl; // 1
	
std::cout << std::is_pod<S1>::value << std::endl; // 1
std::cout << std::is_pod<S2>::value << std::endl; // 0
std::cout << std::is_pod<S3>::value << std::endl; // 0
```

#### 8.2.7 Fields
* Fields (also called *bit-field*) are members of struct field by specified number of bits it is to occupy.

```c++
struct color_16 {
	const char* name;
	bool is_visible: 1; // Occupies 1 bit
	unsigned short r: 4; // Occupies 4 bits
	unsigned short g: 4; // Occupies 4 bits
	unsigned short b: 4; // Occupies 4 bits
};
```

* Using fields to pack several variables into a single byte does not necessarily save space. It saves data space, but the size of the code needed to manipulate these variables increases on most machines. Programs have been known to shrink significantly when binary variables were converted from bit-fields to characters! Furthermore, it is typically much faster to access a **char** or an **int** than to access a field.

### 8.3 Unions
* A **union** is a **struct** in which all members are allocated at the same address so that the **union** occupies only as much space as its largest member.
* Naturally, a **union** can hold a value for only one member at a time.

```c++
union value {
	const char *s;
	long double d;
	bool i;
};

value val;
val.i = 6;
std::cout << sizeof(value) << std::endl; // 16
std::cout << val.i << std::endl; // 1
```

#### 8.3.1 Unions and Classes
* **union** has a lot of restrictions comparing to **class**.
* **union** cannot have or be used as a base class.
* If a **union** has a member with a user-defined constructor, a copy operation, a move operation, or a destructor, then that special function is deleted for that **union**; that is, it cannot be used for an object of the **union** type.
* It is possible to specify an in-class initializer for at most one member. If so, this initializer will be used for default initialization.

```c++
union zip_code {
	unsigned int int_val;
	const char* str_val {""};
};

zip_code nyc; // Default initialized to str_val == ""
std::cout << "nyc zip code is: " << nyc.str_val << std::endl; // nyc zip code is:
nyc.int_val = 10010;
std::cout << "nyc zip code is: " << nyc.int_val << std::endl; // nyc zip code is: 10010
	
zip_code ames {50010}; // error: no matching constructor for initialization of 'zip_code'

```

#### 8.3.2 Anonymous unions
* An anonymous **union** is an object, not a type, and its members can be accessed without mentioning an object name.
* We can use members of an anonymous **union** exactly as we use other members of a class - as long as we remember that **union** members really can be used only one at a time.

```c++
class address {
private:
	const char *street_addr;
	union { // Anonymous union
		const char *zip_str;
		unsigned int zip_int;
	};
	bool zip_is_str;

public:
	address(const char* addr, const char* zip):
		street_addr{addr},
		zip_str{zip},
		zip_is_str{true} { };
	
	address(const char* addr, const unsigned int zip):
		street_addr{addr},
		zip_int{zip},
		zip_is_str{false} { };
	
	std::string street_address() {
		return std::string(street_addr);
	};
	
	std::string zip_code() {
		if (zip_is_str) {
			return std::string(zip_str);
		} else {
			return std::to_string(zip_int);
		}
	};
};

address home {"Mars", "MRS-265128"};
address work {"NYC", 10010};
std::cout << home.street_address() << ", " << home.zip_code() << std::endl; // Mars, MRS-265128
std::cout << work.street_address() << ", " << work.zip_code() << std::endl; // NYC, 10010
```

### 8.4 Enumerations
* An *enumeration* is a type that can hold a set of integer values specified by the user.
* Some of an enumeration's possible values are named and called *enumerators*.
* Cannot initialize an *enumerator* with list initializer (but can with **enum**s).
* There are two kinds of enumerations:
	1. **enum class**es, for which the enumerator names are *local* to the **enum** and their values do *not* implicitly convert to integers.
	2. "Plain **enum**s", for which the enumerator names are in the *same scope* as the **enum** and their values implicitly convert to integers.

```c++
enum class color {
	red, green, blue // Must use "color::red" to access; NOT converted to integers.
};

color c1 {blue}; // error: use of undeclared identifier 'blue'
color c2 {color::blue}; // OK

std::cout << c2 << std::endl;
// error: invalid operands to binary expression ('ostream' (aka 'basic_ostream<char>') and 'color')

enum sorting_order {
	ascending=9, descending // Can be accessed directly; converted to integers, 0 is default, and goes up.
};

sorting_order order1 {sorting_order::ascending}; // OK
sorting_order order2 {descending}; // OK
std::cout << order1 << std::endl; // 9
std::cout << order2 << std::endl; // 10
```

#### 8.4.1 enum classes
* An enumeration is represented by some integer type and each enumerator by some integer value.
* The type used to represent an enumeration is called its *underlying type*.
* The underlying type must be one of the signed or unsigned integer types; the default is **int**.
* Even if there is an underlying type, it will not be implicitly converted; to get the value of underlying type, use casting.

```c++
enum class color: char {
	red='r', green='g', blue='b'
};

color light {color::red};
std::cout << light << std::endl;
// error: invalid operands to binary expression ('ostream' (aka 'basic_ostream<char>') and 'color')
std::cout << static_cast<char>(light) << std::endl; // r
```

* Using bit-wise operators (e.g., **|**, **&**) needs explicit casting of **enum class**.

```c++
enum class options {
	a = 1, b = 2, c = 4, d = 8
};
	
options opt1 {options::a};
options opt2 {options::c};
std::cout << (opt1 | opt2) << std::endl;
// error: invalid operands to binary expression ('options' and 'options')
std::cout << (static_cast<int>(opt1) | static_cast<int>(opt2)) << std::endl; // 5
```

* Even if two enumerators of **enum class**es have the same underlying value, they cannot be compared with each other (which is not true for plain **enum**).

```c++
enum class light {
	red, green, yellow
};
	
enum class color {
	red, green, blue
};
	
light red_light {light::red};
color red_color {color::red};
bool same = red_light == red_color;
// warning: comparison of two values with different enumeration types ('light' and 'color')


enum state {
	new_york, iowa, illinois
};

enum city {
	new_york, // error: redefinition of enumerator 'new_york'
	nyc, // OK
	ames, chicago
};

state new_york_state {new_york};
city new_york_city {nyc};
bool same = new_york_city == new_york_state; // true (NOT GOOD!)
```

* Bit-wise operations for plain **enum**s does not need explicit casting, since they are implicitly converted to their underlying types.

#### 8.4.3 Unnamed enums
* A plain **enum** can be unnamed, we use that when all we need is a set of integer constants, rather than a type to use for variables.

```c++
enum { up=1, left, down, right };
```

___

## 9. Statements

### 9.1 Introduction
* Expressions have values, statements don't.

### 9.2 Statements Summary
* A (possibly empty) sequence of statements within "curly braces" (i.e., **{** and **}**) is called a *block* or a *compound statement*.
* A name declared in a block goes out of scope at the end of its block.

#### 9.4.2 switch Statements
* No need for terminating semicolon at the end of **switch**-statement.
* **switch**-statements needs **break** at the end of each **case**, otherwise it'll fall through like C (unlike Swift).

##### 9.4.2.1 Declarations in Cases
* It is possible, and common, to declare variables within the block of a **switch**-statement. However, it is not possible to bypass an initialization.

```c++
void foo(int x) {
	switch (x) {
		case 0:
			int y {9}; // error: declaration can be bypassed
			int y; // OK, but really bad
			y = 9;
			std::cout << y << std::endl;
			break;
		case 5:
			++x;
			++y; // Bad, accessing uninitialized variable
			std::cout << x << " " << y << std::endl; // 
			break;
		default:
			break;
	}
}

// If executed in (5, 0) order:
foo(0); // 9
foo(5); // 6 10

// If executed in (5, 0) order:
foo(5); // 6 289686817
foo(0); // 9
```

* If we need a variable within a **switch**-statement, we can limit its scope by enclosing its declaration and its use in a block.

#### 9.4.3 Declaration in Conditions

* Purpose: limit the scope of that variable to only that block.
* A declaration in a condition must declare and initialize a single variable or **const**.

```c++
int x {5};
if (bool even = is_even(x)) {
	std::cout << "x is even, is_even = " << even << std::endl;
} else {
	std::cout << "x is odd, is_even = " << even << std::endl;
}
// output: x is odd, is_even = 0
```

#### 9.5.1 Range-for Statements
* For-each loop


```c++
for (T x: v) { /* ... */ }
for (T& x: v) { /* Use reference to modify elements in the vector. */ }
for (const T& x: v) { /* Use const reference for potentially large elements. */ }
```

### 9.6 goto Statements
* Use **goto** only to jump forward to exit complicated loops, otherwise it's too confusing (even better: not using **goto** at all).

```c++
for (const T& x: v) {
	for (/* another for loop */) {
		for (/* and... one more */) {
			// Find element...
			if (found) {
				goto found; // Source of evil code!!!
			}
			// Some other stuff...
		}
		// Some other stuff...
	}
	// Some other stuff...
}

found:
// Blah blah blah...
```

### 9.7 Comments and Indentation
* **/*** **\*/** style comments do NOT nest (unlike Swift).


___

## 10. Expressions

#### 10.3.1 Results
* C++ follows "the usual arithmetic conversions": to produce a result of the "largest" operand type.
* Assignment is an expression.
* Underflow, overflow, and division by zero do not throw standard exceptions.

#### 10.3.2 Order of Evaluation

```c++
int i = 1;
v[i] = i++; // undefined result
// May be evaluated as v[1] = 1 or v[2] = 1, or other undefined result
```

#### 10.3.3 Operator Precedence
* If you start feeling the need for parentheses, you might consider breaking up the expression by using an extra variable.

#### 10.3.4 Temporary Objects
* A temporary object is created by compiler to hold an intermediate result of an expression. (e.g., **val = x + y * z**, there is a temporary project to hold the value of **y * z**)
* Unless bound to a reference or used to initialize a named object, a temporary object is destroyed at the end of the full expression in which is was created. (sometimes this does not work well with certain ways of writing code)

```c++
std::string str1 = "Hello, ";
std::string str2 = "world!";
// c_str() is holding a reference to (str1 + str2), which may be deleted at the end of expression.
const char *c_str = (str1 + str2).c_str();
std::cout << c_str << std::endl;
```

* Solution to this problem: bind the variables (in this case, **str1** and **str2**) to a **const** reference.

### 10.4 Constant Expressions
* C++ offers two related meanings of "constant":
	* **constexpr**: Evaluate at compile time (enable and ensure compile-time evaluation).
	* **const**: Do not modify in this scope (specify immutability in interfaces).
* A constant expression must start out with an integral value, a floating point value, or an enumerator, and we can combine those using operators and **constexpr** functions that in turn produce values.
* **constexpr** is a better choice for defining simple constants.

### 10.4.3 Literal Types
* A class with a **constexpr** constructor is called a *literal type*. To be simple enough to be **constexpr**, a constructor must have an empty body and all members must be initialized by potentially constant expressions.

```c++
struct Point {
	float x, y, z;
	// The "const" in the next line is not required for C++11, but is required for C++14.
	// C++11 implicitly implies the "const", so to be safe it should always be added.
	constexpr Point change(float dist) const {
		return {x + dist, y + dist, z + dist};
	}
	void print() const {
		std::cout<< "Point(" << x << ", " << y << ", " << z << ")" << std::endl;
	}
};

constexpr Point a {1, 2, 3};
constexpr Point b = a.change(9.63f);

a.print(); // Point(1, 2, 3)
b.print(); // Point(10.63, 11.63, 12.63)
```

#### 10.4.5 Address Constant Expressions
* The address of a statically allocated object, such as a global variable, is a constant.

```c++
constexpr const char *p1 = "asdf";
```

#### 10.5 Implicit Type Conversion
* *promotions*: preserve value.
* *narrowing*: not value preserving.
* The **{}**-initializer syntax prevents narrowing.
* **narrow_cast<>()** is a run-time checked conversion function. (use only if not avoidable or intended)
* Any pointer to an object type can be implicitly converted to a **void\***
* Pointers, integral, and floating-point values can be implicitly converted to **true**. A nonzero value is converted to **true**, a zero value is converted to **false**.
* When a floating-point value is converted to an integer value (that's big enough to hold the integer part), the fractional part is discarded.

___

## 11. Select Operations

#### 11.1.2 Bitwise Logical Operators
* They can be used on integral types (signed or unsigned).

### 11.2 Free Store
* **new** and **delete** are used to create/destroy objects on the *free store* respectively.
* An object created by **new** exists until it is explicitly destroyed by **delete**.
* The **delete** and **delete[]** operator may be applied only to a pointer returned by **new** or to the **nullptr**. Applying them to the **nullptr** has no effect.
* If the deleted object is of a class with a destructor, that destructor is called by **delete** before the object's memory is released for reuse.
* To deallocate space allocated by **new**, **delete** and **delete[]** must be able to determine the size of the object allocated (which requires extra space). This overhead is not significant when we allocate many objects or large objects, but it can matter if we allocate lots of small objects on the free store.
* A **return** or an exception thrown before the **delete** will cause a memory leak (unless even more code is added).

#### 11.2.1 Memory Management
* Main problems with free store: *leaked objects*, *premature deletion*, and *double deletion*.
* Don't put objects on the free store if you don't have to; prefer scoped variables.
* When you construct an object on the free store, place its pointer into a *manager object* (*handle*) with a destructor that will destroy it. (e.g., **string**, **vector**, all standard library containers, **unique_ptr**, and **shared_ptr**)
* Rule of thumb: no naked **new**s. **new** belongs in constructors and similar operations, **delete** belongs in destructors.

#### 11.2.2 Arrays
* **delete[]** is used to delete arrays created by **new**.

#### 11.2.3 Getting Memory Space
* The standard implementations of **operator new()** and **operator new\[]()** do not initialize the memory returned. They take arguments or return values of type **void***.

#### 11.2.4 Overloading new
* Placing an object in an area that is not (directly) controlled by the standard free-store manager implies that some care is required when destroying the object.

##### 11.2.4.1 nothrow new
* Use **nothrow** version of **new** and **delete** if no exceptions must be avoided.
* **nothrow** version of **new** returns **nullptr**, rather than throwing **bad_alloc** if the object cannot be allocated.

```c++
std::string* str_ptr {new(std::nothrow) std::string {}};
operator delete(str_ptr, std::nothrow);
```

### 11.3 Lists
* **{}**-list can be used as expressions in many (but not all) places:
	* *Qualified* by a type **T{...}**, meaning "create an object of type **T** initialized by **T{...}**".
	* *Unqualified* **{...}**, for which the type must be determined from the context of use.

#### 11.3.1 Implementation Model
* If the **{}**-list is used to construct an **initializer_list**, elements are typically copied from the **initializer_list** to wherever we use them. Not copied otherwise, except as by-value constructor arguments.

#### 11.3.3 Unqualified Lists
* When used as the initializer for a named object without the use of a **=**, an unqualified **{}**-list performs direct initialization. In all other cases, it performs copy initialization.

```c++
int x {8}; // initializer (direct initialization)
int y = {9}; // initializer (copy initialization)
```

* Use **initializer_list\<T>** to allow initializer lists for built-in or user-defined containers.

```c++
struct my_float {
    float val;
    
    my_float(const std::initializer_list<const std::string> init_lst) {
        const std::string& first {*init_lst.begin()};
        this->val = std::stof(first);
    }
    
    void print_floating_point() {
        std::cout << this->val - std::floor(this->val) << std::endl;
    }
};

float max_ele(const std::initializer_list<const float> init_lst) {
    float cur = std::numeric_limits<float>::min();
    for (auto ele: init_lst) {
        if (ele > cur) {
            cur = ele;
        }
    }
    return cur;
}

my_float f {"3.14159"};
f.print_floating_point(); // 0.14159
    
const float max {max_ele({1, 2.5, - 1.0, -9.9, 3.14159})};
std::cout << max << std::endl; // 3.14159
```

* The type of a **{}**-list can be deduced (only) if all elements are of the same type.
* Unfortunately, C++ does NOT deduce the type of an unqualified list for a plain template argument.

### 11.4 Lambda Expressions
* A lambda expression consists of a sequence of parts:
	* A possibly empty capture *list*.
	* An optional *parameter list*.
	* An optional **mutable** specifier.
	* An optional **noexcept** specifier.
	* An optional return type declaration of the form **->** type.
	* A *body*, specifying the code to be executed.

```c++
// A lambda function that captures output stream and precision.
// Then rounds the argument, prints it and returns it.
[&output_stream, precision](double x) -> double { // output_stream is stored as reference; precision is copy
	double rounded = my_round(x, precision);
	output_stream << rounded << std::endl;
	return rounded;
}
```

* Lambda expressions typically do not yield performance overhead (e.g., identical to replacing the code).

#### 11.4.3 Capture
* Lambda introducers:
	* **[]**: an empty capture list.
	* **[&]**: capture by reference.
	* **[=]**: capture by value.
	* **[***capture-list***]**: explicit capture; the *capture-list* is the list of names of local variables to be captured.
	* **[&,** *capture-list***]**: implicitly capture by reference all local variables with names NOT mentioned in the list.
	* **[=,** *capture-list***]**: implicitly capture by value all local variables with names NOT mentioned in the list.
* Local name preceded by **&** is always captured by reference and a local name not preceded by **&** is always captured by value.
* Lambda might outlive its caller.
* When passing a lambda to another thread, capturing by value (**[=]**) is typically best.

##### 11.4.3.2 Namespace Names
* Don't need to capture namespace variables (including global variables).

##### 11.4.3.3 Lambda and this
* Use **[this]** to capture member **this** from class.
* Members are always captured by reference (i.e., **[this]** and **[=]** are incompatible).

##### 11.4.3.4 mutable Lambdas

```c++
[] mutable { /* lambda body */ }
```

#### 11.4.5 The Type of a Lambda
* *Closure type* is unique to lambda, so no two lambdas have the same type.
* **std::function\<R(AL)>** where **R** is the lambda's return type and **AL** is its argument list of types.

```c++
// Example of a lambda function type (takes a double and an int, returns a boolean.)
std::function<bool (double, int)>
```
* A lambda that captures nothing can be assigned to a pointer to function of an appropriate type.

### 11.5 Explicit Type Conversion
* Named conversions:
	* **const_cast** for getting write access to something declared **const**
	* **static_cast** for reversing a well-defined implicit conversion
	* **reinterpret_cast** for changing the meaning of bit patterns
	* **dynamic_cast** for dynamically checked class hierarchy navigation
* Prefer **T{v}** conversions for well-behaved construction and the named casts (e.g., **static_cast**) for other conversions.

#### 11.5.1 Construction
* The construction of a value of type **T** from a value **e** can be expressed by the notation **T(e)**.


___

## 12. Functions

* The semantics of argument passing are identical to the semantics of copy initialization.

#### 12.1.2 Parts of a Function Declaration
* Function specifiers and modifiers:
	* The name of the function; required
	* The argument list, which may be empty **()**; required
	* The return type, which may be **void** and which may be prefix or suffix (using **auto**); required
	* **inline**, indicating a desire to have function calls implemented by inlining the function body
	* **constexpr**, indicating that it should be possible to evaluate the function at compile time if given constant expressions as arguments
	* **noexcept**, indicating that the function may not throw an exception
	* A linkage specification, for example, **static**
	* **[[noreturn]]**, indicating that the function will not return using the normal call/return mechanism
	* For member functions:
		* **virtual**, indicating that it can be overridden in a derived class
		* **override**, indicating that it must be overriding a virtual function from a base class
		* **final**, indicating that it cannot be overridden in a derived class
		* **static**, indicating that it is not associated with a particular object
		* **const**, indicating that it may not modify its object

#### 12.1.3 Function Definitions
* A **const** is ignored at the highest level of an argument type.

```c++
void foo(int); 			// type is void(int)
void bar(const int); 	// type is void(int)
```

* Other things can be called:
	* *Constructors* are technically not functions; in particular, they don't return a value, can initialize bases and members, and can't have their address taken.
	* *Destructors* can't  be overloaded and can't have their address taken.
	* *Function objects* are not functions (they are objects) and can't be overloaded, but their **operator()**s are functions.
	* *Lambda expressions* are basically a shorthand for defining function objects.

#### 12.1.4 Returning Values
* Can use suffix return type.

```c++
double square(double x);
auto square(double x) -> double; // equivalent to above line declaration

// Use for function template declaration, where the return type depends on the argument
template<class T, class U>
auto product(const vector<T>& x, const vector<U>& y) -> decltype(x * y);
auto square(const T& x) -> decltype(x * x);
```
* The semantics of function value return are identical to the semantics of copy initialization.
* The store is reused after the function returns, so a pointer to a local non-**static** variable should never be returned.
* Five ways to exit a function:
	* Use **return**.
	* Falling off the end in functions return **void** and **main()**.
	* Throwing an exception that isn't caught locally.
	* Terminating because an exception was thrown and not caught locally in a **noexcept** function.
	* Directly or indirectly invoking a system function that doesn't return.

#### 12.1.5 inline Functions
* **inline** specifier tells the compiler it should attempt to generate code inline rather than actually calling the function.

```c++
inline auto fact(const unsigned int n) -> unsigned long long {
    return (n < 2) ? 1 : n * fact(n - 1);
}
```
* To make inlining possible in the absence of unusually clever compilation and linking facilities, the definition - and not just the declaration - of an inline function must be in scope.
* An **inline** specifier does not affect the semantics of a function. In particular, an inline function still has a unique address, and so do **static** variables of an inline function.

#### 12.1.6 constexpr Functions
* When used in an object definition, it means "evaluate the initializer at compile time."
* A **constexpr** function must consist of a single **return**-statement, no loops and no local variables are allowed, cannot have side effects.
* Allows member initialization, recursion, and conditional expressions.
* A **constexpr** function can refer to nonlocal objects as long as it does not write to them.

#### 12.1.7 [[noreturn]] Functions
* A construct **[[...]]** is called an *attribute* and can be placed just about anywhere in the C++ syntax (e.g., in front of a declaration).
* There are only two standard attributes: **[[noreturn]]** and **[[carries_dependency]]**.

```c++
[[noreturn]] inline auto terminate() -> void {
    std::cout << "terminating program..." << std::endl;
    exit(0);
}
```

#### 12.1.8 Local Variables
* If a local variable is declared **static**, a single, statically allocated object will be used to represent that variable in all calls of the function.
* Initialization of **static** local variable does not lead to a data race unless you enter the function containing it recursively or a deadlock occurs.
* The effect of initializing a local **static** recursively is undefined.

### 12.2 Argument Passing
* Unless a formal argument is a reference, a copy of the actual argument is passed to the function.

#### 12.2.1 Reference Arguments
* The absence of **const** in the declaration of a reference argument is taken as a statement of intent to modify the variable.
* Rule of thumb:
	1. Use pass-by-value for small objects
	2. Use pass-by-**const**-reference to pass large values that you don't need to modify
	3. Return a result as a **return** value rather than modifying an object through an argument
	4. Use rvalue references to implement move and forwarding
	5. Pass a pointer if "no object (**nullptr**)" is a valid alternative
	6. Use pass-by-reference only if you have to

#### 12.2.2 Array Arguments
* If an array is used as a function argument, a pointer to its initial element is passed.

```c++
// Three identical function declarations
void odd(int* p);
void odd(int a[]);
void odd(int buf[1024]);
```
* You can pass a reference to array, with number of elements as part of the type.

```c++
void foo(int(&arr)[1024]);
```

#### 12.2.3 List Arguments
* A **{}**-delimited list can be used as an argument to a parameter of:
	1. Type **std::initializer_list\<T>**, where the values of the list can be implicitly converted to **T**
	2. A type that can be initialized with the values provided in the list
	3. A reference to an array of **T**, where the values of the list can be implicitly converted to **T**

#### 12.2.4 Unspecified Number of Arguments
* Three ways to do it:
	1. Use a variadic template
	2. Use an **initializer_list**
	3. Terminate the argument list with the ellipsis (**...**), and use some macros from **\<cstdarg>**

```c++
#include <cstdarg>

void print_args(int first ...) {
    va_list ap;
    va_start(ap, first);
    
    for (;;) {
        int val {va_arg(ap, int)};
        if (val == 0) break;
        std::cout << val << ' ';
    }
    
    va_end(ap);
    std::cout << std::endl;
}

void print_args(const std::initializer_list<int> args) {
    for (auto ele: args) {
        std::cout << ele << ' ';
    }
    std::cout << std::endl;
}

// Use <cstdarg> (bad)
print_args(5, 6, 9, -4, 0, 12); // 6 9 -4
// Use initializer_list (good)
print_args({5, 6, 9, -4, 0, 12}); // 5 6 9 -4 0 12
```

#### 12.2.5 Default Arguments
* Default arguments may be provided for trailing arguments only.

```c++
int foo(int a, int b, int c =0, char *c =nullptr);
int bar(int=3);
int wrong(char*=nullptr); // syntax error: *= is an assignment operator
```

### 12.5 Pointer to Function

```c++
double half(const int x) {
	return x / 2.0;
}
double (* calculate)(const int x); // pointer to function taking an int argument and returning a double

calculate = &half;
// No need to dereference calculate before calling
calculate(3); // 1.5
```

* Need **reinterpret_cast** to convert pointer to function types. (So don't do it!)
* There is no implicit conversion of argument or return types when pointers to functions are assigned or initialized.

```c++
void ssort(void *base, size_t n, size_t sz, bool(const void*, const void*) comparator);

auto comparator1(const void* a, const void* b) -> bool {
	return *static_cast<int*>(a) < *static_cast<int*>(b);
}

auto comparator2(const int* a, const int* b) -> bool {
	return *a < *b;
}

ssort(arr, 10, 10, comparator1); // OK
ssort(arr, 10, 10, comparator2); // Cannot, argument type doesn't match
```

### 12.6 Macros
* Don't use macro unless you have to!
* Macro names cannot be overloaded, and the macro preprocessor cannot handle recursive calls.
* Be careful with possible side effects.

```c++
#define MIN(a, b) (((a)<(b))?(a):(b))
int x = 1;
int y = 2;

int c = MIN(x++, y++); // Side effect: x = 3, y = 4
```
* Can concatenating two strings using the **##** macro operator.
* A single **#** before a parameter name in a replacement string means a string containing the macro argument.

```c++
#define NAME2(a,b) a##b
int NAME2(hel,lo) = 3; // int hello = 3;

#define printx(x) std::cout << #x " = " << x << std::endl
int num = 3;
print(num); // std::cout << "num" " = " << num << std::endl;
```
* Macros can be variadic.

#### 12.6.1 Conditional Compilation
* One of macros' best use case.

```c++
#ifdef SM_61
// Use 8 bits int operations
#endif
```

#### 12.6.2 Predefined Macros

```c++
// Some predefined macros
std::cout << __cplusplus << std::endl; // 201103
std::cout << __DATE__ << std::endl; // Oct 14 2016
std::cout << __TIME__ << std::endl; // 12:30:05
std::cout << __FILE__ << std::endl; // /path/to/main.cpp
std::cout << __LINE__ << std::endl; // 96
std::cout << __func__ << std::endl; // main
```

#### 12.6.3 Pragmas

* Behavior is not defined in C++ standard, but some compilers utilize it.

```c++
#pragma blah blah blah
// Syntax

unsigned int arr[] {2, 9, 5};

#pragma unroll
for (unsigned int i = 0; i < 3; ++i) {
	arr[i] = i;
}
// In CUDA, nvcc compiler will unroll the above for-loop to:
// arr[0] = 0;
// arr[1] = 1;
// arr[2] = 2;

#pragma mark - Implementation
// In Xcode, the above line generates a separator can be accessed through IDE
```

___

## 13. Exception Handling

* Idea: the library designer cannot even know what kind of program the library will become part of:
	* The author of a library can detect a run-time error but does not in general have any idea what to do about it.
	* The user of the library may know how to cope with a run-time error but cannot easily detect it (or else it would have been handled in the user's code and not left for the library to find).

### 13.1.1 Exceptions

* A function that cannot cope with a problem *throws* an exception, hoping that its (direct or indirect) caller can handle the problem.
	* A calling component indicates the kinds of failures that it is willing to handle by specifying those exceptions in a **catch**-clause of a **try**-block.
	* A called component that cannot complete its assigned task reports its failure to do so by throwing an exception using a **throw**-expression.

```c++
auto alloc_mem(size_t size) -> void* {
	// try to alloc...
	if (/* succeed */) {
		return result;
	} else {
		throw Some_error{};
	}
}

void foo() {
	try {
		auto result = alloc_mem();
		// use result
	} catch (Some_error) {
		// handles the error
	}
}
```

* An exception is an object **throw**n to present the occurrence of an error. It can be of any type that can be copied, but it is strongly recommended to use only user-defined types specifically defined for that purpose.

#### 13.1.2 Traditional Error Handling
* *Terminate the program*
	* A library that unconditionally terminates cannot be used in a program that cannot afford to crash.
* *Return an error value*
	* There is often no acceptable "error value". Some operations simply do not have return values (e.g., constructors).
* *Return a legal value and leave the program in an "error state"*
	* The calling function may not notice that the program has been put in an error state. Use of global error state variable is bad for concurrency.
* *Call an error-handler function*
	* Functions with error occurring usually have no idea what to do about the error; if it does, it should not be considered as an error.

##### 13.1.4.2 Exceptions That Are Not Errors
* Although an exception can be anything, but it's the best to think of exception handling as error handling.

#### 13.1.6 Hierarchical Error Handling
* In most systems, it is feasible to design every function to ensure that it always either completes successfully or fails in a well-defined manner.

### 13.2 Exception Guarantees
* We call an operation *exception-free* if that operation leaves the program in a valid state when the operation is terminated by throwing an exception.
* If pieces of nonlocal data are assumed to have a specific relationship, we must consider that an invariant and our recovery action must preserve it.
* Before a **throw**, a function must place all constructed objects in valid states.
* Three types of guarantee:
	* *basic guarantee*: the basic invariants of all objects are maintained, and no resources such as memory, are leaked.
	* *strong guarantee*: *basic guarantee* + either the operation succeeds, or it has no effect.
	* *nothrow guarantee*: *basic guarantee* + some operations are guaranteed not to throw an exception.
* Violating a standard-library requirement, is logically equivalent to violating a fundamental language rule.

### 13.3 Resource Management
* Resources can be released by using destructor, instead of remembering to release them in every **try**/**catch** call.
* RAII (Resource Acquisition Is Initialization): Managing resources using local objects.

### 13.4 Enforcing Invariants
* Three kinds of strategies to enforce invariants (preconditions):
	* *Don't enforce it*: for performance purposes. Frequent condition checking for performance oriented program is expensive.
	* *Terminate the program*: cannot proceed if preconditions are not met. Do this only if terminating is affordable.
	* *Throw an exception*
* Two standard ways for assertion:
	* **assert(A)**: macro in **<cassert>**, run time check.
	* **static_assert(A, message)**: compile time assert.
* For general library code, reporting an error - preferably by throwing an exception - is essential.
* Destructors should not throw, so don't use a throwing **Assert()** in a destructor.

### 13.5 Throwing and Catching Exceptions

#### 13.5.1 Throwing Exceptions

* We can **throw** exceptions of any type that can be copied or moved.
* A **throw x** initializes a temporary variable of **x**'s type with **x**, which may be further copied several times before it is caught.
* The process of passing the exception "up the stack" from the point of throw to a handler is called *stack unwinding*.
* In each scope exited, the destructors are invoked so that every fully constructed object is properly destroyed.
* Objects of types with move semantics are not expensive to throw.

##### 13.5.1.1 noexcept Functions

```c++
void foo() noexcept;
```

* Declaring a function **noexcept** is for logical and compiler optimization reason.
* If an exception is thrown in a **noexcept** function, the program terminates unconditionally by invoking **std::terminate()**.
* It does not invoke destructors from calling functions.
* It is implementation-defined whether destructors from scopes between **throw** and the **noexcept** are invoked.
* By adding a **noexcept** specifier, we indicate that our code was not written to cope with a **throw**.

##### 13.5.1.2 The noexcept Operator

* It is possible to declare a function to be conditionally **noexcept**.

```c++
template<typename T>
void foo(T& x) noexcept(std::is_pod<T>()); // noexcept if T is POD
void bar(T& x) noexcept(noexcept(foo(x))); // noexcept if foo(x) does not throw
```

* The predicate in a **noexcept()** specification must be a constant expression. Plain **noexcept** means **noexcept(true)**.
* **noexcept(expr)** does not go very deep to check whether **expr** throws, it simply look at all operations in **expr** and see if they *all* have **noexcept** evaluated to **true**.

##### 13.5.1.3 Exception Specifications
* Deprecated, don't use. Expensive at run-time, hard to use correctly.

```c++
// Deprecated!!! DO NOT USE!!!
void foo() throw(MyException1, MyException2); // may only throw MyException1 or MyException2 exceptions
// If other types of exceptions are thrown, by default the program will be terminated.
```

#### 13.5.2 Catching Exceptions

* C++ does not support **finally** block in **try**/**catch** semantic, because RAII (Resource Allocation is Initialization) is a better solution than using **finally** block to release resources and do cleanups.

```c++
try {
	throw E {};
} catch (H) {
	// Is invoked if:
	//		1. H is the same type as E
	//		2. H is an unambiguous public base of E
	//		3. H and E are pointer types and 1 or 2 holds for the types to which they refer
	//		4. H is a reference and 1 or 2 holds the type to which H refers
}
```

* We can add **const** to the type used to catch an exception, so it won't be modified.
* Exceptions can be caught by reference.

##### 13.5.2.1 Rethrow

* A rethrow is indicated by a **throw** without an operand.
* A rethrow may occur in a **catch**-clause or in a function called from a **catch**-clause.
* If no exception is to rethrow, the program terminates.
* The type of exception to be thrown is the type of original exception, not just the type of exception in **catch**.

##### 13.5.2.2 Catch Every Exception

* **<stdexcept>** provides a small hierarchy of exception classes with a common base **std::exception**
* Use **catch (...)** to catch all exceptions, including ones that are not derived from **std::exception**.

```c++
try {
    throw std::runtime_error {"Keep going, don't mind me."};
} catch (const std::runtime_error& err) {
    std::cerr << "Caught runtime error: " << err.what() << std::endl;
} catch (const my_error& err) {
	bool can_handle;
	// Set can_handle with some conditions.
	if (can_handle) {
		// Handle it.
	} else {
		throw; // Rethrow the exception.
	}
} catch(...) {
	// Catch everything
}
```

##### 13.5.2.4 Function try-Blocks

* The body of a function can be a **try**-block.

```c++
void foo()
try {
	// Do something...
} catch (...) {
	// Handle the exception...
}
```

* Constructors can catch exceptions by enclosing the complete function body - including the member initializer list - in a **try**-block.

```c++
class my_class {
public:
	my_class(const unsigned int foo, const bool bar)
	try:
		foo_{foo},
		bar_{bar}
	{
		// Initialize other stuff...
	} catch (...) {
		// Handle the exception...
	}
private:
	unsigned int foo_;
	bool bar_;
};
```

##### 13.5.2.5 Termination

* Rules for calling **std::terminate()** are:
	* When no suitable handler was found for a thrown exception
	* When a **noexcept** function tries to exit with a **throw**
	* When a destructor invoked during stack unwinding tries to exit with a **throw**
	* When code invoked to propagate an exception (e.g., a copy constructor) tries to exit with a **throw**
	* When someone tries to rethrow (**throw;**) when there is no current exception being handled
	* When a destructor for a statically allocated or thread-local object tries to exit with a **throw**
	* When an initializer for a statically allocated or thread-local object tries to exit with a **throw**
	* When a function invoked as an **atexit()** function tries to exit with a **throw**
* There is no way of catching exceptions thrown during initialization or destruction of namespace and thread-local variables.

#### 13.5.3 Exceptions and Threads

* If an exception is not caught on a **thread**, **std::terminate()** is called.

### 13.6 A vector Implementation

* The general principles to follow are to
	* Never let go of a piece of information before its replacement is ready for use.
	* Always leave objects in valid states when throwing or rethrowing an exception.
* Throwing an exception from a destructor typically leads to resource leaks and unpredictable behavior of code relying on reasonable behavior of objects.
* There is no really good way to protect against exceptions thrown from destructors, so the library makes no guarantees if an element destructor throws.

___

## 14. Namespaces

### 14.2 Namespaces
* The notion of a *namespace* is provided to directly represent the notion of a set of facilities that directly belong together, for example, the code of a library.
* The entities declared in a namespace are referred to as the members of the namespace.
* A namespace is a (named) scope.

#### 14.2.1 Explicit Qualification
* A member can be declared within a namespace definition and defined later using the *namespace-name::member-name* notation.
* We cannot declare a new member of a namespace outside a namespace definition using the qualifier syntax.
* Cannot redefine function in the namespace (not even with *namespace-name::member-name* notation).

```c++
namespace linalg {
	inline auto matmul(const matrix& A, const matrix& B) -> matrix;
}

inline auto linalg::matmul(const matrix& A, const matrix& B) -> matrix {
	// Multiply matrix and return it...
}
```

#### 14.2.2 using-Declarations

```c++
using std::string; // use "string" for "std::string"
```

#### 14.2.3 using-Directives

```c++
using namespace std; // make every name from std accessible
```

* **using** different namespaces with name clashes alone will not result in compile-time (or run-time) error. However, if ambiguous name is directly referenced, it will result in a compile-time error.

```c++
namespace my_lib { class string { public: string() {} }; }

using namespace std;
using namespace my_lib;

// Fine until here, can use both names spaces.

string str {}; // Error: Reference to 'string' is ambiguous
```

#### 14.2.4 Argument-Dependent Lookup (ADL)

* If a function isn't found in the context of its use, we look in the namespaces of its arguments.

```c++
namespace linalg {
	class matrix { /* ... */ };
	inline void print(const matrix& mat) { /* ... */ }
}

void foo(const linalg::matrix& mat) {
	print(mat); // no need to explicitly call "linalg::print(mat);"
}
```

* General rule of ADL:
	* If an argument is a class member, the associated namespaces are the class itself (including its base classes) and the class's enclosing namespaces.
	* If an argument is a member of a namespace, the associated namespaces are the enclosing namespaces.
	* If an argument is a built-in type, there are no associated namespaces.

#### 14.2.5 Namespaces Are Open

* You can have multiple namespace declarations.

#### 14.4.1 Convenience vs. Safety

* A **using**-declaration adds a name to a local scope (and hides the global one if there is one).
* A **using**-directive does not.

#### 14.4.2 Namespace Aliases

* Long namespace names are okay, use aliases for long namespace names.
* Can also use namespace aliases for versioning.

```c++
using linalg = linalg_v2r11;
```

#### 14.4.8 Unnamed Namespaces

* For preventing name clashes

```c++
namespace {
	const unsigned int a;
}
```
___

## 15. Source Files and Programs

### 15.1 Separate Compilation

* Where the unit of compilation is a file, all of the file must be recompiled whenever a change (however small) has been made to it or to something on which it depends.
* The amount of time spent recompiling can be significantly reduced by partitioning the program into files of suitable size.
* Compilation process:
	1. *source file* presented to the compiler.
	2. Preprocessed (macro processing is done and use **#inlude**s to bring in headers) into *translation unit*.
* The *linker* (*loader*) is the program that binds together the separately compiled parts.
* New code can be added to the running program ("dynamically linked") later.

### 15.2 Linkage

```c++
extern int x; // "extern" indicates it's just a declaration and not a definition.
```

* An object must be defined exactly once in a program. It may be declared many times, but the types must agree exactly.
* A variable defined without an initializer in the global or a namespace scope is initialized by default. This is *not* the case for non-**static** local variables or objects created on the free store.
* *external linkage*: a name that can be used in translation units different from the one in which it was defined.
* *internal linkage*: a name that can  be referred to only in the translation unit in which it is defined.
* When used in namespace scope (including the global scope), the keyword **static** (somewhat illogically) means "not accessible from other source files".
* The keyword **const** implies default internal linkage, precede definition with **extern** to make it external linkage.
* Names that a linker does not see, such as the names of local variables, are said to have *no linkage*.

```c++
int x {5}; // external linkage
static int y {1}; // internal linkage

const char z {'a'}; // internal linkage
extern const char z1 {'b'}; // external linkage

void foo() {
	int num {5}; // no linkage
}
```

* An **inline** function must be defined identically in every translation unit in which it is used.
* Use header files to keep **inline** function definitions consistent.

```c++
// Illegal:

// file1.cpp
	inline const int foo(int x) { return x; }

// file2.cpp
	inline const int foo(int x) { return x + 1; }
```

* By default, **const** objects, **constexpr** objects, type aliases, and anything declared **static** in a namespace scope have internal linkage.
* To ensure consistency, place aliases, **const**s, **constexpr**s, and **inline**s in header files.

#### 15.2.1 File-Local Names

* An unnamed namespace can be used to make names local to a compilation unit. The effect of an unnamed namespace is very similar to that of internal linkage.
* **static** means "use internal linkage", it's one of the confusing leftovers from earliest days of C.

#### 15.2.2 Header Files

* The **#include** mechanism is a text manipulation facility for gathering source program fragments together into a single unit (file) for compilation.
* Unfortunately, spaces are significant within the **< >** or **" "** of an include directive.

```c++
#include < iostream > // will not find <iostream>
```

* A header file should never contain:
	* Ordinary function definitions: **int foo() { return 1; }**
	* Data definitions: **int x;**
	* Aggregate definitions: **short arr[] = {1, 2, 3};**
	* Unnamed namespaces: **namespace { /\* ... \*/ }**
	* **using**-directives: **using namespace bar;**

#### 15.2.3 The One-Definition Rule (the ODR)

* A given class, enumeration, and template, etc., must be defined exactly once in a program.
* Two definitions of a class, template, or inline function are accepted as examples of the same unique definition if and only if:
	1. they appear in different translation units, and
	2. they are token-for-token identical, and
	3. the meaning of those tokens are the same in both translation units.

#### 15.2.4 Standard-Library Headers

* For each C standard-library header **<X.h>**, there is a corresponding standard C++ header **<cX>**.
* The macro **__cplusplus** is defined by the C++ compiler and can be used to distinguish C++ code from code intended for a C compiler.

```c++
#ifdef __cplusplus
namespace mylib {
#endif
	const int foo() { return 1; }
#ifdef __cplusplus
}
#endif
```

#### 15.2.5 Linkage to Non-C++ Code

* Use **extern "C"** to specify something should be linked according to the (system-specific) C linkage conventions.
* Use *linkage block* to perform group linkage specification.

```c++
extern "C" char* strcpy(char*, const char*);

extern "C" {
	#include <string.h>
}

// Used to produce a C++ header from a C header.
#ifdef __cplusplus
extern "C" {
#endif
	int foo();
	bool bar(float);
	// ... etc.
#ifdef __cplusplus
}
#endif
```

#### 15.3.3 Include Guards

* Headers should be included only when necessary.

```c++
#ifdef FILE_NAME_H
#define FILE_NAME_H

// Content...

#endif
```

### 15.4 Programs

* A program can only provide one of two **main()** declarations:

```c++
int main() { /* ... */ }
int main(int argc, char* argv[]) { /* ... */ }
```

#### 15.4.1 Initialization of Nonlocal Variables

* In principle, a variable defined outside any function (that is, global, namespace, and class **static** variables) is initialized before **main()** is invoked.
* The default initializer value for built-in types and enumerations is **0**.
* It is not possible to catch an exception thrown by the initializer of a global variable.
* Often, a function returning a reference is a good alternative to a global variable.

```c++
unsigned int& use_count() {
	static unsigned int uc {0};
	return uc;
}
```

#### 15.4.2 Initialization and Concurrency

* If a program is terminated using the standard-library function **exit()**, the destructors for constructed static objects are called. However, if a program is terminated using standard-library function **abort()**, they are not.
* Calling **exit()** in a destructor may cause an infinite recursion.

___

## 16. Classes

### 16.2 Class Basics

* Summary:
	* A class is a user-defined type.
	* A class consists of a set of members. The most common kinds of members are data members and member functions.
	* Member functions can define the meaning of initialization (creation), copy, move and cleanup (destruction).
	* Members are accessed using **.** (dot) for objects and **->** (arrow) for pointers.
	* Operators, such as **+**, **!**, and **[]**, can be defined for a class.
	* A class is a namespace containing its members.
	* The **public** members provide the class's interface and the **private** members provide implementation details.
	* A **struct** is a **class** where members are by default **public**.

#### 16.2.2 Default Copying

* By default, the copy of a class object is a copy of each member.
* You can define an appropriate assignment operator for a class to provide custom behavior.

```c++
data d1 {date()};
date d2 {d1}; // initialized by copy
```

#### 16.2.3 Access Control

```c++
class foo {
	int a, b, c; // private by default
public:
	inline const int get_product();
};
```

* The protection of private data relies on restriction of the use of the class member names. It can therefore be circumvented by address manipulation and explicit type conversion.
* Only hardware can offer perfect protection against malicious use of a general-purpose language, and even that is hard to do in realistic systems.

#### 16.2.4 class and struct

* By definition, a **struct** is a class in which members are by default public.

```c++
struct S { /* ... */ };
// Equivalent to:
class S { public: /* ... */ }
```

* Access specifiers can be used many times in a single class declaration.

#### 16.2.5 Constructors

* Use **{}** instead of **()** for initialization.

#### 16.2.6 explicit Constructors

* By default, a constructor invoked by a single argument acts as an implicit conversion from its argument type to its type.
* We can specify that a constructor is not used as an *implicit* conversion.
* A constructor declared with the keyword **explicit** can only be used for initialization and explicit conversions.

```c++
class person {
public:
    explicit person(unsigned short age);
};

person me {23}; // OK
person another = person{21}; // OK
person him = {25}; // Not OK (would be if constructor is not explicit)
person her = 22; // Not OK (would be if constructor is not explicit)
```

* An initialization with an **=** is considered a *copy initialization*.
* Leaving out the **=** makes the initialization explicit. Explicit initialization is known as *direct initialization*.
* By default, declare a constructor that can be called with a single argument **explicit**. You need a good reason not to do so (e.g., **complex**).
* **explicit** can also be used on constructors with zero or more than one arguments.

#### 16.2.7 In-Class Initializers

* You can specify default arguments for data members.

```c++
class person {
public:
	explicit person() { }
	explicit person(const unsigned short age): age_{age} { }
private:
	unsigned short age_ {21};
};
```

#### 16.2.8 In-Class Function Definitions

* A member function *defined* within the class definition is taken to be an inline member function.
* If a member function is only *declared* within the class definition, it is not inline by default. However, we can change it to an inline function by adding **inline** (only) to its definition (i.e., not declaration or both).

```c++
class person {
public:
    explicit person() { }
    void grow_up() { ++age_; } // inline
    auto age() -> const unsigned short; // inline
    void print_age(); // not inline
private:
    unsigned short age_ {21};
};

inline
auto person::age() -> const unsigned short {
    return age_;
}

void person::print_age() {
    std::cout << age_ << std::endl;
}
```

#### 16.2.9 Mutability

* const: object that holds *immutable* value; variable: object that holds *mutable* value.
* Immutability is a most useful property in a multi-threaded program.

##### 16.2.9.1 Constant Member Functions

```c++
// The "const" suffix indicates this member function does not modify the state of this object
void person::print_age() const {
    std::cout << age_ << std::endl;
}
```

* **const** is a part of the member function's type.
* A **const** member function can be invoked for both **const** and non-**const** objects, whereas a non-**const** member function can be invoked only for non-**const** objects.

##### 16.2.9.2 Physical and Logical Constness

* Occasionally, a member function is logically **const**, but it still needs to change the value of a member.

##### 16.2.9.3 mutable

* We can define a member of a class to be **mutable**, meaning that it can be modified even in a **const** object.

```c++
class Foo {
public:
    explicit Foo() { }
    void increase() const { ++a_; }
    auto a() const -> const int { return a_; }
private:
    mutable int a_ {0};
};

Foo obj {};
obj.increase();
obj.a(); // 1
```

##### 16.2.9.4 Mutability through Indirection

* Declaring a member **mutable** is most appropriate when only a small part of a representation of a small object is allowed to change.
* More complicated cases are often better handled by placing the changing data in a separate object and accessing it indirectly.

#### 16.2.10 Self-Reference

* In a non-**static** member function, the keyword **this** is pointer to the object for which the function was invoked.
* In a non-**const** member function of class **X**, the type of **this** is **X\***.
* In a **const** member function of class **X**, the type of **this** is **const X\*** to prevent modification of the object itself.
* **this** is an rvalue.

#### 16.2.11 Member Access

* A member of a class **X** can be accessed by applying the **.** (dot) operator to an object of class **X** or by applying the **->** (arrow) operator to a pointer to an object of class **X**.
* From inside a class no operator is needed.
* A member function can refer to the name of a member before it has been declared.
* If we want to refer to a member in general, rather than to a member of a particular object, we qualify by the class name followed by **::**.

#### 16.2.12 [static] Members

* There is exactly one copy of a **static** member for each class.
* **static** must appear in member declaration, but not necessarily definition.

#### 16.2.13 Member Types

* A *member class* (often called a *nested class*) can refer to types and **static** members of its enclosing class.
* A nested class has access to members of its enclosing class, even to **private** members (just as a member function has), but has no notion of a current object of the enclosing class.
* A class does not have special access rights to the members of its nested class.

```c++
template<typename T>
class Tree {
	using value_type = T; // member alias
	enum Policy { rb, splay, treeps }; // member enum
	class Node { // member class
		Node* right;
		Node* left;
		value_type value;
	public:
		void f(Tree*);
	};
	Node* top;
public:
	void g(const T&);
	// ...
}

template<typename T>
void Tree::Node::f(Tree* p) {
	top = right; // error: no object of type Tree specified
	p->top = right; // OK
	value_type v = left->value; // OK: value_type is not associated with an object
}

template<typename T>
void Tree::g(Tree::Node* p) {
	value_type val = right->value; // error: no object of type Tree::Node
	value_type v = p->right->value; // error: Node::right is private
	p->f(this); // OK
}
```

#### 16.3.3 Overloaded Operators

```c++
template<typename T>
inline bool operator==(const Date a, const Date b) {
	return a.Day() == b.Day() && a.Month() == b.Month() && a.Year() == b.Year();
}
```

* Overloaded operators in namespaces can benefit from argument-dependent lookup.

#### 16.3.4 The Significance of Concrete Classes

* Derivation from a concrete class should be done with care and only rarely.

___

## 17. Construction, Cleanup, Copy and Move

### 17.1 Introduction

* The difference between *move* and *copy* is that after a copy two objects must have the same value, whereas after a move the source of the move is not required to have its original value.
* An object is considered an object of its type after its constructor completes, and it remains an object of its type until its destructor starts executing.

```c++
class X {
	X(Sometype); // "ordinary constructor": create an object
	X(); // default constructor
	X(const X&); // copy constructor
	X(X&&); // move constructor
	X& operator=(const X&); // copy assignment: clean up target and copy
	X& operator=(X&&); // move assignment: clean up target and move
	~X(); // destructor: clean up
	// ...
}
```

* There are five situations in which an object is copied or moved:
	* As the source of an assignment
	* As an object initializer
	* As a function argument
	* As a function return value
	* As an exception

### 17.2 Constructors and Destructors

#### 17.2.1 Constructors and Invariants

* Constructor's initialization for class must establish a *class invariant*, that is, something that must hold whenever a member function is called (from outside the class).
* If the constructor cannot establish the invariant, no object is created and the constructor must ensure that no resources are leaked.

#### 17.2.2 Destructors and Resources

* The name of a destructor is **~** followed by the class name.
* A destructor does not take an argument, and a class can have only one destructor.
* Destructor are called implicitly when an automatic variable goes out of scope, an object on the free store is deleted, etc.
* A type that has no destructor declared, such as a built-in type, is considered to have a destructor that does nothing.\

#### 17.2.3 Base and Member Destructors

* A constructor builds a class object "from the bottom up":
	1. first, the constructor invokes its base class constructors,
	2. then, it invokes the member constructors, and
	3. finally, it executes its own body.
* A destructor "tears down" an object in the reverse order:
	1. first, the destructor executes its own body,
	2. then, it invokes its member destructors, and
	3. finally, it invokes its base class destructors.
* A **virtual** base is constructed before any base that might use it and destroyed after all such bases.
* Constructors execute member and base constructors in declaration order (not the order of initializers).

#### 17.2.4 Calling Constructors and Destructors

* A destructor is invoked implicitly upon exit from a scope or by **delete**.
* It's dangerous to explicitly call a destructor.
* We can prevent destruction of an **X** by declaring its destructor **=delete** or **private**.

#### 17.2.5 virtual Destructors

* A destructor can be declared to be **virtual**, and usually should be for a class with a virtual function (if not the wrong destructor may be called).

```c++
class A { public: ~A() { } }
class B: public A { public: ~B() { } }

void f(A* obj) {
	delete obj; // A's destructor is called instead of B's
}
```

### 17.3 Class Object Initialization

#### 17.3.1 Initialization Without Constructors

* We can initialize objects of a class for which we have not defined a constructor using:
	* memberwise initialization
	* copy initialization, or
	* default initialization (without an initializer or with an empty initializer list).
* Where no constructor requiring argument is declared, it is also possible to leave out the initializer completely.
	* For statically allocated objects, the rules are exactly as if you had used **{}**.
	* For local variables and free-store objects, the default initialization is done only for members of class type, and members of built-in type are left uninitialized.
* Naturally, memberwise initialization works only if we can access the members. If a class has a private non-**static** data member, it needs a constructor to initialize it.

#### 17.3.2 Initialization Using Constructors

* If a constructor is declared for a class, some constructor will be used for every object.
* The usual overload resolution rules apply for constructors.
* The **{}** initializer notion does not allow narrowing.

##### 17.3.2.1 Initialization by Constructors

* **()** request to use a constructor in an initialization, whereas **{}** also offers memberwise initialization.
* Prefer **{}** instead of **()**, unless it's necessary to explicitly use constructor.

```c++
std::vector<int> v1 {7}; // one element with value 7
std::vector<int> v2(7); // 7 elements with value 0
```

#### 17.3.3 Default Constructors

* A default argument can make a constructor that takes arguments into a default constructor.
* References and **const**s must be initialized. Therefore, a class containing such member cannot be default constructed unless the programmer supplies in-class member initializers or defines a default constructor that initializes them.

#### 17.3.4 Initializer-List Constructors

* A constructor that takes a single argument of type **std::initializer_list** is called an *initializer-list constructor*.
* The mechanism for accepting a **{}**-list is a function (often a constructor) taking an argument of type **std::initializer_list<T>**.
* The initializer list can be of arbitrary length but must be homogeneous. That is, all elements must be of the template argument type, **T**, or implicitly convertible to **T**.

##### 17.3.4.1 initializer_list Constructor Disambiguation

* Initializer list and other types of constructors' overloading rules:
	* If either a default constructor or an initializer-list constructor could be invoked, prefer the default constructor.
	* If both an initializer-list constructor and an "ordinary constructor" could be invoked, prefer the initializer-list constructor.
* Use **()** notion to explicitly request for non-initializer-constructor.

#### 17.3.4.2 Use of initializer_lists

* A function with an **initializer_list<T>** argument can access it as a sequence using the member functions **begin()**, **end()**, and **size()**.
* **initializer_list** does not provide subscripting.
* An **initializer_list<T>** is passed by value. That is required by the overload resolution rules and does not impose overhead because an **initializer_list<T>** object is just a small handle (typically two words) to an array of **T**s.

```c++
void foo(std::initializer_list<int> args) {
	for (unsigned int i {0}; i < args.size(); ++i) {
		std::cout << args.begin()[i] << std::endl;
	}
}
```

* The elements of an **initializer_list** are immutable.

### 17.4 Member and Base Initialization

#### 17.4.1 Member Initialization

* Arguments for a member's constructor are specified in a *member initializer list* in the definition of the constructor of the containing class.

```c++
Matrix::Matrix(const unsigned int num_row, const unsigned int num_col, const float init_val)
	: m{num_row}, n{num_col}
{
	// ...
}
```
* The member's constructors are called before the body of the containing class's own constructor is executed.
* The constructors are called in the order in which the members are declared in the class rather than the order in which the members appear in the initializer list.
* The member destructors are called in the reverse order of construction after the body of the class's own destructor has been executed.
* An "implicitly initialized" member of a built-in type is left uninitialized.
* A constructor can initialize members and bases of its class, but not members or bases of its members or bases.

##### 17.4.1.1 Member Initialization and Assignment

* A reference member or a **const** member must be initialized.
* Initializers have efficiency advantage over assignment operations (use member initializer list whenever you can).

#### 17.4.2 Base Initializers

* Bases of a derived class are initialized in the same way non-data members are. That is, if a base requires an initializer, it must be provided as a base initializer in a constructor.
* Bases are initialized before members and destroyed after members.

#### 17.4.3 Delegating Constructors

* A member-style initializer using the class's own name (its constructor name) calls another constructor as part of the construction. Such a constructor is called a *delegating constructor* (and occasionally a *forward constructor*).
* You cannot both delegate and explicitly initialize a member.

```c++
class Foo {
public:
	Foo(int x): a{x} { }
	Foo(): Foo(3) { } // delegating Foo(int x)
	Foo(float x): Foo(std::floor(x)), a{std::floor(x)} { } // error
private:
	int a;
};
```

* Delegating by calling another constructor in a constructor's member and base initializer list is very different from explicitly calling a constructor in the body of a constructor. In the later case, the called constructor simply creates a new unnamed object (a temporary) and does nothing with it.
* A destructor will not be called for an object unless its original constructor completed.

#### 17.4.4 In-Class Initializers

* We can specify an initializer for a non-**static** data member in the class declaration.
* Both **{}** and **=** initializer notions can be used for in-class member initializers, but the **()** notion cannot.
* The value of the global variable is obtained at the point where the constructor for a new **S** object is run (bad idea to use global variable to initialize a data member).

#### 17.4.5 static Member Initialization

* Generally, the **static** member declaration acts as a declaration for a definition outside the class.

```c++
class Foo {
	static int x;
};

int Foo::x = 3;
```

* To initialize a **static** member in the class declaration, it must be a **const** of an integral or enumeration type, or a **constexpr** of a literal type, and the initializer must be a *const-expression*.
* You have to define a member somewhere before use it in a way that requires it to be stored as an object in memory.

### 17.5 Copy and Move

```c++
class Matrix {
public:

	Matrix(const Matrix&); // copy constructor
	Matrix& operator=(const Matrix&); // copy assignment

	Matrix(Matrix&&); // move constructor
	Matrix& operator=(Matrix&&); // move assignment

	// ...
}
```

* Typically, a move cannot throw, whereas a copy might.
* When you write a move operation, you should leave the source object in a valid but unspecified state because it will eventually be destroyed and the destructor cannot destroy an object left in an invalid state.
* Design your moves not to throw, and to leave their source objects in a state that allows destruction and assignment.

#### 17.5.1 Copy

* Copy for a class **X** is defined by two operations:
	* Copy constructor: **X(const X&)**
	* Copy assignment: **X& operator=(const X&)**
* Watch out for self-assignment.

##### 17.5.1.1 Beware of Default Constructors

* Copy constructor may leave out some members but the compiler won't warn about it, because of those members' default constructors.

##### 17.5.1.2 Copy of Bases

* Use base copy to avoid redundancy.

```c++
class Foo: Bar1, Bar2 {
public:
	Foo(const Foo& obj): Bar1{obj}, Bar2{obj} { }
};
```

##### 17.5.1.3 The Meaning of Copy

* Copy operation must meet two criteria:
	* *Equivalence*: after **x = y**, **x == y** should be true.
	* *Independence*: changes on one of them should not have influence on the other.
* Sometimes it makes sense not copying everything (e.g., counters, allocators, etc.), but that should not affect the result or comparison operators. (i.e., if you don't copy it, don't compare it)
* Most of the problems related to (lack of) independence have to do with objects that contain pointers.
* Immutable shared data is not a problem for independency.
* *copy-on-write*: The idea is that a copy doesn't actually need independence until a shared state is written to, so we can delay the copying of the shared state until just before the first write to it.

##### 17.5.1.4 Slicing

* A pointer to derived class implicitly converts to a pointer to its public base class.

```c++
class Base { public: Base(const Base& obj); };
class Derived: public Base { public: Derived(const Derived& obj); };

void foo(Base* pt) {
	Base b = *pt; // may slice
}

void bar() {
	Derived d;
	foo(&d);
	Base bb = d; // slices: invokes Base::Base(const Base&), not Derived::Derived(const Derived&)
}

// Making the inheritance private ("class Derived: private Base") will prevent slicing by making initialization of b and bb errors.
// Or prevent conversion from instances of Derived to Base.
```

#### 17.5.2 Move

* We can define **Matrix**'s move constructor to simply take the representation from its source and replace it with an empty **Matrix** (which is cheap to destroy).
* For the move assignment, we can simply do a swap.
* The argument of a move operation must always be left in a state that the destructor can cope with (and preferably deal with very cheaply and easily).
* **std::move()** is a standard-library function returning an rvalue reference to its argument, it does not move anything (a better name would be **std::rval()**).

### 17.6 Generating Default Operations

* Default constructor, copy constructor, copy assignment, move constructor, move assignment and destructor are generated by the compiler by default for a class if the program uses it.
* Rules of suppressing default operation:
	* If the programmer declares any constructor for a class, the default constructor is not generated for that class.
	* If the programmer declares a copy operation, a move operation, or a destructor for a class, no copy operation, move operation, or destructor is generated for that class.

#### 17.6.1 Explicit Defaults

```c++
class Foo {
public:
	Foo() = default;
	~Foo() = default;
	Foo(const Foo& obj) = default;
	Foo& operator=(Foo&& obj) = default;
	// ...
};
```

#### 17.6.2 Default Operations

* The default meaning of each generated operation, is to apply the operation to each base and non-**static** data member of the class.
* Returning value from function is a move construction by default.

```c++
void foo() {
	Matrix result {5, 5};
	return result; // move construction
}
```

* Move operation is destructive. It is not guaranteed (or required) that an arbitrary operation on a move-from object will work correctly. 

#### 17.6.3 Using Default Operations

##### 17.6.3.1 Default Constructors

* Beware of default constructors, members with built-in type will be left uninitialized (if it's generated default implementation)!

##### 17.6.3.2 Maintaining Invariants

* Rules:
	1. Establish an invariant in a constructor (including possibly resource acquisition).
	2. Maintain the invariant with copy and move operations (with the usual names and types).
	3. Do any needed cleanup in the destructor (including possibly resource release).

##### 17.6.3.3 Resource Invariants

* If you define a custom constructor, the generation of copy operation is only deprecated, not banned. So it's possible to get pass compiler warning, but bad idea to do so.
* In general, if a class has a pointer member, the default copy and move operations should be considered suspicious:
	* If that pointer member represents ownership, memberwise copy is wrong.
	* If that pointer member does not represent ownership, memberwise copy *is* appropriate, explicit **=default** and a comment are most likely a good idea.

#### 17.6.4 deleted Functions

```c++
class Foo {
public:
	Foo& operator=(const Foo&) = delete; // disable copy assignment
	// ...
};

template<class T>
T* clone(T* p) {
	return new T{*p};
}

Foo* clone(Foo*) = delete; // disable clone for Foo
```

### 17.7 Advice

* If a class has a virtual function, it needs a virtual destructor.
* Make sure that copy assignments are safe for self-assignment.
* When adding a new member to a class, check to see if there are user-defined constructors that need to be updated to initialize the member.

___

## 18. Operator Overloading

### 18.1 Introduction

* The usual precedence rules for operators hold (**a + b * c** is evaluated as **a + (b * c)**).

### 18.2 Operator Functions

* List of operator functions can be declared:

| Arithmetic | Logical | Bitwise |   Others   |
|:----------:|:-------:|:-------:|:----------:|
|    **+**   |  **==** |  **^**  |  **->* **  |
|    **-**   |  **!**  |  **&**  |   **,**    |
|    **\***  |  **<**  |  **\|** |   **->**   |
|    **/**   |  **>**  |  **~**  |   **[]**   |
|    **%**   |  **!=** |  **^=** |   **()**   |
|   **++**   |  **<=** |  **&=** |   **new**  |
|   **--**   |  **>=** | **\|=** |  **new[]** |
|   **+=**   |  **&&** |  **<<** | **delete** |
|   **\*=**  | **\|\|**|  **>>** |**delete[]**|
|   **/=**   |         | **<<=** |            |
|   **%=**   |         | **>>=** |            |
|    **=**   |         |         |            |