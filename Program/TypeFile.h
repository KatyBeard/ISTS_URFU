//---------------------------------------------------------------------------

#ifndef TypeFileH
#define TypeFileH
#include <vector>
#include <vcl.h>
//---------------------------------------------------------------------------
// Перечисление сигнатур расширений файлов
const char PNG_SIGNS[8] = {'\x89', 'P', 'N', 'G', '\x0D', '\x0A', '\x1A', '\x0A'}; //"\x89PNG\x0D\x0A\x1A\x0A";
const char JPEG_SIGNS[4] = {'\xFF', '\xD8', '\xFF', '\xDB'};// "\xFF\xD8\xFF\xDB";    // "\xFF\xD8\xFF\xDB"
const char BMP_SIGNS[2] = {'B', 'M'}; //"BM";
const char PDF_SIGNS[5] = {'%', 'P', 'D', 'F', '\x2D'}; // "%PDF\x2D"  // {'%', 'P', 'D', 'F', '\x2D', '1', '\x2E'};

enum class ClusterTypeFile {
	NULL_Type=0,
	PNG=1,
	JPEG=2,
	BMP=3,
	PDF=4,
};

ClusterTypeFile FindTypeFile(char* dataBuffer, std::vector<UnicodeString> SelectedTypes);
bool Contains(char* dataBuffer, const char* sign, int start, int len);
char* GetTypeFileName(ClusterTypeFile clusterTypeFile);

#endif
