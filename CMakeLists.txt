
cmake_minimum_required(VERSION 3.20) # common to every CLion project
project(FlappyBird) # project name


set(OpenGlLinkers -lglut -lGLU -lGL) # setting all the Glut libraries as one variable.

################################################

add_executable(FlappyBird main.cpp ) #common to all clion project
target_link_libraries(FlappyBird ${OpenGlLinkers}) # linking opengl libraries to the project
