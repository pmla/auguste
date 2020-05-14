import pytest
import numpy as np
import scipy.linalg
from numpy.testing import assert_allclose
from auguste import symmetrize_lattice
from scipy.spatial.transform import Rotation
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

simple = ["primitive cubic",
          "primitive tetragonal",
          "primitive orthorhombic",
          "primitive hexagonal"]


@pytest.mark.parametrize("seed", range(5))
@pytest.mark.parametrize("name, cell", input_data.items())
def test_strain(name, cell, seed):
    rng = np.random.RandomState(seed=seed)
    P = rng.uniform(-0.1, 0.1, (3, 3))
    _, P = scipy.linalg.polar(P)
    P += np.eye(3)
    strained = (P @ cell.T).T

    distance, symmetrized = symmetrize_lattice(strained, name)

    F = symmetrized.T @ np.linalg.inv(strained.T)
    assert_allclose(symmetrized.T, F @ strained.T, atol=TOL)

    _, P = scipy.linalg.polar(F)
    assert_allclose(distance, np.linalg.norm(P - np.eye(3)), atol=TOL)


@pytest.mark.parametrize("seed", range(5))
@pytest.mark.parametrize("name, cell", [(k, v) for k, v in input_data.items()
                                        if k in simple])
def test_correspondence(name, cell, seed):

    rng = np.random.RandomState(seed=seed)
    q = Rotation.random(random_state=rng).as_matrix()

    inputL = np.array([[1, 2, 3], [0, 1, 0], [0, 0, 1]])
    assert np.linalg.det(inputL) == 1
    cellLQ = (cell.T @ inputL.T).T @ q.T
    result = symmetrize_lattice(cellLQ, name,
                                return_correspondence=True)
    distance, symmetrized, Q, L = result

    assert distance < TOL
    assert_allclose(symmetrized, cellLQ, atol=TOL)
    assert_allclose((cellLQ.T @ L).T @ Q.T, cell, atol=TOL)


@pytest.mark.parametrize("name, cell", input_data.items())
def test_equality(name, cell):
    distance, symmetrized = symmetrize_lattice(cell, name)
    assert distance < TOL
    assert_allclose(symmetrized, cell, atol=TOL)


@pytest.mark.parametrize("name, cell", input_data.items())
def test_calculate_vector(name, cell):
    index = auguste.names.index(name)
    distances = auguste.calculate_vector(cell)
    assert distances[0] < TOL
    assert distances[index] < TOL


@pytest.mark.parametrize("name, cell", input_data.items())
def test_calculate_vector_basis_invariance(name, cell):
    inputL = np.array([[1, 2, 3], [0, 1, 0], [0, 0, 1]])
    cellL = (cell.T @ inputL.T).T

    distancesL = auguste.calculate_vector(cellL)
    distances = auguste.calculate_vector(cell)
    assert_allclose(distances, distancesL, atol=TOL)
