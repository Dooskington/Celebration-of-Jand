CC = g++
SRC_PATH = LD34
BIN_PATH = bin
BUILD_PATH = bin
RES_PATH = LD34/res
FRAMEWORK_PATH = /Library/Frameworks/SDL2
FRAMEWORKS = -framework SDL2 -framework SDL2_ttf -framework SDL2_mixer -framework SDL2_image
BIN_NAME = jand
BUILD_NAME = "Celebration of Jand"
C_FLAGS = -Wall -std=c++14
SRC_FILES := $(wildcard $(SRC_PATH)/*.cpp)
OBJ_FILES := $(SRC_FILES:$(SRC_PATH)%.cpp=$(BIN_PATH)%.o)

# Build the project in either debug or release
all: debug

# Build the project and package it into an app bundle
release: clean build clean_app package_app
	@echo "*** Release build complete ***"

# Build the project and run it through the terminal
debug: clean build
	@echo "*** Debug build complete ***"
	#@./$(BIN_PATH)/$(BIN_NAME)

# Link all the .o files in the bin/ directory to create the executable
build: $(OBJ_FILES)
	@echo "*** Linking ***"
	@$(CC) $(OBJ_FILES) -o $(BIN_PATH)/$(BIN_NAME) -F $(FRAMEWORK_PATH) $(FRAMEWORKS)

# Compile all the .cpp files in the src/ directory to the bin/ directory
$(BIN_PATH)/%.o: $(SRC_PATH)/%.cpp
	@echo "*** Compiling" $< "***"
	@$(CC) -F $(FRAMEWORK_PATH) -c $< -o $@ $(C_FLAGS)

# Clean up all the raw binaries
clean:
	@echo "*** Cleaning Binaries ***"
	@mkdir -p $(BIN_PATH)/
	@rm -f $(BIN_PATH)/$(BIN_NAME) $(BIN_PATH)/*.o

clean_app:
	@rm -f -r $(BUILD_PATH)/$(BUILD_NAME).app

package_app:
	@echo "*** Packaging ***"
	@mkdir -p $(BUILD_PATH)/$(BUILD_NAME).app/Contents/MacOS
	@mkdir -p $(BUILD_PATH)/$(BUILD_NAME).app/Contents/Resources
	@cp info.plist $(BUILD_PATH)/$(BUILD_NAME).app/Contents
	@cp -R $(RES_PATH) $(BUILD_PATH)/$(BUILD_NAME).app/Contents/Resources
	@cp -R $(FRAMEWORK_PATH)/ $(BUILD_PATH)/$(BUILD_NAME).app/Contents/Frameworks
	@cp $(BIN_PATH)/$(BIN_NAME) $(BUILD_PATH)/$(BUILD_NAME).app/Contents/MacOS
	@install_name_tool -add_rpath @loader_path/../Frameworks $(BUILD_PATH)/$(BUILD_NAME).app/Contents/MacOS/jand
