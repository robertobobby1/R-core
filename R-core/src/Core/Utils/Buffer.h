#pragma once
#include "rcpch.h"

namespace RC {

	class Buffer {
	public:
		// Inits an empty buffer
		Buffer() : m_bufferIni(nullptr), m_bufferPos(nullptr), m_bufferSize(0) {}

		// Allocates for the given buffersize
		Buffer(int bufferSize) 
			: m_bufferSize(bufferSize)
		{
			m_bufferIni = ( char*)malloc(bufferSize);
			m_bufferPos = m_bufferIni;
		}

		Buffer(char* buffer, int bufferSize)
			: m_bufferSize(bufferSize), m_bufferIni(buffer), m_bufferPos(buffer)
		{}

		~Buffer() 
		{ 
			if(m_bufferIni != nullptr)
				free((void*)m_bufferIni);  
		}

		inline char* Begin() { return m_bufferIni; }
		inline const char* Get() { return m_bufferPos; }
		inline int Size() { return m_bufferSize; }

		//inline  char* End() { return m_bufferIni + m_bufferSize; }
			
		inline void Move(int moveSize) { m_bufferPos = &m_bufferPos[moveSize]; }
		inline bool IsEnd() { return (m_bufferPos - m_bufferIni) > m_bufferSize; }

		// Destructively appends, deletes the otherBuffer
		inline void Append(Buffer otherBuffer) 
		{
			if (otherBuffer.Begin() == nullptr)
				return;

			Append(Begin(), Size()); 
			otherBuffer.~Buffer();
		}

		inline bool Append(char* otherBuffer, int bufLength)
		{
			// get actual position to reposition on new reallocated buffer
			std::ptrdiff_t bytes = ((char*)m_bufferPos) - ((char*)m_bufferIni);
			// allocate new buffer size and check if it worked
			auto buff = ( char*)realloc((void*)m_bufferIni, m_bufferSize + bufLength);
			if (buff == nullptr)
				return false;

			m_bufferIni = buff;
			m_bufferPos = &m_bufferIni[bytes];
			// copy both buffers one after the other
			memcpy((void*)&m_bufferIni[m_bufferSize], otherBuffer, bufLength);
			m_bufferSize = m_bufferSize + bufLength;

			return true;
		}

	private:

		char* m_bufferIni;
		const char* m_bufferPos;

		int m_bufferSize;
	};
}