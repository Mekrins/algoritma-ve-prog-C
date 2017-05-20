#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#define MAX_KARAKTER 21
#define BOS_KAYIT -1
#define MAX_KAYIT 99

int menu_goruntule();
void skor_girisi(FILE *,FILE *);
void takimAdiyla_arama(FILE *,FILE *);
void takimKoduyla_arama(FILE *,FILE *);
void puanCetveli(FILE *);
void enCok_gol(FILE *);
void hafta_macSonucu(FILE *);
void golKrali(FILE *,FILE *);
void dep_enCok_gol(FILE *);
void lig_ici_transfer(FILE *,FILE *);
void lig_disi_transfer(FILE *,FILE *);
void lig_disi_yeni_futbolcu(FILE *,FILE *);
void goruntule(FILE *,FILE *);

typedef struct takim{
    int kodu;
    char adi[MAX_KARAKTER];
    int galibiyet;
    int beraberlik;
    int maglubiyet;
    int atilan_gol;
    int yenilen_gol;
}Takim;

typedef struct futbolcu{
    int forma_no;
    char ad_soyad[MAX_KARAKTER];
    int attigi_gol;
}Futbolcu;

int main()
{
    FILE *takimlar,*futbolcular;
    int secenek;

    if((takimlar=fopen("takimlar.dat","rb+"))==NULL){//takimlar dosyasý hem içine yeni veriler yazýlacaðý için, hemde içindeki veriler okunacaðý için ve takimlar dosyasý doðrudan eriþimli dosya olduðu için rb+ formatýnda açýlýyor.
        printf("Takimlar dosyasi acilamadi!");//Dosyanýn açýlýp açýlamadýðý "NULL" ile belirleniyor.Eðer dosya açýlamayýp NULL çevirirse uyarý verilriyor.
    }
    else{
        if((futbolcular=fopen("futbolcular.dat","rb+"))==NULL){//takimlar dosyasý ile ilgili olay ayný þekilde futbolcular dosyasýnada uygulanýyor.
            printf("Futbolcular dosyasi acilamadi!");
        }
        else{
                do{//Menü do while ve switch case kullanýlarak oluþturuluyor.
                    secenek=menu_goruntule();
                    //system("cls");
                    switch(secenek){//Her seçeneðe özel fonksiyonlar numaralarýna göre aþýðadaki gibi yazýlýyor.
                        case 1:
                            skor_girisi(takimlar,futbolcular);
                            break;
                        case 2:
                            takimAdiyla_arama(takimlar,futbolcular);
                            break;
                        case 3:
                            takimKoduyla_arama(takimlar,futbolcular);
                            break;
                        case 4:
                            puanCetveli(takimlar);
                            break;
                        case 5:
                            enCok_gol(takimlar);
                            break;
                        case 6:
                            dep_enCok_gol(takimlar);
                            break;
                        case 7:
                            hafta_macSonucu(takimlar);
                            break;
                        case 8:
                            golKrali(takimlar,futbolcular);
                            break;
                        case 9:
                            lig_disi_yeni_futbolcu(takimlar,futbolcular);
                            break;
                        case 10:
                            lig_ici_transfer(takimlar,futbolcular);
                            break;
                        case 11:
                            lig_disi_transfer(takimlar,futbolcular);
                            break;
                    }
                } while(secenek!=12);//Kullanýcý 12 sayýsýný girmedikçe menü tekrar ve tekrara gösteriliyor.
        }
        fclose(takimlar);
        fclose(futbolcular);
    }
    return 0;
}
int menu_goruntule(){

    int secim;

    printf("\n");
    printf("   1. Biten mac skorlarinin girisi\n");
    printf("   2. Bir takimin su anki durumunun ve futbolcularinin listesi\n");
    printf("   3. Bir takimin su anki durumunun ve oynadigi maclarin listesi\n");
    printf("   4. Puan cetvelinin listesi\n");
    printf("   5. En cok gol atan ve en cok gol yiyen takimlarin listesi\n");
    printf("   6. Deplasmanda en cok gol atan takimin listesi\n");
    printf("   7. Bir haftanin mac skorlarinin listesi\n");
    printf("   8. Ligde en cok gol atan futbolcunun bilgileri\n");
    printf("   9. Bir takima lig disindan yeni bir futbolcunun transfer edilmesi\n");
    printf("   10. Bir takimdaki bir fulbolcunun ligdeki baska bir takima transfer edilmesi\n");
    printf("   11. Bir takimdaki futbolcunun lig disindaki bir takima transfer edilmesi\n");
    printf("   12. Cikis \n");
    printf("\n");
    do {
        printf("\nSeciminizi giriniz: ");
        scanf("%d", &secim);
    } while(secim<1 || secim>13);

    return secim;
}

