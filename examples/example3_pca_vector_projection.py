import collections
import numpy as np
import matplotlib.pyplot as plt
from sklearn import decomposition
import auguste


def random_vector(n):
    while 1:
        l = np.random.uniform(0, 1, n)
        norm = np.linalg.norm(l)
        if norm < 1:
            return l / norm


def random_sample(name):
    cell = random_vector(9).reshape((3, 3))
    _, cell = auguste.symmetrize_lattice(cell, name)
    return auguste.calculate_vector(cell)


def sample_path(name, theta):
    s = np.sin(theta)
    c = np.cos(theta)

    if name == "primitive tetragonal":
        cell = np.array([[s, 0, 0], [0, s, 0], [0, 0, c]])
    elif name == "primitive rhombohedral":
        cell = np.array([[s, c, c], [c, s, c], [c, c, s]])
    elif name == "primitive hexagonal":
        th = np.deg2rad(60)
        cell = np.array([[s, 0, 0],
                         [np.cos(th) * s, np.sin(th) * s, 0],
                         [0, 0, c]])
    elif name == "body-centred tetragonal":
        cell = np.array([[-s, s, c], [s, -s, c], [s, s, -c]])
    else:
        raise Exception("Bravais type not implemented")

    return auguste.calculate_vector(cell)


def run():
    print("This example takes 5-10 mins depending on your computer.\n")
    np.random.seed(0)
    data = collections.defaultdict(list)

    # Sample the zero-parameter lattices (distances are scale invariant,
    # which reduces the number of parameters by one).
    print("sampling zero-parameter types...")
    print("\tcubic")
    data['cubic'].append(auguste.calculate_vector(np.eye(3)))
    print("\tfcc")
    data['fcc'].append(auguste.calculate_vector(1 - np.eye(3)))
    print("\tbcc")
    data['bcc'].append(auguste.calculate_vector([[-1, 1, 1],
                                                 [1, -1, 1],
                                                 [1, 1, -1]]))

    # Sample the paths of the 1-parameter lattices
    print("\nsampling single-parameter types...")
    single_types = ["primitive tetragonal",
                    "body-centred tetragonal",
                    "primitive hexagonal",
                    "primitive rhombohedral"]

    for name in single_types:
        print("\t%s" % name)
        for t in np.linspace(0, np.pi / 2, 200)[1:-1]:
            if "rhombohedral" in name:
                t *= 2
            d = sample_path(name, t)
            if np.max(d) < 2:
                data[name].append(d)

    # Sample some points from the monoclinic lattices
    print("\nsampling monoclinic lattice types...")
    num_samples = 500
    double_types = ["base-centred monoclinic", "primitive monoclinic"]
    for name in double_types:
        print("\t%s" % name)
        for i in range(num_samples):
            data[name].append(random_sample(name))

    # Perform a principal component analysis (PCA) of all the data points
    points = np.concatenate(list(data.values()))
    indices = np.where(np.max(points, axis=1) < 2)[0]
    points = points[indices]
    pca = decomposition.PCA()
    pca.fit(points)

    # Plot the zero-parameter lattices in the PCA coordinate system
    for label in ['FCC', 'BCC', 'Cubic']:

        name = label.lower()
        x, y = pca.transform(data[name])[0, :2]
        plt.scatter(x, y, c='k')

        props = {'FCC': ('right', 'top'),
                 'BCC': ('right', 'bottom'),
                 'Cubic': ('right', 'top')}
        ha, va = props[label]
        plt.text(x, y, label, ha=ha, va=va, fontsize=20)

    # Plot the paths of the single-parameter lattices in PCA coordinates
    for i, name in enumerate(single_types):
        x, y = pca.transform(data[name])[:, :2].T
        plt.plot(x, y, c='C%d' % (i // 2), ls='-:'[i % 2], label=name)

    # Plot the monoclinic sample points in PCA coordinates
    for i, name in enumerate(double_types):
        x, y = pca.transform(data[name])[:, :2].T
        plt.scatter(x, y, c='C%d' % i, label=name)

    plt.legend()
    plt.show()


if __name__ == "__main__":
    run()
