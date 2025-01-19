#pragma once
#include "Define.h"
#include "Typedef.h"

namespace Delight
{
	// Engine Version. Not Use WRL::ComPtr..
	// Not Use Original Define.
	// uuidof, void**.
	#define DELIGHT_IID_PPV_ARGS(ppType) __uuidof(**(ppType)), reinterpret_cast<void**>(ppType)

	// default·Î pointer.. id3d...
	template<class ReferenceType>
	class ENGINE_DLL Comptr
	{
	public:
		Comptr()
			:Reference(nullptr)
		{}

		Comptr(ReferenceType* InReference)
			:ReferenceCount(nullptr), Reference(nullptr)
		{	
			if (InReference != nullptr)
			{
				Reference = *InReference;
				AddRef();
			}
		}

		~Comptr()
		{
			Release();
		}

	public:
		ReferenceType* operator->()
		{
			return Reference;
		}

		ReferenceType** operator&()
		{
			return &Reference;
		}

		void operator =(Comptr<ReferenceType>& InComPtr)
		{
			Attach(InComPtr.GetData());
		}

	public:		
		ReferenceType* GetData()
		{
			return Reference;
		}

		void Attach(ReferenceType* NewReference)
		{
			if (IsValid())
			{
				Detach();
				Reference = NewReference;

				AddRef();
			}
		}

		ReferenceType* Detach()
		{
			if (IsValid())
			{
				ReferenceType* RetReference = Reference;

				Release();
				Reference = nullptr;

				return RetReference;
			}
			return nullptr;
		}

		inline bool8 IsValid()
		{
			return (Reference != nullptr);
		}

		inline void AddRef()
		{
			Reference->AddRef();
		}

		inline void Release()
		{
			Reference->Release();
		}

	private:
		ReferenceType* Reference;
	};
}