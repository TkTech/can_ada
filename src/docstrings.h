#pragma once

namespace docs {

inline constexpr char can_parse[] = R"doc(Return whether an absolute URL can be parsed.

:param input: A URL string.)doc";

inline constexpr char can_parse_base[] = R"doc(Return whether a URL can be parsed against a base URL.

:param input: An absolute or relative URL string.
:param base_input: An absolute base URL string.

Invalid URLs and bases return ``False``.)doc";

inline constexpr char parse[] = R"doc(Parse and normalize an absolute URL.

:returns: A mutable :class:`URL`.
:raises ValueError: If the URL cannot be parsed.)doc";

inline constexpr char url[] = R"doc(A mutable, normalized URL, created with :func:`parse`.

Component assignments follow WHATWG normalization rules. Rejected assignments
are silently ignored. Add a string to resolve a reference against this URL.)doc";

inline constexpr char add[] = R"doc(Resolve a URL reference against this URL, returning a new URL.

:raises ValueError: If the reference cannot be parsed.)doc";

inline constexpr char search_params[] = R"doc(An ordered collection of query parameter pairs, allowing duplicate keys.

Construct from a query string with an optional leading ``?``. Parsing decodes
percent escapes and treats ``+`` as a space. Instances are independent of any
:class:`URL`. Keep the collection alive and unchanged while using its iterators.)doc";

inline constexpr char has[] = R"doc(Return whether a key exists, optionally with a matching value.

:param value: Match this value too, unless ``None``.)doc";

inline constexpr char remove[] = R"doc(Remove all pairs matching a key and, optionally, a value.

:param value: Match this value too, unless ``None``.

Missing keys are ignored.)doc";

inline constexpr char getitem[] = R"doc(Return the first value for a key.

:raises KeyError: If the key is absent.)doc";

inline constexpr char next[] = R"doc(Return the next item.

:raises StopIteration: If the iterator is exhausted.)doc";

inline constexpr char idna_encode[] = R"doc(Encode a Unicode domain as ASCII bytes using IDNA.

Returns ``b""`` if conversion fails.)doc";

} // namespace docs
