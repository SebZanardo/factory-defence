import sys
from enum import IntEnum, auto
from PIL import Image


# NOTE: Should match level.h TileType
class TileType(IntEnum):
    EMPTY = 0
    GROUND = auto()
    WATER = auto()
    PATH = auto()
    PATH_INPUT = auto()
    PATH_OUTPUT = auto()


# Where int is value between 0-255
RGBA = tuple[int, int, int, int]

colour_mapping: dict[TileType, RGBA] = {
    (0, 0, 0, 255): TileType.EMPTY,
    (255, 255, 255, 255): TileType.GROUND,
    (0, 255, 255, 255): TileType.WATER,
    (255, 0, 255, 255): TileType.PATH,
    (255, 0, 0, 255): TileType.PATH_INPUT,
    (0, 255, 0, 255): TileType.PATH_OUTPUT,
}


def main() -> None:
    if len(sys.argv) <= 2:
        print(f"ERROR, usage is {sys.argv[0]} input_file output_file")
        return

    input_path = sys.argv[1]
    output_path = sys.argv[2]

    image = Image.open(input_path)
    map = [['0' for _ in range(image.width)] for _ in range(image.height)]

    # Read image file
    pixels = image.load()
    for y in range(image.height):
        for x in range(image.width):
            pixel = pixels[x, y]
            if pixel in colour_mapping:
                map[y][x] = str(colour_mapping[pixel])
            else:
                print(f"ERROR: undefined pixel value {pixel}")

    # Write output file
    with open(output_path, "w") as f:
        for y in range(image.height):
            f.write(" ".join(map[y]) + '\n')


if __name__ == "__main__":
    main()
