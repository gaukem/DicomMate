#pragma once

// DicomImage.h : DICOM image loading and processing using DCMTK
//

#include <string>
#include <memory>

// Forward declarations for DCMTK
class DicomImage;

class CDicomImage
{
public:
	CDicomImage();
	~CDicomImage();

	// Load DICOM file
	BOOL LoadFromFile(const CString& filePath);
	
	// Image information
	int GetWidth() const;
	int GetHeight() const;
	int GetFrameCount() const;
	BOOL IsValid() const;
	
	// Get pixel data for rendering
	const void* GetPixelData(int frame = 0) const;
	
	// Window/Level operations
	void SetWindowCenter(double center);
	void SetWindowWidth(double width);
	double GetWindowCenter() const { return m_windowCenter; }
	double GetWindowWidth() const { return m_windowWidth; }
	
	// Get patient/study information
	CString GetPatientName() const;
	CString GetStudyDescription() const;
	CString GetSeriesDescription() const;
	CString GetModality() const;
	
	// Get raw DICOM object for tag display
	void* GetDicomImage() const { return m_pDicomImage; }

private:
	void* m_pDicomImage;  // DicomImage pointer
	void* m_pDicomFile;   // DcmFileFormat pointer
	
	double m_windowCenter;
	double m_windowWidth;
	
	int m_width;
	int m_height;
	int m_frameCount;
	
	CString m_filePath;
};
