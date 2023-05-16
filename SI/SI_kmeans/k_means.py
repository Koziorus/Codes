import numpy as np


def initialize_centroids_forgy(data, k):
    return np.random.permutation(data)[:k]


def initialize_centroids_kmeans_pp(data, k):
    centroids = np.zeros((k, np.shape(data)[1]))
    centroids[0] = data[np.random.randint(len(data))]

    for new_centroid_i in range(1, k):
        max_distance = 0
        farthest_observation = 0
        for existing_centroid_i in range(new_centroid_i):
            for observation_i in range(len(data)):
                distance = np.sqrt(np.sum((centroids[existing_centroid_i] - data[observation_i]) ** 2))
                if distance > max_distance:
                    max_distance = distance
                    farthest_observation = observation_i

        centroids[new_centroid_i] = data[farthest_observation]

    return centroids


def assign_to_cluster(data, centroids):
    assignments = np.zeros(len(data), dtype=np.dtype(int))

    for observation_i in range(len(data)):
        min_distance = 100000000
        closest_centroid = 0
        for centroid_i in range(len(centroids)):
            distance = np.sqrt(np.sum((centroids[centroid_i] - data[observation_i]) ** 2))
            if distance < min_distance:
                min_distance = distance
                closest_centroid = centroid_i

        assignments[observation_i] = closest_centroid

    return assignments


def update_centroids(data, assignments):
    centroids_num = np.max(assignments) + 1
    centroids = np.zeros((centroids_num, np.shape(data)[1]))

    for i in range(centroids_num):
        centroids[i] = np.average(data[assignments == i], axis=0)

    return centroids


def mean_intra_distance(data, assignments, centroids):
    return np.sqrt(np.sum((data - centroids[assignments, :]) ** 2))


def k_means(data, num_centroids, kmeansplusplus=False):
    # centroids initizalization
    if kmeansplusplus:
        centroids = initialize_centroids_kmeans_pp(data, num_centroids)
    else:
        centroids = initialize_centroids_forgy(data, num_centroids)

    assignments = assign_to_cluster(data, centroids)
    for i in range(100):  # max number of iteration = 100
        print(f"Intra distance after {i} iterations: {mean_intra_distance(data, assignments, centroids)}")
        centroids = update_centroids(data, assignments)
        new_assignments = assign_to_cluster(data, centroids)
        if np.all(new_assignments == assignments):  # stop if nothing changed
            break
        else:
            assignments = new_assignments

    return new_assignments, centroids, mean_intra_distance(data, new_assignments, centroids)
