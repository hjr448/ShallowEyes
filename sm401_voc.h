// Vocs.h: interface for the CVocs class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _SM401_VOC_H_
#define _SM401_VOC_H_

#define MAX_NUM_VOCID			32

/*
enum VOCRESULT
{
	VR_OK = 0,
	VR_INVALIDFILENAME					= - 1,
	VR_BADWAVEFILE						= - 2,
	VR_INVALIDPARAM						= - 3,
	VR_INVALIDVOCID					= - 4,
	VR_NOTSUPPORTEDYET					= - 5,
	VR_WAVEMUSTBEMONO					= - 6,
	VR_WAVEMUSTBEWAVEFORMATPCM			= - 7,
	VR_WAVESMUSTHAVESAMEBITRESOLUTION	= - 8,
	VR_WAVESMUSTHAVESAMEFREQUENCY		= - 9,
	VR_WAVESMUSTHAVESAMEBITRATE			= -10,
	VR_WAVESMUSTHAVESAMEBLOCKALIGNMENT	= -11,
	VR_OFFSETOUTOFDATARANGE				= -12,
	VR_FILEERROR						= -13,
	VR_OUTOFMEMORY						= -14,
	VR_INVALIDSPEAKERPOS				= -15,
	VR_INVALIDWAVEFILETYPE				= -16,
	VR_NOTWAVEFORMATEXTENSIBLEFORMAT	= -17
};
*/
typedef struct
{
	unsigned long	sample_r;
	char			*pData;
	unsigned long	ulDataSize;
	FILE			*pFile;
	unsigned long	ulDataOffset;
} VOCFILEINFO, *LPVOCFILEINFO;

typedef int (__cdecl *PFNALGETENUMVALUE)( const char *szEnumName );
typedef int	VOCID;

class CVoc  
{
public:
	CVoc();
	virtual ~CVoc();
	int LoadVocFile(const char *szFilename, VOCID *VocID);
	int LoadVocBuffer(unsigned char *szBuffer, VOCID *VocID);
	int GetVocData(VOCID VocID, void **ppAudioData);
	int GetVocSize(VOCID VocID, unsigned long *pulDataSize);
	int GetVocFrequency(VOCID VocID, unsigned long *pulFrequency);
	int GetVocALBufferFormat(VOCID VocID, PFNALGETENUMVALUE pfnGetEnumValue, unsigned long *pulFormat);
	//char *GetErrorString(WAVERESULT wr, char *szErrorString, unsigned long nSizeOfErrorString);
private:
	int ParseFile(const char *szFilename, LPVOCFILEINFO pVocInfo);
	LPVOCFILEINFO	m_VocIDs[MAX_NUM_VOCID];
};

#endif 
