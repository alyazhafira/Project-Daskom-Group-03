/*program ini berfungsi untuk toko obat, program bisa menyimpan 
data obat yang keluar dan masuk termasuk jumlah obat, dan harga,
admin bisa menambahkan obat beserta harga dan jumlahnya, lalu user 
bisa memilih obat yang ingin di beli beserta jumlahnya*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<windows.h>

//inisiasi struct
struct Account{
    char username[32];
    char pass[32];
    char nama[32];
    int total;
};

struct Drug{
    char nama_obat[32];
    int harga;
    int jumlah;
};

//typedef
    //typdef untuk struct account
typedef struct Account account;
typedef account *ptrakun; //struct Account *akun -> ptrakun akun

    //typedef untuk struct drug
typedef struct Drug drug;
typedef drug *ptrobat;

//funtion prototype
void count_create(FILE *ptr, int *counter);//jika file belum ada maka dibuat dan berfungsi untuk menghitung jumlah line
void count_create_obat(FILE *ptr_obat, int *counter_obat);
void load_data(FILE *ptr, int *counter, ptrakun *load);//meload data kedalam program
void load_data_obat(FILE *ptr_obat, int *counter_obat, ptrobat *load);
void login(ptrakun login);// halaman login
int akun_checker(ptrakun *check, ptrakun temp, int maks, int *user);//berfungsi mengecek akun ke database
void add_akun(ptrakun add, int *lokasi, ptrakun *database);//register akun
int menu(); //daftar menu
int admin(); //daftar menu admin
void tambah_obat(ptrobat *add, int *lokasi); //fungsi untuk menambah jenis obat
void list_obat(ptrobat *cetak, int maks); //menampilkan list obat
void update_jumlah(ptrobat *update, int maks); //memperbaharui jumlah obat
int cari_obat(ptrobat *cari, char nama[32], int maks); //function untuk mencari obat
void hapus_obat(ptrobat *hapus, int *maks);//berfungsi untuk menghapus obat dari list
void beli_obat(ptrobat *beli, ptrakun *pembeli, int current, int max_obat); //berfungsi untuk membeli obat
void checkout(ptrakun *checkout, int current); //menampilkan nama dan jumlah biaya yang di keluarkan
void keluar(ptrobat *simpan_obat, ptrakun *simpan_akun, int maks_obat, int maks_akun, FILE *ptr, FILE *ptr_obat);//berfungsi untuk menyimpan data ke dalam file

int main(){
    int counter = 0,pilihan = 0,check = 0,counter_obat = 0, user_input, user = 0;
    FILE *ptr; //pointer menunjukan file
    FILE *ptr_obat;
    count_create(ptr, &counter);// campuran pass by reference(yang ada bintang) dan pass by value(tanpa bintang)
    count_create_obat(ptr_obat, &counter_obat);

    ptrakun akun; //deklarasi pointer akun
    account temp;
    ptrobat obat;

    //penyediaan memory dinamis;
    akun = (struct Account*)malloc((counter+10)*sizeof(struct Account));//penyediaan memory akun
    obat = (struct Drug*)malloc((counter_obat+10)*sizeof(struct Drug));//penyediaan memory obat

    //berfungsi untuk meload data dari file
    load_data(ptr, &counter, &akun);
    load_data_obat(ptr_obat, &counter_obat, &obat);

    while(1){
        system("CLS");
        login(&temp); // pass by reference
        check = akun_checker(&akun,&temp, counter, &user);
        Sleep(500);


        //user
        if(check == 1){// if yang berfungsi untuk menentukan apakah akun di temukan atau tidak
            pilihan = 0;
            //printf("\nLogin Berhasil\n");
            while(pilihan !=3 ){// berfungsi menjaga tetap di menu hingga memilih log out
                system("CLS");
                switch(menu())// switch untuk menentukan function mana yang akan di gunakan
                {
                    case 1:
                        system("CLS");
                        list_obat(&obat,counter_obat);
                        system("pause");
                        break;
                    
                    case 2:
                        system("CLS");
                        beli_obat(&obat,&akun,user,counter_obat);
                        break;
                    
                    case 3:
                        system("CLS");
                        checkout(&akun,user);
                        system("pause");
                        break;
                    case 4:
                        system("CLS");
                        pilihan = 3;
                        break;
                    
                    default:
                        system("CLS");
                        printf("\ninput salah silahkan ulangi lagi\n");
                        Sleep(500);
                        break;
                }
            }

        //admin   
        }else if(check == 2){// if yang berfungsi jika akun di temukan berupa admin
            pilihan = 0;
            //printf("\nLogin Berhasil (admin)\n");
            while(pilihan != 3){
                system("CLS");
                user_input = admin();
                switch (user_input)
                {
                    case 1:
                        system("CLS");
                        list_obat(&obat,counter_obat);
                        system("pause");
                        break;
                    
                    case 2:
                        system("CLS");
                        tambah_obat(&obat,&counter_obat);
                        break;

                    case 3:
                        system("CLS");
                        update_jumlah(&obat, counter_obat);
                        break;

                    case 4:
                        system("CLS");
                        hapus_obat(&obat, &counter_obat);
                        break;
                    
                    case 5:
                        system("CLS");
                        pilihan = 3;
                        break;
                    
                    default:
                        system("CLS");
                        printf("\ninput salah silahkan ulangi lagi\n");
                        break;
                }    
            }
            
        //pendaftaran/mengulang
        }else{
            pilihan = 0;
            printf("\nRegister(0) / Retry(1) = ");
            scanf("%d",&pilihan);
            if(pilihan == 0){
                add_akun(&temp,&counter,&akun);
            }else{
                continue;
            }
        }
        pilihan = 0;
        printf("\nKeluar(1) ? : ");//jika memilih 1 maka program akan keluar
        scanf("%d",&pilihan);
        if(pilihan == 1){
            keluar(&obat,&akun,counter_obat,counter,ptr,ptr_obat);
            exit(0);//berfungsi untuk keluar dari program
        }else{
            continue;
        }
    }
}
// membuat file dan menghitung jumlah line dalam file
void count_create(FILE *ptr, int *counter){
    char ch;
    ptr = fopen("akun.txt","r");//membuka file
    if(ptr==NULL){ //if berfungsi untuk mengecek apakah file terbuka
        printf("making file\n");
        fclose(ptr);//menutup file
        fopen("akun.txt","w");
        fclose(ptr);
    }else{
        while(!feof(ptr)){
            ch = fgetc(ptr);
            if(ch == '\n'){
                (*counter)++;
            }
        }
        fclose(ptr);
    }
}

void count_create_obat(FILE *ptr_obat, int *counter_obat){//pass by reference
    char ch;
    ptr_obat = fopen("obat.txt","r");
    if(ptr_obat==NULL){
        printf("making file\n");
        fclose(ptr_obat);
        fopen("obat.txt","w");
        fclose(ptr_obat);
    }else{
        while(!feof(ptr_obat)){//mengecek hingga akhir file
            ch = fgetc(ptr_obat);
            if(ch == '\n'){
                (*counter_obat)++;//menambahkan pada variable pass by reference
            }
        }
        fclose(ptr_obat);
    }
}

// memasukan data akun ke memory akun
void load_data(FILE *ptr, int *counter, ptrakun *load){
    int i=0;
    ptr = fopen("akun.txt","r");
    while(!feof(ptr)){
        fscanf(ptr," %[^\t]\t %[^\t]\t %[^\t]\t%d",(*load+i)->username,(*load+i)->pass,(*load+i)->nama,&(*load+i)->total);
        i++;
    }
}

void load_data_obat(FILE *ptr_obat, int *counter_obat, ptrobat *load){
    int i=0;
    ptr_obat = fopen("obat.txt","r");
    while(!feof(ptr_obat)){
        fscanf(ptr_obat," %[^\t]\t%d\t%d",(*load+i)->nama_obat,&(*load+i)->harga,&(*load+i)->jumlah);
        i++;
    }
}

//halaman login
void login(ptrakun login){
    printf("===========================\n");
    printf("=      Halaman Login      =\n");
    printf("===========================\n");
    printf("\nUsername : ");
    scanf(" %[^\n]",&login->username);
    printf("\npassword : ");
    scanf(" %[^\n]",&login->pass);
}

//mengecek username dan paswoord saat login
int akun_checker(ptrakun *check, ptrakun temp, int maks, int *user){
    int i, flag_user=0, flag_pass=0;
    char admin[10] = "admin";
    if(strcmp(temp->username,"admin")==0 && strcmp(temp->pass,"admin")==0){
        return 2;
    }else{
        for(i=0; i<maks; i++){
            if(strcmp((*check+i)->username, temp->username)==0 && strcmp((*check+i)->pass, temp->pass)==0){
                flag_user = 1;
                *user = i;
                break;
            }
        }

        if(flag_user == 1){
            printf("\nUser Found\n");
            printf("===== Welcome %s =====\n",(*check+(*user))->nama);
            return 1;
        }else{
            printf("\nUser not Found\n");
            printf("Please Regist First\n");
            return 0;
        }
    }  
}

//mendaftarkan akun apabila akun belum ada
void add_akun(ptrakun add, int *lokasi, ptrakun *database){
    printf("============================\n");
    printf("=    Halaman Registrasi    =\n");
    printf("============================\n");
    printf("\nUsername : %s\nPassword : %s",add->username,add->pass);
    printf("\nNama : ");
    scanf(" %[^\n]", add->nama);
    strcpy((*database+(*lokasi))->username,add->username);
    strcpy((*database+(*lokasi))->pass,add->pass);
    strcpy((*database+(*lokasi))->nama,add->nama);
    (*database+(*lokasi))->total = 0;
    *lokasi += 1;
    printf("\nAccount Registered\n\n");
}

//daftar menu untuk user
int menu(){
    int pil = 0;
    printf("============================\n");
    printf("=            Menu          =\n");
    printf("============================\n");
    printf("\n1. List Obat");
    printf("\n2. Beli Obat");
    printf("\n3. Checkout");
    printf("\n4. Logout");
    printf("\n\nPilihan : ");
    scanf("%d",&pil);
    return pil;
}

//daftar menu untuk admin
int admin(){
    int pil = 0;
    printf("=============================\n");
    printf("=         Admin Menu        =\n");
    printf("=============================\n");
    printf("\n1. List Obat");
    printf("\n2. Tambah Obat");
    printf("\n3. Update Jumlah");
    printf("\n4. Hapus Obat");
    printf("\n5. Logout");
    printf("\n\nPilihan : ");
    scanf("%d",&pil);
    return pil;
}

//menambahkan jenis harga dan jumlah obat
void tambah_obat(ptrobat *add, int *lokasi){
    printf("===========================\n");
    printf("=       Tambah Obat       =\n");
    printf("===========================\n");
    printf("\nNama Obat : ");
    scanf(" %[^\n]", (*add+(*lokasi))->nama_obat);
    printf("\nJumlah Obat : ");
    scanf("%d", &(*add+(*lokasi))->jumlah);
    printf("\nHarga Obat : ");
    scanf("%d", &(*add+(*lokasi))->harga);
    *lokasi += 1;
    printf("\nObat Registered\n\n");
}

//menampilkan daftar obat
void list_obat(ptrobat *cetak, int maks){
    int i;
    printf("===========================\n");
    printf("=        list Obat        =\n");
    printf("===========================\n");
    printf("Nama\t\tHarga\t\tJumlah\n\n");
    for(i=0; i<maks; i++){//for berfungsi untuk menampilkan list obat
        printf("%s\t\t%d\t\t%d\n",(*cetak+i)->nama_obat, (*cetak+i)->harga, (*cetak+i)->jumlah);
    }
}

//memperbarui jumlah stock obat
void update_jumlah(ptrobat *update, int maks){
    char obat_temp [32];
    int lokasi, exit = 0;
    while(exit != 1){
        printf("============================\n");
        printf("=    Update Jumlah Obat    =\n");
        printf("============================\n");
        printf("Masukan Nama Obat : ");
        scanf(" %[^\n]", obat_temp);
        lokasi = cari_obat(&*update, obat_temp, maks);
        if(lokasi == -1){
            printf("obat tidak ditemukan\n");
        }else{
            printf("Jumlah Obat %s : ", (*update+lokasi)->nama_obat);
            scanf("%d",&(*update+lokasi)->jumlah);
        }
        printf("Keluar(1) : ");
        scanf("%d",&exit);
    }
}

//mencari obat berdasarkan nama dengan return lokasi pada memory
int cari_obat(ptrobat *cari, char nama[32], int maks){
    int i, flag = 0, lokasi = 0;
    for(i=0; i<maks; i++){//for yang berfungsi untuk membatasi pengecekan
        if(strcmpi((*cari+i)->nama_obat,nama)==0){
            flag = 1;
            lokasi = i;
            break;
        }
    }
    if(flag == 1){
        return lokasi;
    }else{
        return -1;
    }
}

//menghapus obat dari list obat
void hapus_obat(ptrobat *hapus, int *maks){
    char obat_temp [32];
    int lokasi, exit = 0, i;
    while(exit != 1){
        printf("===========================\n");
        printf("=        Hapus Obat       =\n");
        printf("===========================\n");
        printf("Masukan Nama Obat : ");
        scanf(" %[^\n]", obat_temp);
        lokasi = cari_obat(&*hapus, obat_temp, *maks);
        if(lokasi == -1){
            printf("obat tidak ditemukan\n");
        }else{
            for(i=lokasi; i<(*maks)-1; i++){
                strcpy((*hapus+i)->nama_obat,(*hapus+(i+1))->nama_obat);
                (*hapus+i)->harga = (*hapus+(i+1))->harga;
                (*hapus+i)->jumlah = (*hapus+(i+1))->jumlah;
            }
            (*maks)--;
        }
        printf("Keluar(1) : ");
        scanf("%d",&exit);
    }
}

//user membeli obat
void beli_obat(ptrobat *beli, ptrakun *pembeli, int current, int max_obat){
    char temp_obat[32];
    int jenis_obat, jumlah_obat, pilihan = 0;
    while(pilihan != 1){
        list_obat(beli,max_obat);
        printf("\n================== Beli Obat ===================\n\n");
        printf("Nama obat : ");
        scanf(" %[^\n]", temp_obat);
        jenis_obat = cari_obat(beli,temp_obat,max_obat);
        if(jenis_obat != -1){
            printf("Jumlah obat : ");
            scanf("%d", &jumlah_obat);
        }else{
            printf("obat tidak di temukan\n");
        }
        if((*beli+jenis_obat)->jumlah < jumlah_obat){
            printf("obat tidak cukup\n");
            continue;
        }else{
            (*beli+jenis_obat)->jumlah -= jumlah_obat;
            (*pembeli+current)->total += ((*beli+jenis_obat)->harga)*jumlah_obat;
        }
        printf("Keluar(1) ? ");
        scanf("%d",&pilihan);
    }
}

//checkout belanjaan
void checkout(ptrakun *checkout, int current){
    printf("===========================\n");
    printf("=        Checkout         =\n");
    printf("===========================\n");
    printf("\nNama : %s\n",(*checkout+current)->nama);
    printf("\nTotal : %d\n", (*checkout+current)->total);
}

void keluar(ptrobat *simpan_obat, ptrakun *simpan_akun, int maks_obat, int maks_akun, FILE *ptr, FILE *ptr_obat){
    int i;
    ptr = fopen("akun.txt","w");
    for(i = 0; i < maks_akun; i++){
        fprintf(ptr,"%s\t%s\t%s\t%d\n",(*simpan_akun+i)->username,(*simpan_akun+i)->pass,(*simpan_akun+i)->nama,(*simpan_akun+i)->total);
    }
    fclose(ptr);
    Sleep(1000);
    ptr_obat = fopen("obat.txt", "w");
    for(i = 0; i < maks_obat; i++){
        fprintf(ptr_obat,"%s\t%d\t%d\n",(*simpan_obat+i)->nama_obat,(*simpan_obat+i)->harga,(*simpan_obat+i)->jumlah);
    }
    fclose(ptr_obat);
}