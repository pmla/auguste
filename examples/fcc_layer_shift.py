import numpy as np
import matplotlib.pyplot as plt
from auguste import symmetrize_lattice


def run():

    names = ['fcc', 'bcc', 'rhombohedral', 'hexagonal']

    delta = np.array([0, 0, 2]) - [1, 1, 0]
    fcc_cell = np.array([[1, 1, 0], [1, 0, 1], [0, 1, 1.]])

    fs = []
    ts = np.linspace(0, 1, 100)
    for t in ts:
        cell = fcc_cell + t * delta
        row = np.array([symmetrize_lattice(cell, name)[0] for name in names])
        fs.append(row)
        print(t, row)

    fs = np.array(fs)
    for label, ds in zip(names, fs.T):
        plt.plot(ts, ds, label=label)
    plt.legend()
    plt.show()

if __name__ == "__main__":
    run()
