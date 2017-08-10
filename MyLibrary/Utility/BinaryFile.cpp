#include "BinaryFile.h"

#include <fstream>
#include <assert.h>

BinaryFile BinaryFile::LoadFile(const wchar_t * fileName)
{
	BinaryFile bin;

	std::ifstream ifs;

	// �t�@�C���I�[�v��
	ifs.open(fileName, std::ios::in | std::ios::binary);

	// �ǂݍ��ݎ��s���A�����I��
	assert(ifs);

	// �t�@�C���T�C�Y���擾
	ifs.seekg(0, std::fstream::end);
	std::streamoff eofPos = ifs.tellg();
	ifs.clear();
	ifs.seekg(0, std::fstream::beg);
	std::streamoff begPos = ifs.tellg();
	bin.m_Size = eofPos - begPos;

	// �ǂݍ��ނ��߂̃��������m��
	bin.m_Data.reset(new char[bin.m_Size]);

	// �t�@�C���擪����o�b�t�@�փR�s�[ 
	ifs.read(bin.m_Data.get(), bin.m_Size);

	// �t�@�C���N���[�Y
	ifs.close();

	return std::move(bin);
}

BinaryFile::BinaryFile()
{
	m_Size = 0;
}

BinaryFile::BinaryFile(BinaryFile && in)
{
	m_Data = std::move(in.m_Data);
	m_Size = in.m_Size;
}
