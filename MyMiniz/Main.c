#pragma warning(disable: 4668 26451 6386 6297 5045)
#pragma warning(push,3)
#include <Windows.h>
#include "miniz.h"
#pragma warning(pop)
#include <stdlib.h>
#include <stdio.h>

#define OPERATION_ADD		1
#define OPERATION_EXTRACT	2


BOOL FileExists(_In_ char* FileName)
{
	const DWORD attributes = GetFileAttributesA(FileName);
	return (attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY));
}


int main(int argc, char* argv[])
{
	int operation;

	if (argc != 4)
	{
		printf("Adds or extracts files from a compressed archive.\n");
		printf("Usage: myminiz.exe <archive_file> <[+|-]> <filename>\n");
		return 0;
	}

	char* archive_name = argv[1];
	char* fully_qualified_file_name = argv[3];

	if (_stricmp(argv[2], "+") == 0)
	{
		operation = OPERATION_ADD;
	}
	else if (_stricmp(argv[2], "-") == 0)
	{
		operation = OPERATION_EXTRACT;
	}
	else
	{
		printf("Adds or extracts files from a compressed archive.\n");
		printf("Usage: myminiz.exe <archive_file> <[+|-]> <filename>\n");
		return 0;
	}

	if (operation == OPERATION_ADD)
	{

		DWORD bytes_read = 0;
		BYTE* file_buffer;
		HANDLE file_handle;
		LARGE_INTEGER file_size = { 0 };
		DWORD error = ERROR_SUCCESS;
		char file_name[MAX_PATH] = { 0 };
		char file_extension[MAX_PATH] = { 0 };

		if ((file_handle = CreateFileA(fully_qualified_file_name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL)) == INVALID_HANDLE_VALUE)
		{
			error = GetLastError();
			printf("ERROR: The file %s cannot be found, or could not be opened for reading! 0x%08lx\n", fully_qualified_file_name, error);
			return error;
		}
		printf("[+] File %s opened for reading.\n", fully_qualified_file_name);

		if (GetFileSizeEx(file_handle, &file_size) == 0)
		{
			error = GetLastError();
			printf("ERROR: The file %s cannot be found, or could not be opened for reading! 0x%08lx\n", fully_qualified_file_name, error);
			return error;
		}

		printf("[+] File size: %lld\n", file_size.QuadPart);
		if ((file_buffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, file_size.QuadPart)) == NULL)
		{
			error = ERROR_OUTOFMEMORY;
			printf("ERROR: The file %s cannot be found, or could not be opened for reading! 0x%08lx\n", fully_qualified_file_name, error);
			return error;
		}

		if ((ReadFile(file_handle, file_buffer, (DWORD)file_size.QuadPart, &bytes_read, NULL)) == FALSE)
		{
			error = GetLastError();
			printf("ERROR: ReadFile failed! 0x%08lx\n", error);
			return error;
		}

		if (bytes_read != file_size.QuadPart)
		{
			error = ERROR_READ_FAULT;
			printf("ERROR: Bytes read into memory did not match file size!\n");
			return error;
		}

		_splitpath_s(fully_qualified_file_name, NULL, 0, NULL, 0, file_name, sizeof(file_name), file_extension, sizeof(file_extension));

		strcat_s(file_name, sizeof(file_name), file_extension);

		printf("[+] FileName: %s", file_name);

		if (mz_zip_add_mem_to_archive_file_in_place(archive_name, file_name, file_buffer, (size_t)file_size.QuadPart, "", 1, MZ_BEST_COMPRESSION) == MZ_FALSE)
		{
			error = ERROR_COMPRESSED_FILE_NOT_SUPPORTED;
			printf("ERROR: Failed to add file to %s to archive %s! 0x%08lx\n", fully_qualified_file_name, archive_name, error);
			return error;
		}

		printf("[+] File %s successfully added to archive %s.\n", fully_qualified_file_name, archive_name);

		if (file_handle && file_handle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(file_handle);
		}

		return error;

	}
	else if (operation == OPERATION_EXTRACT)
	{

		BOOL file_found_in_archive = FALSE;
		mz_zip_archive zip_archive = { 0 };
		mz_zip_error mz_error = MZ_ZIP_NO_ERROR;

		if (FileExists(archive_name) == FALSE)
		{
			printf("ERROR: Archive %s does not exist!\n", archive_name);
			return ERROR_FILE_NOT_FOUND;
		}

		if (mz_zip_reader_init_file(&zip_archive, archive_name, 0) == FALSE)
		{
			mz_error = mz_zip_get_last_error(&zip_archive);
			printf("ERROR: mz_zip_reader_init_file failed with error code %d!\n", mz_error);
			return mz_error;
		}

		printf("[-] Archive %s opened for reading.\n", archive_name);

		for (int file_index = 0; file_index < (int)mz_zip_reader_get_num_files(&zip_archive); file_index++)
		{
			mz_zip_archive_file_stat compressed_file_statistics = { 0 };
			if (mz_zip_reader_file_stat(&zip_archive, file_index, &compressed_file_statistics) == FALSE)
			{
				mz_error = mz_zip_get_last_error(&zip_archive);
				printf("ERROR: mz_zip_reader_file_stat failed with error code %d!\n", mz_error);
				return mz_error;
			}

			if (_stricmp(compressed_file_statistics.m_filename, fully_qualified_file_name) == 0)
			{
				file_found_in_archive = TRUE;
				printf("[-] File %s found in archive%s.\n", fully_qualified_file_name, archive_name);

				if (mz_zip_reader_extract_to_file(&zip_archive, file_index, fully_qualified_file_name, 0) == FALSE)
				{
					mz_error = mz_zip_get_last_error(&zip_archive);
					printf("ERROR: mz_zip_reader_extract_to_file failed with error code %d!\n", mz_error);
					return mz_error;
				}
				else 
				{
					printf("[-] Successfully extracted file %s.\n", fully_qualified_file_name);
				}
				break;
			}
		}

		if (file_found_in_archive == FALSE) 
		{
			printf("ERROR: File %s not found in archive %s!\n", fully_qualified_file_name, archive_name);
			return ERROR_FILE_NOT_FOUND;
		}


	}
	else
	{
		printf("ERROR: No operation specified!\n");
	}

}
