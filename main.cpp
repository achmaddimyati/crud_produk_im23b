#include <iostream>
#include <string>
#include <fstream>
#include <limits>

using namespace std;

struct Produk{
    int pk;
    char nama[20];
    int harga;
};
int pilihMenu();
void checkDatabase(fstream &data);

void writeData(fstream &data, int posisi, Produk &inputProduk)
{
    data.seekp((posisi-1)*sizeof(Produk), ios::beg);
    data.write(reinterpret_cast<char*>(&inputProduk), sizeof(Produk));
}

int getDataSize(fstream &data)
{
    int start, end;
    data.seekg(0, ios::beg);
    start = data.tellg();
    data.seekg(0, ios::end);
    end = data.tellg();

    return (end - start)/sizeof(Produk);
}

Produk readData(fstream &data, int posisi)
{
    Produk readProduk;
    data.seekg((posisi-1)*sizeof(Produk), ios::beg);
    data.read(reinterpret_cast<char*>(&readProduk), sizeof(Produk));
    return readProduk;
}

void addDataProduk(fstream &data)
{
    Produk inputProduk, lastProduk;
    int size = getDataSize(data);
    if(size == 0) {
       inputProduk.pk = 1;
    } else {
        lastProduk = readData(data, size);
        inputProduk.pk = lastProduk.pk + 1;
    }

    cout << "jumlah produk yang sudah diinput : " << size << endl;
    cout << "pk terakhir : " << lastProduk.pk << endl;
    cout << "nama produk : ";
    cin.getline(inputProduk.nama, 20);
    cout << "harga produk: ";
    cin >> inputProduk.harga;

    writeData(data, size+1, inputProduk);
}

void displayDataProduk(fstream &data)
{
    Produk showProduk;
    int size = getDataSize(data);

    cout << "No. \tpk \tNama \tHarga" << endl;
    for(int i = 1; i <= size; i++)
    {
        showProduk = readData(data, i);
        cout << i << "\t";
        cout << showProduk.pk << "\t";
        cout << showProduk.nama << "\t";
        cout << showProduk.harga << endl;
    }
}

void updateDataProduk(fstream &data)
{
    Produk updateProduk;
    int nomor;

    cout << "pilih nomor produk yang ingin dirubah ";
    cin >> nomor;
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    updateProduk = readData(data, nomor);
    cout << "detail produk yang akan dirubah : " << endl;
    cout << "nama : " << updateProduk.nama << endl;
    cout << "harga: " << updateProduk.harga << endl;

    cout << "nama produk : ";
    cin.getline(updateProduk.nama, 20);
    cout << "harga produk: ";
    cin >> updateProduk.harga;

    writeData(data, nomor, updateProduk);
}

void deleteDataProduk(fstream &data)
{
    int nomor, size, offset;
    Produk blankProduk, temporaryProduk;
    fstream dataSementara;

    size = getDataSize(data);
    cout << "pilih nomor produk yang akan dihapus : ";
    cin >> nomor;
    blankProduk.pk = 0;
    writeData(data, nomor, blankProduk);

    offset = 0;
    dataSementara.open("temp.dat", ios::trunc|ios::out|ios::in|ios::binary);
    for(int i = 1; i <= size; i++)
    {
        temporaryProduk = readData(data, i);
        if(temporaryProduk.pk != 0)
        {
            writeData(dataSementara, i-offset, temporaryProduk);
        } else {
            offset = offset+1; // offset++, offset += 1,
        }
    }

    size = getDataSize(dataSementara);
    data.close();
    data.open("data.bin", ios::trunc|ios::out|ios::binary);
    data.close();
    data.open("data.bin", ios::out|ios::in|ios::binary);

    for(int i = 1; i <= size; i++)
    {
        temporaryProduk = readData(dataSementara, i);
        writeData(data, i, temporaryProduk);
    }
}


int main()
{
    fstream data;
    checkDatabase(data);

    int pilihan = pilihMenu();
    char lanjutNggak;

    while(pilihan != 5)
    {
        switch(pilihan)
        {
            case 1:
                cout << "menu tambah produk" << endl;
                addDataProduk(data);
                break;
            case 2:
                cout << "menu lihat produk" << endl;
                displayDataProduk(data);
                break;
            case 3:
                cout << "menu ubah produk" << endl;
                displayDataProduk(data);
                updateDataProduk(data);
                displayDataProduk(data);
                break;
            case 4:
                cout << "menu hapus produk" << endl;
                displayDataProduk(data);
                deleteDataProduk(data);
                displayDataProduk(data);
                break;
            default:
                cout << "menu tidak valid" << endl;
                break;
        }

        label_continue:
        cout << "lanjutkan?[y/n] : ";
        cin >> lanjutNggak;
        if(lanjutNggak == 'y' | lanjutNggak == 'Y'){
            pilihan = pilihMenu();
        } else if(lanjutNggak == 'n' | lanjutNggak == 'N'){
            break;
        } else {
            goto label_continue;
        }
    }

    return 0;
}

int pilihMenu()
{
    int input;
    cout << "Program CRUD Produk" << endl;
    cout << "*******************" << endl;
    cout << "1. Tambah Produk" << endl;
    cout << "2. Lihat Produk" << endl;
    cout << "3. Ubah Produk" << endl;
    cout << "4. Hapus Produk" << endl;
    cout << "5. Exit" << endl;
    cout << "*******************" << endl;
    cout << "pilih menu?[1-5] : ";
    cin >> input;
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    return input;
}

void checkDatabase(fstream &data)
{
    data.open("data.bin", ios::out|ios::in|ios::binary);
    if(data.is_open()) {
        cout << "database ditemukan!" << endl;
    } else {
        cout << "database belum ada, buat baru" << endl;
        data.open("data.bin", ios::trunc|ios::out|ios::in|ios::binary);
    }
}
