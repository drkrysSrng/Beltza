#include <iostream>
#include <Windows.h>
#include <fstream>
#include <vector>
#include <string>
#include "..\VirtualAES\VirtualAES.h"
#include <Windows.h>
#include <TlHelp32.h>



char output[MAX_PATH];
char name[MAX_PATH];

void askforbinary()
{
	std::cout << "Please enter the Path of the file\n";
	std::cin >> name;
	std::cout << "Enter output name: ";
	std::cin >> output;
}

/*		  AES-256 Bit Encryption Function			*/
void AESEncrypt(char* rawData, int size)
{
    //256 Bit Key
    unsigned char key[KEY_256] = "Zr4u7x!A%D*G-KaPdSgUkXp2s5v8y/B";

    unsigned char plaintext[BLOCK_SIZE];
    unsigned char ciphertext[BLOCK_SIZE];

    aes_ctx_t* ctx;
    virtualAES::initialize();
    ctx = virtualAES::allocatectx(key, sizeof(key));

    int count = 0;
    int index = size/16; //Outer loop range
    int innerCount = 0;
    int innerIndex = 16; //We encrypt&copy 16 Bytes for once.
    int dataIndex = 0; //Non resetting @rawData index for encryption
    int copyIndex = 0; //Non resetting @rawData index for copying encrypted data.

    /*
     * Our Block Size 16 Byte. Outer loop range has to be executablesize/16.
     *
     * First we store first 16 byte of our executable into @plaintext
     * We encrypt @plaintext.
     * @rawData index shouldnt be reset to 0. So @dataCount variable always increasing.
     * Thus @rawData always be like @rawData[16, 32, 64, 128 ... @executablesize]
     *
     * After encryption we copy the encrypted data into @rawData.
     * Again we use special index(@copyCount) which it never be reset to 0.
     */

    for(count; count < index; count++)
    {
        for(innerCount = 0; innerCount < innerIndex; innerCount++)
        {
            plaintext[innerCount] = rawData[dataIndex];
            dataIndex++;
        }

        virtualAES::encrypt(ctx, plaintext, ciphertext);

        for(innerCount = 0; innerCount < innerIndex; innerCount++)
        {
            rawData[copyIndex] = ciphertext[innerCount];
            copyIndex++;
        }
    }

    delete ctx;
}

int encrypt()
{
    std::ifstream data(name, std::ios::binary);		//Open

    data.seekg(0, data.end);							//Go eof
	long datasize = static_cast<long>(data.tellg());	//Get Size
    char *rawData = new char[datasize];

    printf("Size of file %ld\n", data.tellg());

    std::cout << "Opening...\n"<< name;

    if (!data.is_open())
	{
        std::cout << "Error opening...\n"<< name;
		return -1;
	}

	data.seekg(0);
	data.read(rawData, datasize);
	data.close();

    std::fstream built(output, std::ios::binary | std::ios::out);

    std::cout << "Opening...\n"<< output;

    if (!built.is_open())
	{
        std::cout << "Error opening...\n"<< output;
		return -1;
	}

    char filename[]= "Stub.exe";

	std::ifstream file(filename, std::ios::binary);

	std::cout << "Opening...\n"<< filename;

	if (!file.is_open())
	{
        std::cout << "Error opening...\n"<< filename;
		return -1;
	}

    file.seekg(0, file.end);

	long stubsize = static_cast<long>(file.tellg());

	printf("Size of file %ld\n", file.tellg());


    file.seekg(0);

    char *stubData = new char[stubsize];

	file.read(stubData, stubsize);
    file.close();

    for(auto i = 0; i < stubsize; i++)
    {
        built << stubData[i];
    }


    AESEncrypt(rawData, datasize);

    for(auto i = 0; i < datasize; i++)
    {
        built << rawData[i];
    }
    built.close();

    return 0;

}


int main()
{
	askforbinary(); //Read the file
	std::cout << "Opening...\n";
	return encrypt();
}


