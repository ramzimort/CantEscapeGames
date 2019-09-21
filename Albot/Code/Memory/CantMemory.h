#pragma once

namespace CantMemory
{
	namespace Stack
	{
		template <typename T> void* Allocate();
		template <typename T> void Free();
	}

	namespace Pool
	{
		template <typename T> void* Allocate();
		template <typename T> void Free();
	}
}