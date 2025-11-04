// DicomImage.cpp : implementation of DICOM image loading
//

#include "pch.h"
#include "DicomImage.h"
#include "framework.h"

// DCMTK includes
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimgle/dcmimage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDicomImage::CDicomImage()
	: m_pDicomImage(nullptr)
	, m_pDicomFile(nullptr)
	, m_windowCenter(0.0)
	, m_windowWidth(0.0)
	, m_width(0)
	, m_height(0)
	, m_frameCount(0)
{
}

CDicomImage::~CDicomImage()
{
	if (m_pDicomImage)
	{
		delete static_cast<DicomImage*>(m_pDicomImage);
		m_pDicomImage = nullptr;
	}
	if (m_pDicomFile)
	{
		delete static_cast<DcmFileFormat*>(m_pDicomFile);
		m_pDicomFile = nullptr;
	}
}

BOOL CDicomImage::LoadFromFile(const CString& filePath)
{
	// Clean up existing data
	if (m_pDicomImage)
	{
		delete static_cast<DicomImage*>(m_pDicomImage);
		m_pDicomImage = nullptr;
	}
	if (m_pDicomFile)
	{
		delete static_cast<DcmFileFormat*>(m_pDicomFile);
		m_pDicomFile = nullptr;
	}

	// Convert CString to std::string
	CT2A asciiPath(filePath);
	std::string path(asciiPath);

	// Load DICOM file
	DcmFileFormat* fileFormat = new DcmFileFormat();
	OFCondition status = fileFormat->loadFile(path.c_str());
	
	if (status.bad())
	{
		delete fileFormat;
		return FALSE;
	}

	m_pDicomFile = fileFormat;

	// Create DicomImage object
	DicomImage* dicomImage = new DicomImage(path.c_str());
	
	if (dicomImage == nullptr || dicomImage->getStatus() != EIS_Normal)
	{
		if (dicomImage)
			delete dicomImage;
		delete fileFormat;
		return FALSE;
	}

	m_pDicomImage = dicomImage;
	m_pDicomFile = fileFormat;
	m_filePath = filePath;

	// Get image dimensions
	m_width = static_cast<int>(dicomImage->getWidth());
	m_height = static_cast<int>(dicomImage->getHeight());
	m_frameCount = static_cast<int>(dicomImage->getFrameCount());

	// Get default window center/width
	m_windowCenter = dicomImage->getWindowCenter();
	m_windowWidth = dicomImage->getWindowWidth();

	return TRUE;
}

int CDicomImage::GetWidth() const
{
	return m_width;
}

int CDicomImage::GetHeight() const
{
	return m_height;
}

int CDicomImage::GetFrameCount() const
{
	return m_frameCount;
}

BOOL CDicomImage::IsValid() const
{
	return (m_pDicomImage != nullptr);
}

const void* CDicomImage::GetPixelData(int frame) const
{
	if (!m_pDicomImage)
		return nullptr;

	DicomImage* dicomImage = static_cast<DicomImage*>(m_pDicomImage);
	
	// Get pixel data for the specified frame
	const void* pixelData = dicomImage->getOutputData(8 /* bits per sample */, frame);
	
	return pixelData;
}

void CDicomImage::SetWindowCenter(double center)
{
	if (!m_pDicomImage)
		return;

	m_windowCenter = center;
	DicomImage* dicomImage = static_cast<DicomImage*>(m_pDicomImage);
	dicomImage->setWindow(m_windowCenter, m_windowWidth);
}

void CDicomImage::SetWindowWidth(double width)
{
	if (!m_pDicomImage)
		return;

	m_windowWidth = width;
	DicomImage* dicomImage = static_cast<DicomImage*>(m_pDicomImage);
	dicomImage->setWindow(m_windowCenter, m_windowWidth);
}

CString CDicomImage::GetPatientName() const
{
	if (!m_pDicomFile)
		return _T("");

	DcmFileFormat* fileFormat = static_cast<DcmFileFormat*>(m_pDicomFile);
	DcmDataset* dataset = fileFormat->getDataset();
	
	if (!dataset)
		return _T("");

	OFString patientName;
	if (dataset->findAndGetOFString(DCM_PatientName, patientName).good())
	{
		CA2T unicodeName(patientName.c_str());
		return CString(unicodeName);
	}

	return _T("");
}

CString CDicomImage::GetStudyDescription() const
{
	if (!m_pDicomFile)
		return _T("");

	DcmFileFormat* fileFormat = static_cast<DcmFileFormat*>(m_pDicomFile);
	DcmDataset* dataset = fileFormat->getDataset();
	
	if (!dataset)
		return _T("");

	OFString studyDesc;
	if (dataset->findAndGetOFString(DCM_StudyDescription, studyDesc).good())
	{
		CA2T unicodeDesc(studyDesc.c_str());
		return CString(unicodeDesc);
	}

	return _T("");
}

CString CDicomImage::GetSeriesDescription() const
{
	if (!m_pDicomFile)
		return _T("");

	DcmFileFormat* fileFormat = static_cast<DcmFileFormat*>(m_pDicomFile);
	DcmDataset* dataset = fileFormat->getDataset();
	
	if (!dataset)
		return _T("");

	OFString seriesDesc;
	if (dataset->findAndGetOFString(DCM_SeriesDescription, seriesDesc).good())
	{
		CA2T unicodeDesc(seriesDesc.c_str());
		return CString(unicodeDesc);
	}

	return _T("");
}

CString CDicomImage::GetModality() const
{
	if (!m_pDicomFile)
		return _T("");

	DcmFileFormat* fileFormat = static_cast<DcmFileFormat*>(m_pDicomFile);
	DcmDataset* dataset = fileFormat->getDataset();
	
	if (!dataset)
		return _T("");

	OFString modality;
	if (dataset->findAndGetOFString(DCM_Modality, modality).good())
	{
		CA2T unicodeModality(modality.c_str());
		return CString(unicodeModality);
	}

	return _T("");
}
