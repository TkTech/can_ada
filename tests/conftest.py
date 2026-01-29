from pathlib import Path
import pytest


def pytest_addoption(parser):
    parser.addoption(
        '--runslow',
        action='store_true',
        default=False,
        help='run slow tests'
    )


def pytest_configure(config):
    config.addinivalue_line('markers', 'slow: mark test as slow to run')


def pytest_collection_modifyitems(config, items):
    if config.getoption('--runslow'):
        # --runslow given in cli: do not skip slow tests
        return

    skip_slow = pytest.mark.skip(reason='need --runslow option to run')
    for item in items:
        if 'slow' in item.keywords:
            item.add_marker(skip_slow)


@pytest.fixture(scope="session")
def top100str() -> list[str]:
    current_file_dir = Path(__file__).parent
    with open(current_file_dir / "data" / "top100.txt", "r") as f:
        return f.readlines()


@pytest.fixture(scope="session")
def top100bytes() -> list[bytes]:
    current_file_dir = Path(__file__).parent
    with open(current_file_dir / "data" / "top100.txt", "rb") as f:
        return f.readlines()
