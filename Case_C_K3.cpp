#include <iostream>
#include <string>
using namespace std;

//Batas maksimal data dalam sistem
const int MAX_PRODUK = 10;
const int MAX_PESANAN = 20;
const int MAX_ITEM = 3;

//Struct untuk menyimpan data produk
struct Produk{
    string nama;
    long long harga;
    int stok;
    int stokMinimum;
};

//Struct untuk menyimpan produk yang dibeli
struct ItemPesanan{
    int indeksProduk;
    string namaProduk;
    int jumlah;
    long long subtotal;
};

//Struct untuk menyimpan data pesanan pelanggan
struct Pesanan{
    string namaPelanggan;
    ItemPesanan item[MAX_ITEM];
    int jumlahItem;
    long long subtotal;
    int diskonPersen;
    long long nilaiDiskon;
    long long totalBayar;
};
//Validasi input selain angka
int bacaInt(const string &pesan){
    int nilai;
    while (true){
        cout << pesan;
        if (cin >> nilai){
            cin.ignore(10000, '\n');
            return nilai;
        }
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Input harus berupa angka!\n";
    }
}

long long bacaLongLong(const string &pesan){
    long long nilai;
    while (true){
        cout << pesan;
        if (cin >> nilai){
            cin.ignore(10000, '\n');
            return nilai;
        }
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Input harus berupa angka!\n";
    }
}

//Fungsi untuk menambahkan produk baru
//Pass by reference digunakan untuk mengubah jumlah produk
void tambahProduk(Produk *produk, int &jumlahProduk){
    if (jumlahProduk >= MAX_PRODUK) {
        cout << "Kapasitas produk sudah penuh!\n";
        return;
    }

    do {
        cout << "\nMasukkan nama produk: ";
        getline(cin, produk[jumlahProduk].nama);
        if (produk[jumlahProduk].nama.empty()){
            cout << "Nama tidak boleh kosong!\n";
        }
    } while (produk[jumlahProduk].nama.empty());

    do {
        produk[jumlahProduk].harga = bacaLongLong("Masukkan harga produk: Rp");
        if (produk[jumlahProduk].harga <= 0){
            cout << "Harga harus lebih dari nol!\n";
        }
    } while (produk[jumlahProduk].harga <= 0);

    do {
        produk[jumlahProduk].stok = bacaInt("Masukkan stok produk: ");
        if (produk[jumlahProduk].stok < 0){
            cout << "Stok tidak boleh negatif!\n";
        }
    } while (produk[jumlahProduk].stok < 0);

    do {
        produk[jumlahProduk].stokMinimum = bacaInt("Masukkan stok minimum: ");
        if (produk[jumlahProduk].stokMinimum < 0){
            cout << "Stok minimum tidak boleh negatif!\n";
        }
    } while (produk[jumlahProduk].stokMinimum < 0);

    //Menambah jumlah produk yang terdaftar
    jumlahProduk++;

    cout << "Produk berhasil ditambahkan!\n";
}

//Fungsi untuk menampilkan daftar produk
void tampilkanProduk(const Produk *produk, int jumlahProduk){
    if (jumlahProduk == 0) {
        cout << "Belum ada produk yang terdaftar.\n";
        return;
    }

    cout << "\n===== DAFTAR PRODUK =====\n";

    for (int i = 0; i < jumlahProduk; i++){
        cout << "\nNomor         : " << i + 1 << endl;
        cout << "Nama          : " << produk[i].nama << endl;
        cout << "Harga         : Rp" << produk[i].harga << endl;
        cout << "Stok          : " << produk[i].stok << endl;
        cout << "Stok minimum  : " << produk[i].stokMinimum << endl;
    }
}

//Fungsi untuk menampilkan detail pesanan
//Struct Pesanan dikirim menggunakan pass by reference
void tampilkanDetail(const Pesanan &p, int nomor){
    cout << "\n===== DETAIL PESANAN " << nomor << " =====\n";
    cout << "Nama pelanggan: " << p.namaPelanggan << endl;

    for (int i = 0; i < p.jumlahItem; i++){
        cout << p.item[i].namaProduk
             << " x " << p.item[i].jumlah
             << " = Rp" << p.item[i].subtotal << endl;
    }

    cout << "Subtotal      : Rp" << p.subtotal << endl;
    cout << "Diskon        : " << p.diskonPersen
         << "% (Rp" << p.nilaiDiskon << ")" << endl;
    cout << "Total bayar   : Rp" << p.totalBayar << endl;
}

