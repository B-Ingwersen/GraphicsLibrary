#include "GraphicsLibrary/Definitions.h"
#include "GraphicsLibrary/Drawing/Functions.h"
#include "GraphicsLibrary/Drawing/FunctionExecute.h"
#include "GraphicsLibrary/Drawing/FunctionCalls.h"
#include "GraphicsLibrary/Drawing/OffscreenBuffers.h"
#include "GraphicsLibrary/Drawing/Objects.h"
#include "GraphicsLibrary/RenderInfastructure/DrawingEventHandlers.h"
#include "GraphicsLibrary/RenderInfastructure/WindowSections.h"
#include "GraphicsLibrary/RenderInfastructure/DrawingEvents.h"
#include "GraphicsLibrary/RenderInfastructure/SizeChange.h"
#include "GraphicsLibrary/Inputs/Keyboard.h"

void quitApplication( drawData * callingWindow );
void createDrawingEnvironment( void (*initializingFunction)(void * argument), void * initializingArgument );