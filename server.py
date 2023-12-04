import numpy as np
import matplotlib.pyplot as plt
from math import pi
import argparse

def load_image(file_path: str) -> np.array:
    """
    Load an image and convert it to grayscale.

    file_path: str
        Path to the image to load
    
    return: np.array
        Image data
    """
    image = plt.imread(file_path)

    # Invert x and y
    image = np.transpose(image, (1, 0, 2))

    return np.mean(image, axis=-1)

def convert_image(image: str, nb_slices: int, nb_leds: int) -> dict:
    """
    Convert an image to polar coordinates.

    image: np.array
        Image to convert
    nb_slices: int
        Number of slices, a slice is a radial line from the center of the image
    nb_leds: int
        Number of leds per slice

    return: dict
        Dictionary containing the image data in polar coordinates
    """
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
            if r['value'] == 255.:
                plt.polar(theta, r['r'], 'ro')

    plt.show()


if __name__ == "__main__":
    # Parse arguments
    parser = argparse.ArgumentParser(description='Controls the POV display')
    parser.add_argument('--image', type=str, help='Path to the image to display', required=True)
    parser.add_argument('--nb_slices', type=int, help='Number of slices', default=150)
    parser.add_argument('--nb_leds', type=int, help='Number of leds per slice', default=16)

    args = parser.parse_args()

    # Load the image
    image_data = load_image(args.image)

    # Convert image to polar coordinates
    polar_data = convert_image(image_data, args.nb_slices, args.nb_leds)
    
    # Print polar image from data
    plot_polar_image(polar_data)
