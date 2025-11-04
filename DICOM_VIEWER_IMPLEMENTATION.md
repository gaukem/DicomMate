# DICOM Viewer Implementation Guide

## Overview
This document describes the implementation of DICOM image viewing functionality in DicomMate using DCMTK library via vcpkg package manager.

## Prerequisites

### 1. Install vcpkg Package Manager
Follow the instructions in `EnvironmentDevelop.md` to install vcpkg and DCMTK:

```powershell
# Clone vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
git checkout 7dc284a6ad0b9cb8ccd618c8dc78c360fbd93b83

# Bootstrap vcpkg
.\bootstrap-vcpkg.bat

# Integrate with Visual Studio
.\vcpkg.exe integrate install

# Install DCMTK
.\vcpkg.exe install dcmtk:x64-windows-static dcmtk:x64-windows
.\vcpkg.exe install openjpeg:x64-windows-static openjpeg:x64-windows
```

### 2. Configure Visual Studio Project
The project is already configured to use vcpkg. Visual Studio will automatically:
- Link DCMTK libraries
- Add DCMTK include directories
- Configure preprocessor definitions

## Architecture

### Class Structure

#### 1. **CDicomImage** (`DicomImage.h/cpp`)
Wrapper class for DCMTK's DicomImage functionality.

**Key Features:**
- Load DICOM files using DCMTK
- Extract pixel data for rendering
- Window/Level adjustment
- Patient/Study metadata extraction

**Methods:**
- `LoadFromFile(filePath)` - Load a DICOM file
- `GetPixelData(frame)` - Get 8-bit grayscale pixel data
- `SetWindowCenter/Width` - Adjust brightness/contrast
- `GetPatientName/StudyDescription` - Extract metadata

#### 2. **CDicomMateDoc** (`DicomMateDoc.h/cpp`)
Document class that holds the DICOM image data.

**Key Features:**
- Stores CDicomImage instance
- Loads DICOM files via LoadDicomFile()
- Updates all views when image changes

#### 3. **CDicomMateView** (`DicomMateView.h/cpp`)
View class that renders DICOM images.

**Key Features:**
- Converts DICOM pixel data to DIB (Device Independent Bitmap)
- Implements zoom, pan, and window/level controls
- Mouse interaction handling
- Real-time image rendering

**Interaction Modes:**
- **Mouse Wheel**: Zoom in/out
- **Left Click + Drag** (Pan mode): Pan the image
- **Left Click + Drag** (Window/Level mode): Adjust brightness/contrast

#### 4. **CMainFrame** (`MainFrm.h/cpp`)
Main application frame with DICOM commands.

**Implemented Commands:**
- `OnDicomOpenFile()` - Open DICOM file dialog
- `OnViewZoomIn/Out/Fit()` - Zoom controls
- `OnViewPan()` - Enable pan mode
- `OnViewWindowLevel()` - Enable window/level adjustment mode

## How It Works

### Loading a DICOM File

1. User clicks "Open DICOM File" or presses Ctrl+O
2. `CMainFrame::OnDicomOpenFile()` shows file dialog
3. New document/view pair is created
4. `CDicomMateDoc::LoadDicomFile()` is called
5. `CDicomImage::LoadFromFile()` uses DCMTK to load the file
6. DCMTK parses DICOM headers and pixel data
7. View is notified to update (`UpdateAllViews()`)
8. `CDicomMateView::UpdateDIB()` creates a Windows DIB from pixel data
9. Image is displayed in the view

### Rendering Pipeline

```
DICOM File → DCMTK DicomImage → 8-bit Grayscale Pixel Data → 
Windows DIB → GDI StretchDIBits → Screen
```

1. **DCMTK Processing**: DicomImage handles decompression, photometric interpretation, window/level
2. **Conversion**: `getOutputData(8)` converts to 8-bit grayscale
3. **DIB Creation**: Pixel data copied to Windows BITMAPINFO structure with grayscale palette
4. **Rendering**: GDI `StretchDIBits` draws with zoom/pan transformations

### Window/Level Algorithm

Window/Level adjusts the mapping of pixel values to display brightness:
- **Window Center**: Middle gray value
- **Window Width**: Range of values displayed

DCMTK applies this transformation before returning pixel data:
```cpp
dicomImage->setWindow(windowCenter, windowWidth);
const void* pixels = dicomImage->getOutputData(8);
```

## Features Implemented

### ✅ Core Functionality
- [x] Load DICOM files using DCMTK
- [x] Display grayscale medical images
- [x] MDI tabbed interface (Chrome-like tabs)
- [x] Each tab contains one viewer
- [x] Tab close functionality

