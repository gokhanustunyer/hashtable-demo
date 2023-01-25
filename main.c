#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#define SIZE 12

typedef struct Node Node;
typedef struct hashTable hashTable;
struct Node{
    /*
        Hashtable'da cakisma soruna karsin
        Hastable'in dizisinde kullanmak uzere
        default Linkedlist yapisi
    */
    Node  *  next;
    float    data;
};

struct hashTable{
    /*
        Hashtable struct yapisinde node array'imizi
        ve uzunluk bilgisini saklayacagiz
    */
    Node  *  arr[SIZE];
    size_t        size;
};

// Goruntuleme Fonksiyonlari
void printTable(hashTable ht);
void printNode(Node * node);

int getNodeIndex(Node * node, float value);

// Yapici Fonksiyonlar
hashTable createTable(size_t size);

// Islem Fonksiyonlari
int find(hashTable * ht, float data);
int push(hashTable * ht, float data);
int delete(hashTable * ht, float data);
int hash(float data);

// Bellekte Node * kadar dinamik yer tahsis eden Fonksiyon
Node * allocate();


int main(){
    // CreateTable metoduyla ici doldurulmus bir HashTable aliyoruz
    hashTable ht = createTable(SIZE);
    int     i;
    // Rastgele degerler uretmek icin
    srand(time(0));
    int test[12];

    // Test verimizi olusturuyoruz ...
    for(i=0;i<12;i++){
        test[i] = (rand() % 100) + 1;
    }

    // Push fonksiyonunu burada test verilerimizde cagirarak array'i dolduruyoruz ...
    for(i=0;i<12;i++)
        push(&ht, test[i]);


    printf("\n");
    // Tum diziyi goruntuluyoruz ...
    printTable(ht);

    printf("Elemanlar Temizleniyor ...");

    // Delete fonksiyonu ile verileri ekledikten ve goruntuledikten sonra verileri tek tek siliyoruz ...
    for(i=0;i<12;i++)
        delete(&ht, test[i]);
    
    printf("\n");
    // Diziyi tekrardan goruntuluyoruz ...
    printTable(ht);
    

    return 1;
}


int find(hashTable * ht, float data){
    /*
        @brief -> Verilen HashTable'da veri arar
        @param -> hashTable * ht hashTable pointer (call by reference)
                float data
        @return -> -1,int index -> Veri bulunamadiysa -1 bulunduysa index degeri
    */

    // index -> hash'den donen deger isFounded -> o index'de verilen data var mi?
    int     index, isFounded = 0;
    Node                   *temp;
    // *temp -> verilen index'deki node degiskeninde verilen data'yi aramak icin donecegiz

    // hash fonksiyonu ile index alimi
    index = hash(data);
    // temp degeri bulunan index ile baslatiliyor
    temp = ht -> arr[index];
    // verilen data temp node'unca araniyor
    while(!isFounded && temp != NULL){
        // data node daki veri ile eslesiyorsa isFounded true olarak guncelleniyor
        if (temp -> data == data) isFounded = 1;
        // temp bir sonraki node'a geciyor
        temp = temp -> next;
    }
    // Deger listede bulunmussa index bulunmadiysa -1 donuyor
    return (isFounded) ? index : -1;
}


int push(hashTable * ht, float data){
    /*
        @brief -> Verilen HashTable'a veri ekler
        @param -> hashTable * ht hashTable pointer (call by reference)
                float data
        @return -> 0, 1 -> Basarili veya basarisiz
    */
    // Hashtable'daki array dolu ile cikis yapiliyor 0 -> Islem Basarisiz
    if (ht -> size == SIZE){ puts("Table is full"); return 0; }
    // Verilecek data ile doldurulacak yeni node
    Node * pushNode;
    // Elemani array icinde yerlestirebilmek icin index
    int       index;
    // Olusturulan Node'u kullanmak icin bellekte yer tahsis ediyoruz
    pushNode = allocate();
    // Veriyi node degiskenine atiyoruz
    pushNode -> data = data;
    // Sonraki degerini NULL yaparak node'un ucunu kapatiyoruz
    pushNode -> next = NULL;
    // Hangi index'e yerlestirecegimizi hash fonksiyonu ile aliyoruz
    index = hash(data);
    // Degerin next'ini array'in gelen index'ine esitliyoruz
    pushNode -> next = ht -> arr[index];
    // Array'in o indexini yeni node'a esitliyoruz ve yeni elemani o node'un basina eklemis oluyoruz
    ht -> arr[index] = pushNode;
    // Hashtable'in size degiskenini bir arttiriyoruz
    ht -> size ++;
    // 1 -> Islem basarili
    return 1;
}


