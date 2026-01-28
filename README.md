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

Unlike the standard library's `urllib.parse` module, this library is compliant
with the WHATWG URL specification.

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

`can_ada` also supports the `URLSearchParams` API:

```python
from can_ada import URLSearchParams

params = URLSearchParams("q=canada&safe=off")
params.append("page", "2")
params.append("page", "3")
params["q"] = "usa"
print(params) # q=usa&safe=off&page=2&page=3
print(params.has("q")) # True
print(params.get("page")) # 2
print(params.get_all("page")) # [2, 3]
print(params.keys()) # ["q", "safe", "page"]
print(params.values()) # ["usa", "off", "2", "3"]
```

## Performance

We find that `can_ada` is typically ~4x faster than urllib:

```
------------------------------------------------------------------------------------- benchmark: 4 tests ------------------------------------------------------------------------------------
Name (time in ms)              Min                 Max                Mean            StdDev              Median               IQR            Outliers      OPS            Rounds  Iterations
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
test_can_ada_parse         36.7565 (1.0)       40.3057 (1.0)       37.1606 (1.0)      0.6789 (1.0)       36.9869 (1.0)      0.2526 (1.0)           2;3  26.9102 (1.0)          27           1
test_ada_python_parse     134.0627 (3.65)     143.6443 (3.56)     135.8992 (3.66)     3.1977 (4.71)     134.7860 (3.64)     1.2441 (4.92)          1;1   7.3584 (0.27)          8           1
test_urllib_parse         208.8403 (5.68)     212.9208 (5.28)     211.2021 (5.68)     1.7273 (2.54)     211.3141 (5.71)     2.9319 (11.60)         1;0   4.7348 (0.18)          5           1
test_yarl_parse           238.6351 (6.49)     246.4206 (6.11)     242.4351 (6.52)     3.4108 (5.02)     241.8302 (6.54)     6.1566 (24.37)         2;0   4.1248 (0.15)          5           1
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
```

To run the benchmarks locally, use:

```
pytest --runslow
```

[Ada]: https://ada-url.com/
