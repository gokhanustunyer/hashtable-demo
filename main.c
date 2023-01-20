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
    
    // Rastgele degerler uretmek icin
    srand(time(0));
    int test[12];

    // Test verimizi olusturuyoruz ...
    for(int i=0;i<12;i++){
        test[i] = (rand() % 100) + 1;
    }

    // Push fonksiyonunu burada test verilerimizde cagirarak array'i dolduruyoruz ...
    for(int i=0;i<12;i++)
        push(&ht, test[i]);


    printf("\n");
    // Tum diziyi goruntuluyoruz ...
    printTable(ht);

    printf("Elemanlar Temizleniyor ...");

    // Delete fonksiyonu ile verileri ekledikten ve goruntuledikten sonra verileri tek tek siliyoruz ...
    for(int i=0;i<12;i++)
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
    int      c=-1,  isFounded = 0;
    int     index,           *col;
    Node                    *temp;

    index = hash(data);
    temp = ht -> arr[index];
    while(!isFounded && temp != NULL){
        if (temp -> data == data) isFounded = 1;
        temp = temp -> next;
        c++;
    }
    return (isFounded) ? index : -1;
}


int push(hashTable * ht, float data){
    /*
        @brief -> Verilen HashTable'a veri ekler
        @param -> hashTable * ht hashTable pointer (call by reference)
                float data
        @return -> 0, 1 -> Basarili veya basarisiz
    */
    if (ht -> size == SIZE){ puts("Table is full");return 0;}

    Node * pushNode;
    int       index;
    pushNode = allocate();
    pushNode -> data = data;
    pushNode -> next = NULL;

    index = hash(data);
    pushNode -> next = ht -> arr[index];
    ht -> arr[index] = pushNode;
    ht -> size ++;
    return 1;
}


int delete(hashTable * ht, float data){
    /*
        @brief -> Verilen HashTable'dan veri siler
        @param -> hashTable * ht hashTable pointer (call by reference)
                float data
        @return -> 0, 1 -> Basarili veya basarisiz
    */
    int         row,      col;
    Node    *delete,    *iter;

    row = find(ht, data);
    if (row == -1){
        printf("\n%.1f degeri listede bulunamadi", data);
        return 0;
    }

    iter = ht -> arr [ row ];
    col = getNodeIndex(iter, data);
    for(int i=0;i<col-1;i++)
        iter = iter -> next;
    
    if (col == 0){
        delete = iter;
        ht -> arr [ row ] = (iter -> next == NULL) ? NULL : iter -> next;
    }
    else{
        delete = iter -> next;
        iter -> next = iter -> next -> next;
    }
    free(delete);
    return 1;
}


int hash(float data){
    /*
        @brief -> Verilen data degerine ve constant olan SIZE 
                  degerine gore key degeri gonderir
        @param -> float data
        @return -> int index -> array index
    */
    int hash = 1479 + (int)data;
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
    int counter = 0;
    Node      * iter;

    iter   =   node;
    while(iter != NULL && iter -> data != value){
        iter = iter -> next;
        counter ++;
    }
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
    for (i=0;i<ht.size;i++){
        printf("index: %d\t\t", i);
        printNode(ht.arr[i]);
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
    hashTable ht;
    ht.size = 0;
    for(int i=0;i<size;i++){
        ht.arr[i] = NULL;
    }
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

