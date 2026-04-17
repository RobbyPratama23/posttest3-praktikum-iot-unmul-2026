# posttest3-praktikum-iot-unmul-2026

👥 Anggota Kelompok dan Pembagian Tugas
1.Muhammad Ariando Ferdian - 2309106086 (Perancangan rangkaian)
2.Ahmadyani - 2309106055 (Coding ESP32)
3.Muhammad Asadin Nur - 2309106061 (Desain dan implementasi Kodular)
4.Robby Pratama - 2309106113 (Integrasi MQTT dan komunikasi data)

# Sistem Monitoring dan Kontrol Bendungan Pintar Berbasis IoT Menggunakan MQTT dan Kodular
Proyek ini bertujuan untuk membangun sistem miniatur bendungan pintar yang mampu melakukan monitoring ketinggian air serta mengontrol pintu air secara otomatis maupun manual dari jarak jauh.

Sistem ini memanfaatkan:
Sensor ketinggian air untuk membaca level air
Motor servo sebagai penggerak pintu air
Buzzer sebagai alarm peringatan
Protokol MQTT sebagai komunikasi data
Aplikasi Kodular sebagai antarmuka monitoring dan kontrol

Data dari sensor akan dikirim ke MQTT broker, kemudian ditampilkan pada aplikasi Kodular secara real-time. Selain itu, pengguna juga dapat mengontrol sistem melalui mode manual.

📱 Tampilan Kodular
Aplikasi menampilkan:
Nilai ketinggian air
Status level (Aman / Waspada / Bahaya)
Status buzzer
Posisi servo
Tombol mode (Otomatis / Manual)
Kontrol manual aktuator

🧩 Komponen yang Digunakan
- ESP32
- Water Level Sensor
- Motor Servo
- Buzzer
- Kabel jumper
- Breadboard
- Platform MQTT
- Kodular

