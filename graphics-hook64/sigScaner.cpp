#include "sigScaner.h"
#include <windows.h>
#include <vector>
uintptr_t sigScaner::find(const char* module, const char* pattern)
{
	uintptr_t moduleAdressmm = 0;
	moduleAdressmm = *(uintptr_t*)(__readgsqword(0x60) + 0x10);

	static auto patternToByteZmm = [](const char* pattern)
	{
		auto       bytesmm = std::vector<int>{};
		const auto startmm = const_cast<char*>(pattern);
		const auto endmm = const_cast<char*>(pattern) + strlen(pattern);

		for (auto currentmm = startmm; currentmm < endmm; ++currentmm)
		{
			if (*currentmm == '?')
			{
				++currentmm;
				if (*currentmm == '?')
					++currentmm;
				bytesmm.push_back(-1);
			}
			else { bytesmm.push_back(strtoul(currentmm, &currentmm, 16)); }
		}
		return bytesmm;
	};

	const auto dosHeadermm = (PIMAGE_DOS_HEADER)moduleAdressmm;
	const auto ntHeadersmm = (PIMAGE_NT_HEADERS)((std::uint8_t*)moduleAdressmm + dosHeadermm->e_lfanew);

	const auto sizeOfImage = ntHeadersmm->OptionalHeader.SizeOfImage;
	auto       patternBytesmm = patternToByteZmm(pattern);
	const auto scanBytesmm = reinterpret_cast<std::uint8_t*>(moduleAdressmm);

	const auto smm = patternBytesmm.size();
	const auto dmm = patternBytesmm.data();

	for (auto imm = 0ul; imm < sizeOfImage - smm; ++imm)
	{
		bool foundmm = true;
		for (auto jmm = 0ul; jmm < smm; ++jmm)
		{
			if (scanBytesmm[imm + jmm] != dmm[jmm] && dmm[jmm] != -1)
			{
				foundmm = false;
				break;
			}
		}
		if (foundmm) { return reinterpret_cast<uintptr_t>(&scanBytesmm[imm]); }
	}
	return NULL;
}
uintptr_t sigScaner::GetAbsoluteAddress(uintptr_t address, int offset, int size) {
	if (address)
	{
		uintptr_t addrs = address + size + *(int*)(address + offset);
		return addrs;
	}
}
