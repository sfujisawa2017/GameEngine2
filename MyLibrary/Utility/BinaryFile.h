/// <summary>
/// �o�C�i���t�@�C���������N���X
/// </summary>
#pragma once

#include <memory>

class BinaryFile
{
public:
	// �t�@�C�������w�肵�ă��[�h
	static BinaryFile LoadFile(const wchar_t* fileName);

	BinaryFile();
	// ���[�u�R���X�g���N�^
	BinaryFile(BinaryFile&& in);

	// �A�N�Z�b�T
	char* GetData() { return m_Data.get(); }
	unsigned int GetSize() { return m_Size; }

protected:
	// �f�[�^
	std::unique_ptr<char[]> m_Data;
	// �T�C�Y
	unsigned int m_Size;
};