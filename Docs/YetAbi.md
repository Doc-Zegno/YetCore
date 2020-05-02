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
    yet_CompanyName_ProjectName_Image_save__s_S_S_S__V
```

So, as you can see:
 1) Prepend name with a `yet_` prefix
 2) Write down method's full qualification joined with an underscore:
    `CompanyName_ProjectName_Image_save`
 3) Put two underscores in order to separate an argument types' list
 4) Specify types of arguments:

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
    func printNewLine() <=> yet_printNewLine__V__V
```
 * `Any` is denoted with `R` (for `Ref`)
 * `Iterable` is denoted with `E` and not `I`
   (alternative name is `Enumerable`)
 * `Set` is denoted with `H` (think as of `HashSet`)
 * Template types are **not** translated as is. Instead, they get
   an ordinal value. Also they become a part of their owner's name:
```swift
    // This one is a bit tricky.
    // `2tfind_t1_t2` corresponds to `find<E, T>`
    // (you'll get more information about such a format later).
    func find<E, T>(e: E, t: T): T? <=> yet_2tfind_t1_t2__t1_t2__Ot2
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
        yet_2tfind_t1_t2__t1_t2__Ot2()
```
 5) All the prefices are applied in **the reversed order** (so prefix #4
    goes before prefix #3):
```swift
    koalas.DataFrame<Int, String> <=> 2t2pkoalas_DataFrame_I_S
```
 6) Builtin template types are written in the expanded form when used with
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
 7) Shorthand notation is used for parameter/return types that have common
    parts with the full qualification of the method. Replace them with
    a `Nc` mnenomic (`N` common parts):
```swift
    // Two parts in common: `MegaApp` and `Models`.
    // Please, note: `MegaApp.Models.User` actually has 3 parts
    //   but with a `2c` it becomes `2c.User` and thus has only 2 parts
    MegaApp.Models.Util.createFrom(user: MegaApp.Models.User) <=>
        yet_MegaApp_Models_Util_createFrom__2p2c_User__V()
```
 8) Similar shorthand is used when the common parts are borrowed from
    previous argument types. In this case a `NcI` mnemonic is used
    (`N` common parts with argument type #`I`, counting from zero):
```swift
    // Arguments share the same type
    compare(user1: MegaApp.Models.User, user2: MegaApp.Models.User): Int <=>
        yet_compare__3pMegaApp_Models_User_3c0__I()
```
 9) The more common parts the better. That means, if an argument #1
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
        yet_MegaApp_Models_compare__2p2c_User_3c0__I()
```

Now let's try it out:
```swift
    koalas.util.print_header(df: koalas.DataFrame<Int, Array<Images.Filter>>, rows: Int) <=>
        yet_koalas_util_2wprint_header__2t2p1c_DataFrame_I_1tArray_2pImages_Filter_I__V()
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
    Widget_isVisible__get__s__B
    Widget_isVisible__set__s_B__V
```
 2) Operators with `operator`:
 ```swift
    // Re:Lite -- invoke
    operator ()(x: Int) -> String

    // Yet
    invoke__operator__s_I__S

    // Re:Lite -- subscript
    operator [](index: Int) -> Float {
        get { ... }
        set { ... }  // Note: implicit `Float` argument
    }

    // Yet
    get__operator__s_I__F
    set__operator__s_I_F__V
 ```
  3) (Re:Lite special) extensions:
```swift
    // Re:Lite
    func ui.Color.darker(): ui.Color

    // Yet. Also note implicit `ui.Color` parameter.
    // ... and don't forget about sharing common types
    yet_darker__extension__2pui_Color__2c0
```

#### Type variables
Type variables are used to store meta information about types
which can be retrieved at runtime. Most importantly, they contain tables
for virtual method invocation mechanism. Such variables have special suffix
`type`:
```swift
    // Type variable for the class `Images.Filter`
    yet_Images_Filter__type