void skor_girisi(FILE *takimlar,FILE *futbolcular){

    FILE *skorlar;
    Futbolcu bir_futbolcu;// Her fonsiyonda olduðu gibi geçici bir struct oluþtuluyor.
    Takim bir_takim;//Ve yine her fonksiyonda olduðu gibi geçici bir struct yapýsý oluþturuluyor.
    int hafta_no,evSahibi_Kodu,evSahibi_atilanGol,gecici;
    int futbolcu_anh,misafir_Kodu,misafir_atilanGol,no,gol,bulundu=0;
    char devam;

    if((skorlar=fopen("skorlar.txt","a+"))==NULL){
        printf("Takimlar dosyasi acilamadi!");
    }
    else{
        do{
            printf("Hafta numarasini giriniz: ");
            scanf("%d",&hafta_no);
            if(hafta_no<=0)//Eðer hafta numarasý 0'dan küçükse bir uyarý veriliyor ve numaranýn tekrar girilmesi isteniyor.
                printf("Hafta numarasi 0'dan buyuk olmalidir!\n");
        }while(hafta_no<=0);

        do{
            gecici=0;
            do{//
                printf("Ev sahibi takimin kodunu ve attigi gol sayisini bir bosluk birakarak giriniz: ");// Ev sahibi takimin kodu ve attýðý gol sayýsý isteniyor.
                scanf("%d %d",&evSahibi_Kodu,&evSahibi_atilanGol);
                rewind(takimlar);//Dosya pozisyon göstericisi her tekrarda en baþa taþýnýyor aksi takdirde yeni döngüdeki iþlemler, eski döngünün dosya pozisyon göstericisini býraktýðý yerde baþlýyacaðý için hatalar ortaya çýkar.
                while(!feof(takimlar)){//takimlar dosyasý en baþtan en sona kadar aranýyor ve eðer aranan deðer bulunursa bulundu deðiþkenine 1 atýyor.
                    fread(&bir_takim,sizeof(Takim),1,takimlar);
                    if(bir_takim.kodu==evSahibi_Kodu)
                        bulundu=1;
                }
                if(bulundu!=1)
                    printf("Girilen kod numarasiyla takim bulunmamaktadir.Lutfen degerleri tekrar giriniz.\n");
            }while(bulundu!=1);//While döngüsü bulundu 1 olana kadar tekrar tekrar dönüyor.

            if(evSahibi_atilanGol!=0){
                do{
                    bulundu=0;
                    do{
                        printf("Gol atan futbolcunun forma numarasini ve attigi gol sayisini bir bosluk birakarak giriniz: ");
                        scanf("%d %d",&no,&gol);
                        if(gol==0){
                            break;
                        }
                        rewind(futbolcular);//Dosya pozisyon göstericisi her tekrarda en baþa taþýnýyor aksi takdirde yeni döngüdeki iþlemler, eski döngünün dosya pozisyon göstericisini býraktýðý yerde baþlýyacaðý için hatalar ortaya çýkar.
                        while(!feof(futbolcular)){//futbolcular dosyasý en baþtan en sona kadar aranýyor ve eðer aranan deðer bulunursa bulundu deðiþkenine 1 atýyor.
                            fread(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);
                            if(bir_futbolcu.forma_no==no)
                                bulundu=1;
                        }
                        if(bulundu!=1)
                            printf("Girilen forma numarasiyla oyuncu bulunmamaktadir.Lutfen degerleri tekrar giriniz.\n");
                    }while(bulundu!=1);//While döngüsü bulundu 1 olana kadar tekrar tekrar dönüyor.

                    futbolcu_anh=(evSahibi_Kodu*100)+no;//Golü atan futbolcunun futbolcular dosyasýndaki anahtar kodu hesaplanýyor.
                    fseek(futbolcular,(futbolcu_anh-1)*sizeof(Futbolcu),SEEK_SET);//Dosya pozisyon göstericisi futbolcunun anahat koduna getiriliyor.
                    fread(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);//O yerdeki struct, geçici olarak oluþturulan struct üzerine yazýlýyor.
                    bir_futbolcu.attigi_gol=gol+bir_futbolcu.attigi_gol;//Geçici structa futbolcunun attýðý gol ekleniyor.
                    gecici+=gol;//Geçici deðiþkenine toplamda atýlan goller atanýyor.
                    fseek(futbolcular,(futbolcu_anh-1)*sizeof(Futbolcu),SEEK_SET);
                    fwrite(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);//Deðiþtirilen struct, futbolcunun anahtar kodundaki yere yazdýrýlýyor.
                    //rewind(futbolcular);

                    if(gecici>evSahibi_atilanGol){//Futbolcularýn attýðý goller takýmýn ilk kez istenilen atýlan gol sayýsýn geçerse bir uyarý veriliyor ve geçici deðiþkeni sýfýrlanýyor.
                        printf("Toplam gol sayisini asan degerler girdiniz.Lutfen degerleri tekrar girin.\n");
                        gecici=0;
                    }
                }while(gecici<evSahibi_atilanGol);
            }

            do{//Yukarda yapýlan tüm iþlmeler ayný þekilde misfir takim için tekrar uygulanýyor.
                gecici=0;
                printf("Misafir takimin kodunu ve attigi gol sayisini bir bosluk birakarak giriniz: ");
                scanf("%d %d",&misafir_Kodu,&misafir_atilanGol);
                if(evSahibi_Kodu==misafir_Kodu){
                        printf("Ev sahibi takimin kodu ile misafir takimin kodu ayni olamaz.Lutfen kodu tekrar girin.\n");
                }
            }while(evSahibi_Kodu==misafir_Kodu);

            if(misafir_atilanGol!=0){
                do{
                    printf("Gol atan futbolcunun forma numarasini ve attigi gol sayisini bir bosluk birakarak giriniz: ");
                    scanf("%d %d",&no,&gol);
                    if(bir_futbolcu.attigi_gol==0){
                        break;
                    }
                    futbolcu_anh=(misafir_Kodu*100)+no;
                    fseek(futbolcular,(futbolcu_anh-1)*sizeof(Futbolcu),SEEK_SET);
                    fread(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);
                    bir_futbolcu.attigi_gol=gol+bir_futbolcu.attigi_gol;
                    gecici+=gol;
                    fseek(futbolcular,(futbolcu_anh-1)*sizeof(Futbolcu),SEEK_SET);
                    fwrite(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);
                    //rewind(futbolcular);

                    if(gecici>misafir_atilanGol){
                        printf("Toplam gol sayisini asan degerler girdiniz.Lutfen degerleri tekrar girin.\n");
                        gecici=0;
                    }
                }while(gecici<misafir_atilanGol);
            }

            //Ýki takim arasýndaki galibiye, maðlubiyet ve berabarlik deðerleri aþaðýdaki if yapýlarý ile bulunuyor.
            if(evSahibi_atilanGol>misafir_atilanGol){//Ev sahibi takimin attýðý gol fazla ise galibiyeti bir artýyor, diðer takýmýn maðlubiyet deðeri artýyor.
                fseek(takimlar,(evSahibi_Kodu-1)*sizeof(Takim),SEEK_SET);//Ev sahibi takimin dosyada bulunduðu yere gidiliyor.
                fread(&bir_takim,sizeof(Takim),1,takimlar);//O yerdeki deðerler geçici struct üzerine okunuyor.

                bir_takim.galibiyet++;                         //
                bir_takim.atilan_gol+=evSahibi_atilanGol;      //Bilgiler yenileniyor.
                bir_takim.yenilen_gol+=misafir_atilanGol;      //

                fseek(takimlar,(evSahibi_Kodu-1)*sizeof(Takim),SEEK_SET);//Ev sahibi takimin dosyada bulunduðu yere gidiliyor.
                fwrite(&bir_takim,sizeof(Takim),1,takimlar);//Structaki yenilenen deðerler dosyadaki takimin bulunguðu yere tekarar yazýlýyor

                //Yukarda yapýlan iþlemler ayný þekilde aþaðýdaki yerlerede yapýlýyor.
                fseek(takimlar,(misafir_Kodu-1)*sizeof(Takim),SEEK_SET);//
                fread(&bir_takim,sizeof(Takim),1,takimlar);

                bir_takim.maglubiyet++;
                bir_takim.atilan_gol+=misafir_atilanGol;
                bir_takim.yenilen_gol+=evSahibi_atilanGol;

                fseek(takimlar,(misafir_Kodu-1)*sizeof(Takim),SEEK_SET);
                fwrite(&bir_takim,sizeof(Takim),1,takimlar);
            }
            else{
                if(evSahibi_atilanGol<misafir_atilanGol){//Misafir takýmýn attýðý gol daha falzla ise galibiyet deðeri bir artýyor, diðer takýmýn maðlubiyet deðeri bir artýyor.
                    fseek(takimlar,(misafir_Kodu-1)*sizeof(Takim),SEEK_SET);
                    fread(&bir_takim,sizeof(Takim),1,takimlar);

                    bir_takim.galibiyet++;
                    bir_takim.atilan_gol+=misafir_atilanGol;
                    bir_takim.yenilen_gol+=evSahibi_atilanGol;

                    fseek(takimlar,(misafir_Kodu-1)*sizeof(Takim),SEEK_SET);
                    fwrite(&bir_takim,sizeof(Takim),1,takimlar);

                    fseek(takimlar,(evSahibi_Kodu-1)*sizeof(Takim),SEEK_SET);
                    fread(&bir_takim,sizeof(Takim),1,takimlar);

                    bir_takim.maglubiyet++;
                    bir_takim.atilan_gol+=evSahibi_atilanGol;
                    bir_takim.yenilen_gol+=misafir_atilanGol;

                    fseek(takimlar,(evSahibi_Kodu-1)*sizeof(Takim),SEEK_SET);
                    fwrite(&bir_takim,sizeof(Takim),1,takimlar);
                }
                else{//Eðer iki takýmýn attýðý goller bir birine eþitse iki takýmýnda beraberlik deðerleri bir artýyor.
                    fseek(takimlar,(misafir_Kodu-1)*sizeof(Takim),SEEK_SET);
                    fread(&bir_takim,sizeof(Takim),1,takimlar);

                    bir_takim.beraberlik++;
                    bir_takim.atilan_gol+=misafir_atilanGol;
                    bir_takim.yenilen_gol+=evSahibi_atilanGol;

                    fseek(takimlar,(misafir_Kodu-1)*sizeof(Takim),SEEK_SET);
                    fwrite(&bir_takim,sizeof(Takim),1,takimlar);

                    fseek(takimlar,(evSahibi_Kodu-1)*sizeof(Takim),SEEK_SET);
                    fread(&bir_takim,sizeof(Takim),1,takimlar);

                    bir_takim.beraberlik++;
                    bir_takim.atilan_gol+=evSahibi_atilanGol;
                    bir_takim.yenilen_gol+=misafir_atilanGol;

                    fseek(takimlar,(evSahibi_Kodu-1)*sizeof(Takim),SEEK_SET);
                    fwrite(&bir_takim,sizeof(Takim),1,takimlar);
                }
            }
            fprintf(skorlar,"%d %d %d %d %d\n",hafta_no,evSahibi_Kodu,misafir_Kodu,evSahibi_atilanGol,misafir_atilanGol);

            printf("Biten baska mac var mi?(E/H): ");//Kullanýcýdan devam edilip edilmemesi için E(evet) yada H(hayýr) harflerinden birisinin girilmesi isteniyor.
            fflush(stdin);
            scanf("%c",&devam);
            printf("\n");
        }while(devam=='E' || devam=='e');//Kullanýcý e yada E harfini girdikçe döngü dönmeye devam edicek.
    }
    fclose(skorlar);//Dosya kapatýlýyor.
}

