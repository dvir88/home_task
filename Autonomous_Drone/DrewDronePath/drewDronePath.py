import os
import matplotlib.pyplot as plt
import itertools
import re


def get_file_paths_from_directory(directory, file_extension=".txt"):
    """
    This function will return a list of file paths from a directory
    that have the specified file extension.
    """
    file_paths = []
    for filename in os.listdir(directory):
        if filename.endswith(file_extension):
            file_paths.append(os.path.join(directory, filename))
    return file_paths


def plot_drone_paths(file_paths, output_image):
    """
    Reads the coordinates from multiple files, drew the drone paths in different colors,
    and saves the drew as an image file.
    """

    plt.figure(figsize=(10, 6))

    # Define a list of colors to use for each drone path
    color_cycle = itertools.cycle(['b', 'g', 'r', 'c', 'm', 'y', 'k', 'salmon', 'slate gray', 'pink'])  # Blue, Green, Red, Cyan, Magenta, Yellow, Black

    # Iterate over each file and drew the path with a different color
    for i, file_path in enumerate(file_paths):
        x_coords, y_coords = read_coordinates(file_path)

        if x_coords and y_coords:
            # drew the path for the current file with the next color from the cycle
            color = next(color_cycle)
            plt.plot(x_coords, y_coords, marker='o', linestyle='-', color=color, label=f"Drone {i + 1}")
        else:
            print(f"No valid coordinates found in file: {file_path}")

    # Add labels, title, and legend
    plt.xlabel('X Coordinates')
    plt.ylabel('Y Coordinates')
    plt.title('Drone Paths')
    plt.legend()

    # Save the drew as an image
    plt.grid(True)
    plt.savefig(output_image)  # Saving the figure to an image file
    print(f"Paths image saved to {output_image}")
    plt.close()


def read_coordinates(filename):
    """
    Reads the coordinates from the given file and returns lists of x and y coordinates.
    """
    x_coords = []
    y_coords = []

    with open(filename, 'r') as file:
        for line in file:
            # Skip empty lines or comment lines
            if not line.strip() or line.startswith('#'):
                continue

            # extract the current coordinates
            match = re.search(r"\(([-+]?[0-9]*\.?[0-9]+), ([-+]?[0-9]*\.?[0-9]+)\)", line)

            if match:
                # Extract the x and y values from the file
                x = float(match.group(1))
                y = float(match.group(2))

                # Append the coordinates to the lists
                x_coords.append(x)
                y_coords.append(y)
            else:
                print(f"Invalid data format in line: {line}")

    return x_coords, y_coords


if __name__ == '__main__':
    # Specify the directory where the drone path files are located
    directory = 'C:\\Users\\dvir\\source\\repos\\Autonomous_Drone'

    # Get all .txt files in the directory
    file_paths = get_file_paths_from_directory(directory)

    if not file_paths:
        print(f"No .txt files found in directory: {directory}")
    else:
        # Specify the output image file to save the drew
        output_image = 'drone_paths_image.png'

        # drew the drone paths and save as an image
        plot_drone_paths(file_paths, output_image)
