SOURCE="./src/main.cpp"
EXEC="2dengine"
g++ $SOURCE -g -Wall\
    -I./lib/stb\
    -I./lib/gl3w\
    -I./src/\
    ./src/fileio.cpp\
    ./src/texture.cpp\
    ./lib/gl3w/GL/gl3w.c\
    -o $EXEC -lm -lGL\
    `pkg-config --static --libs glfw3`\
    `pkg-config --cflags --libs freetype2`\
