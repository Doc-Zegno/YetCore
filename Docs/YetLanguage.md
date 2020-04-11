## Yet Language
YL can be considered a rather high-level intermediate language
for Yet infrastructure, i.e. another level between some
frontend language such as **Re:Lite** and its backend
(currently C++).

### Purpose
YL aims to separate the platform's frontend from its backend.
This makes it possible to both introduce new frontend PLs
and simplify their compilers since all low-level optimizations
are moved to YL-side.

### Overview
One can treat YL as a Re:Lite without any syntactic sugar.
In fact, even *invocations of virtual methods* get
their own representation (I'm using ObjC-like syntax here)
because they have nothing to do with the statically resolved
function calls:
```swift
    // Re:Lite code
    let rowCount = matrix.rows.count

    // YL code
    let rowCount = [[matrix rows] count];
```

YL lacks of many powerful constructs such as pattern matching
and one should convert his code to more old-style `if-else`
and `switch` alternatives:
```swift
    // Re:Lite code
    return match point {
        Point(0, 0) => "Center"
        Point(x, 0) => "Ox axis"
        Point(0, y) => "Oy axis"
        _ => "Not on axis"
    }

    // YL code
    if point.x == 0 {
        if point.y == 0 {
            return "Center";
        } else {
            return "Oy axis";
        }
    } else {
        if point.y == 0 {
            return "Ox axis";
        } else {
            return "Not on axis";
        }
    } 
```

Switch over strings is still supported out of the box though:
```swift
    // Re:Lite code
    return message match {
        "abc" => 1
        "bcd" => 2
        "cde" => 3
    }

    // YL code
    switch message {
        case "abc":
            return 1;
        case "bcd":
            return 2;
        case "cde":
            return 3:
        default:
            return none;
    }
```
This is done intentionally in order to let the YL-compiler
optimize such a switch.
