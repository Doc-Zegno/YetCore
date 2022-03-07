## Yet ABI

### Method names mangling
The utilized mangling scheme is a trade-off between a verbosity
and readability. It is also fully compatible with C since it
doesn't use any *special* symbols like `$`, `@`, `#`.
However, this makes it necessary to introduce lots of special
prefices used to decode mangled name thus leading to a poorer
readability.

#### Trivial cases
The most straight-forward encoding is for names that obey
Re:Lite naming conventions and use primitive argument types only.
Consider the example below:
```swift
    // Assuming it's a part of the package `CompanyName.ProjectName`
    class Image {
        func save(name: String, directory: String, format: String)
    }
```
This method yields the following name:
```
    yet_CompanyName_ProjectName_Image_saveF__s_S_S_S__V
```

So, as you can see:
 1) Prepend name with a `yet_` prefix
 2) Write down method's full qualification joined with an underscore:
    `CompanyName_ProjectName_Image_save`
 3) Specify method's calling convention, typically it's `F`
    (for more details see a dedicated section on method invocations):

| Letters | Convention |
| ------- | ---------- |
| `F`     | Function   |
| `R`     | Reduced    |
| `D`     | Dynamic    |

 4) Put two underscores in order to separate an argument types' list
 5) Specify types of arguments:

| Letters |  Corresponding type  |
| ------- | -------------------- |
| `s`     | `self`               |
| `V`     | `Void`               |
| `N`     | `Never`              |
| `B`     | `Bool`               |
| `C`     | `Char`               |
| `C8`    | `Char8`              |
| `I`     | `Int`                |
| `I32`   | `Int32`              |
| `U`     | `UInt`               |
| `U64`   | `UInt64`             |
| `F`     | `Float`              |
| `F32`   | `Float32`            |
| `S`     | `String`             |
| `PC`    | C-pointer to `Char`  |
| `R`     | `Any`                |
| `t1`    | template type #1     |
| `OI`    | `Optional<Int>`      |
| `AI`    | `Array<Int>`         |
| `EI`    | `Iterable<Int>`      |
| `MSI`   | `Map<String, Int>`   |
| `HS`    | `Set<String>`        |
| `T2IS`  | `Tuple<Int, String>` |
| `X2IV`  | `Function<Int, Void>`|
| `J2CB`  | `Variant<Char, Bool>`|

 5) Put another double underscore and specify a return type as well.

Please, note:
 * `s` is treated as `self` only when it is **the first** character
 * `V` is used when function has no parameters at all (static function):
```swift
    func printNewLine() <=> yet_printNewLineF__V__V
```
 * `Any` is denoted with `R` (for `Ref`)
 * `Iterable` is denoted with `E` and not `I`
   (alternative name is `Enumerable`)
 * `Set` is denoted with `H` (think as of `HashSet`)
 * Template types are **not** translated as is. Instead, they get
   an ordinal value. Also they become a part of their owner's name:
```swift
    // This one is a bit tricky.
    // `2tfindF_t1_t2` corresponds to `find<E, T>`
    // (you'll get more information about such a format later).
    func find<E, T>(e: E, t: T): T? <=> yet_2tfindF_t1_t2__t1_t2__Ot2
```
 * `Tuple` is followed by a number of its template type arguments:
```swift
    Tuple<Int, Array<String>> <=> T2IAS
    Tuple<Bool, Map<String, Int>, Set<T>> <=> T3BMSIHt1
```
 * The same is true for `Function<Arg1, ... Result>` which is denoted
   with `X` (for `eXecutable`)
 * ...and for `Variant<Type1, ...>` which uses `J` (`Joined`) letter.

#### More complicated cases
Technically, Yet doesn't put any limitations on the naming possibilities.
That means, you can name your method exactly in the same manner as you do
when writing in C. So it can contain underscores and class name doesn't
have to start with a capitalized letter.

In order to address such cases, the following rules are used:
 1) When a name contains underscores, it should be prepended with a special
    prefix `Nw` where `N` is a *number of words*:
