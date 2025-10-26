1) Proje Gereksinimleri

İki program: server ve client (ayrı executable).

Haberleşme: yalnızca SIGUSR1/SIGUSR2.

Karakterler bit bit gönderilir; mesaj ‘\0’ ile biter.

Server başlarken PID’ini yazar.

Kayıp sinyalleri önlemek için hız kontrolü (kısa bekleme) veya ACK (bonus).

Hatalar: geçersiz PID, kapalı server, boş/çok uzun mesaj vb.

Make hedefleri: all, clean, fclean, re (bonus varsa bonus).

2) İzinli Fonksiyonlar (kampüs PDF’ine göre)

write, malloc, free, getpid, kill, signal/sigaction, sigemptyset, sigaddset, pause, usleep/sleep, exit (opsiyonel: kendi ft_printf)

3) Test Planı (özet)

Basit mesaj (“Hello, world!”)

Boş mesaj (“”)

Uzun mesaj (10k+)

Türkçe/Emoji içeren metin

Hız testi (beklemeyi kademeli azalt)

Geçersiz PID / kapalı server

Arka arkaya iki client

4) Build Hedefleri (açıklama)

all: server ve client derlenir

clean: objeler silinir

fclean: objeler + binary’ler silinir

re: fclean ardından all

bonus (varsa): bonus hedefleri üretir

Sprint 1 — Protokol Kararları (README’ye ekle)

Aşağıdakileri birebir README’ne ekle. Sonra “S1 bitti” yaz, Sprint 2’ye geçelim.

Protokol

Sinyal-bite eşleme: SIGUSR1 → 0, SIGUSR2 → 1

Bit sırası: MSB → LSB (soldan sağa; bit7 … bit0)

Çerçeve: 8 bit = 1 byte; mesaj sonu için \0 byte’ı gönderilir

Gönderim hızı: Her bit’ten sonra kısa bekleme (başlangıç değeri: 400–600 µs arası)

ACK (bonus modu): Mesaj sonu \0 alındığında server, client’a tek bir ACK sinyali yollar; client ACK görmeden bitti saymaz

Çoklu client (basit politika): Server, bir mesaj alırken aktif PID’yi kilitler; başka PID’den gelen bitleri mesaj bitene kadar yok sayar

Hata/Sınır Durumları (davranış sözlüğü)

Geçersiz PID / erişilemeyen server: Client ilk sinyal denemesinde hatayı raporlar, temiz çıkış

Boş mesaj: Yalnızca \0 gönderilir; server uygun bir satır sonu davranışı verir

Aşırı hızlı gönderim: Kayıp sinyallerde client beklemeyi kademeli artırır

Beklenmedik bit akışı: Server mesajı mesaj sonuna kadar byte byte yazmaya devam eder (NACK yoksa sessiz)

UTF-8/çok baytlı karakterler: Bayt dizisi olduğu gibi aktarılır; ek işleme yok

Zamanlama Notu (kalibrasyon)

Başlangıçta 500 µs bekleme ile dene.

Kaybolma yoksa 400 → 300 → 200 µs diye kademeli azalt; ilk kayıpta bir üst güvenli değere dön.

Bonus-ACK açıldığında bit başına beklemeyi 300–400 µs’de tutmak genelde yeterli.

Sprint 2 — Skeleton & API Tasarımı (kodsuz, ama direkt kodlamaya hazır görev listesi)
A) Dosya/klasör yapısı (oluştur)

src/server.c — giriş noktası, sinyal kurulumu, PID yazdırma

src/client.c — argüman kontrolü, mesajı bit-bit gönderme

src/utils.c — küçük yardımcılar (örn. mini_atoi, mini_strlen, putstr_fd, putnbr_fd…)

include/minitalk.h — prototipler, makrolar (SIG eşlemesi, bekleme default’u)

Makefile — hedefler: all, clean, fclean, re (+ bonus varsa)

Not: Handler içinde sadece async-safe çağrılar kullanacağımız için utils fonksiyonları handler dışı yerlerde tüketilecek (örn. PID basma, argüman hatası yazdırma vs.).

B) Server için görevler (uygulama sırası)

PID yazdırma: getpid() ile al, ekrana bas.

sigaction kurulumu: SIGUSR1 ve SIGUSR2 için tek bir handler veya iki ayrı handler.

Global durum (minimal ve async güvenli):

volatile sig_atomic_t bit_count; (0–7)

volatile sig_atomic_t current_byte;

(Opsiyonel) volatile sig_atomic_t active_client_pid;

Handler davranışı (mikro görevler):

Gelen sinyale göre bit ekle (0/1).

bit_count == 8 olduğunda: current_byte’ı tek bayt olarak yaz.

Bayt \0 ise: “mesaj bitti” davranışı (ör. newline); (bonus) ACK gönder.

Sayaç ve geçici baytı resetle.

Main döngü: pause() ile sinyal bekleme; handler işi yaptıktan sonra döngü döner.

Hata çıktı standardı: Geçersiz durumlarda stderr’e laconic mesajlar.

C) Client için görevler (uygulama sırası)

Argüman doğrulama: ./client <pid> "<mesaj>"

PID sayısal mı?

(İzinliyse) kill(pid, 0) ile ulaşılabilirlik kontrolü.

Bit gönderim sözleşmesi:

Bit sırası: MSB→LSB

0 için SIGUSR1, 1 için SIGUSR2

Fonksiyonel parçalara böl (isimler öneri, kodu sen yazacaksın):

send_bit(pid, b) → ilgili sinyali yollar, sonra bekleme

send_byte(pid, ch) → 8 bit sırayla yollar

send_message(pid, s) → her byte + final \0

Bekleme kalibrasyonu: başlangıçta ~500µs; gerekirse azalt.

Hata akışı: kill başarısızsa kullanıcıya net mesaj ve temiz çıkış.

(Bonus) ACK: Mesaj sonu sonrası, server’dan tek sinyal bekleme (ör. pause()), gelince “Teslim edildi” yazdırma.

D) Makro & sabitler (minitalk.h içine)

#define BIT0_SIG SIGUSR1

#define BIT1_SIG SIGUSR2

#define BIT_SLEEP_US 500 (başlangıç değeri)

Prototipler: server handler, client gönderim fonksiyonları, utils yardımcıları.

E) Makefile görevleri (hedef davranışı)

all → server ve client üret

clean → objeleri temizle

fclean → objeler + binary’ler temizle

re → fclean ardından all

(bonus) → bonus sürümleri üret

F) İlk derleme & duman testi (manuel adımlar)

Makefile ile build al.

Terminal 1: server’ı çalıştır → PID’i not et.

Terminal 2: client <PID> "Hello" çalıştır.

Server konsolunda "Hello" ve mesaj bitiş davranışı (ör. newline) görünüyor mu?

Boş mesaj ("") ve uzun mesaj testlerini sırayla dene.

Beklemeyi 500→400→300→200 µs düşürerek kayıp olup olmadığını gözle.
