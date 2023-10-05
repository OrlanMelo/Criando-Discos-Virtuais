#include <Windows.h>
#include <virtdisk.h>
#include <string>
#include <iostream>
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"virtdisk.lib")

using namespace std;

/* Tratativa de erro ao usar valores de VIRTUAL_STORAGE_TYPE. */
/* Para usar os valores de VIRTUAL_STORAGE_TYPE, ter� que habilitar a compila��o de cabe�alho pr�-compilado "stdafx.h". */

class cFuncoes
{
private:

	HANDLE Disco;

	VIRTUAL_STORAGE_TYPE VirtType;

	CREATE_VIRTUAL_DISK_PARAMETERS Parametros;

public:

	DWORD Result;

	void CriarDiscoVirtual(bool VHDX, bool Dinamico, wstring Local, ULONGLONG Tamanho)
	{
		if (VHDX == true)
		{
			VirtType = {
			VIRTUAL_STORAGE_TYPE_DEVICE_VHDX,
			VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT
			};
		}
		if (VHDX == false)
		{
			VirtType = {
			VIRTUAL_STORAGE_TYPE_DEVICE_VHD,
			VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT
			};
		}

		memset(&Parametros, 0, sizeof(Parametros));

		Parametros.Version = CREATE_VIRTUAL_DISK_VERSION_1;
		Parametros.Version1.BlockSizeInBytes = CREATE_VIRTUAL_DISK_PARAMETERS_DEFAULT_BLOCK_SIZE;
		Parametros.Version1.SectorSizeInBytes = CREATE_VIRTUAL_DISK_PARAMETERS_DEFAULT_SECTOR_SIZE;
		Parametros.Version1.SourcePath = 0;
		Parametros.Version1.MaximumSize = Tamanho;

		//Esta fun��o n�o possui par�metro de fun��o ass�ncrona, sendo �til apenas de uso s�ncrono.
		//CREATE_VIRTUAL_DISK_FLAG_NONE para discos din�micos.
		//CREATE_VIRTUAL_DISK_FLAG_FULL_PHYSICAL_ALLOCATION para discos fixos.
		if (Dinamico == true)
			Result = CreateVirtualDisk(&VirtType, Local.c_str(), VIRTUAL_DISK_ACCESS_ALL, 0,
				CREATE_VIRTUAL_DISK_FLAG_NONE, 0, &Parametros, 0, &Disco);
		else
			Result = CreateVirtualDisk(&VirtType, Local.c_str(), VIRTUAL_DISK_ACCESS_ALL, 0,
				CREATE_VIRTUAL_DISK_FLAG_FULL_PHYSICAL_ALLOCATION, 0, &Parametros, 0, &Disco);
	}

}Funcoes;

int main()
{

	cout << "O assistente est� criando um arquivo de disco virtual...";

	ULONGLONG TamanhoGB = 1 * 1024 * 1024 * 1024; // 1 GB.
	ULONGLONG TamanhoMB = 500 * 1024 * 1024; // 500 MB
	Funcoes.CriarDiscoVirtual(false, false, L"C:\\DISCOS VIRTUAIS\\DISCO.vhd", TamanhoGB);
	if (Funcoes.Result == ERROR_SUCCESS)
	{
		cout << "O arquivo de disco virtual selecionado foi criado com sucesso.";
	}
	else
	{
		cout << "N�o foi poss�vel prosseguir, verifique o c�digo de erro: " << GetLastError();
	}

	system("pause");
}