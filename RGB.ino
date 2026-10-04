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
