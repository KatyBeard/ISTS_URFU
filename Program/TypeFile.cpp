//---------------------------------------------------------------------------

#pragma hdrstop

#include "TypeFile.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

ClusterTypeFile FindTypeFile(char* dataBuffer, std::vector<UnicodeString> SelectedTypes) {

	for(int i = 0; i < SelectedTypes.size(); i++) {
		UnicodeString typeFile = SelectedTypes[i];
		// ‘ункци€ wcscmp () используетс€ дл€ сравнени€ двух широкосимвольных строк. ≈сли они равны - возвращаетс€ ноль.
		// ѕри этом если не выполн€етс€ перва€ часть услови€ (false), то втора€ часть услови€ уже не провер€етс€.
		if (wcscmp(typeFile.c_str(), L"PNG") == 0 && Contains(dataBuffer, PNG_SIGNS, 0, 8)) {
			return ClusterTypeFile::PNG;
		}

		if (wcscmp(typeFile.c_str(), L"JPEG") == 0 && Contains(dataBuffer, JPEG_SIGNS, 0, 4)) {
			return ClusterTypeFile::JPEG;
		}

		if (wcscmp(typeFile.c_str(), L"BMP") == 0 && Contains(dataBuffer, BMP_SIGNS, 0, 2)) {
			return ClusterTypeFile::BMP;
		}

		if (wcscmp(typeFile.c_str(), L"PDF") == 0 && Contains(dataBuffer, PDF_SIGNS, 0, 5)) {
			return ClusterTypeFile::PDF;
		}
	}

	return ClusterTypeFile::NULL_Type;
}

bool Contains(char* dataBuffer, const char* sign, int start, int len) {
	for (int i = start; i < len-start; i++) {
		if (dataBuffer[i] != sign[i]) {
			return false;
		}
	}

	return true;
}

char* GetTypeFileName(ClusterTypeFile clusterTypeFile) {
	switch (clusterTypeFile) {
	case ClusterTypeFile::PNG:  // »спользуем глобальные переменные, то есть глобальный оператор разрешени€ (::)
		return "PNG";
		break;
	case ClusterTypeFile::JPEG:
		return "JPEG";
		break;
	case ClusterTypeFile::BMP:
		return "BMP";
		break;
	case ClusterTypeFile::PDF:
		return "PDF";
		break;
	default:
		return NULL;
	}
}