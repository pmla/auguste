import pytest
import numpy as np
from numpy.testing import assert_allclose
import auguste


TOL = 1E-10


input_data = {"primitive cubic": np.eye(3),
              "primitive tetragonal": np.diag([1, 1, 2]),
              "primitive orthorhombic": np.diag([1, 2, 3]),
              "primitive hexagonal": np.array([[1, 0, 0], [-1/2, np.sqrt(3) / 2, 0], [0, 0, 1]]),
              "face-centred cubic": np.array([[1, 1, 0], [1, 0, 1], [0, 1, 1]]),
              "body-centred cubic": np.array([[1, 1, -1], [1, -1, 1], [-1, 1, 1]]),
              "base-centred orthorhombic": np.array([[2, 1, 0], [2, -1, 0], [0, 0, 4]])
             }
@pytest.mark.parametrize("name, cell", input_data.items())
def test_equality(name, cell):
    distance, symmetrized = auguste.symmetrize_lattice(cell, name)
    assert distance < TOL
    assert_allclose(symmetrized, cell, atol=TOL)

    index = auguste.names.index(name)
    distances = auguste.calculate_vector(cell)
    assert distances[0] < TOL
    assert distances[index] < TOL
