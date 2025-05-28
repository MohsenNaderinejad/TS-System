CXX = g++
CXXFLAGS = -Iinclude -std=c++17
SOURCES = $(wildcard src/*.cpp exam/*.cpp user/*.cpp user/teacher/*.cpp user/student/*.cpp)
TARGET = TS-APP_minGWC

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(TARGET).exe