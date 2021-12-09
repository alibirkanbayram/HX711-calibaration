//━━━━━━━━━━━━━━━━━━━━━━━━━━━ Gerekli kütüphaneler ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#include "HX711.h"  // Loadcell için gerekli kütüphane

//━━━━━━━━━━━━━━━━━━━━━━━━━━━ LOADCELL tanımlamaları ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
HX711 scale; // HX711 den scale adı altında bir nesne oluşturuldu.
const int LOADCELL_DOUT_PIN = 23; // DOUT pini D5'e bağlanacak.
const int LOADCELL_SCK_PIN = 19; // SCK pini D4'a bağlanacak.
float units, calibration_factor = 30.5; // Ağırlık değişkenleri

//━━━━━━━━━━━━━━━━━━━━━━━━━━━ void setup() fonksiyonu ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void setup() {
  Serial.begin(115200); // Serial haberleşme hızını 115200 baud hızı olarak ayarlandı
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN); // LOADCELL bağlantısı yapılıyor.
  scale.set_scale(); // Kalibrasyon faktörü tanımlama
  scale.tare(); //Reset the scale to 0 -> Ölçeği sıfırla

  // Kalibrasyon için kullanıcı bilgilendirmesi
  Serial.println("HX711 kalibrasyon kodu");
  Serial.println("Lütfen tartı üzerinde duran bir nesene varsa kaldırın.");
  Serial.println("Okumalar başladıktan sonra bilinen ağırlığı tartıya koyun.");
  Serial.println("Kalibrasyon faktörünü artırmak için + veya a'ya basın");
  Serial.println("Kalibrasyon faktörünü azaltmak için - veya z'ye basın");
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━ void setup() fonksiyonu ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void loop() {
  scale.set_scale(calibration_factor); // Her seferinde kalibrasyon yapıyoruz. Sürekli güncel değerler görülür

  Serial.print("Okunuyor: ");
  units = scale.get_units(10),2;
  if (units < 0) // Unit yani ölçülen değer 0'dan düşük ise 0 olarak kabul et.
    units = 0.00;

  Serial.print(units); // Kaç gram olduğu yazılıyor
  Serial.print(" gram");
  Serial.print(" calibration_factor değeri: ");
  Serial.print(calibration_factor);
  Serial.println(); // Bir alt satıra geç.
  //delay(10); // 10ms bekle

  // Kalibrasyon yani calibration_factor değişkenini ayarlamak için kullanıcıdan bilgi istenir.
  if(Serial.available()){ // Kullanıcı serial bilgi gönderildiğinde çalış.
    char temp = Serial.read(); // temp değişkeninin içine gelen bilgiyi yaz
    if(temp == '+' || temp == 'a') // Eğer + veya a gönderilmiş ise calibration_factor değişkenini arttır.
      calibration_factor += 1;
    else if(temp == '-' || temp == 'z') // Eğer - veya z gönderilmiş ise calibration_factor değişkenini azalt.
      calibration_factor -= 1;
  }
}