```swift
    create_map <=> 2wcreate_map
    create___map <=> 2wcreate___map
```
 2) In case a name also contains either leading or trailing underscores,
    it also has to be prepended with a prefix `Nu` where `N` is a
    *total number of underscores*:
```swift
    _Boolean_t <=> 2u2w_Boolean_t
```
 3) When using a fully qualified name, it should be prepended with a prefix
    `Np` where `N` is a *number of parts* whilst the parts should be
    delimited with an underscore:
```swift
    custom_package.graphics.Device <=> 3p2wcustom_package_graphics_Device
```
 4) Types with the template arguments make use of prefix `Nt` where
    `N` is a *number of template arguments* (counting the outermost only)
    whilst the type arguments themselves are separated with an underscore:
```swift
    // Two outermost types: `Int` and `String`
    DataFrame<Int, String> <=> 2tDataFrame_I_S

    // Nested one.
    // Two outermost types: `Int` and `Array<String>`
    DataFrame<Int, Array<String>> <=> 2tDataFrame_I_AS

    // Technically, the same stay true for template function names
    func find<E, T>(e: E, t: T): T? <=>
        yet_2tfindF_t1_t2__t1_t2__Ot2()
```
 5) Referential arguments which are passed via **fat pointers** are
    prepended with a special prefix `0f`:
```swift
    func append(s: String) <=> yet_appendF__0fS__V()
``` 
 6) All the prefices are applied in **the reversed order** (so prefix #4
    goes before prefix #3):
```swift
    koalas.DataFrame<Int, String> <=> 2t2pkoalas_DataFrame_I_S
```
 7) Builtin template types are written in the expanded form when used with
    non-builtin types:
```swift
    // Short
    Array<Int> <=> AI
    Function<String> <=> X1S

    // Short nested
    Array<Array<Int>> <=> AAI
    Function<Function<String>> <=> X1X1S

    // Expanded.
    // Please, note: `Function` doesn't use this strange `X` anymore
    Array<Images.Filter> <=> 1tArray_2pImages_Filter
    Function<Images.Filter> <=> 1tFunction_2pImages_Filter

    // Expanded nested (note: only one outermost type!)
    Array<Array<Images.Filter>> <=> 1tArray_1tArray_2pImages_Filter
```
 8) Shorthand notation is used for parameter/return types that have common
    parts with the full qualification of the method. Replace them with
    a `Nc` mnenomic (`N` common parts):
```swift
    // Two parts in common: `MegaApp` and `Models`.
    // Please, note: `MegaApp.Models.User` actually has 3 parts
    //   but with a `2c` it becomes `2c.User` and thus has only 2 parts
    MegaApp.Models.Util.createFrom(user: MegaApp.Models.User) <=>
        yet_MegaApp_Models_Util_createFromF__2p2c_User__V()
```
 9) Similar shorthand is used when the common parts are borrowed from
    previous argument types. In this case a `NcI` mnemonic is used
    (`N` common parts with argument type #`I`, counting from zero):
```swift
    // Arguments share the same type
    compare(user1: MegaApp.Models.User, user2: MegaApp.Models.User): Int <=>
        yet_compareF__3pMegaApp_Models_User_3c0__I()
```
 10) The more common parts the better. That means, if an argument #1
    has more common parts with an argument #0 than it has with a method's
    qualification, it must share them with a previous argument.
    Otherwise a full qualification (and then arguments with lower indices)
    gets a higher priority.
    This rule is applied **before** previous argument's type gets collapsed
    (in case it has common parts with method's qualification itself):
```swift
    // Argument #1 has 2 common parts with a method's qualification
    // and 3 parts with an argument #0.
    // Despite the fact the first argument's type gets collapsed to
    // `2p2c_User`, the second's argument's type still shares
    // `MegaApp.Models.User` with it and thus turns into `3c0`
    MegaApp.Models.compare(user1: MegaApp.Models.User, user2: MegaApp.Models.User): Int <=>
        yet_MegaApp_Models_compareF__2p2c_User_3c0__I()
```

Now let's try it out:
```swift
    koalas.util.print_header(df: koalas.DataFrame<Int, Array<Images.Filter>>, rows: Int) <=>
        yet_koalas_util_2wprint_headerF__2t2p1c_DataFrame_I_1tArray_2pImages_Filter_I__V()
```
~~Still pretty readable~~.

#### Specials
There are some cases when the *special* kind of functions are used.
To make it possible to distinguish them, arbitrary additional attributes
can be injected between a method name and a parameter list: 
 1) Getter/setter for some property with `get` and `set` respectively:
