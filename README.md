# can_ada

Python bindings for [Ada][], a fast and spec-compliant URL parser.

## Installation

```bash
pip install can_ada
```

Binary wheels are available for most platforms. If not available, a
C++17-or-greater compiler will be required to build the underlying Ada library.

##  WHATWG URL compliance

Unlike the standard library's `urllib.parse` module, this library is compliant with the WHATWG URL specification.

```python
urlstring = "https://www.GOoglé.com/./path/../path2/"
import can_ada
# prints www.xn--googl-fsa.com,
# the correctly parsed domain name according to WHATWG
print(can_ada.parse(urlstring).hostname)
# prints /path2/
# the correctly parsed pathname according to WHATWG
print(can_ada.parse(urlstring).pathname)
import urllib
#prints www.googlé.com
print(urllib.parse.urlparse(urlstring).hostname)
#prints /./path/../path2/
print(urllib.parse.urlparse(urlstring).path)
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

We find that `can_ada` maybe be faster than `urllib`:

```
$ python3 -m timeit -s 'import can_ada' 'can_ada.parse("https://tkte.ch/search?q=canada")'
1000000 loops, best of 5: 378 nsec per loop

$ python3 -m timeit -s 'import urllib' 'urllib.parse.urlparse("https://tkte.ch/search?q=canada")'
500000 loops, best of 5: 491 nsec per loop
```


[Ada]: https://ada-url.com/