void takimAdiyla_arama(FILE *takimlar,FILE *futbolcular){

    Takim bir_takim;
    Futbolcu bir_futbolcu;
    int topOynanan_mac=0,futbolcu_anh,i=0,bulundu=0,averaj,puan;
    char aranan_takimAdi[MAX_KARAKTER];

    printf("Bilgilerini gormek istediginiz takimin adini giriniz: ");
    fflush(stdin);
    scanf("%s",aranan_takimAdi);
    rewind(takimlar);//Dosya pozisyon göstericisi  en baþa getiriliyor.
    while(!feof(takimlar)){//Takilar dosyasý baþtan sona taranýyor.
        fread(&bir_takim,sizeof(Takim),1,takimlar);
        if(bir_takim.kodu!=BOS_KAYIT){
            if(strcmp(aranan_takimAdi,bir_takim.adi)==0){//Aranan takim adi ve geçici structaki takim adi karþýlaþtýrýlýyor, eðer iki ad eþitse 0 sayýsý döndürüyor.
                topOynanan_mac=bir_takim.galibiyet+bir_takim.maglubiyet+bir_takim.beraberlik;
                averaj=bir_takim.atilan_gol-bir_takim.yenilen_gol;
                puan=bir_takim.galibiyet*3+bir_takim.beraberlik;
                printf("     Takim Adi      OO GG BB MM AA YY AVR Puan\n");
                printf("     -------------- -- -- -- -- -- -- --- ----\n");
                printf("     %-14s%3d%3d%3d%3d%3d%3d%3d%5d\n",bir_takim.adi,topOynanan_mac,bir_takim.galibiyet,bir_takim.beraberlik,bir_takim.maglubiyet,bir_takim.atilan_gol,bir_takim.yenilen_gol,averaj,puan);

                printf("\n     Futbolcu Bilgileri:\n");
                printf("     Forma No. Ad Soyad          AA\n");
                printf("     --------- ----------------- --\n");
                for(i=1;i<MAX_KAYIT;i++){
                    futbolcu_anh=(bir_takim.kodu*100)+i;
                    fseek(futbolcular,(futbolcu_anh-1)*sizeof(Futbolcu),SEEK_SET);
                    fread(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);
                    if(bir_futbolcu.forma_no!=BOS_KAYIT)
                        printf("         %-6d%-18s%d\n",bir_futbolcu.forma_no,bir_futbolcu.ad_soyad,bir_futbolcu.attigi_gol);
                }
                bulundu=1;
                break;;//Takim bulundu, ayni isimli baska takim olmayacagi icin arama bitirildi.
            }

        }
    }
    if (bulundu==0) printf("Bilgilerini gormek istediginiz takim bu ligde degil!\n");
    rewind(takimlar);
    rewind(futbolcular);
}

