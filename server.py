import numpy as np
import matplotlib.pyplot as plt
from math import pi
import argparse
import serial
import array
import socket

display_mac_address = ""

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


def package_data(data: dict, resolution_time=52) -> None:
    result = array.array('B', [])

    for theta in data:
        # Convert theta in ms
        date = ms_to_atmega_time(int((theta * resolution_time) / (2 * pi)))
        word = 0b00_00_00_00_00_00_00_00

        for struct in data[theta]:
            if struct["value"] == 255:
                word |= (0b1 << struct["r"])

        print("{" + hex(date) + ", " + hex(word) + "},")
        
    return result.tobytes()


def ms_to_atmega_time(time: int, prescaler: int = 64) -> int:
    return int((13000 * time) / prescaler)

if __name__ == "__main__":
    # Parse arguments
    parser = argparse.ArgumentParser(description='Controls the POV display')
    parser.add_argument('--image', type=str, help='Path to the image to display', required=True)
    parser.add_argument('--nb_slices', type=int, help='Number of slices', default=150)
    parser.add_argument('--nb_leds', type=int, help='Number of leds per slice', default=16)
    parser.add_argument('--display', type=bool, help='Display the image', default=False)

    args = parser.parse_args()

    # Load the image
    image_data = load_image(args.image)

    # Convert image to polar coordinates
    polar_data = convert_image(image_data, args.nb_slices, args.nb_leds)
    payload = package_data(polar_data)

    # Print polar image from data
    if args.display:
        plot_polar_image(polar_data)

    sock = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM)
    adapter = "B8:9A:2A:55:DC:4F" # sudo ls /var/lib/bluetooth/
    device = "98:D3:11:FC:64:AD" # POV 12

    sock.connect((device, 1))
    print("Connected")

    # Retrieving banner
    # print(sock.recv(20))

    # Sending image
    # sock.send(array.array('B', [0x69, 0x6D, 0x67, 0x0D, 0x0A]).tobytes()) # img
    # print(payload)
    # sock.send(payload)
    # sock.send(array.array('B', [0x65, 0x6E, 0x64, 0x0D, 0x0A]).tobytes()) # end

    # print("Image sent")

    try:
        while True:
            data = sock.recv(25)
            print(data)
    except KeyboardInterrupt:
        pass
        
    sock.close()
    