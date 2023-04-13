from itertools import compress
import random
import time
import matplotlib.pyplot as plt
import numpy as np

from data import *


def initial_population(individual_size, population_size):
    return [[random.choice([True, False]) for _ in range(individual_size)] for _ in range(population_size)]


def fitness(items, knapsack_max_capacity, individual):
    total_weight = sum(compress(items['Weight'], individual))
    if total_weight > knapsack_max_capacity:
        return 0
    return sum(compress(items['Value'], individual))


def population_best(items, knapsack_max_capacity, population):
    best_individual = None
    best_individual_fitness = -1
    for individual in population:
        individual_fitness = fitness(items, knapsack_max_capacity, individual)
        if individual_fitness > best_individual_fitness:
            best_individual = individual
            best_individual_fitness = individual_fitness
    return best_individual, best_individual_fitness


def roulette_selection(items, knapsack_max_capacity, population, n):
    fitnesses = []
    fitness_sum = 0
    for i in range(len(population)):
        fitnesses.append(fitness(items, knapsack_max_capacity, population[i]))
        fitness_sum += fitnesses[i]

    probabilities = []
    for i in range(len(population)):
        probabilities.append(fitnesses[i] / fitness_sum)

    parents = random.choices(population, weights=probabilities, k=n)

    return parents


def cross_breed(parents, individual_size):
    pairs = []

    for i in range(len(parents) // 2):
        pairs.append((parents[i], parents[i*2]))

    children = []

    for pair in pairs:
        parent_A_first = pair[0][:individual_size//2]
        parent_B_second = pair[1][individual_size//2:]
        parent_A_second = pair[0][individual_size//2:]
        parent_B_first = pair[1][:individual_size//2]

        children.append(np.concatenate((parent_A_first, parent_B_second)))
        children.append(np.concatenate((parent_A_second, parent_B_first)))

    return children


def get_elite(population, n):
    elite = sorted(population, key=lambda x: -fitness(items, knapsack_max_capacity, x))
    return elite[:n]


def mutate(population, mutation_probability):
    for individual in population:
        r = random.random()
        if r < mutation_probability:
            random_item_indx = random.randint(0, len(items) - 1)
            individual[random_item_indx] = not individual[random_item_indx]

    return population

items, knapsack_max_capacity = get_big()
print(items)

population_size = 100
generations = 200
n_selection = 94
n_elite = 6

max_best_fitness = 0
min_best_fitness = 1000000000000000


for tries in range(10):

    start_time = time.time()
    best_solution = None
    best_fitness = 0
    population_history = []
    best_history = []
    population = initial_population(len(items), population_size)
    for generation in range(generations):
        population_history.append(population)

        # TODO: implement genetic algorithma

        elite = get_elite(population, n_elite)
        parents = roulette_selection(items, knapsack_max_capacity, population, n_selection)
        children = cross_breed(parents, len(items))
        children = mutate(children, 1)
        population = np.concatenate((elite, children))

        best_individual, best_individual_fitness = population_best(items, knapsack_max_capacity, population)
        if best_individual_fitness > best_fitness:
            best_solution = best_individual
            best_fitness = best_individual_fitness
        best_history.append(best_fitness)

    end_time = time.time()
    total_time = end_time - start_time
    print('Best solution:', list(compress(items['Name'], best_solution)))
    print('Best solution value:', best_fitness)
    print('Time: ', total_time)

    # plot generations
    x = []
    y = []
    top_best = 10
    for i, population in enumerate(population_history):
        plotted_individuals = min(len(population), top_best)
        x.extend([i] * plotted_individuals)
        population_fitnesses = [fitness(items, knapsack_max_capacity, individual) for individual in population]
        population_fitnesses.sort(reverse=True)
        y.extend(population_fitnesses[:plotted_individuals])
    plt.scatter(x, y, marker='.')
    plt.plot(best_history, 'r')
    plt.xlabel('Generation')
    plt.ylabel('Fitness')
    plt.show()

    max_best_fitness = (best_fitness if best_fitness > max_best_fitness else max_best_fitness)
    min_best_fitness = (best_fitness if best_fitness < min_best_fitness else min_best_fitness)

print("min ", min_best_fitness)
print("max ", max_best_fitness)
