import pytest
import numpy as np
from auguste import minkowski_reduce


TOL = 1E-12


@pytest.mark.parametrize("seed", range(20))
def test_random_3D(seed):
    rng = np.random.RandomState(seed)
    B = rng.uniform(-1, 1, (3, 3))
    R, H = minkowski_reduce(B)

    assert np.allclose(H @ B, R, atol=TOL)
    assert np.sign(np.linalg.det(B)) == np.sign(np.linalg.det(R))

    norms = np.linalg.norm(R, axis=1)
    assert (np.argsort(norms) == range(3)).all()


# Issue 4
def test_similar_norms():

    cell = np.array([[-5.35538034, -0.1350599, -5.2795042],
                     [ 5.35538034, -5.2795042, -0.1350599],
                     [-5.35538034, -5.2795042, -0.1350599]])
    R, H = minkowski_reduce(cell)
