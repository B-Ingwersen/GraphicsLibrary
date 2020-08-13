SOURCE_DIR = src
INCLUDE_DIR = include
RELATIVE_INCLUDE_DIR = $(INCLUDE_DIR)/GraphicsLibrary
OUTPUT_DIR = obj
CXX = g++
CXXFLAGS = -I$(INCLUDE_DIR)

LIBS = -lSDL2 -lpthread

_INC_DEPS = Core.h Definitions.h RenderInfastructure/DrawingEventHandlers.h RenderInfastructure/DrawingEvents.h RenderInfastructure/RenderTimeOps.h RenderInfastructure/SizeChange.h RenderInfastructure/WindowSections.h Drawing/Functions.h Drawing/Strings.h Drawing/FunctionCalls.h Drawing/OffscreenBuffers.h Drawing/FunctionExecute.h Drawing/Objects.h Inputs/Keyboard.h Extensions/BasicGUIElements.h Extensions/WindowShadows.h Extensions/WindowShadows.h Extensions/DataOperations.h Extensions/Text.h Extensions/ThemeExp.h Extensions/FileTypes/Jpeg.h Extensions/FileTypes/Png.h Extensions/FileTypes/Mp4.h Extensions/Files.h Extensions/TrueType.h

_SRC_DEPS = FBFramework/Framebuffer.h FBFramework/Keyboard.h FBFramework/Mouse.h FBFramework/Framework.h SDLFramework/Keyboard.h SDLFramework/SizeChange.h SDLFramework/Framework.h
INC_DEPS = $(patsubst %,$(RELATIVE_INCLUDE_DIR)/%,$(_INC_DEPS))
SRC_DEPS = $(patsubst %,$(SOURCE_DIR)/%,$(_SRC_DEPS))

_OBJ = Core.o Declarations.o RenderInfastructure/WindowSections.o RenderInfastructure/DrawingEventHandlers.o RenderInfastructure/DrawingEvents.o RenderInfastructure/SizeChange.o RenderInfastructure/RenderTimeOps.o Drawing/Functions.o Drawing/Strings.o Drawing/FunctionCalls.o Drawing/OffscreenBuffers.o Drawing/FunctionExecute.o Drawing/Objects.o Inputs/Keyboard.o FBFramework/Framebuffer.o FBFramework/Keyboard.o FBFramework/Mouse.o FBFramework/Framework.o SDLFramework/Keyboard.o SDLFramework/SizeChange.o SDLFramework/Framework.o Extensions/BasicGUIElements.o Extensions/WindowShadows.o Extensions/DataOperations.o Extensions/Text.o Extensions/ThemeExp.o Extensions/FileTypes/Jpeg.o Extensions/FileTypes/Png.o Extensions/FileTypes/Mp4.o Extensions/Files.o Extensions/TrueType.o
OBJ = $(patsubst %,$(OUTPUT_DIR)/%,$(_OBJ))

$(OUTPUT_DIR)/%.o : $(SOURCE_DIR)/%.cpp $(INC_DEPS) $(SRC_DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

GraphicsLibrary : $(OBJ)
	ld -relocatable -o $(OUTPUT_DIR)/GraphicsLibrary.o $^

Examples/Minesweeper : $(OBJ) $(SOURCE_DIR)/Examples/Minesweeper.o
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJ)