//Fungsi untuk memproses pesanan pelanggan
void prosesPesanan(Produk *produk, int jumlahProduk,
                   Pesanan *pesanan, int &jumlahPesanan,
                   long long &totalPenjualan){

    if (jumlahPesanan >= MAX_PESANAN) {
        cout << "Kapasitas pesanan sudah penuh!\n";
        return;
    }

    if (jumlahProduk == 0) {
        cout << "Daftarkan produk terlebih dahulu!\n";
        return;
    }

    //Menampilkan produk agar pelanggan dapat memilih
    tampilkanProduk(produk, jumlahProduk);

    //Membuat pesanan baru
    Pesanan p{};

    cout << "\nMasukkan nama pelanggan: ";
    getline(cin, p.namaPelanggan);

    // Jumlah item tidak boleh melebihi 3 atau jumlah produk tersedia
    int batasItem = jumlahProduk;

    if (batasItem > MAX_ITEM){
        batasItem = MAX_ITEM;
    }

    do {
        p.jumlahItem = bacaInt("Jumlah jenis produk yang dibeli (1-"
                               + to_string(batasItem) + "): ");

        if (p.jumlahItem < 1 || p.jumlahItem > batasItem) {
            cout << "Jumlah jenis produk tidak valid!\n";
        }
    } while (p.jumlahItem < 1 || p.jumlahItem > batasItem);

    bool valid = true;

    // Memasukkan produk yang ingin dibeli
    for (int i = 0; i < p.jumlahItem; i++){
        int nomorProduk;
        int jumlahBeli;
     
        nomorProduk = bacaInt("\nMasukkan nomor produk: ");
        // Memeriksa nomor produk
        // Mengubah nomor produk menjadi indeks array
        int indeks = nomorProduk - 1;
        if (indeks < 0 || indeks >= jumlahProduk) {
            cout << "Nomor produk tidak ditemukan!\n";
            valid = false;
            break;
        }
        jumlahBeli = bacaInt("Masukkan jumlah pembelian: ");

      
       

        // Memeriksa agar produk yang sama tidak dipilih dua kali
        for (int j = 0; j < i; j++){
            if (p.item[j].indeksProduk == indeks) {
                cout << "Produk tidak boleh dipilih dua kali!\n";
                valid = false;
                break;
            }
        }

        if (!valid) {
            break;
        }

        // Memeriksa jumlah pembelian
        if (jumlahBeli <= 0) {
            cout << "Jumlah pembelian harus lebih dari nol!\n";
            valid = false;
            break;
        }

        // Memeriksa apakah stok mencukupi
        if (jumlahBeli > produk[indeks].stok) {
            cout << "Stok produk tidak mencukupi!\n";
            valid = false;
            break;
        }

        // Menyimpan informasi produk ke pesanan
        p.item[i].indeksProduk = indeks;
        p.item[i].namaProduk = produk[indeks].nama;
        p.item[i].jumlah = jumlahBeli;

        // Menghitung subtotal setiap produk
        p.item[i].subtotal =
            produk[indeks].harga * jumlahBeli;

        // Menambahkan subtotal produk ke subtotal pesanan
        p.subtotal += p.item[i].subtotal;
    }

    // Jika ada kesalahan, pesanan dibatalkan
    // Stok belum dikurangi
    if (!valid) {
        cout << "\nPesanan ditolak. Stok tidak berubah.\n";
        return;
    }

    // Menentukan diskon berdasarkan subtotal pembelian
    if (p.subtotal < 100000) {
        p.diskonPersen = 0;
    } else if (p.subtotal < 300000) {
        p.diskonPersen = 5;
    } else {
        p.diskonPersen = 10;
    }

    // Menghitung nominal diskon dan total pembayaran
    p.nilaiDiskon = p.subtotal * p.diskonPersen / 100;
    p.totalBayar = p.subtotal - p.nilaiDiskon;

    // Mengurangi stok setelah semua produk tervalidasi
    for (int i = 0; i < p.jumlahItem; i++){
        int indeks = p.item[i].indeksProduk;
        produk[indeks].stok -= p.item[i].jumlah;
    }

    // Menyimpan pesanan yang berhasil
    pesanan[jumlahPesanan] = p;
    jumlahPesanan++;

    // Menambahkan total pembayaran ke pendapatan
    totalPenjualan += p.totalBayar;

    cout << "\nPesanan berhasil diproses!\n";

    // Menampilkan detail pesanan
    tampilkanDetail(p, jumlahPesanan);
}

