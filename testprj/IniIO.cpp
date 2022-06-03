#include"IniIo.h"

Infile* readProfile(Infile* ret) {
	WCHAR inFileBuffer[64 * 128] = { 0 };
	GetPrivateProfileSectionNames(inFileBuffer, 64 * 128, L"..\\etc\\docList.ini");
	UINT infileCount = 0;
	UINT charNum = 0;
	for (UINT i = 0; i < 64 * 128; ) {
		ret->nameArray[infileCount][charNum] = inFileBuffer[i];
		if (inFileBuffer[i] == '\0' && inFileBuffer[i + 1] == '\0' && inFileBuffer[i + 2] == '\0') {
			goto out;
		}
		if (inFileBuffer[i] == '\0') {
			infileCount++;
			charNum = 0;
			i++;
			continue;
		}
		charNum++;
		i++;
	}
out:
	return ret;
}

/*
* write ini file
*/
VOID WriteProfile(LPCWSTR section, LPCWSTR item, LPCWSTR info, LPCWSTR relectivePath) {
	WritePrivateProfileStringW(section, item, info, relectivePath);
}