### ✅ Viewing Tools
- [x] Zoom In/Out (toolbar buttons, mouse wheel)
- [x] Zoom to Fit
- [x] Pan (click and drag)
- [x] Window/Level adjustment (click and drag)

### ✅ UI Features
- [x] File open dialog with DICOM file filters
- [x] Real-time image updates
- [x] Smooth zoom and pan
- [x] Status display when no image loaded

## Usage

### Opening a DICOM File
1. Launch DicomMate
2. Click "Open DICOM File" button or press Ctrl+O
3. Select a .dcm file
4. Image opens in a new tab

### Viewing Tools
- **Zoom In**: Click toolbar button or scroll mouse wheel up
- **Zoom Out**: Click toolbar button or scroll mouse wheel down
- **Zoom to Fit**: Click toolbar button
- **Pan**: Click "Pan" button, then click and drag image
- **Window/Level**: Click "Window/Level" button, then drag:
  - Horizontal drag: Adjust center (brightness)
  - Vertical drag: Adjust width (contrast)

### Multiple Images
- Open multiple DICOM files
- Each file opens in a new tab
- Switch between tabs by clicking tab headers
- Close tabs by clicking the X button on each tab

## Supported DICOM Features

### Pixel Data
- ✅ Grayscale images
- ✅ Monochrome1 and Monochrome2
- ✅ 8, 10, 12, 16-bit pixel data
- ✅ Compressed transfer syntaxes (JPEG, RLE, etc.)

### Metadata
- ✅ Patient Name
- ✅ Study Description
- ✅ Series Description
- ✅ Modality

## Building the Project

### Visual Studio 2022
1. Open `DicomMate.sln`
2. Ensure vcpkg integration is active
3. Select x64 platform (Debug or Release)
4. Build → Build Solution (Ctrl+Shift+B)
5. Run → Start Debugging (F5)

### Build Configurations
- **Debug|x64**: Development build with debug symbols
- **Release|x64**: Optimized production build

## Testing

### Test with Sample DICOM Files
You can download sample DICOM files from:
- https://www.rubomedical.com/dicom_files/
- https://barre.dev/medical/samples/

### Basic Test Cases
1. **Load Single Image**: Open a CT or MRI DICOM file
2. **Zoom Test**: Use mouse wheel and toolbar buttons
3. **Pan Test**: Click Pan button and drag image
4. **Window/Level Test**: Click W/L button and adjust
5. **Multiple Tabs**: Open several files and switch between tabs
6. **Close Tab**: Click X on tab to close

## Troubleshooting

### DCMTK Not Found
- Ensure vcpkg is installed and integrated
- Run `vcpkg integrate install` again
- Restart Visual Studio

### Image Not Displaying
- Check if DICOM file is valid
- Verify pixel data is present in file
- Check Output window for DCMTK error messages

### Build Errors
- Clean solution and rebuild
- Verify x64 platform is selected
- Check DCMTK libraries are installed for x64-windows

## Future Enhancements

### Planned Features
- [ ] Multi-frame/Cine loop playback
- [ ] 3D reconstruction (MPR, VRT)
- [ ] Measurement tools (distance, angle, ROI)
- [ ] DICOM tag viewer
- [ ] Export to image formats
- [ ] Annotations
- [ ] Print functionality
- [ ] PACS integration

## Code Style

Follow the coding conventions in `EnvironmentDevelop.md`:

### Commit Messages
```
[Component] Description
Example: [Viewer] Implement DICOM image rendering
```

### Code Comments
- Use comments for complex algorithms
- Document public APIs
- Keep inline comments minimal

## References

### DCMTK Documentation
- Official Site: https://dicom.offis.de/dcmtk
- DicomImage Class: https://support.dcmtk.org/docs/classDicomImage.html
- File I/O: https://support.dcmtk.org/docs/mod_dcmdata.html

### DICOM Standard
- Standard Home: https://www.dicomstandard.org/
- Part 5 (Data Structures): https://dicom.nema.org/medical/dicom/current/output/chtml/part05/

### vcpkg
- GitHub: https://github.com/microsoft/vcpkg
- Getting Started: https://github.com/microsoft/vcpkg#getting-started

## Contributors
- Development Team: DicomMate Project
- DCMTK Integration: OFFIS e.V.
- Package Management: Microsoft vcpkg

## License
This project uses DCMTK which is licensed under a modified BSD license.
See DCMTK documentation for license details.