```swift
    // Re:Lite
    class Widget {
        var isVisible: Bool
    }

    // Yet (standard prefices are omitted). Also note `self` parameter
    Widget_isVisibleF__get__s__B
    Widget_isVisibleF__set__s_B__V
```
 2) Operators with `operator`:
 ```swift
    // Re:Lite -- invoke
    operator ()(x: Int) -> String

    // Yet
    invokeF__operator__s_I__S

    // Re:Lite -- subscript
    operator [](index: Int) -> Float {
        get { ... }
        set { ... }  // Note: implicit `Float` argument
    }

    // Yet
    getF__operator__s_I__F
    setF__operator__s_I_F__V
 ```
  3) (Re:Lite special) extensions:
```swift
    // Re:Lite
    func ui.Color.darker(): ui.Color

    // Yet. Also note implicit `ui.Color` parameter.
    // ... and don't forget about sharing common types
    yet_darkerF__extension__2pui_Color__2c0
```

#### Type variables
Type variables are used to store meta information about types
which can be retrieved at runtime. Most importantly, they contain tables
for virtual method invocation mechanism. Such variables have special suffix
`type`:
```swift
    // Type variable for the class `Images.Filter`.
    // Doesn't contain "F" since it's not a function
    yet_Images_Filter__type
```

---
### Function invocation
This section describes how to properly pass parameters, get a result
and check for errors.

#### Execution context
Technically, there is no portable way to reconstruct current execution
state from the perspective of high-level programming language.

Even if we are trying to inspect current stack trace, its structure and
the obtained function symbols may have nothing in common with the original
source code due to several factors:
 1) You have to provide symbol tables in order to get something
    *meaningful* and not just a bunch of useless hex addresses.
    How often do you specify `-G` flag when compiling your code?
 2) You can get *a faulted line* but usually you are really looking
    for *a faulted expression* which can be spread across multiple lines
    or, to make things worse, there can be multiple similar expressions
    on the same line:
```swift
    // One of these `calc`s has failed. Good luck with it
    let x = calc(a) + calc(b) + calc(c)
```
 3) A compiler can (and actually will) optimize out a significant amount
    of function invocations thus making resulting stack trace useless
 4) On the other hand, there are lots of small utility functions which
    are not exposed to the programmer (usually) so it makes no sense
    to show them in a stack trace
 5) (Probably, the most difficult one) The asynchronous programming is
    a real beast. Technically, when you schedule some task, it may be
    executed on a different thread so the previous stack trace becomes
    *forgotten*:
```swift
    func updateUI() {
        Task.run(&faulty)  // launches a separate thread
    }

    func faulty() {
        raise DemoError()
    }

    // May result in:
    //   at EventDispatcher.run(Task)
    //   at Task.invoke()
    //   at faulty()
    //
    // But where is my `updateUI()`? And what is this `EventDispatcher`
    // doing here?
```

In order to overcome these limitations, Yet runtime dynamically captures
current execution state with the help of `StackFrame` and `ExecutionContext`
instances. Although this process is manual, the things are pretty simple:
 1) All the Yet functions (except for the ones with **reduced** calling
    convention) expect their first argument to be a C-pointer to the current
    `ExecutionContext` (`EC` for short). But you can pass `nullptr`
    if you don't have it
 2) Every function which wants to be shown in a stack trace **must**
    create an instance of `StackFrame` at its very beginning and capture
    current context:
