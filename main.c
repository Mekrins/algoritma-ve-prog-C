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

    if((takimlar=fopen("takimlar.dat","rb+"))==NULL){//takimlar dosyas� hem i�ine yeni veriler yaz�laca�� i�in, hemde i�indeki veriler okunaca�� i�in ve takimlar dosyas� do�rudan eri�imli dosya oldu�u i�in rb+ format�nda a��l�yor.
        printf("Takimlar dosyasi acilamadi!");//Dosyan�n a��l�p a��lamad��� "NULL" ile belirleniyor.E�er dosya a��lamay�p NULL �evirirse uyar� verilriyor.
    }
    else{
        if((futbolcular=fopen("futbolcular.dat","rb+"))==NULL){//takimlar dosyas� ile ilgili olay ayn� �ekilde futbolcular dosyas�nada uygulan�yor.
            printf("Futbolcular dosyasi acilamadi!");
        }
        else{
                do{//Men� do while ve switch case kullan�larak olu�turuluyor.
                    secenek=menu_goruntule();
                    //system("cls");
                    switch(secenek){//Her se�ene�e �zel fonksiyonlar numaralar�na g�re a���adaki gibi yaz�l�yor.
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
                } while(secenek!=12);//Kullan�c� 12 say�s�n� girmedik�e men� tekrar ve tekrara g�steriliyor.
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
    Futbolcu bir_futbolcu;// Her fonsiyonda oldu�u gibi ge�ici bir struct olu�tuluyor.
    Takim bir_takim;//Ve yine her fonksiyonda oldu�u gibi ge�ici bir struct yap�s� olu�turuluyor.
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
            if(hafta_no<=0)//E�er hafta numaras� 0'dan k���kse bir uyar� veriliyor ve numaran�n tekrar girilmesi isteniyor.
                printf("Hafta numarasi 0'dan buyuk olmalidir!\n");
        }while(hafta_no<=0);

        do{
            gecici=0;
            do{//
                printf("Ev sahibi takimin kodunu ve attigi gol sayisini bir bosluk birakarak giriniz: ");// Ev sahibi takimin kodu ve att��� gol say�s� isteniyor.
                scanf("%d %d",&evSahibi_Kodu,&evSahibi_atilanGol);
                rewind(takimlar);//Dosya pozisyon g�stericisi her tekrarda en ba�a ta��n�yor aksi takdirde yeni d�ng�deki i�lemler, eski d�ng�n�n dosya pozisyon g�stericisini b�rakt��� yerde ba�l�yaca�� i�in hatalar ortaya ��kar.
                while(!feof(takimlar)){//takimlar dosyas� en ba�tan en sona kadar aran�yor ve e�er aranan de�er bulunursa bulundu de�i�kenine 1 at�yor.
                    fread(&bir_takim,sizeof(Takim),1,takimlar);
                    if(bir_takim.kodu==evSahibi_Kodu)
                        bulundu=1;
                }
                if(bulundu!=1)
                    printf("Girilen kod numarasiyla takim bulunmamaktadir.Lutfen degerleri tekrar giriniz.\n");
            }while(bulundu!=1);//While d�ng�s� bulundu 1 olana kadar tekrar tekrar d�n�yor.

            if(evSahibi_atilanGol!=0){
                do{
                    bulundu=0;
                    do{
                        printf("Gol atan futbolcunun forma numarasini ve attigi gol sayisini bir bosluk birakarak giriniz: ");
                        scanf("%d %d",&no,&gol);
                        if(gol==0){
                            break;
                        }
                        rewind(futbolcular);//Dosya pozisyon g�stericisi her tekrarda en ba�a ta��n�yor aksi takdirde yeni d�ng�deki i�lemler, eski d�ng�n�n dosya pozisyon g�stericisini b�rakt��� yerde ba�l�yaca�� i�in hatalar ortaya ��kar.
                        while(!feof(futbolcular)){//futbolcular dosyas� en ba�tan en sona kadar aran�yor ve e�er aranan de�er bulunursa bulundu de�i�kenine 1 at�yor.
                            fread(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);
                            if(bir_futbolcu.forma_no==no)
                                bulundu=1;
                        }
                        if(bulundu!=1)
                            printf("Girilen forma numarasiyla oyuncu bulunmamaktadir.Lutfen degerleri tekrar giriniz.\n");
                    }while(bulundu!=1);//While d�ng�s� bulundu 1 olana kadar tekrar tekrar d�n�yor.

                    futbolcu_anh=(evSahibi_Kodu*100)+no;//Gol� atan futbolcunun futbolcular dosyas�ndaki anahtar kodu hesaplan�yor.
                    fseek(futbolcular,(futbolcu_anh-1)*sizeof(Futbolcu),SEEK_SET);//Dosya pozisyon g�stericisi futbolcunun anahat koduna getiriliyor.
                    fread(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);//O yerdeki struct, ge�ici olarak olu�turulan struct �zerine yaz�l�yor.
                    bir_futbolcu.attigi_gol=gol+bir_futbolcu.attigi_gol;//Ge�ici structa futbolcunun att��� gol ekleniyor.
                    gecici+=gol;//Ge�ici de�i�kenine toplamda at�lan goller atan�yor.
                    fseek(futbolcular,(futbolcu_anh-1)*sizeof(Futbolcu),SEEK_SET);
                    fwrite(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);//De�i�tirilen struct, futbolcunun anahtar kodundaki yere yazd�r�l�yor.
                    //rewind(futbolcular);

                    if(gecici>evSahibi_atilanGol){//Futbolcular�n att��� goller tak�m�n ilk kez istenilen at�lan gol say�s�n ge�erse bir uyar� veriliyor ve ge�ici de�i�keni s�f�rlan�yor.
                        printf("Toplam gol sayisini asan degerler girdiniz.Lutfen degerleri tekrar girin.\n");
                        gecici=0;
                    }
                }while(gecici<evSahibi_atilanGol);
            }

            do{//Yukarda yap�lan t�m i�lmeler ayn� �ekilde misfir takim i�in tekrar uygulan�yor.
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

            //�ki takim aras�ndaki galibiye, ma�lubiyet ve berabarlik de�erleri a�a��daki if yap�lar� ile bulunuyor.
            if(evSahibi_atilanGol>misafir_atilanGol){//Ev sahibi takimin att��� gol fazla ise galibiyeti bir art�yor, di�er tak�m�n ma�lubiyet de�eri art�yor.
                fseek(takimlar,(evSahibi_Kodu-1)*sizeof(Takim),SEEK_SET);//Ev sahibi takimin dosyada bulundu�u yere gidiliyor.
                fread(&bir_takim,sizeof(Takim),1,takimlar);//O yerdeki de�erler ge�ici struct �zerine okunuyor.

                bir_takim.galibiyet++;                         //
                bir_takim.atilan_gol+=evSahibi_atilanGol;      //Bilgiler yenileniyor.
                bir_takim.yenilen_gol+=misafir_atilanGol;      //

                fseek(takimlar,(evSahibi_Kodu-1)*sizeof(Takim),SEEK_SET);//Ev sahibi takimin dosyada bulundu�u yere gidiliyor.
                fwrite(&bir_takim,sizeof(Takim),1,takimlar);//Structaki yenilenen de�erler dosyadaki takimin bulungu�u yere tekarar yaz�l�yor

                //Yukarda yap�lan i�lemler ayn� �ekilde a�a��daki yerlerede yap�l�yor.
                fseek(takimlar,(misafir_Kodu-1)*sizeof(Takim),SEEK_SET);//
                fread(&bir_takim,sizeof(Takim),1,takimlar);

                bir_takim.maglubiyet++;
                bir_takim.atilan_gol+=misafir_atilanGol;
                bir_takim.yenilen_gol+=evSahibi_atilanGol;

                fseek(takimlar,(misafir_Kodu-1)*sizeof(Takim),SEEK_SET);
                fwrite(&bir_takim,sizeof(Takim),1,takimlar);
            }
            else{
                if(evSahibi_atilanGol<misafir_atilanGol){//Misafir tak�m�n att��� gol daha falzla ise galibiyet de�eri bir art�yor, di�er tak�m�n ma�lubiyet de�eri bir art�yor.
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
                else{//E�er iki tak�m�n att��� goller bir birine e�itse iki tak�m�nda beraberlik de�erleri bir art�yor.
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

            printf("Biten baska mac var mi?(E/H): ");//Kullan�c�dan devam edilip edilmemesi i�in E(evet) yada H(hay�r) harflerinden birisinin girilmesi isteniyor.
            fflush(stdin);
            scanf("%c",&devam);
            printf("\n");
        }while(devam=='E' || devam=='e');//Kullan�c� e yada E harfini girdik�e d�ng� d�nmeye devam edicek.
    }
    fclose(skorlar);//Dosya kapat�l�yor.
}

void takimAdiyla_arama(FILE *takimlar,FILE *futbolcular){

    Takim bir_takim;
    Futbolcu bir_futbolcu;
    int topOynanan_mac=0,futbolcu_anh,i=0,bulundu=0,averaj,puan;
    char aranan_takimAdi[MAX_KARAKTER];

    printf("Bilgilerini gormek istediginiz takimin adini giriniz: ");
    fflush(stdin);
    scanf("%s",aranan_takimAdi);
    rewind(takimlar);//Dosya pozisyon g�stericisi  en ba�a getiriliyor.
    while(!feof(takimlar)){//Takilar dosyas� ba�tan sona taran�yor.
        fread(&bir_takim,sizeof(Takim),1,takimlar);
        if(bir_takim.kodu!=BOS_KAYIT){
            if(strcmp(aranan_takimAdi,bir_takim.adi)==0){//Aranan takim adi ve ge�ici structaki takim adi kar��la�t�r�l�yor, e�er iki ad e�itse 0 say�s� d�nd�r�yor.
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
    if(bir_takim.kodu==BOS_KAYIT){//E�er aranan kodda bo� kay�t varsa fonsiyon uyar� verip bo� de�er d�nd�r�yor.
        printf("%d koduyla takim bulunmamaktadir!\n",aranan_kod);
        return;
    }
    topOynanan_mac=bir_takim.galibiyet+bir_takim.maglubiyet+bir_takim.beraberlik;//Toplam oynanan ma� hesaplan�yor.

    if((skorlar=fopen("skorlar.txt","r"))==NULL){//skorlar dosyas�nda sadece okuma yap�laca�� i�in ve binary yap�da dosya olmad��� i�in r modunda a��l�yor.
        printf("Takimlar dosyasi acilamadi!");
    }
    else{
        averaj=bir_takim.atilan_gol-bir_takim.yenilen_gol;//Averaj hesaplan�yor.
        puan=bir_takim.galibiyet*3+bir_takim.beraberlik;//Puan hesaplan�yor.

        printf("     Takim Adi     OO GG BB MM AA YY AVR Puan\n");
        printf("     ------------- -- -- -- -- -- -- --- ----\n");
        printf("     %-14s%2d%3d%3d%3d%3d%3d%3d%5d\n",bir_takim.adi,topOynanan_mac,bir_takim.galibiyet,bir_takim.beraberlik,bir_takim.maglubiyet,bir_takim.atilan_gol,bir_takim.yenilen_gol,averaj,puan);

        printf("\nOynadigi maclarin skorlari:\n");
        printf("          Ev Sahibi               Misafir\n");
        printf("Hafta Takim Adi        AA  AA Takim Adi\n");
        printf("----- ---------------- --  -- ------------------\n");

        fscanf(skorlar,"%d %d %d %d %d",&hafta_no,&evSahibi_kod,&misafir_kod,&evSahibi_atilanGol,&misafir_atilanGol);// De�erler dosyadan okunuyor.
        while(!feof(skorlar)){//Dosya ba�tan sona do�ru aran�yor.
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
    fclose(skorlar);//Dosya kapat�l�yor.
}

void puanCetveli(FILE *takimlar){

    typedef struct puan{//Puan, averaj ve kodlar�n bellekte tutulmas� i�in ayr� bir struct yap�s� olu�tuluyor.
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
            bir_puan[takim_say].puan=bir_takim.galibiyet*3+bir_takim.beraberlik;   //Bilgiler hesaplan�p structa yaz�l�yor.
            bir_puan[takim_say].averaj=bir_takim.atilan_gol-bir_takim.yenilen_gol; //
            takim_say++;//Toplam takim say�s� hesaplan�yor.
        }
    }

    //A�a��da tak�mlar puanlar�na g�re b�y�kten k����e s�ralan�yor.
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
    for(i=0;i<takim_say;i++){//Tak�mlar puan cetveli yazd�r�l�yor.
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
        fread(&bir_takim,sizeof(Takim),1,takimlar);//Dosyadaki de�erler ge�ici structa okunuyor.
        if(bir_takim.atilan_gol>max_atilanGol){//En �ok gol atan tak�m bulunuyor.
            max_atilanGol=bir_takim.atilan_gol;
            max_atilanKod=bir_takim.kodu;
        }
        if(bir_takim.yenilen_gol>max_yenilenGol){//En �ok gol yiyyen tak�m bulunuyor.
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
                mac_sayisi++;//Haftan�n gol ortalams� bulunmas� i�in o haftadaki tolam ma� say�s� hesaplan�yor.
                top_gol=evSahibi_atilanGol+misafir_atilanGol+top_gol;//Toplam gol hesaplan�yor.
            }
            fscanf(skorlar,"%d %d %d %d %d",&hafta_no,&evSahibi_Kodu,&misafir_Kodu,&evSahibi_atilanGol,&misafir_atilanGol);
        }
        golOrt=(float)top_gol/mac_sayisi;//Haftan�n ortalama gol say�s� hesaplan�yor.
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
        if(bir_futbolcu.attigi_gol>max_gol){//En �ok gol atan futbolcu numaras� bulunuyor.
            max_gol=bir_futbolcu.attigi_gol;
            golKrali_forma=bir_futbolcu.forma_no;
        }
    }
    for(i=1;i<=99;i++){
        fseek(futbolcular,((i*100)+golKrali_forma-1)*sizeof(Futbolcu),SEEK_SET);//Tak�m kodu bir art�rarak ve gol kral�n�n forma numaras�n� sabit tutularak, futbolcunun hangi tak�mda oldu�u bulunuyor.
        fread(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);
        if(bir_futbolcu.attigi_gol==max_gol){//E�er max gol ile dosyadaki furbolcunun att��� gol sat�s� e�itse i deki de�er golKrali_takimKodu de�erine atan�yor.
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
            if(misafir_atilanGol>dep_maxGol){//Deplasmanda en �ok gol atan takim bulnuyor.
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
            if(bir_takim.kodu==eski_takim_kodu){//Girilen futbolcunun �u an oynad��� tak�m kodunun olup olmad���n� kontrol etmek i�in if yap�s� olu�turuluyor.E�er b�yle kodda bir tak�m varsa bulundu de�i�kenine 1 de�eri atan�yor.
                bulundu=1;
                break;
            }
        }
        if(bulundu!=1)//E�er bulundu de�eri 1'e e�it de�ilse(bu bu kodda bir tak�m�n olmad��� anlm�na gelir) uyar� veriyor.
            printf("Girilen kod numarasiyla takim bulunmamaktadir.Lutfen degerleri tekrar giriniz.\n");
    }while(bulundu!=1);//Bulundu de�eri 1 olana kadar d�ng� devam ediyor.

    bulundu=0;

    do{
        printf("Futbolcunun forma numarasini giriniz: ");
        scanf("%d",&eski_forma_no);
        rewind(futbolcular);
        while(!feof(futbolcular)){
            fread(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);
            if(bir_futbolcu.forma_no==eski_forma_no){//Girilen fubolcunun olup olmad���n� kontrol etmek i�in if yap�s� olu�turuluyor.E�er bu numarada fotbolcu bulursa bulundu de��kenine 1 de�eri atan�yor.
                bulundu=1;
                break;
            }
        }
        if(bulundu!=1)//E�er bulundu de�eri 1'e e�it de�ilse(bu, bu kodda bir futbolcunun olmad��� anlm�na gelir) uyar� veriyor.
            printf("Girilen forma numarasiyla oyuncu bulunmamaktadir.Lutfen degerleri tekrar giriniz.\n");
    }while(bulundu!=1);//Bulundu de�eri 1 olana kadar d�ng� devam ediyor.

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
    fread(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);//Veriler ge�ici structa okunuyor

    futbolcu_anh=(yeni_takim_kodu*100)+yeni_forma_no;
    fseek(futbolcular,(futbolcu_anh-1)*sizeof(Futbolcu),SEEK_SET);//Futbolcunun dosyada yeni anahtar kodundaki yere gidiliyor.
    fwrite(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);//Ge�ici structaki veriler yeni anahtar kodundaki yere yaz�l�yor.

    futbolcu_anh=(eski_takim_kodu*100)+eski_forma_no;
    fseek(futbolcular,(futbolcu_anh-1)*sizeof(Futbolcu),SEEK_SET);//Futbolcunun dosyada eski anahtar kodundaki yere gidiliyor.
    fread(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);//Veriler ge�ici structa okunuyor
    bir_futbolcu.forma_no=BOS_KAYIT;//Ge�ici yap�daki forma numaras�na BOS_KAYIT(-1) de�eri atan�yor.
    fseek(futbolcular,(futbolcu_anh-1)*sizeof(Futbolcu),SEEK_SET);//Futbolcunun dosyada eski anahtar kodundaki yere gidiliyor.
    fwrite(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);//Ge�ici structaki veriler yeni anahtar kodundaki yere yaz�l�yor.Yani art�k bu yerdeki forma no de�eri -1 oluyor.

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
    fseek(futbolcular,(futbolcu_anh-1)*sizeof(Futbolcu),SEEK_SET);//Girilen tak�m kodu ve futbolcu numaras� ile olu�turulan anahtar kodu ile dosyadaki o yere gidiliyor.
    fread(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);//Anahtar kodundaki veriler ge�ici structa okunuyor.
    while(bir_futbolcu.forma_no!=BOS_KAYIT){//Ge�ici structa forma numaras�n�n -1 olup olmad��� kontrol ediliyor.E�er de�er -1'e e�it de�ilse d�ng� devam ediyor.
        printf("Bu takimda girdiginiz forma numarasiyla oyunca bulunmaktadir, lutfen tekrar giriniz: ");
        scanf("%d %d", &takim_kodu,&forma_no);

        futbolcu_anh=(takim_kodu*100)+forma_no;
        fseek(futbolcular,(futbolcu_anh-1)*sizeof(Futbolcu),SEEK_SET);
        fread(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);
    }

    printf("Futbolcunun adini ve soyadini giriniz: ");
    fflush(stdin);
    gets(bir_futbolcu.ad_soyad);      //
    bir_futbolcu.forma_no=forma_no;   //Bilgiler ge�ici structa yaz�l�yor.
    bir_futbolcu.attigi_gol=0;        //

    fseek(futbolcular,(futbolcu_anh-1)*sizeof(Futbolcu),SEEK_SET);//Dosyada yeni anahtar kodundaki yere gidiliyor.
    fwrite(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);//Ge�ici structaki de�erler dosyaya yaz�l�yor.
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
    while(futbolcu_anh==BOS_KAYIT){//Ge�ici structa forma numaras�n�n -1 olup olmad��� kontrol ediliyor.E�er de�er -1'e e�itse d�ng� devam ediyor.
        printf("Bu takimda girdiginiz forma numarasiyla oyunca bulunmamaktadir, lutfen tekrar giriniz: ");
        scanf("%d",&bir_futbolcu.forma_no);

        futbolcu_anh=(takim_kodu*100)+bir_futbolcu.forma_no;
        fseek(futbolcular,(futbolcu_anh-1)*sizeof(Futbolcu),SEEK_SET);
        fread(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);
    }
    bir_futbolcu.forma_no=BOS_KAYIT;//Forma numaras�na -1 atan�yor.
    fseek(futbolcular,(futbolcu_anh-1)*sizeof(Futbolcu),SEEK_SET);//Dosyadaki anahtar koda gidiliyor.
    fwrite(&bir_futbolcu,sizeof(Futbolcu),1,futbolcular);//ge�ici structaki de�erler dosyaya yaz�l�yor.
    printf("Transfer islemi gerceklesti.\n");
}





