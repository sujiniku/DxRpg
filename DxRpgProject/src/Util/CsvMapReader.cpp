#include "Util/CsvMapReader.h"
#include <fstream>
#include <cstring>

namespace Util {
	CsvMapReader::CsvMapReader()
		: size_(0), data_(NULL), mapSize_(0)
	{
		for (int i = 0; i < YBlock * XBlock; i++)
		{
			mapData_[i] = '\0';
		}
	}

	int CsvMapReader::getMapData(int x, int y)
	{
		return mapData_[y * XBlock + x];
	}

	void CsvMapReader::load(const char* filename)
	{
		mapSize_ = YBlock * XBlock;
		// �e�L�X�g���[�h�ŃI�[�v��
		std::ifstream ifs(filename);
		if (ifs.fail())
		{
			//exit(EXIT_FAILURE);    // Fatal Error
			throw("%s Fatal Error. Cannnot read file." __FILE__);
		}
		else
		{
			// �T�C�Y�̌v��
			ifs.seekg(0, std::ifstream::end);
			size_ = static_cast<int>(ifs.tellg());
			// �t�@�C���|�C���^�����߂�
			ifs.seekg(0, std::ifstream::beg);
			// NULL������t��
			data_ = new char[size_];
			data_[size_ - 1] = '\0';
			// ��C�ɓǂݍ���
			ifs.read(data_, size_);
		}
		// CSV�p�[�X
		const char* Delim = ",\n";
		int i = 0;
		for (char* token = strtok(data_, Delim);
			token != NULL && i < mapSize_;
			token = strtok(0, Delim))
		{
			// �]�v�ȃJ���}�����������̑΍�
			if (token != NULL)
			{
				// ���l�ɕϊ�����
				mapData_[i] = atoi(token);
				i++;
			}
		}
		delete[] data_;
		data_ = NULL;
	}
}