void takimKoduyla_arama(FILE *takimlar,FILE *futbolcular){

    int aranan_kod,topOynanan_mac,averaj,puan;
    int hafta_no,evSahibi_kod,misafir_kod,evSahibi_atilanGol,misafir_atilanGol;
    Takim bir_takim;
    FILE *skorlar;

    printf("Bilgilerini gormek istediginiz takimin kodunu giriniz: ");
    scanf("%d",&aranan_kod);

    fseek(takimlar,(aranan_kod-1)*sizeof(Takim),SEEK_SET);
    fread(&bir_takim,sizeof(Takim),1,takimlar);
    if(bir_takim.kodu==BOS_KAYIT){//Eðer aranan kodda boþ kayýt varsa fonsiyon uyarý verip boþ deðer döndürüyor.
        printf("%d koduyla takim bulunmamaktadir!\n",aranan_kod);
        return;
    }
    topOynanan_mac=bir_takim.galibiyet+bir_takim.maglubiyet+bir_takim.beraberlik;//Toplam oynanan maç hesaplanýyor.

    if((skorlar=fopen("skorlar.txt","r"))==NULL){//skorlar dosyasýnda sadece okuma yapýlacaðý için ve binary yapýda dosya olmadýðý için r modunda açýlýyor.
        printf("Takimlar dosyasi acilamadi!");
    }
    else{
        averaj=bir_takim.atilan_gol-bir_takim.yenilen_gol;//Averaj hesaplanýyor.
        puan=bir_takim.galibiyet*3+bir_takim.beraberlik;//Puan hesaplanýyor.

        printf("     Takim Adi     OO GG BB MM AA YY AVR Puan\n");
        printf("     ------------- -- -- -- -- -- -- --- ----\n");
        printf("     %-14s%2d%3d%3d%3d%3d%3d%3d%5d\n",bir_takim.adi,topOynanan_mac,bir_takim.galibiyet,bir_takim.beraberlik,bir_takim.maglubiyet,bir_takim.atilan_gol,bir_takim.yenilen_gol,averaj,puan);

        printf("\nOynadigi maclarin skorlari:\n");
        printf("          Ev Sahibi               Misafir\n");
        printf("Hafta Takim Adi        AA  AA Takim Adi\n");
        printf("----- ---------------- --  -- ------------------\n");

        fscanf(skorlar,"%d %d %d %d %d",&hafta_no,&evSahibi_kod,&misafir_kod,&evSahibi_atilanGol,&misafir_atilanGol);// Deðerler dosyadan okunuyor.
        while(!feof(skorlar)){//Dosya baþtan sona doðru aranýyor.
            if(aranan_kod==evSahibi_kod){
                fseek(takimlar,(aranan_kod-1)*sizeof(Takim),SEEK_SET);
                fread(&bir_takim,sizeof(Takim),1,takimlar);
                printf("  %-4d%-17s%d",hafta_no,bir_takim.adi,evSahibi_atilanGol);
                fseek(takimlar,(misafir_kod-1)*sizeof(Takim),SEEK_SET);
                fread(&bir_takim,sizeof(Takim),1,takimlar);
                printf("   %-2d %s\n",misafir_atilanGol,bir_takim.adi);
                fscanf(skorlar,"%d %d %d %d %d",&hafta_no,&evSahibi_kod,&misafir_kod,&evSahibi_atilanGol,&misafir_atilanGol);
            }
            else{
                if(aranan_kod==misafir_kod){
                    fseek(takimlar,(evSahibi_kod-1)*sizeof(Takim),SEEK_SET);
                    fread(&bir_takim,sizeof(Takim),1,takimlar);
                    printf("  %-4d%-17s%d",hafta_no,bir_takim.adi,evSahibi_atilanGol);
                    fseek(takimlar,(aranan_kod-1)*sizeof(Takim),SEEK_SET);
                    fread(&bir_takim,sizeof(Takim),1,takimlar);
                    printf("   %-2d %s\n",misafir_atilanGol,bir_takim.adi);
                    fscanf(skorlar,"%d %d %d %d %d",&hafta_no,&evSahibi_kod,&misafir_kod,&evSahibi_atilanGol,&misafir_atilanGol);
                }
                else
                    fscanf(skorlar,"%d %d %d %d %d",&hafta_no,&evSahibi_kod,&misafir_kod,&evSahibi_atilanGol,&misafir_atilanGol);
            }
        }
    }
    fclose(skorlar);//Dosya kapatýlýyor.
}

