CV_LIBRARIES = -l opencv_core -l opencv_nonfree -l opencv_highgui -l opencv_imgproc -l opencv_features2d -lopencv_flann -lopencv_calib3d
GL_LIBRARIES = -lGLEW -lGL -lGLU -lglfw
OBJECTS = main.o vision.o renderer.o shader_helper.o texture_helper.o model_helper.o controls.o model.o shader_program.o

all : ar

ar : $(OBJECTS)
	g++ $(OBJECTS) $(CV_LIBRARIES) $(GL_LIBRARIES) -o ar

%.o : %.cpp
	g++ -c $^

clean :
	rm -f ar *.o