```cpp
    void someFunction(EC* context, args...) {
        StackFrame frame(context, functionInfo);
    }
```
 3) If provided `context` pointer is set to `nullptr`, created `StackFrame`
    will find an appropriate context on its own. So the original `context`
    pointer will always point to a valid context after that:
```cpp
    void someFunction(EC* context, args...) {  // Might be null
        StackFrame frame(context, functionInfo);
        context;  // Guaranteed not to be null
    }
```
 4) You may want to provide extra information by storing the location
    of expression to be executed before actual invocation:
```cpp
    // When manually written in C++
    frame.setLineAfter(__LINE__);
    auto result = someFunction(context, args...);

    // When transpiled from Re:Lite or some other language
    frame.setLocation(...);
    auto result = someFunction(context, args...);
```

#### Parameter passing
The following rules are used:
 1) Scalar primitives (i.e. `Bool`, `Char`, `IntN`, `UIntN` and `FloatN`)
    are passed as is
 2) Structures are always passed by a **non-const** C-pointer
 3) Automatic references are not C-structs and thus unmanaged `Ptr`
    (which is ultimately just an `uintptr_t` in disguise) is passed.
    The calling side **must ensure** that reference will stay valid
    (i.e. someone will point to the managed object) during all the
    function execution time. The quickiest and safest way to do that
    is to copy automatic reference onto your stack before invoking target
    method
 4) When passing a reference to the instance of interface type,
    caller side might increase callee's performance by suggesting
    the correct *virtual table*. This is done via `FatPtr`
    which is effectively a pair of two pointers (the first one is
    the passed `Ptr` itself and the second one is the pointer to
    the suggested virtual table):
```swift
    func print(value: Printable)
    <=>
    yet_printF__0fPrintable__V(EC* context, FatPtr value)
```
 5) Top-level `Optional<T>` where `T` is a value type is reduced to a
    C-pointer which can be `NULL`:
```swift
    func setResolution(value: Int?)
    <=>
    yet_setResolutionF__OI__V(EC* context, Int* value)
```
 6) Top-level `Optional<T>` where `T` is a referential type is reduced
    to a `Ptr` which can be `0`

Let's find out how the latest rules work in a bit complicated scenario:
```swift
    func pass(value: Any???)
```
The outer most optional is wrapped around the *value type*
`Optional<Optional<Ptr>>` thus gets replaced with a vanilla C-pointer.
The inner most `Optional<Ptr>` is not a top-level one so there is no
reduction to `Ptr`:
```swift
    // Don't forget about mangling
    yet_passF__OOOR__V(EC* context, Optional<Optional<Ptr>>* value)
```

#### Retrieving the results of invocation and error checking for static functions
Technically, there is no possibility to use standard C++ exception mechanism
since its internal implementation differs from one compiler to another.
In order to make it possible to combine dynamic libraries produced with
the different compilers, a *manual* error handling mechanism is used.

Effectively, this means that **every Yet function must return** some `Ptr`:
 * When an invocation is successful, it returns
   a `Ptr` which equals to zero
 * In case of failure, it points to the instance of type `Error`
   which contains information relative to the occurred failure.

It's a responsibility of each client of Yet functions to check its result
for errors.

That's it for error handling. The actual function's result is passed
according to the following rules:
 1) Scalar primitives and structures are not returned. Instead, a client
    has to pass a C-pointer to the destination buffer as the **last**
    parameter. This buffer is usually allocated at client's stack before the
    function invocation. The function has to fill its fields with
    appropriate values:
