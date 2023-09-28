INT64(__fastcall* sub_F1320)(DWORD a1, char* a2);

// Reimplementation of an exploitable UTF decoding function in titanfall
bool __fastcall CheckUTF8Valid(INT64* a1, DWORD* a2, char* strData)
{
	DWORD v3; // eax
	char* v4; // rbx
	char v5; // si
	char* _strData; // rdi
	char* v7; // rbp
	char v11; // al
	DWORD v12; // er9
	DWORD v13; // ecx
	DWORD v14; // edx
	DWORD v15; // er8
	int v16; // eax
	DWORD v17; // er9
	int v18; // eax
	DWORD v19; // er9
	DWORD v20; // ecx
	int v21; // eax
	int v22; // er9
	DWORD v23; // edx
	int v24; // eax
	int v25; // er9
	DWORD v26; // er9
	DWORD v27; // er10
	DWORD v28; // ecx
	DWORD v29; // edx
	DWORD v30; // er8
	int v31; // eax
	DWORD v32; // er10
	int v33; // eax
	DWORD v34; // er10
	DWORD v35; // ecx
	int v36; // eax
	int v37; // er10
	DWORD v38; // edx
	int v39; // eax
	int v40; // er10
	DWORD v41; // er10

	v3 = a2[2];
	v4 = (char*)(a1[1] + *a2);
	v5 = 0;
	_strData = strData;
	v7 = &v4[*((UINT16*)a2 + 2)];
	if (v3 >= 2)
	{
		++v4;
		--v7;
		if (v3 != 2)
		{
			while (1)
			{
				if (!CMemory(v4).IsMemoryReadable(1))
					return false; // INVALID

				v11 = *v4++; // crash potential
				if (v11 != 92)
					goto LABEL_6;
				v11 = *v4++;
				if (v11 == 110)
					break;
				switch (v11)
				{
				case 't':
					v11 = 9;
					goto LABEL_6;
				case 'r':
					v11 = 13;
					goto LABEL_6;
				case 'b':
					v11 = 8;
					goto LABEL_6;
				case 'f':
					v11 = 12;
					goto LABEL_6;
				}
				if (v11 != 117)
					goto LABEL_6;
				v12 = *v4 | 0x20;
				v13 = v4[1] | 0x20;
				v14 = v4[2] | 0x20;
				v15 = v4[3] | 0x20;
				v16 = 87;
				if (v12 <= 0x39)
					v16 = 48;
				v17 = v12 - v16;
				v18 = 87;
				v19 = v17 << 12;
				if (v13 <= 0x39)
					v18 = 48;
				v20 = v13 - v18;
				v21 = 87;
				v22 = (v20 << 8) | v19;
				if (v14 <= 0x39)
					v21 = 48;
				v23 = v14 - v21;
				v24 = 87;
				v25 = (16 * v23) | v22;
				if (v15 <= 0x39)
					v24 = 48;
				v4 += 4;
				v26 = (v15 - v24) | v25;
				if (v26 - 55296 <= 0x7FF)
				{
					if (v26 >= 0xDC00)
						return true;
					if (*v4 != 92 || v4[1] != 117)
						return true;

					v27 = v4[2] | 0x20;
					v28 = v4[3] | 0x20;
					v29 = v4[4] | 0x20;
					v30 = v4[5] | 0x20;
					v31 = 87;
					if (v27 <= 0x39)
						v31 = 48;
					v32 = v27 - v31;
					v33 = 87;
					v34 = v32 << 12;
					if (v28 <= 0x39)
						v33 = 48;
					v35 = v28 - v33;
					v36 = 87;
					v37 = (v35 << 8) | v34;
					if (v29 <= 0x39)
						v36 = 48;
					v38 = v29 - v36;
					v39 = 87;
					v40 = (16 * v38) | v37;
					if (v30 <= 0x39)
						v39 = 48;
					v4 += 6;
					v41 = ((v30 - v39) | v40) - 56320;
					if (v41 > 0x3FF)
						return true;
					v26 = v41 | ((v26 - 55296) << 10);
				}
				_strData += (DWORD)sub_F1320(v26, _strData);
			LABEL_7:
				if (v4 == v7)
					goto LABEL_48;
			}
			v11 = 10;
		LABEL_6:
			v5 |= v11;
			*_strData++ = v11;
			goto LABEL_7;
		}
	}
LABEL_48:
	return true;
}

// prevent utf8 parser from crashing when provided bad data, which can be sent through user-controlled openinvites
bool (*o_RSON_ParseUTF8)(INT64* a1, DWORD* a2, char* pszData);

bool h_RSON_ParseUTF8(INT64* a1, DWORD* a2, char* strData)
{
	static void* targetRetAddr = CModule("engine.dll").FindPatternSIMD("84 C0 75 2C 49 8B 16");

	// only call if we're parsing utf8 data from the network (i.e. communities), otherwise we get perf issues
	void* pReturnAddress = _ReturnAddress();

	if (pReturnAddress == targetRetAddr && !CheckUTF8Valid(a1, a2, strData))
		return false;

	return o_RSON_ParseUTF8(a1, a2, strData);
}

ON_DLL_LOAD("engine.dll", EngineExploitFixes_UTF8Parser, (CModule module))
{
	o_RSON_ParseUTF8 = module.Offset(0xEF670).RCast<bool (*)(INT64*, DWORD*, char*)>();
	HookAttach(&(PVOID&)o_RSON_ParseUTF8, (PVOID)h_RSON_ParseUTF8);

	sub_F1320 = module.FindPatternSIMD("83 F9 7F 77 08 88 0A").RCast<INT64(__fastcall*)(DWORD, char*)>();
}