// Fungsi untuk menampilkan laporan inventaris dan penjualan
void laporan(const Produk *produk, int jumlahProduk,
             int jumlahPesanan, long long totalPenjualan) {

    cout << "\n===== LAPORAN INVENTARIS =====\n";

    // Menampilkan stok semua produk
    for (int i = 0; i < jumlahProduk; i++) {
        cout << produk[i].nama
             << " | Stok tersisa: " << produk[i].stok
             << " | Stok minimum: " << produk[i].stokMinimum
             << endl;
    }

    cout << "\n===== PRODUK DENGAN STOK RENDAH =====\n";

    bool adaStokRendah = false;

    // Mencari produk yang stoknya di bawah stok minimum
    for (int i = 0; i < jumlahProduk; i++){
        if (produk[i].stok < produk[i].stokMinimum){
            cout << produk[i].nama
                 << " | Stok: " << produk[i].stok << endl;
            adaStokRendah = true;
        }
    }

    if (!adaStokRendah){
        cout << "Tidak ada produk dengan stok rendah.\n";
    }

    // Menampilkan laporan penjualan
    cout << "\n===== LAPORAN PENJUALAN =====\n";
    cout << "Total pesanan berhasil: " << jumlahPesanan << endl;
    cout << "Total pendapatan      : Rp" << totalPenjualan << endl;
}

// Fungsi untuk menampilkan riwayat pesanan
void riwayatPesanan(const Pesanan *pesanan, int jumlahPesanan) {
    if (jumlahPesanan == 0) {
        cout << "Belum ada pesanan yang berhasil.\n";
        return;
    }

    for (int i = 0; i < jumlahPesanan; i++) {
        tampilkanDetail(pesanan[i], i + 1);
    }
}

int main(){
    // Mengalokasikan memori secara dinamis menggunakan pointer
    Produk *produk = new Produk[MAX_PRODUK]{};
    Pesanan *pesanan = new Pesanan[MAX_PESANAN]{};

    int jumlahProduk = 0;
    int jumlahPesanan = 0;
    long long totalPenjualan = 0;
    int pilihan;

    // Menu utama
    do {
        cout << "\n===== WAREHOUSE INVENTORY SYSTEM =====\n";
        cout << "1. Tambah produk\n";
        cout << "2. Tampilkan produk\n";
        cout << "3. Proses pesanan\n";
        cout << "4. Laporan inventaris dan penjualan\n";
        cout << "5. Riwayat pesanan\n";
        cout << "0. Keluar\n";
        pilihan = bacaInt("Pilih menu: ");

        // Menjalankan fungsi berdasarkan pilihan menu
        switch (pilihan){
            case 1:
                tambahProduk(produk, jumlahProduk);
                break;

            case 2:
                tampilkanProduk(produk, jumlahProduk);
                break;

            case 3:
                prosesPesanan(produk, jumlahProduk,
                              pesanan, jumlahPesanan,
                              totalPenjualan);
                break;

            case 4:
                laporan(produk, jumlahProduk,
                        jumlahPesanan, totalPenjualan);
                break;

            case 5:
                riwayatPesanan(pesanan, jumlahPesanan);
                break;

            case 0:
                cout << "Program selesai.\n";
                break;

            default:
                cout << "Pilihan menu tidak tersedia!\n";
        }

    } while (pilihan != 0);

    // Membebaskan memori yang telah dialokasikan
    delete[] produk;
    delete[] pesanan;

    return 0;
}