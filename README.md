# GraphicsLibrary

A lightweight software rendering graphics framework

## Features
- Allows the creation of graphical applications that run in the desktop via SDL and in a tty via the terminal
- All functionality is raw software rendering; there is no reliance on outside rendering libraries
- Small size (~200 kb compiled)
- Contains basic functionality in the following areas:
    - Geometry shapes (lines, rectangles, triangles, etc.)
    - Image formats (basic PNG and JPEG support)
    - Font rendering
        - Rudimentary boxy font
        - Some TrueType support
    - Box Blur (works but needs optimization)
    - Basic GUI elements (text boxes, buttons, etc.)

## Current work
- Cleaning up code and increasing modularity
- Making it a true library instead of being compiled into every application