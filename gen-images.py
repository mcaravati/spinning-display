from PIL import Image, ImageDraw, ImageFont
import os

# Define the size of each zone and the margin
zone_size = 16
margin = 1

# Define the coordinates for each zone
zones = [
    (margin, margin, zone_size + margin, zone_size + margin),             # Upper left
    (16 - margin - zone_size, margin, 16 - margin, zone_size + margin),   # Upper right
    (margin, 16 - margin - zone_size, zone_size + margin, 16 - margin),   # Bottom left
    (16 - margin - zone_size, 16 - margin - zone_size, 16 - margin, 16 - margin),  # Bottom right
]

# Choose a font and size for the digits
font = ImageFont.load_default()


# Create 'upper left' directory if it does not exist
if not os.path.exists('upper_left'):
    os.makedirs('upper_left')

for i in range(0, 3):
    # Create a 32x32 image with a white background
    image = Image.new('RGB', (32, 32), 'black')
    draw = ImageDraw.Draw(image)

    # Write a digit in white at the bottom right corner of the upper left zone
    digit_upper_left = str(i)  # Change this to the desired digit
    text_width, text_height = draw.textsize(digit_upper_left, font=font)
    text_position_upper_left = (margin + zone_size - text_width - 1, margin + zone_size - text_height - 1)
    draw.text(text_position_upper_left, digit_upper_left, font=font, fill='white')

    image = image.convert('L')

    image = image.rotate(-90)

    # Save the image in the 'upper left' directory
    image.save('upper_left/' + str(i) + '.png')

if not os.path.exists('upper_right'):
    os.makedirs('upper_right')

for i in range(0, 10):
    # Create a 32x32 image with a white background
    image = Image.new('RGB', (32, 32), 'black')
    draw = ImageDraw.Draw(image)

    # Write a digit in white at the bottom left corner of the upper right zone
    digit_upper_right = str(i)  # Change this to the desired digit
    text_position_upper_right = (16 + margin + 1, margin + zone_size - text_height - 1)
    draw.text(text_position_upper_right, digit_upper_right, font=font, fill='white')

    image = image.convert('L')

    # Rotate 90deg clockwise
    image = image.rotate(-90)

    # Save the image in the 'upper right' directory
    image.save('upper_right/' + str(i) + '.png')

if not os.path.exists('bottom_left'):
    os.makedirs('bottom_left')

for i in range(0, 7):
    # Create a 32x32 image with a white background
    image = Image.new('RGB', (32, 32), 'black')
    draw = ImageDraw.Draw(image)

    # Write a digit in white at the upper right corner of the bottom left zone
    digit_bottom_left = str(i)  # Change this to the desired digit
    text_width, text_height = draw.textsize(digit_bottom_left, font=font)
    text_position_bottom_left = (zone_size + margin - text_width -1 , zone_size + margin)
    draw.text(text_position_bottom_left, digit_bottom_left, font=font, fill='white')

    image = image.convert('L')
    image = image.rotate(-90)

    # Save the image in the 'bottom left' directory
    image.save('bottom_left/' + str(i) + '.png')

if not os.path.exists('bottom_right'):
    os.makedirs('bottom_right')

for i in range(0, 10):
    # Create a 32x32 image with a white background
    image = Image.new('RGB', (32, 32), 'black')
    draw = ImageDraw.Draw(image)

    # Write a digit in white at the upper left corner of the bottom right zone
    digit_bottom_right = str(i)  # Change this to the desired digit
    text_position_bottom_right = (zone_size + margin + 1, zone_size + margin)
    draw.text(text_position_bottom_right, digit_bottom_right, font=font, fill='white')

    image = image.convert('L')
    image = image.rotate(-90)

    # Save the image in the 'bottom right' directory
    image.save('bottom_right/' + str(i) + '.png')
