CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20
TARGET = library
SRC = AccountClass.cpp LibrarianClass.cpp BookClass.cpp LibraryClass.cpp FacultyClass.cpp StudentClass.cpp LMS.cpp UserClass.cpp 
OBJ = $(SRC:.cpp=.o)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)


# For debugging

# CXX = g++
# CXXFLAGS = -g -Wall -Wextra -pedantic -std=c++17
# TARGET = library
# SRC = AccountClass.cpp LibrarianClass.cpp BookClass.cpp LibraryClass.cpp FacultyClass.cpp StudentClass.cpp LMS.cpp UserClass.cpp 
# OBJ = $(SRC:.cpp=.o)

# $(TARGET): $(OBJ)
# 	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean:
# 	rm -f $(OBJ) $(TARGET)