void puanCetveli(FILE *takimlar){

    typedef struct puan{//Puan, averaj ve kodlarýn bellekte tutulmasý için ayrý bir struct yapýsý oluþtuluyor.
        int puan;
        int averaj;
        int kod;
    }Puan;

    int takim_say=0,topOynanan_mac,i,a,b,max_index;
    Takim bir_takim;
    Puan bir_puan[MAX_KAYIT],Gecici;

    while(!feof(takimlar)){
        fread(&bir_takim,sizeof(Takim),1,takimlar);
        if(bir_takim.kodu!=BOS_KAYIT){
            bir_puan[takim_say].kod=bir_takim.kodu;                                //
            bir_puan[takim_say].puan=bir_takim.galibiyet*3+bir_takim.beraberlik;   //Bilgiler hesaplanýp structa yazýlýyor.
            bir_puan[takim_say].averaj=bir_takim.atilan_gol-bir_takim.yenilen_gol; //
            takim_say++;//Toplam takim sayýsý hesaplanýyor.
        }
    }

    //Aþaðýda takýmlar puanlarýna göre büyükten küçüðe sýralanýyor.
    for(a=0;a<takim_say-1;a++){
        max_index=a;
        for(b=a+1;b<takim_say;b++){
            if(bir_puan[b].puan>bir_puan[max_index].puan)
                max_index=b;
            else{
                if(bir_puan[b].puan==bir_puan[max_index].puan && bir_puan[b].averaj>bir_puan[max_index].averaj)
                    max_index=b;
            }
        }
        if(max_index!=a){
            Gecici=bir_puan[a];
            bir_puan[a]=bir_puan[max_index];
            bir_puan[max_index]=Gecici;
        }
    }
    printf("     Sira Takim Adi        OO GG BB MM AA YY AVR Puan\n");
    printf("     ---- ---------------- -- -- -- -- -- -- --- ----\n");
    for(i=0;i<takim_say;i++){//Takýmlar puan cetveli yazdýrýlýyor.
        fseek(takimlar,(bir_puan[i].kod-1)*sizeof(Takim),SEEK_SET);
        fread(&bir_takim,sizeof(Takim),1,takimlar);
        topOynanan_mac=bir_takim.galibiyet+bir_takim.maglubiyet+bir_takim.beraberlik;
        printf("      %-4d%-16s%3d%3d%3d%3d%3d%3d  %-5d%d\n",i+1,bir_takim.adi,topOynanan_mac,bir_takim.galibiyet,bir_takim.beraberlik,bir_takim.maglubiyet,bir_takim.atilan_gol,bir_takim.yenilen_gol,bir_puan[i].averaj,bir_puan[i].puan);
    }
}

void enCok_gol(FILE *takimlar){

    Takim bir_takim;
    int max_atilanGol=0,max_yenilenGol=0,max_atilanKod=0,max_yenilenKod=0;

    printf("     Takim Adi       AA YY\n");
    printf("     --------------- -- --\n");
    rewind(takimlar);
    while(!feof(takimlar)){
        fread(&bir_takim,sizeof(Takim),1,takimlar);//Dosyadaki deðerler geçici structa okunuyor.
        if(bir_takim.atilan_gol>max_atilanGol){//En çok gol atan takým bulunuyor.
            max_atilanGol=bir_takim.atilan_gol;
            max_atilanKod=bir_takim.kodu;
        }
        if(bir_takim.yenilen_gol>max_yenilenGol){//En çok gol yiyyen takým bulunuyor.
            max_yenilenGol=bir_takim.yenilen_gol;
            max_yenilenKod=bir_takim.kodu;
        }
    }
    fseek(takimlar,(max_atilanKod-1)*sizeof(Takim),SEEK_SET);
    fread(&bir_takim,sizeof(Takim),1,takimlar);
    printf("     %-16s%-d  %d\n",bir_takim.adi,bir_takim.atilan_gol,bir_takim.yenilen_gol);

    fseek(takimlar,(max_yenilenKod-1)*sizeof(Takim),SEEK_SET);
    fread(&bir_takim,sizeof(Takim),1,takimlar);
    printf("     %-16s%-d  %d\n",bir_takim.adi,bir_takim.atilan_gol,bir_takim.yenilen_gol);

    rewind(takimlar);
}

