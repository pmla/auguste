# auguste

A python module for minimum-strain symmetrization of Bravais lattices.  Symmetrization produces a strain tensor, which is reduced to a scalar quantity.  This quantifies the 'distance' of a lattice from each Bravais class.  The distance can be used for classification of Bravais lattices, and visualization in an abstract Bravais space.


### Installation:

To install the module with pip (recommended):
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
>>> d, symcell = auguste.symmetrize_lattice(cell, "primitive tetragonal")
```

Supported lattice names are stored in the module variable:
```
>>> auguste.names
('primitive triclinic', 'primitive monoclinic', 'base-centred monoclinic', 'primitive orthorhombic', 'base-centred orthorhombic', 'body-centred orthorhombic', 'face-centred orthorhombic', 'primitive tetragonal', 'body-centred tetragonal', 'primitive rhombohedral', 'primitive hexagonal', 'primitive cubic', 'body-centred cubic', 'face-centred cubic')
```
Pearson symbols are also supported:
```
["aP", "mP", "mS", "oP", "oS", "oF", "oI", "tP", "tI", "hP", "hR", "cP", "cF", "cI"]
```
To calculate the distances from all Bravais types:
```
>>> distances = auguste.calculate_vector(cell)
```

### Information
`auguste` is written by P. M. Larsen.  The software is provided under the MIT license.  A manuscript describing the method is in preparation.
