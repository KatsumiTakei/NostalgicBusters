/**
 *	@file	CustomStream.h
 *	@date	2017 / 10 / 05
 *	@author	Katsumi Takei
 *	@note	FBXSDK-Sample
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <fbxsdk.h>

class CustomStream : public FbxStream
{
private:
	FILE*	file_;
	int		readerID_;
	int		writerID_;
	char    fileName_[256];
	char    mode_[3];

public:
	CustomStream(FbxManager* argSdkManager, const char *argFilname, const char* argMode)
		:file_(nullptr)
	{
		if (!argMode)
			return;

		FBXSDK_strcpy(fileName_, 256, argFilname);
		FBXSDK_strcpy(mode_, 3, (argMode) ? argMode : "r");

		if (argMode[0] == 'r')
		{
			const char* format = "FBX (*.fbx)";
			readerID_ = argSdkManager->GetIOPluginRegistry()->FindReaderIDByDescription(format);
			writerID_ = -1;
		}
		else
		{
			const char* format = "FBX ascii (*.fbx)";
			writerID_ = argSdkManager->GetIOPluginRegistry()->FindWriterIDByDescription(format);
			readerID_ = -1;
		}
	}

	~CustomStream()
	{
		Close();
	}

public:
	virtual EState GetState() { return (file_) ? FbxStream::eOpen : eClosed; }
	virtual int GetReaderID() const { return readerID_; }
	virtual int GetWriterID() const { return writerID_; }

public:
	virtual bool Open(void* /*pStreamData*/)
	{
		if (!file_)
			FBXSDK_fopen(file_, fileName_, mode_);

		if (file_)
			fseek(file_, 0L, SEEK_SET);

		return (file_ != nullptr);
	}

	virtual bool Close()
	{
		if (file_)
			fclose(file_);
		
		file_ = nullptr;
		return true;
	}

	virtual bool Flush()
	{
		return true;
	}

	virtual int Write(const void* argData, int argSize)
	{
		if (!file_)
			return 0;
		
		return static_cast<int>(fwrite(argData, 1, argSize, file_));
	}

	virtual int Read(void* argData, int argSize) const
	{
		if (!file_)
			return 0;
		
		return static_cast<int>(fread(argData, 1, argSize, file_));
	}
	void Seek(const FbxInt64& argOffset, const FbxFile::ESeekPos& argSeekPos)
	{
		switch (argSeekPos)
		{
		case FbxFile::eBegin:
			fseek(file_, static_cast<long>(argOffset), SEEK_SET);
			break;
		case FbxFile::eCurrent:
			fseek(file_, static_cast<long>(argOffset), SEEK_CUR);
			break;
		case FbxFile::eEnd:
			fseek(file_, static_cast<long>(argOffset), SEEK_END);
			break;
		}
	}

	virtual long GetPosition() const
	{
		if (!file_)
			return 0;

		return ftell(file_);
	}
	virtual void SetPosition(long argPosition)
	{
		if (file_)
			fseek(file_, argPosition, SEEK_SET);
	}

	virtual int GetError() const
	{
		if (!file_)
			return 0;
		
		return ferror(file_);
	}
	virtual void ClearError()
	{
		if (file_)
			clearerr(file_);
	}

};