void hafta_macSonucu(FILE *takimlar){

    int aranan_hafta,mac_sayisi=0,top_gol=0;
    int hafta_no,evSahibi_Kodu,evSahibi_atilanGol,misafir_Kodu,misafir_atilanGol;
    float golOrt;
    FILE *skorlar;
    Takim bir_takim;

    printf("Mac sonuclarini gormek istediginiz haftayi giriniz: ");
    scanf("%d",&aranan_hafta);

    if((skorlar=fopen("skorlar.txt","r"))==NULL){
        printf("Skorlar dosyasi acilamadi");
    }
    else{
        printf("%d. Hafta Mac Sonuclari\n",aranan_hafta);
        printf("          Ev Sahibi             Misafir\n");
        printf("     Takim Adi        AA  AA Takim Adi\n");
        printf("     ---------------- --  -- ----------------\n");
        fscanf(skorlar,"%d %d %d %d %d",&hafta_no,&evSahibi_Kodu,&misafir_Kodu,&evSahibi_atilanGol,&misafir_atilanGol);
        while(!feof(skorlar)){
            if(aranan_hafta==hafta_no){
                fseek(takimlar,(evSahibi_Kodu-1)*sizeof(Takim),SEEK_SET);
                fread(&bir_takim,sizeof(Takim),1,takimlar);
                printf("     %-17s%-4d%-2d",bir_takim.adi,evSahibi_atilanGol,misafir_atilanGol);
                fseek(takimlar,(misafir_Kodu-1)*sizeof(Takim),SEEK_SET);
                fread(&bir_takim,sizeof(Takim),1,takimlar);
                printf(" %s\n",bir_takim.adi);
                mac_sayisi++;//Haftanýn gol ortalamsý bulunmasý için o haftadaki tolam maç sayýsý hesaplanýyor.
                top_gol=evSahibi_atilanGol+misafir_atilanGol+top_gol;//Toplam gol hesaplanýyor.
            }
            fscanf(skorlar,"%d %d %d %d %d",&hafta_no,&evSahibi_Kodu,&misafir_Kodu,&evSahibi_atilanGol,&misafir_atilanGol);
        }
        golOrt=(float)top_gol/mac_sayisi;//Haftanýn ortalama gol sayýsý hesaplanýyor.
        printf("\n     %d. hafta mac basina gol ortalamasi: %.2f\n",aranan_hafta,golOrt);
    }
    rewind(skorlar);
    fclose(skorlar);
}

void golKrali(FILE *takimlar,FILE * futbolcular){

    Takim bir_takim;
    Futbolcu bir_futbolcu;
    int max_gol=0,golKrali_forma,golKrali_takimKodu,i,futbolcu_anh;

    rewind(futbolcular);
    while(!feof(futbolcular)){
        fread(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);
        if(bir_futbolcu.attigi_gol>max_gol){//En çok gol atan futbolcu numarasý bulunuyor.
            max_gol=bir_futbolcu.attigi_gol;
            golKrali_forma=bir_futbolcu.forma_no;
        }
    }
    for(i=1;i<=99;i++){
        fseek(futbolcular,((i*100)+golKrali_forma-1)*sizeof(Futbolcu),SEEK_SET);//Takým kodu bir artýrarak ve gol kralýnýn forma numarasýný sabit tutularak, futbolcunun hangi takýmda olduðu bulunuyor.
        fread(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);
        if(bir_futbolcu.attigi_gol==max_gol){//Eðer max gol ile dosyadaki furbolcunun attýðý gol satýsý eþitse i deki deðer golKrali_takimKodu deðerine atanýyor.
            golKrali_takimKodu=i;
            break;
        }
    }
    fseek(takimlar,(golKrali_takimKodu-1)*sizeof(Takim),SEEK_SET);
    fread(&bir_takim,sizeof(Takim),1,takimlar);
    futbolcu_anh=(bir_takim.kodu*100)+golKrali_forma;
    printf("     Takim Adi        Forma No. Ad Soyad        AA\n");
    printf("     ---------------- --------- --------------- --\n");
    printf("     %-21s%-6d",bir_takim.adi,golKrali_forma);
    fseek(futbolcular,(futbolcu_anh-1)*sizeof(Futbolcu),SEEK_SET);
    fread(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);
    printf("%-16s%d\n",bir_futbolcu.ad_soyad,bir_futbolcu.attigi_gol);
    //rewind(futbolcular);
}