```swift
    // Re:Lite
    func shift(point: Point, offset: Point): Point {
        return Point {
            x: point.x + offset.x,
            y: point.y + offset.y,
        }
    }

    // Yet
    Ptr yet_shiftF__Point_1c0__1c0(EC* context, Point* point, Point* offset, Point* result) {
        result->x = point->x + offset->x;
        result->y = point->y + offset->y;
        return 0;
    }
```
 2) Objects on heap are returned via `Ptr` buffer allocated at client's
    stack (pretty the same as in a previous case). The *caller side*
    must ensure this `Ptr` is initialized with some *reasonable* value.
    Unless you are trying to optimize memory allocations, *reasonable*
    should mean "zero" for you otherwise app can crash. The *callee side*
    must ensure reference counter of this object is not less than 1 in order
    to keep it from early deallocation. There are three things that you have
    to keep in mind so as to prevent memory leaks:
     * The safest way of dealing with a referential results is either
       to use `PtrGuard` or to wrap them with a `Ref` **immediately** using
       `protect()` function. Both `PtrGuarg` and `Ref` have appropriate
       destructors which will automatically decrease a reference counter
       on leaving the scope.
       The difference between them is that a code with `PtrGuard` is a bit
       simpler to use and, what's more important, `PtrGuard` ensures that
       guarded `Ptr` is zeroed before a function call. When using `Ref`,
       you must zero your `Ptr` manually (see examples below).
       Please, note: `protect()` **doesn't** increase a counter so
       it won't become 2.
     * If for some reason you are not going to use these wrappers,
       you can decrease the reference counter manually using `release()`
     * Since both `PtrGuard` and `Ref` dtors will decrease the counter,
       you have to discard it manually if you're going to return the
       pointer further. This is done with a `unprotect()` method:
```cpp
    // Using PtrGuard
    Ptr outer(Int argument, Ptr* result) {
        PtrGuard guard;
        Ptr error = inner(argument, &guard.ptr);
        if (error) {
            // Error handling
        }
        // Some actions on `guard.ptr`
        *result = guard.unprotect();
        return 0;
    }

    // Using Ref
    Ptr outer(Int argument, Ptr* result) {
        Ptr value = 0;  // Zero it manually
        Ptr error = inner(argument, &value);
        if (error) {
            // Error handling
        }
        Ref ref = protect(value);
        // Some actions on `value`
        *result = ref.unprotect();
        return 0;
    }

    // Well, actually, if "Some actions" part is not present, you can
    // just return the result of inner function:
    Ptr outer(Int argument, Ptr* result) {
        Ptr error = inner(argument, result);
        if (error) {
            // Error handling
        }
        return 0;
    }
```
 3) Optionals of referential types are also returned via `Ptr` buffer
    except for its value can be set to zero in order to indicate `none`.
 4) Optionals of value types are returned just like the regular structures:
```swift
    func indexOf(ch: Char, s: String) -> Int?
    <=>
    Ptr yet_indexOfF__C_S__OI(EC* context, Char ch, Ptr s, Optional<Int>* result);
```

#### Reduced functions
Some of the Yet runtime's functions are not supposed to return any error.
Instead, such functions return their values as the regular C-functions.
Also they don't need an `ExecutionContext` as their first argument.
This calling convention is called **reduced** and denoted with `R`.

The remaining rules for them stay the same though. This means you are still
passing structures (and optionals) via pointers.

The common examples of functions with the reduced calling conventions are:
 * `Allocator::allocate(size:)` and similar
 * `retain()` and `release()`
 * object's deinitializer

**Note:** you may want to think that such functions cannot fail.
Well, it's not true in general. Obviously, the allocator *can* fail if
the system has run out of available memory. But instead of raising an error
it will silently return a `null` pointer, so it is the responsibility
of the caller side to check for it and decide whether to treat it as an error
or not.

Some other reduced functions might use different approaches for the handling
of abnormal situations (from totally ignoring them to the immediate termination
of a program), so make sure to consult the documentation of a reduced function
before using it.

So the moral of this fable is the following: a function uses reduced
calling convention only in two cases:
 1. if it's not possible to use any other convention (e.g., deinitializer)
 2. if it's guaranteed to produce **unrecoverable errors only**,
    i.e., when it's not possible to continue the program's execution

When writing your own API, please, consider using a standard functional
calling convention wherever it's possible.

#### Dealing with generic functions
Since generic functions can expect arguments of some unknown type `T`
which can be a primitive, a structure, an optional or even a reference
it's important to handle all of these cases correctly.

