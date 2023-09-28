
static constexpr int LZSS_LOOKSHIFT = 4;

struct lzss_header_t
{
	unsigned int id;
	unsigned int actualSize;
};

// Rewrite of CLZSS::SafeUncompress to fix a vulnerability where malicious compressed payloads could cause the decompressor to try to read
// out of the bounds of the output buffer.

unsigned int (*o_CLZSS__SafeDecompress)(void* self, const unsigned char* pInput, unsigned char* pOutput, unsigned int unBufSize);

unsigned int h_CLZSS__SafeDecompress(void* self, const unsigned char* pInput, unsigned char* pOutput, unsigned int unBufSize)
{
	NOTE_UNUSED(self);
	unsigned int totalBytes = 0;
	int getCmdByte = 0;
	int cmdByte = 0;

	lzss_header_t header = *(lzss_header_t*)pInput;

	if (!pInput || !header.actualSize || header.id != 0x53535A4C || header.actualSize > unBufSize)
		return 0;

	pInput += sizeof(lzss_header_t);

	for (;;)
	{
		if (!getCmdByte)
			cmdByte = *pInput++;

		getCmdByte = (getCmdByte + 1) & 0x07;

		if (cmdByte & 0x01)
		{
			int position = *pInput++ << LZSS_LOOKSHIFT;
			position |= (*pInput >> LZSS_LOOKSHIFT);
			position += 1;
			int count = (*pInput++ & 0x0F) + 1;
			if (count == 1)
				break;

			// Ensure reference chunk exists entirely within our buffer
			if (position > totalBytes)
				return 0;

			totalBytes += count;
			if (totalBytes > unBufSize)
				return 0;

			unsigned char* pSource = pOutput - position;
			for (int i = 0; i < count; i++)
				*pOutput++ = *pSource++;
		}
		else
		{
			totalBytes++;
			if (totalBytes > unBufSize)
				return 0;

			*pOutput++ = *pInput++;
		}
		cmdByte = cmdByte >> 1;
	}

	if (totalBytes != header.actualSize)
		return 0;

	return totalBytes;
}

ON_DLL_LOAD("engine.dll", ExploitFixes_LZSS, (CModule module))
{
	o_CLZSS__SafeDecompress = module.Offset(0x432A10).RCast<unsigned int (*)(void*, const unsigned char*, unsigned char*, unsigned int)>();
	HookAttach(&(PVOID&)o_CLZSS__SafeDecompress, (PVOID)h_CLZSS__SafeDecompress);
}
