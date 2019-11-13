import numpy as np
import matplotlib.pyplot as plt
import auguste


def transform_isometric(p):

    M = np.array([[np.sqrt(3), 0, -np.sqrt(3)],
                  [1, 2, 1],
                  [np.sqrt(2), -np.sqrt(2), np.sqrt(2)]])

    return 1 / np.sqrt(6) * np.dot(M, p.T)[:2].T


def plot_points(cell, ax, c='k'):

    u, v, w = cell
    ps = np.array([[0, 0, 0], u, v, w, u + v, v + w, u + w, u + v + w])
    qs = transform_isometric(ps)

    edges = [[0, 1], [0, 2], [0, 3], [1, 4],
             [1, 6], [2, 4], [2, 5], [3, 5],
             [3, 6], [4, 7], [5, 7], [6, 7]]

    for edge in edges:

        q = qs[edge]
        zorder = -np.max(ps[edge]) + 0.001 * ord(c[0])
        ls = '-:'[4 in edge]
        ax.plot(q[:, 0], q[:, 1], c=c, zorder=zorder, lw=3, ls=ls)

    ax.plot(qs[:, 0], qs[:, 1], c=c, marker='o', markersize=6, ls='')


def run():
    cell = np.array([[0.06, 0, 2.01],
                     [3.04, 0, 0],
                     [1.46, 4.72, 0.88]])

    fig, subplots = plt.subplots(nrows=3, ncols=5, figsize=(14, 10))
    for row in subplots:
        for ax in row:
            ax.axis('off')
            ax.set_aspect('equal', 'datalim')

    maxlen = max([len(name) for name in auguste.names])
    for i, name in enumerate(auguste.names):
        distance, optcell = auguste.symmetrize_lattice(cell, name)
        ax = subplots[i // 5][i % 5]
        ax.set_title("%s\ndistance: %.3f" % (name, distance))
        plot_points(optcell, ax, 'C1')
        plot_points(cell, ax, 'C0')
        print(name.ljust(maxlen + 1), "%.3f" % distance)
    plt.show()


if __name__ == "__main__":
    run()