There are some special generic structures from `InvocationUtil.h`
that can be used in order to facilitate this task:
 1) `ArgumentInfo<T>::Type` will give an appropriate type for passing
    an argument (e.g. `Ptr` for `Ref`, `struct*` for `struct`
    and `Int` for `Int`)
 2) `ResultInfo<T>::Type*` will give an appropriate type for an *output*
    parameter which is used as a function's result (e.g. `Ptr*` for `Ref`,
    `Int*` for `Int` and so on). The pointer-less version can be used
    in order to declare a *buffer* for function's output.

A typical example might be the following:
```cpp
    // Add value to an array
    Ptr BasicArray<E>::addF__s_t1__V(EC* context, Ptr self, typename ArgumentInfo<E>::Type value);

    // Get value at index
    Ptr BasicArray<E>::getF__operator__s_I__t1(EC* context, Ptr self, Int index, typename ResultInfo<E>::Type* result);

    // Declare an output variable
    {
        ...
        typename ResultInfo<E>::Type result;
        BasicArray<E>::getF__operator__s_I__t1(context, array, index, &result);
    }
```

You might want to use a macro `YET_ARG_TYPE(T)` which is basically `typename ArgumentInfo<T>::Type` and `YET_RES_TYPE(T)` instead of `typename ResultInfo<T>::Type` for the sake of simplicity as well.


### Memory management
Yet Runtime uses a custom memory manager which aims to optimize
frequent allocations and deallocations of object of particular
granularity (16 bytes for 64-bit machines).

Since Yet is all about OOP, classes and interfaces, typical programs
will allocate quite a lot (usually rather small) objects. All of them
should be automatically deallocated once they aren't in use anymore.
This is done via *automatic reference counters* and thus brings some
old problems (e.g. *cyclic references*) here which can be solved with
the help of weak references.

Keep in mind that weak references must be automatically zeroed when
an object is deallocated and, even better, actually some of the allocated objects might **not** be allocated on a heap (in order to improve
a performance).
That means the memory management system is not trivial and you must
**never** operate on reference counter's internals. Instead,
you should use (preferred) C++ class `Ref` for automatic reference
counting and `retain()`/`release()` for manual counter increase/decrease
(correspondingly).

Typically, you're advised to use the simplest version of builtin allocation
methods (i.e. the version with one argument). The more advanced ones
are used for optimization purposes only and very risky if you don't know
when it's safe to enable such an optimization (consider reading
a dedicated section below).

#### Allocation options
Under the hood the allocator uses a one-for-all method `allocate(size:options:)`
which behavior is controlled with its second argument of type `Allocator::Options`.
These options can be used to specify a preferred allocation *mode*:
 1. *Standard* if no options are passed
 2. *Emergent* if objects representing information about some error are created
 3. *Permanent* if objects are not supposed to be deallocated until
    the program is terminated (e.g., singletons)
 4. *Stack* if objects effectively have the semantics of automatic variables. 

It's possible to specify additional features as well:
 * Whether to zero the allocated memory block or not. It might be useful
   for the sake of performance if the caller side is going to initialize
   the obtained block's contents anyway.

#### Stack allocation
One of the main problems is a reduction of unnecessary memory allocations
on a process' heap since they're rather expensive. Actually there are
lots of situations when allocated objects are used just like they were
local variables (collections' iterators is a good example). You can say
this about your object in case it meets
the following requirements:
 1) You don't pass it to an outer scope (e.g. you don't return it
    from a function)
 2) One of the following is true:
    * you either don't pass it as function argument at all
    * or corresponding function argument is marked as local itself.