void dep_enCok_gol(FILE *takimlar){

    Takim bir_takim;
    int dep_maxGol=0,hafta_no,evSahibi_Kodu,misafir_Kodu,evSahibi_atilanGol,misafir_atilanGol,dep_maxGol_kod;
    FILE *skorlar;

    if((skorlar=fopen("skorlar.txt","r"))==NULL)
        printf("Skorlar dosyasi acilamadi!");
    else{
        fscanf(skorlar,"%d %d %d %d %d",&hafta_no,&evSahibi_Kodu,&misafir_Kodu,&evSahibi_atilanGol,&misafir_atilanGol);
        while(!feof(skorlar)){
            fseek(takimlar,(misafir_Kodu-1)*sizeof(Takim),SEEK_SET);
            fread(&bir_takim,sizeof(Takim),1,takimlar);
            if(misafir_atilanGol>dep_maxGol){//Deplasmanda en çok gol atan takim bulnuyor.
                dep_maxGol=misafir_atilanGol;
                dep_maxGol_kod=bir_takim.kodu;
            }
            fscanf(skorlar,"%d %d %d %d %d",&hafta_no,&evSahibi_Kodu,&misafir_Kodu,&evSahibi_atilanGol,&misafir_atilanGol);
        }
    }

    fseek(takimlar,(dep_maxGol_kod-1)*sizeof(Takim),SEEK_SET);
    fread(&bir_takim,sizeof(Takim),1,takimlar);

    printf("     Takim Adi        Deplasmanda AA\n");
    printf("     ---------------- --------------\n");
    printf("     %-23s%d\n",bir_takim.adi,dep_maxGol);

    rewind(takimlar);
    rewind(skorlar);
    fclose(skorlar);
}

void lig_ici_transfer(FILE *takimlar,FILE *futbolcular){

    int eski_takim_kodu,yeni_takim_kodu,eski_forma_no,yeni_forma_no,futbolcu_anh,bulundu=0;
    Futbolcu bir_futbolcu;
    Takim bir_takim;

    do{
        printf("Transfer etmek istediginiz futbolcunun su an oynadigi takimin kodunu giriniz: ");
        scanf("%d",&eski_takim_kodu);
        rewind(takimlar);
        while(!feof(takimlar)){
            fread(&bir_takim,sizeof(Takim),1,takimlar);
            if(bir_takim.kodu==eski_takim_kodu){//Girilen futbolcunun þu an oynadýðý takým kodunun olup olmadýðýný kontrol etmek için if yapýsý oluþturuluyor.Eðer böyle kodda bir takým varsa bulundu deðiþkenine 1 deðeri atanýyor.
                bulundu=1;
                break;
            }
        }
        if(bulundu!=1)//Eðer bulundu deðeri 1'e eþit deðilse(bu bu kodda bir takýmýn olmadýðý anlmýna gelir) uyarý veriyor.
            printf("Girilen kod numarasiyla takim bulunmamaktadir.Lutfen degerleri tekrar giriniz.\n");
    }while(bulundu!=1);//Bulundu deðeri 1 olana kadar döngü devam ediyor.

    bulundu=0;

    do{
        printf("Futbolcunun forma numarasini giriniz: ");
        scanf("%d",&eski_forma_no);
        rewind(futbolcular);
        while(!feof(futbolcular)){
            fread(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);
            if(bir_futbolcu.forma_no==eski_forma_no){//Girilen fubolcunun olup olmadýðýný kontrol etmek için if yapýsý oluþturuluyor.Eðer bu numarada fotbolcu bulursa bulundu deðþkenine 1 deðeri atanýyor.
                bulundu=1;
                break;
            }
        }
        if(bulundu!=1)//Eðer bulundu deðeri 1'e eþit deðilse(bu, bu kodda bir futbolcunun olmadýðý anlmýna gelir) uyarý veriyor.
            printf("Girilen forma numarasiyla oyuncu bulunmamaktadir.Lutfen degerleri tekrar giriniz.\n");
    }while(bulundu!=1);//Bulundu deðeri 1 olana kadar döngü devam ediyor.

    bulundu=0;
    do{
        printf("Transfer edilecegi kulubun takim kodunu giriniz: ");
        scanf("%d",&yeni_takim_kodu);
        rewind(takimlar);
        while(!feof(takimlar)){
            fread(&bir_takim,sizeof(Takim),1,takimlar);
            if(bir_takim.kodu==yeni_takim_kodu){
                bulundu=1;
                break;
            }
        }
        if(bulundu!=1)
            printf("Girilen kod numarasiyla takim bulunmamaktadir.Lutfen degerleri tekrar giriniz.\n");
    }while(bulundu!=1);

    bulundu=0;
    do{
        printf("Futbolcunun yeni forma numarasini giriniz: ");
        scanf("%d",&yeni_forma_no);
        rewind(futbolcular);
        while(!feof(futbolcular)){
            fread(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);
            if(bir_futbolcu.forma_no==yeni_forma_no){
                bulundu=1;
                break;
            }
        }
        if(bulundu!=1)
            printf("Girilen forma numarasiyla oyuncu bulunmamaktadir.Lutfen degerleri tekrar giriniz.\n");
    }while(bulundu!=1);

    futbolcu_anh=(eski_takim_kodu*100)+eski_forma_no;
    fseek(futbolcular,(futbolcu_anh-1)*sizeof(Futbolcu),SEEK_SET);//Futbolcunun dosyada eski anahtar kodundaki yere gidiliyor.
    fread(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);//Veriler geçici structa okunuyor

    futbolcu_anh=(yeni_takim_kodu*100)+yeni_forma_no;
    fseek(futbolcular,(futbolcu_anh-1)*sizeof(Futbolcu),SEEK_SET);//Futbolcunun dosyada yeni anahtar kodundaki yere gidiliyor.
    fwrite(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);//Geçici structaki veriler yeni anahtar kodundaki yere yazýlýyor.

    futbolcu_anh=(eski_takim_kodu*100)+eski_forma_no;
    fseek(futbolcular,(futbolcu_anh-1)*sizeof(Futbolcu),SEEK_SET);//Futbolcunun dosyada eski anahtar kodundaki yere gidiliyor.
    fread(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);//Veriler geçici structa okunuyor
    bir_futbolcu.forma_no=BOS_KAYIT;//Geçici yapýdaki forma numarasýna BOS_KAYIT(-1) deðeri atanýyor.
    fseek(futbolcular,(futbolcu_anh-1)*sizeof(Futbolcu),SEEK_SET);//Futbolcunun dosyada eski anahtar kodundaki yere gidiliyor.
    fwrite(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);//Geçici structaki veriler yeni anahtar kodundaki yere yazýlýyor.Yani artýk bu yerdeki forma no deðeri -1 oluyor.

    printf("Transfer islemi gerceklesti.\n");
}