```

---
### Function invocation
This section describes how to properly pass parameters, get a result
and check for errors.

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
 4) Top-level `Optional<T>` where `T` is a value type is reduced to a
    C-pointer which can be `NULL`:
```swift
    func setResolution(value: Int?)
    <=>
    yet_setResolution__OI__V(Int* value)
```
 5) The most nested `Optional<T>` where `T` is a referential type is reduced
    to a `Ptr` which can be `0`

Let's find out how the latest rules work in a bit complicated scenario:
```swift
    func pass(value: Any???)
```
The inner most optional is wrapped around the referential type `Any` thus gets reduced to a `Ptr`. The outer most one is wrapped around the
*value type* `Optional<Optional<Any>>` thus gets replaced with a vanilla
C-pointer:
```swift
    // Don't forget about mangling
    yet_pass__OOOR__V(Optional<Ptr>* value)
```

#### Retrieving the results of invocation and error checking for static functions
Technically, there is no possibility to use standard C++ exception mechanism
since its internal implementation differs from one compiler to another.
In order to make it possible to combine dynamic libraries produced with
the different compilers, a *manual* error handling mechanism is used.

Effectively, this means that **every Yet function must return** a some kind
of `Result` structure instance. Such an instance **must contain** a member
called `error` of type `Ptr`. In particular, functions with a `Void`
return value will actually return a structure with an `error` member only.
 * When an invocation is successful, it returns
a `Result` with `error` equals to zero
 * In case of failure, `error` points to the instance of type `Error`
   which contains information relative to the occurred failure.

It's a responsibility of each client of Yet functions to check its result
for errors.

That's it for error handling. The actual function's result is passed
according to the following rules:
 1) Scalar primitive types are passed as is inside the aforementioned
    `Result` structure (as its `value` member). There are dedicated
    structures like `IntResult`, `BoolResult` and so on
 2) Structure results are not returned. Instead, a client has to pass
    a C-pointer to the destination structure as the **last** parameter.
    This structure is usually allocated at client's stack before the
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

    // Yet.
    // Note: it uses `VoidResult` because it doesn't return a point
    // via the result's instance
    VoidResult yet_shift__Point_1c0__1c0(Point* point, Point* offset, Point* result) {
        result->x = point->x + offset->x;
        result->y = point->y + offset->y;
        return okResult();  // inline for `VoidResult{ 0 }`
    }
```
 3) Objects on heap are returned with a `PtrResult` which has its
    `value` member set to `Ptr` pointing to the object. The *callee side*
    must ensure reference counter of this object is not less than 1 in order
    to keep it from early deallocation. There are three things that you have
    to keep in mind so as to prevent memory leaks:
     * The safest way of dealing with a referential results is to wrap
       them with a `Ref` **immediately** using `refOf()` function.
       `Ref` has appropriate destructor which will automatically decrease
       a reference counter on leaving the scope. Please, note: `refOf()`
       **doesn't** increase a counter so it won't become 2
     * If for some reason you are not going to use `Ref` wrapper,
       you can decrease the reference counter manually using `release()`
     * Since `Ref` dtor will decrease the counter, you have to increase it
       manually if you're going to return the pointer further. This is done
       with a `retain()` method:
```swift
    PtrResult outer(Int argument) {
        PtrResult result = inner(argument);
        if (result.error) {
            // Error handling
        }
        Ref ref = refOf(result.value);
        // Some actions
        return okResult(ref.retain());
    }
```
Well, actually, if "Some actions" part is not present, you can just return
the result of inner function:
```swift
    PtrResult outer(Int argument) {
        PtrResult result = inner(argument);
        if (result.error) {
            // Error handling
        }
        return result;
    }
```

#### Retrieving the results for generic functions
Since generic functions are generated by a compiler on a client's side
(and not exported from DLL), there is no need to follow the same rules
for returning an invocation result. Instead, **every resulting value**
is returned as is wrapped with `Result<T>`. The only notable exception is
for the referential values which are still returned as `Ptr` instances
via `Result<Ptr>`.


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
*on stack* with a dedicated allocator's method.


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