If you have managed to prove your object is local, it can be allocated
*on stack* with a dedicated allocator's option `PlaceHint` which must
contain information about the place on stack which is safe to use
for such a purpose, usually it's done with the help of `StackBuffer` instances
which ensure both a proper data-alignment (16-byte for 64-bit machines)
and a zeroing of its contents right during a construction:
```cpp
    StackBuffer<64> buffer;  // Some reasonable size, 64 bytes must be enough
    auto hint = Allocator::PlaceHint(&buffer, 64);  // Tell buffer's address and size
    auto options = Allocator::Options().withPlaceHint(hint);
    auto allocatedPlace = Allocator::allocate(size, &options);
    assert(allocatedPlace == &buffer);
```
Whilst the idea behind it is pretty simple, there is still a lot of boilerplate
code to write. The good news are there is a handy specialization of `allocate`
method named `allocateOrRaise` which follows the standard calling convention
thus it returns the address of allocated block via the last argument:
```cpp
    // Returns OutOfMemoryError if no free memory left and 0 on success
    Ptr allocateOrRaise(EC* context, UInt size, Ptr* result);
```
This specialization assumes that a place hint is passed via the same last argument
(it became possible since both `Ptr` and `PlaceHint` occupies 8 bytes of memory).
Recall that it's necessary to zero the resulting `Ptr` before a function invokation,
that's exactly because of this assumption, more exactly, 0 means no place hint.

To make use of this fact, you can instruct a `PtrGuard` to properly initialize
a guarded `Ptr` by passing a pointer to the `StackBuffer` to its ctor:
```cpp
    StackBuffer<64> buffer;
    PtrGuard guard(&buffer);  // Now `guard.ptr` contains a hint
    auto error = Allocator::allocateOrRaise(context, size, &guard.ptr); 
```
What's more, almost all the library code is written to use this allocation method.
So if you want, for example, to locally instantiate an array, all you need
is to specify a target `StackBuffer`:
```cpp
    StackBuffer<64> buffer;
    PtrGuard guard(&buffer);
    BasicArray<int>::__new__V__s(nullptr, &guard.ptr);
```


### Object's memory layout
Every object that wants to be *understood* by a Yet Runtime must have
a predefined memory layout:
 1) The first machine word (8 bytes on a 64-bit machine) is occupied by
    an instance of `Manageable` (which is a special struct that is
    responsible for storing both strong and weak reference counts)
 2) The second one is occupied by a C-pointer to an instance of
    `Type` structure and provides necessary type information at runtime
    (most importantly, virtual tables)
 3) If its class `D` is derived from some other `B`, it must
    contain data (typically, fields) of that class. Effectively,
    first words of `D` layout must resemble the ones of `B` so
    a C-pointer to `D` instance can be treated as a pointer to `B`
    without any adjustments
 4) The following space is up to its owner and usually is filled with
    object's fields.

For 64-bit platforms it is also required that object is aligned
at a **16-byte boundary** so the lowest 4 bits of C-pointer to this object
are guaranteed to be set to zero.

**Please, note:** such a layout is pretty rigid and leaves no opportunity
of implementing multiple inheritance without a severe headache.
On the other hand, there is no problem with implementation of multiple
interfaces since all the virtual tables are just stacked within
corresponding `Type` variable (more in the following sections).


### Virtual invocations machinery
Yet **doesn't** use C++ mechanism for virtual method calls since it has
some sad limitations:
 * Strange rules for return type covariance that don't allow you
   to return, say, `Int` from the implementation of some interface
   which requires `Optional<Int>` to be returned
 * A big mess of `vtable` pointers that leads to unpredictable object's
   memory layout and (more importantly) makes a compiler perform some
   pointer adjustments during upcasting.

The latter is the absolute pitfall which makes it impossible to support
co/contravariance for generic types. That is, you can't safely pass
an object of type `Array<String>` to a function expecting argument
of type `Iterable<Any>` without some kind of adapters/wrappers
that will perform necessary pointer adjustments for array's elements.

#### Utilized scheme
So actually Yet uses a model very close to a Java one. Effectively
you just pass some `Ptr`/`Ref` around without knowing its corresponding
type at all. It's totally safe since all the objects have a predictable
memory layout with a pointer to a `Type` instance placed at 2nd machine
word. So when it comes to using methods from `String` interface,
you're looking through the virtual tables of the object this `Ptr` points to
and trying to find an appropriate one. When it's found, just invoke
a function at the offset known at compile-time. Dead simple.

