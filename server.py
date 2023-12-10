import numpy as np
import matplotlib.pyplot as plt
from math import pi
import argparse
import serial
import array
import socket
import os

f = open("image.txt", "w")


# https://blog.kevindoran.co/bluetooth-programming-with-python-3/

def load_image(file_path: str) -> np.array:
    """
    Load an image and convert it to grayscale.

    file_path: str
        Path to the image to load
    
    return: np.array
        Image data
    """
    image = plt.imread(file_path)

    image = np.expand_dims(image, axis=2)
    image = np.transpose(image, (1, 0, 2))

    return np.mean(image, axis=-1)

def convert_image(image: str, nb_slices: int) -> dict:
    """
    Convert an image to polar coordinates.

    image: np.array
        Image to convert
    nb_slices: int
        Number of slices, a slice is a radial line from the center of the image

    return: dict
        Dictionary containing the image data in polar coordinates
    """
    nb_leds = 16
    data = {}
    unit = 2 * pi / nb_slices

    for i in range(nb_slices):
        theta = i * unit
        for r in range(nb_leds):
            # Convert to cartesian coordinates
            x = int(np.cos(theta) * r + nb_leds)
            y = int(- np.sin(theta) * r + nb_leds)

            if theta not in data:
                data[theta] = []

            data[theta].append({
                "r": r,
                "value": image[x, y]
            })

    return data

def plot_polar_image(data: dict) -> None:
    """
    Plot a polar image from data. Only plots the points with value 255.

    data: dict
        Dictionary containing the image data
    """

    plt.figure()
    for theta in data:
        for r in data[theta]:
            if r['value'] == 255. or r['value'] == 1.:
                plt.polar(theta, r['r'], 'ro')

    plt.show()


def package_data(data: dict, resolution_time=52) -> None:
    result = "{"
    nb_frames = 0

    history = []

    for theta in data:
        # Convert theta in ms
        date = ms_to_atmega_time(int((theta * resolution_time) / (2 * pi)))
        word = 0b00_00_00_00_00_00_00_00

        for struct in data[theta]:
            if struct["value"] == 255 or struct["value"] == 1:
                word |= (0b1 << struct["r"])

        a = 0
        for i in range(16):
            if word & (1 << i):
                a |= (1 << (15 - i))

        word = a

        # Do not append if already in the list
        if history and [date, word] in history:
            continue

        history.append([date, word])
        result += "{" + hex(date) + ", " + hex(word) + "},"
        nb_frames += 1

    result += "}"
        
    return result, nb_frames


def ms_to_atmega_time(time: int, prescaler: int = 64) -> int:
    return int((13000 * time) / prescaler)

if __name__ == "__main__":
    # Parse arguments
    parser = argparse.ArgumentParser(description='Controls the POV display')
    parser.add_argument('--nb_slices', type=int, help='Number of slices', default=150)

    args = parser.parse_args()

    serialized = "{"

    for firectory in ['upper_left', 'upper_right', 'bottom_left', 'bottom_right']:

        serialized += "{"
        
        # List all files in the directory
        files = os.listdir(firectory)

        for image in files:
            serialized += "{.frames="

            # Load the image
            image_data = load_image(firectory + '/' + image)
            data, nb_frames = package_data(convert_image(image_data, args.nb_slices))

            serialized += data
            serialized += ", .size=" + str(nb_frames) + "},"

        serialized += "},"

    serialized += "}"

    f.write(serialized)


f.close()