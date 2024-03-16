# can_ada

[Fast] Python bindings for [Ada][], a fast and WHATWG spec-compliant URL parser.
This is the URL parser used in projects like Node.js.

## Installation

```bash
pip install can_ada
```

Binary wheels are available for most platforms. If not available, a
C++17-or-greater compiler will be required to build the underlying Ada library.

##  WHATWG URL compliance

Unlike the standard library's `urllib.parse` module, this library is compliant with the WHATWG URL specification.

```python
import can_ada
urlstring = "https://www.GOoglé.com/./path/../path2/"
url = can_ada.parse(urlstring)
# prints www.xn--googl-fsa.com, the correctly parsed domain name according
# to WHATWG
print(url.hostname)
# prints /path2/, which is the correctly parsed pathname according to WHATWG
print(url.pathname)

import urllib.parse
urlstring = "https://www.GOoglé.com/./path/../path2/"
url = urllib.parse.urlparse(urlstring)
# prints www.googlé.com
print(url.hostname)
# prints /./path/../path2/
print(url.path)
```

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

## Performance

We find that `can_ada` is typically ~4x faster than urllib:

```
---------------------------------------------------------------------------------
Name (time in ms)              Min                 Max                Mean       
---------------------------------------------------------------------------------
test_can_ada_parse         54.1304 (1.0)       54.6734 (1.0)       54.3699 (1.0) 
test_ada_python_parse     107.5653 (1.99)     108.1666 (1.98)     107.7817 (1.98)
test_urllib_parse         251.5167 (4.65)     255.1327 (4.67)     253.2407 (4.66)
---------------------------------------------------------------------------------
```

To run the benchmarks locally, use:

```
pytest --runslow
```

[Ada]: https://ada-url.com/