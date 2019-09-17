// CPP_GetSMBIOS_Parser.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cstdio>
#include "smbios.hpp"
#include <windows.h>
#include <string>

struct RawSMBIOSData
{
	BYTE    Used20CallingMethod;
	BYTE    SMBIOSMajorVersion;
	BYTE    SMBIOSMinorVersion;
	BYTE    DmiRevision;
	DWORD    Length;
	BYTE    SMBIOSTableData[];
};

int main()
{
	DWORD error = ERROR_SUCCESS;
	DWORD smBiosDataSize = 0;
	RawSMBIOSData* smBiosData = NULL; // Defined in this link
	DWORD bytesWritten = 0;

	// Query size of SMBIOS data.
	smBiosDataSize = GetSystemFirmwareTable('RSMB', 0, NULL, 0);

	// Allocate memory for SMBIOS data
	smBiosData = (RawSMBIOSData*)HeapAlloc(GetProcessHeap(), 0, smBiosDataSize);
	if (!smBiosData) {
		error = ERROR_OUTOFMEMORY;
		//goto exit;
	}

	// Retrieve the SMBIOS table
	bytesWritten = GetSystemFirmwareTable('RSMB', 0, smBiosData, smBiosDataSize);

	if (bytesWritten != smBiosDataSize) {
		error = ERROR_INVALID_DATA;
		//goto exit;
	}

	// Process the SMBIOS data and free the memory under an exit label

	using namespace smbios;

	parser meta;
	byte_t* buff = smBiosData->SMBIOSTableData;
	size_t  buff_size = smBiosDataSize;

	//////////////////////////////////////////////////////
	// Code that makes buffer to point (or hold) smbios //
	// structure table. Don't forget about size.        //
	//////////////////////////////////////////////////////

	meta.feed(buff, buff_size);
	int count = 0;
	for (auto it = meta.headers.begin(); it != meta.headers.end(); ++it) {
		//printf("%d ", count++);
		string_array_t strings;

		parser::extract_strings(*it, strings);

		switch ((*it)->type) {
			// ...
		case types::baseboard_info: {
			auto x = static_cast<baseboard_info*>(*it);

			if (x->length == 0)
				break;

			/*for (auto& text : strings) {
				printf("%s\n", text);
			}*/
			std::cout << "Motherboard\n";
			printf("ManufacturerName  %s\n", strings[x->ManufacturerName]);
			printf("ProductName       %s\n", strings[x->ProductName]);
			printf("Version           %s\n", strings[x->Version]);
			printf("SerialNumber      %s\n", strings[x->SerialNumber]);
			printf("\n");
		}break;
		/*case types::processor_info: {
			auto x = static_cast<proc_info*>(*it);

			if (x->length == 0)
				break;
			printf("socket_designation %s\n", strings[x->socket_designation]);
			printf("  manufacturer     %s\n", strings[x->manufacturer]);
			printf("  serial_number    %s\n", strings[x->serial_number]);
			printf("  ID               %ld\n", x->id);
		}break;*/
		case types::memory_device: {
			auto x = static_cast<mem_device*>(*it);

			if (x->total_width == 0)
				break;
			printf("Memory device %s (%s):\n", strings[x->device_locator], strings[x->bank_locator]);
			printf("  Data width:    %d bit\n", x->data_width);
			printf("  Size:          %d M\n", x->size);
			printf("  Speed:         %d MHz\n", x->speed);
			printf("  Clock speed:   %d MHz\n", x->clock_speed);
			printf("  Manufacturer:  %s\n", strings[x->manufacturer]);
			printf("  S/N:           %s\n", strings[x->serial_number]);
			printf("  Tag number:    %s\n", strings[x->assert_tag]);
			printf("  Part number:   %s\n", strings[x->part_number]);
			printf("\n");
		} break;

			// ...
			// Handle all other struct types
			// ...

		//default: printf("Unknown structure type %d\n", (*it)->type);
		}
	}

	//////////////////
	// Free buffers //
	//////////////////
	std::cin.get();
	return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
