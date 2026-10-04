# 🎙️ Kulaklık Mikrofonu (Jack) ve RGB LED ile Ses Duyarlı Arduino Projesi

Bu proje, standart bir kulaklık mikrofonunu (3.5mm TRS/TRRS Jack kullanarak) Arduino'ya nasıl güvenli bir şekilde bağlayacağınızı ve mikrofondan alınan ses sinyallerine göre bir RGB LED'i nasıl kontrol edebileceğinizi adım adım açıklamaktadır. 

Proje iki temel aşamadan oluşur: **Mikrofon & Jack Kablolama Esasları** ve **Arduino Devre Kurulumu**.

---

## 🛠️ 1. Mikrofon Kablolama ve Jack Yapısı (`image_1LHXw9.png`)

Standart kayıt cihazlarının içinde elektret (electret) mikrofonların çalışabilmesi için **"Plug-in Power"** adı verilen düşük bir DC voltaj beslemesi gerekir. Hazırladığımız şemada bu yapı şu şekilde çalışmaktadır:

*   **Kayıt Cihazı İçi (Inside the recording device):** Sağ (Right) ve Sol (Left) ses sinyalleri, DC akımı engellemek ve sadece AC ses sinyalini geçirmek için birer **kondansatör (kapasitör)** üzerinden ses işlemcisine aktarılır. Aynı hatlar üzerinden mikrofonun çalışması için gereken güç (Power) birer direnç vasıtasıyla sağlanır.
*   **Mikrofon Tarafı (The microphone wiring):** 3.5mm Stereo (TRS) Jack ucunda:
    *   **Uç kısım (Tip):** Sol Mikrofonun (Left) pozitif (`+`) kutbuna bağlanır.
    *   **Orta bilezik (Ring):** Sağ Mikrofonun (Right) pozitif (`+`) kutbuna bağlanır.
    *   **Gövde (Sleeve):** Her iki mikrofonun da negatif (`-`) yani şase/GND kutbuna bağlanır.

---

## 🔌 2. Arduino Devre Şeması ve Bağlantıları (`image_x_aw2c.png`)

Projenin Arduino tarafında, mikrofondan gelen analog sinyal okunur ve bu sinyalin şiddetine (ses seviyesine) göre RGB LED'in renkleri dinamik olarak değiştirilir. Araya eklenen buton ise devreyi açıp kapatmak veya mod değiştirmek amacıyla konumlandırılmıştır.

### Bağlantı Tablosu

| Komponent Pin / Kablo | Arduino Pini | Açıklama |
| :--- | :--- | :--- |
| **RGB LED - Kırmızı (Red)** | `D9` (PWM) | Kırmızı renk kontrolü |
| **RGB LED - Yeşil (Green)** | `D10` (PWM) | Yeşil renk kontrolü |
| **RGB LED - Mavi (Blue)** | `D11` (PWM) | Mavi renk kontrolü |
| **Buton (Giriş Ayağı)** | `5V` | Güç hattından besleme alır |
| **Buton (Çıkış Ayağı)** | Mikrofon `+` Hattı | Mikrofonu aktif etmek için tetikleyici |
| **Mikrofon Jackı (GND)** | `GND` | Arduino toprak hattına bağlanır |

*Not: Görseldeki buton yapısına göre mikrofonun sinyal/güç hattı kontrollü olarak Arduino devresine entegre edilmiştir. Mikrofondan gelen ham analog veriyi okumak için jack çıkışından Arduino'nun `A0` gibi bir Analog giriş pinine bağlantı yapılması önerilir.*

---

## 💻 3. Proje Kodu

Projede kullanılan güncel Arduino kaynak koduna aşağıdan ulaşabilirsiniz:

```cpp
// Sabitlenen Fiziksel Bağlantı Tanımlamaları (#define)
#define LED_MAVI    11  // 1. Bacak -> D11
#define LED_YESIL   10  // 2. Bacak -> D10
#define LED_KIRMIZI  9  // 4. Bacak -> D9

const bool ORTAK_ANOT = true; 

void setup() {
  pinMode(LED_KIRMIZI, OUTPUT);
  pinMode(LED_YESIL, OUTPUT);
  pinMode(LED_MAVI, OUTPUT);
}

void loop() {
  // Renk çarkını 0 ile 768 derece/adım arasında kesintisiz döndürüyoruz
  // Bu sayede hiçbir döngü bitişinde sekme veya atlama yaşanmaz.
  for (int derece = 0; derece < 765; derece++) {
    int r, g, b;

    if (derece < 255) {
      // 1. Kısım: Kırmızıdan Yeşile geçiş
      r = 255 - derece;
      g = derece;
      b = 0;
    } else if (derece < 510) {
      // 2. Kısım: Yeşilden Maviye geçiş
      r = 0;
      g = 255 - (derece - 255);
      b = derece - 255;
    } else {
      // 3. Kısım: Maviden Kırmızıya geçiş (Pembedeki sekme burada tamamen pürüzsüzleşti)
      r = derece - 510;
      g = 0;
      b = 255 - (derece - 510);
    }

    renkYaz(r, g, b);
    delay(3); // Geçiş hızı (Daha da hızlandırmak için 1 veya 2 yapabilirsiniz)
  }
}

void renkYaz(int kirmizi, int yesil, int mavi) {
  if (ORTAK_ANOT) {
    analogWrite(LED_KIRMIZI, 255 - kirmizi);
    analogWrite(LED_YESIL, 255 - yesil);
    analogWrite(LED_MAVI, 255 - mavi);
  } else {
    analogWrite(LED_KIRMIZI, kirmizi);
    analogWrite(LED_YESIL, yesil);
    analogWrite(LED_MAVI, mavi);
  }
}

```

---

## 🚀 Nasıl Çalıştırılır?

1.  **Jack Bağlantısını Yapın:** Kulaklık mikrofonunuzu şemada (`image_1LHXw9.png`) gösterilen kutuplara uygun şekilde 3.5mm jack yuvasına lehimleyin veya bağlayın.
2.  **Arduino Devresini Kurun:** Komponentleri ikinci şemadaki (`image_x_aw2c.png`) gibi Breadboard üzerine yerleştirip jumper kablolarla Arduino'ya bağlayın.
3.  **Kodu Yükleyin:** Kendi kodunuzu Arduino IDE ile kartınıza yükleyin.
4.  **Test Edin:** Mikrofona doğru konuştuğunuzda veya üflediğinizde RGB LED'in sesin ritmine göre renk değiştirdiğini gözlemleyin.
