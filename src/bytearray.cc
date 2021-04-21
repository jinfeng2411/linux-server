#include "bytearray.h"
#include <iostream>
namespace JINFENG{

ByteArray::ByteArray(size_t base_size)
	: m_baseSize(base_size), 
	m_position(0), 
	m_capacity(base_size), 
	m_size(0), 
	m_root(new Node(base_size)), 
	m_cur(m_root),
        m_isFull(false)
	{ 
		assert(m_baseSize>0);
	}

ByteArray::~ByteArray()
{
	Node* tmp = m_root;
	while(tmp!=nullptr)
	{
		m_root = m_root->next;
		delete tmp;
		tmp = m_root;
	}
}

void ByteArray::addCapacity(size_t size)
{
	//如果当前余量足够，直接返回
	if(getCapacity()>=size)
		return;
	int left = getCapacity();
	int count = (size-left)/m_baseSize; //需要新创建的 Node 数
	if((size-left)%m_baseSize>0)
		++count;

	Node* tail = m_cur; //指向最后一个 Node
	while(tail->next!=nullptr)
	{
		tail = tail->next;
	}
	while(count--)
	{
		Node* node = new Node(m_baseSize);
		tail->next = node;
		tail = tail->next;
		m_capacity += m_baseSize;
	}
	if(m_isFull)
		m_cur = m_cur->next;
	m_isFull = false;
}

void ByteArray::write(const void* buf, size_t size)
{
	if(size<=0)
		return;
	addCapacity(size);
	int bpos = 0;
	int npos = m_position%m_baseSize;
	int ncap = m_baseSize-npos;
	//std::cout<<getCapacity()<<std::endl;
	//std::cout<<bpos<<" : "<<npos<<" : "<<ncap<<std::endl;
	while(size>0)
	{
		if(ncap>=size)
		{
			memcpy(m_cur->ptr+npos, (const char*)buf+bpos, size);
			if(npos+size==m_baseSize)
				m_isFull = true;
			m_position+=size;
			size = 0;
		}
		else
		{
			memcpy(m_cur->ptr+npos, (const char*)buf+bpos, ncap);
			size -= ncap;
			bpos += ncap;
			m_position += ncap;
			ncap = m_baseSize;
			npos = 0;

			m_cur = m_cur->next;
		}
	}
	if(m_position>m_size)
		m_size=m_position;
}

void ByteArray::read(void* buf, size_t size)
{
	if(size<=0)
		return;
	if(getReadSize()<size)
		throw std::out_of_range("not enough len");
	int bpos = 0;
	int npos = m_position%m_baseSize;
	int ncap = m_baseSize-npos;
	//std::cout<<bpos<<" : "<<npos<<" : "<<ncap<<std::endl;
	while(size>0)
	{
		if(ncap>=size)
		{
			memcpy((char*)buf+bpos, m_cur->ptr+npos, size);
			m_position += size;
			if(npos+size==m_baseSize)
				m_cur = m_cur->next;
			size=0;
		}
		else
		{
			memcpy((char*)buf+bpos, m_cur->ptr+npos, ncap);
			m_position += ncap;
			bpos += ncap;
			npos = 0;
			ncap = m_baseSize;
			m_cur = m_cur->next;
			size -= ncap;
		}
	}
}	

void ByteArray::read(void* buf, size_t size, size_t position)
{
	size_t oldposition = getPosition();
	setPosition(position);
	read(buf, size);
	setPosition(oldposition);
}

void ByteArray::writeFuint8(uint8_t value){
	write(&value, sizeof(value));
}

void ByteArray::writeFint8(int8_t value)
{
	write(&value, sizeof(value));
}

void ByteArray::writeFuint16(uint16_t value)
{
	write(&value, sizeof(value));
}

void ByteArray::writeFint16(int16_t value)
{
	write(&value, sizeof(value));
}

void ByteArray::writeFuint32(uint32_t value)
{
	write(&value, sizeof(value));
}

void ByteArray::writeFint32(int32_t value)
{
	write(&value, sizeof(value));
}

void ByteArray::writeFuint64(uint64_t value)
{
	write(&value, sizeof(value));
}

void ByteArray::writeFint64(int64_t value)
{
	write(&value, sizeof(value));
}


#define xx(type, value) \
	if(value<0) \
		return ((type)(-value))*2-1; \
	else \
		return ((type)value)*2;

static uint16_t EncodeZigzag16(const int16_t& value)
{
	xx(uint16_t, value);
}

static int16_t DecodeZigzag16(const uint16_t& value)
{
	return (value>>1)^-(value&1);
}

static uint32_t EncodeZigzag32(const int32_t& value)
{
	xx(uint32_t, value);
}

static int32_t DecodeZigzag32(const uint32_t& value)
{
	return (value>>1)^-(value&1);
}

static uint64_t EncodeZigzag64(const int64_t& value)
{
	xx(uint64_t, value);
}

static int64_t DecodeZigzag64(const uint64_t& value)
{
	return (value>>1)^-(value&1);
}
#undef xx

void ByteArray::writeUint16(uint16_t value)
{
	uint8_t tmp[3];
	uint8_t i = 0;
	while(value>=0x80)
	{
		tmp[i] = (value&0x7f) | 0x80;
		++i;
		value = value>>7;
	}
	tmp[i] = value & 0x7f;
	write(tmp, i+1);
}

void ByteArray::writeInt16(int16_t value)
{
	writeUint16(EncodeZigzag16(value));
}

void ByteArray::writeUint32(uint32_t value)
{
	uint8_t tmp[5];
	uint8_t i=0;
	while(value>=0x80)
	{
		tmp[i] = (value&0x7f) | 0x80;
		++i;
		value = value>>7;
	}
	tmp[i] = value&0x7f;
	write(tmp, i+1);
}

void ByteArray::writeInt32(int32_t value)
{
	writeUint32(EncodeZigzag32(value));
}

void ByteArray::writeUint64(uint64_t value)
{
	uint8_t tmp[10];
	uint8_t i=0;
	while(value>0x80)
	{
		tmp[i]=(value&0x7f)|0x80;
		++i;
		value = value>>7;
	}
	tmp[i]= value&0x7f;
	write(tmp, i+1);
}

void ByteArray::writeFloat(float value)
{
	write(&value, sizeof(value));
}

void ByteArray::writeDouble(double value)
{
	write(&value, sizeof(value));
}

void ByteArray::writeStringF16(const std::string& value)
{
	writeFuint16(value.size());
	write(value.c_str(), value.size());
	//std::cout<<m_position<<std::endl;
}

void ByteArray::writeStringF32(const std::string& value)
{
	writeFuint32(value.size());
	write(value.c_str(), value.size());
}

void ByteArray::writeStringF64(const std::string& value)
{
	writeFuint64(value.size());
	write(value.c_str(), value.size());
}

void ByteArray::writeStringVint(const std::string& value)
{
	writeUint64(value.size());
	write(value.c_str(), value.size());
}

void ByteArray::writeStringWithoutLength(const std::string& value)
{
	write(value.c_str(), value.size());
}


//读入数据

#define xx(type) \
	type val; \
	read(&val, sizeof(val)); \
	return val;


uint8_t ByteArray::readFuint8()
{
	xx(uint8_t);
}

int8_t ByteArray::readFint8()
{
        xx(int8_t);
}	
uint16_t ByteArray::readFuint16()
{
        xx(uint16_t);
}
int16_t ByteArray::readFint16()
{
        xx(int16_t);
}
uint32_t ByteArray::readFuint32()
{
        xx(uint32_t);
}
int32_t ByteArray::readFint32()
{
        xx(int32_t);
}
uint64_t ByteArray::readFuint64()
{
        xx(uint64_t);
}
int64_t ByteArray::readFint64()
{
        xx(int64_t);
}
float ByteArray::readFloat()
{
        xx(float);
}
double ByteArray::readDouble()
{
        xx(double);
}
#undef xx

#define xx(type) \
	type value=0; \
        uint8_t tmp; \
        read(&tmp, 1); \
        uint8_t count=0; \
        while(tmp>=0x80) \
        { \
                value |= ((type)tmp)<<count*7; \
                read(&tmp, 1); \
        } \
        value |= ((type)tmp)<<count*7; \
        return value; \

uint16_t ByteArray::readUint16()
{
	xx(uint16_t);
}
int16_t ByteArray::readInt16()
{
	return DecodeZigzag16(readUint16());
}
uint32_t ByteArray::readUint32()
{
	xx(uint32_t);
}
int32_t ByteArray::readInt32()
{
	return DecodeZigzag32(readUint32());
}
uint64_t ByteArray::readUint64()
{
	xx(uint64_t);
}
int64_t ByteArray::readInt64()
{
	return DecodeZigzag64(readUint64());
}
#undef xx

std::string ByteArray::readStringF16()
{
	uint16_t size = readFuint16();
	//std::cout<<size<<std::endl;
	char *data = new char[size];
	read(data, size);
	return std::string(data);
}
std::string ByteArray::readStringF32()
{
	uint32_t size = readFuint32();
	char *data=new char[size];
	read(data, size);
	return std::string(data);
}
std::string ByteArray::readStringF64()
{
	uint64_t size = readFuint64();
	char *data=new char[size];
	read(data, size);
	return std::string(data);
}
std::string ByteArray::readStringVint()
{
	int64_t size = readInt64();
	char *data=new char[size];
	read(data, size);
	return std::string(data);
}

void ByteArray::clear()
{
	m_position = 0;
	m_size = 0;
	m_capacity = m_baseSize;
	Node* tmp = m_root->next;
	while(tmp!=nullptr)
	{
		m_cur = tmp;
		tmp = tmp->next;
		delete m_cur;
	}
	m_cur = m_root;
	m_root->next = nullptr;
	m_isFull = false;
}

void ByteArray::setPosition(size_t v)
{
	if(v>m_capacity)
		throw std::out_of_range("set position out of range");
	m_position = v;
	if(m_position>m_size)
		m_size=m_position;
	m_cur = m_root;
	while(v>=m_baseSize)
	{
		m_cur = m_cur->next;
		v -= m_baseSize;
	}
}

bool ByteArray::writeToFile(const std::string& name) const
{
	std::ofstream ofs;
	ofs.open(name, std::ios::trunc | std::ios::binary);
	if(!ofs)
	{
		assert(false);
		return false;
	}
	int64_t read_size = getReadSize();
	Node* cur = m_cur;
	size_t npos = m_position%m_baseSize;
	size_t ncap = m_baseSize-npos;
	while(read_size>0)
	{
		size_t len = read_size>ncap? ncap : read_size;
		ofs.write(cur->ptr+npos, len);
		cur = cur->next;
		ncap = m_baseSize;
		npos = 0;
		read_size -= len;
	}
	return true;
}

bool ByteArray::readFromFile(const std::string& name)
{
	std::ifstream ifs;
	ifs.open(name, std::ios::binary);
	if(!ifs){
		assert(false);
		return false;
	}
	char* buf = new char[m_baseSize];
	while(!ifs.eof())
	{
		ifs.read(buf, m_baseSize);
		write(buf, ifs.gcount());
	}
	return true;
}

std::string ByteArray::toString()
{
	std::string str;
	str.resize(getReadSize());
	if(str.empty())
		return str;
	read(&str[0], str.size(), m_position);
	return str;
}




};



