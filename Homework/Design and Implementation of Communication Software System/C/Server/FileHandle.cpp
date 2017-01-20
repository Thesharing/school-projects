#include "stdAfx.h"
#include "TFTP_server.h"
using namespace std;

bool isFileExist(string filename) {
	HANDLE  hFindFile;
	WIN32_FIND_DATA  FileData;

	hFindFile = FindFirstFile("resource\\*", &FileData);

	if (hFindFile == INVALID_HANDLE_VALUE) {
		return false;
	}
	else {
		FindNextFile(hFindFile, &FileData);
		char br[2] = { '\n','\0' };
		while (FindNextFile(hFindFile, &FileData)) {

			if (strstr(FileData.cFileName, filename.data()) == NULL)
				continue;
			else {
				FindClose(hFindFile); //说明文件存在
				return true;
			}
		}
		FindClose(hFindFile);
		return false;
	}
}

bool openFile(CStdioFile * pfile, REQPacket packet) {
	int flag;
	string path = "resource\\";
	string filePath = path + packet.fileName;

	if (packet.opCode == RRQ) {
		// 读文件
		if (packet.mode == "octet" || packet.mode == "OCTET") {
			// 二进制
			flag = pfile->Open(filePath.data(), CStdioFile::modeRead | CStdioFile::typeBinary);
			if (flag > 0)
				return true;
			else
				return false;
		}
		else {
			// 文本
			flag = pfile->Open(filePath.data(), CStdioFile::modeRead | CStdioFile::typeText);
			if (flag > 0)
				return true;
			else
				return false;
		}
	}
	else {
		// 写文件
		if (packet.mode == "octet" || packet.mode == "OCTET") {
			// 二进制
			flag = pfile->Open(filePath.data(), CStdioFile::modeCreate | CStdioFile::modeWrite |
				CStdioFile::typeBinary);
			if (flag > 0)
				return true;
			else
				return false;
		}
		else {
			// 文本
			flag = pfile->Open(filePath.data(), CStdioFile::modeCreate | CStdioFile::modeWrite |
				CStdioFile::typeText);
			if (flag > 0)
				return true;
			else
				return false;
		}
	}
}

void closeFile(CStdioFile * pfile) {
	pfile->Close();
}

//buf是输出参数，大小为512字节,返回值是实际读到的字符数
int readFile(CStdioFile * pfile, char * buf) {
	int len = 0;
	clear(buf, DATA_LEN);
	len = pfile->Read(buf, DATA_LEN);
	return len;
}

//buf是输入参数
void writeFile(CStdioFile * pfile, char * buf) {
	pfile->Write(buf, DATA_LEN);
}



