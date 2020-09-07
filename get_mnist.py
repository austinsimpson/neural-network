import tensorflow
import sys
import os
import numpy as np
from PIL import Image

mnist = tensorflow.keras.datasets.mnist

labels= {
    0: (1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
    1: (0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
    2: (0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
    3: (0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
    4: (0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0),
    5: (0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0),
    6: (0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0),
    7: (0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0),
    8: (0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0),
    9: (0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0),
}

def main():
    export_mnist_data("C:/Users/Austin/Desktop")


def export_mnist_data(root_directory):
    export_dir = root_directory + "/mnist_data"
    if os.path.exists(export_dir) == False:
        os.mkdir(export_dir)
        for i in range(10):
            os.mkdir(export_dir + "/" + str(i))

    counts = {}
    for i in range(10):
            counts[i] = 0

    constexpr_bois = []

    (x_train, y_train), (x_test, y_test) = mnist.load_data()
    for image, label, _ in zip(x_train, y_train, range(1000)):
        image_flattened = np.reshape(image, (28 * 28))
        constexpr_boi = "\tTrainingPointND<" + str(28 * 28) + ", " + str(10) + ">{{" + ", ".join(str(pixel / 255) for pixel in image_flattened) + "}, {" + label_helper(label) + "}}"
        constexpr_bois.append(constexpr_boi)

    array_body = ",\n".join(constexpr_bois)
    result = "#pragma once\n#include ""TrainingPointND.h""\n\nconstexpr std::array MnistDataSet = {\n" + array_body + "\n};"
    myFile = open(export_dir + "/MnistDataset.h", "w")
    myFile.write(result)
    myFile.close()

def label_helper(label):
    return ", ".join(str(i) for i in labels[label])

if __name__ == "__main__":
    main()