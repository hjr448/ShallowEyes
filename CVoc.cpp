#include <string.h>
#include <stdio.h>
#include "CVoc.h"


typedef struct
{
	char			s1[0x13];
	char 			a1;
	unsigned short	doffs;
	unsigned char	s2[4];
	char 			b1[4];
	unsigned short  time_const;
	char pack;
	char mode;
	char b2;
	unsigned char	size;
	unsigned char	siz;
	unsigned char 	si;
	unsigned char sample_rate;
	unsigned char compression;	
} VOCFILEHEADER;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVoc::CVoc()
{
	memset(&m_VocIDs, 0, sizeof(m_VocIDs));
}

CVoc::~CVoc()
{
	long lLoop;

	for (lLoop = 0; lLoop < MAX_NUM_VOCID; lLoop++)
	{
		if (m_VocIDs[lLoop])
		{
			if (m_VocIDs[lLoop]->pData)
				delete m_VocIDs[lLoop]->pData;

			if (m_VocIDs[lLoop]->pFile)
				fclose(m_VocIDs[lLoop]->pFile);

			delete m_VocIDs[lLoop];
			m_VocIDs[lLoop] = 0;
		}
	}
}

int CVoc::LoadVocFile(const char *szFilename, VOCID *pVocID)
{
	int wr = -14;
	LPVOCFILEINFO pVocInfo;

	pVocInfo = new VOCFILEINFO;
	if (pVocInfo)
	{
		wr = ParseFile(szFilename, pVocInfo);
		if (wr==0)
		{
			// Allocate memory for sample data
			pVocInfo->pData = new char[pVocInfo->ulDataSize];
			if (pVocInfo->pData)
			{
				// Seek to start of audio data
				fseek(pVocInfo->pFile, pVocInfo->ulDataOffset, SEEK_SET);

				// Read Sample Data
				if (fread(pVocInfo->pData, 1, pVocInfo->ulDataSize, pVocInfo->pFile) == pVocInfo->ulDataSize)
				{
					long lLoop = 0;
					for (lLoop = 0; lLoop < MAX_NUM_VOCID; lLoop++)
					{
						if (!m_VocIDs[lLoop])
						{
							m_VocIDs[lLoop] = pVocInfo;
							*pVocID = lLoop;
							//wr = WR_OK;
							break;
						}
					}

					if (lLoop == MAX_NUM_VOCID)
					{
						delete pVocInfo->pData;
						//wr = WR_OUTOFMEMORY;
					}
				}
				else
				{
					delete pVocInfo->pData;
					//wr = WR_BADVOCFILE;
				}
			}
			else
			{
				//wr = WR_OUTOFMEMORY;
			}

			fclose(pVocInfo->pFile);
			pVocInfo->pFile = 0;
		}

		//if (wr != WR_OK)
			//delete pVocInfo;
	}

	return wr;
}


int CVoc::ParseFile(const char *szFilename, LPVOCFILEINFO pVocInfo)
{
	VOCFILEHEADER	vocFileHeader;
	int		wr = 1;

	if (!szFilename || !pVocInfo)
		return -1;

	memset(pVocInfo, 0, sizeof(VOCFILEINFO));

	// Open the wave file for reading
	pVocInfo->pFile=fopen(szFilename, "rb");
	if (pVocInfo->pFile)
	{
		// Read Voc file header
		fread(&vocFileHeader, 1, sizeof(VOCFILEHEADER), pVocInfo->pFile);
		if (!_strnicmp(vocFileHeader.s1, "Creative Voice File", 13) )
		{
			pVocInfo->ulDataSize = vocFileHeader.size+256*vocFileHeader.siz-3;
			pVocInfo->ulDataOffset = 0x28;
			pVocInfo->sample_r=256000000/(65536-vocFileHeader.time_const);
			wr = 0;
		}
	}

	return wr;
}


int CVoc::GetVocData(VOCID VocID, void **lplpAudioData)
{
	if (!lplpAudioData)
		return -1;

	*lplpAudioData = m_VocIDs[VocID]->pData;

	return 0;
}

int CVoc::GetVocSize(VOCID VocID, unsigned long *size)
{
	if (!size)
		return -1;

	*size = m_VocIDs[VocID]->ulDataSize;

	return 0;
}


int CVoc::GetVocFrequency(VOCID VocID, unsigned long *pulFrequency)
{
	int wr = 0;

	{
		if (pulFrequency)
			*pulFrequency = m_VocIDs[VocID]->sample_r;
		else
			wr = -1;
	}

	return wr;
}


