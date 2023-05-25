import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns
import os
from sklearn import preprocessing


def get_data():
    url = 'http://archive.ics.uci.edu/ml/machine-learning-databases/auto-mpg/auto-mpg.data'
    column_names = ['MPG', 'Cylinders', 'Displacement', 'Horsepower', 'Weight',
                    'Acceleration', 'Model Year', 'Origin']

    raw_dataset = pd.read_csv(url, names=column_names, na_values='?', comment='\t',
                              sep=' ', skipinitialspace=True)

    return raw_dataset


def inspect_data(dataset):
    print('Dataset shape:')
    print(dataset.shape)

    print('Tail:')
    print(dataset.tail())

    print('Statistics:')
    print(dataset.describe().transpose())

    print('Missing values per feature:')
    print(dataset.isna().sum())

    sns.pairplot(dataset[['MPG', 'Cylinders', 'Displacement', 'Weight']], diag_kind='kde')
    plt.show()


def data_cleanup(raw_dataset, verbose=0):
    dataset = raw_dataset.copy()

    # remove entries with missing values
    dataset = dataset.dropna()

    # convert categorical column to one-hot encoding
    dataset['Origin'] = dataset['Origin'].map({1: 'USA', 2: 'Europe', 3: 'Japan'})
    dataset = pd.get_dummies(dataset, prefix='', prefix_sep='')

    if verbose:
        print(dataset.tail())

    return dataset


def split_data(dataset):
    train_dataset = dataset.sample(frac=0.8, random_state=0)
    test_dataset = dataset.drop(train_dataset.index)

    return train_dataset, test_dataset


def extract_targets(dataset):
    features = dataset.copy()
    targets = features.pop('MPG')
    features = features.to_numpy(np.float32)
    targets = targets.to_numpy(dtype=np.float32)
    return features, targets


def load_data(path='data/train_data.npz'):
    train_data = np.load(path)
    train_features = train_data['features']
    train_targets = train_data['targets']
    return train_features, train_targets


# TODO
def standardization(features: np.ndarray, standardizer=None):
    """
    Normalization of data using sklearn standardizer.
    See: https://scikit-learn.org/stable/modules/preprocessing.html

    :param features:
    :param standardizer: if None, normalizer is adapted from data (training stage)
    :return: (normalized_features, normalizer)
    """

    if standardizer is None:
        standardizer = preprocessing.StandardScaler().fit(features)

    normalized_features = standardizer.transform(features)

    return normalized_features, standardizer


if __name__ == '__main__':
    raw_data = get_data()

    inspect_data(raw_data)

    data = data_cleanup(raw_data)

    train_dataset, test_dataset = split_data(data)
    train_features, train_targets = extract_targets(train_dataset)
    test_features, test_targets = extract_targets(test_dataset)

    # show preprocessed data
    np.set_printoptions(precision=1, floatmode='fixed', suppress=True)
    print("First 6 training pairs: ")
    for n in range(6):
        print(f"Features: {train_features[n, :]} -> targets: {train_targets[n]:.2f}")
    print(f"Train features shape: {train_features.shape}")
    print(f"Train labels shape: {train_targets.shape}")
    print(f"Test features shape: {train_features.shape}")
    print(f"Test labels shape: {test_targets.shape}")

    # save preprocessed data
    data_dir = 'data_new'
    os.makedirs(data_dir, exist_ok=True)
    np.savez(os.path.join(data_dir, 'train_data'), features=train_features, targets=train_targets)
    np.savez(os.path.join(data_dir, 'test_data'), features=test_features, targets=test_targets)
