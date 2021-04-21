#pragma once
#include <string>
#include <stdint.h>
#include <memory>
#include <fstream>
#include <string.h>
#include <assert.h>
namespace JINFENG{

class ByteArray{
public:
	typedef std::shared_ptr<ByteArray> ptr;

	struct Node{
		Node(size_t s):size(s), ptr(new char[s]),next(nullptr)
		{ }
		Node():size(0), ptr(nullptr), next(nullptr)
		{ }
		~Node() {delete[] ptr;}
		char* ptr;
		size_t size;
		Node* next;
	};

	ByteArray(size_t base_size=4096);
	~ByteArray();

	//写入固定程度
	void writeFuint8(uint8_t value);
	void writeFint8(int8_t value);
	void writeFuint16(uint16_t value);
	void writeFint16(int16_t value);
	void writeFuint32(uint32_t value);
	void writeFint32(int32_t value);
	void writeFuint64(uint64_t value);
	void writeFint64(int64_t value);

	//可压缩写入
	void writeUint16(uint16_t value);
	void writeInt16(int16_t value);
	void writeUint32(uint32_t value);
	void writeInt32(int32_t value);
	void writeUint64(uint64_t value);
	void writeInt64(int64_t value);

	void writeFloat(float value);

	void writeDouble(double value);

	void writeStringF16(const std::string& value);

	void writeStringF32(const std::string& value);

	void writeStringF64(const std::string& value);

	void writeStringVint(const std::string& value);

	void writeStringWithoutLength(const std::string& value);


	//读入数据
	uint8_t readFuint8();
	int8_t readFint8();
	uint16_t readFuint16();
	int16_t readFint16();
	uint32_t readFuint32();
	int32_t readFint32();
	uint64_t readFuint64();
	int64_t readFint64();

	//读入压缩数据
	uint16_t readUint16();
	int16_t readInt16();
	uint32_t readUint32();
	int32_t readInt32();
	uint64_t readUint64();
	int64_t readInt64();


	float readFloat();
	double readDouble();

	std::string readStringF16();
	std::string readStringF32();
	std::string readStringF64();
	std::string readStringVint();
	
	void clear();

	void write(const void* buf, size_t size);

	void read(void* buf, size_t size);

	void read(void* buf, size_t size, size_t position);

	size_t getPosition() const {return m_position;}

	void setPosition(size_t v);

	bool writeToFile(const std::string& name) const;

	bool readFromFile(const std::string& name);

	size_t getBaseSize() const {return m_baseSize;}

	size_t getReadSize() const {return m_size-m_position;}

	std::string toString();

	size_t getSize() const{return m_size;}
private:
	void addCapacity(size_t size);

	size_t getCapacity(){return m_capacity-m_position;}
private:

	size_t m_baseSize;
	size_t m_position;
	size_t m_size;
	size_t m_capacity;
	Node* m_root;
	Node* m_cur;
	bool m_isFull; //是否刚好用完所有空间
};

};
