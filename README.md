# auguste

A python module for minimum-strain symmetrization of Bravais lattices.  Symmetrization produces a strain tensor, which is reduced to a scalar quantity.  This quantifies the 'distance' of a lattice from each Bravais class.  The distance can be used for classification of Bravais lattices, and visualization in an abstract Bravais space.


### Installation:

To install the module with pip (recommended):
```
pip install --user auguste
```

To install directly from the git repository:
```
pip install --user git+https://github.com/pmla/auguste
```

To do a manual build and installation:
```
python3 setup.py build
python3 setup.py install --user
```

### Usage:
To import the module:
```
>>> import auguste
```

To symmetrize a lattice (with rows as unit vectors):
```
>>> cell = [[0, 1, 1], [1, 0, 1], [1, 1, 0]]
>>> distance, symmetrized = auguste.symmetrize_lattice(cell, "primitive tetragonal")
>>> distance
0.4714045207910317
>>> symmetrized
array([[-0.37037037,  0.74074074,  0.74074074],
       [ 0.74074074, -0.37037037,  0.74074074],
       [ 0.74074074,  0.74074074, -0.37037037]])
```

Supported lattice names are stored in the module variables:
```
>>> auguste.names
('primitive triclinic', 'primitive monoclinic', 'base-centred monoclinic', 'primitive orthorhombic', 'base-centred orthorhombic', 'body-centred orthorhombic', 'face-centred orthorhombic', 'primitive tetragonal', 'body-centred tetragonal', 'primitive rhombohedral', 'primitive hexagonal', 'primitive cubic', 'body-centred cubic', 'face-centred cubic')
>>> auguste.pearson
('aP', 'mP', 'mS', 'oP', 'oS', 'oF', 'oI', 'tP', 'tI', 'hP', 'hR', 'cP', 'cF', 'cI')
```
To calculate the symmetrization distances from all Bravais types:
```
>>> auguste.calculate_vector(cell)
array([0.00000000e+00, 4.28372991e-01, 5.24426159e-16, 4.71404521e-01,
       4.28372991e-01, 3.80565279e-16, 1.92296269e-16, 4.71404521e-01,
       4.07921987e-16, 1.92296269e-16, 4.60895951e-01, 4.71404521e-01,
       2.61971659e-01, 0.00000000e+00])
```

### Information
If you use auguste in a publication, please cite:

Peter M. Larsen, Edward L. Pang, Pablo A. Parrilo, and Karsten W. Jacobsen; "Minimum-strain symmetrization of Bravais lattices"; Phys. Rev. Research 2, 013077

Article available at https://doi.org/10.1103/PhysRevResearch.2.013077

Preprint available at https://arxiv.org/abs/1910.03433.
