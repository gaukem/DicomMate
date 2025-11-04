# DicomMate - DICOM Medical Image Viewer

## Overview
DicomMate is a Windows MFC application for viewing DICOM medical images. It features a tabbed interface similar to Chrome, with each tab displaying one DICOM image viewer.

## Features
- **DCMTK Integration**: Uses DCMTK library for robust DICOM file reading
- **Tabbed Interface**: Multiple images in separate tabs with Chrome-like tab management
- **Image Viewing Tools**:
  - Zoom In/Out (mouse wheel or toolbar)
  - Zoom to Fit
  - Pan (click and drag)
  - Window/Level adjustment (brightness/contrast)
- **Supported Formats**: All DICOM formats supported by DCMTK including compressed images
- **Metadata Display**: Patient name, study description, modality, etc.

## Setup

### Prerequisites
- Visual Studio 2022 or later
- Windows 10/11
- vcpkg package manager

### Installation

1. **Install vcpkg** (if not already installed):
   ```powershell
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   git checkout 7dc284a6ad0b9cb8ccd618c8dc78c360fbd93b83
   .\bootstrap-vcpkg.bat
   .\vcpkg.exe integrate install
   ```

2. **Install DCMTK and dependencies**:
   ```powershell
   .\vcpkg.exe install dcmtk:x64-windows-static dcmtk:x64-windows
   .\vcpkg.exe install openjpeg:x64-windows-static openjpeg:x64-windows
   ```

3. **Build DicomMate**:
   - Open `DicomMate.sln` in Visual Studio
   - Select x64 platform (Debug or Release)
   - Build → Build Solution (Ctrl+Shift+B)
   - Run → Start Debugging (F5)

## Usage

### Opening DICOM Files
1. Launch DicomMate
2. Click "Open DICOM File" button or press Ctrl+O
3. Select a .dcm file
4. Image opens in a new tab

### Viewing Controls
- **Zoom**: Mouse wheel or toolbar buttons
- **Pan**: Click "Pan" button, then click and drag
- **Window/Level**: Click "Window/Level" button, then drag to adjust brightness/contrast
- **Multiple Tabs**: Open multiple files, each in its own tab
- **Close Tab**: Click X on tab header

## Documentation
See detailed implementation guide in `DICOM_VIEWER_IMPLEMENTATION.md`

## Development
See development environment setup in `EnvironmentDevelop.md`
