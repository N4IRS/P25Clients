/*
 *   Copyright (C) 2015,2016,2025 by Jonathan Naylor G4KLX
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "Thread.h"

#if defined(_WIN32) || defined(_WIN64)

CThread::CThread() :
m_handle()
{
}

CThread::~CThread()
{
}

bool CThread::run()
{
  m_handle = ::CreateThread(nullptr, 0, &helper, this, 0, nullptr);

  return m_handle != nullptr;
}


void CThread::wait()
{
  ::WaitForSingleObject(m_handle, INFINITE);

  ::CloseHandle(m_handle);
}


DWORD CThread::helper(LPVOID arg)
{
  CThread* p = (CThread*)arg;

  p->entry();

  return 0UL;
}

void CThread::sleep(unsigned int ms)
{
	::Sleep(ms);
}

#else

#include <unistd.h>

CThread::CThread() :
m_thread()
{
}

CThread::~CThread()
{
}

bool CThread::run()
{
  return ::pthread_create(&m_thread, nullptr, helper, this) == 0;
}


void CThread::wait()
{
  ::pthread_join(m_thread, nullptr);
}


void* CThread::helper(void* arg)
{
  CThread* p = (CThread*)arg;

  p->entry();

  return nullptr;
}

void CThread::sleep(unsigned int ms)
{
	::usleep(ms * 1000);
}

#endif
