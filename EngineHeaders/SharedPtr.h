#pragma once

#include "Typedef.h"

namespace Delight
{
	template<typename T>
	class SharedPtr
	{
	public:
		SharedPtr()
			:Reference(nullptr)
		{}

		SharedPtr(T* InReference)
			:Reference(InReference)
		{
			AddRef();
		}

		~SharedPtr()
		{
			Release();
		}
	public:
		SharedPtr(const SharedPtr& other)
			:Reference(other.Reference)
		{
			AddRef();
		}

		SharedPtr& operator = (const SharedPtr& other)
		{
			if (Reference != other.Reference)
			{
				Release();
				Reference = other.Reference;
				AddRef();
			}

			return *this;
		}

	public:
		T* operator->() {
			return Reference;
		}

		T& operator*() {
			return *Reference;
		}

		T* Get() {
			return Reference;
		}

	public:
		void AddRef()
		{
			if (IsValid())
			{
				Reference->AddRef();
			}
		}

		void Release()
		{
			if (IsValid())
			{
				if (Reference->Release())
				{
					Reference = nullptr;
				}
			}
		}
		// 쓸 일이 있을까..?
		void Reset()
		{
			if (IsValid())
			{
				Reference->Release();
				Reference = nullptr;
			}
		}

		bool8 IsValid()
		{
			return Reference != nullptr;
		}

	private:
		T* Reference;
	};
}