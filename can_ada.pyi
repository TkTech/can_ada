from __future__ import annotations
import can_ada
import typing

__all__ = [
    "URL",
    "can_parse",
    "parse",
    "idna_encode",
    "idna_decode"
]


class URL():
    def __str__(self) -> str: ...
    def has_credentials(self) -> bool: ...
    def has_empty_hostname(self) -> bool: ...
    def has_hash(self) -> bool: ...
    def has_hostname(self) -> bool: ...
    def has_non_empty_password(self) -> bool: ...
    def has_non_empty_username(self) -> bool: ...
    def has_password(self) -> bool: ...
    def has_port(self) -> bool: ...
    def has_search(self) -> bool: ...
    def has_valid_domain(self) -> bool: ...
    def to_diagram(self) -> str: ...
    def validate(self) -> bool: ...
    def __add__(self, other) -> "URL": ...
    @property
    def hash(self) -> str:
        """
        :type: str
        """
    @hash.setter
    def hash(self, arg1: str) -> None:
        pass
    @property
    def host(self) -> str:
        """
        :type: str
        """
    @host.setter
    def host(self, arg1: str) -> None:
        pass
    @property
    def hostname(self) -> str:
        """
        :type: str
        """
    @hostname.setter
    def hostname(self, arg1: str) -> None:
        pass
    @property
    def href(self) -> str:
        """
        :type: str
        """
    @href.setter
    def href(self, arg1: str) -> None:
        pass
    @property
    def origin(self) -> str:
        """
        :type: str
        """
    @property
    def password(self) -> str:
        """
        :type: str
        """
    @password.setter
    def password(self, arg1: str) -> None:
        pass
    @property
    def pathname(self) -> str:
        """
        :type: str
        """
    @pathname.setter
    def pathname(self, arg1: str) -> None:
        pass
    @property
    def pathname_length(self) -> int:
        """
        :type: int
        """
    @property
    def port(self) -> str:
        """
        :type: str
        """
    @port.setter
    def port(self, arg1: str) -> None:
        pass
    @property
    def protocol(self) -> str:
        """
        :type: str
        """
    @protocol.setter
    def protocol(self, arg1: str) -> None:
        pass
    @property
    def search(self) -> str:
        """
        :type: str
        """
    @search.setter
    def search(self, arg1: str) -> None:
        pass
    @property
    def username(self) -> str:
        """
        :type: str
        """
    @username.setter
    def username(self, arg1: str) -> None:
        pass
    pass
def can_parse(input: str, base_input: str = None) -> bool:
    pass
def parse(arg0: str) -> URL:
    pass
def idna_encode(arg0: str) -> bytes:
    pass
def idna_decode(arg0: bytes) -> str:
    pass
__version__ = '1.0.0'