int CVoc::GetVocALBufferFormat(VOCID VocID, PFNALGETENUMVALUE pfnGetEnumValue, unsigned long *pulFormat)
{
	int wr = 0;

	{
		if (pfnGetEnumValue && pulFormat)
		{
			*pulFormat = pfnGetEnumValue("AL_FORMAT_MONO8");
		}else{
			wr = -1;
		}
	}

	return wr;
}
/**/
/*

char *CVoc::GetErrorString(VOCRESULT wr, char *szErrorString, unsigned long nSizeOfErrorString)
{	
	switch (wr)
	{
		case WR_OK:
			strncpy_s(szErrorString, nSizeOfErrorString, "Success\n", nSizeOfErrorString-1);
			break;

		case WR_INVALIDFILENAME:
			strncpy_s(szErrorString, nSizeOfErrorString, "Invalid file name or file does not exist\n", nSizeOfErrorString-1);
			break;

		case WR_BADVOCFILE:
			strncpy_s(szErrorString, nSizeOfErrorString, "Invalid Voc file\n", nSizeOfErrorString-1);
			break;

		case WR_INVALIDPARAM:
			strncpy_s(szErrorString, nSizeOfErrorString, "Invalid parameter passed to function\n", nSizeOfErrorString-1);
			break;

		case WR_FILEERROR:
			strncpy_s(szErrorString, nSizeOfErrorString, "File I/O error\n", nSizeOfErrorString-1);
			break;

		case WR_INVALIDVOCID:
			strncpy_s(szErrorString, nSizeOfErrorString, "Invalid VOCID\n", nSizeOfErrorString-1);
			break;

		case WR_NOTSUPPORTEDYET:
			strncpy_s(szErrorString, nSizeOfErrorString, "Function not supported yet\n", nSizeOfErrorString-1);
			break;

		case WR_VOCMUSTBEMONO:
			strncpy_s(szErrorString, nSizeOfErrorString, "Input wave files must be mono\n", nSizeOfErrorString-1);
			break;

		case WR_VOCMUSTBEVOCFORMATPCM:
			strncpy_s(szErrorString, nSizeOfErrorString, "Input wave files must be in Voc Format PCM\n", nSizeOfErrorString-1);
			break;
		
		case WR_VOCSMUSTHAVESAMEBITRESOLUTION:
			strncpy_s(szErrorString, nSizeOfErrorString, "Input wave files must have the same Bit Resolution\n", nSizeOfErrorString-1);
			break;

		case WR_VOCSMUSTHAVESAMEFREQUENCY:
			strncpy_s(szErrorString, nSizeOfErrorString, "Input wave files must have the same Frequency\n", nSizeOfErrorString-1);
			break;

		case WR_VOCSMUSTHAVESAMEBITRATE:
			strncpy_s(szErrorString, nSizeOfErrorString, "Input wave files must have the same Bit Rate\n", nSizeOfErrorString-1);
			break;

		case WR_VOCSMUSTHAVESAMEBLOCKALIGNMENT:
			strncpy_s(szErrorString, nSizeOfErrorString, "Input wave files must have the same Block Alignment\n", nSizeOfErrorString-1);
			break;

		case WR_OFFSETOUTOFDATARANGE:
			strncpy_s(szErrorString, nSizeOfErrorString, "Voc files Offset is not within audio data\n", nSizeOfErrorString-1);
			break;

		case WR_INVALIDSPEAKERPOS:
			strncpy_s(szErrorString, nSizeOfErrorString, "Invalid Speaker Destinations\n", nSizeOfErrorString-1);
			break;

		case WR_OUTOFMEMORY:
			strncpy_s(szErrorString, nSizeOfErrorString, "Out of memory\n", nSizeOfErrorString-1);
			break;

		case WR_INVALIDVOCFILETYPE:
			strncpy_s(szErrorString, nSizeOfErrorString, "Invalid Voc File Type\n", nSizeOfErrorString-1);
			break;

		case WR_NOTVOCFORMATEXTENSIBLEFORMAT:
			strncpy_s(szErrorString, nSizeOfErrorString, "Voc file is not in VOCFORMATEXTENSIBLE format\n", nSizeOfErrorString-1);
			break;

		default:
			strncpy_s(szErrorString, nSizeOfErrorString, "Undefined error\n", nSizeOfErrorString-1);
	}
	szErrorString[nSizeOfErrorString-1] = '\0';
	return szErrorString;
}
*/