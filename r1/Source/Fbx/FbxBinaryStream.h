/**
 *	@file	FbxBinaryStream.h
 *	@date	2017 / 10 / 05
 *	@author	Katsumi Takei
 *	@note	FBXSDK-Sample
 *	@note	https://github.com/eaglesakura/protoground/tree/3c03d3ddd0af7fde562e50caeb288727af8139fd
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <fbxsdk.h>

class FbxBinaryStream : public FbxStream
{
private:
	mutable uint8_t *head_ = nullptr;
	mutable int64_t current_ = 0;
	mutable int64_t length_ = 0;

public:

	FbxBinaryStream(int64_t argLength) 
		: length_(argLength) 
	{
		assert(length_ > 0 && "file size <= 0");
	}
	virtual ~FbxBinaryStream() = default;

	virtual EState GetState() { return (head_ != nullptr) ? eOpen : eClosed; }
	virtual bool Open(void *argStreamData) 
	{
		current_ = 0;
		head_ = static_cast<uint8_t*>(argStreamData);
		return true;
	}
	virtual bool Close() 
	{
		head_ = nullptr;
		current_ = 0;
		return true;
	}
	virtual bool Flush() { return true; }
	virtual int Write(const void * /*argData*/, int /*argData*/) { return 0; }
	virtual int Read(void *argData, int argSize) const
	{
		memcpy(argData, head_ + current_, argSize);
		current_ += argSize;
		return argSize;
	}
	virtual int GetReaderID() const { return 0; }
	virtual int GetWriterID() const { return -1; }
	virtual void Seek(const FbxInt64 &argOffset, const FbxFile::ESeekPos &argSeekPos) 
	{
		switch (argSeekPos) 
		{
		case FbxFile::ESeekPos::eBegin:
			current_ = argOffset;
			break;
		case FbxFile::ESeekPos::eCurrent:
			current_ += argOffset;
			break;
		case FbxFile::ESeekPos::eEnd:
			assert(length_ > 0 && "file size <= 0");
			assert(argOffset <= 0 && "offset > 0");
			current_ = length_ + argOffset;
			break;
		default:
			assert(false && "seekpos type ");
		}
	}
	virtual long GetPosition() const { return current_; }
	virtual void SetPosition(long pPosition) { current_ = pPosition; }
	virtual int GetError() const { return 0; }
	virtual void ClearError() {}


};