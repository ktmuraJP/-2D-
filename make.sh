SOURCE="./src/main.cpp"
EXEC="2dengine"
g++ $SOURCE -g -Wall\
    -I./stb\
    -I./gl3w\
    -I./src/\
    ./src/fileio.cpp\
    ./src/texture.cpp\
    ./gl3w/GL/gl3w.c\
    -o $EXEC -lm -lGL\
    `pkg-config --static --libs glfw3`\
    `pkg-config --cflags --libs freetype2`\