int delete(hashTable * ht, float data){
    /*
        @brief -> Verilen HashTable'dan veri siler
        @param -> hashTable * ht hashTable pointer (call by reference)
                float data
        @return -> 0, 1 -> Basarili veya basarisiz
    */
    // row -> hash den donen index, col -> node'dan gelen kolon degeri
    int         row,    col,i;
    // *delete -> bellekten silinecek yer, *iter -> col degerine kadar gezecek degisken
    Node    *delete,    *iter;
    // row degerinin doldurulmasi
    row = find(ht, data);
    // Eleman listede yoksa fonksiyondan cikis yapiliyor
    if (row == -1){
        printf("\n%.1f degeri listede bulunamadi", data);
        return 0;
    }
    // Donecek degiskenimiz index'deki ilk eleman ile basliyor
    iter = ht -> arr [ row ];
    // Kolon index'i alttaki fonksiyon ile aliniyor
    col = getNodeIndex(iter, data);
    // iter degiskeni donen kolon index'inin bir arkasina kadar gidiyor
    for(i=0;i<col-1;i++)
        iter = iter -> next;
    // Ilk elemansa arraydaki o index bosalacak
    if (col == 0){
        delete = iter;
        ht -> arr [ row ] = (iter -> next == NULL) ? NULL : iter -> next;
    }
    // Ilk eleman degilse aktifi sonrakinin onundekine esitliyerek
    // Node'daki Zincirden veriyi cikartmis oluyoruz
    else{
        delete = iter -> next;
        iter -> next = iter -> next -> next;
    }
    // Bosa dusen (silinen) degerin bellekten temizlenmesi
    free(delete);
    // Islem basariyla tamamlandi
    return 1;
}


int hash(float data){
    /*
        @brief -> Verilen data degerine ve constant olan SIZE 
                  degerine gore key degeri gonderir
        @param -> float data
        @return -> int index -> array index
    */
    // float verilen data'nin int tipindeki key'e cevrilmesi
    int hash = 1479 + (int)data;
    // Gelen hash degerinin array boyutuna indirgenmesi
    return hash % SIZE;
}

int getNodeIndex(Node * node, float value){
    /*
        @brief -> Verilen node da verilen degeri arar
        @param -> Node * node node pointer (call by reference), float data
        @return -> int index -> index degeri (Bu fonksiyondaki kullanimi icin
                   verilen value parametresi her zaman node icinde olacagi icin
                   bu durumu kontrol eden bir kosul ekleme geregi duymadim)
    */
    // Geri dondurecegimiz kolon index'i degeri
    int counter = 0;
    // Veriyi arayacagimiz iter degiskeni
    Node      * iter;
    // node ile iter'i baslatiyoruz
    iter   =   node;
    // deger gelene kadar node'da degeri ariyoruz
    while(iter != NULL && iter -> data != value){
        iter = iter -> next;
        counter ++;
    }
    // Index donduruluyor
    return counter;
}

void printTable(hashTable ht){
    /*
        @brief -> Verilen HashTable'daki verileri yazdirir
        @param -> hashTable * ht hashTable pointer (call by reference)
        @return -> void
    */
    int     i;
    printf("------------\t------------\t------------\n");
    printf("HASH TABLE\n");
    // Node listesinin dondurulmesi
    for (i=0;i<ht.size;i++){
        // Her bir node'un yazilmasi
        printf("index: %d\t\t", i);
        printNode(ht.arr[i]);
        // Node bittikten sonra alt satira gecis
        printf("\n");
    }
    printf("------------\t------------\t------------\n");
}

void printNode(Node * node){
    /*
        @brief -> Verilen Node'daki verileri yazdirir
        @param -> Node * node node pointer (call by reference)
        @return -> void
    */
    // node'u alip sonuna kadar yadiriyor
    while (node != NULL){
        printf("%.2f\t-> ", node -> data);
        node = node -> next;
    }
}

hashTable createTable(size_t size){
    /*
        @brief -> Hashtable olusturup verilen size'a kadar doldurur
                  Bu fonksiyonu Constructor olarak dusunebiliriz
        @param -> size_t size
        @return -> hashTable
    */
    int        i;
    // Doldurulacak HashTable nesnesi
    hashTable ht;
    ht.size =  0;
    // Node arraydaki degerler NULL olarak ataniyor
    for(i=0;i<size;i++)
        ht.arr[i] = NULL;
    // Doldurulan HashTable geri donduruluyor
    return ht;
}

Node* allocate(){
    /*
        @brief -> Bellekte Node kadar yer acar ve
                  pointer olarak geri dondurur
        @param -> void
        @return -> Node *
    */
    return (Node*)malloc(sizeof(Node));
}

