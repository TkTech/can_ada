# can_ada

Python bindings for [Ada][], a fast and spec-compliant URL parser.

## Installation

```bash
pip install can_ada
```

Binary wheels are available for most platforms. If not available, a
C++17-or-greater compiler will be required to build the underlying Ada library.

## Usage

Parsing is simple:

```python
from can_ada import parse

url = parse("https://tkte.ch/search?q=canada")
print(url.protocol) # https:
print(url.host) # tkte.ch
print(url.pathname) # /search
print(url.search) # ?q=canada
```

You can also modify URLs:

```python
from can_ada import parse

url = parse("https://tkte.ch/search?q=canada")
url.host = "google.com"
url.search = "?q=canada&safe=off"
print(url) # https://google.com/search?q=canada&safe=off
```

[Ada]: https://ada-url.com/