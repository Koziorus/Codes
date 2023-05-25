import matplotlib.pyplot as plt
import pickle
import os
import json
from part1_data_preparation import load_data, standardization
from part2_model_definition import build_model, FFN_Hyperparams


def plot_predictions(test_targets, predictions):
    a = plt.axes(aspect='equal')
    plt.scatter(test_targets, predictions)
    plt.xlabel('True Values [MPG]')
    plt.ylabel('Predictions [MPG]')
    lims = [0, 50]
    plt.xlim(lims)
    plt.ylim(lims)
    _ = plt.plot(lims, lims)
    plt.show()


if __name__ == '__main__':

    # load and transform test data
    test_features, test_targets = load_data('data_new/test_data.npz')
    # load standardizer
    with open('data_new/standardizer.pkl', 'rb') as f:
        standardizer = pickle.load(f)
    test_features, _ = standardization(test_features, standardizer=standardizer)

    # TODO choose most promissing setup (according to val_loss) by specifying experiment_dir
    experiment_dir = 'exp_04'

    # load hyperparams
    with open(os.path.join(experiment_dir, 'hp.json')) as f:
        hp = json.load(f)
    hp = FFN_Hyperparams(**hp)

    # build model and load best weights
    model = build_model(hp)
    model.load_weights(os.path.join(experiment_dir, 'model_best_weights'))

    # evaluation
    test_metrics = model.evaluate(test_features, test_targets)

    # prediction and error analysis
    predictions = model.predict(test_features)
    plot_predictions(test_targets, predictions)
