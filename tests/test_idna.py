import can_ada


def test_encode():
    assert can_ada.idna_encode('Bücher.example') == b'xn--bcher-kva.example'


def test_decode():
    assert can_ada.idna_decode(b'xn--bcher-kva.example') == 'bücher.example'
