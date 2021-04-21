#include "bytearray.h"
#include <iostream>
#include <vector>

void test()
{
	JINFENG::ByteArray::ptr ba(new JINFENG::ByteArray(1));
	size_t lastPosition = 0;
#define xx(type, len, write_func, read_func) \
	std::vector<type> vec##type; \
	for(int i=0; i<len; ++i) \
	{\
		vec##type.push_back(i); \
	}\
	std::cout<<#type<<": "<<"write: "; \
	for(auto& item:vec##type){ \
		std::cout<<int(item)<<" "; \
		ba->write_func(item); \
	}\
	std::cout<<std::endl; \
	ba->setPosition(lastPosition); \
	std::cout<<#type<<": "<<"read : "; \
	for(int i=0; i<len; ++i) \
		std::cout<<(int)ba->read_func()<<" "; \
	std::cout<<std::endl; \
	ba->clear();

	//测试固定长度数字
	xx(uint8_t, 10, writeFuint8, readFuint8);
	xx(uint16_t, 10, writeFuint16, readFuint16);
	xx(uint32_t, 10, writeFuint32, readFuint32);
	xx(uint64_t, 10, writeFuint64, readFuint64);
	
	xx(int8_t, 10, writeFint8, readFint8);
	xx(int16_t, 10, writeFint16, readFint16);
	xx(int32_t, 10, writeFint32, readFint32);
	xx(int64_t, 10, writeFint64, readFint64);

	xx(float, 10, writeFloat, readFloat);
	xx(double, 10, writeDouble, readDouble);
#undef xx

	//测试压缩数字
	uint16_t val = 1;
	ba->writeUint16(val);
	ba->setPosition(0);
	std::cout<<ba->readUint16()<<std::endl;
	ba->clear();

        //测试字符串
        std::string str = "hello, jinfeng.";
        ba->writeStringF64(str);
        ba->setPosition(0);
        std::cout<<ba->toString()<<std::endl;
        ba->setPosition(0);
        std::cout<<ba->readStringF64()<<std::endl;
	ba->setPosition(0);

	//测试写入、从文件读出
	ba->writeToFile("bytes");
	ba->readFromFile("bytes");
	ba->setPosition(0);
        std::cout<<ba->toString()<<std::endl;
}



int main()
{
	test();
}
