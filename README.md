# UAS_KeamananKomputer

Kelompok

Nama : 

Rizko Fahrezy

09030582327094

Trisya Ramadani

09030582327091

Ravelina Fitria

09030282327058

<img width="200" height="300" alt="image" src="https://github.com/user-attachments/assets/5577f050-a6f3-430a-b9f3-1ca531c39856" />

<img width="200" height="300" alt="image" src="https://github.com/user-attachments/assets/52a3467a-05e7-4570-809f-3bddcf6536ff" />


Judul : DOS Pengaruh serangan terhadap kinerja smart ventilation

Proses Serangan DoS pada ESP32: TCP Port Flooding
Serangan ini mengeksploitasi keterbatasan sumber daya pada microcontroller ESP32. Tujuannya bukan untuk mencuri data, melainkan untuk melumpuhkan layanan kritis (Web Server di Port 80) dan secara efektif mengganggu logika kontrol fisik (sensor DHT dan servo)
Terbuka di jendela baru
Shutterstock.

1. Peran Perangkat
Perangkat	Peran dalam Serangan	Tujuan
ESP32	Korban	Menjalankan dua tugas: Web Server (Port 80) sebagai target dan Logika Kontrol (DHT/Servo) sebagai fungsi yang akan terganggu.
Kali Linux	Penyerang	Menjalankan Skrip Python untuk membuat dan mengirim ribuan permintaan koneksi TCP palsu secara cepat.

Ekspor ke Spreadsheet

Langkah-Langkah Serangan dari Awal
Tahap I: Persiapan dan Pengaturan Jaringan
Langkah 1: Siapkan ESP32 (Korban)
Hubungkan Hardware: Pasang sensor DHT, LED, Fan, dan Servo ke pin ESP32 sesuai dengan definisi dalam kode Arduino.

Unggah Kode Korban: Upload kode Arduino yang sudah dimodifikasi (Web Server + DHT) ke ESP32.

Catat IP Target: Buka Serial Monitor dan tunggu hingga ESP32 berhasil terhubung ke Wi-Fi. Catat Alamat IP Lokal yang ditampilkan (misalnya: 192.168.43.44). Ini adalah TARGET_IP Anda.

Langkah 2: Siapkan Kali Linux (Penyerang)
Pastikan Koneksi: Pastikan Kali Linux terhubung ke jaringan Wi-Fi yang sama dengan ESP32.

Verifikasi Ping: Uji koneksi dasar di Terminal Kali Linux:

Bash

ping [(192.168.43.44/contoh)]
(Ping harus berhasil: (Reply from...)

Edit Skrip Python: Buka file dos_esp32.py dan ganti TARGET_IP dengan IP ESP32 yang sudah Anda catat.

Tahap II: Eksekusi Serangan (The Flood)
Langkah 3: Verifikasi Layanan Normal
Buka browser di Kali Linux.

Akses IP ESP32 (192.168.43.44/contoh).

Hasil: Web Server harus menampilkan status normal (Web Server BERJALAN NORMAL). Amati juga Serial Monitor ESP32; status suhu dan kontrol harus tercetak secara teratur.

Langkah 4: Luncurkan Serangan
Buka Terminal baru di Kali Linux (pertahankan browser terbuka).

Jalankan skrip flooding:

Bash

python3 dos_esp32.py
Langkah 5: Amati Dampak Serangan (The DoS)
Amati perubahan yang terjadi pada tiga titik:

Tempat Observasi	Hasil yang Diharapkan (Saat Serangan Aktif)	Penjelasan
Terminal Kali	Pesan [+] Koneksi TCP berhasil dibuka akan berselang-seling dengan [-] GAGAL/DITOLAK.	Bukti bahwa socket stack ESP32 terus diisi dan dibersihkan secara paksa.
Browser (Web Server)	Akses web ke IP ESP32 akan menjadi sangat lambat, Connection Timed Out, atau Connection Refused.	Ini adalah DoS: Port 80 tidak dapat memproses permintaan yang sah karena kehabisan memori.
Serial Monitor ESP32	Pencetakan Suhu rata-rata: ... akan melambat drastis atau berhenti total.	Dampak Fisik: CPU terlalu sibuk mengelola flood jaringan sehingga mengabaikan fungsi krusial (bacaSuhuRata, myServo.write), menyebabkan logika kontrol (ventilasi, LED) terhenti.

Ekspor ke Spreadsheet

Tahap III: Pemulihan (Recovery)
Langkah 6: Menghentikan Serangan
Kembali ke Terminal Kali Linux yang menjalankan skrip Python.

Tekan Ctrl + C.

Langkah 7: Verifikasi Pemulihan
Skrip Python akan mencetak [*] Cleanup Selesai. ESP32 harusnya bisa pulih.

Amati Serial Monitor: Pencetakan suhu akan kembali normal (teratur setiap ~2 detik).

Akses kembali browser: Web Server harus dapat diakses kembali dan merespons dengan cepat.

Ini menyimpulkan demonstrasi bahwa serangan jaringan DoS dapat menyebabkan kegagalan fungsional pada perangkat IoT.

Kesimpulan Serangan Denial of Service pada Sistem Ventilasi ESP32
Demonstrasi ini secara efektif membuktikan bahwa prototipe sistem IoT berbasis ESP32 sangat rentan terhadap Denial of Service (DoS) melalui serangan TCP Port Flooding di jaringan lokal. Serangan ini tidak bertujuan mencuri data, melainkan mengeksploitasi keterbatasan sumber daya microcontroller . Mekanisme serangan bekerja dengan cara penyerang (Kali Linux) membanjiri target (Web Server pada Port 80) dengan ribuan permintaan koneksi TCP yang sengaja tidak diselesaikan. Dampaknya terlihat jelas di mana Port 80 gagal diakses oleh pengguna sah (Connection Timed Out), karena seluruh stack memori dan buffer CPU ESP32 telah habis dialokasikan untuk menangani koneksi flood tersebut. Konsekuensi paling serius adalah gangguan pada logika kontrol fisik: karena CPU dipaksa mengelola flood jaringan, ia tidak memiliki waktu yang cukup untuk menjalankan fungsi kritis seperti membaca sensor DHT11 dan menggerakkan Servo serta Kipas. Hal ini menyebabkan pencatatan suhu di Serial Monitor melambat drastis dan sistem ventilasi tidak dapat merespons perubahan suhu secara real-time. Kesimpulannya, kegagalan pada lapisan jaringan (keamanan siber) secara langsung dan fatal menyebabkan kegagalan fungsional (keselamatan fisik) pada perangkat IoT, menegaskan perlunya implementasi mekanisme rate limiting atau firewall pada level firmware untuk mitigasi.