void lig_disi_yeni_futbolcu(FILE *takimlar,FILE * futbolcular){

    int takim_kodu,forma_no,futbolcu_anh,bulundu=0;
    Futbolcu bir_futbolcu;
    Takim bir_takim;

    bulundu=0;
    do{
        printf("Transfer edilecegi kulubun takim kodunu giriniz: ");
        scanf("%d",&takim_kodu);
        rewind(takimlar);
        while(!feof(takimlar)){
            fread(&bir_takim,sizeof(Takim),1,takimlar);
            if(bir_takim.kodu==takim_kodu){
                bulundu=1;
                break;
            }
        }
        if(bulundu!=1)
            printf("Girilen kod numarasiyla takim bulunmamaktadir.Lutfen degerleri tekrar giriniz.\n");
    }while(bulundu!=1);

    printf("Futbolcunun yeni forma numarasini giriniz: ");
    scanf("%d",&forma_no);

    futbolcu_anh=(takim_kodu*100)+forma_no;
    fseek(futbolcular,(futbolcu_anh-1)*sizeof(Futbolcu),SEEK_SET);//Girilen takým kodu ve futbolcu numarasý ile oluþturulan anahtar kodu ile dosyadaki o yere gidiliyor.
    fread(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);//Anahtar kodundaki veriler geçici structa okunuyor.
    while(bir_futbolcu.forma_no!=BOS_KAYIT){//Geçici structa forma numarasýnýn -1 olup olmadýðý kontrol ediliyor.Eðer deðer -1'e eþit deðilse döngü devam ediyor.
        printf("Bu takimda girdiginiz forma numarasiyla oyunca bulunmaktadir, lutfen tekrar giriniz: ");
        scanf("%d %d", &takim_kodu,&forma_no);

        futbolcu_anh=(takim_kodu*100)+forma_no;
        fseek(futbolcular,(futbolcu_anh-1)*sizeof(Futbolcu),SEEK_SET);
        fread(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);
    }

    printf("Futbolcunun adini ve soyadini giriniz: ");
    fflush(stdin);
    gets(bir_futbolcu.ad_soyad);      //
    bir_futbolcu.forma_no=forma_no;   //Bilgiler geçici structa yazýlýyor.
    bir_futbolcu.attigi_gol=0;        //

    fseek(futbolcular,(futbolcu_anh-1)*sizeof(Futbolcu),SEEK_SET);//Dosyada yeni anahtar kodundaki yere gidiliyor.
    fwrite(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);//Geçici structaki deðerler dosyaya yazýlýyor.
    printf("Transfer islemi gerceklesti.\n");
}

void lig_disi_transfer(FILE *takimlar,FILE *futbolcular)
{
    int futbolcu_anh,takim_kodu,bulundu=0;
    Futbolcu bir_futbolcu;
    Takim bir_takim;

    do{
        printf("Lig disindaki takima transfer etmek istediginiz futbolcunun takim kodunu giriniz: ");
        scanf("%d",&takim_kodu);
        rewind(takimlar);
        while(!feof(takimlar)){
            fread(&bir_takim,sizeof(Takim),1,takimlar);
            if(bir_takim.kodu==takim_kodu){
                bulundu=1;
                break;
            }
        }
        if(bulundu!=1)
            printf("Girilen kod numarasiyla takim bulunmamaktadir.Lutfen degerleri tekrar giriniz.\n");
    }while(bulundu!=1);

    printf("Oyuncunun forma numarasini giriniz: ");
    scanf("%d",&bir_futbolcu.forma_no);

    futbolcu_anh=(takim_kodu*100)+bir_futbolcu.forma_no;
    fseek(futbolcular,(futbolcu_anh-1)*sizeof(Futbolcu),SEEK_SET);
    fread(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);
    while(futbolcu_anh==BOS_KAYIT){//Geçici structa forma numarasýnýn -1 olup olmadýðý kontrol ediliyor.Eðer deðer -1'e eþitse döngü devam ediyor.
        printf("Bu takimda girdiginiz forma numarasiyla oyunca bulunmamaktadir, lutfen tekrar giriniz: ");
        scanf("%d",&bir_futbolcu.forma_no);

        futbolcu_anh=(takim_kodu*100)+bir_futbolcu.forma_no;
        fseek(futbolcular,(futbolcu_anh-1)*sizeof(Futbolcu),SEEK_SET);
        fread(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);
    }
    bir_futbolcu.forma_no=BOS_KAYIT;//Forma numarasýna -1 atanýyor.
    fseek(futbolcular,(futbolcu_anh-1)*sizeof(Futbolcu),SEEK_SET);//Dosyadaki anahtar koda gidiliyor.
    fwrite(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);//geçici structaki deðerler dosyaya yazýlýyor.
    printf("Transfer islemi gerceklesti.\n");
}





