import numpy as np
import matplotlib.pyplot as plt
from math import pi
import argparse
import serial
import array
import socket
import os
import math

f = open("images.h", "w")


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

def convert_image(image, nb_slices: int) -> dict:
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

    height, width = image.shape

    # img_resized = cv2.resize(image, (math.floor((nb_leds * 2 - 1) / height * width), nb_leds * 2 - 1))

    # height_resized, width_resized = img_resized.shape
    w_c = math.floor(width / 2)
    h_c = math.floor(height / 2)

    data = {}
    unit = 2 * pi / nb_slices

    for i in range(nb_slices):
        theta = 2 * math.pi / nb_slices * i
        for r in range(nb_leds):
            x = int(np.cos(theta) * r + nb_leds)
            y = int(- np.sin(theta) * r + nb_leds)
            if 0 <= x < width and 0 <= y < height:
                value = int(image[x, y])

                if theta not in data:
                    data[theta] = []

                data[theta].append({
                    "r": r,
                    "value": image[x, y]
                })

    # plot_polar_image(data)

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
        date = ms_to_atmega_time((theta * resolution_time) / (2 * pi))
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
        if (history and [date, word] in history) or word == 0:
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

    serialized = """#ifndef __IMAGES_H__
#define __IMAGES_H__

#include "spi_utils.h"
#include <avr/pgmspace.h>

struct image
{
    struct frame frames[100];
    uint8_t size;
};

const struct image images_library[4][10] PROGMEM = {
    """

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

    serialized += "};\n#endif // !__IMAGES_H__"

    f.write(serialized)


f.close()