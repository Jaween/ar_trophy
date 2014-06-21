LIBRARIES = -l opencv_core -l opencv_nonfree -l opencv_highgui -l opencv_imgproc -l opencv_features2d -lopencv_flann -lopencv_calib3d
OBJECTS = main.o vision.o

all : ar

ar : $(OBJECTS)
	g++ $(OBJECTS) $(LIBRARIES) -o ar

%.o : %.cpp
	g++ -c $^

clean :
	rm -f ar *.o
